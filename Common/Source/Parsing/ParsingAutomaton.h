/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Parsing::Automaton

Classes:
***********************************************************************/

#ifndef VCZH_PARSING_PARSINGAUTOMATON
#define VCZH_PARSING_PARSINGAUTOMATON

#include "ParsingAnalyzer.h"

namespace vl
{
	namespace parsing
	{
		namespace analyzing
		{

/***********************************************************************
×´Ì¬»ú
***********************************************************************/

			class Action;
			class Transition;
			class State;

			class Action : public Object
			{
			public:
				enum ActionType
				{
					Create,
					Assign,
					Using,
					Setter,
				};

				ActionType											actionType;
				ParsingSymbol*										actionTarget;
				ParsingSymbol*										actionSource;

				Action();
				~Action();
			};

			class Transition : public Object
			{
			public:
				enum TransitionType
				{
					Epsilon,
					Finish,
					Symbol,
				};

				State*												source;
				State*												target;
				collections::List<Ptr<Action>>						actions;
				
				collections::List<State*>							stackPattern;
				TransitionType										transitionType;
				ParsingSymbol*										transitionSymbol;
				WString												transitionText;
				ParsingSymbol*										lookAheadSymbol;
				WString												lookAheadText;

				Transition();
				~Transition();
			};

			class State : public Object
			{
			public:
				enum StatePosition
				{
					BeforeNode,
					AfterNode,
				};

				collections::List<Transition*>						transitions;
				collections::List<Transition*>						inputs;

				ParsingSymbol*										ownerRuleSymbol;
				definitions::ParsingDefinitionRuleDefinition*		ownerRule;
				definitions::ParsingDefinitionGrammar*				stateNode;
				StatePosition										statePosition;
				WString												stateName;

				State();
				~State();
			};

			class RuleInfo : public Object
			{
			public:
				State*												rootRuleStartState;
				State*												rootRuleEndState;
				State*												startState;
				collections::List<State*>							endStates;

				RuleInfo();
				~RuleInfo();
			};

			class Automaton : public Object
			{
				typedef collections::Dictionary<definitions::ParsingDefinitionRuleDefinition*, Ptr<RuleInfo>>		RuleInfoMap;
			public:
				ParsingSymbolManager*								symbolManager;
				collections::List<Ptr<Transition>>					transitions;
				collections::List<Ptr<State>>						states;
				RuleInfoMap											ruleInfos;

				Automaton(ParsingSymbolManager* _symbolManager);
				~Automaton();

				State*												RuleStartState(definitions::ParsingDefinitionRuleDefinition* ownerRule);
				State*												RootRuleStartState(definitions::ParsingDefinitionRuleDefinition* ownerRule);
				State*												RootRuleEndState(definitions::ParsingDefinitionRuleDefinition* ownerRule);
				State*												StartState(definitions::ParsingDefinitionRuleDefinition* ownerRule, definitions::ParsingDefinitionGrammar* grammarNode, definitions::ParsingDefinitionGrammar* stateNode);
				State*												EndState(definitions::ParsingDefinitionRuleDefinition* ownerRule, definitions::ParsingDefinitionGrammar* grammarNode, definitions::ParsingDefinitionGrammar* stateNode);

				Transition*											CreateTransition(State* start, State* end);
				Transition*											Epsilon(State* start, State* end);
				Transition*											Finish(State* start, State* end);
			};

/***********************************************************************
¸¨Öúº¯Êý
***********************************************************************/

			extern Ptr<Automaton>						CreateEpsilonPDA(Ptr<definitions::ParsingDefinition> definition, ParsingSymbolManager* manager);
		}
	}
}

#endif