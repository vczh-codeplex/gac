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
#include "GuiInstanceRepresentation.h"

namespace vl
{
	namespace presentation
	{
		using namespace reflection;

/***********************************************************************
Instance Binder
***********************************************************************/

		class IGuiInstanceBinder : public IDescriptable, public Description<IGuiInstanceBinder>
		{
		public:
			virtual WString							GetBindingName()=0;
		};

/***********************************************************************
Instance Loader
***********************************************************************/

		class IGuiInstanceLoader : public IDescriptable, public Description<IGuiInstanceLoader>
		{
		public:
			virtual WString							GetTypeName()=0;
		};

/***********************************************************************
Instance Loader Manager
***********************************************************************/

		class IGuiInstanceLoaderManager : public IDescriptable, public Description<IGuiInstanceLoaderManager>
		{
		public:
			virtual bool							AddInstanceBinder(Ptr<IGuiInstanceBinder> binder)=0;
			virtual IGuiInstanceBinder*				GetInstanceBinder(const WString& bindingName)=0;
			virtual bool							CreateVirtualType(const WString& typeName, const WString& parentType, Ptr<IGuiInstanceLoader> loader)=0;
			virtual bool							SetLoader(Ptr<IGuiInstanceLoader> loader)=0;
			virtual IGuiInstanceLoader*				GetLoader(const WString& typeName)=0;
			virtual IGuiInstanceLoader*				GetParentLoader(IGuiInstanceLoader* loader)=0;
			virtual DescriptableObject*				LoadObject(Ptr<GuiInstanceContext> context, Ptr<GuiResourcePathResolver> resolver)=0;
			virtual bool							LoadObject(DescriptableObject* createdInstance, Ptr<GuiInstanceContext> context, Ptr<GuiResourcePathResolver> resolver)=0;
		};

		extern IGuiInstanceLoaderManager*			GetInstanceLoaderManager();
	}
}

#endif