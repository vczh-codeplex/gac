#include "GuiInstanceSchemaRepresentation.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;
		using namespace parsing::xml;

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
			if (auto attParent = XmlGetAttribute(xml, L"Parent"))
			{
				parentType = attParent->value.value;
			}
			FOREACH(Ptr<XmlElement>, memberElement, XmlGetElements(xml, L"Property"))
			{
				auto prop = GuiInstancePropertySchame::LoadFromXml(memberElement, errors);
				properties.Add(prop);
			}
		}

/***********************************************************************
GuiInstanceDataSchema
***********************************************************************/

		Ptr<GuiInstanceDataSchema> GuiInstanceDataSchema::LoadFromXml(Ptr<parsing::xml::XmlElement> xml, collections::List<WString>& errors)
		{
			auto schema = MakePtr<GuiInstanceDataSchema>();
			schema->GuiInstanceTypeSchema::LoadFromXml(xml, errors);
			schema->referenceType = xml->name.value == L"Class";

			if (!schema->referenceType)
			{
				if (schema->parentType != L"")
				{
					errors.Add(
						L"Struct \"" + schema->parentType +
						L"\" should not have a parent type."
						);
				}
			}

			FOREACH(Ptr<GuiInstancePropertySchame>, prop, schema->properties)
			{
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
			}
			return schema;
		}

/***********************************************************************
GuiInstanceInterfaceSchema
***********************************************************************/

		Ptr<GuiInstanceMethodSchema> GuiInstanceMethodSchema::LoadFromXml(Ptr<parsing::xml::XmlElement> xml, collections::List<WString>& errors)
		{
			auto schema = MakePtr<GuiInstanceMethodSchema>();
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
				schema->returnType = attName->value.value;
			}
			else
			{
				errors.Add(L"Missing attribute \"Type\" in <" + xml->name.value + L">.");
			}
			FOREACH(Ptr<XmlElement>, memberElement, XmlGetElements(xml, L"Argument"))
			{
				auto prop = GuiInstancePropertySchame::LoadFromXml(memberElement, errors);
				schema->arguments.Add(prop);
			}
			return schema;
		}

/***********************************************************************
GuiInstanceInterfaceSchema
***********************************************************************/

		Ptr<GuiInstanceInterfaceSchema> GuiInstanceInterfaceSchema::LoadFromXml(Ptr<parsing::xml::XmlElement> xml, collections::List<WString>& errors)
		{
			auto schema = MakePtr<GuiInstanceInterfaceSchema>();
			schema->GuiInstanceTypeSchema::LoadFromXml(xml, errors);
			FOREACH(Ptr<XmlElement>, memberElement, XmlGetElements(xml, L"Method"))
			{
				auto method = GuiInstanceMethodSchema::LoadFromXml(memberElement, errors);
				schema->methods.Add(method);
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