/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
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
				WString										name;				// name of this symbol
				Ptr<WfType>									type;				// type of this symbol
				Ptr<reflection::description::ITypeInfo>		typeInfo;			// reflection type info of this symbol, nullable
				Ptr<WfDeclaration>							ownerDeclaration;	// nullable
				Ptr<WfStatement>							ownerStatement;		// nullable
				Ptr<WfExpression>							ownerExpression;	// nullable
				WfLexicalScope*								ownerScope;			// scope that contains this symbol

				WfLexicalSymbol(WfLexicalScope* _ownerScope);
				~WfLexicalSymbol();
				
				WString										GetFriendlyName();
			};

			class WfLexicalScope : public Object
			{
				typedef collections::Group<WString, Ptr<WfLexicalSymbol>>		TypeGroup;
			public:
				WfLexicalScopeManager*						ownerManager;		// nullable and inheritable
				Ptr<WfModule>								ownerModule;		// nullable and inheritable
				Ptr<WfDeclaration>							ownerDeclaration;	// nullable and inheritable
				Ptr<WfStatement>							ownerStatement;		// nullable
				Ptr<WfExpression>							ownerExpression;	// nullable

				Ptr<WfLexicalScope>							parentScope;		// null means that this is the root scope
				TypeGroup									symbols;			// all symbols in this scope

				WfLexicalScope(WfLexicalScopeManager* _ownerManager);
				WfLexicalScope(Ptr<WfLexicalScope> _parentScope);
				~WfLexicalScope();

				WfLexicalScopeManager*						FindManager();
				Ptr<WfModule>								FindModule();
				Ptr<WfDeclaration>							FindDeclaration();
				WString										GetFriendlyName();
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
				bool										createdByTypeDescriptor;
				NameMap										children;
				WString										name;
				reflection::description::ITypeDescriptor*	typeDescriptor;		// type that form this name
				DeclarationList								declarations;		// declarations that form this name

				WfLexicalScopeName(bool _createdByTypeDescriptor);
				~WfLexicalScopeName();

				Ptr<WfLexicalScopeName>						AccessChild(const WString& name, bool createdByTypeDescriptor);
				void										RemoveNonTypeDescriptorNames();
				WString										GetFriendlyName();
			};

			class WfLexicalScopeManager : public Object
			{
				typedef collections::List<Ptr<WfModule>>													ModuleList;
				typedef collections::List<Ptr<parsing::ParsingError>>										ParsingErrorList;
				typedef collections::Dictionary<Ptr<WfNamespaceDeclaration>, Ptr<WfLexicalScopeName>>		NamespaceNameMap;
				typedef collections::Dictionary<Ptr<WfModule>, Ptr<WfLexicalScope>>							ModuleScopeMap;
				typedef collections::Dictionary<Ptr<WfDeclaration>, Ptr<WfLexicalScope>>					DeclarationScopeMap;
				typedef collections::Dictionary<Ptr<WfStatement>, Ptr<WfLexicalScope>>						StatementScopeMap;
				typedef collections::Dictionary<Ptr<WfExpression>, Ptr<WfLexicalScope>>						ExpressionScopeMap;
			protected:

				void										BuildGlobalNameFromTypeDescriptors();
				void										BuildGlobalNameFromModules();
				void										BuildName(Ptr<WfLexicalScopeName> name, Ptr<WfDeclaration> declaration);
				void										ValidateScopeName(Ptr<WfLexicalScopeName> name);
			public:
				Ptr<parsing::tabling::ParsingTable>			parsingTable;
				ModuleList									modules;
				ParsingErrorList							errors;

				Ptr<WfLexicalScopeName>						globalName;
				NamespaceNameMap							namespaceNames;

				ModuleScopeMap								moduleScopes;		// the nearest scope for the module
				DeclarationScopeMap							declarationScopes;	// the nearest scope for the declaration
				StatementScopeMap							statementScopes;	// the nearest scope for the statement
				ExpressionScopeMap							expressionScopes;	// the nearest scope for the expression

				WfLexicalScopeManager(Ptr<parsing::tabling::ParsingTable> _parsingTable);
				~WfLexicalScopeManager();
				
				Ptr<WfModule>								AddModule(const WString& moduleCode, vint codeIndex = -1);
				void										Clear(bool keepTypeDescriptorNames, bool deleteModules);
				void										Rebuild(bool keepTypeDescriptorNames);
				void										ResolveSymbol(WfLexicalScope* scope, const WString& symbolName, collections::List<Ptr<WfLexicalSymbol>>& symbols);
				void										ResolveScopeName(WfLexicalScope* scope, const WString& symbolName, collections::List<Ptr<WfLexicalScopeName>>& names);
			};

