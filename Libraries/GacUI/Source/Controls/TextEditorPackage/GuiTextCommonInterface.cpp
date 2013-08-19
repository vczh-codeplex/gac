#include "GuiTextCommonInterface.h"
#include <math.h>

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace elements;
			using namespace elements::text;
			using namespace compositions;

/***********************************************************************
GuiTextBoxCommonInterface::DefaultCallback
***********************************************************************/

			GuiTextBoxCommonInterface::DefaultCallback::DefaultCallback(elements::GuiColorizedTextElement* _textElement, compositions::GuiGraphicsComposition* _textComposition)
				:textElement(_textElement)
				,textComposition(_textComposition)
			{
			}

			GuiTextBoxCommonInterface::DefaultCallback::~DefaultCallback()
			{
			}

			TextPos GuiTextBoxCommonInterface::DefaultCallback::GetLeftWord(TextPos pos)
			{
				return pos;
			}

			TextPos GuiTextBoxCommonInterface::DefaultCallback::GetRightWord(TextPos pos)
			{
				return pos;
			}

			void GuiTextBoxCommonInterface::DefaultCallback::GetWord(TextPos pos, TextPos& begin, TextPos& end)
			{
				begin=pos;
				end=pos;
			}

			vint GuiTextBoxCommonInterface::DefaultCallback::GetPageRows()
			{
				return textComposition->GetBounds().Height()/textElement->GetLines().GetRowHeight();
			}

			bool GuiTextBoxCommonInterface::DefaultCallback::BeforeModify(TextPos start, TextPos end, const WString& originalText, WString& inputText)
			{
				return true;
			}

/***********************************************************************
GuiTextBoxCommonInterface::ShortcutCommand
***********************************************************************/

			GuiTextBoxCommonInterface::ShortcutCommand::ShortcutCommand(bool _ctrl, bool _shift, vint _key, const Func<void()> _action)
				:ctrl(_ctrl)
				,shift(_shift)
				,key(_key)
				,action(_action)
			{
			}

			GuiTextBoxCommonInterface::ShortcutCommand::ShortcutCommand(bool _ctrl, bool _shift, vint _key, const Func<bool()> _action)
				:ctrl(_ctrl)
				,shift(_shift)
				,key(_key)
				,action(Func<void()>(_action))
			{
			}

			GuiTextBoxCommonInterface::ShortcutCommand::~ShortcutCommand()
			{
			}

			bool GuiTextBoxCommonInterface::ShortcutCommand::IsTheRightKey(bool _ctrl, bool _shift, vint _key)
			{
				return _ctrl==ctrl && _shift==shift && _key==key;
			}

			void GuiTextBoxCommonInterface::ShortcutCommand::Execute()
			{
				action();
			}

