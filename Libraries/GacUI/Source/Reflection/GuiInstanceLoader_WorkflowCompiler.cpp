#include "GuiInstanceLoader_WorkflowCompiler.h"
#include "TypeDescriptors\GuiReflectionControls.h"

namespace vl
{
	namespace presentation
	{
		using namespace workflow;
		using namespace workflow::analyzer;
		using namespace workflow::runtime;
		using namespace reflection::description;
		using namespace collections;

#define ERROR_CODE_PREFIX L"================================================================"

/***********************************************************************
Variable
***********************************************************************/

		void Workflow_CreatePointerVariable(Ptr<workflow::WfModule> module, const WString& name, description::ITypeDescriptor* type)
		{
			auto var = MakePtr<WfVariableDeclaration>();
			var->name.value = name;
			{
				Ptr<TypeInfoImpl> elementType = new TypeInfoImpl(ITypeInfo::TypeDescriptor);
				elementType->SetTypeDescriptor(type);

				Ptr<TypeInfoImpl> pointerType = new TypeInfoImpl(ITypeInfo::RawPtr);
				pointerType->SetElementType(elementType);

				var->type = GetTypeFromTypeInfo(pointerType.Obj());
			}

			auto literal = MakePtr<WfLiteralExpression>();
			literal->value = WfLiteralValue::Null;
			var->expression = literal;

			module->declarations.Add(var);
		}

		void Workflow_GetVariableTypes(Ptr<GuiInstanceEnvironment> env, types::VariableTypeMap& types)
		{
			FOREACH_INDEXER(WString, name, index, env->scope->referenceValues.Keys())
			{
				auto value = env->scope->referenceValues.Values()[index];
				types.Add(name, value.GetTypeDescriptor());
			}
		}
		
		void Workflow_CreateVariablesForReferenceValues(Ptr<workflow::WfModule> module, types::VariableTypeMap& types)
		{
			for (vint i = 0; i < types.Count(); i++)
			{
				auto key = types.Keys()[i];
				auto value = types.Values()[i];
				Workflow_CreatePointerVariable(module, key, value);
			}
		}

		void Workflow_SetVariablesForReferenceValues(Ptr<workflow::runtime::WfRuntimeGlobalContext> context, Ptr<GuiInstanceEnvironment> env)
		{
			FOREACH_INDEXER(WString, name, index, env->scope->referenceValues.Keys())
			{
				vint variableIndex = context->assembly->variableNames.IndexOf(name);
				context->globalVariables->variables[variableIndex] = env->scope->referenceValues.Values()[index];
			}
		}

/***********************************************************************
Workflow_ValidateExpression
***********************************************************************/

