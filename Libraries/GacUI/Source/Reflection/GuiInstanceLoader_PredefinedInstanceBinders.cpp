#include "GuiInstanceLoader.h"
#include "TypeDescriptors\GuiReflectionControls.h"
#include "..\Resources\GuiParserManager.h"
#include "..\..\..\Workflow\Source\Analyzer\WfAnalyzer.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;
		using namespace reflection::description;
		using namespace workflow;
		using namespace workflow::analyzer;
		using namespace workflow::runtime;

/***********************************************************************
GuiTextInstanceBinderBase
***********************************************************************/

		class GuiTextInstanceBinderBase : public Object, public IGuiInstanceBinder
		{
		protected:
			ITypeDescriptor*				stringTypeDescriptor;
		public:
			GuiTextInstanceBinderBase()
				:stringTypeDescriptor(description::GetTypeDescriptor<WString>())
			{
			}

			void GetRequiredContexts(collections::List<WString>& contextNames)override
			{
			}

			void GetExpectedValueTypes(collections::List<description::ITypeDescriptor*>& expectedTypes)override
			{
				expectedTypes.Add(stringTypeDescriptor);
			}
		};

/***********************************************************************
GuiResourceInstanceBinder
***********************************************************************/

		class GuiResourceInstanceBinder : public GuiTextInstanceBinderBase
		{
		public:
			WString GetBindingName()override
			{
				return L"uri";
			}

			bool SetPropertyValue(Ptr<GuiInstanceEnvironment> env, IGuiInstanceLoader* loader, IGuiInstanceLoader::PropertyValue& propertyValue)override
			{
				if (propertyValue.propertyValue.GetValueType() == Value::Text)
				{
					WString protocol, path;
					if (IsResourceUrl(propertyValue.propertyValue.GetText(), protocol, path))
					{
						if(Ptr<DescriptableObject> resource=env->resolver->ResolveResource(protocol, path))
						{
							Value value;
							if(Ptr<GuiTextData> text=resource.Cast<GuiTextData>())
							{
								value=Value::From(text->GetText(), stringTypeDescriptor);
							}
							else if(Ptr<DescriptableObject> obj=resource.Cast<DescriptableObject>())
							{
								value=Value::From(obj);
							}

							if(!value.IsNull())
							{
								IGuiInstanceLoader::PropertyValue newValue = propertyValue;
								newValue.propertyValue = value;
								return loader->SetPropertyValue(newValue);
							}
						}
					}
				}
				return false;
			}
		};

/***********************************************************************
GuiReferenceInstanceBinder
***********************************************************************/

		class GuiReferenceInstanceBinder : public GuiTextInstanceBinderBase
		{
		public:
			WString GetBindingName()override
			{
				return L"ref";
			}

			bool SetPropertyValue(Ptr<GuiInstanceEnvironment> env, IGuiInstanceLoader* loader, IGuiInstanceLoader::PropertyValue& propertyValue)override
			{
				if (propertyValue.propertyValue.GetValueType() == Value::Text)
				{
					WString name = propertyValue.propertyValue.GetText();
					vint index = env->scope->referenceValues.Keys().IndexOf(name);
					if (index != -1)
					{
						IGuiInstanceLoader::PropertyValue newValue = propertyValue;
						newValue.propertyValue = env->scope->referenceValues.Values()[index];
						if (!newValue.propertyValue.IsNull())
						{
							return loader->SetPropertyValue(newValue);
						}
					}
				}
				return false;
			}
		};

