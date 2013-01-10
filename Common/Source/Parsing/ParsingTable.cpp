#include "ParsingTable.h"
#include "..\Collections\Operation.h"

namespace vl
{
	namespace parsing
	{
		namespace tabling
		{
			using namespace collections;
			using namespace regex;

/***********************************************************************
ParsingTable::TransitionItem
***********************************************************************/

			enum TransitionLevel
			{
				ReduceTransition,
				LeftRecursiveReduceTransition,
				NormalTransition,
			};

			TransitionLevel GetTransitionLevel(Ptr<ParsingTable::TransitionItem> t)
			{
				bool hasReduce=false;
				bool hasLrReduce=false;
				FOREACH(ParsingTable::Instruction, ins, t->instructions)
				{
					switch(ins.instructionType)
					{
					case ParsingTable::Instruction::Reduce:
						hasReduce=true;
						break;
					case ParsingTable::Instruction::LeftRecursiveReduce:
						hasLrReduce=true;
						break;
					}
				}

				return
					hasLrReduce?LeftRecursiveReduceTransition:
					hasReduce?ReduceTransition:
					NormalTransition;
			}

			ParsingTable::TransitionItem::OrderResult ParsingTable::TransitionItem::CheckOrder(Ptr<TransitionItem> t1, Ptr<TransitionItem> t2, bool forceGivingOrder)
			{
				if(t1->token!=t2->token) return UnknownOrder;
				TransitionLevel level1=GetTransitionLevel(t1);
				TransitionLevel level2=GetTransitionLevel(t2);
				if(level1>level2) return CorrectOrder;
				if(level1<level2) return WrongOrder;

				vint ic1=t1->stackPattern.Count();
				vint ic2=t2->stackPattern.Count();
				vint ic=ic1<ic2?ic1:ic2;

				for(vint i=0;i<ic;i++)
				{
					vint s1=t1->stackPattern[i];
					vint s2=t2->stackPattern[i];

					if(s1>s2)
					{
						return CorrectOrder;
					}
					else if(s1<s2)
					{
						return WrongOrder;
					}
				}

				if(t1->token==TokenFinish)
				{
					if(ic1>ic2)
					{
						return CorrectOrder;
					}
					else if(ic1<ic2)
					{
						return WrongOrder;
					}
				}

				if(forceGivingOrder)
				{
					return t1>t2?CorrectOrder:SameOrder;
				}
				else
				{
					return UnknownOrder;
				}
			}

			vint ParsingTable::TransitionItem::Compare(Ptr<TransitionItem> t1, Ptr<TransitionItem> t2)
			{
				OrderResult order=CheckOrder(t1, t2, true);
				switch(order)
				{
				case CorrectOrder:	return -1;
				case WrongOrder:	return 1;
				default:			return 0;
				}
			}

/***********************************************************************
ParsingTable
***********************************************************************/

			ParsingTable::ParsingTable(vint _tokenCount, vint discardTokenCount, vint _stateCount, vint _ruleCount)
				:tokenCount(_tokenCount+UserTokenStart)
				,stateCount(_stateCount)
				,tokenInfos(_tokenCount+UserTokenStart)
				,discardTokenInfos(discardTokenCount)
				,stateInfos(_stateCount)
				,ruleInfos(_ruleCount)
				,transitionBags((_tokenCount+UserTokenStart)*_stateCount)
			{
			}

			ParsingTable::~ParsingTable()
			{
			}

			vint ParsingTable::GetTokenCount()
			{
				return tokenCount;
			}

			const ParsingTable::TokenInfo& ParsingTable::GetTokenInfo(vint token)
			{
				return tokenInfos[token];
			}

			void ParsingTable::SetTokenInfo(vint token, const TokenInfo& info)
			{
				tokenInfos[token]=info;
			}

			vint ParsingTable::GetDiscardTokenCount()
			{
				return discardTokenInfos.Count();
			}

			const ParsingTable::TokenInfo& ParsingTable::GetDiscardTokenInfo(vint token)
			{
				return discardTokenInfos[token];
			}

			void ParsingTable::SetDiscardTokenInfo(vint token, const TokenInfo& info)
			{
				discardTokenInfos[token]=info;
			}

			vint ParsingTable::GetStateCount()
			{
				return stateCount;
			}

			const ParsingTable::StateInfo& ParsingTable::GetStateInfo(vint state)
			{
				return stateInfos[state];
			}

			void ParsingTable::SetStateInfo(vint state, const StateInfo& info)
			{
				stateInfos[state]=info;
			}

			vint ParsingTable::GetRuleCount()
			{
				return ruleInfos.Count();
			}

			const ParsingTable::RuleInfo& ParsingTable::GetRuleInfo(vint rule)
			{
				return ruleInfos[rule];
			}

			void ParsingTable::SetRuleInfo(vint rule, const RuleInfo& info)
			{
				ruleInfos[rule]=info;
			}

			const regex::RegexLexer& ParsingTable::GetLexer()
			{
				return *lexer.Obj();
			}

			Ptr<ParsingTable::TransitionBag> ParsingTable::GetTransitionBag(vint state, vint token)
			{
				return transitionBags[state*tokenCount+token];
			}

			void ParsingTable::SetTransitionBag(vint state, vint token, Ptr<TransitionBag> bag)
			{
				transitionBags[state*tokenCount+token]=bag;
			}

			void ParsingTable::Initialize()
			{
				List<WString> tokens;
				FOREACH(TokenInfo, info, tokenInfos>>Skip(UserTokenStart))
				{
					tokens.Add(info.regex);
				}
				FOREACH(TokenInfo, info, discardTokenInfos)
				{
					tokens.Add(info.regex);
				}
				lexer=new RegexLexer(tokens);
			}

			bool ParsingTable::IsInputToken(vint regexTokenIndex)
			{
				return regexTokenIndex>=0 && regexTokenIndex<tokenCount-UserTokenStart;
			}

			vint ParsingTable::GetTableTokenIndex(vint regexTokenIndex)
			{
				return IsInputToken(regexTokenIndex)?regexTokenIndex+UserTokenStart:-1;
			}

			vint ParsingTable::GetTableDiscardTokenIndex(vint regexTokenIndex)
			{
				return regexTokenIndex>=tokenCount-UserTokenStart?regexTokenIndex-(tokenCount-UserTokenStart):-1;
			}
		}
	}
}