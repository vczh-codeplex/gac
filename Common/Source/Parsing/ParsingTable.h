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
				regex::RegexLexer							lexer;
				collections::Array<Ptr<TransitionBag>>		transitionBags;
				vint										tokenCount;
				vint										stateCount;
				collections::Array<TokenInfo>				tokenInfos;
				collections::Array<TokenInfo>				discardTokenInfos;
				collections::Array<StateInfo>				stateInfos;
				collections::Array<RuleInfo>				ruleInfos;

			public:
				ParsingTable(const collections::List<WString>& tokenRegex, vint _tokenCount, vint _discardTokenCount, vint _stateCount, vint _ruleCount);
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

				Ptr<TransitionBag>							GetTransitionBag(vint state, vint token);
				void										SetTransitionBag(vint state, vint token, Ptr<TransitionBag> bag);
			};

/***********************************************************************
¸¨Öúº¯Êý
***********************************************************************/

			extern void										Log(Ptr<ParsingTable> table, stream::TextWriter& writer);
		}
	}
}

#endif