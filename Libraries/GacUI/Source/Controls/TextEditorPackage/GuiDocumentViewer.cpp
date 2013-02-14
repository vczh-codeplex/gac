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

			GuiDocumentViewer::GuiDocumentViewer(GuiDocumentViewer::IStyleProvider* styleProvider)
				:GuiScrollContainer(styleProvider)
			{
				SetExtendToFullWidth(true);
				SetHorizontalAlwaysVisible(false);

				documentElement=GuiDocumentElement::Create();
				GuiBoundsComposition* composition=new GuiBoundsComposition;
				composition->SetOwnedElement(documentElement);
				composition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
				composition->SetAlignmentToParent(Margin(5, 5, 5, 5));
				GetContainerComposition()->AddChild(composition);
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