/***********************************************************************
Helper Functions
***********************************************************************/

			extern Ptr<WfType>								GetTypeFromTypeInfo(reflection::description::ITypeInfo* typeInfo);
			extern Ptr<WfLexicalScopeName>					GetScopeNameFromReferenceType(WfLexicalScope* scope, Ptr<WfType> type);
			extern Ptr<reflection::description::ITypeInfo>	CreateTypeInfoFromType(WfLexicalScope* scope, Ptr<WfType> type);

			extern void										SearchOrderedName(Ptr<WfDeclaration> declaration, collections::SortedList<vint>& names);
			extern void										SearchOrderedName(Ptr<WfStatement> statement, collections::SortedList<vint>& names);
			extern void										SearchOrderedName(Ptr<WfExpression> expression, collections::SortedList<vint>& names);
			
			struct ValidateStructureContext
			{
				WfBindExpression*							currentBindExpression;
				WfStatement*								currentLoopStatement;
				WfStatement*								currentCatchStatement;

				ValidateStructureContext();
			};
			extern void										ValidateTypeStructure(WfLexicalScopeManager* manager, Ptr<WfType> type, bool returnType = false);
			extern void										ValidateModuleStructure(WfLexicalScopeManager* manager, Ptr<WfModule> module);
			extern void										ValidateDeclarationStructure(WfLexicalScopeManager* manager, Ptr<WfDeclaration> declaration, parsing::ParsingTreeCustomBase* source = 0);
			extern void										ValidateStatementStructure(WfLexicalScopeManager* manager, ValidateStructureContext* context, Ptr<WfStatement>& statement);
			extern void										ValidateExpressionStructure(WfLexicalScopeManager* manager, ValidateStructureContext* context, Ptr<WfExpression>& expression);

			extern void										BuildScopeForModule(WfLexicalScopeManager* manager, Ptr<WfModule> module);
			extern void										BuildScopeForDeclaration(WfLexicalScopeManager* manager, Ptr<WfLexicalScope> parentScope, Ptr<WfDeclaration> declaration, parsing::ParsingTreeCustomBase* source = 0);
			extern void										BuildScopeForStatement(WfLexicalScopeManager* manager, Ptr<WfLexicalScope> parentScope, Ptr<WfStatement> statement);
			extern void										BuildScopeForExpression(WfLexicalScopeManager* manager, Ptr<WfLexicalScope> parentScope, Ptr<WfExpression> expression);

			extern void										ValidateModuleSemantic(WfLexicalScopeManager* manager, Ptr<WfModule> module);
			extern void										ValidateDeclarationSemantic(WfLexicalScopeManager* manager, Ptr<WfDeclaration> declaration);
			extern void										ValidateStatementSemantic(WfLexicalScopeManager* manager, Ptr<WfStatement> statement);

			struct ResolveExpressionResult
			{
				Ptr<WfLexicalScopeName>						scopeName;
				Ptr<WfLexicalSymbol>						symbol;
				Ptr<reflection::description::ITypeInfo>		type;

				ResolveExpressionResult();
				ResolveExpressionResult(const ResolveExpressionResult& result);
				ResolveExpressionResult(Ptr<WfLexicalScopeName> _scopeName);
				ResolveExpressionResult(Ptr<reflection::description::ITypeInfo> _type);
				ResolveExpressionResult(Ptr<WfLexicalSymbol> _symbol, Ptr<reflection::description::ITypeInfo> _type);
			};

			extern void										ValidateExpressionSemantic(WfLexicalScopeManager* manager, Ptr<WfExpression> expression, Ptr<reflection::description::ITypeInfo> expectedType, collections::List<ResolveExpressionResult>& results);
			extern Ptr<WfLexicalScopeName>					GetExpressionScopeName(WfLexicalScopeManager* manager, Ptr<WfExpression> expression);
			extern Ptr<reflection::description::ITypeInfo>	GetExpressionType(WfLexicalScopeManager* manager, Ptr<WfExpression> expression, Ptr<reflection::description::ITypeInfo> expectedType);
			extern bool										CanConvertToType(reflection::description::ITypeInfo* fromType, reflection::description::ITypeInfo* toType, bool explicitly);
			extern bool										IsSameType(reflection::description::ITypeInfo* fromType, reflection::description::ITypeInfo* toType);

