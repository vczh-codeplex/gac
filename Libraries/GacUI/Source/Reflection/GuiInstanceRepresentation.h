/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
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
		public:
			struct NamespacePattern
			{
				WString								prefix;
				WString								postfix;
			};
			typedef collections::List<NamespacePattern>					NamespacePatternList;

			struct NamespaceInfo
			{
				bool								isDefault;
				WString								name;
				NamespacePatternList				patterns;
			};
			typedef collections::Group<WString, Ptr<NamespaceInfo>>		NamespaceGroup;

			struct Reference
			{
				bool								isFolder;
				WString								protocol;
				WString								path;
			};
			typedef collections::List<Reference>						ReferenceList;
		public:
			Ptr<GuiInstanceRepr>					instance;
			NamespaceGroup							namespaces;
			ReferenceList							references;
			WString									referenceNamespace;
			WString									typeName;

			static Ptr<GuiInstanceContext>			LoadFromXml(Ptr<parsing::xml::XmlDocument> xml, Ptr<GuiResourcePathResolver> resolver);
		};
	}
}

#endif