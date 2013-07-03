#include "Parsing.h"
#include "ParsingAutomaton.h"
#include "..\Collections\OperationForEach.h"
#include "..\Collections\OperationCopyFrom.h"

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
ParsingGeneralParser
***********************************************************************/

			ParsingGeneralParser::ParsingGeneralParser(Ptr<ParsingTable> _table)
				:table(_table)
			{
			}

			ParsingGeneralParser::~ParsingGeneralParser()
			{
			}

			void ParsingGeneralParser::BeginParse()
			{
			}

			Ptr<ParsingTreeNode> ParsingGeneralParser::Parse(ParsingState& state, collections::List<Ptr<ParsingError>>& errors)
			{
				BeginParse();
				ParsingTreeBuilder builder;
				builder.Reset();

				for(vint i=0;i<state.GetTokens().Count();i++)
				{
					const RegexToken* token=&state.GetTokens().Get(i);
					if(token->token==-1)
					{
						errors.Add(new ParsingError(token, L"Unrecognizable token: \""+WString(token->reading, token->length)+L"\"."));
					}
				}

				ParsingState::TransitionResult result;
				while(true)
				{
					ParsingState::TransitionResult result=ParseStep(state, errors);
					if(!result)
					{
						const RegexToken* token=state.GetToken(state.GetCurrentToken());
						errors.Add(new ParsingError(token, L"Internal error when parsing."));
						return 0;
					}
					else if(result.transitionType==ParsingState::TransitionResult::SkipToken)
					{
						if(result.tableTokenIndex==ParsingTable::TokenFinish)
						{
							const RegexToken* token=state.GetToken(state.GetCurrentToken());
							errors.Add(new ParsingError(token, L"Failed to recover error when reaching the end of the input."));
							return 0;
						}
						else
						{
							state.SkipCurrentToken();
							continue;
						}
					}
					else if(!builder.Run(result))
					{
						const RegexToken* token=state.GetToken(state.GetCurrentToken());
						errors.Add(new ParsingError(token, L"Internal error when building the parsing tree."));
						return 0;
					}
					if(result.tableTokenIndex==ParsingTable::TokenFinish)
					{
						break;
					}
				}

				Ptr<ParsingTreeNode> node=builder.GetNode();
				if(!node)
				{
					errors.Add(new ParsingError(L"Internal error when building the parsing tree after a succeeded parsing process."));
					return 0;
				}
				return node;
			}

			Ptr<ParsingTreeNode> ParsingGeneralParser::Parse(const WString& input, const WString& rule, collections::List<Ptr<ParsingError>>& errors)
			{
				ParsingState state(input, table);
				if(state.Reset(rule)==-1)
				{
					errors.Add(new ParsingError(L"Rule \""+rule+L"\" does not exist."));
					return 0;
				}
				return Parse(state, errors);
			}

/***********************************************************************
ParsingStrictParser
***********************************************************************/

			ParsingState::TransitionResult ParsingStrictParser::OnErrorRecover(ParsingState& state, const regex::RegexToken* currentToken, collections::List<Ptr<ParsingError>>& errors)
			{
				const RegexToken* token=state.GetToken(state.GetCurrentToken());
				errors.Add(new ParsingError(token, (token==0?L"Error happened during parsing when reaching the end of the input.":L"Error happened during parsing.")));
				return ParsingState::TransitionResult();
			}

			ParsingStrictParser::ParsingStrictParser(Ptr<ParsingTable> _table)
				:ParsingGeneralParser(_table)
			{
			}

			ParsingStrictParser::~ParsingStrictParser()
			{
			}
			
			ParsingState::TransitionResult ParsingStrictParser::ParseStep(ParsingState& state, collections::List<Ptr<ParsingError>>& errors)
			{
				ParsingState::TransitionResult result=state.ReadToken();
				if(!result)
				{
					const RegexToken* token=state.GetToken(state.GetCurrentToken());
					result=OnErrorRecover(state, token, errors);
				}
				return result;
			}

