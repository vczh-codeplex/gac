/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Reflection: InstanceLoader

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_REFLECTION_GUIREFLECTIONINSTANCELOADER
#define VCZH_PRESENTATION_REFLECTION_GUIREFLECTIONINSTANCELOADER

#include "..\Controls\GuiApplication.h"
#include "..\Controls\Styles\GuiThemeStyleFactory.h"

namespace vl
{
	namespace presentation
	{
		using namespace reflection;

/***********************************************************************
Instance Context
***********************************************************************/

		class GuiTextDescription;
		class GuiObjectDescription;
		class GuiInstanceDescription;

		class GuiValueDescription : public Object, public Description<GuiValueDescription>
		{
		public:
			class IVisitor : public IDescriptable, public Description<IVisitor>
			{
			public:
				virtual void						Visit(GuiTextDescription* description)=0;
				virtual void						Visit(GuiObjectDescription* description)=0;
				virtual void						Visit(GuiInstanceDescription* description)=0;
			};

			virtual void							Accept(IVisitor* visitor)=0;
		};

		class GuiTextDescription : public GuiValueDescription, public Description<GuiTextDescription>
		{
		public:
			WString									binder;
			WString									text;

			void									Accept(IVisitor* visitor)override{visitor->Visit(this);}
		};

		class GuiObjectDescription : public GuiValueDescription, public Description<GuiObjectDescription>
		{
			typedef collections::Dictionary<WString, Ptr<GuiValueDescription>>			PropertyValueMap;
			typedef collections::List<Ptr<GuiInstanceDescription>>						SubInstanceList;
		public:
			PropertyValueMap						PropertyValues;
			SubInstanceList							SubInstances;

			void									Accept(IVisitor* visitor)override{visitor->Visit(this);}
		};

		class GuiInstanceDescription : public GuiObjectDescription, public Description<GuiInstanceDescription>
		{
		public:
			WString									typeName;

			void									Accept(IVisitor* visitor)override{visitor->Visit(this);}
		};

		class GuiInstanceContext : public Object, public Description<GuiInstanceContext>
		{
		public:
			Ptr<GuiInstanceDescription>				instance;

			static Ptr<GuiInstanceContext>			LoadFromXml(Ptr<parsing::xml::XmlDocument> xml);
		};

/***********************************************************************
Instance Binder
***********************************************************************/

		class IGuiInstanceBinder : public IDescriptable, public Description<IGuiInstanceBinder>
		{
		public:
		};

/***********************************************************************
Instance Loader
***********************************************************************/

		class IGuiInstanceLoader : public IDescriptable, public Description<IGuiInstanceLoader>
		{
		public:
		};

/***********************************************************************
Instance Loader Manager
***********************************************************************/

		class IGuiInstanceLoaderManager : public IDescriptable, public Description<IGuiInstanceLoaderManager>
		{
		public:
			virtual bool							AddInstanceBinder(Ptr<IGuiInstanceBinder> binder)=0;
			virtual IGuiInstanceBinder*				GetInstanceBinder(const WString& name)=0;
			virtual bool							SetLoaderForType(description::ITypeDescriptor* typeDescriptor, Ptr<IGuiInstanceLoader> loader)=0;
			virtual IGuiInstanceLoader*				GetLoaderFromType(description::ITypeDescriptor* typeDescriptor)=0;
			virtual IGuiInstanceLoader*				GetLoaderFromContext(Ptr<GuiInstanceContext> context)=0;
			virtual DescriptableObject*				LoadObject(Ptr<GuiInstanceContext> context)=0;
			virtual bool							LoadObject(DescriptableObject* createdInstance, Ptr<GuiInstanceContext> context)=0;
		};

		extern IGuiInstanceLoaderManager*			GetInstanceLoaderManager();
	}
}

#endif