/***********************************************************************
Error Messages
***********************************************************************/

			struct WfErrors
			{
				// A: Expression error
				static Ptr<parsing::ParsingError>			WrongFormatStringSyntax(WfExpression* node);
				static Ptr<parsing::ParsingError>			WrongSimpleObserveExpression(WfExpression* node);
				static Ptr<parsing::ParsingError>			WrongSimpleObserveEvent(WfExpression* node);
				static Ptr<parsing::ParsingError>			ObserveNotInBind(WfExpression* node);
				static Ptr<parsing::ParsingError>			BindInBind(WfExpression* node);
				static Ptr<parsing::ParsingError>			ConstructorMixMapAndList(WfExpression* node);
				static Ptr<parsing::ParsingError>			ConstructorMixClassAndInterface(WfExpression* node);
				static Ptr<parsing::ParsingError>			ScopeNameIsNotExpression(WfExpression* node, Ptr<WfLexicalScopeName> scopeName);
				static Ptr<parsing::ParsingError>			ExpressionIsNotScopeName(WfExpression* node);
				static Ptr<parsing::ParsingError>			ExpressionCannotResolveType(WfExpression* node, Ptr<WfLexicalSymbol> symbol);
				static Ptr<parsing::ParsingError>			NullCannotResolveType(WfExpression* node);
				static Ptr<parsing::ParsingError>			OrderedLambdaCannotResolveType(WfExpression* node);
				static Ptr<parsing::ParsingError>			NullCannotImplicitlyConvertToType(WfExpression* node, reflection::description::ITypeInfo* toType);
				static Ptr<parsing::ParsingError>			OrderedLambdaCannotImplicitlyConvertToType(WfExpression* node, reflection::description::ITypeInfo* toType);
				static Ptr<parsing::ParsingError>			ExpressionCannotImplicitlyConvertToType(WfExpression* node, reflection::description::ITypeInfo* fromType, reflection::description::ITypeInfo* toType);
				static Ptr<parsing::ParsingError>			ExpressionCannotExplicitlyConvertToType(WfExpression* node, reflection::description::ITypeInfo* fromType, reflection::description::ITypeInfo* toType);
				static Ptr<parsing::ParsingError>			CannotWeakCastToType(WfExpression* node, reflection::description::ITypeInfo* toType);
				static Ptr<parsing::ParsingError>			IntegerLiteralOutOfRange(WfIntegerExpression* node);
				static Ptr<parsing::ParsingError>			CannotMergeTwoType(WfExpression* node, reflection::description::ITypeInfo* firstType, reflection::description::ITypeInfo* secondType);

				// B: Type error
				static Ptr<parsing::ParsingError>			WrongVoidType(WfType* node);
				static Ptr<parsing::ParsingError>			WrongInterfaceType(WfType* node);
				static Ptr<parsing::ParsingError>			RawPointerToNonReferenceType(WfType* node, reflection::description::ITypeInfo* typeInfo = 0);
				static Ptr<parsing::ParsingError>			SharedPointerToNonReferenceType(WfType* node, reflection::description::ITypeInfo* typeInfo = 0);
				static Ptr<parsing::ParsingError>			NullableToNonReferenceType(WfType* node, reflection::description::ITypeInfo* typeInfo = 0);
				static Ptr<parsing::ParsingError>			ChildOfNonReferenceType(WfType* node);
				static Ptr<parsing::ParsingError>			TypeNotExists(WfType* node, Ptr<WfLexicalScopeName> scopeName);
				static Ptr<parsing::ParsingError>			TypeNotExists(WfType* node, Ptr<WfLexicalSymbol> symbol);
				static Ptr<parsing::ParsingError>			TypeNotForValue(WfType* node, reflection::description::ITypeInfo* typeInfo);
				
				// C: Statement error
				static Ptr<parsing::ParsingError>			BreakNotInLoop(WfStatement* node);
				static Ptr<parsing::ParsingError>			ContinueNotInLoop(WfStatement* node);
				static Ptr<parsing::ParsingError>			RethrowNotInCatch(WfStatement* node);
				static Ptr<parsing::ParsingError>			TryMissCatchAndFinally(WfStatement* node);

				// D: Declaration error
				static Ptr<parsing::ParsingError>			FunctionShouldHaveName(WfDeclaration* node);
				static Ptr<parsing::ParsingError>			DuplicatedDeclaration(WfDeclaration* node, const WString& firstDeclarationCategory);
				static Ptr<parsing::ParsingError>			DuplicatedSymbol(WfDeclaration* node, Ptr<WfLexicalSymbol> symbol);
				static Ptr<parsing::ParsingError>			DuplicatedSymbol(WfStatement* node, Ptr<WfLexicalSymbol> symbol);
				static Ptr<parsing::ParsingError>			DuplicatedSymbol(WfExpression* node, Ptr<WfLexicalSymbol> symbol);

				// E: Module error
				static Ptr<parsing::ParsingError>			WrongUsingPathWildCard(WfModuleUsingPath* node);

				// F: Symbol Error
				static Ptr<parsing::ParsingError>			TopQualifiedSymbolNotExists(WfType* node, const WString& name);
				static Ptr<parsing::ParsingError>			TopQualifiedSymbolNotExists(WfExpression* node, const WString& name);
				static Ptr<parsing::ParsingError>			ChildSymbolNotExists(WfType* node, Ptr<WfLexicalScopeName> scopeName, const WString& name);
				static Ptr<parsing::ParsingError>			ChildSymbolNotExists(WfExpression* node, Ptr<WfLexicalScopeName> scopeName, const WString& name);
				static Ptr<parsing::ParsingError>			ReferenceNotExists(WfType* node, const WString& name);
				static Ptr<parsing::ParsingError>			ReferenceNotExists(WfExpression* node, const WString& name);
				static Ptr<parsing::ParsingError>			TooManySymbol(WfType* node, collections::List<Ptr<WfLexicalSymbol>>& symbols, const WString& name);
				static Ptr<parsing::ParsingError>			TooManySymbol(WfExpression* node, collections::List<Ptr<WfLexicalSymbol>>& symbols, const WString& name);
				static Ptr<parsing::ParsingError>			TooManyScopeName(WfType* node, collections::List<Ptr<WfLexicalScopeName>>& names, const WString& name);
				static Ptr<parsing::ParsingError>			TooManyScopeName(WfExpression* node, collections::List<Ptr<WfLexicalScopeName>>& names, const WString& name);
			};
		}
	}
}