/***********************************************************************
GuiWorkflowGlobalContext
***********************************************************************/

		struct WorkflowDataBinding
		{
			Value						instance;
			IPropertyInfo*				propertyInfo;
			Ptr<WfBindExpression>		bindExpression;
		};

		class GuiWorkflowGlobalContext : public Object, public IGuiInstanceBindingContext
		{
		public:
			List<WorkflowDataBinding>		dataBindings;
			Ptr<WfRuntimeGlobalContext>		globalContext;

			WString GetContextName()override
			{
				return L"WORKFLOW-GLOBAL-CONTEXT";
			}

			void Initialize(Ptr<GuiInstanceEnvironment> env)override
			{
				auto module = MakePtr<WfModule>();
				CreateVariablesForReferenceValues(module, env);
				CreateVariable(module, L"<this>", env->scope->rootInstance);
				{
					auto func = MakePtr<WfFunctionDeclaration>();
					func->anonymity = WfFunctionAnonymity::Named;
					func->name.value = L"<initialize-data-binding>";
					func->returnType = GetTypeFromTypeInfo(TypeInfoRetriver<void>::CreateTypeInfo().Obj());

					auto block = MakePtr<WfBlockStatement>();
					func->statement = block;
					module->declarations.Add(func);

					Dictionary<DescriptableObject*, WString> valueNames;
					FOREACH_INDEXER(WString, name, index, env->scope->referenceValues.Keys())
					{
						auto value = env->scope->referenceValues.Values()[index];
						valueNames.Add(value.GetRawPtr(), name);
					}
					FOREACH(WorkflowDataBinding, dataBinding, dataBindings)
					{
						WString subscribee = valueNames[dataBinding.instance.GetRawPtr()];
						auto subBlock = MakePtr<WfBlockStatement>();
						block->statements.Add(subBlock);
						{
							auto refThis = MakePtr<WfReferenceExpression>();
							refThis->name.value = L"<this>";

							auto member = MakePtr<WfMemberExpression>();
							member->parent = refThis;
							member->name.value = L"AddSubscription";

							auto call = MakePtr<WfCallExpression>();
							call->function = member;
							call->arguments.Add(dataBinding.bindExpression);

							auto var = MakePtr<WfVariableDeclaration>();
							var->name.value = L"<subscription>";
							var->expression = call;

							auto stat = MakePtr<WfVariableStatement>();
							stat->variable = var;
							subBlock->statements.Add(stat);
						}
						{
							auto callback = MakePtr<WfFunctionDeclaration>();
							callback->anonymity = WfFunctionAnonymity::Anonymous;
							callback->returnType = GetTypeFromTypeInfo(TypeInfoRetriver<void>::CreateTypeInfo().Obj());;
							{
								auto arg = MakePtr<WfFunctionArgument>();
								arg->name.value = L"<value>";
								arg->type = GetTypeFromTypeInfo(TypeInfoRetriver<Value>::CreateTypeInfo().Obj());
								callback->arguments.Add(arg);
							}
							auto callbackBlock = MakePtr<WfBlockStatement>();
							callback->statement = callbackBlock;
							{
								auto refSubscribee = MakePtr<WfReferenceExpression>();
								refSubscribee->name.value = subscribee;

								auto member = MakePtr<WfMemberExpression>();
								member->parent = refSubscribee;
								member->name.value = dataBinding.propertyInfo->GetName();

								auto var = MakePtr<WfVariableDeclaration>();
								var->name.value = L"<old>";
								var->expression = member;

								auto stat = MakePtr<WfVariableStatement>();
								stat->variable = var;
								callbackBlock->statements.Add(stat);
							}
							{
								ITypeInfo* propertyType = dataBinding.propertyInfo->GetReturn();
								if (dataBinding.propertyInfo->GetSetter() && dataBinding.propertyInfo->GetSetter()->GetParameterCount() == 1)
								{
									propertyType = dataBinding.propertyInfo->GetSetter()->GetParameter(0)->GetType();
								}

								auto refValue = MakePtr<WfReferenceExpression>();
								refValue->name.value = L"<value>";

								auto cast = MakePtr<WfTypeCastingExpression>();
								cast->strategy = WfTypeCastingStrategy::Strong;
								cast->expression = refValue;
								cast->type = GetTypeFromTypeInfo(propertyType);

								auto var = MakePtr<WfVariableDeclaration>();
								var->name.value = L"<new>";
								var->expression = cast;

								auto stat = MakePtr<WfVariableStatement>();
								stat->variable = var;
								callbackBlock->statements.Add(stat);
							}
							{
								auto refOld = MakePtr<WfReferenceExpression>();
								refOld->name.value = L"<old>";

								auto refNew = MakePtr<WfReferenceExpression>();
								refNew->name.value = L"<new>";

								auto compare = MakePtr<WfBinaryExpression>();
								compare->op = WfBinaryOperator::EQ;
								compare->first = refOld;
								compare->second = refNew;

								auto ifStat = MakePtr<WfIfStatement>();
								ifStat->expression = compare;
								callbackBlock->statements.Add(ifStat);

								auto ifBlock = MakePtr<WfBlockStatement>();
								ifStat->trueBranch = ifBlock;

								auto returnStat = MakePtr<WfReturnStatement>();
								ifBlock->statements.Add(returnStat);
							}
							{
								auto refSubscribee = MakePtr<WfReferenceExpression>();
								refSubscribee->name.value = subscribee;

								auto member = MakePtr<WfMemberExpression>();
								member->parent = refSubscribee;
								member->name.value = dataBinding.propertyInfo->GetName();

								auto refNew = MakePtr<WfReferenceExpression>();
								refNew->name.value = L"<new>";

								auto assign = MakePtr<WfBinaryExpression>();
								assign->op = WfBinaryOperator::Assign;
								assign->first = member;
								assign->second = refNew;

								auto stat = MakePtr<WfExpressionStatement>();
								stat->expression = assign;
								callbackBlock->statements.Add(stat);
							}

							auto funcExpr = MakePtr<WfFunctionExpression>();
							funcExpr->function = callback;

							auto refThis = MakePtr<WfReferenceExpression>();
							refThis->name.value = L"<subscription>";

							auto member = MakePtr<WfMemberExpression>();
							member->parent = refThis;
							member->name.value = L"Subscribe";

							auto call = MakePtr<WfCallExpression>();
							call->function = member;
							call->arguments.Add(funcExpr);

							auto stat = MakePtr<WfExpressionStatement>();
							stat->expression = call;
							subBlock->statements.Add(stat);
						}
						{
							auto refThis = MakePtr<WfReferenceExpression>();
							refThis->name.value = L"<subscription>";

							auto member = MakePtr<WfMemberExpression>();
							member->parent = refThis;
							member->name.value = L"Update";

							auto call = MakePtr<WfCallExpression>();
							call->function = member;

							auto stat = MakePtr<WfExpressionStatement>();
							stat->expression = call;
							subBlock->statements.Add(stat);
						}
					}
				}

				WString moduleCode;
				{
					stream::MemoryStream stream;
					{
						stream::StreamWriter writer(stream);
						WfPrint(module, L"", writer);
					}
					stream.SeekFromBegin(0);
					stream::StreamReader reader(stream);
					moduleCode = reader.ReadToEnd();
				}
				
				WfLexicalScopeManager manager(0);
				manager.modules.Add(module);
				manager.Rebuild(false);

				env->scope->errors.Add(L"Print code for reference:");
				env->scope->errors.Add(moduleCode);
				if (manager.errors.Count() > 0)
				{
					env->scope->errors.Add(L"Unexpected errors are encountered when initializing data binding.");
					FOREACH(Ptr<parsing::ParsingError>, error, manager.errors)
					{
						env->scope->errors.Add(error->errorMessage);
					}
					return;
				}

				auto assembly = GenerateAssembly(&manager);
				globalContext = new WfRuntimeGlobalContext(assembly);
				
				LoadFunction<void()>(globalContext, L"<initialize>")();
				FOREACH(WString, name, env->scope->referenceValues.Keys())
				{
					vint index = assembly->variableNames.IndexOf(name);
					globalContext->globalVariables->variables[index] = env->scope->referenceValues.Values()[index];
				}
				{
					vint index = assembly->variableNames.IndexOf(L"<this>");
					globalContext->globalVariables->variables[index] = env->scope->rootInstance;
				}
				LoadFunction<void()>(globalContext, L"<initialize-data-binding>")();
			}

			static void CreateVariable(Ptr<WfModule> module, const WString& name, const Value& value)
			{
				auto var = MakePtr<WfVariableDeclaration>();
				var->name.value = name;
				{
					Ptr<TypeInfoImpl> elementType = new TypeInfoImpl(ITypeInfo::TypeDescriptor);
					elementType->SetTypeDescriptor(value.GetTypeDescriptor());

					Ptr<TypeInfoImpl> pointerType = new TypeInfoImpl(ITypeInfo::RawPtr);
					pointerType->SetElementType(elementType);

					var->type = GetTypeFromTypeInfo(pointerType.Obj());
				}

				auto literal = MakePtr<WfLiteralExpression>();
				literal->value = WfLiteralValue::Null;
				var->expression = literal;

				module->declarations.Add(var);
			}

			static void CreateVariablesForReferenceValues(Ptr<WfModule> module, Ptr<GuiInstanceEnvironment> env)
			{
				FOREACH_INDEXER(WString, name, index, env->scope->referenceValues.Keys())
				{
					auto value = env->scope->referenceValues.Values()[index];
					CreateVariable(module, name, value);
				}
			}
		};

