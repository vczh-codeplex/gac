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
Action
***********************************************************************/

			Action::Action()
				:actionType(Create)
				,actionSource(0)
				,actionTarget(0)
			{
			}

			Action::~Action()
			{
			}

/***********************************************************************
Transition
***********************************************************************/

			Transition::Transition()
				:source(0)
				,target(0)
				,transitionType(Epsilon)
				,transitionSymbol(0)
				,lookAheadSymbol(0)
			{
			}

			Transition::~Transition()
			{
			}

/***********************************************************************
State
***********************************************************************/

			State::State()
				:ownerRule(0)
				,ownerRuleSymbol(0)
				,grammarNode(0)
				,stateNode(0)
				,statePosition(BeforeNode)
				,endState(false)
			{
			}

			State::~State()
			{
			}

/***********************************************************************
RuleInfo
***********************************************************************/

			RuleInfo::RuleInfo()
				:rootRuleStartState(0)
				,rootRuleEndState(0)
				,startState(0)
				,stateNameCount(0)
			{
			}

			RuleInfo::~RuleInfo()
			{
			}

/***********************************************************************
Automaton
***********************************************************************/

			Automaton::Automaton(ParsingSymbolManager* _symbolManager)
				:symbolManager(_symbolManager)
			{
			}

			Automaton::~Automaton()
			{
			}

			State* Automaton::RuleStartState(definitions::ParsingDefinitionRuleDefinition* ownerRule)
			{
				State* state=new State;
				states.Add(state);

				state->ownerRule=ownerRule;
				state->ownerRuleSymbol=symbolManager->GetGlobal()->GetSubSymbolByName(ownerRule->name);
				state->stateName=ownerRule->name+L".Start";
				state->stateExpression=L"¡¤ <"+ownerRule->name+L">";
				return state;
			}

			State* Automaton::RootRuleStartState(definitions::ParsingDefinitionRuleDefinition* ownerRule)
			{
				State* state=new State;
				states.Add(state);

				state->ownerRule=ownerRule;
				state->ownerRuleSymbol=symbolManager->GetGlobal()->GetSubSymbolByName(ownerRule->name);
				state->stateName=ownerRule->name+L".RootStart";
				state->stateExpression=L"¡¤ $<"+ownerRule->name+L">";
				return state;
			}

			State* Automaton::RootRuleEndState(definitions::ParsingDefinitionRuleDefinition* ownerRule)
			{
				State* state=new State;
				states.Add(state);

				state->ownerRule=ownerRule;
				state->ownerRuleSymbol=symbolManager->GetGlobal()->GetSubSymbolByName(ownerRule->name);
				state->stateName=ownerRule->name+L".RootEnd";
				state->stateExpression=L"$<"+ownerRule->name+L"> ¡¤";
				return state;
			}

			State* Automaton::StartState(definitions::ParsingDefinitionRuleDefinition* ownerRule, definitions::ParsingDefinitionGrammar* grammarNode, definitions::ParsingDefinitionGrammar* stateNode)
			{
				State* state=new State;
				states.Add(state);

				state->ownerRule=ownerRule;
				state->ownerRuleSymbol=symbolManager->GetGlobal()->GetSubSymbolByName(ownerRule->name);
				state->grammarNode=grammarNode;
				state->stateNode=stateNode;
				state->statePosition=State::BeforeNode;
				state->stateName=ownerRule->name+L"."+itow(++ruleInfos[ownerRule]->stateNameCount);
				stateNode=FindAppropriateGrammarState(grammarNode, stateNode, true);
				state->stateExpression=L"<"+ownerRule->name+L">: "+GrammarStateToString(grammarNode, stateNode, true);
				return state;
			}

			State* Automaton::EndState(definitions::ParsingDefinitionRuleDefinition* ownerRule, definitions::ParsingDefinitionGrammar* grammarNode, definitions::ParsingDefinitionGrammar* stateNode)
			{
				State* state=new State;
				states.Add(state);

				state->ownerRule=ownerRule;
				state->ownerRuleSymbol=symbolManager->GetGlobal()->GetSubSymbolByName(ownerRule->name);
				state->grammarNode=grammarNode;
				state->stateNode=stateNode;
				state->statePosition=State::AfterNode;
				state->stateName=ownerRule->name+L"."+itow(++ruleInfos[ownerRule]->stateNameCount);
				stateNode=FindAppropriateGrammarState(grammarNode, stateNode, false);
				state->stateExpression=L"<"+ownerRule->name+L">: "+GrammarStateToString(grammarNode, stateNode, false);
				return state;
			}

			State* Automaton::CopyState(State* oldState)
			{
				State* resultState=0;
				if(oldState->statePosition==State::BeforeNode)
				{
					resultState=StartState(oldState->ownerRule, oldState->grammarNode, oldState->stateNode);
				}
				else
				{
					resultState=EndState(oldState->ownerRule, oldState->grammarNode, oldState->stateNode);
				}
				resultState->endState=oldState->endState;
				return resultState;
			}

			Transition* Automaton::CreateTransition(State* start, State* end)
			{
				Transition* transition=new Transition;
				transitions.Add(transition);

				start->transitions.Add(transition);
				end->inputs.Add(transition);

				transition->source=start;
				transition->target=end;
				return transition;
			}

			Transition* Automaton::TokenBegin(State* start, State* end)
			{
				Transition* transition=CreateTransition(start, end);
				transition->transitionType=Transition::TokenBegin;
				return transition;
			}

			Transition* Automaton::TokenFinish(State* start, State* end)
			{
				Transition* transition=CreateTransition(start, end);
				transition->transitionType=Transition::TokenFinish;
				return transition;
			}

			Transition* Automaton::Epsilon(State* start, State* end)
			{
				Transition* transition=CreateTransition(start, end);
				transition->transitionType=Transition::Epsilon;
				return transition;
			}

			Transition* Automaton::Symbol(State* start, State* end, ParsingSymbol* transitionSymbol)
			{
				Transition* transition=CreateTransition(start, end);
				transition->transitionType=Transition::Symbol;
				transition->transitionSymbol=transitionSymbol;
				return transition;
			}

			Transition* Automaton::Symbol(State* start, State* end, const WString& transitionText)
			{
				Transition* transition=CreateTransition(start, end);
				transition->transitionType=Transition::Symbol;
				transition->transitionText=transitionText;
				return transition;
			}

			Transition* Automaton::CopyTransition(State* start, State* end, Transition* oldTransition)
			{
				Transition* transition=CreateTransition(start, end);
				transition->transitionType=oldTransition->transitionType;
				transition->transitionSymbol=oldTransition->transitionSymbol;
				transition->transitionText=oldTransition->transitionText;
				return transition;
			}

