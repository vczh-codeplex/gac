#include "GuiTextControls.h"
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
GuiTextElementOperator::DefaultCallback
***********************************************************************/

			GuiTextElementOperator::DefaultCallback::DefaultCallback(elements::GuiColorizedTextElement* _textElement, compositions::GuiGraphicsComposition* _textComposition)
				:textElement(_textElement)
				,textComposition(_textComposition)
			{
			}

			GuiTextElementOperator::DefaultCallback::~DefaultCallback()
			{
			}

			TextPos GuiTextElementOperator::DefaultCallback::GetLeftWord(TextPos pos)
			{
				return pos;
			}

			TextPos GuiTextElementOperator::DefaultCallback::GetRightWord(TextPos pos)
			{
				return pos;
			}

			void GuiTextElementOperator::DefaultCallback::GetWord(TextPos pos, TextPos& begin, TextPos& end)
			{
				begin=pos;
				end=pos;
			}

			int GuiTextElementOperator::DefaultCallback::GetPageRows()
			{
				return textComposition->GetBounds().Height()/textElement->GetLines().GetRowHeight();
			}

			bool GuiTextElementOperator::DefaultCallback::BeforeModify(TextPos start, TextPos end, const WString& originalText, WString& inputText)
			{
				return true;
			}

/***********************************************************************
GuiTextBoxColorizer
***********************************************************************/

			void GuiTextBoxColorizerBase::ColorizerThreadProc(void* argument)
			{
				GuiTextBoxColorizerBase* colorizer=(GuiTextBoxColorizerBase*)argument;
				while(!colorizer->isFinalizing)
				{
					int lineIndex=-1;
					wchar_t* text=0;
					unsigned __int32* colors=0;
					int length=0;
					int lexerState=-1;
					int contextState=-1;

					{
						SpinLock::Scope scope(*colorizer->elementModifyLock);
						if(colorizer->colorizedLineCount>=colorizer->element->GetLines().GetCount())
						{
							colorizer->isColorizerRunning=false;
							break;
						}

						lineIndex=colorizer->colorizedLineCount++;
						TextLine& line=colorizer->element->GetLines().GetLine(lineIndex);
						length=line.dataLength;
						text=new wchar_t[length+2];
						colors=new unsigned __int32[length+2];
						memcpy(text, line.text, sizeof(wchar_t)*length);
						text[length]=L'\r';
						text[length+1]=L'\n';
						lexerState=lineIndex==0?colorizer->GetLexerStartState():colorizer->element->GetLines().GetLine(lineIndex-1).lexerFinalState;
						contextState=lineIndex==0?colorizer->GetContextStartState():colorizer->element->GetLines().GetLine(lineIndex-1).contextFinalState;
					}

					colorizer->ColorizeLineWithCRLF(text, colors, length+2, lexerState, contextState);

					{
						SpinLock::Scope scope(*colorizer->elementModifyLock);
						if(lineIndex<colorizer->colorizedLineCount && lineIndex<colorizer->element->GetLines().GetCount())
						{
							TextLine& line=colorizer->element->GetLines().GetLine(lineIndex);
							line.lexerFinalState=lexerState;
							line.contextFinalState=contextState;
							for(int i=0;i<length;i++)
							{
								line.att[i].colorIndex=colors[i];
							}
						}
						delete[] text;
						delete[] colors;
					}
				}
				colorizer->colorizerRunningEvent.Leave();
			}

			void GuiTextBoxColorizerBase::StartColorizer()
			{
				if(!isColorizerRunning)
				{
					isColorizerRunning=true;
					colorizerRunningEvent.Enter();
					ThreadPoolLite::Queue(&GuiTextBoxColorizerBase::ColorizerThreadProc, this);
				}
			}

			void GuiTextBoxColorizerBase::StopColorizer()
			{
				isFinalizing=true;
				colorizerRunningEvent.Enter();
				colorizerRunningEvent.Leave();
				colorizedLineCount=0;
				isFinalizing=false;
			}

			GuiTextBoxColorizerBase::GuiTextBoxColorizerBase()
				:element(0)
				,elementModifyLock(0)
				,colorizedLineCount(0)
				,isColorizerRunning(false)
				,isFinalizing(false)
			{
			}

			GuiTextBoxColorizerBase::~GuiTextBoxColorizerBase()
			{
				StopColorizer();
			}

			void GuiTextBoxColorizerBase::Attach(elements::GuiColorizedTextElement* _element, SpinLock& _elementModifyLock)
			{
				if(_element)
				{
					SpinLock::Scope scope(_elementModifyLock);
					element=_element;
					elementModifyLock=&_elementModifyLock;
					StartColorizer();
				}
			}

			void GuiTextBoxColorizerBase::Detach()
			{
				if(element && elementModifyLock)
				{
					StopColorizer();
					SpinLock::Scope scope(*elementModifyLock);
					element=0;
					elementModifyLock=0;
				}
			}

			void GuiTextBoxColorizerBase::TextEditNotify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)
			{
				if(element && elementModifyLock)
				{
					SpinLock::Scope scope(*elementModifyLock);
					int line=originalStart.row<originalEnd.row?originalStart.row:originalEnd.row;
					if(colorizedLineCount>line)
					{
						colorizedLineCount=line;
					}
					StartColorizer();
				}
			}

