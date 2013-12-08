#include "Parsing.h"
#include "ParsingAutomaton.h"
#include "../Collections/OperationForEach.h"
#include "../Collections/OperationCopyFrom.h"

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

			Ptr<ParsingTable> ParsingGeneralParser::GetTable()
			{
				return table;
			}

			void ParsingGeneralParser::BeginParse()
			{
			}

			bool ParsingGeneralParser::Parse(ParsingState& state, ParsingTransitionProcessor& processor, collections::List<Ptr<ParsingError>>& errors)
			{
				BeginParse();
				processor.Reset();

				for(vint i=0;i<state.GetTokens().Count();i++)
				{
					const RegexToken* token=&state.GetTokens().Get(i);
					if(token->token==-1 || !token->completeToken)
					{
						errors.Add(new ParsingError(token, L"Unrecognizable token: \""+WString(token->reading, token->length)+L"\"."));
					}
				}

				while(true)
				{
					ParsingState::TransitionResult result=ParseStep(state, errors);
					if(!result)
					{
						const RegexToken* token=state.GetToken(state.GetCurrentToken());
						errors.Add(new ParsingError(token, L"Internal error when parsing."));
						return false;
					}
					else if(result.transitionType==ParsingState::TransitionResult::SkipToken)
					{
						if(state.GetCurrentTableTokenIndex()==ParsingTable::TokenFinish)
						{
							const RegexToken* token=state.GetToken(state.GetCurrentToken());
							errors.Add(new ParsingError(token, L"Failed to recover error when reaching the end of the input."));
							return false;
						}
						else
						{
							state.SkipCurrentToken();
							continue;
						}
					}
					else if(!processor.Run(result))
					{
						const RegexToken* token=state.GetToken(state.GetCurrentToken());
						errors.Add(new ParsingError(token, L"Internal error when building the parsing tree."));
						return false;
					}
					if(result.tableTokenIndex==ParsingTable::TokenFinish && !processor.GetProcessingAmbiguityBranch())
					{
						break;
					}
				}

				return true;
			}

			Ptr<ParsingTreeNode> ParsingGeneralParser::Parse(ParsingState& state, collections::List<Ptr<ParsingError>>& errors)
			{
				ParsingTreeBuilder builder;
				Parse(state, builder, errors);

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

			ParsingState::TransitionResult ParsingStrictParser::OnErrorRecover(ParsingState& state, vint currentTokenIndex, const regex::RegexToken* currentToken, collections::List<Ptr<ParsingError>>& errors)
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
					const RegexToken* currentToken=state.GetToken(state.GetCurrentToken());
					vint currentTokenIndex=(currentToken?table->GetTableTokenIndex(currentToken->token):ParsingTable::TokenFinish);
					result=OnErrorRecover(state, currentTokenIndex, currentToken, errors);
				}
				return result;
			}

