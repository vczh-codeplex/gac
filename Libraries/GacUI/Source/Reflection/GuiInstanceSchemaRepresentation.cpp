#include "GuiInstanceSchemaRepresentation.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;
		using namespace parsing::xml;

/***********************************************************************
GuiInstanceTypeSchema
***********************************************************************/

		void GuiInstanceTypeSchema::LoadFromXml(Ptr<parsing::xml::XmlElement> xml, collections::List<WString>& errors)
		{
			if (auto attName = XmlGetAttribute(xml, L"ref.Class"))
			{
				typeName = attName->value.value;
			}
			else
			{
				errors.Add(L"Missing attribute \"ref.Class\" in <" + xml->name.value + L">.");
			}
		}

/***********************************************************************
GuiInstancePropertySchame
***********************************************************************/

		Ptr<GuiInstancePropertySchame> GuiInstancePropertySchame::LoadFromXml(Ptr<parsing::xml::XmlElement> xml, collections::List<WString>& errors)
		{
			auto schema = MakePtr<GuiInstancePropertySchame>();
			if (auto attName = XmlGetAttribute(xml, L"Name"))
			{
				schema->name = attName->value.value;
			}
			else
			{
				errors.Add(L"Missing attribute \"Name\" in <" + xml->name.value + L">.");
			}
			if (auto attName = XmlGetAttribute(xml, L"Type"))
			{
				schema->typeName = attName->value.value;
			}
			else
			{
				errors.Add(L"Missing attribute \"Type\" in <" + xml->name.value + L">.");
			}
			if (auto attReadonly = XmlGetAttribute(xml, L"Readonly"))
			{
				schema->readonly = attReadonly->value.value == L"true";
			}
			if (auto attObservable = XmlGetAttribute(xml, L"Observable"))
			{
				schema->observable = attObservable->value.value == L"true";
			}
			return schema;
		}

/***********************************************************************
GuiInstanceDataSchema
***********************************************************************/

		Ptr<GuiInstanceDataSchema> GuiInstanceDataSchema::LoadFromXml(Ptr<parsing::xml::XmlElement> xml, collections::List<WString>& errors)
		{
			auto schema = MakePtr<GuiInstanceDataSchema>();
			schema->GuiInstanceTypeSchema::LoadFromXml(xml, errors);
			schema->referenceType = xml->name.value == L"Class";
			FOREACH(Ptr<XmlElement>, memberElement, XmlGetElements(xml))
			{
				if (memberElement->name.value == L"Property")
				{
					auto prop = GuiInstancePropertySchame::LoadFromXml(memberElement, errors);
					if (prop->readonly)
					{
						errors.Add(
							L"Property \"" + prop->name +
							L"\" should not be readonly in data type \"" + schema->typeName +
							L"\"."
							);
					}
					if (prop->observable)
					{
						errors.Add(
							L"Property \"" + prop->name +
							L"\" should not be observable in data type \"" + schema->typeName +
							L"\"."
							);
					}
					schema->properties.Add(prop);
				}
				else
				{
					errors.Add(L"Unknown member type \"" + memberElement->name.value + L"\" in <" + xml->name.value + L">.");
				}
			}
			return schema;
		}

/***********************************************************************
GuiInstanceSchema
***********************************************************************/

		Ptr<GuiInstanceInterfaceSchema> GuiInstanceInterfaceSchema::LoadFromXml(Ptr<parsing::xml::XmlElement> xml, collections::List<WString>& errors)
		{
			auto schema = MakePtr<GuiInstanceInterfaceSchema>();
			schema->GuiInstanceTypeSchema::LoadFromXml(xml, errors);
			FOREACH(Ptr<XmlElement>, memberElement, XmlGetElements(xml))
			{
				if (memberElement->name.value == L"Property")
				{
					auto prop = GuiInstancePropertySchame::LoadFromXml(memberElement, errors);
					schema->properties.Add(prop);
				}
				else
				{
					errors.Add(L"Unknown member type \"" + memberElement->name.value + L"\" in <" + xml->name.value + L">.");
				}
			}
			return schema;
		}

/***********************************************************************
GuiInstanceSchema
***********************************************************************/

		Ptr<GuiInstanceSchema> GuiInstanceSchema::LoadFromXml(Ptr<parsing::xml::XmlDocument> xml, collections::List<WString>& errors)
		{
			auto schema = MakePtr<GuiInstanceSchema>();
			FOREACH(Ptr<XmlElement>, schemaElement, XmlGetElements(xml->rootElement))
			{
				if (schemaElement->name.value == L"Struct" || schemaElement->name.value == L"Class")
				{
					schema->schemas.Add(GuiInstanceDataSchema::LoadFromXml(schemaElement, errors));
				}
				else if (schemaElement->name.value == L"Interface")
				{
					schema->schemas.Add(GuiInstanceInterfaceSchema::LoadFromXml(schemaElement, errors));
				}
				else
				{
					errors.Add(L"Unknown schema type \"" + schemaElement->name.value + L"\".");
				}
			}
			return schema;
		}
	}
}