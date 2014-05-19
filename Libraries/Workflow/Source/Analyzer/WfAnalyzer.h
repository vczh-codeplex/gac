/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Workflow::Analyzer

Interfaces:
**********************************************************************/

#ifndef VCZH_WORKFLOW_ANALYZER_WFANALYZER
#define VCZH_WORKFLOW_ANALYZER_WFANALYZER

#include "../Expression/WfExpression.h"
#include "../Runtime/WfRuntime.h"

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
				Ptr<WfDeclaration>							creatorDeclaration;	// nullable
				Ptr<WfStatement>							creatorStatement;	// nullable
				Ptr<WfExpression>							creatorExpression;	// nullable
				Ptr<WfFunctionArgument>						creatorArgument;	// nullable
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

			struct ResolveExpressionResult
			{
				Ptr<WfLexicalScopeName>						scopeName;
				Ptr<WfLexicalSymbol>						symbol;
				reflection::description::IPropertyInfo*		propertyInfo;
				reflection::description::IMethodInfo*		methodInfo;
				reflection::description::IEventInfo*		eventInfo;
				Ptr<reflection::description::ITypeInfo>		type;
				Ptr<reflection::description::ITypeInfo>		leftValueType;
				Ptr<reflection::description::ITypeInfo>		expectedType;

				ResolveExpressionResult();
				ResolveExpressionResult(Ptr<WfLexicalScopeName> _scopeName);
				ResolveExpressionResult(Ptr<reflection::description::ITypeInfo> _type, Ptr<reflection::description::ITypeInfo> _leftValueType = 0);
				ResolveExpressionResult(Ptr<WfLexicalSymbol> _symbol, Ptr<reflection::description::ITypeInfo> _type, Ptr<reflection::description::ITypeInfo> _leftValueType = 0);
				ResolveExpressionResult(reflection::description::IPropertyInfo* _propertyInfo, Ptr<reflection::description::ITypeInfo> _type, Ptr<reflection::description::ITypeInfo> _leftValueType = 0);
				ResolveExpressionResult(reflection::description::IMethodInfo* _methodInfo, Ptr<reflection::description::ITypeInfo> _type);
				ResolveExpressionResult(reflection::description::IEventInfo* _eventInfo);

				WString										GetFriendlyName()const;
			};

			class WfLexicalScopeManager : public Object
			{
				typedef collections::List<Ptr<WfModule>>													ModuleList;
				typedef collections::List<Ptr<parsing::ParsingError>>										ParsingErrorList;
				typedef collections::Dictionary<Ptr<WfNamespaceDeclaration>, Ptr<WfLexicalScopeName>>		NamespaceNameMap;
				typedef collections::SortedList<Ptr<WfLexicalScope>>										ScopeSortedList;
				typedef collections::Dictionary<Ptr<WfModule>, Ptr<WfLexicalScope>>							ModuleScopeMap;
				typedef collections::Dictionary<Ptr<WfDeclaration>, Ptr<WfLexicalScope>>					DeclarationScopeMap;
				typedef collections::Dictionary<Ptr<WfStatement>, Ptr<WfLexicalScope>>						StatementScopeMap;
				typedef collections::Dictionary<Ptr<WfExpression>, Ptr<WfLexicalScope>>						ExpressionScopeMap;
				typedef collections::Dictionary<Ptr<WfExpression>, ResolveExpressionResult>					ExpressionResolvingMap;
				typedef collections::Group<WfFunctionDeclaration*, Ptr<WfLexicalSymbol>>					FunctionLambdaCaptureGroup;
				typedef collections::Group<WfOrderedLambdaExpression*, Ptr<WfLexicalSymbol>>				OrderedLambdaCaptureGroup;
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
				ScopeSortedList								analyzedScopes;

				ModuleScopeMap								moduleScopes;				// the nearest scope for the module
				DeclarationScopeMap							declarationScopes;			// the nearest scope for the declaration
				StatementScopeMap							statementScopes;			// the nearest scope for the statement
				ExpressionScopeMap							expressionScopes;			// the nearest scope for the expression
				ExpressionResolvingMap						expressionResolvings;		// the resolving result for the expression
				FunctionLambdaCaptureGroup					functionLambdaCaptures;		// all captured symbol in an lambda expression
				OrderedLambdaCaptureGroup					orderedLambdaCaptures;		// all captured symbol in an lambda expression

				WfLexicalScopeManager(Ptr<parsing::tabling::ParsingTable> _parsingTable);
				~WfLexicalScopeManager();
				
				Ptr<WfModule>								AddModule(const WString& moduleCode, vint codeIndex = -1);
				void										Clear(bool keepTypeDescriptorNames, bool deleteModules);
				bool										CheckScopes();
				void										Rebuild(bool keepTypeDescriptorNames);
				void										ResolveSymbol(WfLexicalScope* scope, const WString& symbolName, collections::List<Ptr<WfLexicalSymbol>>& symbols);
				void										ResolveScopeName(WfLexicalScope* scope, const WString& symbolName, collections::List<Ptr<WfLexicalScopeName>>& names);
			};