/***********************************************************************
ParsingAutoRecoverParser
***********************************************************************/

			ParsingState::TransitionResult ParsingAutoRecoverParser::OnErrorRecover(ParsingState& state, vint currentTokenIndex, const regex::RegexToken* currentToken, collections::List<Ptr<ParsingError>>& errors)
			{
				if(recoveringFutureIndex==-1)
				{
					vint processingFutureIndex=-1;
					vint usedFutureCount=0;
					while(processingFutureIndex<usedFutureCount)
					{
						ParsingState::Future* previous=0;
						if(processingFutureIndex!=-1)
						{
							previous=&recoverFutures[processingFutureIndex];
						}
						processingFutureIndex++;
						if(previous && previous->currentState==-1) continue;

						vint currentTableTokenIndex=0;
						while(currentTableTokenIndex<table->GetTokenCount() && usedFutureCount<recoverFutures.Count())
						{
							ParsingState::Future* now=&recoverFutures[usedFutureCount];
							if(state.ReadTokenInFuture(currentTableTokenIndex, previous, now, 0))
							{
								if(currentTableTokenIndex==currentTokenIndex)
								{
									if(previous)
									{
										ParsingState::Future* future=previous;
										while(future->previous)
										{
											future->previous->next=future;
											future=future->previous;
										}
										recoveringFutureIndex=future-&recoverFutures[0];
									}
									else
									{
										recoveringFutureIndex=0;
									}
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
				,recoverFutures(1024)
				,recoveringFutureIndex(-1)
			{
			}

			ParsingAutoRecoverParser::~ParsingAutoRecoverParser()
			{
			}

			void ParsingAutoRecoverParser::BeginParse()
			{
				recoveringFutureIndex = -1;
			}

/***********************************************************************
ParsingAmbiguousParser
***********************************************************************/

			ParsingAmbiguousParser::ParsingAmbiguousParser(Ptr<ParsingTable> _table)
				:ParsingGeneralParser(_table)
				,consumedDecisionCount(0)
			{
			}

			ParsingAmbiguousParser::~ParsingAmbiguousParser()
			{
			}

			void ParsingAmbiguousParser::OnErrorRecover(ParsingState& state, vint currentTokenIndex, collections::List<ParsingState::Future*>& futures, vint& begin, vint& end, collections::List<Ptr<ParsingError>>& errors)
			{
				begin=end;
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

			vint ParsingAmbiguousParser::SearchPathForOneStep(ParsingState& state, collections::List<ParsingState::Future*>& futures, vint& begin, vint& end, collections::List<Ptr<ParsingError>>& errors)
			{
				futures.Add(state.ExploreCreateRootFuture());
				vint previousBegin = 0;
				vint previousEnd = 1;
				vint resolvableFutureLevels = 0;
				bool errorRecovered = false;

				while(true)
				{
					// keep all futures that consumed a token in a list
					List<ParsingState::Future*> consumedTokenFutures;
					vint processBegin = previousBegin;
					vint processEnd = previousEnd;
					while (processEnd > processBegin)
					{
						// explore the current token
						if (state.ExploreStep(futures, processBegin, processEnd - processBegin, futures))
						{
							CopyFrom(
								consumedTokenFutures,
								From(futures)
									.Skip(processEnd)
									.Take(futures.Count() - processEnd),
								true
								);
							futures.RemoveRange(processEnd, futures.Count() - processEnd);
						}

						// explore left recursive reduce and normal reduce
						state.ExploreLeftRecursiveReduce(futures, processBegin, processEnd - processBegin, futures);
						state.ExploreNormalReduce(futures, processBegin, processEnd - processBegin, futures);

						// if a token is consumed, then for those reduce futures, explore them until a token is consumed, and discard all failed futures
						processBegin = processEnd;
						processEnd = futures.Count();
					}

					if (consumedTokenFutures.Count() == 0)
					{
						// failed to get any future that consumed a token, do error recovering
						vint tokenIndex = state.GetCurrentTableTokenIndex();
						OnErrorRecover(state, tokenIndex, futures, previousBegin, previousEnd, errors);
						if (previousBegin == previousEnd)
						{
							break;
						}
					}
					else
					{
						state.SkipCurrentToken();
						// put all futures that consumed a token from consumedTokenFutures back to future list
						previousBegin = futures.Count();
						CopyFrom(futures, consumedTokenFutures, true);
						previousEnd = futures.Count();

						// resolve all futures and see if all futures collapsed into a equivalent single future
						resolvableFutureLevels = GetResolvableFutureLevels(futures, previousBegin, previousEnd);
						if (resolvableFutureLevels != 0)
						{
							break;
						}
					}
				}

				begin = previousBegin;
				end = previousEnd;
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

			void ParsingAmbiguousParser::BuildSingleDecisionPath(ParsingState& state, ParsingState::Future* future, vint lastAvailableInstructionCount)
			{
				List<Pair<ParsingTable::TransitionItem*, regex::RegexToken*>> path;
				while(future && future->selectedToken!=-1)
				{
					path.Add(Pair<ParsingTable::TransitionItem*, regex::RegexToken*>(future->selectedItem, future->selectedRegexToken));
					future = future->previous;
				}

				for (vint j = path.Count() - 1; j >= 0; j--)
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

			void ParsingAmbiguousParser::BuildAmbiguousDecisions(ParsingState& state, collections::List<ParsingState::Future*>& futures, vint begin, vint end, vint resolvableFutureLevels, collections::List<Ptr<ParsingError>>& errors)
			{
				List<ParsingState::Future*> resolvingFutures;
				CopyFrom(
					resolvingFutures,
					From(futures)
						.Skip(begin)
						.Take(end - begin)
					);
				for (vint i = 1; i < resolvableFutureLevels; i++)
				{
					for(vint j=0;j<resolvingFutures.Count();j++)
					{
						resolvingFutures[j] = resolvingFutures[j]->previous;
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
						BuildSingleDecisionPath(state, resolvingFutures[i], conflictReduceIndices[i]);

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
					BuildSingleDecisionPath(state, lastFuture, -1);
				}
			}

			void ParsingAmbiguousParser::BuildDecisions(ParsingState& state, collections::List<ParsingState::Future*>& futures, vint begin, vint end, vint resolvableFutureLevels, collections::List<Ptr<ParsingError>>& errors)
			{
				if(end-begin==0)
				{
					const RegexToken* token=state.GetToken(state.GetCurrentToken());
					errors.Add(new ParsingError(token, (token==0?L"Error happened during parsing when reaching to the end of the input.":L"Error happened during parsing.")));
				}
				else if(end-begin==1)
				{
					BuildSingleDecisionPath(state, futures[begin], -1);
				}
				else
				{
					BuildAmbiguousDecisions(state, futures, begin, end, resolvableFutureLevels, errors);
				}
			}
			
			ParsingState::TransitionResult ParsingAmbiguousParser::ParseStep(ParsingState& state, collections::List<Ptr<ParsingError>>& errors)
			{
				if(decisions.Count()==consumedDecisionCount)
				{
					List<ParsingState::Future*> futures;
					vint resultBegin=0;
					vint resultEnd=0;

					vint resolvableFutureLevels=SearchPathForOneStep(state, futures, resultBegin, resultEnd, errors);
					BuildDecisions(state, futures, resultBegin, resultEnd, resolvableFutureLevels, errors);

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

			void ParsingAutoRecoverAmbiguousParser::OnErrorRecover(ParsingState& state, vint currentTokenIndex, collections::List<ParsingState::Future*>& futures, vint& begin, vint& end, collections::List<Ptr<ParsingError>>& errors)
			{
				vint oldFutureCount=futures.Count();
				while(futures.Count()-oldFutureCount<65536 && begin<end)
				{
					for(vint i=begin;i<end;i++)
					{
						if(state.TestExplore(currentTokenIndex, futures[i]))
						{
							goto FINISH_ERROR_RECOVERY;
						}
					}
					
					for(vint j=ParsingTable::UserTokenStart;j<table->GetTokenCount();j++)
					{
						if(j!=currentTokenIndex)
						{
							for(vint i=begin;i<end;i++)
							{
								ParsingState::Future* now=futures[i];
								state.Explore(j, now, futures);
							}
						}
					}
					if(futures.Count()==end)
					{
						for(vint i=begin;i<end;i++)
						{
							ParsingState::Future* now=futures[i];
							state.Explore(ParsingTable::LeftRecursiveReduce, now, futures);
						}
					}
					if(futures.Count()==end)
					{
						for(vint i=begin;i<end;i++)
						{
							ParsingState::Future* now=futures[i];
							state.Explore(ParsingTable::NormalReduce, now, futures);
						}
					}

					begin=end;
					end=futures.Count();
				}

			FINISH_ERROR_RECOVERY:
				if(begin==end && currentTokenIndex>=ParsingTable::UserTokenStart)
				{
					// skipped one token
				}
			}

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

/***********************************************************************
·´Éä
***********************************************************************/

#ifndef VCZH_DEBUG_NO_REFLECTION

#include "..\Reflection\GuiTypeDescriptorMacros.h"

namespace vl
{
	namespace reflection
	{
		namespace description
		{
			using namespace parsing;

			PARSINGREFLECTION_TYPELIST(IMPL_TYPE_INFO)

/***********************************************************************
Type Declaration
***********************************************************************/

#define _ ,

			BEGIN_STRUCT_MEMBER(ParsingTextPos)
				STRUCT_MEMBER(index)
				STRUCT_MEMBER(row)
				STRUCT_MEMBER(column)
			END_STRUCT_MEMBER(ParsingTextPos)

			BEGIN_STRUCT_MEMBER(ParsingTextRange)
				STRUCT_MEMBER(start)
				STRUCT_MEMBER(end)
			END_STRUCT_MEMBER(ParsingTextRange)

			BEGIN_CLASS_MEMBER(ParsingTreeNode)
				CLASS_MEMBER_PROPERTY_FAST(CodeRange)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Parent)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(SubNodes)

				CLASS_MEMBER_METHOD(Clone, NO_PARAMETER)
				CLASS_MEMBER_METHOD(InitializeQueryCache, NO_PARAMETER)
				CLASS_MEMBER_METHOD(ClearQueryCache, NO_PARAMETER)
				CLASS_MEMBER_METHOD_OVERLOAD(FindSubNode, {L"position"}, ParsingTreeNode*(ParsingTreeNode::*)(const ParsingTextPos&))
				CLASS_MEMBER_METHOD_OVERLOAD(FindSubNode, {L"range"}, ParsingTreeNode*(ParsingTreeNode::*)(const ParsingTextRange&))
				CLASS_MEMBER_METHOD_OVERLOAD(FindDeepestNode, {L"position"}, ParsingTreeNode*(ParsingTreeNode::*)(const ParsingTextPos&))
				CLASS_MEMBER_METHOD_OVERLOAD(FindDeepestNode, {L"range"}, ParsingTreeNode*(ParsingTreeNode::*)(const ParsingTextRange&))
			END_CLASS_MEMBER(ParsingTreeNode)

			BEGIN_CLASS_MEMBER(ParsingTreeToken)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<ParsingTreeToken>(const WString&, vint), {L"value" _ L"tokenIndex"})

				CLASS_MEMBER_PROPERTY_FAST(TokenIndex)
				CLASS_MEMBER_PROPERTY_FAST(Value)
			END_CLASS_MEMBER(ParsingTreeToken)

			BEGIN_CLASS_MEMBER(ParsingTreeObject)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<ParsingTreeObject>(const WString&), {L"type"})

				CLASS_MEMBER_PROPERTY_FAST(Type)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Members)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(MemberNames)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(CreatorRules)

				CLASS_MEMBER_METHOD(GetMember, {L"name"})
				CLASS_MEMBER_METHOD(SetMember, {L"name" _ L"node"})
			END_CLASS_MEMBER(ParsingTreeObject)

			BEGIN_CLASS_MEMBER(ParsingTreeArray)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<ParsingTreeObject>(const WString&), {L"elementType"})

				CLASS_MEMBER_PROPERTY_FAST(ElementType)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Items)

				CLASS_MEMBER_METHOD(GetItem, {L"index"})
				CLASS_MEMBER_METHOD(SetItem, {L"index" _ L"node"})
				CLASS_MEMBER_METHOD(AddItem, {L"node"})
				CLASS_MEMBER_METHOD(InsertItem, {L"index" _ L"node"})
				CLASS_MEMBER_METHOD_OVERLOAD(RemoveItem, {L"index"}, bool(ParsingTreeArray::*)(vint))
				CLASS_MEMBER_METHOD_OVERLOAD(RemoveItem, {L"node"}, bool(ParsingTreeArray::*)(ParsingTreeNode*))
				CLASS_MEMBER_METHOD(IndexOfItem, {L"node"})
				CLASS_MEMBER_METHOD(ContainsItem, {L"node"})
				CLASS_MEMBER_METHOD(Clone, NO_PARAMETER)

				CLASS_MEMBER_METHOD_RENAME(GetCount, Count, NO_PARAMETER)
				CLASS_MEMBER_PROPERTY_READONLY(Count, GetCount)
			END_CLASS_MEMBER(ParsingTreeArray)

			BEGIN_CLASS_MEMBER(ParsingScope)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<ParsingScope>(ParsingScopeSymbol*), {L"ownerSymbol"})

				CLASS_MEMBER_PROPERTY_READONLY_FAST(OwnerSymbol)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(SymbolNames)

				CLASS_MEMBER_METHOD(AddSymbol, {L"value"})
				CLASS_MEMBER_METHOD(RemoveSymbol, {L"value"})
				CLASS_MEMBER_METHOD(GetSymbols, {L"name"})
			END_CLASS_MEMBER(ParsingScope)

			BEGIN_CLASS_MEMBER(ParsingScopeSymbol)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<ParsingScopeSymbol>(const WString&, vint), {L"name" _ L"semanticId"})

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ParentScope)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Name)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(SemanticIds)
				CLASS_MEMBER_PROPERTY_FAST(Node)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Scope)

				CLASS_MEMBER_METHOD(AddSemanticId, {L"semanticId"})
				CLASS_MEMBER_METHOD(CreateScope, NO_PARAMETER)
				CLASS_MEMBER_METHOD(DestroyScope, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetDisplay, {L"semanticId"})
			END_CLASS_MEMBER(ParsingScopeSymbol)

			typedef collections::LazyList<Ptr<ParsingScopeSymbol>> LazySymbolList;

			BEGIN_CLASS_MEMBER(ParsingScopeFinder)
				CLASS_MEMBER_METHOD_OVERLOAD(ParentNode, {L"node"}, ParsingTreeNode*(ParsingScopeFinder::*)(ParsingTreeNode*))
				CLASS_MEMBER_METHOD_OVERLOAD(ParentNode, {L"node"}, ParsingTreeNode*(ParsingScopeFinder::*)(Ptr<ParsingTreeNode>))
				CLASS_MEMBER_METHOD_OVERLOAD(Node, {L"node"}, ParsingTreeNode*(ParsingScopeFinder::*)(ParsingTreeNode*))
				CLASS_MEMBER_METHOD_OVERLOAD(Node, {L"node"}, Ptr<ParsingTreeNode>(ParsingScopeFinder::*)(Ptr<ParsingTreeNode>))
				CLASS_MEMBER_METHOD_OVERLOAD(ParentScope, {L"symbol"}, ParsingScope*(ParsingScopeFinder::*)(ParsingScopeSymbol*))
				CLASS_MEMBER_METHOD_OVERLOAD(ParentScope, {L"symbol"}, ParsingScope*(ParsingScopeFinder::*)(Ptr<ParsingScopeSymbol>))
				CLASS_MEMBER_METHOD_OVERLOAD(Symbol, {L"symbol"}, ParsingScopeSymbol*(ParsingScopeFinder::*)(ParsingScopeSymbol*))
				CLASS_MEMBER_METHOD_OVERLOAD(Symbol, {L"symbol"}, Ptr<ParsingScopeSymbol>(ParsingScopeFinder::*)(Ptr<ParsingScopeSymbol>))
				CLASS_MEMBER_METHOD(Symbols, {L"symbols"})

				CLASS_MEMBER_METHOD(GetSymbolFromNode, {L"node"})
				CLASS_MEMBER_METHOD(GetScopeFromNode, {L"node"})
				CLASS_MEMBER_METHOD_OVERLOAD(GetSymbols, {L"scope" _ L"name"}, LazySymbolList(ParsingScopeFinder::*)(ParsingScope*, const WString&))
				CLASS_MEMBER_METHOD_OVERLOAD(GetSymbols, {L"scope"}, LazySymbolList(ParsingScopeFinder::*)(ParsingScope*))
				CLASS_MEMBER_METHOD_OVERLOAD(GetSymbolsRecursively, {L"scope" _ L"name"}, LazySymbolList(ParsingScopeFinder::*)(ParsingScope*, const WString&))
				CLASS_MEMBER_METHOD_OVERLOAD(GetSymbolsRecursively, {L"scope"}, LazySymbolList(ParsingScopeFinder::*)(ParsingScope*))
			END_CLASS_MEMBER(ParsingScopeFinder)

			BEGIN_CLASS_MEMBER(ParsingTreeCustomBase)
				CLASS_MEMBER_FIELD(codeRange)
				CLASS_MEMBER_FIELD(creatorRules)
			END_CLASS_MEMBER(ParsingTreeCustomBase)

			BEGIN_CLASS_MEMBER(ParsingToken)
				CLASS_MEMBER_BASE(ParsingTreeCustomBase)

				CLASS_MEMBER_FIELD(tokenIndex)
				CLASS_MEMBER_FIELD(value)
			END_CLASS_MEMBER(ParsingToken)

			BEGIN_CLASS_MEMBER(ParsingError)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<ParsingError>(), NO_PARAMETER)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<ParsingError>(const WString&), {L"errorMessage"})
				CLASS_MEMBER_CONSTRUCTOR(Ptr<ParsingError>(ParsingTreeCustomBase*, const WString&), {L"parsingTree" _ L"errorMessage"})

				CLASS_MEMBER_FIELD(codeRange)
				CLASS_MEMBER_FIELD(parsingTree)
				CLASS_MEMBER_FIELD(errorMessage)
			END_CLASS_MEMBER(ParsingError)
#undef _
		}
	}
}

#endif

namespace vl
{
	namespace reflection
	{
		namespace description
		{

/***********************************************************************
Type Loader
***********************************************************************/
			
#ifndef VCZH_DEBUG_NO_REFLECTION
			class ParsingTypeLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					PARSINGREFLECTION_TYPELIST(ADD_TYPE_INFO)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};
#endif

			bool LoadParsingTypes()
			{
#ifndef VCZH_DEBUG_NO_REFLECTION
				ITypeManager* manager=GetGlobalTypeManager();
				if(manager)
				{
					Ptr<ITypeLoader> loader=new ParsingTypeLoader;
					return manager->AddTypeLoader(loader);
				}
#endif
				return false;
			}
		}
	}
}