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
					Create, // new source
					Assign, // source ::= <created symbol>
					Using,  // use <created symbol>
					Setter, // source ::= target
					Shift,
					Reduce,
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

				enum StackOperationType
				{
					None,
					ShiftReduceCompacted,
					LeftRecursive,
				};

				State*												source;
				State*												target;
				collections::List<Ptr<Action>>						actions;
				
				collections::List<State*>							stackPattern;
				TransitionType										transitionType;
				StackOperationType									stackOperationType;
				ParsingSymbol*										transitionSymbol;
				ParsingSymbol*										lookAheadSymbol;

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
				Transition*											CopyTransition(State* start, State* end, Transition* oldTransition);

				void												DeleteTransition(Transition* transition);
				void												DeleteState(State* state);
			};

/***********************************************************************
¸¨Öúº¯Êý
***********************************************************************/
			namespace closure_searching
			{
				struct ClosureItem
				{
					State*											state;			// target state of one path of a closure
					Ptr<collections::List<Transition*>>				transitions;	// path
					bool											cycle;			// true: invalid closure because there are cycles, and in the middle of the path there will be a transition that targets to the state field.

					ClosureItem()
						:state(0)
						,cycle(false)
					{
					}

					ClosureItem(State* _state, Ptr<collections::List<Transition*>> _transitions, bool _cycle)
						:state(_state)
						,transitions(_transitions)
						,cycle(_cycle)
					{
					}
				};

				enum ClosureSearchResult
				{
					Continue,
					Hit,
					Blocked,
				};

				extern void											SearchClosure(ClosureSearchResult(*closurePredicate)(Transition*), State* startState, collections::List<ClosureItem>& closure);
			}

			extern Ptr<Automaton>									CreateEpsilonPDA(Ptr<definitions::ParsingDefinition> definition, ParsingSymbolManager* manager);
			extern void												RemoveEpsilonTransitions(collections::Dictionary<State*, State*>& oldNewStateMap, collections::List<State*>& scanningStates, Ptr<Automaton> automaton);
			extern Ptr<Automaton>									CreateNondeterministicPDAFromEpsilonPDA(Ptr<Automaton> epsilonPDA);
			extern Ptr<Automaton>									CreateJointPDAFromNondeterministicPDA(Ptr<Automaton> nondeterministicPDA);
			extern void												CompactJointPDA(Ptr<Automaton> jointPDA);
			extern void												Log(Ptr<Automaton> automaton, stream::TextWriter& writer);
		}
	}
}

#endif