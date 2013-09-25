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
				DocumentModel*				model;
				Ptr<XmlElement>				container;
				vint						templateLevel;

			public:
				SerializeRunVisitor(DocumentModel* _model, Ptr<XmlElement> _container)
					:model(_model)
					,container(_container)
					,templateLevel(0)
				{
				}

				void VisitContainer(Ptr<XmlElement> _container, DocumentContainerRun* run)
				{
					if(_container)
					{
						container->subNodes.Add(_container);
						Ptr<XmlElement> oldContainer=container;
						container=_container;
						FOREACH(Ptr<DocumentRun>, subRun, run->runs)
						{
							subRun->Accept(this);
						}
						container=oldContainer;
					}
					else
					{
						FOREACH(Ptr<DocumentRun>, subRun, run->runs)
						{
							subRun->Accept(this);
						}
					}
				}

				void Visit(DocumentTextRun* run)override
				{
					if(!templateLevel==0)
					{
						if(run->text!=L"")
						{
							XmlElementWriter writer(container);
							writer.Text(run->text);
						}
					}
				}

				void Visit(DocumentStylePropertiesRun* run)override
				{
					if(templateLevel==0)
					{
						VisitContainer(0, run);
					}
					else
					{
						Ptr<DocumentStyleProperties> sp=run->style;
						Ptr<XmlElement> oldContainer=container;
						if(sp->face || sp->size || sp->color)
						{
							Ptr<XmlElement> element=new XmlElement;
							element->name.value=L"font";
							container->subNodes.Add(element);

							XmlElementWriter writer(element);
							if(sp->face)
							{
								writer.Attribute(L"face", sp->face.Value());
							}
							if(sp->size)
							{
								writer.Attribute(L"size", itow(sp->size.Value()));
							}
							if(sp->color)
							{
								writer.Attribute(L"color", sp->color.Value().ToString());
							}
							if(sp->backgroundColor)
							{
								writer.Attribute(L"bkcolor", sp->color.Value().ToString());
							}
							container=element;
						}
						if(sp->bold)
						{
							Ptr<XmlElement> element=new XmlElement;
							element->name.value=L"b";
							container->subNodes.Add(element);
							container=element;
						}
						if(sp->italic)
						{
							Ptr<XmlElement> element=new XmlElement;
							element->name.value=L"i";
							container->subNodes.Add(element);
							container=element;
						}
						if(sp->underline)
						{
							Ptr<XmlElement> element=new XmlElement;
							element->name.value=L"u";
							container->subNodes.Add(element);
							container=element;
						}
						if(sp->strikeline)
						{
							Ptr<XmlElement> element=new XmlElement;
							element->name.value=L"s";
							container->subNodes.Add(element);
							container=element;
						}
						if(sp->antialias || sp->verticalAntialias)
						{
							bool ha=sp->antialias?sp->antialias.Value():true;
							bool va=sp->verticalAntialias?sp->verticalAntialias.Value():false;
							if(!ha)
							{
								Ptr<XmlElement> element=new XmlElement;
								element->name.value=L"ha";
								container->subNodes.Add(element);
								container=element;
							}
							else if(!va)
							{
								Ptr<XmlElement> element=new XmlElement;
								element->name.value=L"va";
								container->subNodes.Add(element);
								container=element;
							}
							else
							{
								Ptr<XmlElement> element=new XmlElement;
								element->name.value=L"na";
								container->subNodes.Add(element);
								container=element;
							}
						}
						VisitContainer(0, run);
						container=oldContainer;
					}
				}

				void Visit(DocumentStyleApplicationRun* run)override
				{
					if(templateLevel==0)
					{
						VisitContainer(0, run);
					}
					else
					{
						Ptr<XmlElement> element=new XmlElement;
						element->name.value=L"div";
						XmlElementWriter(element).Attribute(L"style", run->styleName);
						VisitContainer(element, run);
					}
				}

				void Visit(DocumentHyperlinkTextRun* run)override
				{
					if(templateLevel==0)
					{
						VisitContainer(0, run);
					}
					else
					{
						Ptr<XmlElement> element=new XmlElement;
						element->name.value=L"a";
						XmlElementWriter writer(element);
						if(run->normalStyleName!=L"#NormalLink")
						{
							writer.Attribute(L"normal", run->normalStyleName);
						}
						if(run->activeStyleName!=L"#ActiveLink")
						{
							writer.Attribute(L"active", run->activeStyleName);
						}
						if(run->hyperlinkId!=-1)
						{
							vint index=model->hyperlinkInfos.Keys().IndexOf(run->hyperlinkId);
							if(index!=-1)
							{
								writer.Attribute(L"href", model->hyperlinkInfos.Values()[index].reference);
							}
						}
						VisitContainer(element, run);
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

				void Visit(DocumentTemplateApplicationRun* run)override
				{
					Ptr<XmlElement> element;
					if(templateLevel==0)
					{
						element=new XmlElement;
						element->name.value=run->templateName;

						XmlElementWriter writer(element);
						for(vint i=0;i<run->attributes.Count();i++)
						{
							WString key=run->attributes.Keys()[i];
							WString value=run->attributes.Values()[i];
							writer.Attribute(key, value);
						}
					}

					templateLevel++;
					VisitContainer(element, run);
					templateLevel--;
				}

				void Visit(DocumentTemplateContentRun* run)override
				{
					if(templateLevel==1)
					{
						templateLevel--;
						VisitContainer(0, run);
						templateLevel++;
					}
					else
					{
						VisitContainer(0, run);
					}
				}

				void Visit(DocumentParagraphRun* run)override
				{
					if(templateLevel==0)
					{
						VisitContainer(0, run);
					}
					else
					{
						Ptr<XmlElement> element=new XmlElement;
						element->name.value=L"p";
						VisitContainer(element, run);
					}
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
					DocumentTemplateApplicationRun*		run;
					XmlElement*							templateElement;
					XmlElement*							contentElement;

					TemplateInfo()
						:run(0)
						,templateElement(0)
						,contentElement(0)
					{
					}
				};

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
								vint index=templateInfo->run->attributes.Keys().IndexOf(name);
								result+=(index==-1?match->Result().Value():templateInfo->run->attributes.Values().Get(index));
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
					Ptr<DocumentTextRun> run=new DocumentTextRun;
					run->text=text;
					container->runs.Add(run);
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
					Ptr<DocumentContainerRun> createdContainer;
					Ptr<TemplateInfo> createdTemplateInfo;
					bool useTemplateInfo=false;
					XmlElement* subNodeContainer=node;

					if(node->name.value==L"br")
					{
						PrintText(L"\r\n");
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
						Ptr<DocumentImageRun> run=new DocumentImageRun;
						if(Ptr<XmlAttribute> source=XmlGetAttribute(node, L"source"))
						{
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
							container->runs.Add(run);
						}
					}
					else if(node->name.value==L"font")
					{
						Ptr<DocumentStylePropertiesRun> run=new DocumentStylePropertiesRun();
						Ptr<DocumentStyleProperties> sp=new DocumentStyleProperties;
						run->style=sp;

						FOREACH(Ptr<XmlAttribute>, att, node->attributes)
						{
							if(att->name.value==L"face")
							{
								sp->face=TranslateAttribute(att->value.value);
							}
							else if(att->name.value==L"size")
							{
								sp->size=wtoi(TranslateAttribute(att->value.value));
							}
							else if(att->name.value==L"color")
							{
								sp->color=Color::Parse(TranslateAttribute(att->value.value));
							}
							else if(att->name.value==L"bkcolor")
							{
								sp->backgroundColor=Color::Parse(TranslateAttribute(att->value.value));
							}
						}
						container->runs.Add(run);
						createdContainer=run;
					}
					else if(node->name.value==L"b")
					{
						Ptr<DocumentStylePropertiesRun> run=new DocumentStylePropertiesRun();
						run->style=new DocumentStyleProperties;
						run->style->bold=true;
						container->runs.Add(run);
						createdContainer=run;
					}
					else if(node->name.value==L"i")
					{
						Ptr<DocumentStylePropertiesRun> run=new DocumentStylePropertiesRun();
						run->style=new DocumentStyleProperties;
						run->style->italic=true;
						container->runs.Add(run);
						createdContainer=run;
					}
					else if(node->name.value==L"u")
					{
						Ptr<DocumentStylePropertiesRun> run=new DocumentStylePropertiesRun();
						run->style=new DocumentStyleProperties;
						run->style->underline=true;
						container->runs.Add(run);
						createdContainer=run;
					}
					else if(node->name.value==L"s")
					{
						Ptr<DocumentStylePropertiesRun> run=new DocumentStylePropertiesRun();
						run->style=new DocumentStyleProperties;
						run->style->strikeline=true;
						container->runs.Add(run);
						createdContainer=run;
					}
					else if(node->name.value==L"ha")
					{
						Ptr<DocumentStylePropertiesRun> run=new DocumentStylePropertiesRun();
						run->style=new DocumentStyleProperties;
						run->style->antialias=true;
						run->style->verticalAntialias=false;
						container->runs.Add(run);
						createdContainer=run;
					}
					else if(node->name.value==L"va")
					{
						Ptr<DocumentStylePropertiesRun> run=new DocumentStylePropertiesRun();
						run->style=new DocumentStyleProperties;
						run->style->antialias=true;
						run->style->verticalAntialias=true;
						container->runs.Add(run);
						createdContainer=run;
					}
					else if(node->name.value==L"na")
					{
						Ptr<DocumentStylePropertiesRun> run=new DocumentStylePropertiesRun();
						run->style=new DocumentStyleProperties;
						run->style->antialias=false;
						run->style->verticalAntialias=false;
						container->runs.Add(run);
						createdContainer=run;
					}
					else if(node->name.value==L"div")
					{
						if(Ptr<XmlAttribute> att=XmlGetAttribute(node, L"style"))
						{
							WString styleName=TranslateAttribute(att->value.value);
							
							Ptr<DocumentStyleApplicationRun> run=new DocumentStyleApplicationRun;
							run->styleName=styleName;
							container->runs.Add(run);
							createdContainer=run;
						}
						else
						{
							createdContainer=container;
						}
					}
					else if(node->name.value==L"a")
					{
						Ptr<DocumentHyperlinkTextRun> run=new DocumentHyperlinkTextRun;
						run->normalStyleName=L"#NormalLink";
						run->activeStyleName=L"#ActiveLink";
						run->hyperlinkId=model->hyperlinkInfos.Count();
						if(Ptr<XmlAttribute> att=XmlGetAttribute(node, L"normal"))
						{
							run->normalStyleName=TranslateAttribute(att->value.value);
						}
						if(Ptr<XmlAttribute> att=XmlGetAttribute(node, L"active"))
						{
							run->activeStyleName=TranslateAttribute(att->value.value);
						}
						run->styleName=run->normalStyleName;
						container->runs.Add(run);
						createdContainer=run;

						WString href;
						if(Ptr<XmlAttribute> att=XmlGetAttribute(node, L"href"))
						{
							href=TranslateAttribute(att->value.value);
						}
						{
							DocumentModel::HyperlinkInfo info;
							info.paragraphIndex=paragraphIndex;
							info.reference=href;
							model->hyperlinkInfos.Add(run->hyperlinkId, info);
						}
					}
					else if(node->name.value==L"p")
					{
						FOREACH(Ptr<XmlNode>, sub, node->subNodes)
						{
							sub->Accept(this);
						}
					}
					else if(node->name.value==L"template-content")
					{
						if(templateInfo && templateInfo->contentElement)
						{
							Ptr<DocumentTemplateContentRun> run=new DocumentTemplateContentRun;
							container->runs.Add(run);
							createdContainer=run;
							createdTemplateInfo=0;
							useTemplateInfo=true;
							if(templateInfo)
							{
								subNodeContainer=templateInfo->contentElement;
							}
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
							Ptr<DocumentTemplateApplicationRun> run=new DocumentTemplateApplicationRun;
							run->templateName=node->name.value;
							FOREACH(Ptr<XmlAttribute>, att, node->attributes)
							{
								run->attributes.Add(att->name.value, TranslateAttribute(att->value.value));
							}
							container->runs.Add(run);
							createdContainer=run;

							createdTemplateInfo=new TemplateInfo;
							createdTemplateInfo->run=run.Obj();
							createdTemplateInfo->templateElement=model->templates.Values().Get(index)->templateDescription.Obj();
							createdTemplateInfo->contentElement=node;
							useTemplateInfo=true;
							subNodeContainer=createdTemplateInfo->templateElement;
						}
					}

					if(createdContainer)
					{
						Ptr<DocumentContainerRun> oldContainer=container;
						container=createdContainer;
						Ptr<TemplateInfo> oldTemplateInfo=templateInfo;
						if(useTemplateInfo)
						{
							templateInfo=createdTemplateInfo;
						}
						FOREACH(Ptr<XmlNode>, subNode, subNodeContainer->subNodes)
						{
							subNode->Accept(this);
						}
						templateInfo=oldTemplateInfo;
						container=oldContainer;
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

		const wchar_t* DocumentModel::DefaultStyleName		= L"#Default";
		const wchar_t* DocumentModel::SelectionStyleName	= L"#Selection";
		const wchar_t* DocumentModel::ContextStyleName		= L"#Context";
		const wchar_t* DocumentModel::NormalLinkStyleName	= L"#NormalLink";
		const wchar_t* DocumentModel::ActiveLinkStyleName	= L"#ActiveLink";

		DocumentModel::DocumentModel()
		{
			{
				FontProperties font=GetCurrentController()->ResourceService()->GetDefaultFont();
				Ptr<DocumentStyleProperties> sp=new DocumentStyleProperties;
				sp->face=font.fontFamily;
				sp->size=font.size;
				sp->color=Color();
				sp->backgroundColor=Color(0, 0, 0, 0);
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
				Ptr<DocumentStyleProperties> sp=new DocumentStyleProperties;
				sp->color=Color(255, 255, 255);
				sp->backgroundColor=Color(51, 153, 255);

				Ptr<DocumentStyle> style=new DocumentStyle;
				style->styles=sp;
				styles.Add(L"#Selection", style);
			}
			{
				Ptr<DocumentStyleProperties> sp=new DocumentStyleProperties;

				Ptr<DocumentStyle> style=new DocumentStyle;
				style->styles=sp;
				styles.Add(L"#Context", style);
			}
			{
				Ptr<DocumentStyleProperties> sp=new DocumentStyleProperties;
				sp->color=Color(0, 0, 255);
				sp->underline=true;

				Ptr<DocumentStyle> style=new DocumentStyle;
				style->parentStyleName=L"#Context";
				style->styles=sp;
				styles.Add(L"#NormalLink", style);
			}
			{
				Ptr<DocumentStyleProperties> sp=new DocumentStyleProperties;
				sp->color=Color(0, 0, 255);
				sp->underline=true;

				Ptr<DocumentStyle> style=new DocumentStyle;
				style->parentStyleName=L"#Context";
				style->styles=sp;
				styles.Add(L"#ActiveLink", style);
			}
		}

		DocumentModel::ResolvedStyle DocumentModel::GetStyle(Ptr<DocumentStyleProperties> sp, const ResolvedStyle& context)
		{
			FontProperties font;
			font.fontFamily			=sp->face				?sp->face.Value()				:context.style.fontFamily;
			font.size				=sp->size				?sp->size.Value()				:context.style.size;
			font.bold				=sp->bold				?sp->bold.Value()				:context.style.bold;
			font.italic				=sp->italic				?sp->italic.Value()				:context.style.italic;
			font.underline			=sp->underline			?sp->underline.Value()			:context.style.underline;
			font.strikeline			=sp->strikeline			?sp->strikeline.Value()			:context.style.strikeline;
			font.antialias			=sp->antialias			?sp->antialias.Value()			:context.style.antialias;
			font.verticalAntialias	=sp->verticalAntialias	?sp->verticalAntialias.Value()	:context.style.verticalAntialias;
			Color color				=sp->color				?sp->color.Value()				:context.color;
			Color backgroundColor	=sp->backgroundColor	?sp->backgroundColor.Value()	:context.backgroundColor;
			return ResolvedStyle(font, color, backgroundColor);
		}

		DocumentModel::ResolvedStyle DocumentModel::GetStyle(const WString& styleName, const ResolvedStyle& context)
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
					if(!sp->backgroundColor		&& csp->backgroundColor)	sp->backgroundColor		=csp->backgroundColor;
					if(!sp->bold				&& csp->bold)				sp->bold				=csp->bold;
					if(!sp->italic				&& csp->italic)				sp->italic				=csp->italic;
					if(!sp->underline			&& csp->underline)			sp->underline			=csp->underline;
					if(!sp->strikeline			&& csp->strikeline)			sp->strikeline			=csp->strikeline;
					if(!sp->antialias			&& csp->antialias)			sp->antialias			=csp->antialias;
					if(!sp->verticalAntialias	&& csp->verticalAntialias)	sp->verticalAntialias	=csp->verticalAntialias;
				}
			}

			Ptr<DocumentStyleProperties> sp=selectedStyle->resolvedStyles;
			return GetStyle(sp, context);
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
							else if(att->name.value==L"bkcolor")
							{
								sp->backgroundColor=Color::Parse(XmlGetValue(att));
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

		Ptr<parsing::xml::XmlDocument> DocumentModel::SaveToXml()
		{
			Ptr<XmlDocument> xml=new XmlDocument;
			Ptr<XmlElement> doc=new XmlElement;
			doc->name.value=L"Doc";
			xml->rootElement=doc;
			{
				Ptr<XmlElement> content=new XmlElement;
				content->name.value=L"Content";
				doc->subNodes.Add(content);
				
				SerializeRunVisitor visitor(this, content);
				FOREACH(Ptr<DocumentParagraphRun>, p, paragraphs)
				{
					p->Accept(&visitor);
				}
			}
			{
				Ptr<XmlElement> stylesElement=new XmlElement;
				stylesElement->name.value=L"Styles";
				doc->subNodes.Add(stylesElement);

				for(vint i=0;i<styles.Count();i++)
				{
					WString name=styles.Keys()[i];
					if(name.Length()>0 && name[0]==L'#') continue;

					Ptr<DocumentStyle> style=styles.Values().Get(i);
					Ptr<DocumentStyleProperties> sp=style->styles;
					Ptr<XmlElement> styleElement=new XmlElement;
					styleElement->name.value=L"Style";
					stylesElement->subNodes.Add(styleElement);

					XmlElementWriter(styleElement).Attribute(L"name", name);
					if(style->parentStyleName!=L"")
					{
						XmlElementWriter(styleElement).Attribute(L"parent", style->parentStyleName);
					}

					if(sp->face)				XmlElementWriter(styleElement).Element(L"face").Text(		sp->face.Value()						);
					if(sp->size)				XmlElementWriter(styleElement).Element(L"size").Text(itow(	sp->size.Value()						));
					if(sp->color)				XmlElementWriter(styleElement).Element(L"color").Text(		sp->color.Value().ToString()			);
					if(sp->backgroundColor)		XmlElementWriter(styleElement).Element(L"bkcolor").Text(	sp->color.Value().ToString()			);
					if(sp->bold)				XmlElementWriter(styleElement).Element(L"bold").Text(		sp->bold.Value()?L"true":L"false"		);
					if(sp->italic)				XmlElementWriter(styleElement).Element(L"italic").Text(		sp->italic.Value()?L"true":L"false"		);
					if(sp->underline)			XmlElementWriter(styleElement).Element(L"underline").Text(	sp->underline.Value()?L"true":L"false"	);
					if(sp->strikeline)			XmlElementWriter(styleElement).Element(L"strikeline").Text(	sp->strikeline.Value()?L"true":L"false"	);
					if(sp->antialias && sp->verticalAntialias)
					{
						bool h=sp->antialias;
						bool v=sp->verticalAntialias;
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
			{
				Ptr<XmlElement> templatesElement=new XmlElement;
				templatesElement->name.value=L"Templates";
				doc->subNodes.Add(templatesElement);

				FOREACH(Ptr<DocumentTemplate>, tp, templates.Values())
				{
					templatesElement->subNodes.Add(tp->templateDescription);
				}
			}
			return xml;
		}
	}
}