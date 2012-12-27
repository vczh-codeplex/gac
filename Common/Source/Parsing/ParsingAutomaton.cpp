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
				,shiftReduceSource(0)
				,shiftReduceTarget(0)
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
				,stackOperationType(None)
				,transitionSymbol(0)
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
				state->stateExpression=L"�� <"+ownerRule->name+L">";
				return state;
			}

			State* Automaton::RootRuleStartState(definitions::ParsingDefinitionRuleDefinition* ownerRule)
			{
				State* state=new State;
				states.Add(state);

				state->ownerRule=ownerRule;
				state->ownerRuleSymbol=symbolManager->GetGlobal()->GetSubSymbolByName(ownerRule->name);
				state->stateName=ownerRule->name+L".RootStart";
				state->stateExpression=L"�� $<"+ownerRule->name+L">";
				return state;
			}

			State* Automaton::RootRuleEndState(definitions::ParsingDefinitionRuleDefinition* ownerRule)
			{
				State* state=new State;
				states.Add(state);

				state->ownerRule=ownerRule;
				state->ownerRuleSymbol=symbolManager->GetGlobal()->GetSubSymbolByName(ownerRule->name);
				state->stateName=ownerRule->name+L".RootEnd";
				state->stateExpression=L"$<"+ownerRule->name+L"> ��";
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

			Transition* Automaton::CopyTransition(State* start, State* end, Transition* oldTransition)
			{
				Transition* transition=CreateTransition(start, end);
				transition->transitionType=oldTransition->transitionType;
				transition->stackOperationType=oldTransition->stackOperationType;
				transition->transitionSymbol=oldTransition->transitionSymbol;
				return transition;
			}

			void Automaton::DeleteTransition(Transition* transition)
			{
				transition->source->transitions.Remove(transition);
				transition->target->inputs.Remove(transition);
				transitions.Remove(transition);
			}

			void Automaton::DeleteState(State* state)
			{
				while(state->inputs.Count())
				{
					DeleteTransition(state->inputs[0]);
				}
				while(state->transitions.Count())
				{
					DeleteTransition(state->transitions[0]);
				}
				states.Remove(state);
			}
		}
	}
}