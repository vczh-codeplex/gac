#include "Parsing.h"
#include "ParsingAutomaton.h"

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
				return ParsingState::TransitionResult();
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