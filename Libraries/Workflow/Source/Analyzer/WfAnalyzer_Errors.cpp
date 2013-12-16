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

			Ptr<parsing::ParsingError> WfErrors::WrongFormatStringSyntax(Ptr<WfExpression> node)
			{
				return new ParsingError(node.Obj(), L"A0: Wrong format string syntax.");
			}

			Ptr<parsing::ParsingError> WfErrors::WrongSimpleObserveExpression(Ptr<WfExpression> node)
			{
				return new ParsingError(node.Obj(), L"A1: Simple observe expression should observe a property under the observed object.");
			}

			Ptr<parsing::ParsingError> WfErrors::WrongSimpleObserveEvent(Ptr<WfExpression> node)
			{
				return new ParsingError(node.Obj(), L"A2: Simple observe expression should react to an event under the observed object.");
			}

			Ptr<parsing::ParsingError> WfErrors::ObserveNotInBind(Ptr<WfExpression> node)
			{
				return new ParsingError(node.Obj(), L"A3: Observe expression should appear in a bind expression.");
			}
		}
	}
}