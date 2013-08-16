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

			void GuiTextBoxAutoCompleteBase::Attach(elements::GuiColorizedTextElement* _element, SpinLock& _elementModifyLock)
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

			void GuiTextBoxAutoCompleteBase::TextEditNotify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)
			{
			}

			void GuiTextBoxAutoCompleteBase::TextCaretChanged(TextPos oldBegin, TextPos oldEnd, TextPos newBegin, TextPos newEnd)
			{
			}

			void GuiTextBoxAutoCompleteBase::TextEditFinished()
			{
			}

/***********************************************************************
GuiGrammarAutoComplete
***********************************************************************/

			void GuiGrammarAutoComplete::Attach(elements::GuiColorizedTextElement* _element, SpinLock& _elementModifyLock)
			{
				GuiTextBoxAutoCompleteBase::Attach(_element, _elementModifyLock);
				if(element && elementModifyLock)
				{
				}
			}

			void GuiGrammarAutoComplete::Detach()
			{
				GuiTextBoxAutoCompleteBase::Detach();
				if(element && elementModifyLock)
				{
					EnsureAutoCompleteFinished();
				}
			}

			void GuiGrammarAutoComplete::TextEditNotify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)
			{
				GuiTextBoxAutoCompleteBase::TextEditNotify(originalStart, originalEnd, originalText, inputStart, inputEnd, inputText);
				if(element && elementModifyLock)
				{
					editing=true;
				}
			}

			void GuiGrammarAutoComplete::TextCaretChanged(TextPos oldBegin, TextPos oldEnd, TextPos newBegin, TextPos newEnd)
			{
				GuiTextBoxAutoCompleteBase::TextCaretChanged(oldBegin, oldEnd, newBegin, newEnd);
				if(element && elementModifyLock && !editing)
				{
					SpinLock::Scope scope(contextLock);
					if(context.root)
					{
						GetApplication()->InvokeAsync([=]()
						{
							SubmitTask(TaskArgumentType(context.root, context.rootCode));
						});
					}
				}
			}

			void GuiGrammarAutoComplete::TextEditFinished()
			{
				GuiTextBoxAutoCompleteBase::TextEditFinished();
				if(element && elementModifyLock)
				{
					editing=false;
				}
			}

			void GuiGrammarAutoComplete::OnParsingFinishedAsync(Ptr<parsing::ParsingTreeObject> node, const WString& code)
			{
				if(element && elementModifyLock)
				{
					GetApplication()->InvokeInMainThread([=]()
					{
						SubmitTask(TaskArgumentType(node, code));
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

			void GuiGrammarAutoComplete::Execute(const TaskArgumentType& input)
			{
				Context newContext;
				newContext.root=input.key;
				newContext.rootCode=input.value;

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
				ParsingTreeNode* found=newContext.root->FindDeepestNode(range);
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
						newContext.contextNodeCode=newContext.rootCode.Sub(start.index, end.index-start.index+1);
					}
					newContext.contextNodeRule=selectedNode->GetCreatorRules()[selectedNode->GetCreatorRules().Count()-1];
				}

				{
					SpinLock::Scope scope(contextLock);
					context=newContext;
					if(context.root && context.contextNode)
					{
						OnContextFinishedAsync(context);
					}
				}
			}

			void GuiGrammarAutoComplete::OnContextFinishedAsync(Context& context)
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
				,editing(false)
			{
				CollectLeftRecursiveRules();
				parsingExecutor->AttachCallback(this);
			}

			GuiGrammarAutoComplete::GuiGrammarAutoComplete(Ptr<parsing::tabling::ParsingGeneralParser> _grammarParser, const WString& _grammarRule)
				:parsingExecutor(new RepeatingParsingExecutor(_grammarParser, _grammarRule))
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