/***********************************************************************
Type Analyzing
***********************************************************************/

			enum class TypeFlag
			{
				Bool,
				I1,
				I2,
				I4,
				I8,
				U1,
				U2,
				U4,
				U8,
				F4,
				F8,
				String,
				Others,
				Count,
				Unknown = -1,
#ifdef VCZH_64
				I = I8,
				U = U8,
#else
				I = I4,
				U = U4,
#endif
			};

			extern TypeFlag									GetTypeFlag(reflection::description::ITypeDescriptor* typeDescriptor);
			extern TypeFlag									GetTypeFlag(reflection::description::ITypeInfo* typeInfo);
			extern Ptr<reflection::description::ITypeInfo>	CreateTypeInfoFromTypeFlag(TypeFlag flag);

			extern Ptr<WfType>								GetTypeFromTypeInfo(reflection::description::ITypeInfo* typeInfo);
			extern Ptr<WfLexicalScopeName>					GetScopeNameFromReferenceType(WfLexicalScope* scope, Ptr<WfType> type);
			extern Ptr<reflection::description::ITypeInfo>	CreateTypeInfoFromType(WfLexicalScope* scope, Ptr<WfType> type);

			extern Ptr<WfType>								CopyType(Ptr<WfType> type);
			extern Ptr<reflection::description::ITypeInfo>	CopyTypeInfo(reflection::description::ITypeInfo* typeInfo);
			extern bool										CanConvertToType(reflection::description::ITypeInfo* fromType, reflection::description::ITypeInfo* toType, bool explicitly);
			extern bool										IsSameType(reflection::description::ITypeInfo* fromType, reflection::description::ITypeInfo* toType);
			extern Ptr<reflection::description::ITypeInfo>	GetMergedType(Ptr<reflection::description::ITypeInfo> firstType, Ptr<reflection::description::ITypeInfo> secondType);
			extern bool										IsNullAcceptableType(reflection::description::ITypeInfo* type);

			extern Ptr<reflection::description::ITypeInfo>	CreateTypeInfoFromMethodInfo(reflection::description::IMethodInfo* info);

			extern bool										IsExpressionDependOnExpectedType(WfLexicalScopeManager* manager, Ptr<WfExpression> expression);
			extern WString									GetExpressionName(Ptr<WfExpression> expression);
			extern void										SearchOrderedName(WfLexicalScope* scope, Ptr<WfDeclaration> declaration, collections::SortedList<vint>& names);
			extern void										SearchOrderedName(WfLexicalScope* scope, Ptr<WfStatement> statement, collections::SortedList<vint>& names);
			extern void										SearchOrderedName(WfLexicalScope* scope, Ptr<WfExpression> expression, collections::SortedList<vint>& names);

/***********************************************************************
Structure Analyzing
***********************************************************************/
			
			struct ValidateStructureContext
			{
				WfBindExpression*							currentBindExpression;
				WfObserveExpression*						currentObserveExpression;
				WfStatement*								currentLoopStatement;
				WfStatement*								currentCatchStatement;

				ValidateStructureContext();
			};
			extern void										ValidateTypeStructure(WfLexicalScopeManager* manager, Ptr<WfType> type, bool returnType = false);
			extern void										ValidateModuleStructure(WfLexicalScopeManager* manager, Ptr<WfModule> module);
			extern void										ValidateDeclarationStructure(WfLexicalScopeManager* manager, Ptr<WfDeclaration> declaration, parsing::ParsingTreeCustomBase* source = 0);
			extern void										ValidateStatementStructure(WfLexicalScopeManager* manager, ValidateStructureContext* context, Ptr<WfStatement>& statement);
			extern void										ValidateExpressionStructure(WfLexicalScopeManager* manager, ValidateStructureContext* context, Ptr<WfExpression>& expression);