/***********************************************************************
GuiTextBoxRegexColorizer
***********************************************************************/

			struct GuiTextBoxRegexColorizerProcData
			{
				GuiTextBoxRegexColorizer*		colorizer;
				const wchar_t*					text;
				unsigned __int32*				colors;
				int								contextState;
			};

			void GuiTextBoxRegexColorizer::ColorizerProc(void* argument, vint start, vint length, vint token)
			{
				GuiTextBoxRegexColorizerProcData& data=*(GuiTextBoxRegexColorizerProcData*)argument;
				data.colorizer->ColorizeTokenContextSensitive(data.text, start, length, token, data.contextState);
				for(int i=0;i<length;i++)
				{
					data.colors[start+i]=token+1;
				}
			}

			GuiTextBoxRegexColorizer::GuiTextBoxRegexColorizer()
			{
				colors.Resize(1);
			}

			GuiTextBoxRegexColorizer::~GuiTextBoxRegexColorizer()
			{
			}

			elements::text::ColorEntry GuiTextBoxRegexColorizer::GetDefaultColor()
			{
				return defaultColor;
			}

			collections::IReadonlyList<WString>& GuiTextBoxRegexColorizer::GetTokenRegexes()
			{
				return tokenRegexes.Wrap();
			}

			collections::IReadonlyList<elements::text::ColorEntry>& GuiTextBoxRegexColorizer::GetTokenColors()
			{
				return tokenColors.Wrap();
			}

			collections::IReadonlyList<elements::text::ColorEntry>& GuiTextBoxRegexColorizer::GetExtraTokenColors()
			{
				return extraTokenColors.Wrap();
			}

			int GuiTextBoxRegexColorizer::GetExtraTokenIndexStart()
			{
				if(lexer)
				{
					return tokenColors.Count();
				}
				else
				{
					return -1;
				}
			}

			bool GuiTextBoxRegexColorizer::SetDefaultColor(elements::text::ColorEntry value)
			{
				if(lexer)
				{
					return false;
				}
				else
				{
					defaultColor=value;
					return true;
				}
			}

			int GuiTextBoxRegexColorizer::AddToken(const WString& regex, elements::text::ColorEntry color)
			{
				if(lexer)
				{
					return -1;
				}
				else
				{
					tokenRegexes.Add(regex);
					tokenColors.Add(color);
					return tokenColors.Count()-1;
				}
			}

			int GuiTextBoxRegexColorizer::AddExtraToken(elements::text::ColorEntry color)
			{
				if(lexer)
				{
					return -1;
				}
				else
				{
					extraTokenColors.Add(color);
					return extraTokenColors.Count()-1;
				}
			}

			bool GuiTextBoxRegexColorizer::Setup()
			{
				if(lexer || tokenRegexes.Count()==0)
				{
					return false;
				}
				else
				{
					lexer=new regex::RegexLexer(tokenRegexes.Wrap());
					colors.Resize(1+tokenRegexes.Count()+extraTokenColors.Count());
					colors[0]=defaultColor;
					for(int i=0;i<tokenColors.Count();i++)
					{
						colors[i+1]=tokenColors[i];
					}
					for(int i=0;i<extraTokenColors.Count();i++)
					{
						colors[i+1+tokenColors.Count()]=extraTokenColors[i];
					}
					colorizer=new regex::RegexLexerColorizer(lexer->Colorize());
					return true;
				}
			}

			void GuiTextBoxRegexColorizer::ColorizeTokenContextSensitive(const wchar_t* text, vint start, vint length, vint& token, int& contextState)
			{
			}

			int GuiTextBoxRegexColorizer::GetLexerStartState()
			{
				return lexer?colorizer->GetStartState():-1;
			}

			int GuiTextBoxRegexColorizer::GetContextStartState()
			{
				return 0;
			}

			void GuiTextBoxRegexColorizer::ColorizeLineWithCRLF(const wchar_t* text, unsigned __int32* colors, int length, int& lexerState, int& contextState)
			{
				if(lexer)
				{
					GuiTextBoxRegexColorizerProcData data;
					data.colorizer=this;
					data.text=text;
					data.colors=colors;
					data.contextState=contextState;

					memset(colors, 0, sizeof(*colors)*length);
					colorizer->Reset(lexerState);
					colorizer->Colorize(text, length, &GuiTextBoxRegexColorizer::ColorizerProc, &data);

					lexerState=colorizer->GetCurrentState();
					contextState=data.contextState;
				}
				else
				{
					lexerState=-1;
					contextState=-1;
				}
			}

			const GuiTextBoxRegexColorizer::ColorArray& GuiTextBoxRegexColorizer::GetColors()
			{
				return colors;
			}