/***********************************************************************
ParsingAutoRecoverParser
***********************************************************************/

			ParsingState::TransitionResult ParsingAutoRecoverParser::OnErrorRecover(ParsingState& state, const regex::RegexToken* currentToken, collections::List<Ptr<ParsingError>>& errors)
			{
				vint targetTableTokenIndex=(currentToken?table->GetTableTokenIndex(currentToken->token):ParsingTable::TokenFinish);
				if(recoveringFutureIndex==-1)
				{
					vint processingFutureIndex=-1;
					vint usedFutureCount=0;
					while(processingFutureIndex<recoverFutures.Count())
					{
						ParsingState::Future* previous=0;
						if(processingFutureIndex!=-1)
						{
							previous=&recoverFutures[processingFutureIndex];
						}
						processingFutureIndex++;

						vint currentTableTokenIndex=0;
						while(currentTableTokenIndex<table->GetTokenCount() && usedFutureCount<recoverFutures.Count())
						{
							ParsingState::Future* now=&recoverFutures[usedFutureCount];
							if(state.ReadTokenInFuture(currentTableTokenIndex, previous, now, 0))
							{
								if(currentTableTokenIndex==targetTableTokenIndex)
								{
									ParsingState::Future* future=previous;
									while(future->previous)
									{
										future->previous->next=future;
										future=future->previous;
									}
									recoveringFutureIndex=future-&recoverFutures[0];
									goto FOUND_ERROR_RECOVER_SOLUTION;
								}
								else
								{
									usedFutureCount++;
								}
							}
							currentTableTokenIndex++;
						}
					}
				}
			FOUND_ERROR_RECOVER_SOLUTION:

				ParsingState::Future* selectedFuture=0;
				if(recoveringFutureIndex!=-1)
				{
					selectedFuture=&recoverFutures[recoveringFutureIndex];
					if(selectedFuture->next)
					{
						recoveringFutureIndex+=selectedFuture->next-selectedFuture;
					}
					else
					{
						recoveringFutureIndex=-1;
					}
				}

				if(selectedFuture)
				{
					return state.RunTransition(selectedFuture->selectedItem, 0);
				}
				else
				{
					return ParsingState::TransitionResult(ParsingState::TransitionResult::SkipToken);
				}
			}

			ParsingAutoRecoverParser::ParsingAutoRecoverParser(Ptr<ParsingTable> _table)
				:ParsingStrictParser(_table)
				,recoverFutures(65536)
				,recoveringFutureIndex(-1)
			{
			}

			ParsingAutoRecoverParser::~ParsingAutoRecoverParser()
			{
			}