		bool Workflow_ValidateExpression(types::VariableTypeMap& types, types::ErrorList& errors, IGuiInstanceLoader::PropertyInfo& bindingTarget, const WString& expressionCode, Ptr<workflow::WfExpression>& expression)
		{
			auto parser = GetParserManager()->GetParser<WfExpression>(L"WORKFLOW-EXPRESSION");
			expression = parser->TypedParse(expressionCode, errors);
			if (!expression)
			{
				errors.Add(ERROR_CODE_PREFIX L"Failed to parse the workflow expression.");
				return false;
			}

			bool failed = false;
			auto td = bindingTarget.typeInfo.typeDescriptor;
			auto propertyInfo = td->GetPropertyByName(bindingTarget.propertyName, true);
			if (!propertyInfo)
			{
				errors.Add(ERROR_CODE_PREFIX L"Property \"" + bindingTarget.propertyName + L"\" does not exist in type \"" + td->GetTypeName() + L"\".");
				failed = true;
			}
			else if (!propertyInfo->IsReadable() || !propertyInfo->IsWritable())
			{
				errors.Add(ERROR_CODE_PREFIX L"Property \"" + bindingTarget.propertyName + L"\" of type \"" + td->GetTypeName() + L"\" should be both readable and writable.");
				failed = true;
			}

			auto module = MakePtr<WfModule>();
			Workflow_CreateVariablesForReferenceValues(module, types);
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

			Workflow_GetSharedManager()->Clear(true, true);
			Workflow_GetSharedManager()->modules.Add(module);
			Workflow_GetSharedManager()->Rebuild(true);
			if (Workflow_GetSharedManager()->errors.Count() > 0)
			{
				errors.Add(ERROR_CODE_PREFIX L"Failed to analyze the workflow expression \"" + expressionCode + L"\".");
				FOREACH(Ptr<parsing::ParsingError>, error, Workflow_GetSharedManager()->errors)
				{
					errors.Add(error->errorMessage);
				}
				failed = true;
			}
			else if (propertyInfo)
			{
				auto bind = expression.Cast<WfBindExpression>();
				auto result = Workflow_GetSharedManager()->expressionResolvings[(bind ? bind->expression : expression).Obj()];
				if (result.type)
				{
					ITypeInfo* propertyType = propertyInfo->GetReturn();
					if (propertyInfo->GetSetter() && propertyInfo->GetSetter()->GetParameterCount() == 1)
					{
						propertyType = propertyInfo->GetSetter()->GetParameter(0)->GetType();
					}
					if (!CanConvertToType(result.type.Obj(), propertyType, false))
					{
						errors.Add(ERROR_CODE_PREFIX L"Failed to analyze the workflow expression \"" + expressionCode + L"\".");
						errors.Add(
							WfErrors::ExpressionCannotImplicitlyConvertToType(expression.Obj(), result.type.Obj(), propertyType)
							->errorMessage);
						failed = true;
					}
				}
			}

			return !failed;
		}

/***********************************************************************
Workflow_CompileExpression
***********************************************************************/

		Ptr<workflow::runtime::WfAssembly> Workflow_CompileExpression(types::VariableTypeMap& types, types::ErrorList& errors, const WString& expressionCode)
		{
			auto parser = GetParserManager()->GetParser<WfExpression>(L"WORKFLOW-EXPRESSION");
			auto expression = parser->TypedParse(expressionCode, errors);
			if (!expression)
			{
				errors.Add(ERROR_CODE_PREFIX L"Failed to parse the workflow expression \"" + expressionCode + L"\".");
				return 0;
			}

			auto module = MakePtr<WfModule>();
			Workflow_CreateVariablesForReferenceValues(module, types);
			{
				auto lambda = MakePtr<WfOrderedLambdaExpression>();
				lambda->body = expression;

				auto var = MakePtr<WfVariableDeclaration>();
				var->name.value = L"<initialize-data-binding>";
				var->expression = lambda;

				module->declarations.Add(var);
			}

			Workflow_GetSharedManager()->Clear(true, true);
			Workflow_GetSharedManager()->modules.Add(module);
			Workflow_GetSharedManager()->Rebuild(true);
			if (Workflow_GetSharedManager()->errors.Count() > 0)
			{
				errors.Add(ERROR_CODE_PREFIX L"Failed to analyze the workflow expression \"" + expressionCode + L"\".");
				FOREACH(Ptr<parsing::ParsingError>, error, Workflow_GetSharedManager()->errors)
				{
					errors.Add(error->errorMessage);
				}
				return 0;
			}

			return GenerateAssembly(Workflow_GetSharedManager());
		}

/***********************************************************************
Workflow_CompileEventHandler
***********************************************************************/