/***********************************************************************
GuiTextElementOperator
***********************************************************************/

			void GuiTextElementOperator::UpdateCaretPoint()
			{
				GuiGraphicsHost* host=textComposition->GetRelatedGraphicsHost();
				if(host)
				{
					Rect caret=textElement->GetLines().GetRectFromTextPos(textElement->GetCaretEnd());
					Point view=textElement->GetViewPosition();
					int textMargin=callback->GetTextMargin();
					int x=caret.x1-view.x;
					int y=caret.y2-view.y;
					host->SetCaretPoint(Point(x, y), textComposition);
				}
			}

			void GuiTextElementOperator::Move(TextPos pos, bool shift)
			{
				TextPos oldBegin=textElement->GetCaretBegin();
				TextPos oldEnd=textElement->GetCaretEnd();

				pos=textElement->GetLines().Normalize(pos);
				if(!shift)
				{
					textElement->SetCaretBegin(pos);
				}
				textElement->SetCaretEnd(pos);
				textElement->SetCaretVisible(true);

				Rect bounds=textElement->GetLines().GetRectFromTextPos(pos);
				Rect view=Rect(textElement->GetViewPosition(), textComposition->GetBounds().GetSize());
				Point viewPoint=view.LeftTop();
				int offsetX=textElement->GetLines().GetRowHeight()*5;

				if(bounds.x1<view.x1)
				{
					viewPoint.x=bounds.x1-offsetX;
				}
				else if(bounds.x2>view.x2)
				{
					viewPoint.x=bounds.x2-view.Width()+offsetX;
				}
				if(bounds.y1<view.y1)
				{
					viewPoint.y=bounds.y1;
				}
				else if(bounds.y2>view.y2)
				{
					viewPoint.y=bounds.y2-view.Height();
				}

				callback->ScrollToView(viewPoint);
				UpdateCaretPoint();

				if(textBoxCommonInterface)
				{
					if(oldBegin!=textElement->GetCaretBegin() || oldEnd!=textElement->GetCaretEnd())
					{
						textBoxCommonInterface->RaiseSelectionChanged();
					}
				}
			}

			void GuiTextElementOperator::Modify(TextPos start, TextPos end, const WString& input)
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
					for(int i=0;i<textEditCallbacks.Count();i++)
					{
						textEditCallbacks[i]->TextEditNotify(originalStart, originalEnd, originalText, start, end, inputText);
					}
					Move(end, false);

					if(textBoxCommonInterface)
					{
						textBoxCommonInterface->RaiseTextChanged();
					}
				}
			}

			void GuiTextElementOperator::ProcessKey(int code, bool shift, bool ctrl)
			{
				if(!shift && ctrl)
				{
					switch(code)
					{
					case L'A':
						SelectAll();
						return;
					case L'X':
						Cut();
						return;
					case L'C':
						Copy();
						return;
					case L'V':
						Paste();
						return;
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
					break;
				case VKEY_DOWN:
					{
						end.row++;
						Move(end, shift);
					}
					break;
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
								end.row--;
								end=textElement->GetLines().Normalize(end);
								end.column=textElement->GetLines().GetLine(end.row).dataLength;
							}
							else
							{
								end.column--;
							}
							Move(end, shift);
						}
					}
					break;
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
								end.row++;
								end.column=0;
							}
							else
							{
								end.column++;
							}
							Move(end, shift);
						}
					}
					break;
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
					break;
				case VKEY_END:
					{
						if(ctrl)
						{
							end.row=textElement->GetLines().GetCount()-1;
						}
						end.column=textElement->GetLines().GetLine(end.row).dataLength;
						Move(end, shift);
					}
					break;
				case VKEY_PRIOR:
					{
						end.row-=callback->GetPageRows();
						Move(end, shift);
					}
					break;
				case VKEY_NEXT:
					{
						end.row+=callback->GetPageRows();
						Move(end, shift);
					}
					break;
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
					}
					break;
				}
			}

			void GuiTextElementOperator::OnGotFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				textElement->SetFocused(true);
				textElement->SetCaretVisible(true);
				UpdateCaretPoint();
			}

			void GuiTextElementOperator::OnLostFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				textElement->SetFocused(false);
				textElement->SetCaretVisible(false);
			}

			void GuiTextElementOperator::OnCaretNotify(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				textElement->SetCaretVisible(!textElement->GetCaretVisible());
			}

			void GuiTextElementOperator::OnLeftButtonDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
			{
				if(textControl->GetVisuallyEnabled() && arguments.compositionSource==arguments.eventSource)
				{
					dragging=true;
					TextPos pos=GetNearestTextPos(Point(arguments.x, arguments.y));
					Move(pos, arguments.shift);
				}
			}

			void GuiTextElementOperator::OnLeftButtonUp(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
			{
				if(textControl->GetVisuallyEnabled() && arguments.compositionSource==arguments.eventSource)
				{
					dragging=false;
				}
			}

			void GuiTextElementOperator::OnMouseMove(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
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

			void GuiTextElementOperator::OnKeyDown(compositions::GuiGraphicsComposition* sender, compositions::GuiKeyEventArgs& arguments)
			{
				if(textControl->GetVisuallyEnabled() && arguments.compositionSource==arguments.eventSource)
				{
					ProcessKey(arguments.code, arguments.shift, arguments.ctrl);
				}
			}

			void GuiTextElementOperator::OnCharInput(compositions::GuiGraphicsComposition* sender, compositions::GuiCharEventArgs& arguments)
			{
				if(textControl->GetVisuallyEnabled() && arguments.compositionSource==arguments.eventSource)
				{
					if(!readonly && arguments.code!=VKEY_ESCAPE && arguments.code!=VKEY_BACK && !arguments.ctrl)
					{
						SetSelectionText(WString(arguments.code));
					}
				}
			}

			GuiTextElementOperator::GuiTextElementOperator()
				:textElement(0)
				,textComposition(0)
				,textControl(0)
				,textBoxCommonInterface(0)
				,callback(0)
				,dragging(false)
				,readonly(false)
			{
			}

			GuiTextElementOperator::~GuiTextElementOperator()
			{
				for(int i=0;i<textEditCallbacks.Count();i++)
				{
					textEditCallbacks[i]->Detach();
				}
				textEditCallbacks.Clear();
			}

			void GuiTextElementOperator::Install(elements::GuiColorizedTextElement* _textElement, compositions::GuiGraphicsComposition* _textComposition, GuiControl* _textControl)
			{
				textElement=_textElement;
				textComposition=_textComposition;
				textControl=_textControl;
				textComposition->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetSystemCursor(INativeCursor::IBeam));

				GuiGraphicsComposition* focusableComposition=textControl->GetFocusableComposition();
				focusableComposition->GetEventReceiver()->gotFocus.AttachMethod(this, &GuiTextElementOperator::OnGotFocus);
				focusableComposition->GetEventReceiver()->lostFocus.AttachMethod(this, &GuiTextElementOperator::OnLostFocus);
				focusableComposition->GetEventReceiver()->caretNotify.AttachMethod(this, &GuiTextElementOperator::OnCaretNotify);
				textComposition->GetEventReceiver()->leftButtonDown.AttachMethod(this, &GuiTextElementOperator::OnLeftButtonDown);
				textComposition->GetEventReceiver()->leftButtonUp.AttachMethod(this, &GuiTextElementOperator::OnLeftButtonUp);
				textComposition->GetEventReceiver()->mouseMove.AttachMethod(this, &GuiTextElementOperator::OnMouseMove);
				focusableComposition->GetEventReceiver()->keyDown.AttachMethod(this, &GuiTextElementOperator::OnKeyDown);
				focusableComposition->GetEventReceiver()->charInput.AttachMethod(this, &GuiTextElementOperator::OnCharInput);

				for(int i=0;i<textEditCallbacks.Count();i++)
				{
					textEditCallbacks[i]->Attach(textElement, elementModifyLock);
				}
			}
			
			GuiTextElementOperator::ICallback* GuiTextElementOperator::GetCallback()
			{
				return callback;
			}

			void GuiTextElementOperator::SetCallback(ICallback* value)
			{
				callback=value;
			}

			bool GuiTextElementOperator::AttachTextEditCallback(Ptr<ITextEditCallback> value)
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

			bool GuiTextElementOperator::DetachTextEditCallback(Ptr<ITextEditCallback> value)
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

			GuiTextBoxCommonInterface* GuiTextElementOperator::GetTextBoxCommonInterface()
			{
				return textBoxCommonInterface;
			}

			void GuiTextElementOperator::SetTextBoxCommonInterface(GuiTextBoxCommonInterface* value)
			{
				textBoxCommonInterface=value;
			}

			elements::GuiColorizedTextElement* GuiTextElementOperator::GetTextElement()
			{
				return textElement;
			}

			compositions::GuiGraphicsComposition* GuiTextElementOperator::GetTextComposition()
			{
				return textComposition;
			}

			void GuiTextElementOperator::Select(TextPos begin, TextPos end)
			{
				Move(begin, false);
				Move(end, true);
			}

			TextPos GuiTextElementOperator::GetNearestTextPos(Point point)
			{
				Point viewPosition=textElement->GetViewPosition();
				Point mousePosition=Point(point.x+viewPosition.x, point.y+viewPosition.y);
				TextPos pos=textElement->GetLines().GetTextPosFromPoint(mousePosition);
				if(pos.column<textElement->GetLines().GetLine(pos.row).dataLength)
				{
					Rect rect=textElement->GetLines().GetRectFromTextPos(pos);
					if(abs(rect.x1-mousePosition.x)>=abs(rect.x2-1-mousePosition.x))
					{
						pos.column++;
					}
				}
				return pos;
			}

			WString GuiTextElementOperator::GetSelectionText()
			{
				TextPos selectionBegin=textElement->GetCaretBegin()<textElement->GetCaretEnd()?textElement->GetCaretBegin():textElement->GetCaretEnd();
				TextPos selectionEnd=textElement->GetCaretBegin()>textElement->GetCaretEnd()?textElement->GetCaretBegin():textElement->GetCaretEnd();
				return textElement->GetLines().GetText(selectionBegin, selectionEnd);
			}

			void GuiTextElementOperator::SetSelectionText(const WString& value)
			{
				Modify(textElement->GetCaretBegin(), textElement->GetCaretEnd(), value);
			}

			void GuiTextElementOperator::SetText(const WString& value)
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

			bool GuiTextElementOperator::CanCut()
			{
				return !readonly && textElement->GetCaretBegin()!=textElement->GetCaretEnd();
			}

			bool GuiTextElementOperator::CanCopy()
			{
				return textElement->GetCaretBegin()!=textElement->GetCaretEnd();
			}

			bool GuiTextElementOperator::CanPaste()
			{
				return !readonly && GetCurrentController()->ClipboardService()->ContainsText();
			}

			void GuiTextElementOperator::SelectAll()
			{
				int row=textElement->GetLines().GetCount()-1;
				Move(TextPos(0, 0), false);
				Move(TextPos(row, textElement->GetLines().GetLine(row).dataLength), true);
			}

			bool GuiTextElementOperator::Cut()
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

			bool GuiTextElementOperator::Copy()
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

			bool GuiTextElementOperator::Paste()
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
			
			bool GuiTextElementOperator::GetReadonly()
			{
				return readonly;
			}

			void GuiTextElementOperator::SetReadonly(bool value)
			{
				readonly=value;
			}

/***********************************************************************
GuiTextElementOperator
***********************************************************************/

			void GuiTextBoxCommonInterface::RaiseTextChanged()
			{
				textControl->TextChanged.Execute(textControl->GetNotifyEventArguments());
			}

			void GuiTextBoxCommonInterface::RaiseSelectionChanged()
			{
				SelectionChanged.Execute(textControl->GetNotifyEventArguments());
			}

			void GuiTextBoxCommonInterface::InitializeCommonInterface(GuiControl* _textControl, GuiTextElementOperator* _textElementOperator)
			{
				textElementOperator=_textElementOperator;
				textControl=_textControl;
				SelectionChanged.SetAssociatedComposition(textControl->GetBoundsComposition());
				textElementOperator->SetTextBoxCommonInterface(this);
			}

			GuiTextBoxCommonInterface::GuiTextBoxCommonInterface()
				:textElementOperator(0)
				,textControl(0)
			{
			}

			GuiTextBoxCommonInterface::~GuiTextBoxCommonInterface()
			{
			}

			compositions::GuiGraphicsComposition* GuiTextBoxCommonInterface::GetTextComposition()
			{
				return textElementOperator->GetTextComposition();
			}

			Ptr<GuiTextBoxColorizerBase> GuiTextBoxCommonInterface::GetColorizer()
			{
				return colorizer;
			}

			void GuiTextBoxCommonInterface::SetColorizer(Ptr<GuiTextBoxColorizerBase> value)
			{
				if(textElementOperator)
				{
					if(colorizer)
					{
						textElementOperator->DetachTextEditCallback(colorizer);
					}
					colorizer=value;
					if(colorizer)
					{
						textElementOperator->AttachTextEditCallback(colorizer);
						textElementOperator->GetTextElement()->SetColors(colorizer->GetColors());
					}
				}
			}

			bool GuiTextBoxCommonInterface::CanCut()
			{
				return textElementOperator->CanCut();
			}

			bool GuiTextBoxCommonInterface::CanCopy()
			{
				return textElementOperator->CanCopy();
			}

			bool GuiTextBoxCommonInterface::CanPaste()
			{
				return textElementOperator->CanPaste();
			}

			void GuiTextBoxCommonInterface::SelectAll()
			{
				textElementOperator->SelectAll();
			}

			bool GuiTextBoxCommonInterface::Cut()
			{
				return textElementOperator->Cut();
			}

			bool GuiTextBoxCommonInterface::Copy()
			{
				return textElementOperator->Copy();
			}

			bool GuiTextBoxCommonInterface::Paste()
			{
				return textElementOperator->Paste();
			}

			WString GuiTextBoxCommonInterface::GetRowText(int row)
			{
				TextPos start=textElementOperator->GetTextElement()->GetLines().Normalize(TextPos(row, 0));
				TextPos end=TextPos(start.row, textElementOperator->GetTextElement()->GetLines().GetLine(start.row).dataLength);
				return GetFragmentText(start, end);
			}

			WString GuiTextBoxCommonInterface::GetFragmentText(TextPos start, TextPos end)
			{
				start=textElementOperator->GetTextElement()->GetLines().Normalize(start);
				end=textElementOperator->GetTextElement()->GetLines().Normalize(end);
				return textElementOperator->GetTextElement()->GetLines().GetText(start, end);
			}
				
			int GuiTextBoxCommonInterface::GetRowWidth(int row)
			{
				return textElementOperator->GetTextElement()->GetLines().GetRowWidth(row);
			}

			int GuiTextBoxCommonInterface::GetRowHeight()
			{
				return textElementOperator->GetTextElement()->GetLines().GetRowHeight();
			}

			int GuiTextBoxCommonInterface::GetMaxWidth()
			{
				return textElementOperator->GetTextElement()->GetLines().GetMaxWidth();
			}

			int GuiTextBoxCommonInterface::GetMaxHeight()
			{
				return textElementOperator->GetTextElement()->GetLines().GetMaxHeight();
			}

			TextPos GuiTextBoxCommonInterface::GetTextPosFromPoint(Point point)
			{
				Point view=textElementOperator->GetTextElement()->GetViewPosition();
				return textElementOperator->GetTextElement()->GetLines().GetTextPosFromPoint(Point(point.x+view.x, point.y+view.y));
			}

			Point GuiTextBoxCommonInterface::GetPointFromTextPos(TextPos pos)
			{
				Point view=textElementOperator->GetTextElement()->GetViewPosition();
				Point result=textElementOperator->GetTextElement()->GetLines().GetPointFromTextPos(pos);
				return Point(result.x-view.x, result.y-view.y);
			}

			Rect GuiTextBoxCommonInterface::GetRectFromTextPos(TextPos pos)
			{
				Point view=textElementOperator->GetTextElement()->GetViewPosition();
				Rect result=textElementOperator->GetTextElement()->GetLines().GetRectFromTextPos(pos);
				return Rect(Point(result.x1-view.x, result.y1-view.y), result.GetSize());
			}

			TextPos GuiTextBoxCommonInterface::GetNearestTextPos(Point point)
			{
				return textElementOperator->GetNearestTextPos(point);
			}

			TextPos GuiTextBoxCommonInterface::GetCaretBegin()
			{
				return textElementOperator->GetTextElement()->GetCaretBegin();
			}

			TextPos GuiTextBoxCommonInterface::GetCaretEnd()
			{
				return textElementOperator->GetTextElement()->GetCaretEnd();
			}

			TextPos GuiTextBoxCommonInterface::GetCaretSmall()
			{
				TextPos c1=GetCaretBegin();
				TextPos c2=GetCaretBegin();
				return c1<c2?c1:c2;
			}

			TextPos GuiTextBoxCommonInterface::GetCaretLarge()
			{
				TextPos c1=GetCaretBegin();
				TextPos c2=GetCaretBegin();
				return c1>c2?c1:c2;
			}

			void GuiTextBoxCommonInterface::Select(TextPos begin, TextPos end)
			{
				begin=textElementOperator->GetTextElement()->GetLines().Normalize(begin);
				end=textElementOperator->GetTextElement()->GetLines().Normalize(end);
				textElementOperator->Select(begin, end);
			}

			WString GuiTextBoxCommonInterface::GetSelectionText()
			{
				return textElementOperator->GetSelectionText();
			}

			void GuiTextBoxCommonInterface::SetSelectionText(const WString& value)
			{
				return textElementOperator->SetSelectionText(value);
			}

			bool GuiTextBoxCommonInterface::GetReadonly()
			{
				return textElementOperator->GetReadonly();
			}

			void GuiTextBoxCommonInterface::SetReadonly(bool value)
			{
				textElementOperator->SetReadonly(value);
			}

/***********************************************************************
GuiMultilineTextBox::StyleController
***********************************************************************/

			GuiMultilineTextBox::StyleController::StyleController(GuiScrollView::IStyleProvider* styleProvider)
				:GuiScrollView::StyleController(styleProvider)
				,textElement(0)
				,textComposition(0)
			{
				textElement=GuiColorizedTextElement::Create();

				textComposition=new GuiBoundsComposition;
				textComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
				textComposition->SetOwnedElement(textElement);

				GetInternalContainerComposition()->AddChild(textComposition);
			}

			GuiMultilineTextBox::StyleController::~StyleController()
			{
			}

			elements::GuiColorizedTextElement* GuiMultilineTextBox::StyleController::GetTextElement()
			{
				return textElement;
			}

			compositions::GuiGraphicsComposition* GuiMultilineTextBox::StyleController::GetTextComposition()
			{
				return textComposition;
			}

			GuiTextElementOperator* GuiMultilineTextBox::StyleController::GetTextElementOperator()
			{
				return &textElementOperator;
			}

			void GuiMultilineTextBox::StyleController::SetViewPosition(Point value)
			{
				textElement->SetViewPosition(value);
			}

			void GuiMultilineTextBox::StyleController::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
				GuiScrollView::StyleController::SetFocusableComposition(value);
				textElementOperator.Install(textElement, textComposition, scrollView);
				if(!textElementOperator.GetCallback())
				{
					if(!defaultCallback)
					{
						defaultCallback=new TextElementOperatorCallback(dynamic_cast<GuiMultilineTextBox*>(scrollView));
					}
					textElementOperator.SetCallback(defaultCallback.Obj());
				}
			}

			WString GuiMultilineTextBox::StyleController::GetText()
			{
				return textElement->GetLines().GetText();
			}

			void GuiMultilineTextBox::StyleController::SetText(const WString& value)
			{
				textElementOperator.SetText(value);
				textElement->SetCaretBegin(TextPos(0, 0));
				textElement->SetCaretEnd(TextPos(0, 0));
				GuiScrollView::StyleController::SetText(value);
			}

			void GuiMultilineTextBox::StyleController::SetFont(const FontProperties& value)
			{
				textElement->SetFont(value);
				GuiScrollView::StyleController::SetFont(value);
			}

			void GuiMultilineTextBox::StyleController::SetVisuallyEnabled(bool value)
			{
				textElement->SetVisuallyEnabled(value);
				GuiScrollView::StyleController::SetVisuallyEnabled(value);
			}