/***********************************************************************
ParsingStrictParser
***********************************************************************/

			ParsingAmbiguousParser::ParsingAmbiguousParser(Ptr<ParsingTable> _table)
				:ParsingGeneralParser(_table)
				,consumedDecisionCount(0)
			{
			}

			ParsingAmbiguousParser::~ParsingAmbiguousParser()
			{
			}

			vint ParsingAmbiguousParser::GetResolvableFutureLevels(collections::List<ParsingState::Future*>& futures, vint begin, vint end)
			{
				if(end-begin<2)
				{
					return 1;
				}
				List<ParsingState::Future*> resolvingFutures;
				for(vint i=begin;i<end;i++)
				{
					resolvingFutures.Add(futures[i]);
				}

				vint level=0;
				while(true)
				{
					for(vint i=0;i<resolvingFutures.Count()-1;i++)
					{
						ParsingState::Future* first=resolvingFutures[i];
						ParsingState::Future* second=resolvingFutures[i+1];

						if(first->currentState!=second->currentState
							|| first->reduceStateCount!=second->reduceStateCount
							|| first->shiftStates.Count()!=second->shiftStates.Count()
							)
						{
							return level;
						}
						else
						{
							for(vint j=0;j<first->shiftStates.Count();j++)
							{
								if(first->shiftStates[j]!=second->shiftStates[j])
								{
									return level;
								}
							}
						}
					}
					level++;

					for(vint i=0;i<resolvingFutures.Count();i++)
					{
						if(!(resolvingFutures[i]=resolvingFutures[i]->previous))
						{
							return level;
						}
					}
				}
			}

			vint ParsingAmbiguousParser::SearchPathForOneStep(ParsingState& state, collections::List<ParsingState::Future*>& futures, collections::List<regex::RegexToken*>& tokens, vint& begin, vint& end, collections::List<Ptr<ParsingError>>& errors)
			{
				futures.Add(state.ExploreCreateRootFuture());
				vint previousBegin=0;
				vint previousEnd=1;
				vint resolvableFutureLevels=0;

				while(true)
				{
					regex::RegexToken* token=state.ExploreStep(futures, previousBegin, previousEnd-previousBegin, futures);
					if(futures.Count()==previousEnd)
					{
						state.ExploreTryReduce(futures, previousBegin, previousEnd-previousBegin, futures);
					}

					if(futures.Count()>previousEnd && token)
					{
						tokens.Add(token);
					}
					previousBegin=previousEnd;
					previousEnd=futures.Count();
					
					resolvableFutureLevels=GetResolvableFutureLevels(futures, previousBegin, previousEnd);
					if(resolvableFutureLevels!=0)
					{
						break;
					}
				}

				begin=previousBegin;
				end=previousEnd;
				return resolvableFutureLevels;
			}

			vint ParsingAmbiguousParser::GetConflictReduceCount(collections::List<ParsingState::Future*>& futures)
			{
				vint conflictReduceCount=-1;
				for(vint i=0;i<futures.Count()-1;i++)
				{
					vint count=0;
					ParsingState::Future* first=futures[i];
					ParsingState::Future* second=futures[i+1];
					vint firstIndex=first->selectedItem->instructions.Count();
					vint secondIndex=second->selectedItem->instructions.Count();
					while(--firstIndex>=0 && --secondIndex>=0)
					{
						ParsingTable::Instruction* firstIns=&first->selectedItem->instructions[firstIndex];
						ParsingTable::Instruction* secondIns=&second->selectedItem->instructions[secondIndex];
						if(firstIns && secondIns)
						{
							if(firstIns->instructionType==secondIns->instructionType
								&& firstIns->nameParameter==secondIns->nameParameter
								&& firstIns->stateParameter==secondIns->stateParameter
								&& firstIns->value==secondIns->value
								)
							{
								if(firstIns->instructionType==ParsingTable::Instruction::Reduce || firstIns->instructionType==ParsingTable::Instruction::LeftRecursiveReduce)
								{
									count++;
								}
								continue;
							}
						}
						break;
					}
					if(conflictReduceCount==-1 || conflictReduceCount>count)
					{
						conflictReduceCount=count;
					}
				}
				if(conflictReduceCount==-1)
				{
					conflictReduceCount=0;
				}
				return conflictReduceCount;
			}

			void ParsingAmbiguousParser::GetConflictReduceIndices(collections::List<ParsingState::Future*>& futures, vint conflictReduceCount, collections::Array<vint>& conflictReduceIndices)
			{
				conflictReduceIndices.Resize(futures.Count());
				for(vint i=0;i<futures.Count();i++)
				{
					ParsingState::Future* future=futures[i];
					vint index=future->selectedItem->instructions.Count();
					vint count=0;
					while(count<conflictReduceCount && index>0)
					{
						switch(future->selectedItem->instructions[--index].instructionType)
						{
						case ParsingTable::Instruction::Reduce:
						case ParsingTable::Instruction::LeftRecursiveReduce:
							count++;
							break;
						}
					}
					conflictReduceIndices[i]=index;
				}
			}
			vint ParsingAmbiguousParser::GetAffectedStackNodeCount(collections::List<ParsingState::Future*>& futures, collections::Array<vint>& conflictReduceIndices)
			{
				vint affectedStackNodeCount=-1;
				for(vint i=0;i<futures.Count();i++)
				{
					ParsingState::Future* future=futures[i];
					vint count=1;
					while(future && future->selectedItem)
					{
						vint start=(future==futures[i]?conflictReduceIndices[i]:future->selectedItem->instructions.Count())-1;
						for(vint j=start;j>=0;j--)
						{
							switch(future->selectedItem->instructions[j].instructionType)
							{
							case ParsingTable::Instruction::Reduce:
								count++;
								break;
							case ParsingTable::Instruction::Shift:
								count--;
								break;
							}
						}
						future=future->previous;
					}

					if(affectedStackNodeCount==-1)
					{
						affectedStackNodeCount=count;
					}
					else if(affectedStackNodeCount!=count)
					{
						return -1;
					}
				}
				return affectedStackNodeCount;
			}

			void ParsingAmbiguousParser::BuildSingleDecisionPath(ParsingState& state, ParsingState::Future* future, collections::List<regex::RegexToken*>& tokens, vint availableTokenCount, vint lastAvailableInstructionCount)
			{
				vint currentRegexToken=availableTokenCount-1;
				List<Pair<ParsingTable::TransitionItem*, regex::RegexToken*>> path;
				while(future && future->selectedToken!=-1)
				{
					regex::RegexToken* token=0;
					if(future->selectedToken>=ParsingTable::UserTokenStart)
					{
						token=tokens[currentRegexToken--];
					}
					path.Add(Pair<ParsingTable::TransitionItem*, regex::RegexToken*>(future->selectedItem, token));
					future=future->previous;
				}

				for(vint j=path.Count()-1;j>=0;j--)
				{
					if(j==0 && lastAvailableInstructionCount!=-1)
					{
						decisions.Add(state.RunTransition(path[j].key, path[j].value, 0, lastAvailableInstructionCount, false));
					}
					else
					{
						decisions.Add(state.RunTransition(path[j].key, path[j].value));
					}
				}
			}

			void ParsingAmbiguousParser::BuildAmbiguousDecisions(ParsingState& state, collections::List<ParsingState::Future*>& futures, collections::List<regex::RegexToken*>& tokens, vint begin, vint end, vint resolvableFutureLevels, collections::List<Ptr<ParsingError>>& errors)
			{
				vint availableTokenCount=tokens.Count();
				List<ParsingState::Future*> resolvingFutures;
				for(vint i=begin;i<end;i++)
				{
					resolvingFutures.Add(futures[i]);
				}
				for(vint i=1;i<resolvableFutureLevels;i++)
				{
					if(resolvingFutures[0]->selectedToken>=ParsingTable::UserTokenStart)
					{
						availableTokenCount--;
					}
					for(vint j=0;j<resolvingFutures.Count();j++)
					{
						resolvingFutures[j]=resolvingFutures[j]->previous;
					}
				}

				Array<vint> conflictReduceIndices;
				vint conflictReduceCount=GetConflictReduceCount(resolvingFutures);
				GetConflictReduceIndices(resolvingFutures, conflictReduceCount, conflictReduceIndices);

				WString ambiguityNodeType, ambiguityRuleName;
				if(resolvingFutures[0]->selectedItem->instructions.Count()==conflictReduceIndices[0])
				{
					vint rootStartState=state.GetParsingRuleStartState();
					ambiguityNodeType=state.GetTable()->GetStateInfo(rootStartState).ruleAmbiguousType;
					ambiguityRuleName=state.GetParsingRule();
				}
				else
				{
					ParsingTable::Instruction& ins=resolvingFutures[0]->selectedItem->instructions[conflictReduceIndices[0]];
					ambiguityNodeType=state.GetTable()->GetStateInfo(ins.stateParameter).ruleAmbiguousType;
					ambiguityRuleName=state.GetTable()->GetStateInfo(ins.stateParameter).ruleName;
				}
				if(ambiguityNodeType==L"")
				{
					const RegexToken* token=state.GetToken(state.GetCurrentToken());
					errors.Add(new ParsingError(token, L"Ambiguity happens when reducing rule \""+ambiguityRuleName+L"\" but this rule does not have an associated ambiguous node type."));
					return;
				}

				vint affectedStackNodeCount=GetAffectedStackNodeCount(resolvingFutures, conflictReduceIndices);
				if(affectedStackNodeCount==-1)
				{
					const RegexToken* token=state.GetToken(state.GetCurrentToken());
					errors.Add(new ParsingError(token, (token==0?L"Failed to pass ambiguity checking during parsing when reaching to the end of the input.":L"Failed to pass ambiguity checking during parsing.")));
					return;
				}

				Ptr<ParsingState::StateGroup> stateGroup;
				for(vint i=0;i<resolvingFutures.Count();i++)
				{
					if(stateGroup)
					{
						state.RestoreSnapshot(stateGroup);
					}
					else
					{
						stateGroup=state.TakeSnapshot();
					}
					{
						ParsingState::TransitionResult result(
							i==0
							?ParsingState::TransitionResult::AmbiguityBegin
							:ParsingState::TransitionResult::AmbiguityBranch
							);
						if(i==0)
						{
							result.ambiguityAffectedStackNodeCount=affectedStackNodeCount;
						}
						decisions.Add(result);
					}
					{
						BuildSingleDecisionPath(state, resolvingFutures[i], tokens, availableTokenCount, conflictReduceIndices[i]);

						if(i==resolvingFutures.Count()-1)
						{
							ParsingState::TransitionResult result(ParsingState::TransitionResult::AmbiguityEnd);
							result.ambiguityNodeType=ambiguityNodeType;
							decisions.Add(result);

							vint start=conflictReduceIndices[i];
							ParsingState::TransitionResult lastDecision=decisions[decisions.Count()-2];
							decisions.Add(state.RunTransition(lastDecision.transition, lastDecision.token, start, lastDecision.transition->instructions.Count()-start, true));
						}
					}
				}

				ParsingState::Future* lastFuture=futures[end-1];
				ParsingState::Future** futureCleaner=&lastFuture;
				for(int i=1;i<resolvableFutureLevels;i++)
				{
					futureCleaner=&(*futureCleaner)->previous;
				}
				*futureCleaner=0;

				if(lastFuture)
				{
					BuildSingleDecisionPath(state, lastFuture, tokens, tokens.Count(), -1);
				}
			}

			void ParsingAmbiguousParser::BuildDecisions(ParsingState& state, collections::List<ParsingState::Future*>& futures, collections::List<regex::RegexToken*>& tokens, vint begin, vint end, vint resolvableFutureLevels, collections::List<Ptr<ParsingError>>& errors)
			{
				if(end-begin==0)
				{
					const RegexToken* token=state.GetToken(state.GetCurrentToken());
					errors.Add(new ParsingError(token, (token==0?L"Error happened during parsing when reaching to the end of the input.":L"Error happened during parsing.")));
				}
				else if(end-begin==1)
				{
					BuildSingleDecisionPath(state, futures[begin], tokens, tokens.Count(), -1);
				}
				else
				{
					BuildAmbiguousDecisions(state, futures, tokens, begin, end, resolvableFutureLevels, errors);
				}
			}
			
			ParsingState::TransitionResult ParsingAmbiguousParser::ParseStep(ParsingState& state, collections::List<Ptr<ParsingError>>& errors)
			{
				if(decisions.Count()==consumedDecisionCount)
				{
					List<ParsingState::Future*> futures;
					List<regex::RegexToken*> tokens;
					vint resultBegin=0;
					vint resultEnd=0;

					vint resolvableFutureLevels=SearchPathForOneStep(state, futures, tokens, resultBegin, resultEnd, errors);
					BuildDecisions(state, futures, tokens, resultBegin, resultEnd, resolvableFutureLevels, errors);

					FOREACH(ParsingState::Future*, future, futures)
					{
						delete future;
					}
				}

				if(decisions.Count()>consumedDecisionCount)
				{
					ParsingState::TransitionResult result=decisions[consumedDecisionCount++];
					if(consumedDecisionCount==decisions.Count())
					{
						decisions.Clear();
						consumedDecisionCount=0;
					}
					return result;
				}
				else
				{
					return ParsingState::TransitionResult();
				}
			}

			void ParsingAmbiguousParser::BeginParse()
			{
				decisions.Clear();
				consumedDecisionCount=0;
			}

