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

		void GuiInstanceContext::FillAttSetter(Ptr<GuiAttSetterRepr> setter, Ptr<parsing::xml::XmlElement> xml, Ptr<GuiResourcePathResolver> resolver)
		{
		}

		Ptr<GuiConstructorRepr> GuiInstanceContext::LoadCtor(Ptr<parsing::xml::XmlElement> xml, Ptr<GuiResourcePathResolver> resolver)
		{
			if(auto parser=GetParserManager()->GetParser<ElementName>(L"INSTANCE-ELEMENT-NAME"))
			if(auto name=parser->TypedParse(xml->name.value))
			if(name->IsCtorName())
			{
				Ptr<GuiConstructorRepr> ctor=new GuiConstructorRepr;
				ctor->typeNamespace=name->namespaceName;
				ctor->typeName=name->name;
				FillAttSetter(ctor, xml, resolver);
				return ctor;
			}
			return 0;
		}

		Ptr<GuiInstanceContext> GuiInstanceContext::LoadFromXml(Ptr<parsing::xml::XmlDocument> xml, Ptr<GuiResourcePathResolver> resolver)
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
				FOREACH(Ptr<XmlAttribute>, att, xml->rootElement->attributes)
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
					context->instance=LoadCtor(element, resolver);
				}
			}

			return context->instance?context:0;
		}
	}
}