/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
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
			virtual bool							SetLoaderForType(description::ITypeDescriptor* baseType, Ptr<IGuiInstanceLoader> loader)=0;
			virtual bool							SetLoaderForVirtualType(const WString& typeName, description::ITypeDescriptor* baseType, Ptr<IGuiInstanceLoader> loader)=0;
			virtual IGuiInstanceLoader*				GetLoaderFromType(const WString& typeName)=0;
			virtual IGuiInstanceLoader*				GetLoaderForInstance(Ptr<GuiInstanceContext> context, Ptr<GuiInstanceRepr> instance, Ptr<GuiResourcePathResolver> resolver)=0;
			virtual DescriptableObject*				LoadObject(Ptr<GuiInstanceContext> context, Ptr<GuiResourcePathResolver> resolver)=0;
			virtual bool							LoadObject(DescriptableObject* createdInstance, Ptr<GuiInstanceContext> context, Ptr<GuiResourcePathResolver> resolver)=0;
		};

		extern IGuiInstanceLoaderManager*			GetInstanceLoaderManager();
	}
}

#endif