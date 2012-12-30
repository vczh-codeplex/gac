/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Parsing::Automaton

Classes:
***********************************************************************/

#ifndef VCZH_PARSING_PARSINGTABLE
#define VCZH_PARSING_PARSINGTABLE

#include "ParsingAnalyzer.h"
#include "..\Regex\Regex.h"

namespace vl
{
	namespace parsing
	{
		namespace tabling
		{

/***********************************************************************
Ìø×ª±í
***********************************************************************/

			class ParsingTable : public Object
			{
			public:
				static const vint							TokenBegin=0;
				static const vint							TokenFinish=1;
				static const vint							UserTokenStart=2;

				class TokenInfo
				{
				public:
					WString									name;
					WString									regex;
				};

				class StateInfo
				{
				public:
					WString									ruleName;
					WString									stateName;
					WString									stateExpression;
				};

				class RuleInfo
				{
				public:
					WString									name;
					WString									type;
					vint									rootStartState;
				};

				class Instruction
				{
				public:
					enum InstructionType
					{
						Create,
						Assign,
						Item,
						Using,
						Setter,
						Shift,
						Reduce,
						LeftRecursiveReduce,
					};

					InstructionType							instructionType;
					vint									stateParameter;
					WString									nameParameter;
					WString									value;

					Instruction()
						:instructionType(Create)
						,stateParameter(0)
					{
					}
				};

				class TransitionItem
				{
				public:
					vint									token;
					vint									targetState;
					collections::List<vint>					stackPattern;
					collections::List<Instruction>			instructions;

					enum OrderResult
					{
						CorrectOrder,
						WrongOrder,
						SameOrder,
						UnknownOrder,
					};

					static OrderResult						CheckOrder(Ptr<TransitionItem> t1, Ptr<TransitionItem> t2, bool forceGivingOrder);
					static vint								Compare(Ptr<TransitionItem> t1, Ptr<TransitionItem> t2);
				};

				class TransitionBag
				{
				public:
					collections::List<Ptr<TransitionItem>>	transitionItems;
				};

			protected:
				Ptr<regex::RegexLexer>						lexer;
				collections::Array<Ptr<TransitionBag>>		transitionBags;
				vint										tokenCount;
				vint										stateCount;
				collections::Array<TokenInfo>				tokenInfos;
				collections::Array<TokenInfo>				discardTokenInfos;
				collections::Array<StateInfo>				stateInfos;
				collections::Array<RuleInfo>				ruleInfos;

			public:
				ParsingTable(vint _tokenCount, vint _discardTokenCount, vint _stateCount, vint _ruleCount);
				~ParsingTable();

				vint										GetTokenCount();
				const TokenInfo&							GetTokenInfo(vint token);
				void										SetTokenInfo(vint token, const TokenInfo& info);

				vint										GetDiscardTokenCount();
				const TokenInfo&							GetDiscardTokenInfo(vint token);
				void										SetDiscardTokenInfo(vint token, const TokenInfo& info);

				vint										GetStateCount();
				const StateInfo&							GetStateInfo(vint state);
				void										SetStateInfo(vint state, const StateInfo& info);

				vint										GetRuleCount();
				const RuleInfo&								GetRuleInfo(vint rule);
				void										SetRuleInfo(vint rule, const RuleInfo& info);

				const regex::RegexLexer&					GetLexer();
				Ptr<TransitionBag>							GetTransitionBag(vint state, vint token);
				void										SetTransitionBag(vint state, vint token, Ptr<TransitionBag> bag);
				void										Initialize();
				bool										IsInputToken(vint regexTokenIndex);
				vint										GetTableTokenIndex(vint regexTokenIndex);
				vint										GetTableDiscardTokenIndex(vint regexTokenIndex);
			};

/***********************************************************************
Óï·¨·ÖÎöÆ÷
***********************************************************************/

			class ParsingState : public Object
			{
			public:
				class TransitionResult
				{
				public:
					vint									tableTokenIndex;
					vint									tableStateSource;
					vint									tableStateTarget;
					regex::RegexToken*						token;
					ParsingTable::TransitionItem*			transition;

					TransitionResult()
						:tableTokenIndex(-1)
						,tableStateSource(-1)
						,tableStateTarget(-1)
						,token(0)
						,transition(0)
					{
					}

					operator bool()const
					{
						return transition!=0;
					}
				};
			private:
				WString										input;
				Ptr<ParsingTable>							table;
				collections::List<regex::RegexToken>		tokens;

				collections::List<vint>						stateStack;
				vint										currentState;
				vint										currentToken;
			public:
				ParsingState(const WString& _input, Ptr<ParsingTable> _table, vint codeIndex=-1);
				~ParsingState();

				const WString&								GetInput();
				Ptr<ParsingTable>							GetTable();
				const collections::List<regex::RegexToken>&	GetTokens();

				vint										Reset(const WString& rule);
				TransitionResult							ReadToken();
				const collections::List<vint>&				GetStateStack();
			};

/***********************************************************************
Óï·¨Ê÷Éú³ÉÆ÷
***********************************************************************/

			class ParsingTreeBuilder : public Object
			{
			protected:
				collections::List<Ptr<ParsingTreeObject>>	nodeStack;

			public:
				ParsingTreeBuilder();
				~ParsingTreeBuilder();

				void										Reset();
				void										Run(const ParsingState::TransitionResult& result);
				Ptr<ParsingTreeObject>						GetNode();
			};

/***********************************************************************
¸¨Öúº¯Êý
***********************************************************************/

			extern void										Log(Ptr<ParsingTable> table, stream::TextWriter& writer);
		}
	}
}

#endif