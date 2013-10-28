/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GacUI Reflection: InstanceLoader

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_REFLECTION_GUIINSTANCEREPRESENTATION
#define VCZH_PRESENTATION_REFLECTION_GUIINSTANCEREPRESENTATION

#include "..\Resources\GuiResource.h"

namespace vl
{
	namespace presentation
	{

/***********************************************************************
Instance Context
***********************************************************************/

		class GuiResourceRepr;
		class GuiAttSetterRepr;
		class GuiInstanceRepr;

		class GuiValueRepr : public Object, public Description<GuiValueRepr>
		{
		public:
			class IVisitor : public IDescriptable, public Description<IVisitor>
			{
			public:
				virtual void						Visit(GuiResourceRepr* description)=0;
				virtual void						Visit(GuiAttSetterRepr* description)=0;
				virtual void						Visit(GuiInstanceRepr* description)=0;
			};

			virtual void							Accept(IVisitor* visitor)=0;
		};

		class GuiResourceRepr : public GuiValueRepr, public Description<GuiResourceRepr>
		{
		public:
			Ptr<Object>								resource;

			void									Accept(IVisitor* visitor)override{visitor->Visit(this);}
		};

		class GuiAttSetterRepr : public GuiValueRepr, public Description<GuiAttSetterRepr>
		{
		public:
			typedef collections::List<Ptr<GuiValueRepr>>				ValueList;

			struct Setter
			{
				WString								binding;
				ValueList							values;
			};

			typedef collections::Dictionary<WString, Ptr<Setter>>		SetterMap;
		public:
			SetterMap								setters;
			ValueList								children;

			void									Accept(IVisitor* visitor)override{visitor->Visit(this);}
		};

		class GuiInstanceRepr : public GuiAttSetterRepr, public Description<GuiInstanceRepr>
		{
		public:
			WString									typeNamespace;
			WString									typeName;

			void									Accept(IVisitor* visitor)override{visitor->Visit(this);}
		};

		class GuiInstanceContext : public Object, public Description<GuiInstanceContext>
		{
			typedef collections::Group<WString, WString>			NamespaceGroup;
		public:
			Ptr<GuiInstanceRepr>					instance;
			NamespaceGroup							namespaces;

			static Ptr<GuiInstanceContext>			LoadFromXml(Ptr<parsing::xml::XmlDocument> xml);
		};
	}
}

#endif