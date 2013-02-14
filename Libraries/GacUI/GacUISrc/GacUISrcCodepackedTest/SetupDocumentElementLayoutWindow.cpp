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
		Ptr<DocumentImageRun>			imageRun;
		vint							paragraphIndex;
		GuiDocumentViewer*				documentViewer;
	public:
		GifAnimation(Ptr<DocumentImageRun> _imageRun, vint _paragraphIndex, GuiDocumentViewer* _documentViewer)
			:imageRun(_imageRun)
			,paragraphIndex(_paragraphIndex)
			,documentViewer(_documentViewer)
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
			documentViewer->NotifyParagraphUpdated(paragraphIndex);
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
	
	GuiDocumentViewer* documentViewer=g::NewDocumentViewer();
	documentViewer->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
	documentViewer->GetBoundsComposition()->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetSystemCursor(INativeCursor::LargeWaiting));
	
	GetApplication()->InvokeAsync([=]()
	{
		Ptr<DocumentModel> document;
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
			documentViewer->SetDocument(document);
			documentViewer->GetBoundsComposition()->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetDefaultSystemCursor());

			FOREACH_INDEXER(Ptr<DocumentParagraph>, p, i, document->paragraphs)
			FOREACH(Ptr<DocumentLine>, l, p->lines)
			FOREACH(Ptr<DocumentRun>, r, l->runs)
			{
				Ptr<DocumentImageRun> image=r.Cast<DocumentImageRun>();
				if(image && image->image->GetFrameCount()>1)
				{
					Ptr<GifAnimation> gifAnimation=new GifAnimation(image, i, documentViewer);
					controlHost->GetGraphicsHost()->GetAnimationManager()->AddAnimation(gifAnimation);
				}
			}
		});
	});

	container->GetContainerComposition()->AddChild(documentViewer->GetBoundsComposition());
}