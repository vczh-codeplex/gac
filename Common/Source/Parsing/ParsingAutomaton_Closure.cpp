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
CreateNondeterministicPDAFromEpsilonPDA::closure_searching
***********************************************************************/

			namespace closure_searching
			{
				struct ClosureItem
				{
					State*						state;			// target state of one path of a closure
					Ptr<List<Transition*>>		transitions;	// path

					ClosureItem()
						:state(0)
					{
					}

					ClosureItem(State* _state, Ptr<List<Transition*>> _transitions)
						:state(_state)
						,transitions(_transitions)
					{
					}
				};

				enum ClosureSearchResult
				{
					Continue,
					Hit,
					Blocked,
				};

				// closure function for searching <epsilon* symbol> reachable states
				ClosureSearchResult EpsilonClosure(Transition* transition)
				{
					return
						transition->transitionType!=Transition::Epsilon?Hit:
						transition->target->endState?Blocked:
						Continue;
				}

				// closure function for searching <epsilon*> reachable end states
				ClosureSearchResult EndStateClosure(Transition* transition)
				{
					return
						transition->transitionType!=Transition::Epsilon?Blocked:
						transition->target->endState?Hit:
						Continue;
				}

				// closure searching function
				void SearchClosureInternal(ClosureSearchResult(*closurePredicate)(Transition*), List<Transition*>& transitionPath, Transition* transition, State* state, List<ClosureItem>& closure)
				{
					ClosureSearchResult result=transition?closurePredicate(transition):Continue;
					switch(result)
					{
					case Continue:
						{
							FOREACH(Transition*, newTransition, state->transitions)
							{
								if(!transitionPath.Contains(newTransition))
								{
									transitionPath.Add(newTransition);
									SearchClosureInternal(closurePredicate, transitionPath, newTransition, newTransition->target, closure);
									transitionPath.RemoveAt(transitionPath.Count()-1);
								}
							}
						}
						break;
					case Hit:
						{
							Ptr<List<Transition*>> path=new List<Transition*>;
							CopyFrom(*path.Obj(), transitionPath);
							closure.Add(ClosureItem(state, path));
						}
						break;
					}
				}

				void SearchClosure(ClosureSearchResult(*closurePredicate)(Transition*), State* startState, List<ClosureItem>& closure)
				{
					List<Transition*> transitionPath;
					SearchClosureInternal(closurePredicate, transitionPath, 0, startState, closure);
				}

				// map old state to new state and track all states that are not visited yet
				State* GetMappedState(Ptr<Automaton> newAutomaton, State* oldState, List<State*>& scanningStates, Dictionary<State*, State*>& oldNewStateMap)
				{
					State* newState=0;
					vint mapIndex=oldNewStateMap.Keys().IndexOf(oldState);
					if(mapIndex==-1)
					{
						newState=newAutomaton->CopyState(oldState);
						oldNewStateMap.Add(oldState, newState);
						scanningStates.Add(oldState);
					}
					else
					{
						newState=oldNewStateMap.Values().Get(mapIndex);
					}
					return newState;
				}
			}
			using namespace closure_searching;

/***********************************************************************
RemoveEpsilonTransitions
***********************************************************************/

			void RemoveEpsilonTransitions(collections::Dictionary<State*, State*>& oldNewStateMap, collections::List<State*>& scanningStates, Ptr<Automaton> automaton)
			{
				vint currentStateIndex=0;
				Dictionary<State*, Ptr<List<ClosureItem>>> endStateClosures;

				while(currentStateIndex<scanningStates.Count())
				{
					// map visiting state to new state
					State* currentOldState=scanningStates[currentStateIndex++];
					State* currentNewState=GetMappedState(automaton, currentOldState, scanningStates, oldNewStateMap);

					// search for epsilon closure
					List<ClosureItem> closure;
					SearchClosure(&EpsilonClosure, currentOldState, closure);
					FOREACH(ClosureItem, closureItem, closure)
					{
						// search for end state closure of the target state of each path in the closure
						Ptr<List<ClosureItem>> endStateClosure;
						vint endStateIndex=endStateClosures.Keys().IndexOf(closureItem.state);
						if(endStateIndex==-1)
						{
							endStateClosure=new List<ClosureItem>;
							SearchClosure(&EndStateClosure, closureItem.state, *endStateClosure.Obj());
							endStateClosures.Add(closureItem.state, endStateClosure);
						}
						else
						{
							endStateClosure=endStateClosures.Values().Get(endStateIndex);
						}

						// build compacted non-epsilon transition to the target state of the path
						Transition* oldTransition=closureItem.transitions->Get(closureItem.transitions->Count()-1);
						{
							State* newEndState=GetMappedState(automaton, oldTransition->target, scanningStates, oldNewStateMap);
							Transition* transition=automaton->CopyTransition(currentNewState, newEndState, oldTransition);
							FOREACH(Transition*, pathTransition, *closureItem.transitions.Obj())
							{
								CopyFrom(transition->actions, pathTransition->actions, true);
							}
						}

						// if there is any path in the end state closure of the target state
						FOREACH(ClosureItem, endStateClosureItem, *endStateClosure.Obj())
						{
							// build a transition to the mapped end state
							State* newEndState=GetMappedState(automaton, endStateClosureItem.state, scanningStates, oldNewStateMap);
							Transition* transition=automaton->CopyTransition(currentNewState, newEndState, oldTransition);
							// copy all actions in the compacted non-epsilon transition
							FOREACH(Transition*, pathTransition, *closureItem.transitions.Obj())
							{
								CopyFrom(transition->actions, pathTransition->actions, true);
							}
							// copy all actions in the end state path
							FOREACH(Transition*, pathTransition, *endStateClosureItem.transitions.Obj())
							{
								CopyFrom(transition->actions, pathTransition->actions, true);
							}
						}
					}
				}
			}
		}
	}
}