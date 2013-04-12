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
					if(!builder.Run(result))
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
				errors.Add(new ParsingError(token, (token==0?L"Error happened during parsing when reaching to the end of the input.":L"Error happened during parsing.")));
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
					while(true)
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
					return ParsingState::TransitionResult();
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

			bool ParsingAmbiguousParser::IsAmbiguityResolvable(collections::List<ParsingState::Future*>& futures, vint begin, vint end)
			{
				for(vint i=begin;i<end-1;i++)
				{
					ParsingState::Future* first=futures[i];
					ParsingState::Future* second=futures[i+1];

					if(first->currentState!=second->currentState
						|| first->reduceStateCount!=second->reduceStateCount
						|| first->shiftStates.Count()!=second->shiftStates.Count()
						)
					{
						return false;
					}
					else
					{
						for(vint j=0;j<first->shiftStates.Count();j++)
						{
							if(first->shiftStates[j]!=second->shiftStates[j])
							{
								return false;
							}
						}
					}
				}
				return true;
			}

			void ParsingAmbiguousParser::SearchPath(ParsingState& state, collections::List<ParsingState::Future*>& futures, collections::List<regex::RegexToken*>& tokens, vint& begin, vint& end, collections::List<Ptr<ParsingError>>& errors)
			{
				futures.Add(state.ExploreCreateRootFuture());
				vint previousBegin=0;
				vint previousEnd=1;

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
					
					if(IsAmbiguityResolvable(futures, previousBegin, previousEnd))
					{
						break;
					}
				}

				begin=previousBegin;
				end=previousEnd;
			}

			void ParsingAmbiguousParser::BuildAmbiguousDecisions(ParsingState& state, collections::List<ParsingState::Future*>& futures, collections::List<regex::RegexToken*>& tokens, vint begin, vint end, collections::List<Ptr<ParsingError>>& errors)
			{
				vint conflictReduceCount=-1;
				for(vint i=begin;i<end-1;i++)
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

				Array<vint> conflictReduceIndices(end-begin);
				for(vint i=begin;i<end;i++)
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
					conflictReduceIndices[i-begin]=index;
				}

				vint affectedStackNodeCount=-1;
				for(vint i=begin;i<end;i++)
				{
					ParsingState::Future* future=futures[i];
					vint count=1;
					while(future && future->selectedItem)
					{
						vint start=(future==futures[i]?conflictReduceIndices[i-begin]:future->selectedItem->instructions.Count())-1;
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
						goto FAIL_TO_CHECK;
					}
				}

				BuildDeterministicDecisions(state, futures, tokens, begin, end, errors);
			FAIL_TO_CHECK:
				const RegexToken* token=state.GetToken(state.GetCurrentToken());
				errors.Add(new ParsingError(token, (token==0?L"Failed to pass ambiguity checking during parsing when reaching to the end of the input.":L"Failed to pass ambiguity checking during parsing.")));
			}

			void ParsingAmbiguousParser::BuildDeterministicDecisions(ParsingState& state, collections::List<ParsingState::Future*>& futures, collections::List<regex::RegexToken*>& tokens, vint begin, vint end, collections::List<Ptr<ParsingError>>& errors)
			{
				ParsingState::Future* currentFuture=futures[begin];
				vint currentRegexToken=tokens.Count()-1;
				List<Pair<ParsingTable::TransitionItem*, regex::RegexToken*>> path;
				while(currentFuture && currentFuture->selectedToken!=-1)
				{
					regex::RegexToken* token=0;
					if(currentFuture->selectedToken>=ParsingTable::UserTokenStart)
					{
						token=tokens[currentRegexToken--];
					}
					path.Add(Pair<ParsingTable::TransitionItem*, regex::RegexToken*>(currentFuture->selectedItem, token));
					currentFuture=currentFuture->previous;
				}

				for(vint i=path.Count()-1;i>=0;i--)
				{
					decisions.Add(state.RunTransition(path[i].key, path[i].value));
				}
			}

			void ParsingAmbiguousParser::BuildDecisions(ParsingState& state, collections::List<ParsingState::Future*>& futures, collections::List<regex::RegexToken*>& tokens, vint begin, vint end, collections::List<Ptr<ParsingError>>& errors)
			{
				if(end-begin==0)
				{
					const RegexToken* token=state.GetToken(state.GetCurrentToken());
					errors.Add(new ParsingError(token, (token==0?L"Error happened during parsing when reaching to the end of the input.":L"Error happened during parsing.")));
				}
				else if(end-begin==1)
				{
					BuildDeterministicDecisions(state, futures, tokens, begin, end, errors);
				}
				else
				{
					BuildAmbiguousDecisions(state, futures, tokens, begin, end, errors);
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

					SearchPath(state, futures, tokens, resultBegin, resultEnd, errors);
					BuildDecisions(state, futures, tokens, resultBegin, resultEnd, errors);

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
					return new ParsingAutoRecoverParser(table);
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