/***********************************************************************
CreateEpsilonPDA
***********************************************************************/

			class CreateEpsilonPDAVisitor : public Object, public ParsingDefinitionGrammar::IVisitor
			{
			public:
				Ptr<Automaton>						automaton;
				ParsingDefinitionGrammar*			ruleGrammar;
				State*								startState;
				State*								endState;
				Transition*							result;

				CreateEpsilonPDAVisitor(Ptr<Automaton> _automaton, ParsingDefinitionGrammar* _ruleGrammar, State* _startState, State* _endState)
					:automaton(_automaton)
					,ruleGrammar(_ruleGrammar)
					,startState(_startState)
					,endState(_endState)
					,result(0)
				{
				}

				static Transition* Create(ParsingDefinitionGrammar* grammar, Ptr<Automaton> automaton, ParsingDefinitionGrammar* ruleGrammar, State* startState, State* endState)
				{
					CreateEpsilonPDAVisitor visitor(automaton, ruleGrammar, startState, endState);
					grammar->Accept(&visitor);
					return visitor.result;
				}

				Transition* Create(ParsingDefinitionGrammar* grammar, State* startState, State* endState)
				{
					return Create(grammar, automaton, ruleGrammar, startState, endState);
				}

				void Visit(ParsingDefinitionPrimitiveGrammar* node)override
				{
					result=automaton->Symbol(startState, endState, automaton->symbolManager->CacheGetSymbol(node));
				}

				void Visit(ParsingDefinitionTextGrammar* node)override
				{
					result=automaton->Symbol(startState, endState, node->text);
				}

				void Visit(ParsingDefinitionSequenceGrammar* node)override
				{
					State* middleState=automaton->EndState(startState->ownerRule, ruleGrammar, node->first.Obj());
					Create(node->first.Obj(), startState, middleState);
					Create(node->second.Obj(), middleState, endState);
				}

				void Visit(ParsingDefinitionAlternativeGrammar* node)override
				{
					Create(node->first.Obj(), startState, endState);
					Create(node->second.Obj(), startState, endState);
				}

				void Visit(ParsingDefinitionLoopGrammar* node)override
				{
					State* loopStart=automaton->StartState(startState->ownerRule, ruleGrammar, node->grammar.Obj());
					automaton->Epsilon(startState, loopStart);
					automaton->Epsilon(loopStart, endState);
					Create(node->grammar.Obj(), loopStart, loopStart);
				}

				void Visit(ParsingDefinitionOptionalGrammar* node)override
				{
					Create(node->grammar.Obj(), startState, endState);
					automaton->Epsilon(startState, endState);
				}

				void Visit(ParsingDefinitionCreateGrammar* node)override
				{
					State* middleState=automaton->EndState(startState->ownerRule, ruleGrammar, node->grammar.Obj());
					Create(node->grammar.Obj(), startState, middleState);
					Transition* transition=automaton->Epsilon(middleState, endState);

					Ptr<Action> action=new Action;
					action->actionType=Action::Create;
					action->actionSource=automaton->symbolManager->CacheGetType(node->type.Obj(), 0);
					transition->actions.Add(action);
				}

				void Visit(ParsingDefinitionAssignGrammar* node)override
				{
					Transition* transition=Create(node->grammar.Obj(), startState, endState);

					Ptr<Action> action=new Action;
					action->actionType=Action::Assign;
					action->actionSource=automaton->symbolManager->CacheGetSymbol(node);
					transition->actions.Add(action);
				}

				void Visit(ParsingDefinitionUseGrammar* node)override
				{
					Transition* transition=Create(node->grammar.Obj(), startState, endState);

					Ptr<Action> action=new Action;
					action->actionType=Action::Using;
					transition->actions.Add(action);
				}

				void Visit(ParsingDefinitionSetterGrammar* node)override
				{
					State* middleState=automaton->EndState(startState->ownerRule, ruleGrammar, node->grammar.Obj());
					Create(node->grammar.Obj(), startState, middleState);
					Transition* transition=automaton->Epsilon(middleState, endState);

					Ptr<Action> action=new Action;
					action->actionType=Action::Setter;
					action->actionSource=automaton->symbolManager->CacheGetSymbol(node);
					action->actionTarget=action->actionSource->GetDescriptorSymbol()->GetSubSymbolByName(node->value);
					transition->actions.Add(action);
				}
			};

			void CreateEpsilonPDA(Ptr<Automaton> automaton, Ptr<definitions::ParsingDefinitionRuleDefinition> rule, ParsingSymbolManager* manager)
			{
				Ptr<RuleInfo> ruleInfo=new RuleInfo;
				automaton->ruleInfos.Add(rule.Obj(), ruleInfo);

				ruleInfo->rootRuleStartState=automaton->RootRuleStartState(rule.Obj());
				ruleInfo->rootRuleEndState=automaton->RootRuleEndState(rule.Obj());
				ruleInfo->startState=automaton->RuleStartState(rule.Obj());
				automaton->Epsilon(ruleInfo->rootRuleStartState, ruleInfo->startState);

				FOREACH(Ptr<ParsingDefinitionGrammar>, grammar, rule->grammars)
				{
					State* grammarStartState=automaton->StartState(rule.Obj(), grammar.Obj(), grammar.Obj());
					State* grammarEndState=automaton->EndState(rule.Obj(), grammar.Obj(), grammar.Obj());
					grammarEndState->stateName+=L".End";
					grammarEndState->endState=true;
					automaton->TokenBegin(ruleInfo->startState, grammarStartState);
					automaton->TokenFinish(grammarEndState, ruleInfo->rootRuleEndState);
					ruleInfo->endStates.Add(grammarEndState);
					CreateEpsilonPDAVisitor::Create(grammar.Obj(), automaton, grammar.Obj(), grammarStartState, grammarEndState);
				}
			}

			Ptr<Automaton> CreateEpsilonPDA(Ptr<definitions::ParsingDefinition> definition, ParsingSymbolManager* manager)
			{
				Ptr<Automaton> automaton=new Automaton(manager);
				FOREACH(Ptr<ParsingDefinitionRuleDefinition>, rule, definition->rules)
				{
					CreateEpsilonPDA(automaton, rule, manager);
				}
				return automaton;
			}

