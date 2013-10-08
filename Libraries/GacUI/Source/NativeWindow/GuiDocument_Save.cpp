#include "GuiResource.h"
#include "..\..\..\..\Common\Source\Stream\FileStream.h"
#include "..\..\..\..\Common\Source\Stream\Accessor.h"
#include "..\..\..\..\Common\Source\Stream\CharFormat.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;
		using namespace parsing::xml;

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
							element->name.value=sp->bold.Value()?L"b":L"b-";
							container->subNodes.Add(element);
							container=element;
						}
						if(sp->italic)
						{
							Ptr<XmlElement> element=new XmlElement;
							element->name.value=sp->italic.Value()?L"i":L"i-";
							container->subNodes.Add(element);
							container=element;
						}
						if(sp->underline)
						{
							Ptr<XmlElement> element=new XmlElement;
							element->name.value=sp->underline.Value()?L"u":L"u-";
							container->subNodes.Add(element);
							container=element;
						}
						if(sp->strikeline)
						{
							Ptr<XmlElement> element=new XmlElement;
							element->name.value=sp->strikeline.Value()?L"s":L"s-";
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
DocumentModel
***********************************************************************/

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