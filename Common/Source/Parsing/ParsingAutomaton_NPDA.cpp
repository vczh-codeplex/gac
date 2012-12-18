#include "ParsingAutomaton.h"
#include "..\Collections\Operation.h"

namespace vl
{
	namespace parsing
	{
		using namespace collections;
		using namespace definitions;

		namespace analyzing
		{

/***********************************************************************
CreateNondeterministicPDAFromEpsilonPDA::state_clearning
***********************************************************************/

			namespace state_clearning
			{
				void DeleteUnnecessaryStates(Ptr<Automaton> automaton, Ptr<RuleInfo> newRuleInfo, List<State*>& newStates)
				{
					// delete all states that are not reachable to the end state
					while(true)
					{
						// find a non-end state without out transitions
						vint deleteCount=0;
						for(vint i=newStates.Count()-1;i>=0;i--)
						{
							State* newState=newStates[i];
							if(newState->transitions.Count()==0)
							{
								if(newState!=newRuleInfo->rootRuleEndState && !newState->endState)
								{
									automaton->DeleteState(newState);
									newStates.RemoveAt(i);
								}
							}
						}
						if(deleteCount==0)
						{
							break;
						}
					}
				}

				bool IsMergableCandidate(State* state, Ptr<RuleInfo> newRuleInfo)
				{
					return state!=newRuleInfo->rootRuleStartState && state!=newRuleInfo->rootRuleEndState && state!=newRuleInfo->startState;
				}

				vint CompareTransitionForRearranging(Transition* t1, Transition* t2)
				{
					if(t1->transitionType<t2->transitionType) return -1;
					if(t1->transitionType>t2->transitionType) return 1;
					if(t1->transitionSymbol<t2->transitionSymbol) return -1;
					if(t1->transitionSymbol>t2->transitionSymbol) return 1;
					if(t1->lookAheadSymbol<t2->lookAheadSymbol) return -1;
					if(t1->lookAheadSymbol>t2->lookAheadSymbol) return 1;
					return 0;
				}

				vint CompareActionForRearranging(Ptr<Action> a1, Ptr<Action> a2)
				{
					if(a1->actionType<a2->actionType) return -1;
					if(a1->actionType>a2->actionType) return 1;
					if(a1->actionSource<a2->actionSource) return -1;
					if(a1->actionSource>a2->actionSource) return 1;
					if(a1->actionTarget<a2->actionTarget) return -1;
					if(a1->actionTarget>a2->actionTarget) return 1;
					return 0;
				}

				void RearrangeState(State* state, SortedList<State*>& stateContentSorted)
				{
					if(!stateContentSorted.Contains(state))
					{
						FOREACH(Transition*, transition, state->transitions)
						{
							CopyFrom(transition->actions, transition->actions>>OrderBy(&CompareActionForRearranging));
						}
						CopyFrom(state->transitions, state->transitions>>OrderBy(&CompareTransitionForRearranging));
						stateContentSorted.Add(state);
					}
				}

				bool IsSameTransitionContent(Transition* t1, Transition* t2)
				{
					if(	t1->actions.Count()!=t2->actions.Count() ||
						t1->transitionType!=t2->transitionType ||
						t1->transitionSymbol!=t2->transitionSymbol ||
						t1->lookAheadSymbol!=t2->lookAheadSymbol)
					{
						return false;
					}
					for(vint j=0;j<t1->actions.Count();j++)
					{
						Ptr<Action> a1=t1->actions[j];
						Ptr<Action> a2=t2->actions[j];
						if(	a1->actionType!=a2->actionType ||
							a1->actionSource!=a2->actionSource ||
							a1->actionTarget!=a2->actionTarget)
						{
							return false;
						}
					}
					return true;
				}

				bool IsMergableBecauseTransitions(State* state1, State* state2)
				{
					if(state1->transitions.Count()!=state2->transitions.Count()) return false;
					if(state1->transitions.Count()==1 && state2->transitions.Count()==1)
					{
						Transition* t1=state1->transitions[0];
						Transition* t2=state2->transitions[0];
						if(CompareTransitionForRearranging(t1, t2)==0 && !IsSameTransitionContent(t1, t2) && t1->target==t2->target)
						{
							FOREACH(Transition*, t, state1->inputs)
							{
								CopyFrom(t->actions, t1->actions, true);
							}
							FOREACH(Transition*, t, state2->inputs)
							{
								CopyFrom(t->actions, t2->actions, true);
							}
							t1->actions.Clear();
							t2->actions.Clear();
						}
					}
					for(vint i=0;i<state1->transitions.Count();i++)
					{
						Transition* t1=state1->transitions[i];
						Transition* t2=state2->transitions[i];
						if(!IsSameTransitionContent(t1, t2) || t1->target!=t2->target)
						{
							return false;
						}
					}
					return true;
				}

				bool IsMergableBecauseInputs(State* state1, State* state2)
				{
					if(state1->inputs.Count()!=state2->inputs.Count()) return false;
					for(vint i=0;i<state1->inputs.Count();i++)
					{
						Transition* t1=state1->inputs[i];
						Transition* t2=state2->inputs[i];
						if(!IsSameTransitionContent(t1, t2) || t1->source!=t2->source)
						{
							return false;
						}
					}
					return true;
				}