/***********************************************************************
GuiMultilineTextBox::DefaultTextElementOperatorCallback
***********************************************************************/

			GuiMultilineTextBox::TextElementOperatorCallback::TextElementOperatorCallback(GuiMultilineTextBox* _textControl)
				:GuiTextElementOperator::DefaultCallback(
					dynamic_cast<StyleController*>(_textControl->GetStyleController())->GetTextElement(),
					dynamic_cast<StyleController*>(_textControl->GetStyleController())->GetTextComposition()
					)
				,textControl(_textControl)
				,textController(dynamic_cast<StyleController*>(_textControl->GetStyleController()))
			{
			}

			void GuiMultilineTextBox::TextElementOperatorCallback::AfterModify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)
			{
				textControl->CalculateView();
			}
			
			void GuiMultilineTextBox::TextElementOperatorCallback::ScrollToView(Point point)
			{
				point.x+=TextMargin;
				point.y+=TextMargin;
				Point oldPoint(textControl->GetHorizontalScroll()->GetPosition(), textControl->GetVerticalScroll()->GetPosition());
				int marginX=0;
				int marginY=0;
				if(oldPoint.x<point.x)
				{
					marginX=TextMargin;
				}
				else if(oldPoint.x>point.x)
				{
					marginX=-TextMargin;
				}
				if(oldPoint.y<point.y)
				{
					marginY=TextMargin;
				}
				else if(oldPoint.y>point.y)
				{
					marginY=-TextMargin;
				}
				textControl->GetHorizontalScroll()->SetPosition(point.x+marginX);
				textControl->GetVerticalScroll()->SetPosition(point.y+marginY);
			}

			int GuiMultilineTextBox::TextElementOperatorCallback::GetTextMargin()
			{
				return TextMargin;
			}

