/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Workflow::Analyzer

Interfaces:
**********************************************************************/

#include "..\Expression\WfExpression.h"

namespace vl
{
	namespace workflow
	{
		namespace analyzer
		{

			class WfLexicalSymbol;
			class WfLexicalScope;
			class WfLexicalScopeManager;

/***********************************************************************
Scope
***********************************************************************/

			class WfLexicalSymbol : public Object
			{
			public:
				Ptr<WfType>									type;				// type of this symbol
				Ptr<WfDeclaration>							declaration;		// declaration of this symbol, nullable
				Ptr<reflection::description::ITypeInfo>		typeInfo;			// reflection type info of this symbol, nullable

				WfLexicalSymbol(Ptr<WfDeclaration> declaration);
				~WfLexicalSymbol();
			};

			class WfLexicalScope : public Object
			{
				typedef collections::Dictionary<WString, Ptr<WfLexicalSymbol>>		TypeMap;
			public:
				WfLexicalScopeManager*						ownerManager;		// nullable and inheritable
				Ptr<WfModule>								ownerModule;		// nullable and inheritable
				Ptr<WfDeclaration>							ownerDeclaration;	// nullable and inheritable
				Ptr<WfStatement>							ownerStatement;		// nullable
				Ptr<WfExpression>							ownerExpression;	// nullable

				Ptr<WfLexicalScope>							parentScope;		// null means that this is the root scope
				TypeMap										symbols;			// all symbols in this scope

				WfLexicalScope(WfLexicalScopeManager* _ownerManager);
				WfLexicalScope(Ptr<WfLexicalScope> _parentScope);
				~WfLexicalScope();

				WfLexicalScopeManager*						FindManager();
				Ptr<WfModule>								FindModule();
				Ptr<WfDeclaration>							FindDeclaration();
				Ptr<WfLexicalSymbol>						ResolveSymbol(const WString& symbolName);
			};

/***********************************************************************
Scope Manager
***********************************************************************/

			class WfLexicalScopeName : public Object
			{
				typedef collections::Dictionary<WString, Ptr<WfLexicalScopeName>>		NameMap;
				typedef collections::List<Ptr<WfDeclaration>>							DeclarationList;
			public:
				WfLexicalScopeName*							parent;
				NameMap										children;
				WString										name;
				reflection::description::ITypeDescriptor*	typeDescriptor;		// type that form this name
				DeclarationList								declarations;		// declarations that form this name

				WfLexicalScopeName();
				~WfLexicalScopeName();

				Ptr<WfLexicalScopeName>						AccessChild(const WString& name);
			};

			class WfLexicalScopeManager : public Object
			{
				typedef collections::Dictionary<Ptr<WfNamespaceDeclaration>, Ptr<WfLexicalScopeName>>		NamespaceNameMap;
				typedef collections::Dictionary<Ptr<WfModule>, Ptr<WfLexicalScope>>							ModuleScopeMap;
				typedef collections::Dictionary<Ptr<WfDeclaration>, Ptr<WfLexicalScope>>					DeclarationScopeMap;
				typedef collections::Dictionary<Ptr<WfStatement>, Ptr<WfLexicalScope>>						StatementScopeMap;
				typedef collections::Dictionary<Ptr<WfExpression>, Ptr<WfLexicalScope>>						ExpressionScopeMap;
			protected:

				void										BuildGlobalNameFromTypeDescriptors();
				void										BuildGlobalNameFromModules();
				void										BuildName(Ptr<WfLexicalScopeName> name, Ptr<WfDeclaration> declaration);
			public:
				collections::List<Ptr<WfModule>>			modules;

				Ptr<WfLexicalScopeName>						globalName;
				NamespaceNameMap							namespaceNames;

				ModuleScopeMap								moduleScopes;		// the nearest scope for the module
				DeclarationScopeMap							declarationScopes;	// the nearest scope for the declaration
				StatementScopeMap							statementScopes;	// the nearest scope for the statement
				ExpressionScopeMap							expressionScopes;	// the nearest scope for the expression

				WfLexicalScopeManager();
				~WfLexicalScopeManager();

				void										BuildGlobalName();
				void										BuildScopes();
			};

/***********************************************************************
Helper Functions
***********************************************************************/

			extern Ptr<WfType>								GetTypeFromDeclaration(Ptr<WfDeclaration> declaration);
			extern Ptr<WfType>								GetTypeFromTypeInfo(reflection::description::ITypeInfo* typeInfo);
			extern Ptr<reflection::description::ITypeInfo>	CreateTypeInfoFromType(WfLexicalScope* scope, Ptr<WfType> type);

			extern void										BuildScopeForModule(WfLexicalScopeManager* manager, Ptr<WfModule> module);
			extern void										BuildScopeForDeclaration(WfLexicalScopeManager* manager, Ptr<WfLexicalScope> parentScope, Ptr<WfDeclaration> declaration);
			extern void										BuildScopeForStatement(WfLexicalScopeManager* manager, Ptr<WfLexicalScope> parentScope, Ptr<WfStatement> statement);
			extern void										BuildScopeForExpression(WfLexicalScopeManager* manager, Ptr<WfLexicalScope> parentScope, Ptr<WfExpression> expression);
		}
	}
}