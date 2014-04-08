#include "WfAnalyzer.h"

namespace vl
{
	namespace workflow
	{
		namespace analyzer
		{
			using namespace collections;
			using namespace parsing;
			using namespace reflection;
			using namespace reflection::description;

/***********************************************************************
WfErrors
***********************************************************************/

			Ptr<parsing::ParsingError> WfErrors::WrongFormatStringSyntax(WfExpression* node)
			{
				return new ParsingError(node, L"A0: Wrong format string syntax.");
			}

			Ptr<parsing::ParsingError> WfErrors::WrongSimpleObserveExpression(WfExpression* node)
			{
				return new ParsingError(node, L"A1: Simple observe expression should observe a property under the observed object.");
			}

			Ptr<parsing::ParsingError> WfErrors::WrongSimpleObserveEvent(WfExpression* node)
			{
				return new ParsingError(node, L"A2: Simple observe expression should react to an event under the observed object.");
			}

			Ptr<parsing::ParsingError> WfErrors::EmptyObserveEvent(WfExpression* node)
			{
				return new ParsingError(node, L"A2: Observe expression should react to at least one event.");
			}

			Ptr<parsing::ParsingError> WfErrors::ObserveNotInBind(WfExpression* node)
			{
				return new ParsingError(node, L"A3: Observe expression should appear in a bind expression.");
			}

			Ptr<parsing::ParsingError> WfErrors::ObserveInObserveEvent(WfExpression* node)
			{
				return new ParsingError(node, L"A4: Observe expression should not appear in the event expression in another observe expression.");
			}

			Ptr<parsing::ParsingError> WfErrors::BindInBind(WfExpression* node)
			{
				return new ParsingError(node, L"A4: Bind expression should not appear in another bind expression.");
			}

			Ptr<parsing::ParsingError> WfErrors::AttachInBind(WfExpression* node)
			{
				return new ParsingError(node, L"A4: Attach expression should not appear in another bind expression.");
			}

			Ptr<parsing::ParsingError> WfErrors::DetachInBind(WfExpression* node)
			{
				return new ParsingError(node, L"A4: Detach expression should not appear in another bind expression.");
			}

			Ptr<parsing::ParsingError> WfErrors::ConstructorMixMapAndList(WfExpression* node)
			{
				return new ParsingError(node, L"A5: Key-value pairs are not allowed in list constructor expression.");
			}

			Ptr<parsing::ParsingError> WfErrors::ConstructorMixClassAndInterface(WfExpression* node)
			{
				return new ParsingError(node, L"A6: Functions are not allowed in class constructor expression.");
			}

			Ptr<parsing::ParsingError> WfErrors::ScopeNameIsNotExpression(WfExpression* node, Ptr<WfLexicalScopeName> scopeName)
			{
				return new ParsingError(node, L"A7: Symbol \"" + scopeName->GetFriendlyName() + L"\" cannot be used as an expression.");
			}

			Ptr<parsing::ParsingError> WfErrors::EventIsNotExpression(WfExpression* node, reflection::description::IEventInfo* eventInfo)
			{
				return new ParsingError(node, L"A7: Event \"" + eventInfo->GetName() + L"\" of type \"" + eventInfo->GetOwnerTypeDescriptor()->GetTypeName() + L"\" cannot be used as an expression.");
			}

			Ptr<parsing::ParsingError> WfErrors::ExpressionIsNotScopeName(WfExpression* node)
			{
				return new ParsingError(node, L"A8: Expression does not reference to a declaration.");
			}

			Ptr<parsing::ParsingError> WfErrors::ExpressionIsNotEvent(WfExpression* node)
			{
				return new ParsingError(node, L"A8: Expression does not reference to an event.");
			}

			Ptr<parsing::ParsingError> WfErrors::ExpressionCannotResolveType(WfExpression* node, Ptr<WfLexicalSymbol> symbol)
			{
				return new ParsingError(node, L"A9: Expression referencing to symbol \"" + symbol->name + L"\" failed to resolve its type.");
			}

			Ptr<parsing::ParsingError> WfErrors::NullCannotResolveType(WfExpression* node)
			{
				return new ParsingError(node, L"A9: Expression \"null\" failed to resolve its type.");
			}

			Ptr<parsing::ParsingError> WfErrors::ConstructorCannotResolveType(WfExpression* node)
			{
				return new ParsingError(node, L"A9: Expression \"{}\" failed to resolve its type.");
			}

