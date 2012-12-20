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
RemoveEpsilonTransitions
***********************************************************************/

			Ptr<Automaton> CreateJointPDAFromNondeterministicPDA(Ptr<Automaton> nondeterministicPDA)
			{
				Ptr<Automaton> automaton=new Automaton(nondeterministicPDA->symbolManager);
				Dictionary<State*, State*> oldNewStateMap;
				FOREACH(Ptr<State>, oldState, nondeterministicPDA->states)
				{
					State* newState=automaton->CopyState(oldState.Obj());
					oldNewStateMap.Add(oldState.Obj(), newState);
				}

				// build rule info data
				Dictionary<WString, ParsingDefinitionRuleDefinition*> ruleMap;
				FOREACH(ParsingDefinitionRuleDefinition*, rule, nondeterministicPDA->ruleInfos.Keys())
				{
					// build new rule info
					Ptr<RuleInfo> ruleInfo=nondeterministicPDA->ruleInfos[rule];
					Ptr<RuleInfo> newRuleInfo=new RuleInfo;
					automaton->ruleInfos.Add(rule, newRuleInfo);
					ruleMap.Add(rule->name, rule);

					// rebuild new rule info data
					newRuleInfo->rootRuleStartState=oldNewStateMap[ruleInfo->rootRuleStartState];
					newRuleInfo->rootRuleEndState=oldNewStateMap[ruleInfo->rootRuleEndState];
					newRuleInfo->startState=oldNewStateMap[ruleInfo->startState];
					FOREACH(State*, endState, ruleInfo->endStates)
					{
						newRuleInfo->endStates.Add(oldNewStateMap[endState]);
					}
				}

				// create transitions
				FOREACH(ParsingDefinitionRuleDefinition*, rule, nondeterministicPDA->ruleInfos.Keys())
				{
					Ptr<RuleInfo> ruleInfo=nondeterministicPDA->ruleInfos[rule];

					// create joint transitions according to old automaton
					List<State*> scanningStates;
					vint currentStateIndex=0;
					scanningStates.Add(ruleInfo->rootRuleStartState);

					while(currentStateIndex<scanningStates.Count())
					{
						State* currentOldState=scanningStates[currentStateIndex++];
						State* currentNewState=oldNewStateMap[currentOldState];
						FOREACH(Transition*, oldTransition, currentOldState->transitions)
						{
							State* oldSource=oldTransition->source;
							State* oldTarget=oldTransition->target;
							State* newSource=oldNewStateMap[oldSource];
							State* newTarget=oldNewStateMap[newTarget];

							if(oldTransition->transitionType==Transition::Symbol && oldTransition->transitionSymbol->GetType()==ParsingSymbol::RuleDef)
							{
								// if this is a rule transition, create
								// source -> ruleStart
								// ruleEnd[] -> target
								ParsingDefinitionRuleDefinition* rule=ruleMap[oldTransition->transitionSymbol->GetName()];
								Ptr<RuleInfo> oldRuleInfo=nondeterministicPDA->ruleInfos[rule];

								{
									Transition* shiftTransition=automaton->Epsilon(newSource, oldNewStateMap[oldRuleInfo->startState]);
									Ptr<Action> action=new Action;
									action->actionType=Action::Shift;
									shiftTransition->actions.Add(action);
								}

								FOREACH(State*, oldEndState, oldRuleInfo->endStates)
								{
									Transition* reduceTransition=automaton->Epsilon(oldNewStateMap[oldEndState], newTarget);
									Ptr<Action> action=new Action;
									action->actionType=Action::Reduce;
									reduceTransition->actions.Add(action);
									CopyFrom(reduceTransition->actions, oldTransition->actions, true);
									reduceTransition->stackPattern.Add(newSource);
								}
							}
							else
							{
								// if not, just copy
								Transition* newTransition=automaton->CopyTransition(newSource, newTarget, oldTransition);
								CopyFrom(newTransition->actions, oldTransition->actions);
							}
						}
					}
				}
				return automaton;
			}
		}
	}
}