/***********************************************************************
GuiScriptInstanceBinder
***********************************************************************/

		class GuiScriptInstanceBinder : public GuiTextInstanceBinderBase
		{
		public:
			virtual WString TranslateExpression(const WString& input) = 0;

			void GetRequiredContexts(collections::List<WString>& contextNames)override
			{
				contextNames.Add(L"WORKFLOW-GLOBAL-CONTEXT");
			}

			bool SetPropertyValue(Ptr<GuiInstanceEnvironment> env, IGuiInstanceLoader* loader, IGuiInstanceLoader::PropertyValue& propertyValue)override
			{
				if (propertyValue.propertyValue.GetValueType() == Value::Text)
				{
					WString expressionCode = TranslateExpression(propertyValue.propertyValue.GetText());
					auto parser = GetParserManager()->GetParser<WfExpression>(L"WORKFLOW-EXPRESSION");
					auto expression = parser->TypedParse(expressionCode).Cast<WfBindExpression>();
					if (!expression)
					{
						env->scope->errors.Add(L"Failed to parse the workflow expression \"" + expressionCode + L"\".");
						return false;
					}

					bool failed = false;
					auto td = propertyValue.instanceValue.GetTypeDescriptor();
					auto propertyInfo = td->GetPropertyByName(propertyValue.propertyName, true);
					if (!propertyInfo)
					{
						env->scope->errors.Add(L"Property \"" + propertyValue.propertyName + L"\" does not exist in type \"" + td->GetTypeName() + L"\".");
						failed = true;
					}
					else if (!propertyInfo->IsReadable() || !propertyInfo->IsWritable())
					{
						env->scope->errors.Add(L"Property \"" + propertyValue.propertyName + L"\" of type \"" + td->GetTypeName() + L"\" should be both readable and writable.");
						failed = true;
					}

					{
						auto module = MakePtr<WfModule>();
						GuiWorkflowGlobalContext::CreateVariablesForReferenceValues(module, env);
						{
							auto func = MakePtr<WfFunctionDeclaration>();
							func->anonymity = WfFunctionAnonymity::Named;
							func->name.value = L"<initialize-data-binding>";
							func->returnType = GetTypeFromTypeInfo(TypeInfoRetriver<void>::CreateTypeInfo().Obj());

							auto stat = MakePtr<WfExpressionStatement>();
							stat->expression = expression;
							func->statement = stat;

							module->declarations.Add(func);
						}

						WfLexicalScopeManager manager(0);
						manager.modules.Add(module);
						manager.Rebuild(false);
						if (manager.errors.Count() > 0)
						{
							env->scope->errors.Add(L"Failed to analyze the workflow expression \"" + expressionCode + L"\".");
							FOREACH(Ptr<parsing::ParsingError>, error, manager.errors)
							{
								env->scope->errors.Add(error->errorMessage);
							}
							failed = true;
						}
						else if (propertyInfo)
						{
							auto result = manager.expressionResolvings[expression->expression.Obj()];
							if (result.type)
							{
								ITypeInfo* propertyType = propertyInfo->GetReturn();
								if (propertyInfo->GetSetter() && propertyInfo->GetSetter()->GetParameterCount() == 1)
								{
									propertyType = propertyInfo->GetSetter()->GetParameter(0)->GetType();
								}
								if (!CanConvertToType(result.type.Obj(), propertyType, false))
								{
									env->scope->errors.Add(L"Failed to analyze the workflow expression \"" + expressionCode + L"\".");
									env->scope->errors.Add(
										WfErrors::ExpressionCannotImplicitlyConvertToType(expression->expression.Obj(), result.type.Obj(), propertyType)
										->errorMessage);
									failed = true;
								}
							}
						}
					}

					if (!failed)
					{
						expression->expandedExpression = 0;

						WorkflowDataBinding dataBinding;
						dataBinding.instance = propertyValue.instanceValue;
						dataBinding.propertyInfo = propertyInfo;
						dataBinding.bindExpression = expression;

						auto context = env->scope->bindingContexts[L"WORKFLOW-GLOBAL-CONTEXT"].Cast<GuiWorkflowGlobalContext>();
						context->dataBindings.Add(dataBinding);
					}
					return !failed;
				}
				return false;
			}
		};

