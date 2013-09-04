#include "GuiTextAutoComplete.h"
#include "..\GuiApplication.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace elements;
			using namespace elements::text;
			using namespace compositions;
			using namespace regex;
			using namespace parsing;
			using namespace parsing::tabling;
			using namespace collections;

/***********************************************************************
GuiTextBoxAutoCompleteBase
***********************************************************************/

			GuiTextBoxAutoCompleteBase::GuiTextBoxAutoCompleteBase()
				:element(0)
				,elementModifyLock(0)
			{
			}

			GuiTextBoxAutoCompleteBase::~GuiTextBoxAutoCompleteBase()
			{
			}

			void GuiTextBoxAutoCompleteBase::Attach(elements::GuiColorizedTextElement* _element, SpinLock& _elementModifyLock, vuint editVersion)
			{
				if(_element)
				{
					SPIN_LOCK(_elementModifyLock)
					{
						element=_element;
						elementModifyLock=&_elementModifyLock;
					}
				}
			}

			void GuiTextBoxAutoCompleteBase::Detach()
			{
				if(element && elementModifyLock)
				{
					SPIN_LOCK(*elementModifyLock)
					{
						element=0;
						elementModifyLock=0;
					}
				}
			}

			void GuiTextBoxAutoCompleteBase::TextEditNotify(const TextEditNotifyStruct& arguments)
			{
			}

			void GuiTextBoxAutoCompleteBase::TextCaretChanged(const TextCaretChangedStruct& arguments)
			{
			}

			void GuiTextBoxAutoCompleteBase::TextEditFinished(vuint editVersion)
			{
			}