/***********************************************************************
GuiMultilineTextBox
***********************************************************************/

			void GuiMultilineTextBox::CalculateViewAndSetScroll()
			{
				CalculateView();
				int smallMove=styleController->GetTextElement()->GetLines().GetRowHeight();
				int bigMove=smallMove*5;
				styleController->GetHorizontalScroll()->SetSmallMove(smallMove);
				styleController->GetHorizontalScroll()->SetBigMove(bigMove);
				styleController->GetVerticalScroll()->SetSmallMove(smallMove);
				styleController->GetVerticalScroll()->SetBigMove(bigMove);
			}

			void GuiMultilineTextBox::OnRenderTargetChanged(elements::IGuiGraphicsRenderTarget* renderTarget)
			{
				CalculateViewAndSetScroll();
				GuiScrollView::OnRenderTargetChanged(renderTarget);
			}

			Size GuiMultilineTextBox::QueryFullSize()
			{
				text::TextLines& lines=styleController->GetTextElement()->GetLines();
				return Size(lines.GetMaxWidth()+TextMargin*2, lines.GetMaxHeight()+TextMargin*2);
			}

			void GuiMultilineTextBox::UpdateView(Rect viewBounds)
			{
				styleController->SetViewPosition(viewBounds.LeftTop()-Size(TextMargin, TextMargin));
			}

			void GuiMultilineTextBox::OnBoundsMouseButtonDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
			{
				if(GetVisuallyEnabled())
				{
					boundsComposition->GetRelatedGraphicsHost()->SetFocus(boundsComposition);
				}
			}

			GuiMultilineTextBox::GuiMultilineTextBox(GuiMultilineTextBox::IStyleProvider* styleProvider)
				:GuiScrollView(new StyleController(styleProvider))
			{
				styleController=dynamic_cast<StyleController*>(GetStyleController());

				boundsComposition->GetEventReceiver()->leftButtonDown.AttachMethod(this, &GuiMultilineTextBox::OnBoundsMouseButtonDown);
				boundsComposition->GetEventReceiver()->middleButtonDown.AttachMethod(this, &GuiMultilineTextBox::OnBoundsMouseButtonDown);
				boundsComposition->GetEventReceiver()->rightButtonDown.AttachMethod(this, &GuiMultilineTextBox::OnBoundsMouseButtonDown);
				SetFocusableComposition(boundsComposition);

				InitializeCommonInterface(this, styleController->GetTextElementOperator());
			}

			GuiMultilineTextBox::~GuiMultilineTextBox()
			{
			}

			const WString& GuiMultilineTextBox::GetText()
			{
				text=styleController->GetText();
				return text;
			}

			void GuiMultilineTextBox::SetText(const WString& value)
			{
				GuiScrollView::SetText(value);
				CalculateView();
			}

			void GuiMultilineTextBox::SetFont(const FontProperties& value)
			{
				GuiScrollView::SetFont(value);
				CalculateViewAndSetScroll();
			}

