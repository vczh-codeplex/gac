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

		void Visit(DocumentTextRun* run)override
		{
			if(run->text!=L"")
			{
				XmlElementWriter writer(container);
				Font(run, writer);
			}
		}

		void Visit(DocumentImageRun* run)override
		{
			XmlElementWriter writer(container);
			writer
				.Element(L"img")
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

	class DeserializeNodeVisitor : public XmlNode::IVisitor
	{
	public:
		List<Pair<FontProperties, Color>>	styleStack;
		Ptr<DocumentParagraph>				paragraph;
		Ptr<DocumentLine>					line;
		WString								workingDirectory;

		DeserializeNodeVisitor(Ptr<DocumentParagraph> _paragraph, const WString& _workingDirectory)
			:paragraph(_paragraph)
			,workingDirectory(_workingDirectory)
		{
			styleStack.Add(Pair<FontProperties, Color>(GetCurrentController()->ResourceService()->GetDefaultFont(), Color()));
		}

		void PrintText(const WString& text)
		{
			if(!line)
			{
				line=new DocumentLine;
				paragraph->lines.Add(line);
			}
			Ptr<DocumentTextRun> run=new DocumentTextRun;
			run->style=styleStack[styleStack.Count()-1].key;
			run->color=styleStack[styleStack.Count()-1].value;
			run->text=text;
			line->runs.Add(run);
		}

		void Visit(XmlText* node)override
		{
			PrintText(node->content.value);
		}

		void Visit(XmlCData* node)override
		{
			PrintText(node->content.value);
		}

		void Visit(XmlAttribute* node)override
		{
		}

		void Visit(XmlComment* node)override
		{
		}

		void Visit(XmlElement* node)override
		{
			if(node->name.value==L"br")
			{
				if(!line) PrintText(L"");
				line=0;
			}
			else if(node->name.value==L"img")
			{
				if(!line)
				{
					line=new DocumentLine;
					paragraph->lines.Add(line);
				}
				Ptr<DocumentImageRun> run=new DocumentImageRun;
				FOREACH(Ptr<XmlAttribute>, att, node->attributes)
				{
					if(att->name.value==L"width")
					{
						run->size.x=wtoi(att->value.value);
					}
					else if(att->name.value==L"height")
					{
						run->size.y=wtoi(att->value.value);
					}
					else if(att->name.value==L"baseline")
					{
						run->baseline=wtoi(att->value.value);
					}
					else if(att->name.value==L"frameIndex")
					{
						run->frameIndex=wtoi(att->value.value);
					}
					else if(att->name.value==L"source")
					{
						run->source=att->value.value;
						if(Locale::Invariant().StartsWith(run->source, L"file://", Locale::IgnoreCase))
						{
							WString filename=run->source.Sub(7, run->source.Length()-7);
							if(filename.Length()>=2 && filename[1]!=L':')
							{
								filename=workingDirectory+filename;
							}
							run->image=GetCurrentController()->ImageService()->CreateImageFromFile(filename);
						}
					}
				}
				line->runs.Add(run);
			}
			else if(node->name.value==L"font")
			{
				auto style=styleStack[styleStack.Count()-1];
				FOREACH(Ptr<XmlAttribute>, att, node->attributes)
				{
					if(att->name.value==L"face")
					{
						style.key.fontFamily=att->value.value;
					}
					else if(att->name.value==L"size")
					{
						style.key.size=wtoi(att->value.value);
					}
					else if(att->name.value==L"color")
					{
						style.value=Color::Parse(att->value.value);
					}
				}
				styleStack.Add(style);
				FOREACH(Ptr<XmlNode>, sub, node->subNodes)
				{
					sub->Accept(this);
				}
				styleStack.RemoveAt(styleStack.Count()-1);
			}
			else if(node->name.value==L"b")
			{
				auto style=styleStack[styleStack.Count()-1];
				style.key.bold=true;
				styleStack.Add(style);
				FOREACH(Ptr<XmlNode>, sub, node->subNodes)
				{
					sub->Accept(this);
				}
				styleStack.RemoveAt(styleStack.Count()-1);
			}
			else if(node->name.value==L"i")
			{
				auto style=styleStack[styleStack.Count()-1];
				style.key.italic=true;
				styleStack.Add(style);
				FOREACH(Ptr<XmlNode>, sub, node->subNodes)
				{
					sub->Accept(this);
				}
				styleStack.RemoveAt(styleStack.Count()-1);
			}
			else if(node->name.value==L"u")
			{
				auto style=styleStack[styleStack.Count()-1];
				style.key.underline=true;
				styleStack.Add(style);
				FOREACH(Ptr<XmlNode>, sub, node->subNodes)
				{
					sub->Accept(this);
				}
				styleStack.RemoveAt(styleStack.Count()-1);
			}
			else if(node->name.value==L"s")
			{
				auto style=styleStack[styleStack.Count()-1];
				style.key.strikeline=true;
				styleStack.Add(style);
				FOREACH(Ptr<XmlNode>, sub, node->subNodes)
				{
					sub->Accept(this);
				}
				styleStack.RemoveAt(styleStack.Count()-1);
			}
			else if(node->name.value==L"va")
			{
				auto style=styleStack[styleStack.Count()-1];
				style.key.antialias=true;
				style.key.verticalAntialias=true;
				styleStack.Add(style);
				FOREACH(Ptr<XmlNode>, sub, node->subNodes)
				{
					sub->Accept(this);
				}
				styleStack.RemoveAt(styleStack.Count()-1);
			}
			else if(node->name.value==L"na")
			{
				auto style=styleStack[styleStack.Count()-1];
				style.key.antialias=false;
				style.key.verticalAntialias=false;
				styleStack.Add(style);
				FOREACH(Ptr<XmlNode>, sub, node->subNodes)
				{
					sub->Accept(this);
				}
				styleStack.RemoveAt(styleStack.Count()-1);
			}
			else
			{
				FOREACH(Ptr<XmlNode>, sub, node->subNodes)
				{
					sub->Accept(this);
				}
			}
		}

		void Visit(XmlInstruction* node)override
		{
		}

		void Visit(XmlDocument* node)override
		{
		}
	};

	Ptr<DocumentModel> Deserialize(Ptr<XmlDocument> xml, const WString& workingDirectory)
	{
		Ptr<DocumentModel> model=new DocumentModel;
		if(xml->rootElement->name.value==L"Doc")
		if(Ptr<XmlElement> content=XmlGetElement(xml->rootElement, L"Content"))
		FOREACH(Ptr<XmlElement>, p, XmlGetElements(content, L"p"))
		{
			Ptr<DocumentParagraph> paragraph=new DocumentParagraph;
			model->paragraphs.Add(paragraph);
			DeserializeNodeVisitor visitor(paragraph, workingDirectory);
			p->Accept(&visitor);
		}
		return model;
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
			document=Deserialize(xml, L"Resources\\");
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