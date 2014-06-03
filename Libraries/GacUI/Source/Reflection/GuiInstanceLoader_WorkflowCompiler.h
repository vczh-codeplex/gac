/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
GacUI Reflection: Instance Schema Representation

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_REFLECTION_GUIINSTANCESCHE_WORKFLOWCOMPILER
#define VCZH_PRESENTATION_REFLECTION_GUIINSTANCESCHE_WORKFLOWCOMPILER

#include "GuiInstanceLoader.h"
#include "..\Resources\GuiParserManager.h"
#include "..\..\..\Workflow\Source\Analyzer\WfAnalyzer.h"

namespace vl
{
	namespace presentation
	{
		extern workflow::analyzer::WfLexicalScopeManager*	Workflow_GetSharedManager();

		extern void											Workflow_CreateVariable(Ptr<workflow::WfModule> module, const WString& name, const description::Value& value);
		extern void											Workflow_CreateVariablesForReferenceValues(Ptr<workflow::WfModule> module, Ptr<GuiInstanceEnvironment> env);
		extern void											Workflow_SetVariablesForReferenceValues(Ptr<workflow::runtime::WfRuntimeGlobalContext> context, Ptr<GuiInstanceEnvironment> env);

		extern bool											Workflow_ValidateExpression(Ptr<GuiInstanceEnvironment> env, IGuiInstanceLoader::PropertyInfo& bindingTarget, const WString& expressionCode, Ptr<workflow::WfExpression>& expression);
		extern Ptr<workflow::runtime::WfAssembly>			Workflow_CompileExpression(Ptr<GuiInstanceEnvironment> env, const WString& expressionCode);
		extern Ptr<workflow::runtime::WfAssembly>			Workflow_CompileEventHandler(Ptr<GuiInstanceEnvironment> env, IGuiInstanceLoader::PropertyInfo& bindingTarget, const WString& statementCode);

		struct WorkflowDataBinding
		{
			description::Value								instance;
			description::IPropertyInfo*						propertyInfo = 0;
			Ptr<workflow::WfExpression>						bindExpression; // WfBindExpression for bind, else for assign
		};

		extern void											Workflow_GetDataBindingContext(Ptr<GuiInstanceEnvironment> env, collections::Dictionary<DescriptableObject*, WString>& valueNames);
		extern void											Workflow_FillDataBindingContext(Ptr<GuiInstanceEnvironment> env, collections::List<WorkflowDataBinding>& dataBindings);
		extern WString										Workflow_ModuleToString(Ptr<workflow::WfModule> module);
		extern Ptr<workflow::runtime::WfAssembly>			Workflow_CompileDataBinding(Ptr<GuiInstanceEnvironment> env, collections::List<WorkflowDataBinding>& dataBindings);
	}
}

#endif