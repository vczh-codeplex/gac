/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Parsing::Table

Classes:
***********************************************************************/

#ifndef VCZH_PARSING_PARSINGTABLE
#define VCZH_PARSING_PARSINGTABLE

#include "ParsingTree.h"

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
				static const vint							TryReduce=2;
				static const vint							UserTokenStart=3;

				class TokenInfo
				{
				public:
					WString									name;
					WString									regex;
					vint									regexTokenIndex;

					TokenInfo():regexTokenIndex(-1){}

					TokenInfo(const WString& _name, const WString& _regex)
						:name(_name)
						,regex(_regex)
						,regexTokenIndex(-1)
					{
					}
				};

				class StateInfo
				{
				public:
					WString									ruleName;
					WString									stateName;
					WString									stateExpression;

					StateInfo(){}

					StateInfo(const WString& _ruleName, const WString& _stateName, const WString& _stateExpression)
						:ruleName(_ruleName)
						,stateName(_stateName)
						,stateExpression(_stateExpression)
					{
					}
				};

				class RuleInfo
				{
				public:
					WString									name;
					WString									type;
					vint									rootStartState;

					RuleInfo(){}

					RuleInfo(const WString& _name, const WString& _type, vint _rootStartState)
						:name(_name)
						,type(_type)
						,rootStartState(_rootStartState)
					{
					}
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

					Instruction(InstructionType _instructionType, vint _stateParameter, const WString& _nameParameter, const WString& _value)
						:instructionType(_instructionType)
						,stateParameter(_stateParameter)
						,nameParameter(_nameParameter)
						,value(_value)
					{
					}
				};

				class LookAheadInfo
				{
				public:
					collections::List<vint>					tokens;
					vint									state;

					LookAheadInfo()
						:state(-1)
					{
					}

					enum PrefixResult
					{
						Prefix,
						Equal,
						NotPrefix,
					};

					static PrefixResult						TestPrefix(Ptr<LookAheadInfo> a, Ptr<LookAheadInfo> b);
					static void								Walk(Ptr<ParsingTable> table, Ptr<LookAheadInfo> previous, vint state, collections::List<Ptr<LookAheadInfo>>& newInfos);
				};

				class TransitionItem
				{
				public:
					vint									token;
					vint									targetState;
					collections::List<Ptr<LookAheadInfo>>	lookAheads;
					collections::List<vint>					stackPattern;
					collections::List<Instruction>			instructions;

					enum OrderResult
					{
						CorrectOrder,
						WrongOrder,
						SameOrder,
						UnknownOrder,
					};

					TransitionItem(){}

					TransitionItem(vint _token, vint _targetState)
						:token(_token)
						,targetState(_targetState)
					{
					}

					static OrderResult						CheckOrder(Ptr<TransitionItem> t1, Ptr<TransitionItem> t2, bool forceGivingOrder);
					static vint								Compare(Ptr<TransitionItem> t1, Ptr<TransitionItem> t2);
				};

				class TransitionBag
				{
				public:
					collections::List<Ptr<TransitionItem>>	transitionItems;
				};

			protected:
				bool										ambiguity;
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

				bool										GetAmbiguity();
				void										SetAmbiguity(bool value);

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
¸¨Öúº¯Êý
***********************************************************************/

			extern void										Log(Ptr<ParsingTable> table, stream::TextWriter& writer);
		}
	}
}

#endif