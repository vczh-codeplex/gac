/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Reflection: Basic

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_REFLECTION_GUIREFLECTIOTEMPLATES
#define VCZH_PRESENTATION_REFLECTION_GUIREFLECTIOTEMPLATES

#include "GuiReflectionControls.h"

namespace vl
{
	namespace reflection
	{
		namespace description
		{

#ifndef VCZH_DEBUG_NO_REFLECTION

/***********************************************************************
Type List
***********************************************************************/

#define GUIREFLECTIONTEMPLATES_TYPELIST(F)\
			F(presentation::templates::GuiTemplate)\
			F(presentation::templates::GuiTemplate::IFactory)\
			F(presentation::templates::GuiListItemTemplate)\

			GUIREFLECTIONTEMPLATES_TYPELIST(DECL_TYPE_INFO)

/***********************************************************************
Interface Proxy
***********************************************************************/

#pragma warning(push)
#pragma warning(disable:4250)
			namespace interface_proxy
			{
				class GuiTemplate_IFactory : public ValueInterfaceRoot, public virtual GuiTemplate::IFactory
				{
				public:
					GuiTemplate_IFactory(Ptr<IValueInterfaceProxy> _proxy)
						:ValueInterfaceRoot(_proxy)
					{
					}

					static Ptr<GuiTemplate::IFactory> Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new GuiTemplate_IFactory(proxy);
					}

					GuiTemplate* CreateTemplate(const Value& viewModel)override
					{
						return INVOKEGET_INTERFACE_PROXY(CreateTemplate, viewModel);
					}
				};
			}
#pragma warning(pop)

/***********************************************************************
Type Loader
***********************************************************************/

#endif

			extern bool						LoadGuiTemplateTypes();
		}
	}
}

#endif