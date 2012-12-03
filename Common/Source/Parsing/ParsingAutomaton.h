/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
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
״̬��
***********************************************************************/

			class Action;
			class Transition;
			class State;

			class Action : public Object
			{
			public:
				enum ActionType
				{
					Create, // new source
					Assign, // source ::= <created symbol>
					Using,  // use <created symbol>
					Setter, // source ::= target
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
					TokenBegin,
					TokenFinish,
					Epsilon,
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
				bool												endState;

				ParsingSymbol*										ownerRuleSymbol;
				definitions::ParsingDefinitionRuleDefinition*		ownerRule;
				definitions::ParsingDefinitionGrammar*				grammarNode;
				definitions::ParsingDefinitionGrammar*				stateNode;
				StatePosition										statePosition;
				WString												stateName;
				WString												stateExpression;

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
				int													stateNameCount;

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
				State*												CopyState(State* oldState);

				Transition*											CreateTransition(State* start, State* end);
				Transition*											TokenBegin(State* start, State* end);
				Transition*											TokenFinish(State* start, State* end);
				Transition*											Epsilon(State* start, State* end);
				Transition*											Symbol(State* start, State* end, ParsingSymbol* transitionSymbol);
				Transition*											Symbol(State* start, State* end, const WString& transitionText);
				Transition*											CopyTransition(State* start, State* end, Transition* oldTransition);
			};

/***********************************************************************
��������
***********************************************************************/

			extern Ptr<Automaton>									CreateEpsilonPDA(Ptr<definitions::ParsingDefinition> definition, ParsingSymbolManager* manager);
			extern Ptr<Automaton>									CreateNondeterministicPDAFromEpsilonPDA(Ptr<Automaton> epsilonPDA);
			extern void												Log(Ptr<Automaton> automaton, stream::TextWriter& writer);
		}
	}
}

#endif