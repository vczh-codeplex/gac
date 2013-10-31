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

		void GuiInstanceContext::CollectValues(collections::Dictionary<WString, Ptr<GuiAttSetterRepr::SetterValue>>& setters, Ptr<parsing::xml::XmlElement> xml)
		{
			if(auto parser=GetParserManager()->GetParser<ElementName>(L"INSTANCE-ELEMENT-NAME"))
			{
				// test if there is only one text value in the xml
				Ptr<GuiAttSetterRepr::SetterValue> defaultValue=new GuiAttSetterRepr::SetterValue;
				if(xml->subNodes.Count()==1)
				{
					if(Ptr<XmlText> text=xml->subNodes[0].Cast<XmlText>())
					{
						Ptr<GuiTextRepr> value=new GuiTextRepr;
						value->text=text->content.value;
						defaultValue->values.Add(value);
					}
					else if(Ptr<XmlCData> text=xml->subNodes[0].Cast<XmlCData>())
					{
						Ptr<GuiTextRepr> value=new GuiTextRepr;
						value->text=text->content.value;
						defaultValue->values.Add(value);
					}
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
									Dictionary<WString, Ptr<GuiAttSetterRepr::SetterValue>> newSetters;
									CollectValues(newSetters, element);
									vint index=newSetters.Keys().IndexOf(L"");
									if(index!=-1)
									{
										CopyFrom(sv->values, newSetters.Values()[index]->values);
									}
								}

								if(sv->values.Count()>0)
								{
									setters.Add(name->name, sv);
								}
							}
						}
						else if(name->IsCtorName())
						{
							// collect constructor values in the default attribute setter
							auto ctor=LoadCtor(element);
							if(ctor)
							{
								defaultValue->values.Add(ctor);
							}
						}
					}
				}

				if(defaultValue->values.Count()>0)
				{
					setters.Add(L"", defaultValue);
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
				}

				// collect children as setters
				CollectValues(setter->setters, xml);
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
				if(Ptr<XmlAttribute> att=XmlGetAttribute(xml->rootElement, L"name"))
				{
					context->typeName=att->value.value;
				}

				// load namespaces
				List<Ptr<XmlAttribute>> namespaceAttributes;
				CopyFrom(namespaceAttributes, xml->rootElement->attributes);
				if(!XmlGetAttribute(xml->rootElement, L"xmlns"))
				{
					Ptr<XmlAttribute> att=new XmlAttribute;
					att->name.value=L"xmlns";
					att->value.value=
						L"system::*;"
						L"system::reflection::*;"
						L"presentation::*;"
						L"presentation::Gui*;"
						L"presentation::controls::*;"
						L"presentation::controls::Gui*;"
						L"presentation::controls::list::*;"
						L"presentation::controls::tree::*;"
						L"presentation::elements::*;"
						L"presentation::elements::Gui*Element;"
						L"presentation::elements::text*;"
						L"presentation::compositions::*;"
						L"presentation::compositions::Gui*Composition";
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

							WString protocol, path;
							if(IsResourceUrl(pattern, protocol, path))
							{
								// resource pattern
								Ptr<GuiInstanceResourcePattern> patternItem=new GuiInstanceResourcePattern;
								patternItem->isFolder=protocol==L"res" && pattern[pattern.Length()-1]==L'/';
								patternItem->protocol=protocol;
								patternItem->path=path;
								info->patterns.Add(patternItem);
							}
							else
							{
								// name pattern
								Ptr<GuiInstanceNamePattern> patternItem=new GuiInstanceNamePattern;
								Pair<vint, vint> star=INVLOC.FindFirst(pattern, L"*", Locale::None);
								if(star.key==-1)
								{
									patternItem->prefix=pattern;
								}
								else
								{
									patternItem->prefix=pattern.Sub(0, star.key);
									patternItem->postfix=pattern.Sub(star.key+star.value, pattern.Length()-star.key-star.value);
								}
								info->patterns.Add(patternItem);
							}
						}
					}
				}

				// load instance
				if(Ptr<XmlElement> element=XmlGetElements(xml->rootElement).First(0))
				{
					context->instance=LoadCtor(element);
				}
			}

			return context->instance?context:0;
		}
	}
}