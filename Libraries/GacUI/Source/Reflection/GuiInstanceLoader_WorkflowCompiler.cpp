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

/***********************************************************************
Variable
***********************************************************************/

		void Workflow_CreateVariable(Ptr<workflow::WfModule> module, const WString& name, const description::Value& value)
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
		
		void Workflow_CreateVariablesForReferenceValues(Ptr<workflow::WfModule> module, Ptr<GuiInstanceEnvironment> env)
		{
			FOREACH_INDEXER(WString, name, index, env->scope->referenceValues.Keys())
			{
				auto value = env->scope->referenceValues.Values()[index];
				Workflow_CreateVariable(module, name, value);
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
	}
}