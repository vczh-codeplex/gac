/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Reflection: Instance Representation

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

		class GuiTextRepr;
		class GuiAttSetterRepr;
		class GuiConstructorRepr;

		class GuiValueRepr : public Object, public Description<GuiValueRepr>
		{
		public:
			class IVisitor : public IDescriptable, public Description<IVisitor>
			{
			public:
				virtual void						Visit(GuiTextRepr* repr)=0;
				virtual void						Visit(GuiAttSetterRepr* repr)=0;
				virtual void						Visit(GuiConstructorRepr* repr)=0;
			};

			virtual void							Accept(IVisitor* visitor)=0;
		};

		class GuiTextRepr : public GuiValueRepr, public Description<GuiTextRepr>
		{
		public:
			WString									text;

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
			typedef collections::Dictionary<WString, WString>					EventHandlerMap;
		public:
			SetteValuerMap							setters;					// empty key means default property
			EventHandlerMap							eventHandlers;

			void									Accept(IVisitor* visitor)override{visitor->Visit(this);}
		};

		class GuiConstructorRepr : public GuiAttSetterRepr, public Description<GuiConstructorRepr>
		{
			typedef collections::Dictionary<WString, WString>					ReferenceAttrubuteMap;
		public:
			WString									typeNamespace;
			WString									typeName;
			Nullable<WString>						instanceName;

			void									Accept(IVisitor* visitor)override{visitor->Visit(this);}
		};

/***********************************************************************
Instance Namespace
***********************************************************************/

		class GuiInstanceNamespace : public Object, public Description<GuiInstanceNamespace>
		{
		public:
			WString									prefix;
			WString									postfix;
		};

/***********************************************************************
Instance Context
***********************************************************************/

		class GuiInstanceContext : public Object, public Description<GuiInstanceContext>
		{
		public:
			typedef collections::List<Ptr<GuiInstanceNamespace>>				NamespaceList;

			struct NamespaceInfo
			{
				WString								name;
				NamespaceList						namespaces;
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
				bool IsReferenceAttributeName(){ return namespaceName==L"" && category==L"ref" && name!=L"" && binding==L""; }
				bool IsPropertyAttributeName(){ return namespaceName==L"" && category==L"" && name!=L""; }
				bool IsPropertyElementName(){ return namespaceName==L"" && category==L"att" && name!=L""; }
				bool IsEventAttributeName(){ return namespaceName==L"" && category==L"ev" && name!=L"" && binding==L""; }
				bool IsEventElementName(){ return namespaceName==L"" && category==L"ev" && name!=L"" && binding==L""; }
			};
		public:
			Ptr<GuiConstructorRepr>					instance;
			NamespaceMap							namespaces;
			Nullable<WString>						className;

			static void								CollectDefaultAttributes(GuiAttSetterRepr::ValueList& values, Ptr<parsing::xml::XmlElement> xml);
			static void								CollectAttributes(GuiAttSetterRepr::SetteValuerMap& setters, Ptr<parsing::xml::XmlElement> xml);
			static void								CollectEvents(GuiAttSetterRepr::EventHandlerMap& eventHandlers, Ptr<parsing::xml::XmlElement> xml);
			static void								FillAttSetter(Ptr<GuiAttSetterRepr> setter, Ptr<parsing::xml::XmlElement> xml);
			static Ptr<GuiConstructorRepr>			LoadCtor(Ptr<parsing::xml::XmlElement> xml);
			static Ptr<GuiInstanceContext>			LoadFromXml(Ptr<parsing::xml::XmlDocument> xml);
		};
	}
}

#endif