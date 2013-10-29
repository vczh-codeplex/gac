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
Instance Representation
***********************************************************************/

		class GuiResourceRepr;
		class GuiAttSetterRepr;
		class GuiConstructorRepr;

		class GuiValueRepr : public Object, public Description<GuiValueRepr>
		{
		public:
			class IVisitor : public IDescriptable, public Description<IVisitor>
			{
			public:
				virtual void						Visit(GuiResourceRepr* repr)=0;
				virtual void						Visit(GuiAttSetterRepr* repr)=0;
				virtual void						Visit(GuiConstructorRepr* repr)=0;
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
			typedef collections::List<Ptr<GuiValueRepr>>						ValueList;

			struct SetterValue
			{
				WString								binding;
				ValueList							values;
			};

			typedef collections::Dictionary<WString, Ptr<SetterValue>>			SetteValuerMap;
		public:
			SetteValuerMap							setters;					// empty key means default property

			void									Accept(IVisitor* visitor)override{visitor->Visit(this);}
		};

		class GuiConstructorRepr : public GuiAttSetterRepr, public Description<GuiConstructorRepr>
		{
		public:
			WString									typeNamespace;
			WString									typeName;

			void									Accept(IVisitor* visitor)override{visitor->Visit(this);}
		};

/***********************************************************************
Instance Namespace
***********************************************************************/

		class GuiInstanceResourcePattern;
		class GuiInstanceNamePattern;

		class GuiInstancePattern : public Object, public Description<GuiInstancePattern>
		{
		public:
			class IVisitor : public IDescriptable, public Description<IVisitor>
			{
			public:
				virtual void						Visit(GuiInstanceResourcePattern* ns)=0;
				virtual void						Visit(GuiInstanceNamePattern* ns)=0;
			};

			virtual void							Accept(IVisitor* visitor)=0;
		};

		class GuiInstanceResourcePattern : public GuiInstancePattern, public Description<GuiInstanceResourcePattern>
		{
		public:
			bool									isFolder;
			WString									protocol;
			WString									path;

			void									Accept(IVisitor* visitor)override{visitor->Visit(this);}
		};

		class GuiInstanceNamePattern : public GuiInstancePattern, public Description<GuiInstanceNamePattern>
		{
		public:
			WString									prefix;
			WString									postfix;

			void									Accept(IVisitor* visitor)override{visitor->Visit(this);}
		};

/***********************************************************************
Instance Context
***********************************************************************/

		class GuiInstanceContext : public Object, public Description<GuiInstanceContext>
		{
		public:
			typedef collections::List<Ptr<GuiInstancePattern>>					NamespacePatternList;

			struct NamespaceInfo
			{
				WString								name;
				NamespacePatternList				patterns;
			};
			typedef collections::Dictionary<WString, Ptr<NamespaceInfo>>		NamespaceMap;

			class ElementName : public Object
			{
			public:
				WString								namespaceName;				// empty key means default namespace
				WString								category;
				WString								name;
				WString								binding;

				bool IsCtorName(){ return category==L"" && name!=L"" && binding==L""; }
				bool IsPropertyAttributeName(){ return namespaceName==L"" && category==L"" && name!=L""; }
				bool IsPropertyElementName(){ return namespaceName==L"" && category==L"att" && name!=L""; }
			};
		public:
			Ptr<GuiConstructorRepr>					instance;
			NamespaceMap							namespaces;
			WString									typeName;

			static void								CollectValues(collections::Dictionary<WString, Ptr<GuiAttSetterRepr::SetterValue>>& setters, Ptr<parsing::xml::XmlElement> xml, Ptr<GuiResourcePathResolver> resolver);
			static void								FillAttSetter(Ptr<GuiAttSetterRepr> setter, Ptr<parsing::xml::XmlElement> xml, Ptr<GuiResourcePathResolver> resolver);
			static Ptr<GuiConstructorRepr>			LoadCtor(Ptr<parsing::xml::XmlElement> xml, Ptr<GuiResourcePathResolver> resolver);
			static Ptr<GuiInstanceContext>			LoadFromXml(Ptr<parsing::xml::XmlDocument> xml, Ptr<GuiResourcePathResolver> resolver);
		};
	}
}

#endif