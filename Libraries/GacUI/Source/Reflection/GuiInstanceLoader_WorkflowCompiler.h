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
		namespace types
		{
			typedef collections::Dictionary<WString, description::ITypeDescriptor*>		VariableTypeMap;
			typedef collections::Dictionary<WString, IGuiInstanceLoader::TypeInfo>		VariableTypeInfoMap;
			typedef collections::List<WString>											ErrorList;
		}
		extern workflow::analyzer::WfLexicalScopeManager*	Workflow_GetSharedManager();
		

/***********************************************************************
WorkflowCompiler
***********************************************************************/

		extern void											Workflow_CreatePointerVariable(Ptr<workflow::WfModule> module, const WString& name, description::ITypeDescriptor* type);
		extern void											Workflow_GetVariableTypes(Ptr<GuiInstanceEnvironment> env, types::VariableTypeMap& types);
		extern void											Workflow_CreateVariablesForReferenceValues(Ptr<workflow::WfModule> module, types::VariableTypeMap& types);
		extern void											Workflow_SetVariablesForReferenceValues(Ptr<workflow::runtime::WfRuntimeGlobalContext> context, Ptr<GuiInstanceEnvironment> env);

		extern bool											Workflow_ValidateExpression(types::VariableTypeMap& types, types::ErrorList& errors, IGuiInstanceLoader::PropertyInfo& bindingTarget, const WString& expressionCode, Ptr<workflow::WfExpression>& expression);
		extern Ptr<workflow::runtime::WfAssembly>			Workflow_CompileExpression(types::VariableTypeMap& types, types::ErrorList& errors, const WString& expressionCode);
		extern Ptr<workflow::runtime::WfAssembly>			Workflow_CompileEventHandler(types::VariableTypeMap& types, types::ErrorList& errors, IGuiInstanceLoader::PropertyInfo& bindingTarget, const WString& statementCode);

		struct WorkflowDataBinding
		{
			WString											variableName;
			description::IPropertyInfo*						propertyInfo = 0;
			Ptr<workflow::WfExpression>						bindExpression; // WfBindExpression for bind, else for assign
		};

		extern WString										Workflow_ModuleToString(Ptr<workflow::WfModule> module);
		extern Ptr<workflow::runtime::WfAssembly>			Workflow_CompileDataBinding(types::VariableTypeMap& types, description::ITypeDescriptor* thisType, types::ErrorList& errors, collections::List<WorkflowDataBinding>& dataBindings);

		extern void											Workflow_PrecompileInstanceContext(Ptr<GuiInstanceContext> context, types::ErrorList& errors);

/***********************************************************************
GuiWorkflowCache
***********************************************************************/

		class GuiWorkflowCache : public Object, public IGuiInstanceCache
		{
		public:
			static const wchar_t*							CacheTypeName;
			static const wchar_t*							CacheContextName;

			Ptr<workflow::runtime::WfAssembly>				assembly;

			GuiWorkflowCache();
			GuiWorkflowCache(Ptr<workflow::runtime::WfAssembly> _assembly);

			WString											GetCacheTypeName()override;
		};

		class GuiWorkflowCacheResolver : public Object, public IGuiInstanceCacheResolver
		{
		public:
			WString											GetCacheTypeName()override;
			bool											Serialize(Ptr<IGuiInstanceCache> cache, stream::IStream& stream)override;
			Ptr<IGuiInstanceCache>							Deserialize(stream::IStream& stream)override;
		};
	}
}

#endif