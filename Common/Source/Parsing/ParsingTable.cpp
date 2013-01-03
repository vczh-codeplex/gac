#include "ParsingTable.h"
#include "ParsingAutomaton.h"
#include "..\Collections\Operation.h"

namespace vl
{
	namespace parsing
	{
		namespace tabling
		{
			using namespace definitions;
			using namespace analyzing;
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

/***********************************************************************
ParsingState
***********************************************************************/

			ParsingState::ParsingState(const WString& _input, Ptr<ParsingTable> _table, vint codeIndex)
				:input(_input)
				,table(_table)
				,currentState(-1)
				,currentToken(-1)
				,shiftToken(0)
				,reduceToken(0)
			{
				CopyFrom(tokens, table->GetLexer().Parse(input, codeIndex));
			}

			ParsingState::~ParsingState()
			{
			}

			const WString& ParsingState::GetInput()
			{
				return input;
			}

			Ptr<ParsingTable> ParsingState::GetTable()
			{
				return table;
			}

			const collections::List<regex::RegexToken>& ParsingState::GetTokens()
			{
				return tokens;
			}

			vint ParsingState::Reset(const WString& rule)
			{
				for(vint i=0;i<table->GetRuleCount();i++)
				{
					const ParsingTable::RuleInfo& info=table->GetRuleInfo(i);
					if(info.name==rule)
					{
						stateStack.Clear();
						currentState=info.rootStartState;
						currentToken=-1;
						shiftToken=0;
						reduceToken=0;
						return currentState;
					}
				}
				return -1;
			}

			ParsingState::TransitionResult ParsingState::ReadToken()
			{
				vint token=-1;
				RegexToken* regexToken=0;
				while(0<=currentToken && currentToken<tokens.Count())
				{
					if(table->IsInputToken((regexToken=&tokens[currentToken])->token))
					{
						break;
					}
					else
					{
						regexToken=0;
						currentToken++;
					}
				}
				if(currentToken==-1)
				{
					token=ParsingTable::TokenBegin;
				}
				else if(currentToken==tokens.Count())
				{
					token=ParsingTable::TokenFinish;
				}
				else if(0<=currentToken && currentToken<tokens.Count())
				{
					token=table->GetTableTokenIndex(regexToken->token);
				}
				else
				{
					return TransitionResult();
				}

				bool tryReduce=false;
				TransitionResult result=ReadToken(token, regexToken);
				if(!result)
				{
					result=ReadToken(ParsingTable::TryReduce, 0);
					tryReduce=true;
				}

				if(result && !tryReduce)
				{
					currentToken++;
				}
				return result;
			}

			ParsingState::TransitionResult ParsingState::ReadToken(vint tableTokenIndex, regex::RegexToken* regexToken)
			{
				ParsingTable::TransitionBag* bag=table->GetTransitionBag(currentState, tableTokenIndex).Obj();
				if(bag)
				{
					for(vint i=0;i<bag->transitionItems.Count();i++)
					{
						ParsingTable::TransitionItem* item=bag->transitionItems[i].Obj();
						if(item->stackPattern.Count()<=stateStack.Count())
						{
							if(tableTokenIndex!=ParsingTable::TokenFinish || item->stackPattern.Count()==stateStack.Count())
							{
								bool match=true;
								for(vint j=0;j<item->stackPattern.Count();j++)
								{
									if(item->stackPattern[j]!=stateStack[stateStack.Count()-1-j])
									{
										match=false;
										break;
									}
								}

								if(match)
								{
									if(regexToken)
									{
										if(!shiftToken)
										{
											shiftToken=regexToken;
											reduceToken=regexToken;
											shiftTokenStack.Add(shiftToken);
										}
									}

									TransitionResult result;
									result.tableTokenIndex=tableTokenIndex;
									result.token=regexToken;
									result.tokenIndexInStream=regexToken?currentToken:-1;
									result.tableStateSource=currentState;
									result.tableStateTarget=item->targetState;
									result.transition=item;

									for(vint j=0;j<item->instructions.Count();j++)
									{
										ParsingTable::Instruction& ins=item->instructions[j];
										switch(ins.instructionType)
										{
										case ParsingTable::Instruction::Shift:
											{
												stateStack.Add(ins.stateParameter);

												shiftTokenStack.Add(shiftToken);
												shiftToken=regexToken;
												reduceToken=regexToken;
											}
											break;
										case ParsingTable::Instruction::Reduce:
											{
												stateStack.RemoveAt(stateStack.Count()-1);

												result.AddShiftReduceRange(shiftToken, reduceToken);
												shiftToken=shiftTokenStack[shiftTokenStack.Count()-1];
												shiftTokenStack.RemoveAt(shiftTokenStack.Count()-1);
											}
											break;
										case ParsingTable::Instruction::LeftRecursiveReduce:
											{
												reduceToken=regexToken;
											}
											break;
										}
									}

									if(regexToken)
									{
										reduceToken=regexToken;
									}

									if(tableTokenIndex==ParsingTable::TokenFinish)
									{
										shiftToken=shiftTokenStack[shiftTokenStack.Count()-1];
										shiftTokenStack.RemoveAt(shiftTokenStack.Count()-1);
										result.AddShiftReduceRange(shiftToken, reduceToken);
									}

									currentState=item->targetState;
									return result;
								}
							}
						}
					}
				}
				return TransitionResult();
			}