			Ptr<parsing::ParsingError> WfErrors::OrderedLambdaCannotResolveType(WfExpression* node)
			{
				return new ParsingError(node, L"A9: Ordered lambda expression failed to resolve its type.");
			}

			Ptr<parsing::ParsingError> WfErrors::NullCannotImplicitlyConvertToType(WfExpression* node, reflection::description::ITypeInfo* toType)
			{
				return new ParsingError(node, L"A10: Expression \"null\" cannot implicitly convert to \"" + toType->GetTypeFriendlyName() + L"\".");
			}

			Ptr<parsing::ParsingError> WfErrors::ConstructorCannotImplicitlyConvertToType(WfExpression* node, reflection::description::ITypeInfo* toType)
			{
				return new ParsingError(node, L"A10: Expression \"{}\" cannot implicitly convert to \"" + toType->GetTypeFriendlyName() + L"\".");
			}

			Ptr<parsing::ParsingError> WfErrors::OrderedLambdaCannotImplicitlyConvertToType(WfExpression* node, reflection::description::ITypeInfo* toType)
			{
				return new ParsingError(node, L"A10: Ordered lambda expression cannot implicitly convert to \"" + toType->GetTypeFriendlyName() + L"\".");
			}

			Ptr<parsing::ParsingError> WfErrors::ExpressionCannotImplicitlyConvertToType(WfExpression* node, reflection::description::ITypeInfo* fromType, reflection::description::ITypeInfo* toType)
			{
				return new ParsingError(node, L"A10: Expression of type \"" + fromType->GetTypeFriendlyName() + L"\" cannot implicitly convert to \"" + toType->GetTypeFriendlyName() + L"\".");
			}

			Ptr<parsing::ParsingError> WfErrors::ExpressionCannotExplicitlyConvertToType(WfExpression* node, reflection::description::ITypeInfo* fromType, reflection::description::ITypeInfo* toType)
			{
				return new ParsingError(node, L"A11: Expression of type \"" + fromType->GetTypeFriendlyName() + L"\" cannot explicitly convert to \"" + toType->GetTypeFriendlyName() + L"\".");
			}

			Ptr<parsing::ParsingError> WfErrors::CannotWeakCastToType(WfExpression* node, reflection::description::ITypeInfo* toType)
			{
				return new ParsingError(node, L"A12: Expression cannot weakly cast to \"" + toType->GetTypeFriendlyName() + L"\" because it cannot be \"null\".");
			}

			Ptr<parsing::ParsingError> WfErrors::IntegerLiteralOutOfRange(WfIntegerExpression* node)
			{
				return new ParsingError(node, L"A13: Integer literal \"" + node->value.value + L"\" out of range.");
			}

			Ptr<parsing::ParsingError> WfErrors::CannotMergeTwoType(WfExpression* node, reflection::description::ITypeInfo* firstType, reflection::description::ITypeInfo* secondType)
			{
				return new ParsingError(node, L"A14: Failed to merge type \"" + firstType->GetTypeFriendlyName() + L"\" with type \"" + secondType->GetTypeFriendlyName() + L"\" together to calculate the result type.");
			}

			Ptr<parsing::ParsingError> WfErrors::RangeShouldBeInteger(WfExpression* node, reflection::description::ITypeInfo* type)
			{
				return new ParsingError(node, L"A15: Elements in a range expression should be integer, not \"" + type->GetTypeFriendlyName() + L"\".");
			}

			Ptr<parsing::ParsingError> WfErrors::UnaryOperatorOnWrongType(WfUnaryExpression* node, reflection::description::ITypeInfo* type)
			{
				return new ParsingError(node, L"A16: Unary operator cannot apply on expression of type \"" + type->GetTypeFriendlyName() + L"\".");
			}

			Ptr<parsing::ParsingError> WfErrors::BinaryOperatorOnWrongType(WfBinaryExpression* node, reflection::description::ITypeInfo* type)
			{
				return new ParsingError(node, L"A16: Binary operator cannot apply on expression of type \"" + type->GetTypeFriendlyName() + L"\".");
			}

			Ptr<parsing::ParsingError> WfErrors::IndexOperatorOnWrongType(WfBinaryExpression* node, reflection::description::ITypeInfo* containerType)
			{
				return new ParsingError(node, L"A17: Container of type \"" + containerType->GetTypeFriendlyName() + L"\" cannot be accessed using index.");
			}

			Ptr<parsing::ParsingError> WfErrors::ExpressionIsNotCollection(WfExpression* node, reflection::description::ITypeInfo* type)
			{
				return new ParsingError(node, L"A18: Expression of type \"" + type->GetTypeFriendlyName() + L"\" is not an enumerable type.");
			}

