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
							TextEditNotifyStruct trace;
							trace.editVersion=arguments.editVersion;
							trace.originalStart=arguments.oldBegin;
							trace.originalEnd=arguments.oldEnd;
							trace.inputStart=arguments.newBegin;
							trace.inputEnd=arguments.newEnd;
							if(trace.originalStart>trace.originalEnd)
							{
								TextPos temp=trace.originalStart;
								trace.originalStart=trace.originalEnd;
								trace.originalEnd=temp;
							}
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
								RepeatingParsingOutput input;
								input.editVersion=context.input.editVersion;
								SubmitTask(input);
							}
							else if(context.input.editVersion=arguments.editVersion)
							{
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

				ParsingTextPos start(startPos.row, startPos.column);
				ParsingTextPos end(endPos.row, endPos.column);
				ParsingTextRange range(start, end);
				ParsingTreeNode* found=newContext.input.node->FindDeepestNode(range);
				ParsingTreeObject* selectedNode=0;

				if(!found)
				{
					found=newContext.input.node.Obj();
				}

				if(!selectedNode)
				{
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
					start=selectedNode->GetCodeRange().start;
					end=selectedNode->GetCodeRange().end;

					newContext.rule=selectedNode->GetCreatorRules()[selectedNode->GetCreatorRules().Count()-1];
					newContext.originalRange=selectedNode->GetCodeRange();
					newContext.originalNode=selectedNode->TryGetPtr(newContext.input.node).Cast<ParsingTreeObject>();
					newContext.modifiedNode=newContext.originalNode;
					newContext.modifiedEditVersion=newContext.input.editVersion;
					if(start.index>=0 && end.index>=0)
					{
						newContext.modifiedCode=newContext.input.code.Sub(start.index, end.index-start.index+1);
					}
				}
			}

			bool GuiGrammarAutoComplete::NormalizeTextPos(Context& newContext, elements::text::TextLines& lines, TextPos& pos)
			{
				TextPos start(newContext.originalRange.start.row, newContext.originalRange.start.column);
				TextPos end
					=lines.GetCount()<=1
					?TextPos(start.row, start.column+lines.GetLine(0).dataLength)
					:TextPos(start.row+lines.GetCount()-1, lines.GetLine(lines.GetCount()-1).dataLength)
					;

				if(start<=pos && pos<=end)
				{
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

				bool failed=false;
				if(usedTrace.Count()>0)
				{
					if(usedTrace[0].editVersion!=newContext.modifiedEditVersion+1)
					{
						failed=true;
					}
					else
					{
						text::TextLines lines;
						lines.SetText(newContext.modifiedCode);
						FOREACH(TextEditNotifyStruct, trace, usedTrace)
						{
							TextPos start=trace.originalStart;
							TextPos end=trace.originalEnd;
							if(NormalizeTextPos(newContext, lines, start) && NormalizeTextPos(newContext, lines, end))
							{
								lines.Modify(start, end, trace.inputText);
							}
							else
							{
								failed=true;
								break;
							}
						}
						
						newContext.modifiedCode=lines.GetText();
						List<Ptr<ParsingError>> errors;
						Ptr<ParsingTreeNode> parsedNode=grammarParser->Parse(newContext.modifiedCode, newContext.rule, errors);
						newContext.modifiedNode=parsedNode.Cast<ParsingTreeObject>();
					}
				}

				if(usedTrace.Count()>0)
				{
					newContext.modifiedEditVersion=usedTrace[usedTrace.Count()-1].editVersion;
				}
			}

			void GuiGrammarAutoComplete::ExecuteCalculateList(Context& newContext)
			{
				if(newContext.modifiedNode)
				{
					ParsingState state(newContext.modifiedCode, grammarParser->GetTable());
					state.Reset(newContext.rule);

					List<ParsingState::TransitionResult> transitions;
					ParsingTransitionCollector collector(transitions);
					List<Ptr<ParsingError>> errors;
					if(grammarParser->Parse(state, collector, errors))
					{
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