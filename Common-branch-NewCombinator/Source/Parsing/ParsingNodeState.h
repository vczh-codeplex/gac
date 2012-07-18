/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Parsing::Interface

Classes:
***********************************************************************/

#ifndef VCZH_PARSING_PARSINGNODESTATE
#define VCZH_PARSING_PARSINGNODESTATE

#include "ParsingNode.h"

namespace vl
{
	namespace parsing
	{
		namespace parsing_internal
		{
			class ParsingNodeState;
			class ParsingNodeTransition;

			class ParsingNodeState : public Object
			{
			public:
				enum ReferencePosition
				{
					Before,
					After,
				};

				RuleNode*									rule;
				ParsingNode*								referenceNode;
				ReferencePosition							referencePosition;
				collections::List<ParsingNodeTransition*>	transitionOuts;
				collections::List<ParsingNodeTransition*>	transitionIns;
			};

			class ParsingNodeTransition : public Object
			{
			public:
				enum TransitionCondition
				{
					Rule,
					Token,
					Epsilon,
				};

				ParsingNodeState*							stateIn;
				ParsingNodeState*							stateOut;
				TransitionCondition							transitionCondition;
				RuleNode*									transitionRule;
				vint										transitionToken;
			};
		}
	}
}

#endif