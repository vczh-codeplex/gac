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

			Ptr<ParsingTreeNode> ParsingGeneralParser::Parse(ParsingState& state, collections::List<Ptr<ParsingError>>& errors)
			{
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
			{
			}

			ParsingAmbiguousParser::~ParsingAmbiguousParser()
			{
			}
			
			ParsingState::TransitionResult ParsingAmbiguousParser::ParseStep(ParsingState& state, collections::List<Ptr<ParsingError>>& errors)
			{
				if(decision.Count()==0)
				{
					List<ParsingState::Future*> futures;
					List<regex::RegexToken*> tokens;

					futures.Add(state.ExploreCreateRootFuture());
					vint previousBegin=0;
					vint previousEnd=1;

					do
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
					}while(previousEnd-previousBegin>1);

					if(previousEnd-previousBegin==1)
					{
						ParsingState::Future* currentFuture=futures[previousBegin];
						vint currentRegexToken=tokens.Count()-1;
						while(currentFuture && currentFuture->selectedToken!=-1)
						{
							regex::RegexToken* token=0;
							if(currentFuture->selectedToken>=ParsingTable::UserTokenStart)
							{
								token=tokens[currentRegexToken--];
							}
							decision.Add(DecisionStep(currentFuture->selectedItem, token));
							currentFuture=currentFuture->previous;
						}
					}

					FOREACH(ParsingState::Future*, future, futures)
					{
						delete future;
					}
				}

				if(decision.Count()>0)
				{
					DecisionStep step=decision[decision.Count()-1];
					decision.RemoveAt(decision.Count()-1);
					return state.RunTransition(step.transitionItem, step.token);
				}
				else
				{
					return ParsingState::TransitionResult();
				}
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