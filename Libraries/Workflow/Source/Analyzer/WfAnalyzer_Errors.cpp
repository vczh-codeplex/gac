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

			Ptr<parsing::ParsingError> WfErrors::WrongVoidType(WfType* node)
			{
				return new ParsingError(node, L"B0: Void is not a type for a value.");
			}

			Ptr<parsing::ParsingError> WfErrors::WrongNamespaceType(WfType* node)
			{
				return new ParsingError(node, L"B1: Namespace is not a type for a value.");
			}

			Ptr<parsing::ParsingError> WfErrors::WrongInterfaceType(WfType* node)
			{
				return new ParsingError(node, L"B2: Interface is not a type for a value.");
			}

			Ptr<parsing::ParsingError> WfErrors::RawPointerToNonReferenceType(WfType* node)
			{
				return new ParsingError(node, L"B3: A raw pointer should pointing to a reference type.");
			}

			Ptr<parsing::ParsingError> WfErrors::SharedPointerToNonReferenceType(WfType* node)
			{
				return new ParsingError(node, L"B4: A shared pointer should pointing to a reference type.");
			}

			Ptr<parsing::ParsingError> WfErrors::NullableToNonReferenceType(WfType* node)
			{
				return new ParsingError(node, L"B5: A nullable value should pointing to a reference type.");
			}
		}
	}
}