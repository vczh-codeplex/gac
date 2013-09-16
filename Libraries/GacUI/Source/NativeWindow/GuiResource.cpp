#include "GuiResource.h"
#include "..\..\..\..\Common\Source\Stream\FileStream.h"
#include "..\..\..\..\Common\Source\Stream\Accessor.h"
#include "..\..\..\..\Common\Source\Stream\CharFormat.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;
		using namespace parsing::tabling;
		using namespace parsing::xml;
		using namespace regex;

		WString GetFolderPath(const WString& filePath)
		{
			for(vint i=filePath.Length()-1;i>=0;i--)
			{
				if(filePath[i]==L'\\' || filePath[i]==L'/')
				{
					return filePath.Sub(0, i+1);
				}
			}
			return L"";
		}

		WString GetFileName(const WString& filePath)
		{
			for(vint i=filePath.Length()-1;i>=0;i--)
			{
				if(filePath[i]==L'\\' || filePath[i]==L'/')
				{
					return filePath.Sub(i+1, filePath.Length()-i-1);
				}
			}
			return filePath;
		}

		bool LoadTextFile(const WString& filePath, WString& text)
		{
			stream::FileStream fileStream(filePath, stream::FileStream::ReadOnly);
			return LoadTextFromStream(fileStream, text);
		}

		bool LoadTextFromStream(stream::IStream& stream, WString& text)
		{
			if(stream.IsAvailable())
			{
				stream::BomDecoder decoder;
				stream::DecoderStream decoderStream(stream, decoder);
				stream::StreamReader reader(decoderStream);
				text=reader.ReadToEnd();
				return true;
			}
			return false;
		}

/***********************************************************************
GuiImageData
***********************************************************************/

		GuiImageData::GuiImageData()
			:frameIndex(-1)
		{
		}

		GuiImageData::GuiImageData(Ptr<INativeImage> _image, vint _frameIndex)
			:image(_image)
			,frameIndex(_frameIndex)
		{
		}

		GuiImageData::~GuiImageData()
		{
		}

		Ptr<INativeImage> GuiImageData::GetImage()
		{
			return image;
		}

		vint GuiImageData::GetFrameIndex()
		{
			return frameIndex;
		}

/***********************************************************************
DocumentResolver
***********************************************************************/

		DocumentResolver::DocumentResolver(Ptr<DocumentResolver> _previousResolver)
			:previousResolver(_previousResolver)
		{
		}

		DocumentResolver::~DocumentResolver()
		{
		}

		Ptr<INativeImage> DocumentResolver::ResolveImage(const WString& protocol, const WString& path)
		{
			auto result=ResolveImageInternal(protocol, path);
			if(!result && previousResolver)
			{
				result=previousResolver->ResolveImage(protocol, path);
			}
			return result;
		}