/***********************************************************************
GuiSinglelineTextBox::StyleController
***********************************************************************/

			GuiSinglelineTextBox::StyleController::StyleController(IStyleProvider* _styleProvider)
				:styleProvider(_styleProvider)
				,boundsComposition(0)
				,containerComposition(0)
				,textBox(0)
				,textElement(0)
				,textComposition(0)
			{
				boundsComposition=new GuiBoundsComposition;
				containerComposition=styleProvider->InstallBackground(boundsComposition);

				textElement=GuiColorizedTextElement::Create();
				textElement->SetViewPosition(Point(-TextMargin, -TextMargin));

				textCompositionTable=new GuiTableComposition;
				textCompositionTable->SetAlignmentToParent(Margin(0, 0, 0, 0));
				textCompositionTable->SetRowsAndColumns(3, 1);
				textCompositionTable->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
				textCompositionTable->SetRowOption(1, GuiCellOption::AbsoluteOption(0));
				textCompositionTable->SetRowOption(2, GuiCellOption::PercentageOption(0.5));
				textCompositionTable->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
				containerComposition->AddChild(textCompositionTable);

				textComposition=new GuiCellComposition;
				textComposition->SetOwnedElement(textElement);
				textCompositionTable->AddChild(textComposition);
				textComposition->SetSite(1, 0, 1, 1);

				styleProvider->AssociateStyleController(this);
			}

			GuiSinglelineTextBox::StyleController::~StyleController()
			{
			}

			void GuiSinglelineTextBox::StyleController::SetTextBox(GuiSinglelineTextBox* value)
			{
				textBox=value;
			}

			void GuiSinglelineTextBox::StyleController::RearrangeTextElement()
			{
				textCompositionTable->SetRowOption(1, GuiCellOption::AbsoluteOption(textElement->GetLines().GetRowHeight()+2*TextMargin));
			}

			compositions::GuiBoundsComposition* GuiSinglelineTextBox::StyleController::GetBoundsComposition()
			{
				return boundsComposition;
			}

			compositions::GuiGraphicsComposition* GuiSinglelineTextBox::StyleController::GetContainerComposition()
			{
				return containerComposition;
			}

			void GuiSinglelineTextBox::StyleController::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
				styleProvider->SetFocusableComposition(value);
				textElementOperator.Install(textElement, textComposition, textBox);
				if(!textElementOperator.GetCallback())
				{
					if(!defaultCallback)
					{
						defaultCallback=new TextElementOperatorCallback(textBox);
					}
					textElementOperator.SetCallback(defaultCallback.Obj());
				}
			}

			WString GuiSinglelineTextBox::StyleController::GetText()
			{
				return textElement->GetLines().GetText();
			}

			void GuiSinglelineTextBox::StyleController::SetText(const WString& value)
			{
				textElementOperator.SetText(value);
				textElement->SetCaretBegin(TextPos(0, 0));
				textElement->SetCaretEnd(TextPos(0, 0));
				styleProvider->SetText(value);
			}

			void GuiSinglelineTextBox::StyleController::SetFont(const FontProperties& value)
			{
				textElement->SetFont(value);
				styleProvider->SetFont(value);
			}

			void GuiSinglelineTextBox::StyleController::SetVisuallyEnabled(bool value)
			{
				textElement->SetVisuallyEnabled(value);
				styleProvider->SetVisuallyEnabled(value);
			}

			elements::GuiColorizedTextElement* GuiSinglelineTextBox::StyleController::GetTextElement()
			{
				return textElement;
			}

			compositions::GuiGraphicsComposition* GuiSinglelineTextBox::StyleController::GetTextComposition()
			{
				return textComposition;
			}

			GuiTextElementOperator* GuiSinglelineTextBox::StyleController::GetTextElementOperator()
			{
				return &textElementOperator;
			}

			void GuiSinglelineTextBox::StyleController::SetViewPosition(Point value)
			{
				textElement->SetViewPosition(value);
			}

