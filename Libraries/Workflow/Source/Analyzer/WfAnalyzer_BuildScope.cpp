#include "WfAnalyzer.h"

namespace vl
{
	namespace workflow
	{
		namespace analyzer
		{
			using namespace collections;
			using namespace reflection;
			using namespace reflection::description;

/***********************************************************************
BuildScopeForModule
***********************************************************************/

			void BuildScopeForModule(WfLexicalScopeManager* manager, Ptr<WfModule> module)
			{
				Ptr<WfLexicalScope> scope = new WfLexicalScope(manager);
				scope->ownerModule = module;
				manager->moduleScopes.Add(module, scope);
			}

/***********************************************************************
BuildScopeForDeclaration
***********************************************************************/

			void BuildScopeForDeclaration(WfLexicalScopeManager* manager, Ptr<WfLexicalScope> parentScope, Ptr<WfDeclaration> declaration)
			{
				throw 0;
			}

/***********************************************************************
BuildScopeForStatement
***********************************************************************/

			void BuildScopeForStatement(WfLexicalScopeManager* manager, Ptr<WfLexicalScope> parentScope, Ptr<WfStatement> statement)
			{
				throw 0;
			}

/***********************************************************************
BuildScopeForExpression
***********************************************************************/

			void BuildScopeForExpression(WfLexicalScopeManager* manager, Ptr<WfLexicalScope> parentScope, Ptr<WfExpression> expression)
			{
				throw 0;
			}
		}
	}
}