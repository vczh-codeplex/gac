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
		int								paragraphIndex;
		GuiDocumentElement*				documentElement;
	public:
		GifAnimation(Ptr<text::DocumentImageRun> _imageRun, int _paragraphIndex, GuiDocumentElement* _documentElement)
			:imageRun(_imageRun)
			,paragraphIndex(_paragraphIndex)
			,documentElement(_documentElement)
			,startTime(DateTime::LocalTime().totalMilliseconds)
		{
		}

		int GetTotalLength()
		{
			return 1;
		}

		int GetCurrentPosition()
		{
			return 0;
		}

		void Play(int currentPosition, int totalLength)
		{
			unsigned __int64 ms=DateTime::LocalTime().totalMilliseconds-startTime;
			int frameIndex=(ms/100)%imageRun->image->GetFrameCount();
			imageRun->frameIndex=frameIndex;
			documentElement->NotifyParagraphUpdated(paragraphIndex);
		}

		void Stop()
		{
		}
	};
}
using namespace document;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsDirect2DRenderer();
}

class GuiRichTextWindow : public GuiWindow
{
protected:
	GuiScrollContainer*				scriptDocumentView;
public:
	GuiRichTextWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		this->SetText(L"Rendering.TextLayout.RichText");
		this->SetClientSize(Size(640, 480));
		this->MoveToScreenCenter();
		this->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	
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
					FileStream fileStream(L"..\\Resources\\document2.xml", FileStream::ReadOnly);
					BomDecoder decoder;
					DecoderStream decoderStream(fileStream, decoder);
					StreamReader writer(decoderStream);
					text=writer.ReadToEnd();
				}
				auto table=XmlLoadTable();
				Ptr<XmlDocument> xml=XmlParseDocument(text, table);
				document=DocumentModel::LoadFromXml(xml, L"..\\Resources\\");
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
						this->GetGraphicsHost()->GetAnimationManager()->AddAnimation(gifAnimation);
					}
				}
			});
		});

		this->GetContainerComposition()->AddChild(scriptDocumentView->GetBoundsComposition());
	}
};

void GuiMain()
{
	GuiRichTextWindow window;
	GetApplication()->Run(&window);
}