#include "..\..\Public\Source\GacUI.h"

using namespace vl::stream;
using namespace vl::regex;
using namespace vl::collections;
using namespace vl::parsing::xml;
using namespace vl::presentation::elements::text;

namespace document
{
	class GifAnimation : public Object, public IGuiGraphicsAnimation
	{
	protected:
		unsigned __int64				startTime;
		Ptr<text::DocumentImageRun>		imageRun;
		vint							paragraphIndex;
		GuiDocumentElement*				documentElement;
	public:
		GifAnimation(Ptr<text::DocumentImageRun> _imageRun, vint _paragraphIndex, GuiDocumentElement* _documentElement)
			:imageRun(_imageRun)
			,paragraphIndex(_paragraphIndex)
			,documentElement(_documentElement)
			,startTime(DateTime::LocalTime().totalMilliseconds)
		{
		}

		vint GetTotalLength()
		{
			return 1;
		}

		vint GetCurrentPosition()
		{
			return 0;
		}

		void Play(vint currentPosition, vint totalLength)
		{
			unsigned __int64 ms=DateTime::LocalTime().totalMilliseconds-startTime;
			vint frameIndex=(ms/100)%imageRun->image->GetFrameCount();
			imageRun->frameIndex=frameIndex;
			documentElement->NotifyParagraphUpdated(paragraphIndex);
		}

		void Stop()
		{
		}
	};
}
using namespace document;

void SetupDocumentElementLayoutWindow(GuiControlHost* controlHost, GuiControl* container, const WString& filename)
{
	container->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	
	GuiScrollContainer* scriptDocumentView=new GuiScrollContainer(GetCurrentTheme()->CreateMultilineTextBoxStyle());
	scriptDocumentView->SetExtendToFullWidth(true);
	scriptDocumentView->SetHorizontalAlwaysVisible(false);
	scriptDocumentView->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
	scriptDocumentView->GetBoundsComposition()->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetSystemCursor(INativeCursor::LargeWaiting));
	
	GetApplication()->InvokeAsync([=]()
	{
		Ptr<text::DocumentModel> document;
		{
			WString text;
			{
				FileStream fileStream(filename, FileStream::ReadOnly);
				BomDecoder decoder;
				DecoderStream decoderStream(fileStream, decoder);
				StreamReader writer(decoderStream);
				text=writer.ReadToEnd();
			}
			auto table=XmlLoadTable();
			Ptr<XmlDocument> xml=XmlParseDocument(text, table);
			document=DocumentModel::LoadFromXml(xml, L"Resources\\");
		}
		GetApplication()->InvokeInMainThreadAndWait([=]()
		{
			scriptDocumentView->GetBoundsComposition()->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetDefaultSystemCursor());
			GuiDocumentElement* element=GuiDocumentElement::Create();
			element->SetDocument(document);

			GuiBoundsComposition* composition=new GuiBoundsComposition;
			composition->SetOwnedElement(element);
			composition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
			composition->SetAlignmentToParent(Margin(10, 10, 10, 10));
			scriptDocumentView->GetContainerComposition()->AddChild(composition);

			FOREACH_INDEXER(Ptr<DocumentParagraph>, p, i, document->paragraphs)
			FOREACH(Ptr<DocumentLine>, l, p->lines)
			FOREACH(Ptr<DocumentRun>, r, l->runs)
			{
				Ptr<DocumentImageRun> image=r.Cast<DocumentImageRun>();
				if(image && image->image->GetFrameCount()>1)
				{
					Ptr<GifAnimation> gifAnimation=new GifAnimation(image, i, element);
					controlHost->GetGraphicsHost()->GetAnimationManager()->AddAnimation(gifAnimation);
				}
			}
		});
	});

	container->GetContainerComposition()->AddChild(scriptDocumentView->GetBoundsComposition());
}