/***********************************************************************
document_serialization_visitors::SerializeRunVisitor
***********************************************************************/

		namespace document_serialization_visitors
		{
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

				void Visit(DocumentHyperlinkTextRun* run)override
				{
					Visit(static_cast<DocumentTextRun*>(run));
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
		}
		using namespace document_serialization_visitors;

/***********************************************************************
document_serialization_visitors::DeserializeNodeVisitor
***********************************************************************/

		namespace document_serialization_visitors
		{
			class DeserializeNodeVisitor : public XmlNode::IVisitor
			{
			public:
				struct TemplateInfo
				{
					Dictionary<WString, WString>	attributes;
					XmlElement*						templateElement;
					XmlElement*						contentElement;

					TemplateInfo()
						:templateElement(0)
						,contentElement(0)
					{
					}
				};

				struct StyleStackItem
				{
					Pair<FontProperties, Color>		normalStyle;
					Pair<FontProperties, Color>		activeStyle;
					vint							hyperlinkId;

					StyleStackItem()
						:hyperlinkId(DocumentRun::NullHyperlinkId)
					{
					}
				};

				List<StyleStackItem>				styleStack;
				Ptr<DocumentModel>					model;
				Ptr<DocumentParagraph>				paragraph;
				vint								paragraphIndex;
				Ptr<DocumentLine>					line;
				Ptr<DocumentResolver>				resolver;
				Ptr<TemplateInfo>					templateInfo;
				Regex								regexAttributeApply;

				DeserializeNodeVisitor(Ptr<DocumentModel> _model, Ptr<DocumentParagraph> _paragraph, vint _paragraphIndex, Ptr<DocumentResolver> _resolver)
					:model(_model)
					,paragraph(_paragraph)
					,paragraphIndex(_paragraphIndex)
					,resolver(_resolver)
					,regexAttributeApply(L"/{@(<value>[^{}]+)/}")
				{
					StyleStackItem item;
					item.normalStyle=Pair<FontProperties, Color>(GetCurrentController()->ResourceService()->GetDefaultFont(), Color());
					item.activeStyle=item.normalStyle;
					styleStack.Add(item);
				}

				WString TranslateAttribute(const WString& value)
				{
					if(templateInfo)
					{
						WString result;
						RegexMatch::List matches;
						regexAttributeApply.Cut(value, false, matches);
						FOREACH(RegexMatch::Ref, match, matches)
						{
							if(match->Success())
							{
								WString name=match->Groups()[L"value"].Get(0).Value();
								vint index=templateInfo->attributes.Keys().IndexOf(name);
								result+=(index==-1?match->Result().Value():templateInfo->attributes.Values().Get(index));
							}
							else
							{
								result+=match->Result().Value();
							}
						}
						return result;
					}
					else
					{
						return value;
					}
				}

				void PrintText(const WString& text)
				{
					if(!line)
					{
						line=new DocumentLine;
						paragraph->lines.Add(line);
					}
					Ptr<DocumentTextRun> run;
					const StyleStackItem& item=styleStack[styleStack.Count()-1];
					if(item.hyperlinkId==DocumentRun::NullHyperlinkId || item.normalStyle==item.activeStyle)
					{
						run=new DocumentTextRun;
					}
					else
					{
						Ptr<DocumentHyperlinkTextRun> hyperlink=new DocumentHyperlinkTextRun;
						hyperlink->normalStyle=item.normalStyle.key;
						hyperlink->normalColor=item.normalStyle.value;
						hyperlink->activeStyle=item.activeStyle.key;
						hyperlink->activeColor=item.activeStyle.value;
						run=hyperlink;
					}
					run->hyperlinkId=item.hyperlinkId;
					run->style=item.normalStyle.key;
					run->color=item.normalStyle.value;
					run->text=text;
					line->runs.Add(run);
				}

				void Visit(XmlText* node)override
				{
					PrintText(TranslateAttribute(node->content.value));
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
					else if(node->name.value==L"sp")
					{
						PrintText(L" ");
					}
					else if(node->name.value==L"tab")
					{
						PrintText(L"\t");
					}
					else if(node->name.value==L"img")
					{
						if(!line)
						{
							line=new DocumentLine;
							paragraph->lines.Add(line);
						}
						Ptr<DocumentImageRun> run=new DocumentImageRun;
						if(Ptr<XmlAttribute> source=XmlGetAttribute(node, L"source"))
						{
							run->hyperlinkId=styleStack[styleStack.Count()-1].hyperlinkId;
							run->source=TranslateAttribute(source->value.value);
							Pair<vint, vint> index=INVLOC.FindFirst(run->source, L"://", Locale::IgnoreCase);
							if(index.key!=-1)
							{
								WString protocol=run->source.Sub(0, index.key);
								WString path=run->source.Sub(index.key+index.value, run->source.Length()-index.key-index.value);
								run->image=resolver->ResolveImage(protocol, path);
								if(run->image && run->image->GetFrameCount()>0)
								{
									run->size=run->image->GetFrame(0)->GetSize();
									run->baseline=run->size.y;
									run->frameIndex=0;
								}
							}

							FOREACH(Ptr<XmlAttribute>, att, node->attributes)
							{
								if(att->name.value==L"width")
								{
									run->size.x=wtoi(TranslateAttribute(att->value.value));
								}
								else if(att->name.value==L"height")
								{
									run->size.y=wtoi(TranslateAttribute(att->value.value));
								}
								else if(att->name.value==L"baseline")
								{
									run->baseline=wtoi(TranslateAttribute(att->value.value));
								}
								else if(att->name.value==L"frameIndex")
								{
									run->frameIndex=wtoi(TranslateAttribute(att->value.value));
								}
							}
							line->runs.Add(run);
						}
					}
					else if(node->name.value==L"font")
					{
						auto style=styleStack[styleStack.Count()-1];
						FOREACH(Ptr<XmlAttribute>, att, node->attributes)
						{
							if(att->name.value==L"face")
							{
								style.normalStyle.key.fontFamily=TranslateAttribute(att->value.value);
								style.activeStyle.key.fontFamily=TranslateAttribute(att->value.value);
							}
							else if(att->name.value==L"size")
							{
								style.normalStyle.key.size=wtoi(TranslateAttribute(att->value.value));
								style.activeStyle.key.size=wtoi(TranslateAttribute(att->value.value));
							}
							else if(att->name.value==L"color")
							{
								style.normalStyle.value=Color::Parse(TranslateAttribute(att->value.value));
								style.activeStyle.value=Color::Parse(TranslateAttribute(att->value.value));
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
						style.normalStyle.key.bold=true;
						style.activeStyle.key.bold=true;
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
						style.normalStyle.key.italic=true;
						style.activeStyle.key.italic=true;
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
						style.normalStyle.key.underline=true;
						style.activeStyle.key.underline=true;
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
						style.normalStyle.key.strikeline=true;
						style.activeStyle.key.strikeline=true;
						styleStack.Add(style);
						FOREACH(Ptr<XmlNode>, sub, node->subNodes)
						{
							sub->Accept(this);
						}
						styleStack.RemoveAt(styleStack.Count()-1);
					}
					else if(node->name.value==L"ha")
					{
						auto style=styleStack[styleStack.Count()-1];
						style.normalStyle.key.antialias=true;
						style.normalStyle.key.verticalAntialias=false;
						style.activeStyle.key.antialias=true;
						style.activeStyle.key.verticalAntialias=false;
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
						style.normalStyle.key.antialias=true;
						style.normalStyle.key.verticalAntialias=true;
						style.activeStyle.key.antialias=true;
						style.activeStyle.key.verticalAntialias=true;
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
						style.normalStyle.key.antialias=false;
						style.normalStyle.key.verticalAntialias=false;
						style.activeStyle.key.antialias=false;
						style.activeStyle.key.verticalAntialias=false;
						styleStack.Add(style);
						FOREACH(Ptr<XmlNode>, sub, node->subNodes)
						{
							sub->Accept(this);
						}
						styleStack.RemoveAt(styleStack.Count()-1);
					}
					else if(node->name.value==L"div")
					{
						auto style=styleStack[styleStack.Count()-1];
						WString styleName;
						if(Ptr<XmlAttribute> att=XmlGetAttribute(node, L"style"))
						{
							styleName=TranslateAttribute(att->value.value);
						}
						style.normalStyle=model->GetStyle(styleName, style.normalStyle);
						style.activeStyle=model->GetStyle(styleName, style.activeStyle);
						styleStack.Add(style);
						FOREACH(Ptr<XmlNode>, sub, node->subNodes)
						{
							sub->Accept(this);
						}
						styleStack.RemoveAt(styleStack.Count()-1);
					}
					else if(node->name.value==L"a")
					{
						auto style=styleStack[styleStack.Count()-1];
						WString normalStyle=L"#NormalLink";
						WString activeStyle=L"#ActiveLink";
						if(Ptr<XmlAttribute> att=XmlGetAttribute(node, L"normal"))
						{
							normalStyle=TranslateAttribute(att->value.value);
						}
						if(Ptr<XmlAttribute> att=XmlGetAttribute(node, L"active"))
						{
							activeStyle=TranslateAttribute(att->value.value);
						}
						style.normalStyle=model->GetStyle(normalStyle, style.normalStyle);
						style.activeStyle=model->GetStyle(activeStyle, style.activeStyle);
						style.hyperlinkId=model->hyperlinkInfos.Count();
						styleStack.Add(style);

						WString href;
						if(Ptr<XmlAttribute> att=XmlGetAttribute(node, L"href"))
						{
							href=TranslateAttribute(att->value.value);
						}
						{
							DocumentModel::HyperlinkInfo info;
							info.paragraphIndex=paragraphIndex;
							info.reference=href;
							model->hyperlinkInfos.Add(style.hyperlinkId, info);
						}
						FOREACH(Ptr<XmlNode>, sub, node->subNodes)
						{
							sub->Accept(this);
						}

						styleStack.RemoveAt(styleStack.Count()-1);
					}
					else if(node->name.value==L"template-content")
					{
						if(templateInfo && templateInfo->contentElement)
						{
							Ptr<TemplateInfo> info=templateInfo;
							templateInfo=0;
							FOREACH(Ptr<XmlNode>, sub, info->contentElement->subNodes)
							{
								sub->Accept(this);
							}
							templateInfo=info;
						}
					}
					else
					{
						vint index=model->templates.Keys().IndexOf(node->name.value);
						if(index==-1)
						{
							FOREACH(Ptr<XmlNode>, sub, node->subNodes)
							{
								sub->Accept(this);
							}
						}
						else
						{
							Ptr<TemplateInfo> newInfo=new TemplateInfo;
							newInfo->templateElement=model->templates.Values().Get(index).Obj();
							newInfo->contentElement=node;
							FOREACH(Ptr<XmlAttribute>, att, newInfo->contentElement->attributes)
							{
								newInfo->attributes.Add(att->name.value, TranslateAttribute(att->value.value));
							}

							Ptr<TemplateInfo> info=templateInfo;
							templateInfo=newInfo;
							FOREACH(Ptr<XmlNode>, sub, newInfo->templateElement->subNodes)
							{
								sub->Accept(this);
							}
							templateInfo=info;
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
		}
		using namespace document_serialization_visitors;

/***********************************************************************
document_serialization_visitors::ActivateHyperlinkVisitor
***********************************************************************/

		namespace document_serialization_visitors
		{
			class ActivateHyperlinkVisitor : public Object, public DocumentRun::IVisitor
			{
			public:
				vint			hyperlinkId;
				bool			active;

				ActivateHyperlinkVisitor(vint _hyperlinkId, bool _active)
					:hyperlinkId(_hyperlinkId)
					,active(_active)
				{
				}

				void Visit(DocumentTextRun* run)override
				{
				}

				void Visit(DocumentHyperlinkTextRun* run)override
				{
					if(run->hyperlinkId==hyperlinkId)
					{
						if(active)
						{
							run->style=run->activeStyle;
							run->color=run->activeColor;
						}
						else
						{
							run->style=run->normalStyle;
							run->color=run->normalColor;
						}
					}
				}

				void Visit(DocumentImageRun* run)override
				{
				}
			};
		}
		using namespace document_serialization_visitors;

/***********************************************************************
DocumentModel
***********************************************************************/

		DocumentModel::DocumentModel()
		{
			{
				FontProperties font=GetCurrentController()->ResourceService()->GetDefaultFont();
				Ptr<DocumentStyle> style=new DocumentStyle;
				style->face=font.fontFamily;
				style->size=font.size;
				style->color=Color();
				style->bold=font.bold;
				style->italic=font.italic;
				style->underline=font.underline;
				style->strikeline=font.strikeline;
				style->antialias=font.antialias;
				style->verticalAntialias=font.verticalAntialias;
				styles.Add(L"#Default", style);
			}
			{
				Ptr<DocumentStyle> style=new DocumentStyle;
				styles.Add(L"#Context", style);
			}
			{
				Ptr<DocumentStyle> style=new DocumentStyle;
				style->parentStyleName=L"#Context";
				style->color=Color(0, 0, 255);
				style->underline=true;
				styles.Add(L"#NormalLink", style);
			}
			{
				Ptr<DocumentStyle> style=new DocumentStyle;
				style->parentStyleName=L"#Context";
				style->color=Color(0, 0, 255);
				style->underline=true;
				styles.Add(L"#ActiveLink", style);
			}
		}

		DocumentModel::RawStylePair DocumentModel::GetStyle(const WString& styleName, const RawStylePair& context)
		{
			DocumentStyle result;
			Ptr<DocumentStyle> currentStyle;
			WString currentName=styleName;
			while(true)
			{
				vint index=styles.Keys().IndexOf(currentName);
				if(index==-1) break;
				currentStyle=styles.Values().Get(index);
				currentName=currentStyle->parentStyleName;

				if(!result.face && currentStyle->face) result.face=currentStyle->face;
				if(!result.size && currentStyle->size) result.size=currentStyle->size;
				if(!result.color && currentStyle->color) result.color=currentStyle->color;
				if(!result.bold && currentStyle->bold) result.bold=currentStyle->bold;
				if(!result.italic && currentStyle->italic) result.italic=currentStyle->italic;
				if(!result.underline && currentStyle->underline) result.underline=currentStyle->underline;
				if(!result.strikeline && currentStyle->strikeline) result.strikeline=currentStyle->strikeline;
				if(!result.antialias && currentStyle->antialias) result.antialias=currentStyle->antialias;
				if(!result.verticalAntialias && currentStyle->verticalAntialias) result.verticalAntialias=currentStyle->verticalAntialias;
			}

			if(!result.face) result.face=context.key.fontFamily;
			if(!result.size) result.size=context.key.size;
			if(!result.color) result.color=context.value;
			if(!result.bold) result.bold=context.key.bold;
			if(!result.italic) result.italic=context.key.italic;
			if(!result.underline) result.underline=context.key.underline;
			if(!result.strikeline) result.strikeline=context.key.strikeline;
			if(!result.antialias) result.antialias=context.key.antialias;
			if(!result.verticalAntialias) result.verticalAntialias=context.key.verticalAntialias;

			FontProperties font;
			font.fontFamily=result.face.Value();
			font.size=result.size.Value();
			font.bold=result.bold.Value();
			font.italic=result.italic.Value();
			font.underline=result.underline.Value();
			font.strikeline=result.strikeline.Value();
			font.antialias=result.antialias.Value();
			font.verticalAntialias=result.verticalAntialias.Value();
			return RawStylePair(font, result.color.Value());
		}

		vint DocumentModel::ActivateHyperlink(vint hyperlinkId, bool active)
		{
			vint index=hyperlinkInfos.Keys().IndexOf(hyperlinkId);
			if(index!=-1)
			{
				vint paragraphIndex=hyperlinkInfos.Values().Get(index).paragraphIndex;
				if(0<=paragraphIndex && paragraphIndex<paragraphs.Count())
				{
					Ptr<DocumentParagraph> paragraph=paragraphs[paragraphIndex];
					ActivateHyperlinkVisitor visitor(hyperlinkId, active);
					FOREACH(Ptr<DocumentLine>, line, paragraph->lines)
					{
						FOREACH(Ptr<DocumentRun>, run, line->runs)
						{
							run->Accept(&visitor);
						}
					}
					return paragraphIndex;
				}
			}
			return  -1;
		}

		Ptr<DocumentModel> DocumentModel::LoadFromXml(Ptr<parsing::xml::XmlDocument> xml, Ptr<DocumentResolver> resolver)
		{
			Ptr<DocumentModel> model=new DocumentModel;
			if(xml->rootElement->name.value==L"Doc")
			{
				if(Ptr<XmlElement> styles=XmlGetElement(xml->rootElement, L"Styles"))
				{
					FOREACH(Ptr<XmlElement>, styleElement, XmlGetElements(styles, L"Style"))
					if(Ptr<XmlAttribute> name=XmlGetAttribute(styleElement, L"name"))
					if(!model->styles.Keys().Contains(name->value.value))
					{
						Ptr<DocumentStyle> style=new DocumentStyle;
						model->styles.Add(name->value.value, style);

						if(Ptr<XmlAttribute> parent=XmlGetAttribute(styleElement, L"parent"))
						{
							style->parentStyleName=parent->value.value;
						}

						FOREACH(Ptr<XmlElement>, att, XmlGetElements(styleElement))
						{
							if(att->name.value==L"face")
							{
								style->face=XmlGetValue(att);
							}
							else if(att->name.value==L"size")
							{
								style->size=wtoi(XmlGetValue(att));
							}
							else if(att->name.value==L"color")
							{
								style->color=Color::Parse(XmlGetValue(att));
							}
							else if(att->name.value==L"b")
							{
								style->bold=XmlGetValue(att)==L"true";
							}
							else if(att->name.value==L"i")
							{
								style->italic=XmlGetValue(att)==L"true";
							}
							else if(att->name.value==L"u")
							{
								style->underline=XmlGetValue(att)==L"true";
							}
							else if(att->name.value==L"s")
							{
								style->strikeline=XmlGetValue(att)==L"true";
							}
							else if(att->name.value==L"antialias")
							{
								WString value=XmlGetValue(att);
								if(value==L"horizontal" || value==L"default")
								{
									style->antialias=true;
									style->verticalAntialias=false;
								}
								else if(value==L"no")
								{
									style->antialias=false;
									style->verticalAntialias=false;
								}
								else if(value==L"vertical")
								{
									style->antialias=true;
									style->verticalAntialias=true;
								}
							}
						}
					}
				}
				if(Ptr<XmlElement> styles=XmlGetElement(xml->rootElement, L"Templates"))
				{
					FOREACH(Ptr<XmlElement>, templateElement, XmlGetElements(styles, L"Template"))
					if(Ptr<XmlAttribute> name=XmlGetAttribute(templateElement, L"name"))
					if(!model->styles.Keys().Contains(name->value.value))
					{
						model->templates.Add(name->value.value, templateElement);
					}
				}
				if(Ptr<XmlElement> content=XmlGetElement(xml->rootElement, L"Content"))
				{
					FOREACH_INDEXER(Ptr<XmlElement>, p, i, XmlGetElements(content, L"p"))
					{
						Ptr<DocumentParagraph> paragraph=new DocumentParagraph;
						if(Ptr<XmlAttribute> att=XmlGetAttribute(p, L"align"))
						{
							if(att->value.value==L"Left")
							{
								paragraph->alignment=Alignment::Left;
							}
							else if(att->value.value==L"Center")
							{
								paragraph->alignment=Alignment::Center;
							}
							else if(att->value.value==L"Right")
							{
								paragraph->alignment=Alignment::Right;
							}
						}
						model->paragraphs.Add(paragraph);
						DeserializeNodeVisitor visitor(model, paragraph, i, resolver);
						p->Accept(&visitor);
					}
				}
			}
			return model;
		}

		Ptr<DocumentModel> DocumentModel::LoadFromXml(Ptr<parsing::xml::XmlDocument> xml, const WString& workingDirectory)
		{
			Ptr<DocumentFileProtocolResolver> resolver=new DocumentFileProtocolResolver(workingDirectory);
			return LoadFromXml(xml, resolver);
		}

		Ptr<parsing::xml::XmlDocument> DocumentModel::SaveToXml(bool persistMetadata)
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

				FOREACH(Ptr<DocumentParagraph>, p, paragraphs)
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
			if(persistMetadata)
			{
				Ptr<XmlElement> stylesElement=new XmlElement;
				stylesElement->name.value=L"Styles";
				doc->subNodes.Add(stylesElement);

				for(vint i=0;i<styles.Count();i++)
				{
					WString name=styles.Keys()[i];
					if(name.Length()>0 && name[0]==L'#') continue;

					Ptr<DocumentStyle> style=styles.Values().Get(i);
					Ptr<XmlElement> styleElement=new XmlElement;
					styleElement->name.value=L"Style";
					stylesElement->subNodes.Add(styleElement);

					XmlElementWriter(styleElement).Attribute(L"name", name);
					if(style->parentStyleName!=L"")
					{
						XmlElementWriter(styleElement).Attribute(L"parent", style->parentStyleName);
					}

					if(style->face) XmlElementWriter(styleElement).Element(L"face").Text(style->face.Value());
					if(style->size) XmlElementWriter(styleElement).Element(L"size").Text(itow(style->size.Value()));
					if(style->color) XmlElementWriter(styleElement).Element(L"color").Text(style->color.Value().ToString());
					if(style->bold) XmlElementWriter(styleElement).Element(L"bold").Text(style->bold.Value()?L"true":L"false");
					if(style->italic) XmlElementWriter(styleElement).Element(L"italic").Text(style->italic.Value()?L"true":L"false");
					if(style->underline) XmlElementWriter(styleElement).Element(L"underline").Text(style->underline.Value()?L"true":L"false");
					if(style->strikeline) XmlElementWriter(styleElement).Element(L"strikeline").Text(style->strikeline.Value()?L"true":L"false");
					if(style->antialias && style->verticalAntialias)
					{
						bool h=style->antialias;
						bool v=style->verticalAntialias;
						if(!h)
						{
							XmlElementWriter(styleElement).Element(L"antialias").Text(L"no");
						}
						else if(!v)
						{
							XmlElementWriter(styleElement).Element(L"antialias").Text(L"default");
						}
						else
						{
							XmlElementWriter(styleElement).Element(L"antialias").Text(L"vertical");
						}
					}
				}
			}
			if(persistMetadata)
			{
				Ptr<XmlElement> templatesElement=new XmlElement;
				templatesElement->name.value=L"Templates";
				doc->subNodes.Add(templatesElement);

				FOREACH(Ptr<XmlElement>, element, templates.Values())
				{
					templatesElement->subNodes.Add(element);
				}
			}
			return xml;
		}

/***********************************************************************
DocumentFileProtocolResolver
***********************************************************************/

		Ptr<INativeImage> DocumentFileProtocolResolver::ResolveImageInternal(const WString& protocol, const WString& path)
		{
			if(INVLOC.ToUpper(protocol)!=L"FILE") return 0;
			WString filename=path;
			if(filename.Length()>=2 && filename[1]!=L':')
			{
				filename=workingDirectory+filename;
			}
			return GetCurrentController()->ImageService()->CreateImageFromFile(filename);
		}

		DocumentFileProtocolResolver::DocumentFileProtocolResolver(const WString& _workingDirectory, Ptr<DocumentResolver> previousResolver)
			:DocumentResolver(previousResolver)
			,workingDirectory(_workingDirectory)
		{
		}

/***********************************************************************
DocumentResProtocolResolver
***********************************************************************/

		Ptr<INativeImage> DocumentResProtocolResolver::ResolveImageInternal(const WString& protocol, const WString& path)
		{
			if(INVLOC.ToUpper(protocol)!=L"RES") return 0;
			Ptr<GuiImageData> image=resource->GetValueByPath(path).Cast<GuiImageData>();
			if(image)
			{
				return image->GetImage();
			}
			else
			{
				return 0;
			}
		}

		DocumentResProtocolResolver::DocumentResProtocolResolver(Ptr<GuiResource> _resource, Ptr<DocumentResolver> previousResolver)
			:DocumentResolver(previousResolver)
			,resource(_resource)
		{
		}

/***********************************************************************
GuiResourceNodeBase
***********************************************************************/

		GuiResourceNodeBase::GuiResourceNodeBase()
			:parent(0)
		{
		}

		GuiResourceNodeBase::~GuiResourceNodeBase()
		{
		}

		const WString& GuiResourceNodeBase::GetName()
		{
			return name;
		}

		GuiResourceFolder* GuiResourceNodeBase::GetParent()
		{
			return parent;
		}

/***********************************************************************
GuiResourceItem
***********************************************************************/

		GuiResourceItem::GuiResourceItem()
		{
		}

		GuiResourceItem::~GuiResourceItem()
		{
		}

		Ptr<Object> GuiResourceItem::GetContent()
		{
			return content;
		}

		void GuiResourceItem::SetContent(Ptr<Object> value)
		{
			content=value;
		}

		Ptr<GuiImageData> GuiResourceItem::AsImage()
		{
			return content.Cast<GuiImageData>();
		}

		Ptr<parsing::xml::XmlDocument> GuiResourceItem::AsXml()
		{
			return content.Cast<XmlDocument>();
		}

		Ptr<ObjectBox<WString>> GuiResourceItem::AsString()
		{
			return content.Cast<ObjectBox<WString>>();
		}

		Ptr<DocumentModel> GuiResourceItem::AsDocument()
		{
			return content.Cast<DocumentModel>();
		}

/***********************************************************************
GuiResourceFolder
***********************************************************************/

		void GuiResourceFolder::LoadResourceFolderXml(DelayLoading& delayLoading, const WString& containingFolder, Ptr<parsing::xml::XmlElement> folderXml, Ptr<parsing::tabling::ParsingTable> xmlParsingTable)
		{
			ClearItems();
			ClearFolders();
			FOREACH(Ptr<XmlElement>, element, XmlGetElements(folderXml))
			{
				WString name;
				if(Ptr<XmlAttribute> nameAtt=XmlGetAttribute(element, L"name"))
				{
					name=nameAtt->value.value;
				}
				if(element->name.value==L"Folder")
				{
					if(name!=L"")
					{
						Ptr<GuiResourceFolder> folder=new GuiResourceFolder;
						if(AddFolder(name, folder))
						{
							WString newContainingFolder=containingFolder;
							Ptr<XmlElement> newFolderXml=element;
							if(Ptr<XmlAttribute> contentAtt=XmlGetAttribute(element, L"content"))
							{
								if(contentAtt->value.value==L"Link")
								{
									WString filePath=containingFolder+XmlGetValue(element);
									WString text;
									if(LoadTextFile(filePath, text))
									{
										Ptr<XmlDocument> xml=XmlParseDocument(text, xmlParsingTable);
										if(xml)
										{
											newContainingFolder=GetFolderPath(filePath);
											newFolderXml=xml->rootElement;
										}
									}
								}
							}
							folder->LoadResourceFolderXml(delayLoading, newContainingFolder, newFolderXml, xmlParsingTable);
						}
					}
				}
				else
				{
					WString filePath;
					if(Ptr<XmlAttribute> contentAtt=XmlGetAttribute(element, L"content"))
					{
						if(contentAtt->value.value==L"File")
						{
							filePath=containingFolder+XmlGetValue(element);
							if(name==L"")
							{
								name=GetFileName(filePath);
							}
						}
					}

					Ptr<GuiResourceItem> item=new GuiResourceItem;
					if(AddItem(name, item))
					{
						if(filePath!=L"")
						{
							stream::FileStream fileStream(filePath, stream::FileStream::ReadOnly);
							if(fileStream.IsAvailable())
							{
								if(element->name.value==L"Xml" || element->name.value==L"Doc")
								{
									WString text;
									if(LoadTextFromStream(fileStream, text))
									{
										Ptr<XmlDocument> xml=XmlParseDocument(text, xmlParsingTable);
										if(xml)
										{
											item->SetContent(xml);
											if(element->name.value==L"Doc")
											{
												delayLoading.documentModelFolders.Add(item, containingFolder);
											}
										}
									}
								}
								else if(element->name.value==L"Text")
								{
									WString text;
									if(LoadTextFromStream(fileStream, text))
									{
										item->SetContent(new ObjectBox<WString>(text));
									}
								}
								else if(element->name.value==L"Image")
								{
									Ptr<INativeImage> image=GetCurrentController()->ImageService()->CreateImageFromStream(fileStream);
									if(image)
									{
										Ptr<GuiImageData> imageData=new GuiImageData(image, 0);
										item->SetContent(imageData);
									}
								}
							}
						}
						else
						{
							if(element->name.value==L"Xml" || element->name.value==L"Doc")
							{
								Ptr<XmlElement> root=XmlGetElements(element).First(0);
								if(root)
								{
									Ptr<XmlDocument> xml=new XmlDocument;
									xml->rootElement=root;
									item->SetContent(xml);
									if(element->name.value==L"Doc")
									{
										delayLoading.documentModelFolders.Add(item, containingFolder);
									}
								}
							}
							else if(element->name.value==L"Text")
							{
								WString text=XmlGetValue(element);
								item->SetContent(new ObjectBox<WString>(text));
							}
							else if(element->name.value==L"Image")
							{
							}
						}
						if(!item->GetContent())
						{
							RemoveItem(name);
						}
					}
				}
			}
		}

		GuiResourceFolder::GuiResourceFolder()
		{
		}

		GuiResourceFolder::~GuiResourceFolder()
		{
		}

		const GuiResourceFolder::ItemList& GuiResourceFolder::GetItems()
		{
			return items.Values();
		}

		Ptr<GuiResourceItem> GuiResourceFolder::GetItem(const WString& name)
		{
			vint index=items.Keys().IndexOf(name);
			return index==-1?0:items.Values().Get(index);
		}

		bool GuiResourceFolder::AddItem(const WString& name, Ptr<GuiResourceItem> item)
		{
			if(item->GetParent()!=0 || items.Keys().Contains(name)) return false;
			items.Add(name, item);
			item->parent=this;
			item->name=name;
			return true;
		}

		Ptr<GuiResourceItem> GuiResourceFolder::RemoveItem(const WString& name)
		{
			Ptr<GuiResourceItem> item=GetItem(name);
			if(!item) return 0;
			items.Remove(name);
			return item;
		}

		void GuiResourceFolder::ClearItems()
		{
			items.Clear();
		}

		const GuiResourceFolder::FolderList& GuiResourceFolder::GetFolders()
		{
			return folders.Values();
		}

		Ptr<GuiResourceFolder> GuiResourceFolder::GetFolder(const WString& name)
		{
			vint index=folders.Keys().IndexOf(name);
			return index==-1?0:folders.Values().Get(index);
		}

		bool GuiResourceFolder::AddFolder(const WString& name, Ptr<GuiResourceFolder> folder)
		{
			if(folder->GetParent()!=0 || folders.Keys().Contains(name)) return false;
			folders.Add(name, folder);
			folder->parent=this;
			folder->name=name;
			return true;
		}

		Ptr<GuiResourceFolder> GuiResourceFolder::RemoveFolder(const WString& name)
		{
			Ptr<GuiResourceFolder> folder=GetFolder(name);
			if(!folder) return 0;
			folders.Remove(name);
			return folder;
		}

		void GuiResourceFolder::ClearFolders()
		{
			folders.Clear();
		}

		Ptr<Object> GuiResourceFolder::GetValueByPath(const WString& path)
		{
			const wchar_t* buffer=path.Buffer();
			const wchar_t* index=wcschr(buffer, L'\\');
			if(!index) index=wcschr(buffer, '/');

			if(index)
			{
				WString name=path.Sub(0, index-buffer);
				Ptr<GuiResourceFolder> folder=GetFolder(name);
				if(folder)
				{
					vint start=index-buffer+1;
					return folder->GetValueByPath(path.Sub(start, path.Length()-start));
				}
			}
			else
			{
				Ptr<GuiResourceItem> item=GetItem(path);
				if(item)
				{
					return item->GetContent();
				}
			}
			return 0;
		}

/***********************************************************************
GuiResource
***********************************************************************/

		GuiResource::GuiResource()
		{
		}

		GuiResource::~GuiResource()
		{
		}

		Ptr<GuiResource> GuiResource::LoadFromXml(const WString& filePath)
		{
			Ptr<ParsingTable> table;
			Ptr<XmlDocument> xml;
			Ptr<GuiResource> resource;
			{
				WString text;
				if(LoadTextFile(filePath, text))
				{
					table=XmlLoadTable();
					xml=XmlParseDocument(text, table);
				}
			}
			if(xml)
			{
				resource=new GuiResource;
				DelayLoading delayLoading;
				resource->LoadResourceFolderXml(delayLoading, GetFolderPath(filePath), xml->rootElement, table);

				for(vint i=0;i<delayLoading.documentModelFolders.Count();i++)
				{
					Ptr<GuiResourceItem> item=delayLoading.documentModelFolders.Keys()[i];
					WString folder=delayLoading.documentModelFolders.Values().Get(i);
					if(Ptr<XmlDocument> xml=item->AsXml())
					{
						Ptr<DocumentResolver> resolver=new DocumentResProtocolResolver(resource, new DocumentFileProtocolResolver(folder));
						Ptr<DocumentModel> model=DocumentModel::LoadFromXml(xml, resolver);
						item->SetContent(model);
					}
				}
			}
			return resource;
		}

		Ptr<DocumentModel> GuiResource::GetDocumentByPath(const WString& path)
		{
			Ptr<DocumentModel> result=GetValueByPath(path).Cast<DocumentModel>();
			if(!result) throw ArgumentException(L"Path not exists.", L"GuiResource::GetDocumentByPath", L"path");
			return result;
		}

		Ptr<GuiImageData> GuiResource::GetImageByPath(const WString& path)
		{
			Ptr<GuiImageData> result=GetValueByPath(path).Cast<GuiImageData>();
			if(!result) throw ArgumentException(L"Path not exists.", L"GuiResource::GetImageByPath", L"path");
			return result;
		}

		Ptr<parsing::xml::XmlDocument> GuiResource::GetXmlByPath(const WString& path)
		{
			Ptr<XmlDocument> result=GetValueByPath(path).Cast<XmlDocument>();
			if(!result) throw ArgumentException(L"Path not exists.", L"GuiResource::GetXmlByPath", L"path");
			return result;
		}

		WString GuiResource::GetStringByPath(const WString& path)
		{
			Ptr<ObjectBox<WString>> result=GetValueByPath(path).Cast<ObjectBox<WString>>();
			if(!result) throw ArgumentException(L"Path not exists.", L"GuiResource::GetStringByPath", L"path");
			return result->Unbox();
		}
	}
}