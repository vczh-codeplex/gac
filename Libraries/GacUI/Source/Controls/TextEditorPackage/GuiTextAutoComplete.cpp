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
					if(!editing)
					{
						SPIN_LOCK(editTraceLock)
						{
							// if the current caret changing is not caused by editing
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

			void GuiGrammarAutoComplete::PrepareAutoCompleteMetadata()
			{
				Ptr<ParsingTable> table=parsingExecutor->GetParser()->GetTable();
				autoCompleteCandidates.Clear();
				autoCompleteTokens.Clear();
				autoCompleteTypes.Clear();
				fieldAutoCompleteTypes.Clear();

				// prepare tokens
				{
					vint tokenCount=table->GetTokenCount();
					for(vint token=ParsingTable::UserTokenStart;token<tokenCount;token++)
					{
						const ParsingTable::TokenInfo& tokenInfo=table->GetTokenInfo(token);
						autoCompleteCandidates.Add(parsingExecutor->GetAutoCompleteCandidateAttribute(tokenInfo.attributeIndex));
						autoCompleteTokens.Add(parsingExecutor->GetAutoCompleteTokenAttribute(tokenInfo.attributeIndex));
					}
				}

				// prepare fields
				{
					vint fieldCount=table->GetTreeFieldInfoCount();

					for(vint field=0;field<fieldCount;field++)
					{
						const ParsingTable::TreeFieldInfo& fieldInfo=table->GetTreeFieldInfo(field);
						if(Ptr<ParsingTable::AttributeInfo> att=parsingExecutor->GetAutoCompleteTypeAttribute(fieldInfo.attributeIndex))
						{
							vint index=autoCompleteTypes.Keys().IndexOf(att->arguments[0]);
							if(index==-1)
							{
								autoCompleteTypes.Add(att->arguments[0], autoCompleteTypes.Count());
							}
						}
					}

					for(vint field=0;field<fieldCount;field++)
					{
						const ParsingTable::TreeFieldInfo& fieldInfo=table->GetTreeFieldInfo(field);
						if(Ptr<ParsingTable::AttributeInfo> att=parsingExecutor->GetAutoCompleteTypeAttribute(fieldInfo.attributeIndex))
						{
							vint index=autoCompleteTypes.Keys().IndexOf(att->arguments[0]);
							fieldAutoCompleteTypes.Add(FieldDesc(fieldInfo.type, fieldInfo.field), autoCompleteTypes.Values().Get(index));
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
				}

				// locate the deepest node using the text selection
				ParsingTextPos start(startPos.row, startPos.column);
				ParsingTextPos end(endPos.row, endPos.column);
				ParsingTextRange range(start, end);
				ParsingTreeNode* found=newContext.input.node->FindDeepestNode(range);
				ParsingTreeObject* selectedNode=0;

				// if the location failed, choose the root node
				if(!found)
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
					newContext.originalNode=selectedNode->TryGetPtr(newContext.input.node).Cast<ParsingTreeObject>();
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
				FOREACH(ParsingState::Future*, future, futures)
				{
					delete future;
				}
			}

			vint GuiGrammarAutoComplete::TraverseTransitions(
				parsing::tabling::ParsingState& state,
				collections::List<parsing::tabling::ParsingState::TransitionResult>& transitions,
				vint firstTransitionIndex,
				TextPos stopPosition,
				collections::List<parsing::tabling::ParsingState::Future*>& nonRecoveryFutures,
				collections::List<parsing::tabling::ParsingState::Future*>& recoveryFutures,
				vint ambiguityRecursiveLevel
				)
			{
				vint index=firstTransitionIndex;
				while(index<transitions.Count())
				{
					ParsingState::TransitionResult& transition=transitions[index];
					switch(transition.transitionType)
					{
					case ParsingState::TransitionResult::AmbiguityBegin:
						{
							List<ParsingState::Future*> accumulatedNonRecoveryFutures, accumulatedRecoveryFutures;
							Ptr<ParsingState::StateGroup> stateGroup=state.TakeSnapshot();
							while(transitions[index].transitionType!=ParsingState::TransitionResult::AmbiguityEnd)
							{
								if(transitions[index].transitionType==ParsingState::TransitionResult::AmbiguityBranch)
								{
									state.RestoreSnapshot(stateGroup);
								}
								List<ParsingState::Future*> newNonRecoveryFutures, newRecoveryFutures;
								newNonRecoveryFutures.Add(nonRecoveryFutures[0]->Clone());
								if(recoveryFutures.Count()>0)
								{
									newRecoveryFutures.Add(recoveryFutures[0]->Clone());
								}
								index=TraverseTransitions(state, transitions, index+1, stopPosition, nonRecoveryFutures, newRecoveryFutures, ambiguityRecursiveLevel+1);
								CopyFrom(accumulatedNonRecoveryFutures, newNonRecoveryFutures, true);
								CopyFrom(accumulatedRecoveryFutures, newRecoveryFutures, true);
							}

							DeleteFutures(nonRecoveryFutures);
							DeleteFutures(recoveryFutures);
							CopyFrom(nonRecoveryFutures, accumulatedNonRecoveryFutures);
							CopyFrom(recoveryFutures, accumulatedRecoveryFutures);
						}
						break;
					case ParsingState::TransitionResult::AmbiguityBranch:
					case ParsingState::TransitionResult::AmbiguityEnd:
						return index;
					case ParsingState::TransitionResult::ExecuteInstructions:
						{
							ParsingState::Future* currentFuture=
								recoveryFutures.Count()>0
								?recoveryFutures[0]
								:nonRecoveryFutures[0];

							vint tableTokenIndex=transition.tableTokenIndex;
							List<ParsingState::Future*> possibilities;
							state.Explore(tableTokenIndex, currentFuture, possibilities);

							if(transition.token)
							{
								DeleteFutures(nonRecoveryFutures);
								DeleteFutures(recoveryFutures);
								CopyFrom(nonRecoveryFutures, possibilities);
							}
							else
							{
								DeleteFutures(recoveryFutures);
								CopyFrom(recoveryFutures, possibilities);
							}
						}
						break;
					}
					index++;
				}
				return index;
			}

			void GuiGrammarAutoComplete::SearchValidInputToken(
				parsing::tabling::ParsingState& state,
				collections::List<parsing::tabling::ParsingState::TransitionResult>& transitions,
				Context& newContext,
				collections::SortedList<vint>& tableTokenIndices
				)
			{
				TextPos stopPosition;
				SPIN_LOCK(editTraceLock)
				{
					vint index=UnsafeGetEditTraceIndex(newContext.modifiedEditVersion);
					if(index==-1)
					{
						stopPosition=editTrace[index].inputStart;
					}
					else
					{
						return;
					}
				}

				state.Reset(newContext.rule);
				List<ParsingState::Future*> nonRecoveryFutures, recoveryFutures;
				nonRecoveryFutures.Add(state.ExploreCreateRootFuture());
				TraverseTransitions(state, transitions, 0, stopPosition, nonRecoveryFutures, recoveryFutures, 0);
				
				DeleteFutures(nonRecoveryFutures);
				DeleteFutures(recoveryFutures);
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
				List<ParsingState::TransitionResult> transitions;
				ParsingTransitionCollector collector(transitions);
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
						FOREACH(ParsingState::TransitionResult, transition, transitions)
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
						// find all possible token before the current caret using the PDA
						// to collect all keywords that can be put into the auto complete list
						SortedList<vint> tableTokenIndices;
						SearchValidInputToken(state, transitions, newContext, tableTokenIndices);
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

			void GuiGrammarAutoComplete::OnContextFinishedAsync(const Context& context)
			{
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
				PrepareAutoCompleteMetadata();
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

			vint GuiGrammarAutoComplete::GetAutoCompleteTypeId(const WString& type)
			{
				vint index=autoCompleteTypes.Keys().IndexOf(type);
				return index==-1?-1:autoCompleteTypes.Values().Get(index);
			}

			Ptr<RepeatingParsingExecutor> GuiGrammarAutoComplete::GetParsingExecutor()
			{
				return parsingExecutor;
			}
		}
	}
}