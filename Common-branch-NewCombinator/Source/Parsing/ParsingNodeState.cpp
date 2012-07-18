#include "ParsingNodeState.h"

namespace vl
{
	using namespace collections;

	namespace parsing
	{
		namespace parsing_internal
		{

/***********************************************************************
ParsingNodeState
***********************************************************************/

			ParsingNodeState::ParsingNodeState()
				:rule(0)
				,referenceNode(0)
				,referencePosition(Before)
				,finalState(false)
			{
			}

			ParsingNodeState::~ParsingNodeState()
			{
			}

/***********************************************************************
ParsingNodeTransition
***********************************************************************/

			ParsingNodeTransition::ParsingNodeTransition()
				:stateIn(0)
				,stateOut(0)
				,transitionCondition(Epsilon)
				,transitionRule(0)
				,transitionToken(-1)
			{
			}

			ParsingNodeTransition::~ParsingNodeTransition()
			{
			}

/***********************************************************************
ParsingNodeAutomaton
***********************************************************************/

			ParsingNodeAutomaton::ParsingNodeAutomaton()
			{
			}

			ParsingNodeAutomaton::~ParsingNodeAutomaton()
			{
			}

			void ParsingNodeAutomaton::Clear()
			{
				states.Clear();
				transitions.Clear();
				ruleInfos.Clear();
			}

			ParsingNodeState* ParsingNodeAutomaton::NewState(const RuleNode* rule, ParsingNode* referenceNode, ParsingNodeState::ReferencePosition referencePosition)
			{
				ParsingNodeState* state=new ParsingNodeState;
				states.Add(state);

				state->rule=rule;
				state->referenceNode=referenceNode;
				state->referencePosition=referencePosition;

				return state;
			}

			void ParsingNodeAutomaton::SetBeginState(ParsingNodeState* state)
			{
				Ptr<ParsingNodeRuleInfo> ruleInfo=new ParsingNodeRuleInfo;
				ruleInfo->beginState=state;
				ruleInfos.Add(state->rule, ruleInfo);
			}

			void ParsingNodeAutomaton::SetFinalState(ParsingNodeState* state)
			{
				Ptr<ParsingNodeRuleInfo> ruleInfo=ruleInfos[state->rule];
				state->finalState=true;
				ruleInfo->finalStates.Add(state);
			}

			ParsingNodeTransition* ParsingNodeAutomaton::NewRule(ParsingNodeState* stateFrom, ParsingNodeState* stateTo, RuleNode* transitionRule)
			{
				ParsingNodeTransition* transition=new ParsingNodeTransition;
				transitions.Add(transition);
				stateFrom->transitionOuts.Add(transition);
				stateTo->transitionIns.Add(transition);

				transition->stateIn=stateFrom;
				transition->stateOut=stateTo;
				transition->transitionCondition=ParsingNodeTransition::Rule;
				transition->transitionRule=transitionRule;

				return transition;
			}

			ParsingNodeTransition* ParsingNodeAutomaton::NewToken(ParsingNodeState* stateFrom, ParsingNodeState* stateTo, vint transitionToken)
			{
				ParsingNodeTransition* transition=new ParsingNodeTransition;
				transitions.Add(transition);
				stateFrom->transitionOuts.Add(transition);
				stateTo->transitionIns.Add(transition);

				transition->stateIn=stateFrom;
				transition->stateOut=stateTo;
				transition->transitionCondition=ParsingNodeTransition::Token;
				transition->transitionToken=transitionToken;

				return transition;
			}

			ParsingNodeTransition* ParsingNodeAutomaton::NewEpsilon(ParsingNodeState* stateFrom, ParsingNodeState* stateTo)
			{
				ParsingNodeTransition* transition=new ParsingNodeTransition;
				transitions.Add(transition);
				stateFrom->transitionOuts.Add(transition);
				stateTo->transitionIns.Add(transition);

				transition->stateIn=stateFrom;
				transition->stateOut=stateTo;
				transition->transitionCondition=ParsingNodeTransition::Epsilon;

				return transition;
			}
			
/***********************************************************************
CreateAutomaton
***********************************************************************/

			class RuleAutomatonBuilder : public Object, public IParsingNodeVisitor
			{
			protected:
				ParsingNodeAutomaton&					automaton;

			public:
				RuleAutomatonBuilder(ParsingNodeAutomaton& _automaton)
					:automaton(_automaton)
				{
				}

				void Visit(parsing_internal::_Seq* node)override
				{
				}

				void Visit(parsing_internal::_Alt* node)override
				{
				}

				void Visit(parsing_internal::_Loop* node)override
				{
				}

				void Visit(parsing_internal::_Token* node)override
				{
				}

				void Visit(parsing_internal::_Rule* node)override
				{
				}

				void Visit(parsing_internal::_Action* node)override
				{
				}
			};

			void CreateAutomaton(RuleNode* rootRule, ParsingNodeAutomaton& automaton)
			{
				List<const RuleNode*> rules;
				SearchRulesFromRule(rootRule, rules);
			}
		}
	}
}