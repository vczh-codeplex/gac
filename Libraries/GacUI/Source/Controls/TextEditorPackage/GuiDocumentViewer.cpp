#include "GuiDocumentViewer.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace elements;
			using namespace compositions;

/***********************************************************************
GuiDocumentViewer
***********************************************************************/

			bool GuiDocumentCommonInterface::ProcessKey(vint code, bool shift, bool ctrl)
			{
				switch(code)
				{
				case VKEY_UP:
					{
					}
					break;
				case VKEY_DOWN:
					{
					}
					break;
				case VKEY_LEFT:
					{
					}
					break;
				case VKEY_RIGHT:
					{
					}
					break;
				case VKEY_HOME:
					{
					}
					break;
				case VKEY_END:
					{
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
					if(ProcessKey(arguments.code, arguments.shift, arguments.ctrl))
					{
						arguments.handled=true;
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

			void GuiDocumentCommonInterface::NotifyParagraphUpdated(vint index)
			{
				documentElement->NotifyParagraphUpdated(index);
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