			vint ParsingState::GetCurrentToken()
			{
				return currentToken;
			}

			const collections::List<vint>& ParsingState::GetStateStack()
			{
				return stateStack;
			}

/***********************************************************************
ParsingTreeBuilder
***********************************************************************/

			ParsingTreeBuilder::ParsingTreeBuilder()
			{
			}

			ParsingTreeBuilder::~ParsingTreeBuilder()
			{
			}

			void ParsingTreeBuilder::Reset()
			{
				createdObject=0;
				operationTarget=new ParsingTreeObject();
				nodeStack.Clear();
			}

			bool ParsingTreeBuilder::Run(const ParsingState::TransitionResult& result)
			{
				if(!operationTarget)
				{
					return false;
				}
				vint shiftReduceRangeIndex=0;
				for(vint j=0;j<result.transition->instructions.Count();j++)
				{
					ParsingTable::Instruction& ins=result.transition->instructions[j];
					switch(ins.instructionType)
					{
					case ParsingTable::Instruction::Create:
						{
							if(operationTarget->GetType()!=L"")
							{
								return false;
							}
							operationTarget->SetType(ins.nameParameter);
						}
						break;
					case ParsingTable::Instruction::Using:
						{
							if(operationTarget->GetType()!=L"" || !createdObject)
							{
								return false;
							}
							Ptr<ParsingTreeObject> obj=createdObject.Cast<ParsingTreeObject>();
							if(!obj)
							{
								return false;
							}
							for(vint i=0;i<operationTarget->GetMembers().Count();i++)
							{
								WString name=operationTarget->GetMembers().Keys().Get(i);
								Ptr<ParsingTreeNode> value=operationTarget->GetMembers().Values().Get(i);
								obj->SetMember(name, value);
							}
							operationTarget=obj;
							createdObject=0;
						}
						break;
					case ParsingTable::Instruction::Assign:
						{
							if(!createdObject)
							{
								if(result.token==0)
								{
									return false;
								}
								Ptr<ParsingTreeToken> value=new ParsingTreeToken(WString(result.token->reading, result.token->length), result.tokenIndexInStream);
								value->SetCodeRange(ParsingTextRange(result.token, result.token));
								operationTarget->SetMember(ins.nameParameter, value);
							}
							else
							{
								operationTarget->SetMember(ins.nameParameter, createdObject);
								createdObject=0;
							}
						}
						break;
					case ParsingTable::Instruction::Item:
						{
							Ptr<ParsingTreeArray> arr=operationTarget->GetMember(ins.nameParameter).Cast<ParsingTreeArray>();;
							if(!arr)
							{
								arr=new ParsingTreeArray();
								operationTarget->SetMember(ins.nameParameter, arr);
							}
							if(!createdObject)
							{
								if(result.token==0)
								{
									return false;
								}
								Ptr<ParsingTreeToken> value=new ParsingTreeToken(WString(result.token->reading, result.token->length), result.tokenIndexInStream);
								arr->AddItem(value);
							}
							else
							{
								arr->AddItem(createdObject);
								createdObject=0;
							}
						}
						break;
					case ParsingTable::Instruction::Setter:
						{
							Ptr<ParsingTreeToken> value=new ParsingTreeToken(ins.value, -1);
							operationTarget->SetMember(ins.nameParameter, value);
						}
						break;
					case ParsingTable::Instruction::Shift:
						{
							nodeStack.Add(operationTarget);
							operationTarget=new ParsingTreeObject();
							createdObject=0;
						}
						break;
					case ParsingTable::Instruction::Reduce:
						{
							if(nodeStack.Count()==0)
							{
								return false;
							}
							createdObject=operationTarget;
							operationTarget=nodeStack[nodeStack.Count()-1];
							nodeStack.RemoveAt(nodeStack.Count()-1);

							if(result.shiftReduceRanges)
							{
								ParsingState::ShiftReduceRange tokenRange=result.shiftReduceRanges->Get(shiftReduceRangeIndex++);
								if(tokenRange.shiftToken && tokenRange.reduceToken)
								{
									ParsingTextRange codeRange(tokenRange.shiftToken, tokenRange.reduceToken);
									createdObject->SetCodeRange(codeRange);
								}
							}
						}
						break;
					case ParsingTable::Instruction::LeftRecursiveReduce:
						{
							createdObject=operationTarget;
							operationTarget=new ParsingTreeObject();
						}
						break;
					default:
						return false;
					}
				}

				if(result.tableTokenIndex==ParsingTable::TokenFinish)
				{
					if(result.shiftReduceRanges)
					{
						ParsingState::ShiftReduceRange tokenRange=result.shiftReduceRanges->Get(shiftReduceRangeIndex++);
						if(tokenRange.shiftToken && tokenRange.reduceToken)
						{
							ParsingTextRange codeRange(tokenRange.shiftToken, tokenRange.reduceToken);
							operationTarget->SetCodeRange(codeRange);
						}
					}
				}
				return true;
			}

