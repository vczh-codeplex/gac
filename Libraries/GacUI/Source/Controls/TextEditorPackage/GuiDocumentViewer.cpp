#include "GuiDocumentViewer.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace collections;
			using namespace elements;
			using namespace compositions;

/***********************************************************************
GuiDocumentViewer
***********************************************************************/

			void GuiDocumentCommonInterface::Move(TextPos caret, bool shift, bool frontSide)
			{
				TextPos begin=documentElement->GetCaretBegin();
				TextPos end=documentElement->GetCaretEnd();
				
				TextPos newBegin=shift?begin:caret;
				TextPos newEnd=caret;
				documentElement->SetCaret(newBegin, newEnd, frontSide);
				documentElement->SetCaretVisible(true);

				Rect bounds=documentElement->GetCaretBounds(newEnd, frontSide);
				if(bounds!=Rect())
				{
					bounds.x1-=15;
					bounds.y1-=15;
					bounds.x2+=15;
					bounds.y2+=15;
					EnsureRectVisible(bounds);
				}
			}

			bool GuiDocumentCommonInterface::ProcessKey(vint code, bool shift, bool ctrl)
			{
				TextPos currentCaret=documentElement->GetCaretEnd();
				bool frontSide=documentElement->IsCaretEndPreferFrontSide();
				TextPos begin=documentElement->GetCaretBegin();
				TextPos end=documentElement->GetCaretEnd();

				switch(code)
				{
				case VKEY_UP:
					{
						TextPos newCaret=documentElement->CalculateCaret(currentCaret, IGuiGraphicsParagraph::CaretMoveUp, frontSide);
						Move(newCaret, shift, frontSide);
					}
					break;
				case VKEY_DOWN:
					{
						TextPos newCaret=documentElement->CalculateCaret(currentCaret, IGuiGraphicsParagraph::CaretMoveDown, frontSide);
						Move(newCaret, shift, frontSide);
					}
					break;
				case VKEY_LEFT:
					{
						TextPos newCaret=documentElement->CalculateCaret(currentCaret, IGuiGraphicsParagraph::CaretMoveLeft, frontSide);
						Move(newCaret, shift, frontSide);
					}
					break;
				case VKEY_RIGHT:
					{
						TextPos newCaret=documentElement->CalculateCaret(currentCaret, IGuiGraphicsParagraph::CaretMoveRight, frontSide);
						Move(newCaret, shift, frontSide);
					}
					break;
				case VKEY_HOME:
					{
						TextPos newCaret=documentElement->CalculateCaret(currentCaret, IGuiGraphicsParagraph::CaretLineFirst, frontSide);
						if(newCaret==currentCaret)
						{
							newCaret=documentElement->CalculateCaret(currentCaret, IGuiGraphicsParagraph::CaretFirst, frontSide);
						}
						Move(newCaret, shift, frontSide);
					}
					break;
				case VKEY_END:
					{
						TextPos newCaret=documentElement->CalculateCaret(currentCaret, IGuiGraphicsParagraph::CaretLineLast, frontSide);
						if(newCaret==currentCaret)
						{
							newCaret=documentElement->CalculateCaret(currentCaret, IGuiGraphicsParagraph::CaretLast, frontSide);
						}
						Move(newCaret, shift, frontSide);
					}
					break;
				case VKEY_PRIOR:
					{
					}
					break;
				case VKEY_NEXT:
					{
					}
					break;
				case VKEY_BACK:
					if(editMode==Editable)
					{
						if(begin==end)
						{
							ProcessKey(VKEY_LEFT, true, false);
						}
						Array<WString> text;
						EditText(documentElement->GetCaretBegin(), documentElement->GetCaretEnd(), documentElement->IsCaretEndPreferFrontSide(), text);
						return true;
					}
					break;
				case VKEY_DELETE:
					if(editMode==Editable)
					{
						if(begin==end)
						{
							ProcessKey(VKEY_RIGHT, true, false);
						}
						Array<WString> text;
						EditText(documentElement->GetCaretBegin(), documentElement->GetCaretEnd(), documentElement->IsCaretEndPreferFrontSide(), text);
						return true;
					}
					break;
				case VKEY_RETURN:
					if(editMode==Editable)
					{
						if(ctrl)
						{
							Array<WString> text(1);
							text[0]=L"\r\n";
							EditText(documentElement->GetCaretBegin(), documentElement->GetCaretEnd(), documentElement->IsCaretEndPreferFrontSide(), text);
						}
						else
						{
							Array<WString> text(2);
							EditText(documentElement->GetCaretBegin(), documentElement->GetCaretEnd(), documentElement->IsCaretEndPreferFrontSide(), text);
						}
						return true;
					}
					break;
				}
				return false;
			}

			void GuiDocumentCommonInterface::InstallDocumentViewer(GuiControl* _sender, compositions::GuiGraphicsComposition* _container)
			{
				senderControl=_sender;

				documentElement=GuiDocumentElement::Create();
				documentComposition=new GuiBoundsComposition;
				documentComposition->SetOwnedElement(documentElement);
				documentComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
				documentComposition->SetAlignmentToParent(Margin(5, 5, 5, 5));
				_container->AddChild(documentComposition);

				documentComposition->GetEventReceiver()->mouseMove.AttachMethod(this, &GuiDocumentCommonInterface::OnMouseMove);
				documentComposition->GetEventReceiver()->leftButtonDown.AttachMethod(this, &GuiDocumentCommonInterface::OnMouseDown);
				documentComposition->GetEventReceiver()->leftButtonUp.AttachMethod(this, &GuiDocumentCommonInterface::OnMouseUp);
				documentComposition->GetEventReceiver()->mouseLeave.AttachMethod(this, &GuiDocumentCommonInterface::OnMouseLeave);

				_sender->GetFocusableComposition()->GetEventReceiver()->caretNotify.AttachMethod(this, &GuiDocumentCommonInterface::OnCaretNotify);
				_sender->GetFocusableComposition()->GetEventReceiver()->gotFocus.AttachMethod(this, &GuiDocumentCommonInterface::OnGotFocus);
				_sender->GetFocusableComposition()->GetEventReceiver()->lostFocus.AttachMethod(this, &GuiDocumentCommonInterface::OnLostFocus);
				_sender->GetFocusableComposition()->GetEventReceiver()->keyDown.AttachMethod(this, &GuiDocumentCommonInterface::OnKeyDown);
				_sender->GetFocusableComposition()->GetEventReceiver()->charInput.AttachMethod(this, &GuiDocumentCommonInterface::OnCharInput);

				ActiveHyperlinkChanged.SetAssociatedComposition(_sender->GetBoundsComposition());
				ActiveHyperlinkExecuted.SetAssociatedComposition(_sender->GetBoundsComposition());
			}

			void GuiDocumentCommonInterface::SetActiveHyperlinkId(vint value)
			{
				if(activeHyperlinkId!=value)
				{
					documentElement->ActivateHyperlink(activeHyperlinkId, false);
					activeHyperlinkId=value;
					documentElement->ActivateHyperlink(activeHyperlinkId, true);
					ActiveHyperlinkChanged.Execute(senderControl->GetNotifyEventArguments());
				}
			}

			void GuiDocumentCommonInterface::OnCaretNotify(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				if(senderControl->GetVisuallyEnabled())
				{
					if(editMode!=ViewOnly)
					{
						documentElement->SetCaretVisible(!documentElement->GetCaretVisible());
					}
				}
			}

			void GuiDocumentCommonInterface::OnGotFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				if(senderControl->GetVisuallyEnabled())
				{
					if(editMode!=ViewOnly)
					{
						documentElement->SetCaretVisible(true);
					}
				}
			}

			void GuiDocumentCommonInterface::OnLostFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				if(senderControl->GetVisuallyEnabled())
				{
					documentElement->SetCaretVisible(false);
				}
			}

			void GuiDocumentCommonInterface::OnKeyDown(compositions::GuiGraphicsComposition* sender, compositions::GuiKeyEventArgs& arguments)
			{
				if(senderControl->GetVisuallyEnabled())
				{
					if(editMode!=ViewOnly)
					{
						if(ProcessKey(arguments.code, arguments.shift, arguments.ctrl))
						{
							arguments.handled=true;
						}
					}
				}
			}

			void GuiDocumentCommonInterface::OnCharInput(compositions::GuiGraphicsComposition* sender, compositions::GuiCharEventArgs& arguments)
			{
				if(senderControl->GetVisuallyEnabled())
				{
					if(editMode==Editable && arguments.code!=VKEY_ESCAPE && arguments.code!=VKEY_BACK && arguments.code!=VKEY_RETURN && !arguments.ctrl)
					{
						Array<WString> text(1);
						text[0]=WString(arguments.code);
						EditText(documentElement->GetCaretBegin(), documentElement->GetCaretEnd(), documentElement->IsCaretEndPreferFrontSide(), text);
					}
				}
			}

			void GuiDocumentCommonInterface::OnMouseMove(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
			{
				if(senderControl->GetVisuallyEnabled())
				{
					switch(editMode)
					{
					case ViewOnly:
						{
							vint id=documentElement->GetHyperlinkIdFromPoint(Point(arguments.x, arguments.y));
							if(dragging && id!=draggingHyperlinkId)
							{
								id=DocumentRun::NullHyperlinkId;
							}
							SetActiveHyperlinkId(id);
							if(id==DocumentRun::NullHyperlinkId)
							{
								documentComposition->SetAssociatedCursor(0);
							}
							else
							{
								INativeCursor* cursor=GetCurrentController()->ResourceService()->GetSystemCursor(INativeCursor::Hand);
								documentComposition->SetAssociatedCursor(cursor);
							}
						}
						break;
					case Selectable:
					case Editable:
						if(dragging)
						{
							TextPos caret=documentElement->CalculateCaretFromPoint(Point(arguments.x, arguments.y));
							TextPos oldCaret=documentElement->GetCaretBegin();
							Move(caret, true, (oldCaret==caret?documentElement->IsCaretEndPreferFrontSide():caret<oldCaret));
						}
						break;
					}
				}
			}

			void GuiDocumentCommonInterface::OnMouseDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
			{
				if(senderControl->GetVisuallyEnabled())
				{
					senderControl->SetFocus();
					switch(editMode)
					{
					case ViewOnly:
						{
							vint id=documentElement->GetHyperlinkIdFromPoint(Point(arguments.x, arguments.y));
							draggingHyperlinkId=id;
							SetActiveHyperlinkId(id);
						}
						break;
					case Selectable:
					case Editable:
						{
							TextPos caret=documentElement->CalculateCaretFromPoint(Point(arguments.x, arguments.y));
							TextPos oldCaret=documentElement->GetCaretEnd();
							if(caret!=oldCaret)
							{
								Move(caret, false, caret<oldCaret);
							}
						}
						break;
					}
					dragging=true;
				}
			}

			void GuiDocumentCommonInterface::OnMouseUp(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
			{
				if(senderControl->GetVisuallyEnabled())
				{
					dragging=false;
					switch(editMode)
					{
					case ViewOnly:
						{
							vint id=documentElement->GetHyperlinkIdFromPoint(Point(arguments.x, arguments.y));
							if(id==draggingHyperlinkId && id!=DocumentRun::NullHyperlinkId)
							{
								ActiveHyperlinkExecuted.Execute(senderControl->GetNotifyEventArguments());
							}
							draggingHyperlinkId=DocumentRun::NullHyperlinkId;
						}
						break;
					}
				}
			}

			void GuiDocumentCommonInterface::OnMouseLeave(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				SetActiveHyperlinkId(DocumentRun::NullHyperlinkId);
			}

			void GuiDocumentCommonInterface::EnsureRectVisible(Rect bounds)
			{
			}

			GuiDocumentCommonInterface::GuiDocumentCommonInterface()
				:documentElement(0)
				,documentComposition(0)
				,activeHyperlinkId(DocumentRun::NullHyperlinkId)
				,draggingHyperlinkId(DocumentRun::NullHyperlinkId)
				,dragging(false)
				,editMode(ViewOnly)
				,senderControl(0)
			{
			}

			GuiDocumentCommonInterface::~GuiDocumentCommonInterface()
			{
			}

			Ptr<DocumentModel> GuiDocumentCommonInterface::GetDocument()
			{
				return documentElement->GetDocument();
			}

			void GuiDocumentCommonInterface::SetDocument(Ptr<DocumentModel> value)
			{
				SetActiveHyperlinkId(DocumentRun::NullHyperlinkId);
				documentElement->SetDocument(value);
			}

			void GuiDocumentCommonInterface::NotifyParagraphUpdated(vint index, vint oldCount, vint newCount, bool updatedText)
			{
				documentElement->NotifyParagraphUpdated(index, oldCount, newCount, updatedText);
			}

			void GuiDocumentCommonInterface::EditText(TextPos begin, TextPos end, bool frontSide, const collections::Array<WString>& text)
			{
				documentElement->EditText(begin, end, frontSide, text);

				if(begin>end)
				{
					TextPos temp=begin;
					begin=end;
					end=temp;
				}

				TextPos caret;
				if(text.Count()==0)
				{
					caret=begin;
				}
				else if(text.Count()==1)
				{
					caret=TextPos(begin.row, begin.column+text[0].Length());
					frontSide=true;
				}
				else
				{
					caret=TextPos(begin.row+text.Count()-1, text[text.Count()-1].Length());
					frontSide=true;
				}
				documentElement->SetCaret(caret, caret, frontSide);
			}

			void GuiDocumentCommonInterface::EditStyle(TextPos begin, TextPos end, Ptr<DocumentStyleProperties> style)
			{
				documentElement->EditStyle(begin, end, style);
			}

			vint GuiDocumentCommonInterface::GetActiveHyperlinkId()
			{
				return activeHyperlinkId;
			}

			WString GuiDocumentCommonInterface::GetActiveHyperlinkReference()
			{
				if(activeHyperlinkId==DocumentRun::NullHyperlinkId) return L"";
				Ptr<DocumentModel> document=documentElement->GetDocument();
				if(!document) return L"";
				vint index=document->hyperlinkInfos.Keys().IndexOf(activeHyperlinkId);
				if(index==-1) return L"";
				return document->hyperlinkInfos.Values().Get(index).reference;
			}

			GuiDocumentCommonInterface::EditMode GuiDocumentCommonInterface::GetEditMode()
			{
				return editMode;
			}

			void GuiDocumentCommonInterface::SetEditMode(EditMode value)
			{
				editMode=value;
				if(editMode==ViewOnly)
				{
					documentComposition->SetAssociatedCursor(0);
				}
				else
				{
					INativeCursor* cursor=GetCurrentController()->ResourceService()->GetSystemCursor(INativeCursor::IBeam);
					documentComposition->SetAssociatedCursor(cursor);
				}
			}

/***********************************************************************
GuiDocumentViewer
***********************************************************************/

			void GuiDocumentViewer::EnsureRectVisible(Rect bounds)
			{
				Rect viewBounds=GetViewBounds();
				vint offset=0;
				if(bounds.y1<viewBounds.y1)
				{
					offset=bounds.y1-viewBounds.y1;
				}
				else if(bounds.y2>viewBounds.y2)
				{
					offset=bounds.y2-viewBounds.y2;
				}

				GetVerticalScroll()->SetPosition(viewBounds.y1+offset);
			}

			GuiDocumentViewer::GuiDocumentViewer(GuiDocumentViewer::IStyleProvider* styleProvider)
				:GuiScrollContainer(styleProvider)
			{
				SetExtendToFullWidth(true);
				SetHorizontalAlwaysVisible(false);
				SetFocusableComposition(GetBoundsComposition());
				InstallDocumentViewer(this, GetContainerComposition());
			}

			GuiDocumentViewer::~GuiDocumentViewer()
			{
			}

/***********************************************************************
GuiDocumentLabel
***********************************************************************/

			GuiDocumentLabel::GuiDocumentLabel(GuiDocumentLabel::IStyleController* styleController)
				:GuiControl(styleController)
			{
				GetContainerComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
				SetFocusableComposition(GetBoundsComposition());
				InstallDocumentViewer(this, GetContainerComposition());
			}

			GuiDocumentLabel::~GuiDocumentLabel()
			{
			}
		}
	}
}