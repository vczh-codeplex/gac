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

			void GuiDocumentCommonInterface::OnMouseMove(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
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

			void GuiDocumentCommonInterface::OnMouseDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
			{
				vint id=documentElement->GetHyperlinkIdFromPoint(Point(arguments.x, arguments.y));
				draggingHyperlinkId=id;
				SetActiveHyperlinkId(id);
				dragging=true;
			}

			void GuiDocumentCommonInterface::OnMouseUp(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
			{
				dragging=false;
				vint id=documentElement->GetHyperlinkIdFromPoint(Point(arguments.x, arguments.y));
				if(id==draggingHyperlinkId && id!=DocumentRun::NullHyperlinkId)
				{
					ActiveHyperlinkExecuted.Execute(senderControl->GetNotifyEventArguments());
				}
				draggingHyperlinkId=DocumentRun::NullHyperlinkId;
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

/***********************************************************************
GuiDocumentViewer
***********************************************************************/

			GuiDocumentViewer::GuiDocumentViewer(GuiDocumentViewer::IStyleProvider* styleProvider)
				:GuiScrollContainer(styleProvider)
			{
				SetExtendToFullWidth(true);
				SetHorizontalAlwaysVisible(false);
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
				InstallDocumentViewer(this, GetContainerComposition());
			}

			GuiDocumentLabel::~GuiDocumentLabel()
			{
			}
		}
	}
}