				void MergeState2ToState1BecauseTransitions(Ptr<Automaton> automaton, State* state1, State* state2)
				{
					// modify state1's expression
					state1->stateExpression+=L"\r\n"+state2->stateExpression;

					// retarget state2's input to state1
					for(vint i=state2->inputs.Count()-1;i>=0;i--)
					{
						Transition* t2=state2->inputs[i];
						bool add=true;
						FOREACH(Transition*, t1, state1->inputs)
						{
							if(IsSameTransitionContent(t1, t2) && t1->source==t2->source)
							{
								add=false;
								break;
							}
						}
						if(add)
						{
							state1->inputs.Add(t2);
							t2->target=state1;
							state2->inputs.RemoveAt(i);
						}
					}

					automaton->DeleteState(state2);
				}

				void MergeState2ToState1BecauseInputs(Ptr<Automaton> automaton, State* state1, State* state2)
				{
					// modify state1's expression
					state1->stateExpression+=L"\r\n"+state2->stateExpression;

					// retarget state2's input to state1
					for(vint i=state2->transitions.Count()-1;i>=0;i--)
					{
						Transition* t2=state2->transitions[i];
						bool add=true;
						FOREACH(Transition*, t1, state1->transitions)
						{
							if(IsSameTransitionContent(t1, t2) && t1->target==t2->target)
							{
								add=false;
								break;
							}
						}
						if(add)
						{
							state1->transitions.Add(t2);
							t2->source=state1;
							state2->transitions.RemoveAt(i);
						}
					}

					automaton->DeleteState(state2);
				}

				void MergeStates(Ptr<Automaton> automaton, Ptr<RuleInfo> newRuleInfo, List<State*>& newStates)
				{
					SortedList<State*> stateContentSorted;
					while(true)
					{
						vint mergeCount=0;
						for(vint i=0;i<newStates.Count();i++)
						{
							State* state1=newStates[i];
							if(IsMergableCandidate(state1, newRuleInfo))
							{
								for(vint j=i+1;j<newStates.Count();j++)
								{
									State* state2=newStates[j];
									if(state1!=state2 && IsMergableCandidate(state2, newRuleInfo))
									{
										RearrangeState(state1, stateContentSorted);
										RearrangeState(state2, stateContentSorted);
										if(IsMergableBecauseTransitions(state1, state2))
										{
											MergeState2ToState1BecauseTransitions(automaton, state1, state2);
											newStates.RemoveAt(j);
											goto MERGED_STATES_PAIR;
										}
										else if(IsMergableBecauseInputs(state1, state2))
										{
											MergeState2ToState1BecauseInputs(automaton, state1, state2);
											newStates.RemoveAt(j);
											goto MERGED_STATES_PAIR;
										}
									}
								}
							}
						}
						break;
					MERGED_STATES_PAIR:
						continue;
					}
				}
			}
			using namespace state_clearning;

/***********************************************************************
CreateNondeterministicPDAFromEpsilonPDA
***********************************************************************/

			Ptr<Automaton> CreateNondeterministicPDAFromEpsilonPDA(Ptr<Automaton> epsilonPDA)
			{
				Ptr<Automaton> automaton=new Automaton(epsilonPDA->symbolManager);
				FOREACH(ParsingDefinitionRuleDefinition*, rule, epsilonPDA->ruleInfos.Keys())
				{
					// build new rule info
					Ptr<RuleInfo> ruleInfo=epsilonPDA->ruleInfos[rule];
					Ptr<RuleInfo> newRuleInfo=new RuleInfo;
					automaton->ruleInfos.Add(rule, newRuleInfo);

					newRuleInfo->rootRuleStartState=automaton->RootRuleStartState(rule);
					newRuleInfo->rootRuleEndState=automaton->RootRuleEndState(rule);
					newRuleInfo->startState=automaton->RuleStartState(rule);

					// build state mapping and state visiting tracking
					Dictionary<State*, State*> oldNewStateMap;
					List<State*> scanningStates;
					vint currentStateIndex=0;
					oldNewStateMap.Add(ruleInfo->rootRuleStartState, newRuleInfo->rootRuleStartState);
					oldNewStateMap.Add(ruleInfo->rootRuleEndState, newRuleInfo->rootRuleEndState);
					oldNewStateMap.Add(ruleInfo->startState, newRuleInfo->startState);
					// begin with a root rule state state
					scanningStates.Add(ruleInfo->rootRuleStartState);
					// remove epsilon transitions
					RemoveEpsilonTransitions(oldNewStateMap, scanningStates, automaton);

					List<State*> newStates;
					CopyFrom(newStates, oldNewStateMap.Values());
					DeleteUnnecessaryStates(automaton, newRuleInfo, newStates);
					MergeStates(automaton, newRuleInfo, newStates);
					FOREACH(State*, state, newStates)
					{
						if(state->endState)
						{
							newRuleInfo->endStates.Add(state);
						}
					}
				}
				return automaton;
			}
		}
	}
}