/***********************************************************************
GuiSinglelineTextBox::DefaultTextElementOperatorCallback
***********************************************************************/

			GuiSinglelineTextBox::TextElementOperatorCallback::TextElementOperatorCallback(GuiSinglelineTextBox* _textControl)
				:GuiTextElementOperator::DefaultCallback(
					dynamic_cast<StyleController*>(_textControl->GetStyleController())->GetTextElement(),
					dynamic_cast<StyleController*>(_textControl->GetStyleController())->GetTextComposition()
					)
				,textControl(_textControl)
				,textController(dynamic_cast<StyleController*>(_textControl->GetStyleController()))
			{
			}

			bool GuiSinglelineTextBox::TextElementOperatorCallback::BeforeModify(TextPos start, TextPos end, const WString& originalText, WString& inputText)
			{
				int length=inputText.Length();
				const wchar_t* input=inputText.Buffer();
				for(int i=0;i<length;i++)
				{
					if(*input==0 || *input==L'\r' || *input==L'\n')
					{
						length=i;
						break;
					}
				}
				if(length!=inputText.Length())
				{
					inputText=inputText.Left(length);
				}
				return true;
			}

			void GuiSinglelineTextBox::TextElementOperatorCallback::AfterModify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)
			{
			}
			
			void GuiSinglelineTextBox::TextElementOperatorCallback::ScrollToView(Point point)
			{
				int newX=point.x;
				int oldX=textElement->GetViewPosition().x;
				int marginX=0;
				if(oldX<newX)
				{
					marginX=TextMargin;
				}
				else if(oldX>newX)
				{
					marginX=-TextMargin;
				}

				newX+=marginX;
				int minX=-TextMargin;
				int maxX=textElement->GetLines().GetMaxWidth()+TextMargin-textComposition->GetBounds().Width();
				if(newX>=maxX)
				{
					newX=maxX-1;
				}
				if(newX<minX)
				{
					newX=minX;
				}
				textElement->SetViewPosition(Point(newX, -TextMargin));
			}

			int GuiSinglelineTextBox::TextElementOperatorCallback::GetTextMargin()
			{
				return TextMargin;
			}

