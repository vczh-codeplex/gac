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
					SpinLock::Scope scope(_elementModifyLock);
					element=_element;
					elementModifyLock=&_elementModifyLock;
				}
			}

			void GuiTextBoxAutoCompleteBase::Detach()
			{
				if(element && elementModifyLock)
				{
					SpinLock::Scope scope(*elementModifyLock);
					element=0;
					elementModifyLock=0;
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
				parsingExecutor->ActivateCallback(this);
				if(element && elementModifyLock && autoPushing)
				{
					SpinLock::Scope scope(*elementModifyLock);
					RepeatingParsingInput input;
					input.editVersion=editVersion;
					input.code=element->GetLines().GetText();
					parsingExecutor->SubmitTask(input);
				}
			}

			void GuiGrammarAutoComplete::Detach()
			{
				GuiTextBoxAutoCompleteBase::Detach();
				parsingExecutor->DeactivateCallback(this);
				if(element && elementModifyLock)
				{
					EnsureAutoCompleteFinished();
				}
			}

			void GuiGrammarAutoComplete::TextEditNotify(const TextEditNotifyStruct& arguments)
			{
				GuiTextBoxAutoCompleteBase::TextEditNotify(arguments);
				if(element && elementModifyLock)
				{
					editing=true;
				}
			}

			void GuiGrammarAutoComplete::TextCaretChanged(const TextCaretChangedStruct& arguments)
			{
				GuiTextBoxAutoCompleteBase::TextCaretChanged(arguments);
				if(element && elementModifyLock && !editing)
				{
					SpinLock::Scope scope(contextLock);
					if(context.input.node)
					{
						GetApplication()->InvokeAsync([=]()
						{
							SubmitTask(context.input);
						});
					}
				}
			}

			void GuiGrammarAutoComplete::TextEditFinished(vuint editVersion)
			{
				GuiTextBoxAutoCompleteBase::TextEditFinished(editVersion);
				if(element && elementModifyLock)
				{
					if(autoPushing)
					{
						SpinLock::Scope scope(*elementModifyLock);
						RepeatingParsingInput input;
						input.editVersion=editVersion;
						input.code=element->GetLines().GetText();
						parsingExecutor->SubmitTask(input);
					}
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

			void GuiGrammarAutoComplete::RequireAutoSubmitTask(bool enabled)
			{
				autoPushing=enabled;
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

			void GuiGrammarAutoComplete::Execute(const RepeatingParsingOutput& input)
			{
				Context newContext;
				newContext.input=input;

				TextPos startPos, endPos;
				{
					SpinLock::Scope scope(*elementModifyLock);
					startPos=element->GetCaretBegin();
					endPos=element->GetCaretEnd();
					if(startPos>endPos)
					{
						TextPos temp=startPos;
						startPos=endPos;
						endPos=temp;
					}
				}

				ParsingTextPos start(startPos.row, startPos.column);
				ParsingTextPos end(endPos.row, endPos.column);
				ParsingTextRange range(start, end);
				ParsingTreeNode* found=newContext.input.node->FindDeepestNode(range);
				ParsingTreeObject* selectedNode=0;

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

					newContext.contextNode=selectedNode;
					if(start.index>=0 && end.index>=0)
					{
						newContext.contextNodeCode=newContext.input.code.Sub(start.index, end.index-start.index+1);
					}
					newContext.contextNodeRule=selectedNode->GetCreatorRules()[selectedNode->GetCreatorRules().Count()-1];
				}

				{
					SpinLock::Scope scope(contextLock);
					context=newContext;
					if(context.input.node && context.contextNode)
					{
						OnContextFinishedAsync(context);
					}
				}
			}

			void GuiGrammarAutoComplete::OnContextFinishedAsync(const Context& context)
			{
			}

			void GuiGrammarAutoComplete::EnsureAutoCompleteFinished()
			{
				parsingExecutor->EnsureTaskFinished();
				SpinLock::Scope scope(contextLock);
				context=Context();
			}

			GuiGrammarAutoComplete::GuiGrammarAutoComplete(Ptr<RepeatingParsingExecutor> _parsingExecutor)
				:parsingExecutor(_parsingExecutor)
				,autoPushing(false)
				,editing(false)
			{
				CollectLeftRecursiveRules();
				parsingExecutor->AttachCallback(this);
			}

			GuiGrammarAutoComplete::GuiGrammarAutoComplete(Ptr<parsing::tabling::ParsingGeneralParser> _grammarParser, const WString& _grammarRule)
				:parsingExecutor(new RepeatingParsingExecutor(_grammarParser, _grammarRule))
				,autoPushing(false)
				,editing(false)
			{
				CollectLeftRecursiveRules();
				parsingExecutor->AttachCallback(this);
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