/***********************************************************************
GuiGrammarAutoComplete
***********************************************************************/

			void GuiGrammarAutoComplete::Attach(elements::GuiColorizedTextElement* _element, SpinLock& _elementModifyLock, vuint editVersion)
			{
				GuiTextBoxAutoCompleteBase::Attach(_element, _elementModifyLock, editVersion);
				RepeatingParsingExecutor::CallbackBase::Attach(_element, _elementModifyLock, editVersion);
			}

			void GuiGrammarAutoComplete::Detach()
			{
				GuiTextBoxAutoCompleteBase::Detach();
				RepeatingParsingExecutor::CallbackBase::Detach();
				if(element && elementModifyLock)
				{
					EnsureAutoCompleteFinished();
				}
			}

			void GuiGrammarAutoComplete::TextEditNotify(const TextEditNotifyStruct& arguments)
			{
				GuiTextBoxAutoCompleteBase::TextEditNotify(arguments);
				RepeatingParsingExecutor::CallbackBase::TextEditNotify(arguments);
				if(element && elementModifyLock)
				{
					editing=true;
					SPIN_LOCK(editTraceLock)
					{
						editTrace.Add(arguments);
					}
				}
			}

			void GuiGrammarAutoComplete::TextCaretChanged(const TextCaretChangedStruct& arguments)
			{
				GuiTextBoxAutoCompleteBase::TextCaretChanged(arguments);
				RepeatingParsingExecutor::CallbackBase::TextCaretChanged(arguments);
				if(element && elementModifyLock)
				{
					SPIN_LOCK(editTraceLock)
					{
						// queue a fake TextEditNotifyStruct
						// a fake struct can be detected by (trace.originalText==L"" && trace.inputText==L"")
						TextEditNotifyStruct trace;
						trace.editVersion=arguments.editVersion;
						trace.originalStart=arguments.oldBegin;
						trace.originalEnd=arguments.oldEnd;
						trace.inputStart=arguments.newBegin;
						trace.inputEnd=arguments.newEnd;

						// ensure trace.originalStart<=trace.originalEnd
						if(trace.originalStart>trace.originalEnd)
						{
							TextPos temp=trace.originalStart;
							trace.originalStart=trace.originalEnd;
							trace.originalEnd=temp;
						}
						// ensure trace.inputStart<=trace.inputEnd
						if(trace.inputStart>trace.inputEnd)
						{
							TextPos temp=trace.inputStart;
							trace.inputStart=trace.inputEnd;
							trace.inputEnd=temp;
						}
						editTrace.Add(trace);
					}

					SPIN_LOCK(contextLock)
					{
						if(context.input.node)
						{
							if(editing)
							{
								// if the current caret changing is caused by editing
								// submit a task with valid editVersion and invalid node and code
								RepeatingParsingOutput input;
								input.editVersion=context.input.editVersion;
								SubmitTask(input);
							}
							else if(context.input.editVersion=arguments.editVersion)
							{
								// if the current caret changing is not caused by editing
								// submit a task with the previous input
								SubmitTask(context.input);
							}
						}
					}
				}
			}

			void GuiGrammarAutoComplete::TextEditFinished(vuint editVersion)
			{
				GuiTextBoxAutoCompleteBase::TextEditFinished(editVersion);
				RepeatingParsingExecutor::CallbackBase::TextEditFinished(editVersion);
				if(element && elementModifyLock)
				{
					editing=false;
				}
			}

			void GuiGrammarAutoComplete::OnParsingFinishedAsync(const RepeatingParsingOutput& arguments)
			{
				if(element && elementModifyLock)
				{
					GetApplication()->InvokeInMainThread([=]()
					{
						// submit a task if the RepeatingParsingExecutor notices a new parsing result
						SubmitTask(arguments);
					});
				}
			}

			void GuiGrammarAutoComplete::CollectLeftRecursiveRules()
			{
				leftRecursiveRules.Clear();
				Ptr<ParsingGeneralParser> parser=parsingExecutor->GetParser();
				Ptr<ParsingTable> table=parser->GetTable();
				vint stateCount=table->GetStateCount();
				vint tokenCount=table->GetTokenCount();
				for(vint i=0;i<stateCount;i++)
				{
					for(vint j=0;j<tokenCount;j++)
					{
						Ptr<ParsingTable::TransitionBag> bag=table->GetTransitionBag(i, j);
						if(bag)
						{
							FOREACH(Ptr<ParsingTable::TransitionItem>, item, bag->transitionItems)
							{
								FOREACH(ParsingTable::Instruction, ins, item->instructions)
								{
									if(ins.instructionType==ParsingTable::Instruction::LeftRecursiveReduce)
									{
										if(!leftRecursiveRules.Contains(ins.creatorRule))
										{
											leftRecursiveRules.Add(ins.creatorRule);
										}
									}
								}
							}
						}
					}
				}
			}

			vint GuiGrammarAutoComplete::UnsafeGetEditTraceIndex(vuint editVersion)
			{
				// get the index of the latest TextEditNotifyStruct of a specified edit version
				// this function should be called inside SPIN_LOCK(editTraceLock)
				// perform a binary search
				vint start=0;
				vint end=editTrace.Count()-1;
				while(start<=end)
				{
					vint middle=(start+end)/2;
					TextEditNotifyStruct& trace=editTrace[middle];

					if(editVersion<trace.editVersion)
					{
						end=middle-1;
					}
					else if(editVersion>trace.editVersion)
					{
						start=middle+1;
					}
					else
					{
						// if multiple TextEditNotifyStruct is found, choose the latest one
						while(middle<editTrace.Count()-1)
						{
							if(editTrace[middle+1].editVersion==editTrace[middle].editVersion)
							{
								middle++;
							}
							else
							{
								break;
							}
						}
						return middle;
					}
				}
				return -1;
			}

			TextPos GuiGrammarAutoComplete::ChooseCorrectTextPos(TextPos pos, const regex::RegexTokens& tokens)
			{
				Ptr<ParsingTable> table=grammarParser->GetTable();
				RegexToken lastToken;
				lastToken.reading=0;

				FOREACH(RegexToken, token, tokens)
				{
					// we treat "class| Name" as editing the first token
					if(TextPos(token.rowEnd, token.columnEnd+1)>=pos)
					{
						if(table->GetTableTokenIndex(token.token)!=-1 && lastToken.reading)
						{
							pos=TextPos(lastToken.rowStart, lastToken.columnStart);
						}
						break;
					}
					lastToken=token;
				}
				return pos;
			}

			void GuiGrammarAutoComplete::ExecuteRefresh(Context& newContext)
			{
				// process the input of a task is submitted not by text editing
				// find the text selection by the edit version of the input
				TextPos startPos, endPos;
				{
					SPIN_LOCK(editTraceLock)
					{
						vint traceIndex=UnsafeGetEditTraceIndex(newContext.input.editVersion);
						if(traceIndex==-1) return;

						TextEditNotifyStruct& trace=editTrace[traceIndex];
						startPos=trace.inputStart;
						endPos=trace.inputEnd;
					}

					const RegexLexer& lexer=grammarParser->GetTable()->GetLexer();
					RegexTokens tokens=lexer.Parse(newContext.input.code);
					startPos=ChooseCorrectTextPos(startPos, tokens);
				}

				// locate the deepest node using the text selection
				ParsingTextPos start(startPos.row, startPos.column);
				ParsingTextPos end(endPos.row, endPos.column);
				ParsingTextRange range(start, end);
				ParsingTreeNode* found=newContext.input.node->FindDeepestNode(range);
				ParsingTreeObject* selectedNode=0;

				// if the location failed, choose the root node
				if(!found || startPos==TextPos(0, 0))
				{
					found=newContext.input.node.Obj();
				}

				if(!selectedNode)
				{
					// from the deepest node, traverse towards the root node
					// find the deepest node whose created rule is a left recursive rule and whose parent is not
					ParsingTreeObject* lrec=0;
					ParsingTreeNode* current=found;
					while(current)
					{
						ParsingTreeObject* obj=dynamic_cast<ParsingTreeObject*>(current);
						if(obj)
						{
							FOREACH(WString, rule, obj->GetCreatorRules())
							{
								if(leftRecursiveRules.Contains(rule))
								{
									lrec=obj;
									break;
								}
							}
							if(obj && lrec && lrec!=obj)
							{
								selectedNode=lrec;
								break;
							}
						}
						current=current->GetParent();
					}
				}

				if(!selectedNode)
				{
					// if there is no left recursive rule that creates the deepest node and all indirect parents
					// choose the deepest ParsingTreeObject
					ParsingTreeNode* current=found;
					while(current)
					{
						ParsingTreeObject* obj=dynamic_cast<ParsingTreeObject*>(current);
						if(obj)
						{
							selectedNode=obj;
							break;
						}
						current=current->GetParent();
					}
				}

				if(selectedNode)
				{
					// get the code range of the selected node
					start=selectedNode->GetCodeRange().start;
					end=selectedNode->GetCodeRange().end;

					// get all properties from the selected node
					newContext.rule=selectedNode->GetCreatorRules()[selectedNode->GetCreatorRules().Count()-1];
					newContext.originalRange=selectedNode->GetCodeRange();
					newContext.originalNode=dynamic_cast<ParsingTreeObject*>(selectedNode);
					newContext.modifiedNode=newContext.originalNode;
					newContext.modifiedEditVersion=newContext.input.editVersion;

					// get the corresponding code of the selected node
					if(start.index>=0 && end.index>=0)
					{
						newContext.modifiedCode=newContext.input.code.Sub(start.index, end.index-start.index+1).Buffer();
					}
				}
			}

			bool GuiGrammarAutoComplete::NormalizeTextPos(Context& newContext, elements::text::TextLines& lines, TextPos& pos)
			{
				// get the start position
				TextPos start(newContext.originalRange.start.row, newContext.originalRange.start.column);

				// get the end position of the end of lines
				TextPos end
					=lines.GetCount()<=1
					?TextPos(start.row, start.column+lines.GetLine(0).dataLength)
					:TextPos(start.row+lines.GetCount()-1, lines.GetLine(lines.GetCount()-1).dataLength)
					;

				if(start<=pos && pos<=end)
				{
					// if the pos is inside the range
					// normalize the pos to a new coordinate that the beginning position of lines is (row=0, column=0)
					pos.row-=start.row;
					if(pos.row==0)
					{
						pos.column-=start.column;
					}
					return true;
				}
				else
				{
					return false;
				}
			}

			void GuiGrammarAutoComplete::ExecuteEdit(Context& newContext)
			{
				// process the input of a task is submitted by text editing
				// this function make an approximiation to the context if the RepeatingParsingExecutor is not fast enough
				// copy all TextEditNotifyStruct that is caused by a text editing before (and including) the edit version of the input
				List<TextEditNotifyStruct> usedTrace;
				{
					SPIN_LOCK(editTraceLock)
					{
						CopyFrom(
							usedTrace,
							From(editTrace)
								.Where([&newContext](const TextEditNotifyStruct& value)
								{
									return (value.originalText!=L"" || value.inputText!=L"") && value.editVersion>newContext.modifiedEditVersion;
								})
							);
					}
				}

				// apply all modification to get the new modifiedCode
				bool failed=false;
				if(usedTrace.Count()>0)
				{
					if(usedTrace[0].editVersion!=newContext.modifiedEditVersion+1)
					{
						// failed if any TextEditNotifyStruct is missing 
						failed=true;
					}
					else
					{
						// initialize a TextLines with the latest modifiedCode
						text::TextLines lines;
						lines.SetText(newContext.modifiedCode);
						FOREACH(TextEditNotifyStruct, trace, usedTrace)
						{
							// apply a modification to lines
							TextPos start=trace.originalStart;
							TextPos end=trace.originalEnd;

							// only if the modification is meaningful
							if(NormalizeTextPos(newContext, lines, start) && NormalizeTextPos(newContext, lines, end))
							{
								lines.Modify(start, end, trace.inputText);
							}
							else
							{
								// otherwise, failed
								failed=true;
								break;
							}
						}
						
						if(!failed)
						{
							newContext.modifiedCode=lines.GetText();
						}
					}
				}

				if(failed)
				{
					// clear originalNode to notify that the current context goes wrong
					newContext.originalNode=0;
				}

				if(usedTrace.Count()>0)
				{
					// update the edit version
					newContext.modifiedEditVersion=usedTrace[usedTrace.Count()-1].editVersion;
				}
			}

			void GuiGrammarAutoComplete::DeleteFutures(collections::List<parsing::tabling::ParsingState::Future*>& futures)
			{
				// delete all futures and clear the list
				FOREACH(ParsingState::Future*, future, futures)
				{
					delete future;
				}
				futures.Clear();
			}

			regex::RegexToken* GuiGrammarAutoComplete::TraverseTransitions(
				parsing::tabling::ParsingState& state,
				parsing::tabling::ParsingTransitionCollector& transitionCollector,
				TextPos stopPosition,
				collections::List<parsing::tabling::ParsingState::Future*>& nonRecoveryFutures,
				collections::List<parsing::tabling::ParsingState::Future*>& recoveryFutures
				)
			{
				const List<ParsingState::TransitionResult>& transitions=transitionCollector.GetTransitions();
				for(vint index=0;index<transitions.Count();index++)
				{
					const ParsingState::TransitionResult& transition=transitions[index];
					switch(transition.transitionType)
					{
					case ParsingState::TransitionResult::AmbiguityBegin:
						break;
					case ParsingState::TransitionResult::AmbiguityBranch:
						// ambiguity branches are not nested
						// tokens in different braches are the same
						// so we only need to run one branch, and skip the others
						index=transitionCollector.GetAmbiguityEndFromBegin(transitionCollector.GetAmbiguityBeginFromBranch(index));
						break;
					case ParsingState::TransitionResult::AmbiguityEnd:
						break;
					case ParsingState::TransitionResult::ExecuteInstructions:
						{
							// test does the token reach the stop position
							if(transition.token)
							{
								// we treat "A|B" as editing A if token A is endless, otherwise treated as editing B
								TextPos tokenEnd(transition.token->rowEnd, transition.token->columnEnd+1);

								// if the caret is not at the end of the token
								if(tokenEnd>stopPosition)
								{
									// stop the traversing and return the editing token
									return transition.token;
								}
								else if(tokenEnd==stopPosition)
								{
									// if the caret is at the end of the token, and it is a closed token
									// e.g. identifier is not a closed token, string is a closed token
									if(!grammarParser->GetTable()->GetLexer().Walk().IsClosedToken(transition.token->reading, transition.token->length))
									{
										// stop the traversing and return the editing token
										return transition.token;
									}
								}
							}

							// traverse the PDA using the token specified in the current transition
							vint tableTokenIndex=transition.tableTokenIndex;
							List<ParsingState::Future*> possibilities;
							if(recoveryFutures.Count()>0)
							{
								FOREACH(ParsingState::Future*, future, recoveryFutures)
								{
									state.Explore(tableTokenIndex, future, possibilities);
								}
							}
							else
							{
								FOREACH(ParsingState::Future*, future, nonRecoveryFutures)
								{
									state.Explore(tableTokenIndex, future, possibilities);
								}
							}

							// delete duplicated futures
							List<ParsingState::Future*> selectedPossibilities;
							for(vint i=0;i<possibilities.Count();i++)
							{
								ParsingState::Future* candidateFuture=possibilities[i];
								bool duplicated=false;
								FOREACH(ParsingState::Future*, future, selectedPossibilities)
								{
									if(
										candidateFuture->currentState==future->currentState &&
										candidateFuture->reduceStateCount==future->reduceStateCount &&
										candidateFuture->shiftStates.Count()==future->shiftStates.Count()
										)
									{
										bool same=true;
										for(vint j=0;j<future->shiftStates.Count();j++)
										{
											if(candidateFuture->shiftStates[i]!=future->shiftStates[i])
											{
												same=false;
												break;
											}
										}

										if(duplicated=same)
										{
											break;
										}
									}
								}

								if(duplicated)
								{
									delete candidateFuture;
								}
								else
								{
									selectedPossibilities.Add(candidateFuture);
								}
							}

							// step forward
							if(transition.token || transition.tableTokenIndex==ParsingTable::TokenBegin)
							{
								DeleteFutures(nonRecoveryFutures);
								DeleteFutures(recoveryFutures);
								CopyFrom(nonRecoveryFutures, selectedPossibilities);
							}
							else
							{
								DeleteFutures(recoveryFutures);
								CopyFrom(recoveryFutures, selectedPossibilities);
							}
						}
						break;
					}
				}
				return 0;
			}

			regex::RegexToken* GuiGrammarAutoComplete::SearchValidInputToken(
				parsing::tabling::ParsingState& state,
				parsing::tabling::ParsingTransitionCollector& transitionCollector,
				TextPos stopPosition,
				Context& newContext,
				collections::SortedList<vint>& tableTokenIndices
				)
			{
				// initialize the PDA state
				state.Reset(newContext.rule);
				List<ParsingState::Future*> nonRecoveryFutures, recoveryFutures;
				nonRecoveryFutures.Add(state.ExploreCreateRootFuture());

				// traverse the PDA until it reach the stop position
				// nonRecoveryFutures store the state when the last token (existing) is reached
				// recoveryFutures store the state when the last token (inserted by error recovery) is reached
				RegexToken* token=TraverseTransitions(state, transitionCollector, stopPosition, nonRecoveryFutures, recoveryFutures);

				// explore all possibilities from the last token before the stop position
				List<ParsingState::Future*> possibilities;
				FOREACH(ParsingState::Future*, future, nonRecoveryFutures)
				{
					vint count=state.GetTable()->GetTokenCount();
					for(vint i=0;i<count;i++)
					{
						state.Explore(i, future, possibilities);
					}
				}

				// get all possible tokens that marked using @AutoCompleteCandidate
				FOREACH(ParsingState::Future*, future, possibilities)
				{
					if(!tableTokenIndices.Contains(future->selectedToken))
					{
						tableTokenIndices.Add(future->selectedToken);
					}
				}

				// release all data
				DeleteFutures(possibilities);
				DeleteFutures(nonRecoveryFutures);
				DeleteFutures(recoveryFutures);

				// return the editing token
				return token;
			}

			TextPos GuiGrammarAutoComplete::GlobalTextPosToModifiedTextPos(Context& newContext, TextPos pos)
			{
				pos.row-=newContext.originalRange.start.row;
				if(pos.row==0)
				{
					pos.column-=newContext.originalRange.start.column;
				}
				return pos;
			}

			TextPos GuiGrammarAutoComplete::ModifiedTextPosToGlobalTextPos(Context& newContext, TextPos pos)
			{
				if(pos.row==0)
				{
					pos.column+=newContext.originalRange.start.column;
				}
				pos.row+=newContext.originalRange.start.row;
				return pos;
			}

			void GuiGrammarAutoComplete::ExecuteCalculateList(Context& newContext)
			{
				// calcuate the content of the auto complete list
				// it is sad that, because the parser's algorithm is too complex
				// we need to reparse and track the internal state of the PDA(push-down automaton) here.
				// initialize the PDA
				ParsingState state(newContext.modifiedCode, grammarParser->GetTable());
				state.Reset(newContext.rule);

				// prepare to get all transitions
				ParsingTransitionCollector collector;
				List<Ptr<ParsingError>> errors;

				// reparse and get all transitions during parsing
				if(grammarParser->Parse(state, collector, errors))
				{
					// if modifiedNode is not prepared (the task is submitted because of text editing)
					// use the transition to build the syntax tree
					if(!newContext.modifiedNode)
					{
						ParsingTreeBuilder builder;
						builder.Reset();
						bool succeeded=true;
						FOREACH(ParsingState::TransitionResult, transition, collector.GetTransitions())
						{
							if(!(succeeded=builder.Run(transition)))
							{
								break;
							}
						}

						if(succeeded)
						{
							Ptr<ParsingTreeNode> parsedNode=builder.GetNode();
							newContext.modifiedNode=parsedNode.Cast<ParsingTreeObject>();
						}
					}

					if(newContext.modifiedNode)
					{
						// get the latest text editing trace
						TextEditNotifyStruct trace;
						SPIN_LOCK(editTraceLock)
						{
							vint index=UnsafeGetEditTraceIndex(newContext.modifiedEditVersion);
							if(index==-1)
							{
								return;
							}
							else
							{
								trace=editTrace[index];
							}
						}
						
						// calculate the stop position for PDA traversing
						TextPos stopPosition=GlobalTextPosToModifiedTextPos(newContext, trace.inputStart);

						// find all possible token before the current caret using the PDA
						Ptr<AutoCompleteData> autoComplete=new AutoCompleteData;
						SortedList<vint> tableTokenIndices;
						RegexToken* editingToken=SearchValidInputToken(state, collector, stopPosition, newContext, tableTokenIndices);

						// collect all auto complete types
						{
							// collect all keywords that can be put into the auto complete list
							FOREACH(vint, token, tableTokenIndices)
							{
								vint regexToken=token-ParsingTable::UserTokenStart;
								if(regexToken>=0)
								{
									autoComplete->candidates.Add(regexToken);
									if(parsingExecutor->GetTokenMetaData(regexToken).isCandidate)
									{
										autoComplete->shownCandidates.Add(regexToken);
									}
								}
							}

							// calculate the arranged stopPosition
							if(editingToken)
							{
								TextPos tokenPos(editingToken->rowStart, editingToken->columnStart);
								if(tokenPos<stopPosition)
								{
									stopPosition=tokenPos;
								}
							}

							// calculate the start/end position for PDA traversing
							TextPos startPos, endPos;
							{
								startPos=ModifiedTextPosToGlobalTextPos(newContext, stopPosition);
								autoComplete->startPosition=startPos;
								endPos=trace.inputEnd;
								if(newContext.modifiedNode!=newContext.originalNode)
								{
									startPos=GlobalTextPosToModifiedTextPos(newContext, startPos);
									endPos=GlobalTextPosToModifiedTextPos(newContext, endPos);
								}
								if(startPos<endPos && endPos.column>0)
								{
									endPos.column--;
								}
							}

							// calculate the auto complete type
							if(editingToken && parsingExecutor->GetTokenMetaData(editingToken->token).hasAutoComplete)
							{
								ParsingTextRange range(ParsingTextPos(startPos.row, startPos.column), ParsingTextPos(endPos.row, endPos.column));
								AutoCompleteData::RetriveContext(*autoComplete.Obj(), range, newContext.modifiedNode.Obj(), parsingExecutor.Obj());
							}
						}
						newContext.autoComplete=autoComplete;
					}
				}
			}

			void GuiGrammarAutoComplete::Execute(const RepeatingParsingOutput& input)
			{
				SPIN_LOCK(contextLock)
				{
					if(input.editVersion<context.input.editVersion)
					{
						return;
					}
				}
				Context newContext;

				if(input.node)
				{
					newContext.input=input;
					ExecuteRefresh(newContext);
				}
				else
				{
					SPIN_LOCK(contextLock)
					{
						newContext=context;
						newContext.modifiedNode=0;
						newContext.autoComplete=0;
					}
					if(newContext.originalNode)
					{
						ExecuteEdit(newContext);
					}
				}

				if(newContext.originalNode)
				{
					ExecuteCalculateList(newContext);
				}

				SPIN_LOCK(contextLock)
				{
					context=newContext;
				}
				if(newContext.modifiedNode)
				{
					OnContextFinishedAsync(context);
					GetApplication()->InvokeInMainThread([=]()
					{
						PostList(newContext);
					});
				}
			}

			void GuiGrammarAutoComplete::PostList(const Context& newContext)
			{
				SPIN_LOCK(editTraceLock)
				{
					vint traceIndex=UnsafeGetEditTraceIndex(newContext.input.editVersion);
					if(traceIndex==-1) return;

					editTrace.RemoveRange(0, traceIndex+1);
				}
			}

			void GuiGrammarAutoComplete::OnContextFinishedAsync(Context& context)
			{
				Ptr<ILanguageProvider> languageProvider=parsingExecutor->GetLanguageProvider();
				if(languageProvider)
				{
					if(context.autoComplete && context.autoComplete->acceptableSemanticIds)
					{
						ParsingTreeObject* originalNode=context.originalNode.Obj();
						ParsingTreeObject* replacedNode=context.modifiedNode.Obj();
						ParsingScopeSymbol* originalSymbol=0;
						{
							ParsingTreeNode* originalSymbolNode=originalNode;
							while(originalSymbolNode && !originalSymbol)
							{
								originalSymbol=context.input.finder->GetSymbolFromNode(dynamic_cast<ParsingTreeObject*>(originalSymbolNode));
								originalSymbolNode=originalSymbolNode->GetParent();
							}
						}
						if(originalSymbol)
						{
							Ptr<ParsingScopeFinder> newFinder=new ParsingScopeFinder(new ParsingScopeFinder::IndirectSymbolMapper(0, 0, originalNode, replacedNode));
							Ptr<ParsingScopeSymbol> replacedSymbol=languageProvider->CreateSymbolFromNode(newFinder->Obj(originalSymbol->GetNode()), GetParsingExecutor().Obj(), newFinder.Obj());
				
							if(replacedSymbol)
							{
								newFinder=new ParsingScopeFinder(new ParsingScopeFinder::IndirectSymbolMapper(originalSymbol, replacedSymbol.Obj(), originalNode, replacedNode));
								newFinder->InitializeQueryCache(replacedSymbol.Obj(), context.input.finder.Obj());
								LazyList<Ptr<ParsingScopeSymbol>> symbols=languageProvider->FindPossibleSymbols(context.autoComplete->tokenParent, context.autoComplete->field, newFinder.Obj());
								CopyFrom(
									context.autoComplete->candidateSymbols,
									From(symbols)
										.Where([&context](Ptr<ParsingScopeSymbol> symbol)
										{
											return From(symbol->GetSemanticIds()).Intersect(*context.autoComplete->acceptableSemanticIds.Obj()).First(-1)!=-1;
										})
									);
							}
						}
					}
				}
			}

			void GuiGrammarAutoComplete::EnsureAutoCompleteFinished()
			{
				parsingExecutor->EnsureTaskFinished();
				SPIN_LOCK(contextLock)
				{
					context=Context();
				}
			}

			void GuiGrammarAutoComplete::Initialize()
			{
				grammarParser=CreateAutoRecoverParser(parsingExecutor->GetParser()->GetTable());
				CollectLeftRecursiveRules();
				parsingExecutor->AttachCallback(this);
			}

			GuiGrammarAutoComplete::GuiGrammarAutoComplete(Ptr<RepeatingParsingExecutor> _parsingExecutor)
				:RepeatingParsingExecutor::CallbackBase(_parsingExecutor)
				,editing(false)
			{
				Initialize();
			}

			GuiGrammarAutoComplete::GuiGrammarAutoComplete(Ptr<parsing::tabling::ParsingGeneralParser> _grammarParser, const WString& _grammarRule)
				:RepeatingParsingExecutor::CallbackBase(new RepeatingParsingExecutor(_grammarParser, _grammarRule))
				,editing(false)
			{
				Initialize();
			}

			GuiGrammarAutoComplete::~GuiGrammarAutoComplete()
			{
				EnsureAutoCompleteFinished();
				parsingExecutor->DetachCallback(this);
			}

			Ptr<RepeatingParsingExecutor> GuiGrammarAutoComplete::GetParsingExecutor()
			{
				return parsingExecutor;
			}
		}
	}
}