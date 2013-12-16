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
		}
	}
}