			Ptr<parsing::ParsingError> WfErrors::ExpressionIsNotFunction(WfExpression* node, reflection::description::ITypeInfo* type)
			{
				return new ParsingError(node, L"A19: Expression of type \"" + type->GetTypeFriendlyName() + L"\" is not an invokable function type.");
			}

			Ptr<parsing::ParsingError> WfErrors::FunctionArgumentCountMismatched(WfExpression* node, vint expectedCount, const ResolveExpressionResult& function)
			{
				return new ParsingError(node, L"A20: Function " + function.GetFriendlyName() + L"\" is not allowed to call with " + itow(expectedCount) + L" arguments.");
			}

			Ptr<parsing::ParsingError> WfErrors::FunctionArgumentTypeMismatched(WfExpression* node, const ResolveExpressionResult& function, vint index, reflection::description::ITypeInfo* fromType, reflection::description::ITypeInfo* toType)
			{
				return new ParsingError(node, L"A21: The " + itow(index) + L"-th argument of function " + function.GetFriendlyName() + L" cannot implicitly convert from \"" + fromType->GetTypeFriendlyName() + L"\" to \"" + toType->GetTypeFriendlyName() + L"\".");
			}

			Ptr<parsing::ParsingError> WfErrors::CannotPickOverloadedFunctions(WfExpression* node, collections::List<ResolveExpressionResult>& results)
			{
				WString description;
				FOREACH_INDEXER(ResolveExpressionResult, result, index, results)
				{
					description += L"\r\n\t";
					description += result.GetFriendlyName();
				}
				return new ParsingError(node, L"A22: Cannot decide which function to call in multiple targets: " + description + L".");
			}

			Ptr<parsing::ParsingError> WfErrors::ClassContainsNoConstructor(WfExpression* node, reflection::description::ITypeInfo* type)
			{
				return new ParsingError(node, L"A23: Class \"" + type->GetTypeFriendlyName() + L"\" does not contain any constructor.");
			}

			Ptr<parsing::ParsingError> WfErrors::InterfaceContainsNoConstructor(WfExpression* node, reflection::description::ITypeInfo* type)
			{
				Ptr<ITypeInfo> proxy = TypeInfoRetriver<Ptr<IValueInterfaceProxy>>::CreateTypeInfo();
				return new ParsingError(node, L"A23: Interface \"" + type->GetTypeFriendlyName() + L"\" does not contain any constructor receiving an \"" + proxy->GetTypeFriendlyName() + L"\".");
			}

			Ptr<parsing::ParsingError> WfErrors::ConstructorReturnTypeMismatched(WfExpression* node, const ResolveExpressionResult& function, reflection::description::ITypeInfo* fromType, reflection::description::ITypeInfo* toType)
			{
				return new ParsingError(node, L"A24: The return type of " + function.GetFriendlyName() + L" cannot implicitly convert from \"" + fromType->GetTypeFriendlyName() + L"\" to \"" + toType->GetTypeFriendlyName() + L"\".");
			}

			Ptr<parsing::ParsingError> WfErrors::ExpressionIsNotLeftValue(WfExpression* node, const ResolveExpressionResult& result)
			{
				return new ParsingError(node, L"A25: Expression " + result.GetFriendlyName() + L" is not assignable.");
			}

			Ptr<parsing::ParsingError> WfErrors::WrongVoidType(WfType* node)
			{
				return new ParsingError(node, L"B0: Void is not a type for a value.");
			}

			Ptr<parsing::ParsingError> WfErrors::WrongInterfaceType(WfType* node)
			{
				return new ParsingError(node, L"B1: Interface is not a type for a value.");
			}

			Ptr<parsing::ParsingError> WfErrors::RawPointerToNonReferenceType(WfType* node, reflection::description::ITypeInfo* typeInfo)
			{
				return new ParsingError(node, L"B2: A raw pointer" + (typeInfo ? L" \"" + typeInfo->GetTypeFriendlyName() + L"\"" : L"") + L" should point to a class or interface.");
			}

			Ptr<parsing::ParsingError> WfErrors::SharedPointerToNonReferenceType(WfType* node, reflection::description::ITypeInfo* typeInfo)
			{
				return new ParsingError(node, L"B3: A shared pointer" + (typeInfo ? L" \"" + typeInfo->GetTypeFriendlyName() + L"\"" : L"") + L" should point to a class or interface.");
			}