/***********************************************************************
Scope Analyzing
***********************************************************************/

			extern void										BuildScopeForModule(WfLexicalScopeManager* manager, Ptr<WfModule> module);
			extern void										BuildScopeForDeclaration(WfLexicalScopeManager* manager, Ptr<WfLexicalScope> parentScope, Ptr<WfDeclaration> declaration, parsing::ParsingTreeCustomBase* source = 0);
			extern void										BuildScopeForStatement(WfLexicalScopeManager* manager, Ptr<WfLexicalScope> parentScope, Ptr<WfStatement> statement);
			extern void										BuildScopeForExpression(WfLexicalScopeManager* manager, Ptr<WfLexicalScope> parentScope, Ptr<WfExpression> expression);

/***********************************************************************
Semantic Analyzing
***********************************************************************/

			class WfObservingDependency : public Object
			{
				typedef collections::Group<WfExpression*, WfExpression*>			DependencyGroup;
				typedef collections::List<WfExpression*>							ObserveList;
			public:
				ObserveList							inputObserves;
				ObserveList							outputObserves;
				DependencyGroup&					dependencies;
				
				WfObservingDependency(WfObservingDependency& dependency);
				WfObservingDependency(DependencyGroup& _dependencies);
				WfObservingDependency(DependencyGroup& _dependencies, ObserveList& _inputObserves);
				
				void								Prepare(WfExpression* observe);
				void								AddInternal(WfExpression* observe, WfExpression* dependedObserve);
				void								Add(WfExpression* observe);
				void								Add(WfExpression* observe, WfObservingDependency& dependency);
				void								TurnToInput();
				void								Cleanup();
			};

			extern void										ValidateModuleSemantic(WfLexicalScopeManager* manager, Ptr<WfModule> module);
			extern void										ValidateDeclarationSemantic(WfLexicalScopeManager* manager, Ptr<WfDeclaration> declaration);
			extern void										ValidateStatementSemantic(WfLexicalScopeManager* manager, Ptr<WfStatement> statement);
			extern void										ValidateExpressionSemantic(WfLexicalScopeManager* manager, Ptr<WfExpression> expression, Ptr<reflection::description::ITypeInfo> expectedType, collections::List<ResolveExpressionResult>& results);
			extern void										GetObservingDependency(WfLexicalScopeManager* manager, Ptr<WfExpression> expression, WfObservingDependency& dependency);
			extern Ptr<WfExpression>						ExpandObserveExpression(WfExpression* expression, collections::Dictionary<WfExpression*, WString>& cacheNames, collections::Dictionary<WString, WString>& referenceReplacement, bool useCache = true);
			extern Ptr<WfExpression>						CopyExpression(Ptr<WfExpression> expression);
			extern Ptr<WfStatement>							CopyStatement(Ptr<WfStatement> statement);
			extern void										ExpandBindExpression(WfLexicalScopeManager* manager, WfBindExpression* node);

			extern Ptr<WfLexicalScopeName>					GetExpressionScopeName(WfLexicalScopeManager* manager, Ptr<WfExpression> expression);
			extern reflection::description::IEventInfo*		GetExpressionEventInfo(WfLexicalScopeManager* manager, Ptr<WfExpression> expression);
			extern void										GetExpressionTypes(WfLexicalScopeManager* manager, Ptr<WfExpression> expression, Ptr<reflection::description::ITypeInfo> expectedType, collections::List<ResolveExpressionResult>& results);
			extern Ptr<reflection::description::ITypeInfo>	GetExpressionType(WfLexicalScopeManager* manager, Ptr<WfExpression> expression, Ptr<reflection::description::ITypeInfo> expectedType);
			extern Ptr<reflection::description::ITypeInfo>	GetLeftValueExpressionType(WfLexicalScopeManager* manager, Ptr<WfExpression> expression);
			extern Ptr<reflection::description::ITypeInfo>	GetEnumerableExpressionItemType(WfLexicalScopeManager* manager, Ptr<WfExpression> expression, Ptr<reflection::description::ITypeInfo> expectedType);