/***********************************************************************
GuiSinglelineTextBox
***********************************************************************/

			void GuiSinglelineTextBox::OnRenderTargetChanged(elements::IGuiGraphicsRenderTarget* renderTarget)
			{
				styleController->RearrangeTextElement();
				GuiControl::OnRenderTargetChanged(renderTarget);
			}

			void GuiSinglelineTextBox::OnBoundsMouseButtonDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
			{
				if(GetVisuallyEnabled())
				{
					boundsComposition->GetRelatedGraphicsHost()->SetFocus(boundsComposition);
				}
			}

			GuiSinglelineTextBox::GuiSinglelineTextBox(GuiSinglelineTextBox::IStyleProvider* styleProvider)
				:GuiControl(new StyleController(styleProvider))
			{
				styleController=dynamic_cast<StyleController*>(GetStyleController());
				styleController->SetTextBox(this);

				boundsComposition->GetEventReceiver()->leftButtonDown.AttachMethod(this, &GuiSinglelineTextBox::OnBoundsMouseButtonDown);
				boundsComposition->GetEventReceiver()->middleButtonDown.AttachMethod(this, &GuiSinglelineTextBox::OnBoundsMouseButtonDown);
				boundsComposition->GetEventReceiver()->rightButtonDown.AttachMethod(this, &GuiSinglelineTextBox::OnBoundsMouseButtonDown);
				SetFocusableComposition(boundsComposition);

				InitializeCommonInterface(this, styleController->GetTextElementOperator());
			}

			GuiSinglelineTextBox::~GuiSinglelineTextBox()
			{
			}

			const WString& GuiSinglelineTextBox::GetText()
			{
				text=styleController->GetText();
				return text;
			}

			void GuiSinglelineTextBox::SetText(const WString& value)
			{
				GuiControl::SetText(value);
			}

			void GuiSinglelineTextBox::SetFont(const FontProperties& value)
			{
				GuiControl::SetFont(value);
				styleController->RearrangeTextElement();
			}
		}
	}
}