/***********************************************************************
GuiBindInstanceBinder
***********************************************************************/

		class GuiBindInstanceBinder : public GuiScriptInstanceBinder
		{
		public:
			WString GetBindingName()override
			{
				return L"bind";
			}

			WString TranslateExpression(const WString& input)override
			{
				return L"bind(" + input + L")";
			}
		};

/***********************************************************************
GuiFormatInstanceBinder
***********************************************************************/

		class GuiFormatInstanceBinder : public GuiScriptInstanceBinder
		{
		public:
			WString GetBindingName()override
			{
				return L"format";
			}

			WString TranslateExpression(const WString& input)override
			{
				return L"bind($\"" + input + L"\")";
			}
		};

/***********************************************************************
GuiPredefinedInstanceBindersPlugin
***********************************************************************/

		class GuiPredefinedInstanceBindersPlugin : public Object, public IGuiPlugin
		{
		public:
			void Load()override
			{
				WfLoadTypes();
			}

			void AfterLoad()override
			{
				{
					IGuiParserManager* manager = GetParserManager();
					manager->SetParsingTable(L"WORKFLOW", &WfLoadTable);
					manager->SetTableParser(L"WORKFLOW", L"WORKFLOW-EXPRESSION", &WfParseExpression);
					manager->SetTableParser(L"WORKFLOW", L"WORKFLOW-MODULE", &WfParseModule);
				}
				{
					IGuiInstanceLoaderManager* manager=GetInstanceLoaderManager();

					manager->AddInstanceBindingContextFactory(new GuiInstanceBindingContextFactory<GuiWorkflowGlobalContext>(L"WORKFLOW-GLOBAL-CONTEXT"));

					manager->AddInstanceBinder(new GuiResourceInstanceBinder);
					manager->AddInstanceBinder(new GuiReferenceInstanceBinder);
					manager->AddInstanceBinder(new GuiBindInstanceBinder);
					manager->AddInstanceBinder(new GuiFormatInstanceBinder);
				}
			}

			void Unload()override
			{
			}
		};
		GUI_REGISTER_PLUGIN(GuiPredefinedInstanceBindersPlugin)
	}
}