/***********************************************************************
ParsingAutoRecoverAmbiguousParser
***********************************************************************/

			ParsingAutoRecoverAmbiguousParser::ParsingAutoRecoverAmbiguousParser(Ptr<ParsingTable> _table)
				:ParsingAmbiguousParser(_table)
			{
			}

			ParsingAutoRecoverAmbiguousParser::~ParsingAutoRecoverAmbiguousParser()
			{
			}

/***********************************************************************
¸¨Öúº¯Êý
***********************************************************************/

			Ptr<ParsingGeneralParser> CreateStrictParser(Ptr<ParsingTable> table)
			{
				if(table)
				{
					if(table->GetAmbiguity())
					{
						return new ParsingAmbiguousParser(table);
					}
					else
					{
						return new ParsingStrictParser(table);
					}
				}
				else
				{
					return 0;
				}
			}

			Ptr<ParsingGeneralParser> CreateAutoRecoverParser(Ptr<ParsingTable> table)
			{
				if(table)
				{
					if(table->GetAmbiguity())
					{
						return new ParsingAutoRecoverAmbiguousParser(table);
					}
					else
					{
						return new ParsingAutoRecoverParser(table);
					}
				}
				else
				{
					return 0;
				}
			}

			Ptr<ParsingGeneralParser> CreateBootstrapStrictParser()
			{
				List<Ptr<ParsingError>> errors;
				Ptr<ParsingDefinition> definition=CreateParserDefinition();
				Ptr<ParsingTable> table=GenerateTable(definition, false, errors);
				return CreateStrictParser(table);
			}

			Ptr<ParsingGeneralParser> CreateBootstrapAutoRecoverParser()
			{
				List<Ptr<ParsingError>> errors;
				Ptr<ParsingDefinition> definition=CreateParserDefinition();
				Ptr<ParsingTable> table=GenerateTable(definition, false, errors);
				return CreateAutoRecoverParser(table);
			}
		}
	}
}