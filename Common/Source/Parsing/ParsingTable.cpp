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
ParsingTable::LookAheadInfo
***********************************************************************/

			ParsingTable::LookAheadInfo::PrefixResult ParsingTable::LookAheadInfo::TestPrefix(Ptr<LookAheadInfo> a, Ptr<LookAheadInfo> b)
			{
				if(a->tokens.Count()>b->tokens.Count())
				{
					return ParsingTable::LookAheadInfo::NotPrefix;
				}
				for(vint i=0;i<a->tokens.Count();i++)
				{
					if(a->tokens[i]!=b->tokens[i])
					{
						return ParsingTable::LookAheadInfo::NotPrefix;
					}
				}
				return a->tokens.Count()<b->tokens.Count()?ParsingTable::LookAheadInfo::Prefix:ParsingTable::LookAheadInfo::Equal;
			}

			void ParsingTable::LookAheadInfo::Walk(Ptr<ParsingTable> table, Ptr<LookAheadInfo> previous, vint state, collections::List<Ptr<LookAheadInfo>>& newInfos)
			{
				for(vint i=0;i<table->GetTokenCount();i++)
				{
					Ptr<TransitionBag> bag=table->GetTransitionBag(state, i);
					if(bag)
					{
						SortedList<vint> newStates;
						FOREACH(Ptr<TransitionItem>, item, bag->transitionItems)
						{
							if(!newStates.Contains(item->targetState))
							{
								newStates.Add(item->targetState);
								Ptr<LookAheadInfo> info=new LookAheadInfo;
								info->state=item->targetState;
								if(previous)
								{
									CopyFrom(info->tokens, previous->tokens);
								}
								info->tokens.Add(i);
								newInfos.Add(info);
							}
						}
					}
				}
			}

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
				if(forceGivingOrder)
				{
					TransitionLevel level1=GetTransitionLevel(t1);
					TransitionLevel level2=GetTransitionLevel(t2);
					if(level1>level2) return CorrectOrder;
					if(level1<level2) return WrongOrder;
				}

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

			ParsingTable::ParsingTable(vint _attributeInfoCount, vint _treeTypeInfoCount, vint _treeFieldInfoCount, vint _tokenCount, vint discardTokenCount, vint _stateCount, vint _ruleCount)
				:ambiguity(false)
				,tokenCount(_tokenCount+UserTokenStart)
				,stateCount(_stateCount)
				,attributeInfos(_attributeInfoCount)
				,treeTypeInfos(_treeTypeInfoCount)
				,treeFieldInfos(_treeFieldInfoCount)
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

			bool ParsingTable::GetAmbiguity()
			{
				return ambiguity;
			}

			void ParsingTable::SetAmbiguity(bool value)
			{
				ambiguity=value;
			}

			vint ParsingTable::GetAttributeInfoCount()
			{
				return attributeInfos.Count();
			}

			Ptr<ParsingTable::AttributeInfoList> ParsingTable::GetAttributeInfo(vint index)
			{
				return attributeInfos[index];
			}

			void ParsingTable::SetAttributeInfo(vint index, Ptr<AttributeInfoList> info)
			{
				attributeInfos[index]=info;
			}

			vint ParsingTable::GetTreeTypeInfoCount()
			{
				return treeTypeInfos.Count();
			}

			const ParsingTable::TreeTypeInfo& ParsingTable::GetTreeTypeInfo(vint index)
			{
				return treeTypeInfos[index];
			}

			const ParsingTable::TreeTypeInfo& ParsingTable::GetTreeTypeInfo(const WString& type)
			{
				vint index=treeTypeInfoMap.Keys().IndexOf(type);
				if(index==-1) return *(const TreeTypeInfo*)0;
				return treeTypeInfos[treeTypeInfoMap.Values().Get(index)];
			}

			void ParsingTable::SetTreeTypeInfo(vint index, const TreeTypeInfo& info)
			{
				treeTypeInfos[index]=info;
			}

			vint ParsingTable::GetTreeFieldInfoCount()
			{
				return treeFieldInfos.Count();
			}

			const ParsingTable::TreeFieldInfo& ParsingTable::GetTreeFieldInfo(vint index)
			{
				return treeFieldInfos[index];
			}

			const ParsingTable::TreeFieldInfo& ParsingTable::GetAttributeInfo(const WString& type, const WString& field)
			{
				Pair<WString, WString> key(type, field);
				vint index=treeFieldInfoMap.Keys().IndexOf(key);
				if(index==-1) return *(const TreeFieldInfo*)0;
				return treeFieldInfos[treeFieldInfoMap.Values().Get(index)];
			}

			void ParsingTable::SetTreeFieldInfo(vint index, const TreeFieldInfo& info)
			{
				treeFieldInfos[index]=info;
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

			const ParsingTable::RuleInfo& ParsingTable::GetRuleInfo(const WString& ruleName)
			{
				vint index=ruleMap.Keys().IndexOf(ruleName);
				if(index==-1) return *(const RuleInfo*)0;
				return ruleInfos[ruleMap.Values().Get(index)];
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
				FOREACH(TokenInfo, info, From(tokenInfos).Skip(UserTokenStart))
				{
					tokens.Add(info.regex);
				}
				FOREACH(TokenInfo, info, discardTokenInfos)
				{
					tokens.Add(info.regex);
				}

				vint regexTokenIndex=0;
				for(vint i=UserTokenStart;i<tokenInfos.Count();i++)
				{
					tokenInfos[i].regexTokenIndex=regexTokenIndex++;
				}
				for(vint i=0;i<discardTokenInfos.Count();i++)
				{
					discardTokenInfos[i].regexTokenIndex=regexTokenIndex++;
				}
				lexer=new RegexLexer(tokens);

				ruleMap.Clear();
				FOREACH_INDEXER(RuleInfo, rule, index, ruleInfos)
				{
					ruleMap.Add(rule.name, index);
				}
				for(vint i=0;i<stateInfos.Count();i++)
				{
					StateInfo& info=stateInfos[i];
					info.ruleAmbiguousType=ruleInfos[ruleMap[info.ruleName]].ambiguousType;
				}

				treeTypeInfoMap.Clear();
				FOREACH_INDEXER(TreeTypeInfo, info, index, treeTypeInfos)
				{
					treeTypeInfoMap.Add(info.type, index);
				}

				treeFieldInfoMap.Clear();
				FOREACH_INDEXER(TreeFieldInfo, info, index, treeFieldInfos)
				{
					Pair<WString, WString> key(info.type, info.field);
					treeFieldInfoMap.Add(key, index);
				}
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