/***********************************************************************
GuiTextBoxCommonInterface
***********************************************************************/

			void GuiTextBoxCommonInterface::UpdateCaretPoint()
			{
				GuiGraphicsHost* host=textComposition->GetRelatedGraphicsHost();
				if(host)
				{
					Rect caret=textElement->GetLines().GetRectFromTextPos(textElement->GetCaretEnd());
					Point view=textElement->GetViewPosition();
					vint textMargin=callback->GetTextMargin();
					vint x=caret.x1-view.x;
					vint y=caret.y2-view.y;
					host->SetCaretPoint(Point(x, y), textComposition);
				}
			}

			void GuiTextBoxCommonInterface::Move(TextPos pos, bool shift)
			{
				TextPos oldBegin=textElement->GetCaretBegin();
				TextPos oldEnd=textElement->GetCaretEnd();

				pos=textElement->GetLines().Normalize(pos);
				if(!shift)
				{
					textElement->SetCaretBegin(pos);
				}
				textElement->SetCaretEnd(pos);
				if(textControl)
				{
					GuiGraphicsHost* host=textComposition->GetRelatedGraphicsHost();
					if(host)
					{
						if(host->GetFocusedComposition()==textControl->GetFocusableComposition())
						{
							textElement->SetCaretVisible(true);
						}
					}
				}

				Rect bounds=textElement->GetLines().GetRectFromTextPos(pos);
				Rect view=Rect(textElement->GetViewPosition(), textComposition->GetBounds().GetSize());
				Point viewPoint=view.LeftTop();

				if(view.x2>view.x1 && view.y2>view.y1)
				{
					if(bounds.x1<view.x1)
					{
						viewPoint.x=bounds.x1;
					}
					else if(bounds.x2>view.x2)
					{
						viewPoint.x=bounds.x2-view.Width();
					}
					if(bounds.y1<view.y1)
					{
						viewPoint.y=bounds.y1;
					}
					else if(bounds.y2>view.y2)
					{
						viewPoint.y=bounds.y2-view.Height();
					}
				}

				callback->ScrollToView(viewPoint);
				UpdateCaretPoint();

				TextPos newBegin=textElement->GetCaretBegin();
				TextPos newEnd=textElement->GetCaretEnd();
				if(oldBegin!=newBegin || oldEnd!=newEnd)
				{
					ICommonTextEditCallback::TextCaretChangedStruct arguments;
					arguments.oldBegin=oldBegin;
					arguments.oldEnd=oldEnd;
					arguments.newBegin=newBegin;
					arguments.newEnd=newEnd;
					for(vint i=0;i<textEditCallbacks.Count();i++)
					{
						textEditCallbacks[i]->TextCaretChanged(arguments);
					}
					SelectionChanged.Execute(textControl->GetNotifyEventArguments());
				}
			}

			void GuiTextBoxCommonInterface::Modify(TextPos start, TextPos end, const WString& input)
			{
				if(start>end)
				{
					TextPos temp=start;
					start=end;
					end=temp;
				}
				TextPos originalStart=start;
				TextPos originalEnd=end;
				WString originalText=textElement->GetLines().GetText(start, end);
				WString inputText=input;
				if(callback->BeforeModify(start, end, originalText, inputText))
				{
					{
						SpinLock::Scope scope(elementModifyLock);
						end=textElement->GetLines().Modify(start, end, inputText);
					}
					callback->AfterModify(originalStart, originalEnd, originalText, start, end, inputText);
					
					ICommonTextEditCallback::TextEditNotifyStruct arguments;
					arguments.originalStart=originalStart;
					arguments.originalEnd=originalEnd;
					arguments.originalText=originalText;
					arguments.inputStart=start;
					arguments.inputEnd=end;
					arguments.inputText=inputText;
					for(vint i=0;i<textEditCallbacks.Count();i++)
					{
						textEditCallbacks[i]->TextEditNotify(arguments);
					}
					Move(end, false);
					
					for(vint i=0;i<textEditCallbacks.Count();i++)
					{
						textEditCallbacks[i]->TextEditFinished();
					}
					textControl->TextChanged.Execute(textControl->GetNotifyEventArguments());
				}
			}

			bool GuiTextBoxCommonInterface::ProcessKey(vint code, bool shift, bool ctrl)
			{
				for(vint i=0;i<shortcutCommands.Count();i++)
				{
					if(shortcutCommands[i]->IsTheRightKey(ctrl, shift, code))
					{
						shortcutCommands[i]->Execute();
						return true;
					}
				}
				TextPos begin=textElement->GetCaretBegin();
				TextPos end=textElement->GetCaretEnd();
				switch(code)
				{
				case VKEY_UP:
					{
						end.row--;
						Move(end, shift);
					}
					return true;
				case VKEY_DOWN:
					{
						end.row++;
						Move(end, shift);
					}
					return true;
				case VKEY_LEFT:
					{
						if(ctrl)
						{
							Move(callback->GetLeftWord(end), shift);
						}
						else
						{
							if(end.column==0)
							{
								if(end.row>0)
								{
									end.row--;
									end=textElement->GetLines().Normalize(end);
									end.column=textElement->GetLines().GetLine(end.row).dataLength;
								}
							}
							else
							{
								end.column--;
							}
							Move(end, shift);
						}
					}
					return true;
				case VKEY_RIGHT:
					{
						if(ctrl)
						{
							Move(callback->GetRightWord(end), shift);
						}
						else
						{
							if(end.column==textElement->GetLines().GetLine(end.row).dataLength)
							{
								if(end.row<textElement->GetLines().GetCount()-1)
								{
									end.row++;
									end.column=0;
								}
							}
							else
							{
								end.column++;
							}
							Move(end, shift);
						}
					}
					return true;
				case VKEY_HOME:
					{
						if(ctrl)
						{
							Move(TextPos(0, 0), shift);
						}
						else
						{
							end.column=0;
							Move(end, shift);
						}
					}
					return true;
				case VKEY_END:
					{
						if(ctrl)
						{
							end.row=textElement->GetLines().GetCount()-1;
						}
						end.column=textElement->GetLines().GetLine(end.row).dataLength;
						Move(end, shift);
					}
					return true;
				case VKEY_PRIOR:
					{
						end.row-=callback->GetPageRows();
						Move(end, shift);
					}
					return true;
				case VKEY_NEXT:
					{
						end.row+=callback->GetPageRows();
						Move(end, shift);
					}
					return true;
				case VKEY_BACK:
					if(!readonly)
					{
						if(ctrl && !shift)
						{
							ProcessKey(VKEY_LEFT, true, true);
							ProcessKey(VKEY_BACK, false, false);
						}
						else if(!ctrl && shift)
						{
							ProcessKey(VKEY_UP, true, false);
							ProcessKey(VKEY_BACK, false, false);
						}
						else
						{
							if(begin==end)
							{
								ProcessKey(VKEY_LEFT, true, false);
							}
							SetSelectionText(L"");
						}
						return true;
					}
					break;
				case VKEY_DELETE:
					if(!readonly)
					{
						if(ctrl && !shift)
						{
							ProcessKey(VKEY_RIGHT, true, true);
							ProcessKey(VKEY_DELETE, false, false);
						}
						else if(!ctrl && shift)
						{
							ProcessKey(VKEY_DOWN, true, false);
							ProcessKey(VKEY_DELETE, false, false);
						}
						else
						{
							if(begin==end)
							{
								ProcessKey(VKEY_RIGHT, true, false);
							}
							SetSelectionText(L"");
						}
						return true;
					}
					break;
				}
				return false;
			}

			void GuiTextBoxCommonInterface::OnGotFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				textElement->SetFocused(true);
				textElement->SetCaretVisible(true);
				UpdateCaretPoint();
			}

			void GuiTextBoxCommonInterface::OnLostFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				textElement->SetFocused(false);
				textElement->SetCaretVisible(false);
			}

			void GuiTextBoxCommonInterface::OnCaretNotify(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				textElement->SetCaretVisible(!textElement->GetCaretVisible());
			}

			void GuiTextBoxCommonInterface::OnLeftButtonDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
			{
				if(textControl->GetVisuallyEnabled() && arguments.compositionSource==arguments.eventSource)
				{
					dragging=true;
					TextPos pos=GetNearestTextPos(Point(arguments.x, arguments.y));
					Move(pos, arguments.shift);
				}
			}

			void GuiTextBoxCommonInterface::OnLeftButtonUp(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
			{
				if(textControl->GetVisuallyEnabled() && arguments.compositionSource==arguments.eventSource)
				{
					dragging=false;
				}
			}

			void GuiTextBoxCommonInterface::OnMouseMove(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
			{
				if(textControl->GetVisuallyEnabled() && arguments.compositionSource==arguments.eventSource)
				{
					if(dragging)
					{
						TextPos pos=GetNearestTextPos(Point(arguments.x, arguments.y));
						Move(pos, true);
					}
				}
			}

			void GuiTextBoxCommonInterface::OnKeyDown(compositions::GuiGraphicsComposition* sender, compositions::GuiKeyEventArgs& arguments)
			{
				if(textControl->GetVisuallyEnabled() && arguments.compositionSource==arguments.eventSource)
				{
					if(ProcessKey(arguments.code, arguments.shift, arguments.ctrl))
					{
						arguments.handled=true;
					}
				}
			}

			void GuiTextBoxCommonInterface::OnCharInput(compositions::GuiGraphicsComposition* sender, compositions::GuiCharEventArgs& arguments)
			{
				if(textControl->GetVisuallyEnabled() && arguments.compositionSource==arguments.eventSource)
				{
					if(!readonly && arguments.code!=VKEY_ESCAPE && arguments.code!=VKEY_BACK && !arguments.ctrl)
					{
						SetSelectionText(WString(arguments.code));
					}
				}
			}

			void GuiTextBoxCommonInterface::Install(elements::GuiColorizedTextElement* _textElement, compositions::GuiGraphicsComposition* _textComposition, GuiControl* _textControl)
			{
				textElement=_textElement;
				textComposition=_textComposition;
				textControl=_textControl;
				textComposition->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetSystemCursor(INativeCursor::IBeam));
				SelectionChanged.SetAssociatedComposition(textControl->GetBoundsComposition());

				GuiGraphicsComposition* focusableComposition=textControl->GetFocusableComposition();
				focusableComposition->GetEventReceiver()->gotFocus.AttachMethod(this, &GuiTextBoxCommonInterface::OnGotFocus);
				focusableComposition->GetEventReceiver()->lostFocus.AttachMethod(this, &GuiTextBoxCommonInterface::OnLostFocus);
				focusableComposition->GetEventReceiver()->caretNotify.AttachMethod(this, &GuiTextBoxCommonInterface::OnCaretNotify);
				textComposition->GetEventReceiver()->leftButtonDown.AttachMethod(this, &GuiTextBoxCommonInterface::OnLeftButtonDown);
				textComposition->GetEventReceiver()->leftButtonUp.AttachMethod(this, &GuiTextBoxCommonInterface::OnLeftButtonUp);
				textComposition->GetEventReceiver()->mouseMove.AttachMethod(this, &GuiTextBoxCommonInterface::OnMouseMove);
				focusableComposition->GetEventReceiver()->keyDown.AttachMethod(this, &GuiTextBoxCommonInterface::OnKeyDown);
				focusableComposition->GetEventReceiver()->charInput.AttachMethod(this, &GuiTextBoxCommonInterface::OnCharInput);

				for(vint i=0;i<textEditCallbacks.Count();i++)
				{
					textEditCallbacks[i]->Attach(textElement, elementModifyLock);
				}
			}
			
			GuiTextBoxCommonInterface::ICallback* GuiTextBoxCommonInterface::GetCallback()
			{
				return callback;
			}

			void GuiTextBoxCommonInterface::SetCallback(ICallback* value)
			{
				callback=value;
			}

			bool GuiTextBoxCommonInterface::AttachTextEditCallback(Ptr<ICommonTextEditCallback> value)
			{
				if(textEditCallbacks.Contains(value.Obj()))
				{
					return false;
				}
				else
				{
					textEditCallbacks.Add(value);
					if(textElement)
					{
						value->Attach(textElement, elementModifyLock);
					}
					return true;
				}
			}

			bool GuiTextBoxCommonInterface::DetachTextEditCallback(Ptr<ICommonTextEditCallback> value)
			{
				if(textEditCallbacks.Remove(value.Obj()))
				{
					value->Detach();
					return true;
				}
				else
				{
					return false;
				}
			}

			void GuiTextBoxCommonInterface::AddShortcutCommand(Ptr<ShortcutCommand> shortcutCommand)
			{
				shortcutCommands.Add(shortcutCommand);
			}

			elements::GuiColorizedTextElement* GuiTextBoxCommonInterface::GetTextElement()
			{
				return textElement;
			}

			void GuiTextBoxCommonInterface::UnsafeSetText(const WString& value)
			{
				if(textElement)
				{
					TextPos end;
					if(textElement->GetLines().GetCount()>0)
					{
						end.row=textElement->GetLines().GetCount()-1;
						end.column=textElement->GetLines().GetLine(end.row).dataLength;
					}
					Modify(TextPos(), end, value);
				}
			}

			GuiTextBoxCommonInterface::GuiTextBoxCommonInterface()
				:textElement(0)
				,textComposition(0)
				,textControl(0)
				,callback(0)
				,dragging(false)
				,readonly(false)
			{
				undoRedoProcessor=new GuiTextBoxUndoRedoProcessor(this);
				AttachTextEditCallback(undoRedoProcessor);

				AddShortcutCommand(new ShortcutCommand(true, false, 'Z', Func<bool()>(this, &GuiTextBoxCommonInterface::Undo)));
				AddShortcutCommand(new ShortcutCommand(true, false, 'Y', Func<bool()>(this, &GuiTextBoxCommonInterface::Redo)));
				AddShortcutCommand(new ShortcutCommand(true, false, 'A', Func<void()>(this, &GuiTextBoxCommonInterface::SelectAll)));
				AddShortcutCommand(new ShortcutCommand(true, false, 'X', Func<bool()>(this, &GuiTextBoxCommonInterface::Cut)));
				AddShortcutCommand(new ShortcutCommand(true, false, 'C', Func<bool()>(this, &GuiTextBoxCommonInterface::Copy)));
				AddShortcutCommand(new ShortcutCommand(true, false, 'V', Func<bool()>(this, &GuiTextBoxCommonInterface::Paste)));
			}

			GuiTextBoxCommonInterface::~GuiTextBoxCommonInterface()
			{
				if(colorizer)
				{
					DetachTextEditCallback(colorizer);
					colorizer=0;
				}
				if(undoRedoProcessor)
				{
					DetachTextEditCallback(undoRedoProcessor);
					undoRedoProcessor=0;
				}

				for(vint i=0;i<textEditCallbacks.Count();i++)
				{
					textEditCallbacks[i]->Detach();
				}
				textEditCallbacks.Clear();
			}

			//================ clipboard operations

			bool GuiTextBoxCommonInterface::CanCut()
			{
				return !readonly && textElement->GetCaretBegin()!=textElement->GetCaretEnd() && textElement->GetPasswordChar()==L'\0';
			}

			bool GuiTextBoxCommonInterface::CanCopy()
			{
				return textElement->GetCaretBegin()!=textElement->GetCaretEnd() && textElement->GetPasswordChar()==L'\0';
			}

			bool GuiTextBoxCommonInterface::CanPaste()
			{
				return !readonly && GetCurrentController()->ClipboardService()->ContainsText() && textElement->GetPasswordChar()==L'\0';
			}

			bool GuiTextBoxCommonInterface::Cut()
			{
				if(CanCut())
				{
					GetCurrentController()->ClipboardService()->SetText(GetSelectionText());
					SetSelectionText(L"");
					return true;
				}
				else
				{
					return false;
				}
			}

			bool GuiTextBoxCommonInterface::Copy()
			{
				if(CanCopy())
				{
					GetCurrentController()->ClipboardService()->SetText(GetSelectionText());
					return true;
				}
				else
				{
					return false;
				}
			}

			bool GuiTextBoxCommonInterface::Paste()
			{
				if(CanPaste())
				{
					SetSelectionText(GetCurrentController()->ClipboardService()->GetText());
					return true;
				}
				else
				{
					return false;
				}
			}
			
			//================ editing control
			
			bool GuiTextBoxCommonInterface::GetReadonly()
			{
				return readonly;
			}

			void GuiTextBoxCommonInterface::SetReadonly(bool value)
			{
				readonly=value;
			}

			//================ text operations

			void GuiTextBoxCommonInterface::Select(TextPos begin, TextPos end)
			{
				Move(begin, false);
				Move(end, true);
			}

			void GuiTextBoxCommonInterface::SelectAll()
			{
				vint row=textElement->GetLines().GetCount()-1;
				Move(TextPos(0, 0), false);
				Move(TextPos(row, textElement->GetLines().GetLine(row).dataLength), true);
			}

			WString GuiTextBoxCommonInterface::GetSelectionText()
			{
				TextPos selectionBegin=textElement->GetCaretBegin()<textElement->GetCaretEnd()?textElement->GetCaretBegin():textElement->GetCaretEnd();
				TextPos selectionEnd=textElement->GetCaretBegin()>textElement->GetCaretEnd()?textElement->GetCaretBegin():textElement->GetCaretEnd();
				return textElement->GetLines().GetText(selectionBegin, selectionEnd);
			}

			void GuiTextBoxCommonInterface::SetSelectionText(const WString& value)
			{
				Modify(textElement->GetCaretBegin(), textElement->GetCaretEnd(), value);
			}

			WString GuiTextBoxCommonInterface::GetRowText(vint row)
			{
				TextPos start=textElement->GetLines().Normalize(TextPos(row, 0));
				TextPos end=TextPos(start.row, textElement->GetLines().GetLine(start.row).dataLength);
				return GetFragmentText(start, end);
			}

			WString GuiTextBoxCommonInterface::GetFragmentText(TextPos start, TextPos end)
			{
				start=textElement->GetLines().Normalize(start);
				end=textElement->GetLines().Normalize(end);
				return textElement->GetLines().GetText(start, end);
			}

			TextPos GuiTextBoxCommonInterface::GetCaretBegin()
			{
				return textElement->GetCaretBegin();
			}

			TextPos GuiTextBoxCommonInterface::GetCaretEnd()
			{
				return textElement->GetCaretEnd();
			}

			TextPos GuiTextBoxCommonInterface::GetCaretSmall()
			{
				TextPos c1=GetCaretBegin();
				TextPos c2=GetCaretEnd();
				return c1<c2?c1:c2;
			}

			TextPos GuiTextBoxCommonInterface::GetCaretLarge()
			{
				TextPos c1=GetCaretBegin();
				TextPos c2=GetCaretEnd();
				return c1>c2?c1:c2;
			}

			//================ position query
				
			vint GuiTextBoxCommonInterface::GetRowWidth(vint row)
			{
				return textElement->GetLines().GetRowWidth(row);
			}

			vint GuiTextBoxCommonInterface::GetRowHeight()
			{
				return textElement->GetLines().GetRowHeight();
			}

			vint GuiTextBoxCommonInterface::GetMaxWidth()
			{
				return textElement->GetLines().GetMaxWidth();
			}

			vint GuiTextBoxCommonInterface::GetMaxHeight()
			{
				return textElement->GetLines().GetMaxHeight();
			}

			TextPos GuiTextBoxCommonInterface::GetTextPosFromPoint(Point point)
			{
				Point view=textElement->GetViewPosition();
				return textElement->GetLines().GetTextPosFromPoint(Point(point.x+view.x, point.y+view.y));
			}

			Point GuiTextBoxCommonInterface::GetPointFromTextPos(TextPos pos)
			{
				Point view=textElement->GetViewPosition();
				Point result=textElement->GetLines().GetPointFromTextPos(pos);
				return Point(result.x-view.x, result.y-view.y);
			}

			Rect GuiTextBoxCommonInterface::GetRectFromTextPos(TextPos pos)
			{
				Point view=textElement->GetViewPosition();
				Rect result=textElement->GetLines().GetRectFromTextPos(pos);
				return Rect(Point(result.x1-view.x, result.y1-view.y), result.GetSize());
			}

			TextPos GuiTextBoxCommonInterface::GetNearestTextPos(Point point)
			{
				Point viewPosition=textElement->GetViewPosition();
				Point mousePosition=Point(point.x+viewPosition.x, point.y+viewPosition.y);
				TextPos pos=textElement->GetLines().GetTextPosFromPoint(mousePosition);
				if(pos.column<textElement->GetLines().GetLine(pos.row).dataLength)
				{
					Rect rect=textElement->GetLines().GetRectFromTextPos(pos);
					if(abs((int)(rect.x1-mousePosition.x))>=abs((int)(rect.x2-1-mousePosition.x)))
					{
						pos.column++;
					}
				}
				return pos;
			}

			//================ colorizing

			Ptr<GuiTextBoxColorizerBase> GuiTextBoxCommonInterface::GetColorizer()
			{
				return colorizer;
			}

			void GuiTextBoxCommonInterface::SetColorizer(Ptr<GuiTextBoxColorizerBase> value)
			{
				if(colorizer)
				{
					DetachTextEditCallback(colorizer);
				}
				colorizer=value;
				if(colorizer)
				{
					AttachTextEditCallback(colorizer);
					GetTextElement()->SetColors(colorizer->GetColors());
				}
			}

			//================ auto complete

			Ptr<GuiTextBoxAutoCompleteBase> GuiTextBoxCommonInterface::GetAutoComplete()
			{
				return autoComplete;
			}

			void GuiTextBoxCommonInterface::SetAutoComplete(Ptr<GuiTextBoxAutoCompleteBase> value)
			{
				if(autoComplete)
				{
					DetachTextEditCallback(autoComplete);
				}
				autoComplete=value;
				if(autoComplete)
				{
					AttachTextEditCallback(autoComplete);
				}
			}

			//================ undo redo control

			bool GuiTextBoxCommonInterface::CanUndo()
			{
				return undoRedoProcessor->CanUndo();
			}

			bool GuiTextBoxCommonInterface::CanRedo()
			{
				return undoRedoProcessor->CanRedo();
			}

			void GuiTextBoxCommonInterface::ClearUndoRedo()
			{
				undoRedoProcessor->ClearUndoRedo();
			}

			bool GuiTextBoxCommonInterface::GetModified()
			{
				return undoRedoProcessor->GetModified();
			}

			void GuiTextBoxCommonInterface::NotifyModificationSaved()
			{
				undoRedoProcessor->NotifyModificationSaved();
			}

			bool GuiTextBoxCommonInterface::Undo()
			{
				return undoRedoProcessor->Undo();
			}

			bool GuiTextBoxCommonInterface::Redo()
			{
				return undoRedoProcessor->Redo();
			}
		}
	}
}