			Ptr<parsing::ParsingError> WfErrors::NullableToNonReferenceType(WfType* node, reflection::description::ITypeInfo* typeInfo)
			{
				return new ParsingError(node, L"B4: A nullable value" + (typeInfo ? L" \"" + typeInfo->GetTypeFriendlyName() + L"\"" : L"") + L" should point to a struct.");
			}

			Ptr<parsing::ParsingError> WfErrors::ChildOfNonReferenceType(WfType* node)
			{
				return new ParsingError(node, L"B5: Only a reference type have child types.");
			}

			Ptr<parsing::ParsingError> WfErrors::TypeNotExists(WfType* node, Ptr<WfLexicalScopeName> scopeName)
			{
				return new ParsingError(node, L"B6: \"" + scopeName->GetFriendlyName() + L"\" is not a type.");
			}

			Ptr<parsing::ParsingError> WfErrors::TypeNotExists(WfType* node, Ptr<WfLexicalSymbol> symbol)
			{
				return new ParsingError(node, L"B6: \"" + symbol->name + L"\" is not a type.");
			}

			Ptr<parsing::ParsingError> WfErrors::TypeNotForValue(WfType* node, reflection::description::ITypeInfo* typeInfo)
			{
				return new ParsingError(node, L"B7: Type \"" + typeInfo->GetTypeFriendlyName() + L"\" is not a type for a value.");
			}

			Ptr<parsing::ParsingError> WfErrors::BreakNotInLoop(WfStatement* node)
			{
				return new ParsingError(node, L"C0: Break statement should appear in a loop.");
			}

			Ptr<parsing::ParsingError> WfErrors::ContinueNotInLoop(WfStatement* node)
			{
				return new ParsingError(node, L"C1: Continue statement should appear in a loop.");
			}

			Ptr<parsing::ParsingError> WfErrors::RethrowNotInCatch(WfStatement* node)
			{
				return new ParsingError(node, L"C2: Re-raise exception statement should appear in catch.");
			}

			Ptr<parsing::ParsingError> WfErrors::TryMissCatchAndFinally(WfStatement* node)
			{
				return new ParsingError(node, L"C3: Try statement should not appear without both catch and finally.");
			}

			Ptr<parsing::ParsingError> WfErrors::ReturnMissExpression(WfStatement* node, reflection::description::ITypeInfo* type)
			{
				return new ParsingError(node, L"C4: Return statement requires an expression of type \"" + type->GetTypeFriendlyName() + L"\".");
			}

			Ptr<parsing::ParsingError> WfErrors::DeleteNonRawPointer(WfStatement* node, reflection::description::ITypeInfo* type)
			{
				return new ParsingError(node, L"C5: Delete statement cannot apply on an expression of type \"" + type->GetTypeFriendlyName() + L"\", which is expected to be a raw pointer.");
			}

			Ptr<parsing::ParsingError> WfErrors::FunctionShouldHaveName(WfDeclaration* node)
			{
				return new ParsingError(node, L"D0: Function should have a name.");
			}

			Ptr<parsing::ParsingError> WfErrors::DuplicatedDeclaration(WfDeclaration* node, const WString& firstDeclarationCategory)
			{
				return new ParsingError(node, L"D1: Duplicated declaration \"" + node->name.value + L"\", it has already beed defined as a " + firstDeclarationCategory + L".");
			}

			Ptr<parsing::ParsingError> WfErrors::DuplicatedSymbol(WfDeclaration* node, Ptr<WfLexicalSymbol> symbol)
			{
				return new ParsingError(node, L"D2: Duplicated symbol \"" + symbol->name + L"\".");
			}

			Ptr<parsing::ParsingError> WfErrors::DuplicatedSymbol(WfFunctionArgument* node, Ptr<WfLexicalSymbol> symbol)
			{
				return new ParsingError(node, L"D2: Duplicated symbol \"" + symbol->name + L"\".");
			}

			Ptr<parsing::ParsingError> WfErrors::DuplicatedSymbol(WfStatement* node, Ptr<WfLexicalSymbol> symbol)
			{
				return new ParsingError(node, L"D2: Duplicated symbol \"" + symbol->name + L"\".");
			}

			Ptr<parsing::ParsingError> WfErrors::DuplicatedSymbol(WfExpression* node, Ptr<WfLexicalSymbol> symbol)
			{
				return new ParsingError(node, L"D2: Duplicated symbol \"" + symbol->name + L"\".");
			}