		Ptr<workflow::runtime::WfAssembly> Workflow_CompileEventHandler(types::VariableTypeMap& types, types::ErrorList& errors, IGuiInstanceLoader::PropertyInfo& bindingTarget, const WString& statementCode)
		{
			auto parser = GetParserManager()->GetParser<WfStatement>(L"WORKFLOW-STATEMENT");
			auto statement = parser->TypedParse(statementCode, errors);
			if (!statement)
			{
				errors.Add(ERROR_CODE_PREFIX L"Failed to parse the workflow statement.");
				return 0;
			}

			auto module = MakePtr<WfModule>();
			Workflow_CreateVariablesForReferenceValues(module, types);
			{
				auto func = MakePtr<WfFunctionDeclaration>();
				func->anonymity = WfFunctionAnonymity::Named;
				func->name.value = L"<event-handler>";
				func->returnType = GetTypeFromTypeInfo(TypeInfoRetriver<void>::CreateTypeInfo().Obj());

				auto td = bindingTarget.typeInfo.typeDescriptor;
				auto eventInfo = td->GetEventByName(bindingTarget.propertyName, true);
				if (eventInfo)
				{
					vint count = eventInfo->GetHandlerType()->GetElementType()->GetGenericArgumentCount() - 1;
					auto type = TypeInfoRetriver<Value>::CreateTypeInfo();
					for (vint i = 0; i < count; i++)
					{
						auto arg = MakePtr<WfFunctionArgument>();
						arg->name.value = L"<argument>" + itow(i + 1);
						arg->type = GetTypeFromTypeInfo(type.Obj());
						func->arguments.Add(arg);
					}
				}
						
				auto block = MakePtr<WfBlockStatement>();
				block->statements.Add(statement);
				func->statement = block;

				module->declarations.Add(func);
			}

			Workflow_GetSharedManager()->Clear(true, true);
			Workflow_GetSharedManager()->modules.Add(module);
			Workflow_GetSharedManager()->Rebuild(true);
			if (Workflow_GetSharedManager()->errors.Count() > 0)
			{
				errors.Add(ERROR_CODE_PREFIX L"Failed to analyze the workflow statement \"" + statementCode + L"\".");
				FOREACH(Ptr<parsing::ParsingError>, error, Workflow_GetSharedManager()->errors)
				{
					errors.Add(error->errorMessage);
				}
				return 0;
			}

			return GenerateAssembly(Workflow_GetSharedManager());
		}

/***********************************************************************
Workflow_CompileDataBinding
***********************************************************************/

		WString Workflow_ModuleToString(Ptr<workflow::WfModule> module)
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

