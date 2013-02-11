#include "..\..\Public\Source\GacUI.h"

using namespace vl::stream;
using namespace vl::regex;
using namespace vl::collections;
using namespace vl::parsing::xml;

namespace document
{
	using namespace vl::presentation::elements::text;

	class SerializeRunVisitor : public Object, public DocumentRun::IVisitor
	{
	protected:

		const XmlElementWriter& Text(DocumentTextRun* run, const XmlElementWriter& writer)
		{
			return writer.Text(run->text);
		}

		const XmlElementWriter& Antialias(DocumentTextRun* run, const XmlElementWriter& writer)
		{
			if(!run->style.antialias)
			{
				return Text(run, writer.Element(L"na")).End();
			}
			else if(!run->style.verticalAntialias)
			{
				return Text(run, writer);
			}
			else
			{
				return Text(run, writer.Element(L"va")).End();
			}
		}

		const XmlElementWriter& Strikeline(DocumentTextRun* run, const XmlElementWriter& writer)
		{
			if(run->style.strikeline)
			{
				return Antialias(run, writer.Element(L"s")).End();
			}
			else
			{
				return Antialias(run, writer);
			}
		}

		const XmlElementWriter& Underline(DocumentTextRun* run, const XmlElementWriter& writer)
		{
			if(run->style.underline)
			{
				return Strikeline(run, writer.Element(L"u")).End();
			}
			else
			{
				return Strikeline(run, writer);
			}
		}

		const XmlElementWriter& Italic(DocumentTextRun* run, const XmlElementWriter& writer)
		{
			if(run->style.italic)
			{
				return Underline(run, writer.Element(L"i")).End();
			}
			else
			{
				return Underline(run, writer);
			}
		}

		const XmlElementWriter& Bold(DocumentTextRun* run, const XmlElementWriter& writer)
		{
			if(run->style.bold)
			{
				return Italic(run, writer.Element(L"b")).End();
			}
			else
			{
				return Italic(run, writer);
			}
		}

		const XmlElementWriter& Font(DocumentTextRun* run, const XmlElementWriter& writer)
		{
			const XmlElementWriter& font=writer.Element(L"font");
			font.Attribute(L"face", run->style.fontFamily);
			font.Attribute(L"size", itow(run->style.size));
			font.Attribute(L"color", run->color.ToString());
			Bold(run, font);
			return writer;
		}
	public:
		Ptr<XmlElement> container;

		void Visit(DocumentTextRun* run)
		{
			if(run->text!=L"")
			{
				XmlElementWriter writer(container);
				Font(run, writer);
			}
		}

		void Visit(DocumentImageRun* run)
		{
			XmlElementWriter writer(container);
			writer
				.Attribute(L"width", itow(run->size.x))
				.Attribute(L"height", itow(run->size.y))
				.Attribute(L"baseline", itow(run->baseline))
				.Attribute(L"frameIndex", itow(run->frameIndex))
				.Attribute(L"source", run->source)
				;
		}
	};

	Ptr<XmlDocument> Serialize(Ptr<DocumentModel> model)
	{
		SerializeRunVisitor visitor;

		Ptr<XmlDocument> xml=new XmlDocument;
		Ptr<XmlElement> doc=new XmlElement;
		doc->name.value=L"Doc";
		xml->rootElement=doc;
		{
			Ptr<XmlElement> content=new XmlElement;
			content->name.value=L"Content";
			doc->subNodes.Add(content);

			FOREACH(Ptr<DocumentParagraph>, p, model->paragraphs)
			{
				Ptr<XmlElement> paragraph=new XmlElement;
				paragraph->name.value=L"p";
				content->subNodes.Add(paragraph);

				FOREACH(Ptr<DocumentLine>, l, p->lines)
				{
					FOREACH(Ptr<DocumentRun>, r, l->runs)
					{
						visitor.container=paragraph;
						r->Accept(&visitor);
					}
					{
						Ptr<XmlElement> line=new XmlElement;
						line->name.value=L"br";
						paragraph->subNodes.Add(line);
					}
				}
			}
		}

		return xml;
	}

	int ConvertHex(wchar_t c)
	{
		if(L'a'<=c && c<=L'f') return c-L'a'+10;
		if(L'A'<=c && c<=L'F') return c-L'A'+10;
		if(L'0'<=c && c<=L'9') return c-L'0';
		return 0;
	}

	Color ConvertColor(const WString& colorString)
	{
		return Color(
			ConvertHex(colorString[1])*16+ConvertHex(colorString[2]),
			ConvertHex(colorString[3])*16+ConvertHex(colorString[4]),
			ConvertHex(colorString[5])*16+ConvertHex(colorString[6])
			);
	}

	struct GifRun
	{
		Ptr<text::DocumentImageRun>		imageRun;
		vint							paragraphIndex;
	};
	
