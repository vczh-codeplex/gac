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

			typedef Pair<State*, Ptr<List<Transition*>>> EpsilonClosureItem;

			void SearchEpsilonClosureInternal(List<Transition*>& transitionPath, Transition* transition, State* state, List<EpsilonClosureItem>& closure)
			{
				if(!transition || transition->transitionType==Transition::Epsilon)
				{
					FOREACH(Transition*, newTransition, state->transitions)
					{
						if(!transitionPath.Contains(newTransition))
						{
							transitionPath.Add(newTransition);
							SearchEpsilonClosureInternal(transitionPath, newTransition, newTransition->target, closure);
							transitionPath.RemoveAt(transitionPath.Count()-1);
						}
					}
				}
				else
				{
					Ptr<List<Transition*>> path=new List<Transition*>;
					CopyFrom(*path.Obj(), transitionPath);
					closure.Add(EpsilonClosureItem(state, path));
				}
			}

			void SearchEpsilonClosure(State* startState, List<EpsilonClosureItem>& closure)
			{
				List<Transition*> transitionPath;
				SearchEpsilonClosureInternal(transitionPath, 0, startState, closure);
			}

			State* GetMappedState(Ptr<Automaton> newAutomaton, Ptr<RuleInfo> newRuleInfo, State* oldState, List<State*>& scanningStates, Dictionary<State*, State*>& oldNewStateMap)
			{
				State* newState=0;
				vint mapIndex=oldNewStateMap.Keys().IndexOf(oldState);
				if(mapIndex==-1)
				{
					newState=newAutomaton->CopyState(oldState);
					oldNewStateMap.Add(oldState, newState);
					scanningStates.Add(oldState);
					if(newState->endState)
					{
						newRuleInfo->endStates.Add(newState);
					}
				}
				else
				{
					newState=oldNewStateMap.Values().Get(mapIndex);
				}
				return newState;
			}

			Ptr<Automaton> CreateNondeterministicPDAFromEpsilonPDA(Ptr<Automaton> epsilonPDA)
			{
				Ptr<Automaton> automaton=new Automaton(epsilonPDA->symbolManager);
				FOREACH(ParsingDefinitionRuleDefinition*, rule, epsilonPDA->ruleInfos.Keys())
				{
					Ptr<RuleInfo> ruleInfo=epsilonPDA->ruleInfos[rule];
					Ptr<RuleInfo> newRuleInfo=new RuleInfo;
					automaton->ruleInfos.Add(rule, newRuleInfo);

					newRuleInfo->rootRuleStartState=automaton->RootRuleStartState(rule);
					newRuleInfo->rootRuleEndState=automaton->RootRuleEndState(rule);
					newRuleInfo->startState=automaton->RuleStartState(rule);

					Dictionary<State*, State*> oldNewStateMap;
					List<State*> scanningStates;
					vint currentStateIndex=0;
					oldNewStateMap.Add(ruleInfo->rootRuleStartState, newRuleInfo->rootRuleStartState);
					oldNewStateMap.Add(ruleInfo->rootRuleEndState, newRuleInfo->rootRuleEndState);
					oldNewStateMap.Add(ruleInfo->startState, newRuleInfo->startState);
					scanningStates.Add(ruleInfo->rootRuleStartState);

					while(currentStateIndex<scanningStates.Count())
					{
						State* currentOldState=scanningStates[currentStateIndex++];
						State* currentNewState=GetMappedState(automaton, newRuleInfo, currentOldState, scanningStates, oldNewStateMap);

						List<EpsilonClosureItem> closure;
						SearchEpsilonClosure(currentOldState, closure);
						FOREACH(EpsilonClosureItem, closureItem, closure)
						{
							Transition* oldTransition=closureItem.value->Get(closureItem.value->Count()-1);
							State* newEndState=GetMappedState(automaton, newRuleInfo, oldTransition->target, scanningStates, oldNewStateMap);
							Transition* transition=automaton->CopyTransition(currentNewState, newEndState, oldTransition);
							FOREACH(Transition*, pathTransition, *closureItem.value.Obj())
							{
								CopyFrom(transition->actions, pathTransition->actions, true);
							}
						}
					}
				}
				return automaton;
			}
		}
	}
}