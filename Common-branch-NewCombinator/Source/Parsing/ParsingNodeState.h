/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Parsing::Interface

Classes:
***********************************************************************/

#ifndef VCZH_PARSING_PARSINGNODESTATE
#define VCZH_PARSING_PARSINGNODESTATE

#include "ParsingNode.h"
#include "..\Collections\Dictionary.h"

namespace vl
{
	namespace parsing
	{
		namespace parsing_internal
		{
			class ParsingNodeState;
			class ParsingNodeTransition;

/***********************************************************************
文法节点自动机
***********************************************************************/

			class ParsingNodeState : public Object
			{
			public:
				enum ReferencePosition
				{
					Before,
					After,
				};

				const RuleNode*									rule;
				ParsingNode*									referenceNode;
				ReferencePosition								referencePosition;
				collections::List<ParsingNodeTransition*>		transitionOuts;
				collections::List<ParsingNodeTransition*>		transitionIns;
				bool											finalState;

				ParsingNodeState();
				~ParsingNodeState();
			};

			class ParsingNodeAction : public Object
			{
			public:
				enum ReferencePosition
				{
					Before,
					After,
				};
				
				ReferencePosition								referencePosition;
				IParsingNodeAction*								action;
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

				ParsingNodeState*								stateIn;
				ParsingNodeState*								stateOut;
				TransitionCondition								transitionCondition;
				_Rule*											transitionRule;
				_Token*											transitionToken;
				collections::List<Ptr<ParsingNodeAction>>		actions;

				ParsingNodeTransition();
				~ParsingNodeTransition();

				void											AddBeforeAction(IParsingNodeAction* action);
				void											AddAfterAction(IParsingNodeAction* action);
			};

			class ParsingNodeRuleInfo : public Object
			{
			public:
				ParsingNodeState*								beginState;
				collections::List<ParsingNodeState*>			finalStates;
			};

			class ParsingNodeAutomaton : public Object
			{
				typedef collections::Dictionary<const RuleNode*, Ptr<ParsingNodeRuleInfo>>	RuleInfoMap;
			public:
				collections::List<Ptr<ParsingNodeState>>		states;
				collections::List<Ptr<ParsingNodeTransition>>	transitions;
				RuleInfoMap										ruleInfos;

				ParsingNodeAutomaton();
				~ParsingNodeAutomaton();

				void											Clear();
				ParsingNodeState*								NewState(const RuleNode* rule, ParsingNode* referenceNode, ParsingNodeState::ReferencePosition referencePosition);
				void											SetBeginState(ParsingNodeState* state);
				void											SetFinalState(ParsingNodeState* state);
				ParsingNodeTransition*							NewRule(ParsingNodeState* stateFrom, ParsingNodeState* stateTo, _Rule* transitionRule);
				ParsingNodeTransition*							NewToken(ParsingNodeState* stateFrom, ParsingNodeState* stateTo, _Token* transitionToken);
				ParsingNodeTransition*							NewEpsilon(ParsingNodeState* stateFrom, ParsingNodeState* stateTo);
			};

/***********************************************************************
辅助函数
***********************************************************************/

			extern void CreateAutomaton(const RuleNode* rootRule, ParsingNodeAutomaton& automaton);
			extern void CompressAutomaton(ParsingNodeAutomaton& automatonIn, ParsingNodeAutomaton& automatonOut);
			extern void LogAutomaton(ParsingNodeAutomaton& automaton, stream::TextWriter& writer);
		}
	}
}

#endif