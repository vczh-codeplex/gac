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

			void GuiDocumentViewer::OnMouseMove(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
			{
				vint id=documentElement->GetHyperlinkIdFromPoint(Point(arguments.x, arguments.y));
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

			GuiDocumentViewer::GuiDocumentViewer(GuiDocumentViewer::IStyleProvider* styleProvider)
				:GuiScrollContainer(styleProvider)
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
				documentElement->SetDocument(value);
			}

			void GuiDocumentViewer::NotifyParagraphUpdated(vint index)
			{
				documentElement->NotifyParagraphUpdated(index);
			}
		}
	}
}