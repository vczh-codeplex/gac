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
				Ptr<DocumentContainerRun>			container;
				vint								paragraphIndex;
				Ptr<DocumentResolver>				resolver;
				Ptr<TemplateInfo>					templateInfo;
				Regex								regexAttributeApply;

				DeserializeNodeVisitor(Ptr<DocumentModel> _model, Ptr<DocumentParagraphRun> _paragraph, vint _paragraphIndex, Ptr<DocumentResolver> _resolver)
					:model(_model)
					,container(_paragraph)
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

				void VisitContainer(DocumentContainerRun* run)
				{
					FOREACH(Ptr<DocumentRun>, subRun, run->runs)
					{
						subRun->Accept(this);
					}
				}

				void Visit(DocumentTextRun* run)override
				{
				}

				void Visit(DocumentStylePropertiesRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentStyleApplicationRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentHyperlinkTextRun* run)override
				{
					VisitContainer(run);
					if(run->hyperlinkId==hyperlinkId)
					{
						if(active)
						{
							run->styleName=run->activeStyleName;
						}
						else
						{
							run->styleName=run->normalStyleName;
						}
					}
				}

				void Visit(DocumentImageRun* run)override
				{
				}

				void Visit(DocumentTemplateApplicationRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentTemplateContentRun* run)override
				{
					VisitContainer(run);
				}

				void Visit(DocumentParagraphRun* run)override
				{
					VisitContainer(run);
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
				Ptr<DocumentStyleProperties> sp=new DocumentStyleProperties;
				sp->face=font.fontFamily;
				sp->size=font.size;
				sp->color=Color();
				sp->bold=font.bold;
				sp->italic=font.italic;
				sp->underline=font.underline;
				sp->strikeline=font.strikeline;
				sp->antialias=font.antialias;
				sp->verticalAntialias=font.verticalAntialias;

				Ptr<DocumentStyle> style=new DocumentStyle;
				style->styles=sp;
				styles.Add(L"#Default", style);
			}
			{
				Ptr<DocumentStyle> style=new DocumentStyle;
				styles.Add(L"#Context", style);
			}
			{
				Ptr<DocumentStyleProperties> sp=new DocumentStyleProperties;
				sp->parentStyleName=L"#Context";
				sp->color=Color(0, 0, 255);
				sp->underline=true;

				Ptr<DocumentStyle> style=new DocumentStyle;
				style->styles=sp;
				styles.Add(L"#NormalLink", style);
			}
			{
				Ptr<DocumentStyleProperties> sp=new DocumentStyleProperties;
				sp->parentStyleName=L"#Context";
				sp->color=Color(0, 0, 255);
				sp->underline=true;

				Ptr<DocumentStyle> style=new DocumentStyle;
				style->styles=sp;
				styles.Add(L"#ActiveLink", style);
			}
		}

		DocumentModel::RawStylePair DocumentModel::GetStyle(const WString& styleName, const RawStylePair& context)
		{
			Ptr<DocumentStyle> selectedStyle;
			{
				vint index=styles.Keys().IndexOf(styleName);
				if(index!=-1)
				{
					selectedStyle=styles.Values()[index];
				}
				else
				{
					selectedStyle=styles[L"#Default"];
				}
			}

			if(!selectedStyle->resolvedStyles)
			{
				Ptr<DocumentStyleProperties> sp=new DocumentStyleProperties;
				selectedStyle->resolvedStyles=sp;

				Ptr<DocumentStyle> currentStyle;
				WString currentName=styleName;
				while(true)
				{
					vint index=styles.Keys().IndexOf(currentName);
					if(index==-1) break;
					currentStyle=styles.Values().Get(index);
					currentName=currentStyle->parentStyleName;
					Ptr<DocumentStyleProperties> csp=currentStyle->styles;

					if(!sp->face				&& csp->face)				sp->face				=csp->face;
					if(!sp->size				&& csp->size)				sp->size				=csp->size;
					if(!sp->color				&& csp->color)				sp->color				=csp->color;
					if(!sp->bold				&& csp->bold)				sp->bold				=csp->bold;
					if(!sp->italic				&& csp->italic)				sp->italic				=csp->italic;
					if(!sp->underline			&& csp->underline)			sp->underline			=csp->underline;
					if(!sp->strikeline			&& csp->strikeline)			sp->strikeline			=csp->strikeline;
					if(!sp->antialias			&& csp->antialias)			sp->antialias			=csp->antialias;
					if(!sp->verticalAntialias	&& csp->verticalAntialias)	sp->verticalAntialias	=csp->verticalAntialias;
				}
			}

			Ptr<DocumentStyleProperties> sp=selectedStyle->resolvedStyles;
			FontProperties font;
			font.fontFamily			=sp->face				?sp->face.Value()				:context.key.fontFamily;
			font.size				=sp->size				?sp->size.Value()				:context.key.size;
			font.bold				=sp->bold				?sp->bold.Value()				:context.key.bold;
			font.italic				=sp->italic				?sp->italic.Value()				:context.key.italic;
			font.underline			=sp->underline			?sp->underline.Value()			:context.key.underline;
			font.strikeline			=sp->strikeline			?sp->strikeline.Value()			:context.key.strikeline;
			font.antialias			=sp->antialias			?sp->antialias.Value()			:context.key.antialias;
			font.verticalAntialias	=sp->verticalAntialias	?sp->verticalAntialias.Value()	:context.key.verticalAntialias;
			Color color				=sp->color				?sp->color.Value()				:context.value;
			return RawStylePair(font, color);
		}

		vint DocumentModel::ActivateHyperlink(vint hyperlinkId, bool active)
		{
			vint index=hyperlinkInfos.Keys().IndexOf(hyperlinkId);
			if(index!=-1)
			{
				vint paragraphIndex=hyperlinkInfos.Values().Get(index).paragraphIndex;
				if(0<=paragraphIndex && paragraphIndex<paragraphs.Count())
				{
					Ptr<DocumentParagraphRun> paragraph=paragraphs[paragraphIndex];
					ActivateHyperlinkVisitor visitor(hyperlinkId, active);
					paragraph->Accept(&visitor);
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

						Ptr<DocumentStyleProperties> sp=new DocumentStyleProperties;
						style->styles=sp;

						FOREACH(Ptr<XmlElement>, att, XmlGetElements(styleElement))
						{
							if(att->name.value==L"face")
							{
								sp->face=XmlGetValue(att);
							}
							else if(att->name.value==L"size")
							{
								sp->size=wtoi(XmlGetValue(att));
							}
							else if(att->name.value==L"color")
							{
								sp->color=Color::Parse(XmlGetValue(att));
							}
							else if(att->name.value==L"b")
							{
								sp->bold=XmlGetValue(att)==L"true";
							}
							else if(att->name.value==L"i")
							{
								sp->italic=XmlGetValue(att)==L"true";
							}
							else if(att->name.value==L"u")
							{
								sp->underline=XmlGetValue(att)==L"true";
							}
							else if(att->name.value==L"s")
							{
								sp->strikeline=XmlGetValue(att)==L"true";
							}
							else if(att->name.value==L"antialias")
							{
								WString value=XmlGetValue(att);
								if(value==L"horizontal" || value==L"default")
								{
									sp->antialias=true;
									sp->verticalAntialias=false;
								}
								else if(value==L"no")
								{
									sp->antialias=false;
									sp->verticalAntialias=false;
								}
								else if(value==L"vertical")
								{
									sp->antialias=true;
									sp->verticalAntialias=true;
								}
							}
						}
					}
				}
				if(Ptr<XmlElement> styles=XmlGetElement(xml->rootElement, L"Templates"))
				{
					FOREACH(Ptr<XmlElement>, templateElement, XmlGetElements(styles, L"Template"))
					if(Ptr<XmlAttribute> name=XmlGetAttribute(templateElement, L"name"))
					if(!model->templates.Keys().Contains(name->value.value))
					{
						Ptr<DocumentTemplate> tp=new DocumentTemplate;
						tp->templateDescription=templateElement;
						model->templates.Add(name->value.value, tp);
					}
				}
				if(Ptr<XmlElement> content=XmlGetElement(xml->rootElement, L"Content"))
				{
					FOREACH_INDEXER(Ptr<XmlElement>, p, i, XmlGetElements(content, L"p"))
					{
						Ptr<DocumentParagraphRun> paragraph=new DocumentParagraphRun;
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
	}
}