/***********************************************************************
CreatePDAFromEpsilonPDA
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

			void DeleteState(Ptr<Automaton> automaton, State* state)
			{
				FOREACH(Transition*, transition, state->inputs)
				{
					transition->source->transitions.Remove(transition);
					automaton->transitions.Remove(transition);
				}
				FOREACH(Transition*, transition, state->transitions)
				{
					transition->target->inputs.Remove(transition);
					automaton->transitions.Remove(transition);
				}
				automaton->states.Remove(state);
			}

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
								DeleteState(automaton, newState);
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

			bool IsMergable(State* state1, State* state2)
			{
				return false;
			}

			void MergeState2ToState1(Ptr<Automaton> automaton, State* state1, State* state2)
			{
				// retarget state2's input to state1
				CopyFrom(state1->inputs, state2->inputs, true);
				FOREACH(Transition*, transition, state2->inputs)
				{
					transition->target=state1;
				}
				state2->inputs.Clear();
				DeleteState(automaton, state2);
			}

			void MergeStates(Ptr<Automaton> automaton, Ptr<RuleInfo> newRuleInfo, List<State*>& newStates)
			{
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
									if(IsMergable(state1, state2))
									{
										MergeState2ToState1(automaton, state1, state2);
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
					Dictionary<State*, Ptr<List<ClosureItem>>> endStateClosures;
					List<State*> scanningStates;
					vint currentStateIndex=0;
					oldNewStateMap.Add(ruleInfo->rootRuleStartState, newRuleInfo->rootRuleStartState);
					oldNewStateMap.Add(ruleInfo->rootRuleEndState, newRuleInfo->rootRuleEndState);
					oldNewStateMap.Add(ruleInfo->startState, newRuleInfo->startState);
					// begin with a root rule state state
					scanningStates.Add(ruleInfo->rootRuleStartState);

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

					List<State*> newStates;
					CopyFrom(newStates, oldNewStateMap.Values());
					DeleteUnnecessaryStates(automaton, newRuleInfo, newStates);
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