/***********************************************************************
Code Generation
***********************************************************************/

			struct WfCodegenLambdaContext
			{
				WfFunctionDeclaration*				functionDeclaration = 0;
				WfFunctionExpression*				functionExpression = 0;
				WfOrderedLambdaExpression*			orderedLambdaExpression = 0;
				WfMemberExpression*					methodReferenceExpression = 0;
				WfExpression*						staticMethodReferenceExpression = 0;
			};

			enum class WfCodegenScopeType
			{
				Function,	// contains the whole function
				Switch,		// contains all switchs
				Loop,		// contains all loops
				TryCatch,	// contains try and catch, not finally
			};

			class WfCodegenScopeContext : public Object
			{
				typedef collections::List<vint>										InstructionIndexList;
				typedef collections::List<runtime::WfInstruction>					InstructionList;
			public:
				WfCodegenScopeType					type = WfCodegenScopeType::Function;
				InstructionIndexList				continueInstructions;
				InstructionIndexList				breakInstructions;
				
				InstructionList						exitInstructions;
				Ptr<WfStatement>					exitStatement;
			};

			class WfCodegenFunctionContext : public Object
			{
				typedef collections::Dictionary<WfLexicalSymbol*, vint>				VariableIndexMap;
				typedef collections::Dictionary<vint, WfCodegenLambdaContext>		ClosureIndexMap;
				typedef collections::List<Ptr<WfCodegenScopeContext>>				ScopeContextList;
			public:
				Ptr<runtime::WfAssemblyFunction>	function;
				VariableIndexMap					capturedVariables;
				VariableIndexMap					arguments;
				VariableIndexMap					localVariables;
				ClosureIndexMap						closuresToCodegen;
				ScopeContextList					scopeContextStack;

				WfCodegenFunctionContext();
				
				Ptr<WfCodegenScopeContext>			GetCurrentScopeContext();
				Ptr<WfCodegenScopeContext>			GetCurrentScopeContext(WfCodegenScopeType type);
				Ptr<WfCodegenScopeContext>			PushScopeContext(WfCodegenScopeType type);
				void								PopScopeContext();
			};

			class WfCodegenContext : public Object
			{
				typedef collections::Dictionary<WfLexicalSymbol*, vint>				VariableIndexMap;
				typedef collections::Dictionary<WfLexicalSymbol*, vint>				FunctionIndexMap;
			public:
				Ptr<runtime::WfAssembly>			assembly;
				WfLexicalScopeManager*				manager;
				VariableIndexMap					globalVariables;
				FunctionIndexMap					globalFunctions;
				Ptr<WfCodegenFunctionContext>		functionContext;

				WfCodegenContext(Ptr<runtime::WfAssembly> _assembly, WfLexicalScopeManager* _manager);
			};

			extern void										GenerateGlobalDeclarationMetadata(WfCodegenContext& context, Ptr<WfDeclaration> declaration, const WString& namePrefix = L"");
			extern void										GenerateClosureInstructions(WfCodegenContext& context, Ptr<WfCodegenFunctionContext> functionContext);
			extern void										GenerateInitializeInstructions(WfCodegenContext& context, Ptr<WfDeclaration> declaration);
			extern void										GenerateDeclarationInstructions(WfCodegenContext& context, Ptr<WfDeclaration> declaration);
			extern void										GenerateStatementInstructions(WfCodegenContext& context, Ptr<WfStatement> statement);
			extern Ptr<reflection::description::ITypeInfo>	GenerateExpressionInstructions(WfCodegenContext& context, Ptr<WfExpression> expression, Ptr<reflection::description::ITypeInfo> expectedType = 0);
			extern void										GenerateTypeCastInstructions(WfCodegenContext& context, Ptr<reflection::description::ITypeInfo> expectedType, bool strongCast);
			extern void										GenerateTypeTestingInstructions(WfCodegenContext& context, Ptr<reflection::description::ITypeInfo> expectedType);
			extern runtime::WfInsType						GetInstructionTypeArgument(Ptr<reflection::description::ITypeInfo> expectedType);
			extern Ptr<runtime::WfAssembly>					GenerateAssembly(WfLexicalScopeManager* manager);
			extern Ptr<runtime::WfAssembly>					Compile(Ptr<parsing::tabling::ParsingTable> table, WfLexicalScopeManager* manager, collections::List<WString>& moduleCodes, collections::List<Ptr<parsing::ParsingError>>& errors);
			extern Ptr<runtime::WfAssembly>					Compile(Ptr<parsing::tabling::ParsingTable> table, collections::List<WString>& moduleCodes, collections::List<Ptr<parsing::ParsingError>>& errors);

