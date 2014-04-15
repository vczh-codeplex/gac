/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Reflection: Instance Schema Representation

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_REFLECTION_GUIINSTANCESCHEMAREPRESENTATION
#define VCZH_PRESENTATION_REFLECTION_GUIINSTANCESCHEMAREPRESENTATION

#include "..\Resources\GuiResource.h"

namespace vl
{
	namespace presentation
	{
		class GuiInstanceTypeSchema : public Object, public Description<GuiInstanceTypeSchema>
		{
		public:
		};

/***********************************************************************
Instance Struct/Class Schema
***********************************************************************/

		class GuiInstanceDataSchema : public GuiInstanceTypeSchema, public Description<GuiInstanceDataSchema>
		{
		public:
			static Ptr<GuiInstanceDataSchema>			LoadFromXml(Ptr<parsing::xml::XmlElement> xml, collections::List<WString>& errors);
		};

/***********************************************************************
Instance Interface Schema
***********************************************************************/

		class GuiInstanceInterfaceSchema : public GuiInstanceTypeSchema, public Description<GuiInstanceInterfaceSchema>
		{
		public:
			static Ptr<GuiInstanceInterfaceSchema>		LoadFromXml(Ptr<parsing::xml::XmlElement> xml, collections::List<WString>& errors);
		};

/***********************************************************************
Instance Schema Representation
***********************************************************************/

		class GuiInstanceSchema : public Object, public Description<GuiInstanceSchema>
		{
			typedef collections::List<Ptr<GuiInstanceTypeSchema>>		TypeSchemaList;
		public:
			TypeSchemaList								schemas;

			static Ptr<GuiInstanceSchema>				LoadFromXml(Ptr<parsing::xml::XmlDocument> xml, collections::List<WString>& errors);
		};
	}
}

#endif