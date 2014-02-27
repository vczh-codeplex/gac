#include "WfAnalyzer.h"

namespace vl
{
	namespace workflow
	{
		namespace analyzer
		{
			using namespace collections;
			using namespace parsing;

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

			Ptr<parsing::ParsingError> WfErrors::ObserveNotInBind(WfExpression* node)
			{
				return new ParsingError(node, L"A3: Observe expression should appear in a bind expression.");
			}

			Ptr<parsing::ParsingError> WfErrors::BindInBind(WfExpression* node)
			{
				return new ParsingError(node, L"A4: Bind expression should not appear in another bind expression.");
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

			Ptr<parsing::ParsingError> WfErrors::ExpressionIsNotScopeName(WfExpression* node)
			{
				return new ParsingError(node, L"A8: Expression does not reference to a declaration.");
			}

			Ptr<parsing::ParsingError> WfErrors::ExpressionCannotResolveType(WfExpression* node, Ptr<WfLexicalSymbol> symbol)
			{
				return new ParsingError(node, L"A9: Expression referencing to symbol \"" + symbol->name + L"\" failed to resolve its type.");
			}

			Ptr<parsing::ParsingError> WfErrors::NullCannotResolveType(WfExpression* node)
			{
				return new ParsingError(node, L"A9: Expression \"null\" failed to resolve its type.");
			}

			Ptr<parsing::ParsingError> WfErrors::OrderedLambdaCannotResolveType(WfExpression* node)
			{
				return new ParsingError(node, L"A9: Ordered lambda expression failed to resolve its type.");
			}

			Ptr<parsing::ParsingError> WfErrors::NullCannotImplicitlyConvertToType(WfExpression* node, reflection::description::ITypeInfo* toType)
			{
				return new ParsingError(node, L"A10: Expression \"null\" cannot implicitly convert to \"" + toType->GetTypeFriendlyName() + L"\".");
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

			Ptr<parsing::ParsingError> WfErrors::DuplicatedSymbol(WfStatement* node, Ptr<WfLexicalSymbol> symbol)
			{
				return new ParsingError(node, L"D2: Duplicated symbol \"" + symbol->name + L"\".");
			}

			Ptr<parsing::ParsingError> WfErrors::DuplicatedSymbol(WfExpression* node, Ptr<WfLexicalSymbol> symbol)
			{
				return new ParsingError(node, L"D2: Duplicated symbol \"" + symbol->name + L"\".");
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

			Ptr<parsing::ParsingError> WfErrors::ReferenceNotExists(parsing::ParsingTreeCustomBase* node, const WString& name)
			{
				return new ParsingError(node, L"F2: Symbol \"" + name + L"\" does not exist in the current scope.");
			}

			Ptr<parsing::ParsingError> WfErrors::TooManyTargets(parsing::ParsingTreeCustomBase* node, collections::List<ResolveExpressionResult>& results, const WString& name)
			{
				WString description;
				FOREACH_INDEXER(ResolveExpressionResult, result, index, results)
				{
					if (index) description += L", ";
					if (result.scopeName)
					{
						description += result.scopeName->GetFriendlyName();
					}
					else if (result.symbol)
					{
						description += result.symbol->GetFriendlyName();
					}
					else if (result.type)
					{
						description += L"expression of type \"" + result.type->GetTypeFriendlyName() + L"\".";
					}
					else
					{
						description += L"<unknown>";
					}
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