	Ptr<text::DocumentModel> BuildDocumentModel(const WString& fileName, List<Ptr<GifRun>>& animations)
	{
		HDC dc=CreateCompatibleDC(NULL);
		int dpi=GetDeviceCaps(dc, LOGPIXELSY);
		DeleteDC(dc);
		Ptr<text::DocumentModel> document=new text::DocumentModel;

		WString rawDocument;
		{
			FileStream fileStream(fileName, FileStream::ReadOnly);
			BomDecoder decoder;
			DecoderStream decoderStream(fileStream, decoder);
			StreamReader reader(decoderStream);
			rawDocument=reader.ReadToEnd();
		}

		WString regexTag_s=L"<(<tag>s)>(<font>[^:]+):(<bold>[^:]+):(<color>[^:]+):(<size>[^:]+):(<text>/.*?)<//s>";
		WString regexTag_i=L"<(<tag>i)>(<cx>[^:]+),(<cy>[^:]+):(<b>[^:]+):(<file>/.*?)<//i>";
		WString regexTag_p=L"<(<tag>p)//>";
		Regex regexTag(regexTag_s+L"|"+regexTag_i+L"|"+regexTag_p);
		Regex regexLine(L"\r\n");
		RegexMatch::List matches;
		regexTag.Search(rawDocument, matches);

		Ptr<text::DocumentParagraph> paragraph=0;
		Ptr<text::DocumentLine> line=0;
		
		for(int i=0;i<matches.Count();i++)
		{
			Ptr<RegexMatch> match=matches[i];
			if(match->Groups()[L"tag"].Get(0).Value()==L"p")
			{
				paragraph=0;
				line=0;
			}
			else if(match->Groups()[L"tag"].Get(0).Value()==L"i")
			{
				vint cx=wtoi(match->Groups()[L"cx"].Get(0).Value());
				vint cy=wtoi(match->Groups()[L"cy"].Get(0).Value());
				vint b=wtoi(match->Groups()[L"b"].Get(0).Value());
				WString file=match->Groups()[L"file"].Get(0).Value();

				if(!paragraph)
				{
					paragraph=new text::DocumentParagraph;
					document->paragraphs.Add(paragraph);
					line=0;
				}
				if(!line)
				{
					line=new text::DocumentLine;
					paragraph->lines.Add(line);
				}

				Ptr<text::DocumentImageRun> run=new text::DocumentImageRun;
				run->size=Size(cx, cy);
				run->baseline=b;
				run->image=GetCurrentController()->ImageService()->CreateImageFromFile(L"Resources\\"+file);
				run->frameIndex=0;
				run->source=L"file://"+file;
				line->runs.Add(run);

				if(run->image->GetFrameCount()>1)
				{
					Ptr<GifRun> gifRun=new GifRun;
					gifRun->imageRun=run;
					gifRun->paragraphIndex=document->paragraphs.Count()-1;
					animations.Add(gifRun);
				}
			}
			else if(match->Groups()[L"tag"].Get(0).Value()==L"s")
			{
				FontProperties fontStyle;
				Color fontColor;
				RegexMatch::List lines;
				{
					WString font=match->Groups()[L"font"].Get(0).Value();
					WString bold=match->Groups()[L"bold"].Get(0).Value();
					WString color=match->Groups()[L"color"].Get(0).Value();
					WString size=match->Groups()[L"size"].Get(0).Value();
					WString text=match->Groups()[L"text"].Get(0).Value();

					fontStyle.fontFamily=font;
					fontStyle.bold=bold==L"true";
					fontStyle.size=(int)(wtof(size)*dpi/72);
					fontColor=ConvertColor(color);
					regexLine.Split(text, true, lines);
				}

				for(int j=0;j<lines.Count();j++)
				{
					WString lineText=lines[j]->Result().Value();
					if(!paragraph)
					{
						paragraph=new text::DocumentParagraph;
						document->paragraphs.Add(paragraph);
						line=0;
					}
					if(!line || j>0)
					{
						line=new text::DocumentLine;
						paragraph->lines.Add(line);
					}

					Ptr<text::DocumentTextRun> run=new text::DocumentTextRun;
					run->style=fontStyle;
					run->color=fontColor;
					run->text=lineText;
					line->runs.Add(run);
				}
			}
		}

		return document;
	}

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
		List<Ptr<GifRun>> animations;
		Ptr<text::DocumentModel> document=BuildDocumentModel(filename, animations);
		{
			Ptr<XmlDocument> xml=Serialize(document);
			FileStream fileStream(filename+L".xml", FileStream::WriteOnly);
			BomEncoder encoder(BomEncoder::Utf8);
			EncoderStream encoderStream(fileStream, encoder);
			StreamWriter writer(encoderStream);
			XmlPrint(xml, writer);
		}
		GetApplication()->InvokeInMainThreadAndWait([=, &animations]()
		{
			scriptDocumentView->GetBoundsComposition()->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetDefaultSystemCursor());
			GuiDocumentElement* element=GuiDocumentElement::Create();
			element->SetDocument(document);

			GuiBoundsComposition* composition=new GuiBoundsComposition;
			composition->SetOwnedElement(element);
			composition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
			composition->SetAlignmentToParent(Margin(10, 10, 10, 10));
			scriptDocumentView->GetContainerComposition()->AddChild(composition);

			for(int i=0;i<animations.Count();i++)
			{
				Ptr<GifAnimation> gifAnimation=new GifAnimation(animations[i]->imageRun, animations[i]->paragraphIndex, element);
				controlHost->GetGraphicsHost()->GetAnimationManager()->AddAnimation(gifAnimation);
			}
		});
	});

	container->GetContainerComposition()->AddChild(scriptDocumentView->GetBoundsComposition());
}