/***********************************************************************
Error Messages
***********************************************************************/

			struct WfErrors
			{
				// A: Expression error
				static Ptr<parsing::ParsingError>			WrongFormatStringSyntax(WfExpression* node);
				static Ptr<parsing::ParsingError>			WrongSimpleObserveExpression(WfExpression* node);
				static Ptr<parsing::ParsingError>			WrongSimpleObserveEvent(WfExpression* node);
				static Ptr<parsing::ParsingError>			EmptyObserveEvent(WfExpression* node);
				static Ptr<parsing::ParsingError>			ObserveNotInBind(WfExpression* node);
				static Ptr<parsing::ParsingError>			ObserveInObserveEvent(WfExpression* node);
				static Ptr<parsing::ParsingError>			BindInBind(WfExpression* node);
				static Ptr<parsing::ParsingError>			AttachInBind(WfExpression* node);
				static Ptr<parsing::ParsingError>			DetachInBind(WfExpression* node);
				static Ptr<parsing::ParsingError>			ConstructorMixMapAndList(WfExpression* node);
				static Ptr<parsing::ParsingError>			ConstructorMixClassAndInterface(WfExpression* node);
				static Ptr<parsing::ParsingError>			ScopeNameIsNotExpression(WfExpression* node, Ptr<WfLexicalScopeName> scopeName);
				static Ptr<parsing::ParsingError>			EventIsNotExpression(WfExpression* node, reflection::description::IEventInfo* eventInfo);
				static Ptr<parsing::ParsingError>			ExpressionIsNotScopeName(WfExpression* node);
				static Ptr<parsing::ParsingError>			ExpressionIsNotEvent(WfExpression* node);
				static Ptr<parsing::ParsingError>			ExpressionCannotResolveType(WfExpression* node, Ptr<WfLexicalSymbol> symbol);
				static Ptr<parsing::ParsingError>			NullCannotResolveType(WfExpression* node);
				static Ptr<parsing::ParsingError>			ConstructorCannotResolveType(WfExpression* node);
				static Ptr<parsing::ParsingError>			OrderedLambdaCannotResolveType(WfExpression* node);
				static Ptr<parsing::ParsingError>			NullCannotImplicitlyConvertToType(WfExpression* node, reflection::description::ITypeInfo* toType);
				static Ptr<parsing::ParsingError>			ConstructorCannotImplicitlyConvertToType(WfExpression* node, reflection::description::ITypeInfo* toType);
				static Ptr<parsing::ParsingError>			OrderedLambdaCannotImplicitlyConvertToType(WfExpression* node, reflection::description::ITypeInfo* toType);
				static Ptr<parsing::ParsingError>			ExpressionCannotImplicitlyConvertToType(WfExpression* node, reflection::description::ITypeInfo* fromType, reflection::description::ITypeInfo* toType);
				static Ptr<parsing::ParsingError>			ExpressionCannotExplicitlyConvertToType(WfExpression* node, reflection::description::ITypeInfo* fromType, reflection::description::ITypeInfo* toType);
				static Ptr<parsing::ParsingError>			CannotWeakCastToType(WfExpression* node, reflection::description::ITypeInfo* toType);
				static Ptr<parsing::ParsingError>			IntegerLiteralOutOfRange(WfIntegerExpression* node);
				static Ptr<parsing::ParsingError>			CannotMergeTwoType(WfExpression* node, reflection::description::ITypeInfo* firstType, reflection::description::ITypeInfo* secondType);
				static Ptr<parsing::ParsingError>			RangeShouldBeInteger(WfExpression* node, reflection::description::ITypeInfo* type);
				static Ptr<parsing::ParsingError>			UnaryOperatorOnWrongType(WfUnaryExpression* node, reflection::description::ITypeInfo* type);
				static Ptr<parsing::ParsingError>			BinaryOperatorOnWrongType(WfBinaryExpression* node, reflection::description::ITypeInfo* type);
				static Ptr<parsing::ParsingError>			IndexOperatorOnWrongType(WfBinaryExpression* node, reflection::description::ITypeInfo* containerType);
				static Ptr<parsing::ParsingError>			ExpressionIsNotCollection(WfExpression* node, reflection::description::ITypeInfo* type);
				static Ptr<parsing::ParsingError>			ExpressionIsNotFunction(WfExpression* node, reflection::description::ITypeInfo* type);
				static Ptr<parsing::ParsingError>			FunctionArgumentCountMismatched(WfExpression* node, vint expectedCount, const ResolveExpressionResult& function);
				static Ptr<parsing::ParsingError>			FunctionArgumentTypeMismatched(WfExpression* node, const ResolveExpressionResult& function, vint index, reflection::description::ITypeInfo* fromType, reflection::description::ITypeInfo* toType);
				static Ptr<parsing::ParsingError>			CannotPickOverloadedFunctions(WfExpression* node, collections::List<ResolveExpressionResult>& results);
				static Ptr<parsing::ParsingError>			ClassContainsNoConstructor(WfExpression* node, reflection::description::ITypeInfo* type);
				static Ptr<parsing::ParsingError>			InterfaceContainsNoConstructor(WfExpression* node, reflection::description::ITypeInfo* type);
				static Ptr<parsing::ParsingError>			ConstructorReturnTypeMismatched(WfExpression* node, const ResolveExpressionResult& function, reflection::description::ITypeInfo* fromType, reflection::description::ITypeInfo* toType);
				static Ptr<parsing::ParsingError>			ExpressionIsNotLeftValue(WfExpression* node, const ResolveExpressionResult& result);

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
				static Ptr<parsing::ParsingError>			ReturnMissExpression(WfStatement* node, reflection::description::ITypeInfo* type);
				static Ptr<parsing::ParsingError>			DeleteNonRawPointer(WfStatement* node, reflection::description::ITypeInfo* type);

				// D: Declaration error
				static Ptr<parsing::ParsingError>			FunctionShouldHaveName(WfDeclaration* node);
				static Ptr<parsing::ParsingError>			DuplicatedDeclaration(WfDeclaration* node, const WString& firstDeclarationCategory);
				static Ptr<parsing::ParsingError>			DuplicatedSymbol(WfDeclaration* node, Ptr<WfLexicalSymbol> symbol);
				static Ptr<parsing::ParsingError>			DuplicatedSymbol(WfFunctionArgument* node, Ptr<WfLexicalSymbol> symbol);
				static Ptr<parsing::ParsingError>			DuplicatedSymbol(WfStatement* node, Ptr<WfLexicalSymbol> symbol);
				static Ptr<parsing::ParsingError>			DuplicatedSymbol(WfExpression* node, Ptr<WfLexicalSymbol> symbol);
				static Ptr<parsing::ParsingError>			InterfaceMethodNotImplemented(WfNewTypeExpression* node, reflection::description::IMethodInfo* method);
				static Ptr<parsing::ParsingError>			InterfaceMethodNotFound(WfFunctionDeclaration* node, reflection::description::ITypeInfo* interfaceType, reflection::description::ITypeInfo* methodType);
				static Ptr<parsing::ParsingError>			CannotPickOverloadedInterfaceMethods(WfExpression* node, collections::List<ResolveExpressionResult>& results);
				static Ptr<parsing::ParsingError>			CannotPickOverloadedImplementMethods(WfFunctionDeclaration* node, reflection::description::ITypeInfo* type);

				// E: Module error
				static Ptr<parsing::ParsingError>			WrongUsingPathWildCard(WfModuleUsingPath* node);

				// F: Symbol Error
				static Ptr<parsing::ParsingError>			TopQualifiedSymbolNotExists(parsing::ParsingTreeCustomBase* node, const WString& name);
				static Ptr<parsing::ParsingError>			ChildSymbolNotExists(parsing::ParsingTreeCustomBase* node, Ptr<WfLexicalScopeName> scopeName, const WString& name);
				static Ptr<parsing::ParsingError>			MemberNotExists(parsing::ParsingTreeCustomBase* node, reflection::description::ITypeDescriptor* typeDescriptor, const WString& name);
				static Ptr<parsing::ParsingError>			ReferenceNotExists(parsing::ParsingTreeCustomBase* node, const WString& name);
				static Ptr<parsing::ParsingError>			TooManyTargets(parsing::ParsingTreeCustomBase* node, collections::List<ResolveExpressionResult>& results, const WString& name);
				static Ptr<parsing::ParsingError>			TooManyTargets(parsing::ParsingTreeCustomBase* node, collections::List<Ptr<WfLexicalSymbol>>& symbols, const WString& name);
				static Ptr<parsing::ParsingError>			TooManyTargets(parsing::ParsingTreeCustomBase* node, collections::List<Ptr<WfLexicalScopeName>>& names, const WString& name);
			};
		}
	}
}

#endif