			Ptr<ParsingTreeObject> ParsingTreeBuilder::GetNode()
			{
				if(nodeStack.Count()==0)
				{
					return operationTarget;
				}
				else
				{
					return 0;
				}
			}

/***********************************************************************
ParsingRestrictParser
***********************************************************************/

			const regex::RegexToken* ParsingRestrictParser::ConvertToken(vint token, ParsingState& state)
			{
				if(token<=0)
				{
					token=0;
				}
				else if(token>state.GetTokens().Count())
				{
					token=state.GetTokens().Count();
				}

				return token==state.GetTokens().Count()?0:&state.GetTokens().Get(token);
			}

			ParsingRestrictParser::ParsingRestrictParser(Ptr<ParsingTable> _table)
				:table(_table)
			{
			}

			ParsingRestrictParser::~ParsingRestrictParser()
			{
			}

			Ptr<ParsingTreeNode> ParsingRestrictParser::Parse(const WString& input, const WString& rule, ParsingError& error)
			{
				ParsingState state(input, table);
				if(state.Reset(rule)==-1)
				{
					error=ParsingError(L"Rule \""+rule+L"\" does not exist.");
				}
				ParsingTreeBuilder builder;
				builder.Reset();

				for(vint i=0;i<state.GetTokens().Count();i++)
				{
					const RegexToken* token=&state.GetTokens().Get(i);
					if(token->token==-1)
					{
						error=ParsingError(token, L"Unrecognizable token.");
					}
				}

				ParsingState::TransitionResult result;
				while(true)
				{
					result=state.ReadToken();
					if(!result)
					{
						const RegexToken* token=ConvertToken(state.GetCurrentToken(), state);
						error=ParsingError(token, (token==0?L"Error happened during parsing.":L"Error happened during parsing when reaching to the end of the input."));
						return 0;
					}
					else if(!builder.Run(result))
					{
						const RegexToken* token=ConvertToken(state.GetCurrentToken(), state);
						error=ParsingError(token, L"Internal error when building the parsing tree.");
						return 0;
					}
					else if(result.tableTokenIndex==ParsingTable::TokenFinish)
					{
						break;
					}
				}

				Ptr<ParsingTreeNode> node=builder.GetNode();
				if(!node)
				{
					error=ParsingError(L"Internal error when building the parsing tree after a succeeded parsing process.");
					return 0;
				}
				return node;
			}

			Ptr<ParsingRestrictParser> CreateBootstrapParser()
			{
				List<Ptr<ParsingError>> errors;
				Ptr<ParsingDefinition> definition=CreateParserDefinition();
				Ptr<ParsingTable> table=GenerateTable(definition, errors);
				if(table)
				{
					return new ParsingRestrictParser(table);
				}
				return 0;
			}
		}
	}
}