			Ptr<parsing::ParsingError> WfErrors::InterfaceMethodNotImplemented(WfNewTypeExpression* node, reflection::description::IMethodInfo* method)
			{
				ResolveExpressionResult result(method, CreateTypeInfoFromMethodInfo(method));
				return new ParsingError(node, L"D3: Interface method not implemented: " + result.GetFriendlyName() + L".");
			}

			Ptr<parsing::ParsingError> WfErrors::InterfaceMethodNotFound(WfFunctionDeclaration* node, reflection::description::ITypeInfo* interfaceType, reflection::description::ITypeInfo* methodType)
			{
				return new ParsingError(node, L"D4: Interface \"" + interfaceType->GetTypeFriendlyName() + L"\" does not contain method \"" + node->name.value + L"\" which is in type \"" + methodType->GetTypeFriendlyName() + L"\".");
			}

			Ptr<parsing::ParsingError> WfErrors::CannotPickOverloadedInterfaceMethods(WfExpression* node, collections::List<ResolveExpressionResult>& results)
			{
				WString description;
				FOREACH_INDEXER(ResolveExpressionResult, result, index, results)
				{
					description += L"\r\n\t";
					description += result.GetFriendlyName();
				}
				return new ParsingError(node, L"D5: Cannot decide which function to implement in multiple targets: " + description + L".");
			}

			Ptr<parsing::ParsingError> WfErrors::CannotPickOverloadedImplementMethods(WfFunctionDeclaration* node, reflection::description::ITypeInfo* type)
			{
				return new ParsingError(node, L"D6: There are some other methods named \"" + node->name.value + L"\" whose types are also \"" + type->GetTypeFriendlyName() + L"\".");
			}

			Ptr<parsing::ParsingError> WfErrors::WrongUsingPathWildCard(WfModuleUsingPath* node)
			{
				return new ParsingError(node, L"E0: Wild card \"*\" should only appear in the last item of the using path and should appear once.");
			}

			Ptr<parsing::ParsingError> WfErrors::TopQualifiedSymbolNotExists(parsing::ParsingTreeCustomBase* node, const WString& name)
			{
				return new ParsingError(node, L"F0: Top qualified symbol \"" + name + L"\" does not exist.");
			}

			Ptr<parsing::ParsingError> WfErrors::ChildSymbolNotExists(parsing::ParsingTreeCustomBase* node, Ptr<WfLexicalScopeName> scopeName, const WString& name)
			{
				return new ParsingError(node, L"F1: Symbol \"" + name + L"\" does not exist in \"" + scopeName->GetFriendlyName() + L"\".");
			}

			Ptr<parsing::ParsingError> WfErrors::MemberNotExists(parsing::ParsingTreeCustomBase* node, reflection::description::ITypeDescriptor* typeDescriptor, const WString& name)
			{
				return new ParsingError(node, L"F1: Member \"" + name + L"\" does not exist in \"" + typeDescriptor->GetTypeName() + L"\".");
			}

			Ptr<parsing::ParsingError> WfErrors::ReferenceNotExists(parsing::ParsingTreeCustomBase* node, const WString& name)
			{
				return new ParsingError(node, L"F2: Symbol \"" + name + L"\" does not exist in the current scope.");
			}

			Ptr<parsing::ParsingError> WfErrors::TooManyTargets(parsing::ParsingTreeCustomBase* node, collections::List<ResolveExpressionResult>& results, const WString& name)
			{
				WString description;
				FOREACH_INDEXER(ResolveExpressionResult, result, index, results)
				{
					description += L"\r\n\t";
					description += result.GetFriendlyName();
				}
				return new ParsingError(node, L"F3: Symbol \"" + name + L"\" references to too many targets: " + description + L".");
			}

			Ptr<parsing::ParsingError> WfErrors::TooManyTargets(parsing::ParsingTreeCustomBase* node, collections::List<Ptr<WfLexicalSymbol>>& symbols, const WString& name)
			{
				List<ResolveExpressionResult> results;
				CopyFrom(
					results,
					From(symbols)
					.Select([](Ptr<WfLexicalSymbol> symbol)
					{
						return ResolveExpressionResult(symbol, 0);
					}));
				return TooManyTargets(node, results, name);
			}

			Ptr<parsing::ParsingError> WfErrors::TooManyTargets(parsing::ParsingTreeCustomBase* node, collections::List<Ptr<WfLexicalScopeName>>& names, const WString& name)
			{
				List<ResolveExpressionResult> results;
				CopyFrom(
					results,
					From(names)
					.Select([](Ptr<WfLexicalScopeName> name)
					{
						return ResolveExpressionResult(name);
					}));
				return TooManyTargets(node, results, name);
			}
		}
	}
}