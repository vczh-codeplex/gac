#include "GuiInstanceRepresentation.h"
#include "GuiInstanceLoader.h"
#include "InstanceQuery\GuiInstanceQuery.h"
#include "..\Resources\GuiParserManager.h"
#include "..\Controls\Templates\GuiControlTemplateStyles.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;
		using namespace parsing;
		using namespace parsing::xml;
		using namespace templates;
		using namespace stream;

/***********************************************************************
GuiTextRepr
***********************************************************************/

		Ptr<GuiValueRepr> GuiTextRepr::Clone()
		{
			auto repr = MakePtr<GuiTextRepr>();
			repr->fromStyle = fromStyle;
			repr->text = text;
			return repr;
		}

		void GuiTextRepr::FillXml(Ptr<parsing::xml::XmlElement> xml, bool serializePrecompiledResource)
		{
			if (!fromStyle || serializePrecompiledResource)
			{
				auto xmlText = MakePtr<XmlText>();
				xmlText->content.value = text;
				xml->subNodes.Add(xmlText);
			}
		}

/***********************************************************************
GuiAttSetterRepr
***********************************************************************/

		void GuiAttSetterRepr::CloneBody(Ptr<GuiAttSetterRepr> repr)
		{
			CopyFrom(repr->eventHandlers, eventHandlers);
			FOREACH(WString, name, setters.Keys())
			{
				Ptr<SetterValue> src = setters[name];
				Ptr<SetterValue> dst = new SetterValue;
				dst->binding = src->binding;
				FOREACH(Ptr<GuiValueRepr>, value, src->values)
				{
					dst->values.Add(value->Clone());
				}
				repr->setters.Add(name, dst);
			}
			repr->instanceName = instanceName;
		}

		Ptr<GuiValueRepr> GuiAttSetterRepr::Clone()
		{
			auto repr = MakePtr<GuiAttSetterRepr>();
			repr->fromStyle = fromStyle;
			CloneBody(repr);
			return repr;
		}

		void GuiAttSetterRepr::FillXml(Ptr<parsing::xml::XmlElement> xml, bool serializePrecompiledResource)
		{
			if (!fromStyle || serializePrecompiledResource)
			{
				if (instanceName)
				{
					auto attName = MakePtr<XmlAttribute>();
					attName->name.value = L"ref.Name";
					attName->value.value = instanceName.Value();
					xml->attributes.Add(attName);
				}

				for (vint i = 0; i < setters.Count(); i++)
				{
					auto key = setters.Keys()[i];
					auto value = setters.Values()[i];
					if (key == L"")
					{
						FOREACH(Ptr<GuiValueRepr>, repr, value->values)
						{
							repr->FillXml(xml, serializePrecompiledResource);
						}
					}
					else if (value->values.Count() == 1 && value->values[0].Cast<GuiTextRepr>())
					{
						if (!value->values[0]->fromStyle || serializePrecompiledResource)
						{
							auto att = MakePtr<XmlAttribute>();
							att->name.value = key;
							if (value->binding != L"")
							{
								att->name.value += L"-" + value->binding;
							}
							att->value.value = value->values[0].Cast<GuiTextRepr>()->text;
							xml->attributes.Add(att);
						}
					}
					else
					{
						auto xmlProp = MakePtr<XmlElement>();
						xmlProp->name.value = L"att." + key;
						if (value->binding != L"")
						{
							xmlProp->name.value += L"-" + value->binding;
						}

						FOREACH(Ptr<GuiValueRepr>, repr, value->values)
						{
							repr->FillXml(xmlProp, serializePrecompiledResource);
						}
						xml->subNodes.Add(xmlProp);
					}
				}

				for (vint i = 0; i < eventHandlers.Count(); i++)
				{
					auto key = eventHandlers.Keys()[i];
					auto value = eventHandlers.Values()[i];

					auto xmlEvent = MakePtr<XmlElement>();
					xmlEvent->name.value = L"ev." + key;
					if (value->binding != L"")
					{
						xmlEvent->name.value += L"-" + value->binding;
					}
					xml->subNodes.Add(xmlEvent);

					auto xmlText = MakePtr<XmlText>();
					xmlText->content.value = value->value;
					xmlEvent->subNodes.Add(xmlText);
				}
			}
		}

