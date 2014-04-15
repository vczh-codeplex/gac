#include "GuiInstanceSchemaRepresentation.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;
		using namespace parsing::xml;

/***********************************************************************
GuiInstanceDataSchema
***********************************************************************/

		Ptr<GuiInstanceDataSchema> GuiInstanceDataSchema::LoadFromXml(Ptr<parsing::xml::XmlElement> xml, collections::List<WString>& errors)
		{
			auto schema = MakePtr<GuiInstanceDataSchema>();
			return schema;
		}

/***********************************************************************
GuiInstanceSchema
***********************************************************************/

		Ptr<GuiInstanceInterfaceSchema> GuiInstanceInterfaceSchema::LoadFromXml(Ptr<parsing::xml::XmlElement> xml, collections::List<WString>& errors)
		{
			auto schema = MakePtr<GuiInstanceInterfaceSchema>();
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