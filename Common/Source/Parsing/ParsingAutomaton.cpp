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
				,stateNode(0)
				,statePosition(BeforeNode)
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
				state->stateName=L"¡¤ <"+ownerRule->name+L">";
				return state;
			}

			State* Automaton::RootRuleStartState(definitions::ParsingDefinitionRuleDefinition* ownerRule)
			{
				State* state=new State;
				states.Add(state);

				state->ownerRule=ownerRule;
				state->ownerRuleSymbol=symbolManager->GetGlobal()->GetSubSymbolByName(ownerRule->name);
				state->stateName=L"¡¤ $<"+ownerRule->name+L">";
				return state;
			}

			State* Automaton::RootRuleEndState(definitions::ParsingDefinitionRuleDefinition* ownerRule)
			{
				State* state=new State;
				states.Add(state);

				state->ownerRule=ownerRule;
				state->ownerRuleSymbol=symbolManager->GetGlobal()->GetSubSymbolByName(ownerRule->name);
				state->stateName=L"$<"+ownerRule->name+L"> ¡¤";
				return state;
			}

			State* Automaton::StartState(definitions::ParsingDefinitionRuleDefinition* ownerRule, definitions::ParsingDefinitionGrammar* grammarNode, definitions::ParsingDefinitionGrammar* stateNode)
			{
				State* state=new State;
				states.Add(state);

				state->ownerRule=ownerRule;
				state->ownerRuleSymbol=symbolManager->GetGlobal()->GetSubSymbolByName(ownerRule->name);
				state->stateNode=stateNode;
				state->statePosition=State::BeforeNode;
				state->stateName=L"<"+ownerRule->name+L">: "+GrammarStateToString(grammarNode, stateNode, true);
				return state;
			}

			State* Automaton::EndState(definitions::ParsingDefinitionRuleDefinition* ownerRule, definitions::ParsingDefinitionGrammar* grammarNode, definitions::ParsingDefinitionGrammar* stateNode)
			{
				State* state=new State;
				states.Add(state);

				state->ownerRule=ownerRule;
				state->ownerRuleSymbol=symbolManager->GetGlobal()->GetSubSymbolByName(ownerRule->name);
				state->stateNode=stateNode;
				state->statePosition=State::BeforeNode;
				state->stateName=L"<"+ownerRule->name+L">: "+GrammarStateToString(grammarNode, stateNode, false);
				return state;
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

			Transition* Automaton::Epsilon(State* start, State* end)
			{
				Transition* transition=CreateTransition(start, end);
				transition->transitionType=Transition::Epsilon;
				return transition;
			}

			Transition* Automaton::Finish(State* start, State* end)
			{
				Transition* transition=CreateTransition(start, end);
				transition->transitionType=Transition::Finish;
				return transition;
			}

/***********************************************************************
CreateEpsilonPDA
***********************************************************************/

			class CreateEpsilonPDAVisitor : public Object, public ParsingDefinitionGrammar::IVisitor
			{
			public:
				Ptr<Automaton>					automaton;
				ParsingDefinitionGrammar*		ruleGrammar;
				State*							startState;
				State*							endState;

				CreateEpsilonPDAVisitor(Ptr<Automaton> _automaton, ParsingDefinitionGrammar* _ruleGrammar, State* _startState, State* _endState)
					:automaton(_automaton)
					,ruleGrammar(_ruleGrammar)
					,startState(_startState)
					,endState(_endState)
				{
				}

				static void Create(ParsingDefinitionGrammar* grammar, Ptr<Automaton> automaton, ParsingDefinitionGrammar* ruleGrammar, State* startState, State* endState)
				{
					CreateEpsilonPDAVisitor visitor(automaton, ruleGrammar, startState, endState);
					grammar->Accept(&visitor);
				}

				void Create(ParsingDefinitionGrammar* grammar, State* startState, State* endState)
				{
					Create(grammar, automaton, ruleGrammar, startState, endState);
				}

				void Visit(ParsingDefinitionPrimitiveGrammar* node)override
				{
				}

				void Visit(ParsingDefinitionTextGrammar* node)override
				{
				}

				void Visit(ParsingDefinitionSequenceGrammar* node)override
				{
				}

				void Visit(ParsingDefinitionAlternativeGrammar* node)override
				{
				}

				void Visit(ParsingDefinitionLoopGrammar* node)override
				{
				}

				void Visit(ParsingDefinitionOptionalGrammar* node)override
				{
				}

				void Visit(ParsingDefinitionCreateGrammar* node)override
				{
				}

				void Visit(ParsingDefinitionAssignGrammar* node)override
				{
				}

				void Visit(ParsingDefinitionUseGrammar* node)override
				{
				}

				void Visit(ParsingDefinitionSetterGrammar* node)override
				{
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

				FOREACH(Ptr<ParsingDefinitionGrammar>, grammar, rule->grammars.Wrap())
				{
					State* grammarStartState=automaton->StartState(rule.Obj(), grammar.Obj(), grammar.Obj());
					State* grammarEndState=automaton->EndState(rule.Obj(), grammar.Obj(), grammar.Obj());
					automaton->Epsilon(ruleInfo->startState, grammarStartState);
					automaton->Finish(grammarEndState, ruleInfo->rootRuleEndState);
					ruleInfo->endStates.Add(grammarEndState);
					CreateEpsilonPDAVisitor::Create(grammar.Obj(), automaton, grammar.Obj(), grammarStartState, grammarEndState);
				}
			}

			Ptr<Automaton> CreateEpsilonPDA(Ptr<definitions::ParsingDefinition> definition, ParsingSymbolManager* manager)
			{
				Ptr<Automaton> automaton=new Automaton(manager);
				FOREACH(Ptr<ParsingDefinitionRuleDefinition>, rule, definition->rules.Wrap())
				{
					CreateEpsilonPDA(automaton, rule, manager);
				}
				return automaton;
			}
		}
	}
}