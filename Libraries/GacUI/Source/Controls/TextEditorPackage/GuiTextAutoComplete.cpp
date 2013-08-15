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

			void GuiGrammarAutoComplete::UpdateScopeInfo()
			{
				Ptr<ParsingTreeNode> node=parsingExecutor->ThreadSafeGetTreeNode();
				TextPos startPos=element->GetCaretBegin();
				TextPos endPos=element->GetCaretEnd();
				if(startPos>endPos)
				{
					TextPos temp=startPos;
					startPos=endPos;
					endPos=temp;
				}
				ParsingTextPos start(startPos.row, startPos.column);
				ParsingTextPos end(endPos.row, endPos.column);
				ParsingTextRange range(start, end);
				ParsingTreeNode* found=node->FindDeepestNode(range);

				ParsingTreeObject* _selectedRoot=dynamic_cast<ParsingTreeObject*>(node.Obj());
				ParsingTreeObject* _selectedNode=0;
				WString _selectedCode, _selectedRule;

				if(!_selectedNode)
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
								_selectedNode=lrec;
								break;
							}
						}
						current=current->GetParent();
					}
				}

				if(!_selectedNode)
				{
					ParsingTreeNode* current=found;
					while(current)
					{
						ParsingTreeObject* obj=dynamic_cast<ParsingTreeObject*>(current);
						if(obj)
						{
							_selectedNode=obj;
							break;
						}
						current=current->GetParent();
					}
				}

				if(_selectedNode)
				{
					start=_selectedNode->GetCodeRange().start;
					end=_selectedNode->GetCodeRange().end;
					startPos=TextPos(start.row, start.column);
					endPos=TextPos(end.row, end.column+1);
					_selectedCode=element->GetLines().GetText(startPos, endPos);
					_selectedRule=_selectedNode->GetCreatorRules()[_selectedNode->GetCreatorRules().Count()-1].Buffer();
				}
				node=0;
				parsingExecutor->ThreadSafeReturnTreeNode();

				{
					SpinLock::Scope scope(selectedNodeLock);
					context.selectedRoot=_selectedRoot;
					context.selectedNode=_selectedNode;
					context.selectedCode=_selectedCode;
					context.selectedRule=_selectedRule;
				}

				GetApplication()->InvokeAsync([=]()
				{
					OnSelectingFinished();
				});
			}

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
				if(element && elementModifyLock)
				{
					UpdateScopeInfo();
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

			void GuiGrammarAutoComplete::OnParsingFinished(bool generatedNewNode, RepeatingParsingExecutor* parsingExecutor)
			{
				if(element && elementModifyLock)
				{
					GetApplication()->InvokeInMainThread([=]()
					{
						UpdateScopeInfo();
					});
				}
			}

			void GuiGrammarAutoComplete::EnsureAutoCompleteFinished()
			{
				parsingExecutor->EnsureTaskFinished();
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

			GuiGrammarAutoComplete::Context GuiGrammarAutoComplete::GetCodeContext()
			{
				SpinLock::Scope scope(selectedNodeLock);
				Context result;

				result.selectedRoot=context.selectedRoot;
				result.selectedNode=context.selectedNode;
				result.selectedCode=context.selectedCode.Buffer();
				result.selectedRule=context.selectedRule.Buffer();

				return result;
			}

			Ptr<RepeatingParsingExecutor> GuiGrammarAutoComplete::GetParsingExecutor()
			{
				return parsingExecutor;
			}
		}
	}
}