#include "GuiInstanceRepresentation.h"
#include "..\Resources\GuiParserManager.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;
		using namespace parsing;
		using namespace parsing::xml;

/***********************************************************************
GuiInstanceContext
***********************************************************************/

		void GuiInstanceContext::CollectDefaultAttributes(GuiAttSetterRepr::ValueList& values, Ptr<parsing::xml::XmlElement> xml)
		{
			if(auto parser=GetParserManager()->GetParser<ElementName>(L"INSTANCE-ELEMENT-NAME"))
			{
				// test if there is only one text value in the xml
				if(xml->subNodes.Count()==1)
				{
					if(Ptr<XmlText> text=xml->subNodes[0].Cast<XmlText>())
					{
						Ptr<GuiTextRepr> value=new GuiTextRepr;
						value->text=text->content.value;
						values.Add(value);
					}
					else if(Ptr<XmlCData> text=xml->subNodes[0].Cast<XmlCData>())
					{
						Ptr<GuiTextRepr> value=new GuiTextRepr;
						value->text=text->content.value;
						values.Add(value);
					}
				}

				// collect default attributes
				FOREACH(Ptr<XmlElement>, element, XmlGetElements(xml))
				{
					if(auto name=parser->TypedParse(element->name.value))
					{
						if(name->IsCtorName())
						{
							// collect constructor values in the default attribute setter
							auto ctor=LoadCtor(element);
							if(ctor)
							{
								values.Add(ctor);
							}
						}
					}
				}
			}
		}

		void GuiInstanceContext::CollectAttributes(GuiAttSetterRepr::SetteValuerMap& setters, Ptr<parsing::xml::XmlElement> xml)
		{
			if(auto parser=GetParserManager()->GetParser<ElementName>(L"INSTANCE-ELEMENT-NAME"))
			{
				Ptr<GuiAttSetterRepr::SetterValue> defaultValue=new GuiAttSetterRepr::SetterValue;

				// collect default attributes
				CollectDefaultAttributes(defaultValue->values, xml);
				if(defaultValue->values.Count()>0)
				{
					setters.Add(L"", defaultValue);
				}

				// collect values
				FOREACH(Ptr<XmlElement>, element, XmlGetElements(xml))
				{
					if(auto name=parser->TypedParse(element->name.value))
					{
						if(name->IsPropertyElementName())
						{
							// collect a value as a new attribute setter
							if(!setters.Keys().Contains(name->name))
							{
								Ptr<GuiAttSetterRepr::SetterValue> sv=new GuiAttSetterRepr::SetterValue;
								sv->binding=name->binding;

								if(name->binding==L"set")
								{
									// if the binding is "set", it means that this element is a complete setter element
									Ptr<GuiAttSetterRepr> setter=new GuiAttSetterRepr;
									FillAttSetter(setter, element);
									sv->values.Add(setter);
								}
								else
								{
									// if the binding is not "set", then this is a single-value attribute or a colection attribute
									// fill all data into this attribute
									CollectDefaultAttributes(sv->values, element);
								}

								if(sv->values.Count()>0)
								{
									setters.Add(name->name, sv);
								}
							}
						}
					}
				}
			}
		}

		void GuiInstanceContext::CollectEvents(GuiAttSetterRepr::EventHandlerMap& eventHandlers, Ptr<parsing::xml::XmlElement> xml)
		{
			if(auto parser=GetParserManager()->GetParser<ElementName>(L"INSTANCE-ELEMENT-NAME"))
			{
				// collect values
				FOREACH(Ptr<XmlElement>, element, XmlGetElements(xml))
				{
					if(auto name=parser->TypedParse(element->name.value))
					{
						if(name->IsEventElementName())
						{
							// collect a value as a new attribute setter
							if(!eventHandlers.Keys().Contains(name->name))
							{
								// test if there is only one text value in the xml
								if(element->subNodes.Count()==1)
								{
									if(Ptr<XmlText> text=element->subNodes[0].Cast<XmlText>())
									{
										eventHandlers.Add(name->name, text->content.value);
									}
									else if(Ptr<XmlCData> text=element->subNodes[0].Cast<XmlCData>())
									{
										eventHandlers.Add(name->name, text->content.value);
									}
								}
							}
						}
					}
				}
			}
		}

		void GuiInstanceContext::FillAttSetter(Ptr<GuiAttSetterRepr> setter, Ptr<parsing::xml::XmlElement> xml)
		{
			if(auto parser=GetParserManager()->GetParser<ElementName>(L"INSTANCE-ELEMENT-NAME"))
			{
				// collect attributes as setters
				FOREACH(Ptr<XmlAttribute>, att, xml->attributes)
				{
					if(auto name=parser->TypedParse(att->name.value))
					if(name->IsPropertyAttributeName())
					{
						if(!setter->setters.Keys().Contains(name->name))
						{
							Ptr<GuiAttSetterRepr::SetterValue> sv=new GuiAttSetterRepr::SetterValue;
							sv->binding=name->binding;
							setter->setters.Add(name->name, sv);

							Ptr<GuiTextRepr> value=new GuiTextRepr;
							value->text=att->value.value;
							sv->values.Add(value);
						}
					}
					else if (name->IsEventAttributeName())
					{
						if (!setter->eventHandlers.Keys().Contains(name->name))
						{
							setter->eventHandlers.Add(name->name, att->value.value);
						}
					}
				}

				// collect attributes and events
				CollectAttributes(setter->setters, xml);
				CollectEvents(setter->eventHandlers, xml);
			}
		}

		Ptr<GuiConstructorRepr> GuiInstanceContext::LoadCtor(Ptr<parsing::xml::XmlElement> xml)
		{
			if(auto parser=GetParserManager()->GetParser<ElementName>(L"INSTANCE-ELEMENT-NAME"))
			if(auto name=parser->TypedParse(xml->name.value))
			if(name->IsCtorName())
			{
				Ptr<GuiConstructorRepr> ctor=new GuiConstructorRepr;
				ctor->typeNamespace=name->namespaceName;
				ctor->typeName=name->name;
				
				// collect reference attributes
				FOREACH(Ptr<XmlAttribute>, att, xml->attributes)
				{
					if(auto name=parser->TypedParse(att->name.value))
					if(name->IsReferenceAttributeName())
					{
						if (name->name == L"Name")
						{
							ctor->instanceName = att->value.value;
						}
					}
				}
				// collect attributes as setters
				FillAttSetter(ctor, xml);
				return ctor;
			}
			return 0;
		}

		Ptr<GuiInstanceContext> GuiInstanceContext::LoadFromXml(Ptr<parsing::xml::XmlDocument> xml)
		{
			Ptr<GuiInstanceContext> context=new GuiInstanceContext;
			if(xml->rootElement->name.value==L"Instance")
			{
				// load type name
				if(Ptr<XmlAttribute> att=XmlGetAttribute(xml->rootElement, L"ref.Class"))
				{
					context->className=att->value.value;
				}

				// load namespaces
				List<Ptr<XmlAttribute>> namespaceAttributes;
				CopyFrom(namespaceAttributes, xml->rootElement->attributes);
				if(!XmlGetAttribute(xml->rootElement, L"xmlns"))
				{
					Ptr<XmlAttribute> att=new XmlAttribute;
					att->name.value=L"xmlns";
					att->value.value =
						L"presentation::controls::Gui*;"
						L"presentation::elements::Gui*Element;"
						L"presentation::compositions::Gui*Composition;"
						L"presentation::compositions::Gui*;"
						L"system::*;"
						L"system::reflection::*;"
						L"presentation::*;"
						L"presentation::Gui*;"
						L"presentation::controls::*;"
						L"presentation::controls::list::*;"
						L"presentation::controls::tree::*;"
						L"presentation::elements::*;"
						L"presentation::elements::Gui*;"
						L"presentation::elements::text*;"
						L"presentation::compositions::*";
					namespaceAttributes.Add(att);
				}
				FOREACH(Ptr<XmlAttribute>, att, namespaceAttributes)
				{
					// check if the attribute defines a namespace
					WString attName=att->name.value;
					if(attName.Length()>=5 && attName.Left(5)==L"xmlns")
					{
						WString ns;
						if(attName.Length()>6)
						{
							if(attName.Left(6)==L"xmlns:")
							{
								ns=attName.Sub(6, attName.Length()-6);
							}
							else
							{
								continue;
							}
						}

						// create a data structure for the namespace
						Ptr<NamespaceInfo> info;
						vint index=context->namespaces.Keys().IndexOf(ns);
						if(index==-1)
						{
							info=new NamespaceInfo;
							info->name=ns;
							context->namespaces.Add(ns, info);
						}
						else
						{
							info=context->namespaces.Values()[index];
						}

						// extract all patterns in the namespace
						const wchar_t* attValue=att->value.value.Buffer();
						while(*attValue)
						{
							// split the value by ';'
							const wchar_t* attSemicolon=wcschr(attValue, L';');
							WString pattern;
							if(attSemicolon)
							{
								pattern=WString(attValue, attSemicolon-attValue);
								attValue=attSemicolon+1;
							}
							else
							{
								vint len=wcslen(attValue);
								pattern=WString(attValue, len);
								attValue+=len;
							}

							// add the pattern to the namespace
							Ptr<GuiInstanceNamespace> ns=new GuiInstanceNamespace;
							Pair<vint, vint> star=INVLOC.FindFirst(pattern, L"*", Locale::None);
							if(star.key==-1)
							{
								ns->prefix=pattern;
							}
							else
							{
								ns->prefix=pattern.Sub(0, star.key);
								ns->postfix=pattern.Sub(star.key+star.value, pattern.Length()-star.key-star.value);
							}
							info->namespaces.Add(ns);
						}
					}
				}

				// load instance
				FOREACH(Ptr<XmlElement>, element, XmlGetElements(xml->rootElement))
				{
					if (element->name.value == L"ref.Parameter")
					{
						auto attName = XmlGetAttribute(element, L"Name");
						auto attClass = XmlGetAttribute(element, L"Class");
						if (attName && attClass)
						{
							auto parameter = MakePtr<GuiInstanceParameter>();
							parameter->name = attName->value.value;
							parameter->className = attClass->value.value;
							context->parameters.Add(parameter);
						}
					}
					else if (!context->instance)
					{
						context->instance=LoadCtor(element);
					}
				}
			}

			return context->instance?context:0;
		}
	}
}