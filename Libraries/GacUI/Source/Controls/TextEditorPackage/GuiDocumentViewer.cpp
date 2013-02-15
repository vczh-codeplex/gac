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

			void GuiDocumentViewer::SetActiveHyperlinkId(vint value)
			{
				if(activeHyperlinkId!=value)
				{
					documentElement->ActivateHyperlink(activeHyperlinkId, false);
					activeHyperlinkId=value;
					documentElement->ActivateHyperlink(activeHyperlinkId, true);
					ActiveHyperlinkChanged.Execute(GetNotifyEventArguments());
				}
			}

			void GuiDocumentViewer::OnMouseMove(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
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

			void GuiDocumentViewer::OnMouseDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
			{
				vint id=documentElement->GetHyperlinkIdFromPoint(Point(arguments.x, arguments.y));
				draggingHyperlinkId=id;
				SetActiveHyperlinkId(id);
				dragging=true;
			}

			void GuiDocumentViewer::OnMouseUp(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
			{
				dragging=false;
				vint id=documentElement->GetHyperlinkIdFromPoint(Point(arguments.x, arguments.y));
				if(id==draggingHyperlinkId && id!=DocumentRun::NullHyperlinkId)
				{
					ActiveHyperlinkExecuted.Execute(GetNotifyEventArguments());
				}
				draggingHyperlinkId=DocumentRun::NullHyperlinkId;
			}

			void GuiDocumentViewer::OnMouseLeave(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				SetActiveHyperlinkId(DocumentRun::NullHyperlinkId);
			}

			GuiDocumentViewer::GuiDocumentViewer(GuiDocumentViewer::IStyleProvider* styleProvider)
				:GuiScrollContainer(styleProvider)
				,activeHyperlinkId(DocumentRun::NullHyperlinkId)
				,draggingHyperlinkId(DocumentRun::NullHyperlinkId)
				,dragging(false)
			{
				SetExtendToFullWidth(true);
				SetHorizontalAlwaysVisible(false);

				documentElement=GuiDocumentElement::Create();
				documentComposition=new GuiBoundsComposition;
				documentComposition->SetOwnedElement(documentElement);
				documentComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
				documentComposition->SetAlignmentToParent(Margin(5, 5, 5, 5));
				GetContainerComposition()->AddChild(documentComposition);

				documentComposition->GetEventReceiver()->mouseMove.AttachMethod(this, &GuiDocumentViewer::OnMouseMove);
				documentComposition->GetEventReceiver()->leftButtonDown.AttachMethod(this, &GuiDocumentViewer::OnMouseDown);
				documentComposition->GetEventReceiver()->leftButtonUp.AttachMethod(this, &GuiDocumentViewer::OnMouseUp);
				documentComposition->GetEventReceiver()->mouseLeave.AttachMethod(this, &GuiDocumentViewer::OnMouseLeave);

				ActiveHyperlinkChanged.SetAssociatedComposition(GetBoundsComposition());
				ActiveHyperlinkExecuted.SetAssociatedComposition(GetBoundsComposition());
			}

			GuiDocumentViewer::~GuiDocumentViewer()
			{
			}

			Ptr<DocumentModel> GuiDocumentViewer::GetDocument()
			{
				return documentElement->GetDocument();
			}

			void GuiDocumentViewer::SetDocument(Ptr<DocumentModel> value)
			{
				SetActiveHyperlinkId(DocumentRun::NullHyperlinkId);
				documentElement->SetDocument(value);
			}

			void GuiDocumentViewer::NotifyParagraphUpdated(vint index)
			{
				documentElement->NotifyParagraphUpdated(index);
			}

			vint GuiDocumentViewer::GetActiveHyperlinkId()
			{
				return activeHyperlinkId;
			}

			WString GuiDocumentViewer::GetActiveHyperlinkReference()
			{
				if(activeHyperlinkId==DocumentRun::NullHyperlinkId) return L"";
				Ptr<DocumentModel> document=documentElement->GetDocument();
				if(!document) return L"";
				vint index=document->hyperlinkInfos.Keys().IndexOf(activeHyperlinkId);
				if(index==-1) return L"";
				return document->hyperlinkInfos.Values().Get(index).reference;
			}
		}
	}
}