/***********************************************************************
GuiConstructorRepr
***********************************************************************/

		Ptr<GuiValueRepr> GuiConstructorRepr::Clone()
		{
			auto repr = MakePtr<GuiConstructorRepr>();
			repr->fromStyle = fromStyle;
			repr->typeNamespace = typeNamespace;
			repr->typeName = typeName;
			repr->styleName = styleName;
			CloneBody(repr);
			return repr;
		}

		void GuiConstructorRepr::FillXml(Ptr<parsing::xml::XmlElement> xml, bool serializePrecompiledResource)
		{
			if (!fromStyle || serializePrecompiledResource)
			{
				auto xmlCtor = MakePtr<XmlElement>();
				if (typeNamespace == L"")
				{
					xmlCtor->name.value = typeName;
				}
				else
				{
					xmlCtor->name.value = typeNamespace + L":" + typeName;
				}

				if (styleName)
				{
					auto attStyle = MakePtr<XmlAttribute>();
					attStyle->name.value = L"ref.Style";
					attStyle->value.value = styleName.Value();
					xml->attributes.Add(attStyle);
				}

				GuiAttSetterRepr::FillXml(xmlCtor, serializePrecompiledResource);
				xml->subNodes.Add(xmlCtor);
			}
		}

/***********************************************************************
GuiInstanceContext
***********************************************************************/

		void GuiInstanceContext::CollectDefaultAttributes(GuiAttSetterRepr::ValueList& values, Ptr<parsing::xml::XmlElement> xml, collections::List<WString>& errors)
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
					if(auto name=parser->TypedParse(element->name.value, errors))
					{
						if(name->IsCtorName())
						{
							// collect constructor values in the default attribute setter
							auto ctor=LoadCtor(element, errors);
							if(ctor)
							{
								values.Add(ctor);
							}
						}
					}
				}
			}
		}

		void GuiInstanceContext::CollectAttributes(GuiAttSetterRepr::SetteValuerMap& setters, Ptr<parsing::xml::XmlElement> xml, collections::List<WString>& errors)
		{
			if(auto parser=GetParserManager()->GetParser<ElementName>(L"INSTANCE-ELEMENT-NAME"))
			{
				Ptr<GuiAttSetterRepr::SetterValue> defaultValue=new GuiAttSetterRepr::SetterValue;

				// collect default attributes
				CollectDefaultAttributes(defaultValue->values, xml, errors);
				if(defaultValue->values.Count()>0)
				{
					setters.Add(L"", defaultValue);
				}

				// collect values
				FOREACH(Ptr<XmlElement>, element, XmlGetElements(xml))
				{
					if(auto name=parser->TypedParse(element->name.value, errors))
					{
						if(name->IsPropertyElementName())
						{
							// collect a value as a new attribute setter
							if (setters.Keys().Contains(name->name))
							{
								errors.Add(L"Duplicated attribute name \"" + name->name + L"\".");
							}
							else
							{
								Ptr<GuiAttSetterRepr::SetterValue> sv=new GuiAttSetterRepr::SetterValue;
								sv->binding=name->binding;

								if(name->binding==L"set")
								{
									// if the binding is "set", it means that this element is a complete setter element
									Ptr<GuiAttSetterRepr> setter=new GuiAttSetterRepr;
									FillAttSetter(setter, element, errors);
									sv->values.Add(setter);
								}
								else
								{
									// if the binding is not "set", then this is a single-value attribute or a colection attribute
									// fill all data into this attribute
									CollectDefaultAttributes(sv->values, element, errors);
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

		void GuiInstanceContext::CollectEvents(GuiAttSetterRepr::EventHandlerMap& eventHandlers, Ptr<parsing::xml::XmlElement> xml, collections::List<WString>& errors)
		{
			if(auto parser=GetParserManager()->GetParser<ElementName>(L"INSTANCE-ELEMENT-NAME"))
			{
				// collect values
				FOREACH(Ptr<XmlElement>, element, XmlGetElements(xml))
				{
					if(auto name=parser->TypedParse(element->name.value, errors))
					{
						if(name->IsEventElementName())
						{
							// collect a value as a new attribute setter
							if (eventHandlers.Keys().Contains(name->name))
							{
								errors.Add(L"Duplicated event name \"" + name->name + L"\".");
							}
							else
							{
								// test if there is only one text value in the xml
								if(element->subNodes.Count()==1)
								{
									if(Ptr<XmlText> text=element->subNodes[0].Cast<XmlText>())
									{
										auto value = MakePtr<GuiAttSetterRepr::EventValue>();
										value->binding = name->binding;
										value->value = text->content.value;
										eventHandlers.Add(name->name, value);
									}
									else if(Ptr<XmlCData> text=element->subNodes[0].Cast<XmlCData>())
									{
										auto value = MakePtr<GuiAttSetterRepr::EventValue>();
										value->binding = name->binding;
										value->value = text->content.value;
										eventHandlers.Add(name->name, value);
									}
								}
							}
						}
					}
				}
			}
		}

		void GuiInstanceContext::FillAttSetter(Ptr<GuiAttSetterRepr> setter, Ptr<parsing::xml::XmlElement> xml, collections::List<WString>& errors)
		{
			if(auto parser=GetParserManager()->GetParser<ElementName>(L"INSTANCE-ELEMENT-NAME"))
			{
				// collect attributes as setters
				FOREACH(Ptr<XmlAttribute>, att, xml->attributes)
				{
					if(auto name=parser->TypedParse(att->name.value, errors))
					if(name->IsReferenceAttributeName())
					{
						// collect reference attributes
						if (name->name == L"Name")
						{
							setter->instanceName = att->value.value;
						}
					}
					else if(name->IsPropertyAttributeName())
					{
						// collect attributes setters
						if (setter->setters.Keys().Contains(name->name))
						{
							errors.Add(L"Duplicated attribute name \"" + name->name + L"\".");
						}
						else
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
						// collect event setters
						if (!setter->eventHandlers.Keys().Contains(name->name))
						{
							auto value = MakePtr<GuiAttSetterRepr::EventValue>();
							value->binding = name->binding;
							value->value = att->value.value;
							setter->eventHandlers.Add(name->name, value);
						}
					}
				}

				// collect attributes and events
				CollectAttributes(setter->setters, xml, errors);
				CollectEvents(setter->eventHandlers, xml, errors);
			}
		}

		Ptr<GuiConstructorRepr> GuiInstanceContext::LoadCtor(Ptr<parsing::xml::XmlElement> xml, collections::List<WString>& errors)
		{
			if(auto parser=GetParserManager()->GetParser<ElementName>(L"INSTANCE-ELEMENT-NAME"))
			if(auto name=parser->TypedParse(xml->name.value, errors))
			if(name->IsCtorName())
			{
				Ptr<GuiConstructorRepr> ctor=new GuiConstructorRepr;
				ctor->typeNamespace=name->namespaceName;
				ctor->typeName=name->name;
				// collect attributes as setters
				FOREACH(Ptr<XmlAttribute>, att, xml->attributes)
				{
					if(auto name=parser->TypedParse(att->name.value, errors))
					if(name->IsReferenceAttributeName())
					{
						if (name->name == L"Style")
						{
							ctor->styleName = att->value.value;
						}
					}
				}
				FillAttSetter(ctor, xml, errors);
				return ctor;
			}
			else
			{
				errors.Add(L"Wrong constructor name \"" + xml->name.value + L"\".");
			}
			return 0;
		}

		Ptr<GuiInstanceContext> GuiInstanceContext::LoadFromXml(Ptr<parsing::xml::XmlDocument> xml, collections::List<WString>& errors)
		{
			Ptr<GuiInstanceContext> context=new GuiInstanceContext;
			if(xml->rootElement->name.value==L"Instance")
			{
				// load type name
				if (auto classAttr = XmlGetAttribute(xml->rootElement, L"ref.Class"))
				{
					context->className = classAttr->value.value;
				}

				// load style names
				if (auto styleAttr = XmlGetAttribute(xml->rootElement, L"ref.Styles"))
				{
					SplitBySemicolon(styleAttr->value.value, context->stylePaths);
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
						L"presentation::templates::Gui*;"
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
						L"presentation::compositions::*"
						L"presentation::templates::*";
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

						// extract all patterns in the namespace, split the value by ';'
						List<WString> patterns;
						SplitBySemicolon(att->value.value, patterns);
						FOREACH(WString, pattern, patterns)
						{
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
					else if (element->name.value == L"ref.Cache")
					{
						auto attName = XmlGetAttribute(element, L"Name");
						auto attType = XmlGetAttribute(element, L"Type");
						if (attName && attType)
						{
							auto resolver = GetInstanceLoaderManager()->GetInstanceCacheResolver(attType->value.value);

							MemoryStream stream;
							HexToBinary(stream, XmlGetValue(element));
							stream.SeekFromBegin(0);

							auto cache = resolver->Deserialize(stream);
							context->precompiledCaches.Add(attName->value.value, cache);
						}
					}
					else if (!context->instance)
					{
						context->instance=LoadCtor(element, errors);
					}
				}
			}

			return context->instance?context:0;
		}

		Ptr<parsing::xml::XmlDocument> GuiInstanceContext::SaveToXml(bool serializePrecompiledResource)
		{
			auto xmlInstance = MakePtr<XmlElement>();
			xmlInstance->name.value = L"Instance";
			
			if (className)
			{
				auto attClass = MakePtr<XmlAttribute>();
				attClass->name.value = L"ref.Class";
				attClass->value.value = className.Value();
				xmlInstance->attributes.Add(attClass);
			}

			for (vint i = 0; i < namespaces.Count(); i++)
			{
				auto key = namespaces.Keys()[i];
				auto value = namespaces.Values()[i];

				auto xmlns = MakePtr<XmlAttribute>();
				xmlns->name.value = L"xmlns";
				if (key != L"")
				{
					xmlns->name.value += L":" + key;
				}
				xmlInstance->attributes.Add(xmlns);

				for (vint j = 0; j < value->namespaces.Count(); j++)
				{
					auto ns = value->namespaces[j];
					if (j != 0)
					{
						xmlns->value.value += L";";
					}
					xmlns->value.value += ns->prefix + L"*" + ns->postfix;
				}
			}

			FOREACH(Ptr<GuiInstanceParameter>, parameter, parameters)
			{
				auto xmlParameter = MakePtr<XmlElement>();
				xmlParameter->name.value = L"ref.Parameter";
				xmlInstance->subNodes.Add(xmlParameter);

				auto attName = MakePtr<XmlAttribute>();
				attName->name.value = L"Name";
				attName->value.value = parameter->name;
				xmlParameter->attributes.Add(attName);

				auto attClass = MakePtr<XmlAttribute>();
				attClass->name.value = L"Class";
				attClass->value.value = parameter->className;
				xmlParameter->attributes.Add(attClass);
			}

			if (!serializePrecompiledResource && stylePaths.Count() > 0)
			{
				auto attStyles = MakePtr<XmlAttribute>();
				attStyles->name.value = L"ref.Styles";
				xmlInstance->attributes.Add(attStyles);

				for (vint j = 0; j < stylePaths.Count(); j++)
				{
					if (j != 0)
					{
						attStyles->value.value += L";";
					}
					attStyles->value.value += stylePaths[j];
				}
			}

			if (serializePrecompiledResource)
			{
				for (vint i = 0; i < precompiledCaches.Count(); i++)
				{
					auto key = precompiledCaches.Keys()[i];
					auto value = precompiledCaches.Values()[i];
					auto resolver = GetInstanceLoaderManager()->GetInstanceCacheResolver(value->GetCacheTypeName());

					MemoryStream stream;
					resolver->Serialize(value, stream);
					stream.SeekFromBegin(0);
					auto hex = BinaryToHex(stream);
					
					auto xmlCache = MakePtr<XmlElement>();
					xmlCache->name.value = L"ref.Cache";
					xmlInstance->subNodes.Add(xmlCache);

					auto attName = MakePtr<XmlAttribute>();
					attName->name.value = L"Name";
					attName->value.value = key;
					xmlCache->attributes.Add(attName);

					auto attType = MakePtr<XmlAttribute>();
					attType->name.value = L"Type";
					attType->value.value = value->GetCacheTypeName();
					xmlCache->attributes.Add(attType);

					auto xmlContent = MakePtr<XmlCData>();
					xmlContent->content.value = hex;
					xmlCache->subNodes.Add(xmlContent);
				}
			}

			instance->FillXml(xmlInstance, serializePrecompiledResource);

			auto doc = MakePtr<XmlDocument>();
			doc->rootElement = xmlInstance;
			return doc;
		}

		bool GuiInstanceContext::ApplyStyles(Ptr<GuiResourcePathResolver> resolver, collections::List<WString>& errors)
		{
			if (!appliedStyles)
			{
				appliedStyles = true;

				List<Ptr<GuiInstanceStyle>> styles;
				FOREACH(WString, uri, stylePaths)
				{
					WString protocol, path;
					if (IsResourceUrl(uri, protocol, path))
					{
						if (auto styleContext = resolver->ResolveResource(protocol, path).Cast<GuiInstanceStyleContext>())
						{
							CopyFrom(styles, styleContext->styles, true);
						}
						else
						{
							errors.Add(L"Failed to find the style referred in attribute \"ref.Styles\": \"" + uri + L"\".");
						}
					}
					else
					{
						errors.Add(L"Invalid path in attribute \"ref.Styles\": \"" + uri + L"\".");
					}
				}

				FOREACH(Ptr<GuiInstanceStyle>, style, styles)
				{
					List<Ptr<GuiConstructorRepr>> output;
					ExecuteQuery(style->query, this, output);
					FOREACH(Ptr<GuiConstructorRepr>, ctor, output)
					{
						ApplyStyle(style, ctor);
					}
				}

				return true;
			}
			else
			{
				return false;
			}
		}

/***********************************************************************
GuiInstanceStyle
***********************************************************************/

		namespace visitors
		{
			class SetStyleMarkVisitor : public Object, public GuiValueRepr::IVisitor
			{
			public:
				void Visit(GuiTextRepr* repr)override
				{
					repr->fromStyle = true;
				}

				void Visit(GuiAttSetterRepr* repr)override
				{
					repr->fromStyle = true;
					FOREACH(Ptr<GuiAttSetterRepr::SetterValue>, value, repr->setters.Values())
					{
						FOREACH(Ptr<GuiValueRepr>, subValue, value->values)
						{
							subValue->Accept(this);
						}
					}
					FOREACH(Ptr<GuiAttSetterRepr::EventValue>, value, repr->eventHandlers.Values())
					{
						value->fromStyle = true;
					}
				}

				void Visit(GuiConstructorRepr* repr)override
				{
					Visit((GuiAttSetterRepr*)repr);
				}
			};
		}
		using namespace visitors;

		Ptr<GuiInstanceStyle> GuiInstanceStyle::LoadFromXml(Ptr<parsing::xml::XmlElement> xml, collections::List<WString>& errors)
		{
			auto style = MakePtr<GuiInstanceStyle>();
			if (auto pathAttr = XmlGetAttribute(xml, L"ref.Path"))
			{
				auto parser = GetParserManager()->GetParser<GuiIqQuery>(L"INSTANCE-QUERY");
				if (auto query = parser->TypedParse(pathAttr->value.value, errors))
				{
					style->query = query;
				}
				else
				{
					return 0;
				}
			}
			else
			{
				errors.Add(L"Missing attribute \"ref.Path\" in <Style>.");
			}
			style->setter = MakePtr<GuiAttSetterRepr>();
			GuiInstanceContext::FillAttSetter(style->setter, xml, errors);

			SetStyleMarkVisitor visitor;
			style->setter->Accept(&visitor);
			return style;
		}

		Ptr<parsing::xml::XmlElement> GuiInstanceStyle::SaveToXml()
		{
			auto xmlStyle = MakePtr<XmlElement>();
			xmlStyle->name.value = L"Style";

			auto attPath = MakePtr<XmlAttribute>();
			attPath->name.value = L"ref.Path";
			{
				MemoryStream stream;
				{
					StreamWriter writer(stream);
					GuiIqPrint(query, writer);
				}
				stream.SeekFromBegin(0);
				{
					StreamReader reader(stream);
					attPath->value.value = reader.ReadToEnd();
				}
			}
			xmlStyle->attributes.Add(attPath);

			setter->FillXml(xmlStyle, true);

			return xmlStyle;
		}

/***********************************************************************
GuiInstanceStyleContext
***********************************************************************/

		Ptr<GuiInstanceStyleContext> GuiInstanceStyleContext::LoadFromXml(Ptr<parsing::xml::XmlDocument> xml, collections::List<WString>& errors)
		{
			auto context = MakePtr<GuiInstanceStyleContext>();
			FOREACH(Ptr<XmlElement>, styleElement, XmlGetElements(xml->rootElement))
			{
				if (styleElement->name.value == L"Style")
				{
					if (auto style = GuiInstanceStyle::LoadFromXml(styleElement, errors))
					{
						context->styles.Add(style);
					}
				}
				else
				{
					errors.Add(L"Unknown style type \"" + styleElement->name.value + L"\".");
				}
			}
			return context;
		}

		Ptr<parsing::xml::XmlDocument> GuiInstanceStyleContext::SaveToXml()
		{
			auto xmlStyles = MakePtr<XmlElement>();
			xmlStyles->name.value = L"Styles";

			FOREACH(Ptr<GuiInstanceStyle>, style, styles)
			{
				xmlStyles->subNodes.Add(style->SaveToXml());
			}

			auto doc = MakePtr<XmlDocument>();
			doc->rootElement = xmlStyles;
			return doc;
		}
	}
}