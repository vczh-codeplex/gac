#include "ParsingState.h"
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
ParsingState
***********************************************************************/

			ParsingState::ParsingState(const WString& _input, Ptr<ParsingTable> _table, vint codeIndex)
				:input(_input)
				,table(_table)
				,currentState(-1)
				,currentToken(-1)
				,tokenSequenceIndex(0)
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

			regex::RegexToken* ParsingState::GetToken(vint index)
			{
				if(index<=0)
				{
					index=0;
				}
				else if(index>tokens.Count())
				{
					index=tokens.Count();
				}

				return index==tokens.Count()?0:&tokens[index];
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
						tokenSequenceIndex=0;
						shiftToken=0;
						reduceToken=0;
						return currentState;
					}
				}
				return -1;
			}

			vint ParsingState::GetCurrentToken()
			{
				return currentToken;
			}

			const collections::List<vint>& ParsingState::GetStateStack()
			{
				return stateStack;
			}

			vint ParsingState::GetCurrentState()
			{
				return currentState;
			}

			ParsingTable::TransitionItem* ParsingState::MatchToken(vint tableTokenIndex)
			{
				Future future;
				future.currentState=currentState;
				return MatchTokenInFuture(tableTokenIndex, &future);
			}

			ParsingTable::TransitionItem* ParsingState::MatchTokenInFuture(vint tableTokenIndex, Future* future)
			{
				ParsingTable::TransitionBag* bag=table->GetTransitionBag(future->currentState, tableTokenIndex).Obj();
				if(bag)
				{
					for(vint i=0;i<bag->transitionItems.Count();i++)
					{
						ParsingTable::TransitionItem* item=bag->transitionItems[i].Obj();
						vint availableStackDepth=stateStack.Count()-future->reduceStateCount;
						vint totalStackDepth=stateStack.Count()-future->reduceStateCount+future->shiftStates.Count();
						if(item->stackPattern.Count()<=totalStackDepth)
						{
							if(tableTokenIndex!=ParsingTable::TokenFinish || item->stackPattern.Count()==totalStackDepth)
							{
								bool match=true;
								for(vint j=0;j<item->stackPattern.Count();j++)
								{
									vint state=
										j<future->shiftStates.Count()
										?future->shiftStates[future->shiftStates.Count()-1-j]
										:stateStack[availableStackDepth-1-(j-future->shiftStates.Count())]
										;
									if(item->stackPattern[j]!=state)
									{
										match=false;
									}
								}
								if(match)
								{
									return item;
								}
							}
						}
					}
				}
				return 0;
			}

			ParsingState::TransitionResult ParsingState::ReadToken(vint tableTokenIndex, regex::RegexToken* regexToken)
			{
				ParsingTable::TransitionItem* item=MatchToken(tableTokenIndex);
				if(item)
				{
					if(regexToken)
					{
						if(!shiftToken)
						{
							shiftToken=regexToken;
							reduceToken=regexToken;
						}
					}
					if(tableTokenIndex>=ParsingTable::UserTokenStart)
					{
						if(tokenSequenceIndex==0)
						{
							shiftTokenStack.Add(shiftToken);
						}
						tokenSequenceIndex++;
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
								result.AddShiftReduceRange(shiftToken, reduceToken);
								if(regexToken)
								{
									reduceToken=regexToken;
								}
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
				return TransitionResult();
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

			bool ParsingState::ReadTokenInFuture(vint tableTokenIndex, Future* previous, Future* now)
			{
				ParsingTable::TransitionItem* selectedItem=0;
				if(previous)
				{
					selectedItem=MatchTokenInFuture(tableTokenIndex, previous);
				}
				else
				{
					selectedItem=MatchToken(tableTokenIndex);
				}

				if(selectedItem)
				{
					if(previous)
					{
						now->reduceStateCount=previous->reduceStateCount;
						CopyFrom(now->shiftStates, previous->shiftStates);
					}
					else
					{
						now->reduceStateCount=0;
						now->shiftStates.Clear();
					}
					now->currentState=selectedItem->targetState;
					now->selectedToken=tableTokenIndex;
					now->previous=previous;
					now->next=0;

					for(vint j=0;j<selectedItem->instructions.Count();j++)
					{
						ParsingTable::Instruction& ins=selectedItem->instructions[j];
						switch(ins.instructionType)
						{
						case ParsingTable::Instruction::Shift:
							{
								now->shiftStates.Add(ins.stateParameter);
							}
							break;
						case ParsingTable::Instruction::Reduce:
							{
								if(now->shiftStates.Count()==0)
								{
									now->reduceStateCount++;
								}
								else
								{
									now->shiftStates.RemoveAt(now->shiftStates.Count()-1);
								}
							}
							break;
						}
					}

					return true;
				}
				else
				{
					return false;
				}
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
								Ptr<ParsingTreeToken> value;
								if(result.token==0)
								{
									value=new ParsingTreeToken(L"", result.tokenIndexInStream);
								}
								else
								{
									value=new ParsingTreeToken(WString(result.token->reading, result.token->length), result.tokenIndexInStream);
									value->SetCodeRange(ParsingTextRange(result.token, result.token));
								}
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
							ParsingTextRange arrRange=arr->GetCodeRange();
							ParsingTextRange itemRange;
							if(!createdObject)
							{
								Ptr<ParsingTreeToken> value;
								if(result.token==0)
								{
									value=new ParsingTreeToken(L"", result.tokenIndexInStream);
								}
								else
								{
									value=new ParsingTreeToken(WString(result.token->reading, result.token->length), result.tokenIndexInStream);
									value->SetCodeRange(ParsingTextRange(result.token, result.token));
									itemRange=value->GetCodeRange();
								}
								arr->AddItem(value);
							}
							else
							{
								arr->AddItem(createdObject);
								itemRange=createdObject->GetCodeRange();
								createdObject=0;
							}

							if(arrRange.start.index==ParsingTextPos::UnknownValue || itemRange.start<arrRange.start)
							{
								arrRange.start=itemRange.start;
							}
							if(arrRange.end.index==ParsingTextPos::UnknownValue || itemRange.end>arrRange.end)
							{
								arrRange.end=itemRange.end;
							}
							arr->SetCodeRange(arrRange);
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
		}
	}
}