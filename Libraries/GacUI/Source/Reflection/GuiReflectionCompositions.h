/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Reflection: Compositions

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_REFLECTION_GUIREFLECTIONCOMPOSITIONS
#define VCZH_PRESENTATION_REFLECTION_GUIREFLECTIONCOMPOSITIONS

#include "GuiReflectionBasic.h"

namespace vl
{
	namespace reflection
	{
		namespace description
		{

/***********************************************************************
Type List
***********************************************************************/

#define GUIREFLECTIONCOMPOSITION_TYPELIST(F)\
			F(presentation::compositions::GuiStackComposition)\
			F(presentation::compositions::GuiStackComposition::Direction)\
			F(presentation::compositions::GuiStackItemComposition)\
			F(presentation::compositions::GuiCellOption)\
			F(presentation::compositions::GuiCellOption::ComposeType)\
			F(presentation::compositions::GuiTableComposition)\
			F(presentation::compositions::GuiCellComposition)\
			F(presentation::compositions::GuiSideAlignedComposition)\
			F(presentation::compositions::GuiSideAlignedComposition::Direction)\
			F(presentation::compositions::GuiPartialViewComposition)\
			F(presentation::compositions::GuiSubComponentMeasurer)\
			F(presentation::compositions::GuiSubComponentMeasurer::Direction)\
			F(presentation::compositions::GuiSubComponentMeasurer::IMeasuringSource)\
			F(presentation::compositions::GuiSubComponentMeasurer::MeasuringSource)\

			GUIREFLECTIONCOMPOSITION_TYPELIST(DECL_TYPE_INFO)

/***********************************************************************
Interface Proxy
***********************************************************************/

#pragma warning(push)
#pragma warning(disable:4250)
			namespace interface_proxy
			{
				class GuiSubComponentMeasurer_IMeasuringSource : public ValueInterfaceRoot, public virtual GuiSubComponentMeasurer::IMeasuringSource
				{
				public:
					GuiSubComponentMeasurer_IMeasuringSource(Ptr<IValueInterfaceProxy> _proxy)
						:ValueInterfaceRoot(_proxy)
					{
					}

					static Ptr<GuiSubComponentMeasurer::IMeasuringSource> Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new GuiSubComponentMeasurer_IMeasuringSource(proxy);
					}

					void AttachMeasurer(GuiSubComponentMeasurer* value)override
					{
						INVOKE_INTERFACE_PROXY(AttachMeasurer, value);
					}

					void DetachMeasurer(GuiSubComponentMeasurer* value)override
					{
						INVOKE_INTERFACE_PROXY(DetachMeasurer, value);
					}

					GuiSubComponentMeasurer* GetAttachedMeasurer()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetAttachedMeasurer);
					}

					WString GetMeasuringCategory()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetMeasuringCategory);
					}

					vint GetSubComponentCount()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetSubComponentCount);
					}

					WString GetSubComponentName(vint index)override
					{
						return INVOKEGET_INTERFACE_PROXY(GetSubComponentName, index);
					}

					GuiGraphicsComposition* GetSubComponentComposition(vint index)override
					{
						return INVOKEGET_INTERFACE_PROXY(GetSubComponentComposition, index);
					}

					GuiGraphicsComposition* GetSubComponentComposition(const WString& name)override
					{
						return INVOKEGET_INTERFACE_PROXY(GetSubComponentComposition, name);
					}

					GuiGraphicsComposition* GetMainComposition()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetMainComposition);
					}

					void SubComponentPreferredMinSizeUpdated()override
					{
						INVOKE_INTERFACE_PROXY_NOPARAM(SubComponentPreferredMinSizeUpdated);
					}
				};
			}
#pragma warning(pop)

/***********************************************************************
Type Loader
***********************************************************************/

			extern bool						LoadGuiCompositionTypes();
		}
	}
}

#endif