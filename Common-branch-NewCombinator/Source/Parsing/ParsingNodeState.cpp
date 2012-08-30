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
				,transitionToken(0)
			{
			}

			ParsingNodeTransition::~ParsingNodeTransition()
			{
			}

			void ParsingNodeTransition::AddBeforeAction(IParsingNodeAction* action)
			{
				ParsingNodeAction* nodeAction=new ParsingNodeAction;
				nodeAction->referencePosition=ParsingNodeAction::Before;
				nodeAction->action=action;
				actions.Add(nodeAction);
			}

			void ParsingNodeTransition::AddAfterAction(IParsingNodeAction* action)
			{
				ParsingNodeAction* nodeAction=new ParsingNodeAction;
				nodeAction->referencePosition=ParsingNodeAction::After;
				nodeAction->action=action;
				actions.Add(nodeAction);
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

			ParsingNodeTransition* ParsingNodeAutomaton::NewRule(ParsingNodeState* stateFrom, ParsingNodeState* stateTo, _Rule* transitionRule)
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

			ParsingNodeTransition* ParsingNodeAutomaton::NewToken(ParsingNodeState* stateFrom, ParsingNodeState* stateTo, _Token* transitionToken)
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
					SetResult(b, e);
				}

				void Visit(parsing_internal::_Token* node)override
				{
					ParsingNodeState *b=automaton.NewState(currentRule, node, ParsingNodeState::Before);
					ParsingNodeState *e=automaton.NewState(currentRule, node, ParsingNodeState::After);
					automaton.NewToken(b, e, node);
					SetResult(b, e);
				}

				void Visit(parsing_internal::_Rule* node)override
				{
					ParsingNodeState *b=automaton.NewState(currentRule, node, ParsingNodeState::Before);
					ParsingNodeState *e=automaton.NewState(currentRule, node, ParsingNodeState::After);
					automaton.NewRule(b, e, node);
					SetResult(b, e);
				}

				void Visit(parsing_internal::_Action* node)override
				{
					ParsingNodeState *b1=0, *e1=0;
					ParsingNodeState *b=automaton.NewState(currentRule, node, ParsingNodeState::Before);
					ParsingNodeState *e=automaton.NewState(currentRule, node, ParsingNodeState::After);
					Call(node->node, b1, e1);
					automaton.NewEpsilon(b, b1)->AddBeforeAction(node->action.Obj());
					automaton.NewEpsilon(e1, e)->AddAfterAction(node->action.Obj());
					SetResult(b, e);
				}
			};

			void CreateAutomaton(const RuleNode* rootRule, ParsingNodeAutomaton& automaton)
			{
				List<const RuleNode*> rules;
				SearchRulesFromRule(rootRule, rules, 0);

				RuleAutomatonBuilder ruleAutomatonBuilder(automaton);
				FOREACH(const RuleNode*, rule, rules.Wrap())
				{
					ruleAutomatonBuilder.ProcessRule(rule);
				}
			}
			
/***********************************************************************
CompressAutomaton
***********************************************************************/

			void CompressAutomaton(ParsingNodeAutomaton& automatonIn, ParsingNodeAutomaton& automatonOut)
			{
			}
			
/***********************************************************************
LogAutomaton
***********************************************************************/

			void LogParsingNodeStateForAutomaton(ParsingNodeState* state, stream::TextWriter& writer)
			{
				if(state->referenceNode==state->rule->node.Obj())
				{
					if(state->referencePosition==ParsingNodeState::Before)
					{
						writer.WriteString(L"$ ");
					}
					writer.WriteString(state->rule->name);
					if(state->referencePosition==ParsingNodeState::After)
					{
						writer.WriteString(L" $");
					}
					writer.WriteLine(L"");
				}
				else
				{
					LogGrammarFromRule(state->rule, false, state->referenceNode, state->referencePosition==ParsingNodeState::Before, writer);
				}
			}

			void LogAutomaton(ParsingNodeAutomaton& automaton, stream::TextWriter& writer)
			{
				FOREACH(Ptr<ParsingNodeState>, state, automaton.states.Wrap())
				{
					LogParsingNodeStateForAutomaton(state.Obj(), writer);
					FOREACH(ParsingNodeTransition*, transition, state->transitionOuts.Wrap())
					{
						writer.WriteString(L"    ");
						switch(transition->transitionCondition)
						{
						case ParsingNodeTransition::Rule:
							writer.WriteString(L"<RULE>");
							writer.WriteString(transition->transitionRule->rule->name);
							break;
						case ParsingNodeTransition::Token:
							writer.WriteString(L"<TOKEN>");
							writer.WriteString(transition->transitionToken->name);
							break;
						case ParsingNodeTransition::Epsilon:
							writer.WriteString(L"<EPSILON>");
							break;
						}
						writer.WriteString(L" => ");
						LogParsingNodeStateForAutomaton(transition->stateIn, writer);
						FOREACH(Ptr<ParsingNodeAction>, action, transition->actions.Wrap())
						{
							writer.WriteString(L"        ");
							switch(action->referencePosition)
							{
							case ParsingNodeAction::Before:
								writer.WriteString(L"<BEFORE>");
								break;
							case ParsingNodeAction::After:
								writer.WriteString(L"<AFTER>");
								break;
							}
							writer.WriteString(action->action->GetName());
							writer.WriteLine(L"");
						}
					}
					writer.WriteLine(L"");
				}
			}
		}
	}
}