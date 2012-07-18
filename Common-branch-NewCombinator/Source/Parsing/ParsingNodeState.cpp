#include "ParsingNodeState.h"
#include "..\Collections\OperationForEach.h"

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
				const RuleNode*							currentRule;
				ParsingNodeState*						resultBeginState;
				ParsingNodeState*						resultEndState;

				void SetResult(ParsingNodeState* beginState, ParsingNodeState* endState)
				{
					resultBeginState=beginState;
					resultEndState=endState;
				}

				void Call(Ptr<ParsingNode> node, ParsingNodeState*& beginState, ParsingNodeState*& endState)
				{
					resultBeginState=0;
					resultEndState=0;
					node->Accept(this);
					beginState=resultBeginState;
					endState=resultEndState;
				}
			public:
				RuleAutomatonBuilder(ParsingNodeAutomaton& _automaton)
					:automaton(_automaton)
					,resultBeginState(0)
					,resultEndState(0)
				{
				}

				void ProcessRule(const RuleNode* rule)
				{
					currentRule=rule;
					ParsingNodeState* beginState=0;
					ParsingNodeState* endState=0;
					Call(rule->node, beginState, endState);
					automaton.SetBeginState(beginState);
					automaton.SetFinalState(endState);
				}

				void Visit(parsing_internal::_Seq* node)override
				{
					ParsingNodeState *b1=0, *e1=0, *b2=0, *e2=0;
					Call(node->first, b1, e1);
					Call(node->second, b2, e2);
					automaton.NewEpsilon(e1, b2);
					SetResult(b1, e2);
				}

				void Visit(parsing_internal::_Alt* node)override
				{
					ParsingNodeState *b1=0, *e1=0, *b2=0, *e2=0;
					ParsingNodeState *b=automaton.NewState(currentRule, node, ParsingNodeState::Before);
					ParsingNodeState *e=automaton.NewState(currentRule, node, ParsingNodeState::After);
					Call(node->first, b1, e1);
					Call(node->second, b2, e2);
					automaton.NewEpsilon(b, b1);
					automaton.NewEpsilon(b, b2);
					automaton.NewEpsilon(e1, e);
					automaton.NewEpsilon(e2, e);
					SetResult(b, e);
				}

				void Visit(parsing_internal::_Loop* node)override
				{
					ParsingNodeState *b1=0, *e1=0;
					ParsingNodeState *b=automaton.NewState(currentRule, node, ParsingNodeState::Before);
					ParsingNodeState *e=automaton.NewState(currentRule, node, ParsingNodeState::After);
					Call(node->node, b1, e1);
					automaton.NewEpsilon(b, b1);
					automaton.NewEpsilon(e1, e);
					automaton.NewEpsilon(e1, b1);
					SetResult(b, e);
				}

				void Visit(parsing_internal::_Token* node)override
				{
					ParsingNodeState *b=automaton.NewState(currentRule, node, ParsingNodeState::Before);
					ParsingNodeState *e=automaton.NewState(currentRule, node, ParsingNodeState::After);
					automaton.NewToken(b, e, node->token);
					SetResult(b, e);
				}

				void Visit(parsing_internal::_Rule* node)override
				{
					ParsingNodeState *b=automaton.NewState(currentRule, node, ParsingNodeState::Before);
					ParsingNodeState *e=automaton.NewState(currentRule, node, ParsingNodeState::After);
					automaton.NewRule(b, e, node->rule);
					SetResult(b, e);
				}

				void Visit(parsing_internal::_Action* node)override
				{
					node->node->Accept(this);
				}
			};

			void CreateAutomaton(const RuleNode* rootRule, ParsingNodeAutomaton& automaton)
			{
				List<const RuleNode*> rules;
				SearchRulesFromRule(rootRule, rules);

				RuleAutomatonBuilder ruleAutomatonBuilder(automaton);
				FOREACH(const RuleNode*, rule, rules.Wrap())
				{
					ruleAutomatonBuilder.ProcessRule(rule);
				}
			}
		}
	}
}