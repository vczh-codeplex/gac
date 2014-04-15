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

			bool ApplicableToConstructorArgument()override
			{
				return false;
			}

			void GetRequiredContexts(collections::List<WString>& contextNames)override
			{
			}

			void GetExpectedValueTypes(collections::List<description::ITypeDescriptor*>& expectedTypes)override
			{
				expectedTypes.Add(stringTypeDescriptor);
			}

			description::Value GetValue(Ptr<GuiInstanceEnvironment> env, const description::Value& propertyValue)override
			{
				return Value();
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

#define ERROR_CODE_PREFIX L"========<" + env->scope->rootInstance.GetTypeDescriptor()->GetTypeName() + L">======== "

		WString WorkflowModuleToString(Ptr<WfModule> module)
		{
			stream::MemoryStream stream;
			{
				stream::StreamWriter writer(stream);
				WfPrint(module, L"", writer);
			}
			stream.SeekFromBegin(0);
			stream::StreamReader reader(stream);
			return reader.ReadToEnd();
		}

		struct WorkflowDataBinding
		{
			Value						instance;
			IPropertyInfo*				propertyInfo;
			Ptr<WfExpression>			bindExpression; // WfBindExpression for bind, else for assign
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
						vint index = valueNames.Keys().IndexOf(dataBinding.instance.GetRawPtr());
						WString subscribee;
						if (index == -1)
						{
							subscribee = L"<temp>" + itow(valueNames.Count());
							valueNames.Add(dataBinding.instance.GetRawPtr(), subscribee);
							env->scope->referenceValues.Add(subscribee, dataBinding.instance);
							CreateVariable(module, subscribee, dataBinding.instance);
						}
						else
						{
							subscribee = valueNames.Values()[index];
						}

						if (dataBinding.bindExpression.Cast<WfBindExpression>())
						{
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
						else
						{
							auto refSubscribee = MakePtr<WfReferenceExpression>();
							refSubscribee->name.value = subscribee;

							auto member = MakePtr<WfMemberExpression>();
							member->parent = refSubscribee;
							member->name.value = dataBinding.propertyInfo->GetName();

							auto assign = MakePtr<WfBinaryExpression>();
							assign->op = WfBinaryOperator::Assign;
							assign->first = member;
							assign->second = dataBinding.bindExpression;

							auto stat = MakePtr<WfExpressionStatement>();
							stat->expression = assign;
							block->statements.Add(stat);
						}
					}
				}

				WString moduleCode = WorkflowModuleToString(module);
				WfLexicalScopeManager manager(0);
				manager.modules.Add(module);
				manager.Rebuild(false);

				if (manager.errors.Count() > 0)
				{
					env->scope->errors.Add(ERROR_CODE_PREFIX L"Unexpected errors are encountered when initializing data binding.");
					FOREACH(Ptr<parsing::ParsingError>, error, manager.errors)
					{
						env->scope->errors.Add(error->errorMessage);
					}
					env->scope->errors.Add(ERROR_CODE_PREFIX L"Print code for reference:");
					env->scope->errors.Add(moduleCode);
					return;
				}
#ifdef _DEBUG
				else
				{
					// for debug only, always show code in error
					env->scope->errors.Add(ERROR_CODE_PREFIX L"Print code for reference (debug mode only):");
					env->scope->errors.Add(moduleCode);
				}
#endif

				auto assembly = GenerateAssembly(&manager);
				globalContext = new WfRuntimeGlobalContext(assembly);
				
				LoadFunction<void()>(globalContext, L"<initialize>")();
				SetVariablesForReferenceValues(globalContext, env);
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

			static void SetVariablesForReferenceValues(Ptr<WfRuntimeGlobalContext> context, Ptr<GuiInstanceEnvironment> env)
			{
				FOREACH_INDEXER(WString, name, index, env->scope->referenceValues.Keys())
				{
					vint variableIndex = context->assembly->variableNames.IndexOf(name);
					context->globalVariables->variables[variableIndex] = env->scope->referenceValues.Values()[index];
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
					auto expression = parser->TypedParse(expressionCode, env->scope->errors);
					if (!expression)
					{
						env->scope->errors.Add(ERROR_CODE_PREFIX L"Failed to parse the workflow expression \"" + expressionCode + L"\".");
						return false;
					}

					bool failed = false;
					auto td = propertyValue.instanceValue.GetTypeDescriptor();
					auto propertyInfo = td->GetPropertyByName(propertyValue.propertyName, true);
					if (!propertyInfo)
					{
						env->scope->errors.Add(ERROR_CODE_PREFIX L"Property \"" + propertyValue.propertyName + L"\" does not exist in type \"" + td->GetTypeName() + L"\".");
						failed = true;
					}
					else if (!propertyInfo->IsReadable() || !propertyInfo->IsWritable())
					{
						env->scope->errors.Add(ERROR_CODE_PREFIX L"Property \"" + propertyValue.propertyName + L"\" of type \"" + td->GetTypeName() + L"\" should be both readable and writable.");
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

						WString moduleCode = WorkflowModuleToString(module);
						WfLexicalScopeManager manager(0);
						manager.modules.Add(module);
						manager.Rebuild(false);
						if (manager.errors.Count() > 0)
						{
							env->scope->errors.Add(ERROR_CODE_PREFIX L"Failed to analyze the workflow expression \"" + expressionCode + L"\".");
							FOREACH(Ptr<parsing::ParsingError>, error, manager.errors)
							{
								env->scope->errors.Add(error->errorMessage);
							}
							failed = true;
						}
						else if (propertyInfo)
						{
							auto bind = expression.Cast<WfBindExpression>();
							auto result = manager.expressionResolvings[(bind ? bind->expression : expression).Obj()];
							if (result.type)
							{
								ITypeInfo* propertyType = propertyInfo->GetReturn();
								if (propertyInfo->GetSetter() && propertyInfo->GetSetter()->GetParameterCount() == 1)
								{
									propertyType = propertyInfo->GetSetter()->GetParameter(0)->GetType();
								}
								if (!CanConvertToType(result.type.Obj(), propertyType, false))
								{
									env->scope->errors.Add(ERROR_CODE_PREFIX L"Failed to analyze the workflow expression \"" + expressionCode + L"\".");
									env->scope->errors.Add(
										WfErrors::ExpressionCannotImplicitlyConvertToType(expression.Obj(), result.type.Obj(), propertyType)
										->errorMessage);
									failed = true;
								}
							}
						}
					}

					if (!failed)
					{
						{
							auto expr = expression;
							if (auto bind = expr.Cast<WfBindExpression>())
							{
								bind->expandedExpression = 0;
								expr = bind->expression;
							}
							if (auto format = expr.Cast<WfFormatExpression>())
							{
								format->expandedExpression = 0;
							}
						}
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
GuiEvalInstanceBinder
***********************************************************************/

		class GuiEvalInstanceBinder : public GuiScriptInstanceBinder
		{
		public:
			WString GetBindingName()override
			{
				return L"eval";
			}

			bool ApplicableToConstructorArgument()override
			{
				return true;
			}

			description::Value GetValue(Ptr<GuiInstanceEnvironment> env, const description::Value& propertyValue)override
			{
				if (propertyValue.GetValueType() == Value::Text)
				{
					WString expressionCode = TranslateExpression(propertyValue.GetText());
					auto parser = GetParserManager()->GetParser<WfExpression>(L"WORKFLOW-EXPRESSION");
					auto expression = parser->TypedParse(expressionCode, env->scope->errors);
					if (!expression)
					{
						env->scope->errors.Add(ERROR_CODE_PREFIX L"Failed to parse the workflow expression \"" + expressionCode + L"\".");
						return Value();
					}

					auto module = MakePtr<WfModule>();
					GuiWorkflowGlobalContext::CreateVariablesForReferenceValues(module, env);
					{
						auto lambda = MakePtr<WfOrderedLambdaExpression>();
						lambda->body = expression;

						auto var = MakePtr<WfVariableDeclaration>();
						var->name.value = L"<initialize-data-binding>";
						var->expression = lambda;

						module->declarations.Add(var);
					}

					WString moduleCode = WorkflowModuleToString(module);
					WfLexicalScopeManager manager(0);
					manager.modules.Add(module);
					manager.Rebuild(false);
					if (manager.errors.Count() > 0)
					{
						env->scope->errors.Add(ERROR_CODE_PREFIX L"Failed to analyze the workflow expression \"" + expressionCode + L"\".");
						FOREACH(Ptr<parsing::ParsingError>, error, manager.errors)
						{
							env->scope->errors.Add(error->errorMessage);
						}
						return Value();
					}
#ifdef _DEBUG
					else
					{
						// for debug only, always show code in error
						env->scope->errors.Add(ERROR_CODE_PREFIX L"Print code for reference (debug mode only):");
						env->scope->errors.Add(moduleCode);
					}
#endif

					auto assembly = GenerateAssembly(&manager);
					auto globalContext = MakePtr<WfRuntimeGlobalContext>(assembly);
				
					LoadFunction<void()>(globalContext, L"<initialize>")();
					GuiWorkflowGlobalContext::SetVariablesForReferenceValues(globalContext, env);
					vint variableIndex = assembly->variableNames.IndexOf(L"<initialize-data-binding>");
					auto variable = globalContext->globalVariables->variables[variableIndex];
					auto proxy = UnboxValue<Ptr<IValueFunctionProxy>>(variable);
					auto translated = proxy->Invoke(IValueList::Create());

					// the global context contains a closure variable <initialize-data-binding> which captured the context
					// clear all variables to break the circle references
					globalContext->globalVariables = 0;
					return translated;
				}
				return Value();
			}

			WString TranslateExpression(const WString& input)override
			{
				return input;
			}
		};

#undef ERROR_CODE_PREFIX

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
					manager->AddInstanceBinder(new GuiEvalInstanceBinder);
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