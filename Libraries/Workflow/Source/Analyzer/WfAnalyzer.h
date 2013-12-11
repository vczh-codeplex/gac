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
Symbol
***********************************************************************/

			class WfLexicalSymbol : public Object
			{
			public:
				Ptr<WfType>									type;				// type of this symbol
				Ptr<WfDeclaration>							declaration;		// declaration of this symbol, nullable
				Ptr<reflection::description::ITypeInfo>		typeInfo;			// reflection type info of this symbol, nullable

				WfLexicalSymbol(Ptr<WfDeclaration> declaration);
				WfLexicalSymbol(Ptr<reflection::description::ITypeInfo> _typeInfo);
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

			class WfLexicalScopeManager : public Object
			{
			public:
				collections::List<Ptr<WfModule>>			modules;

				WfLexicalScopeManager();
				~WfLexicalScopeManager();
			};

/***********************************************************************
Helper Functions
***********************************************************************/

			extern Ptr<WfType>								GetTypeFromDeclaration(Ptr<WfDeclaration> declaration);
			extern Ptr<WfType>								GetTypeFromTypeInfo(reflection::description::ITypeInfo* typeInfo);
			extern Ptr<reflection::description::ITypeInfo>	CreateTypeInfoFromType(WfLexicalScope* scope, Ptr<WfType> type);
		}
	}
}