		Ptr<workflow::runtime::WfAssembly> Workflow_CompileDataBinding(types::VariableTypeMap& types, description::ITypeDescriptor* thisType, types::ErrorList& errors, collections::List<WorkflowDataBinding>& dataBindings)
		{
			auto module = MakePtr<WfModule>();
			Workflow_CreateVariablesForReferenceValues(module, types);
			Workflow_CreatePointerVariable(module, L"<this>", thisType);

			auto func = MakePtr<WfFunctionDeclaration>();
			func->anonymity = WfFunctionAnonymity::Named;
			func->name.value = L"<initialize-data-binding>";
			func->returnType = GetTypeFromTypeInfo(TypeInfoRetriver<void>::CreateTypeInfo().Obj());

			auto block = MakePtr<WfBlockStatement>();
			func->statement = block;
			module->declarations.Add(func);
			

			FOREACH(WorkflowDataBinding, dataBinding, dataBindings)
			{
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
							refSubscribee->name.value = dataBinding.variableName;

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
							refSubscribee->name.value = dataBinding.variableName;

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
				else if (dataBinding.bindExpression)
				{
					auto refSubscribee = MakePtr<WfReferenceExpression>();
					refSubscribee->name.value = dataBinding.variableName;

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

			Workflow_GetSharedManager()->Clear(true, true);
			Workflow_GetSharedManager()->modules.Add(module);
			Workflow_GetSharedManager()->Rebuild(true);
			WString moduleCode = Workflow_ModuleToString(module);

			if (Workflow_GetSharedManager()->errors.Count() > 0)
			{
				errors.Add(ERROR_CODE_PREFIX L"Unexpected errors are encountered when initializing data binding.");
				FOREACH(Ptr<parsing::ParsingError>, error, Workflow_GetSharedManager()->errors)
				{
					errors.Add(error->errorMessage);
				}
				errors.Add(ERROR_CODE_PREFIX L"Print code for reference:");
				errors.Add(moduleCode);
				return 0;
			}
			return GenerateAssembly(Workflow_GetSharedManager());
		}

/***********************************************************************
Workflow_GetSharedManager
***********************************************************************/

		class WorkflowReferenceNamesVisitor : public Object, public GuiValueRepr::IVisitor
		{
		public:
			Ptr<GuiInstanceContext>				context;
			types::VariableTypeMap&				types;
			types::ErrorList&					errors;

			IGuiInstanceLoader::TypeInfo		bindingTargetTypeInfo;
			vint								generatedNameCount;

			WorkflowReferenceNamesVisitor(Ptr<GuiInstanceContext> _context, types::VariableTypeMap& _types, types::ErrorList& _errors)
				:context(_context)
				, types(_types)
				, errors(_errors)
				, generatedNameCount(0)
			{
			}

			void Visit(GuiTextRepr* repr)override
			{
			}

			void Visit(GuiAttSetterRepr* repr)override
			{
				auto reprTypeInfo = bindingTargetTypeInfo;
				auto loader = GetInstanceLoaderManager()->GetLoader(reprTypeInfo.typeName);

				if (repr->instanceName && reprTypeInfo.typeDescriptor)
				{
					WString name = repr->instanceName.Value();
					if (types.Keys().Contains(name))
					{
						errors.Add(L"Precompile: Parameter \"" + name + L"\" conflict with an existing named object.");
					}
					else
					{
						types.Add(name, reprTypeInfo.typeDescriptor);
					}
				}
				
				FOREACH_INDEXER(Ptr<GuiAttSetterRepr::SetterValue>, setter, index, repr->setters.Values())
				{
					IGuiInstanceLoader::TypeInfo propertyTypeInfo;

					if (setter->binding != L"" && setter->binding != L"set")
					{
						auto binder = GetInstanceLoaderManager()->GetInstanceBinder(setter->binding);
						if (!binder)
						{
							errors.Add(L"The appropriate IGuiInstanceBinder of binding \"" + setter->binding + L"\" cannot be found.");
						}
						else if (binder->RequireInstanceName() && !repr->instanceName && reprTypeInfo.typeDescriptor)
						{
							WString name = L"<precompile>" + itow(generatedNameCount++);
							repr->instanceName = name;
							types.Add(name, reprTypeInfo.typeDescriptor);
						}
					}

					if (setter->binding == L"set")
					{
						IGuiInstanceLoader::PropertyInfo info;
						info.typeInfo = reprTypeInfo;
						info.propertyName = repr->setters.Keys()[index];
						auto currentLoader = loader;

						while (currentLoader)
						{
							auto typeInfo = currentLoader->GetPropertyType(info);
							if (typeInfo)
							{
								if (typeInfo->support != typeInfo->NotSupport)
								{
									propertyTypeInfo.typeDescriptor = typeInfo->acceptableTypes[0];
									propertyTypeInfo.typeName = typeInfo->acceptableTypes[0]->GetTypeName();
									break;
								}
							}
							currentLoader = GetInstanceLoaderManager()->GetParentLoader(currentLoader);
						}
					}

					FOREACH(Ptr<GuiValueRepr>, value, setter->values)
					{
						bindingTargetTypeInfo = propertyTypeInfo;
						value->Accept(this);
					}
				}

				FOREACH(Ptr<GuiAttSetterRepr::EventValue>, handler, repr->eventHandlers.Values())
				{
					if (handler->binding != L"")
					{
						auto binder = GetInstanceLoaderManager()->GetInstanceEventBinder(handler->binding);
						if (!binder)
						{
							errors.Add(L"The appropriate IGuiInstanceEventBinder of binding \"" + handler->binding + L"\" cannot be found.");
						}
						else if (binder->RequireInstanceName() && !repr->instanceName && reprTypeInfo.typeDescriptor)
						{
							WString name = L"<precompile>" + itow(generatedNameCount++);
							repr->instanceName = name;
							types.Add(name, reprTypeInfo.typeDescriptor);
						}
					}
				}
			}

			void Visit(GuiConstructorRepr* repr)override
			{
				auto source = FindInstanceLoadingSource(context, repr);
				bindingTargetTypeInfo.typeName = source.typeName;
				bindingTargetTypeInfo.typeDescriptor = GetInstanceLoaderManager()->GetTypeDescriptorForType(source.typeName);
				if (!bindingTargetTypeInfo.typeDescriptor)
				{
					errors.Add(
						L"Precompile: Failed to find type \"" +
						(repr->typeNamespace == L"" 
							? repr->typeName
							: repr->typeNamespace + L":" + repr->typeName
							) +
						L"\".");
				}
				Visit((GuiAttSetterRepr*)repr);
			}
		};

		class WorkflowCompileVisitor : public Object, public GuiValueRepr::IVisitor
		{
		public:
			Ptr<GuiInstanceContext>				context;
			types::VariableTypeMap&				types;
			types::ErrorList&					errors;

			WorkflowCompileVisitor(Ptr<GuiInstanceContext> _context, types::VariableTypeMap& _types, types::ErrorList& _errors)
				:context(_context)
				, types(_types)
				, errors(_errors)
			{
			}

			void Visit(GuiTextRepr* repr)override
			{
			}

			void Visit(GuiAttSetterRepr* repr)override
			{
				FOREACH_INDEXER(Ptr<GuiAttSetterRepr::SetterValue>, setter, index, repr->setters.Values())
				{
					if (setter->binding==L"bind")
					{
					}
					else if (setter->binding == L"format")
					{
					}
					else if (setter->binding == L"eval")
					{
					}

					FOREACH(Ptr<GuiValueRepr>, value, setter->values)
					{
						value->Accept(this);
					}
				}

				FOREACH(Ptr<GuiAttSetterRepr::EventValue>, handler, repr->eventHandlers.Values())
				{
					if (handler->binding == L"eval")
					{
					}

					if (handler->binding != L"")
					{
					}
				}
			}

			void Visit(GuiConstructorRepr* repr)override
			{
				Visit((GuiAttSetterRepr*)repr);
			}
		};

		void Workflow_PrecompileInstanceContext(Ptr<GuiInstanceContext> context, types::ErrorList& errors)
		{
			types::VariableTypeMap types;
			{
				FOREACH(Ptr<GuiInstanceParameter>, parameter, context->parameters)
				{
					auto type = GetTypeDescriptor(parameter->className);
					if (!type)
					{
						errors.Add(L"Precompile: Cannot find type \"" + parameter->className + L"\".");
					}
					else if (types.Keys().Contains(parameter->name))
					{
						errors.Add(L"Precompile: Parameter \"" + parameter->name + L"\" conflict with an existing named object.");
					}
					else
					{
						types.Add(parameter->name, type);
					}
				}

				WorkflowReferenceNamesVisitor visitor(context, types, errors);
				context->instance->Accept(&visitor);
			}
			{
				WorkflowReferenceNamesVisitor visitor(context, types, errors);
				context->instance->Accept(&visitor);
			}
		}

/***********************************************************************
Workflow_GetSharedManager
***********************************************************************/

#undef ERROR_CODE_PREFIX

		class GuiWorkflowSharedManagerPlugin;
		GuiWorkflowSharedManagerPlugin* sharedManagerPlugin = 0;

		class GuiWorkflowSharedManagerPlugin : public Object, public IGuiPlugin
		{
		protected:
			Ptr<WfLexicalScopeManager>		workflowManager;

		public:
			GuiWorkflowSharedManagerPlugin()
			{
			}

			void Load()override
			{
			}

			void AfterLoad()override
			{
				sharedManagerPlugin = this;
			}

			void Unload()override
			{
				sharedManagerPlugin = 0;
			}

			WfLexicalScopeManager* GetWorkflowManager()
			{
				if (!workflowManager)
				{
					workflowManager = new WfLexicalScopeManager(GetParserManager()->GetParsingTable(L"WORKFLOW"));
				}
				return workflowManager.Obj();
			}
		};
		GUI_REGISTER_PLUGIN(GuiWorkflowSharedManagerPlugin)

		WfLexicalScopeManager* Workflow_GetSharedManager()
		{
			return sharedManagerPlugin->GetWorkflowManager();
		}
	}
}