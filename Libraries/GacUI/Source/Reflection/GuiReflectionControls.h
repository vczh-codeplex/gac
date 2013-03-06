/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Reflection: Basic

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_REFLECTION_GUIREFLECTIONCONTROLS
#define VCZH_PRESENTATION_REFLECTION_GUIREFLECTIONCONTROLS

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

#define GUIREFLECTIONCONTROLS_TYPELIST(F)\
			F(controls::GuiLabel)\
			F(controls::GuiLabel::IStyleController)\
			F(controls::GuiButton)\
			F(controls::GuiButton::ControlState)\
			F(controls::GuiButton::IStyleController)\
			F(controls::GuiSelectableButton)\
			F(controls::GuiSelectableButton::IStyleController)\
			F(controls::GuiSelectableButton::GroupController)\
			F(controls::GuiSelectableButton::MutexGroupController)\
			F(controls::GuiScroll)\
			F(controls::GuiScroll::ICommandExecutor)\
			F(controls::GuiScroll::IStyleController)\

			GUIREFLECTIONCONTROLS_TYPELIST(DECL_TYPE_INFO)

/***********************************************************************
Interface Proxy
***********************************************************************/

#pragma warning(push)
#pragma warning(disable:4250)
			namespace interface_proxy
			{
				class GuiLabel_IStyleController : public virtual GuiControl_IStyleController, public virtual GuiLabel::IStyleController
				{
				public:
					GuiLabel_IStyleController(Ptr<IValueInterfaceProxy> _proxy)
						:GuiControl_IStyleController(_proxy)
					{
					}

					static GuiLabel::IStyleController* Create(Ptr<IValueInterfaceProxy> _proxy)
					{
						return new GuiLabel_IStyleController(_proxy);
					}

					Color GetDefaultTextColor()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(Color, GetDefaultTextColor);
					}

					void SetTextColor(Color value)override
					{
						INVOKE_INTERFACE_PROXY(SetTextColor, value);
					}
				};

				class GuiButton_IStyleController : public virtual GuiControl_IStyleController, public virtual GuiButton::IStyleController
				{
				public:
					GuiButton_IStyleController(Ptr<IValueInterfaceProxy> _proxy)
						:GuiControl_IStyleController(_proxy)
					{
					}

					static GuiButton::IStyleController* Create(Ptr<IValueInterfaceProxy> _proxy)
					{
						return new GuiButton_IStyleController(_proxy);
					}

					void Transfer(GuiButton::ControlState value)override
					{
						INVOKE_INTERFACE_PROXY(Transfer, value);
					}
				};

				class GuiSelectableButton_IStyleController : public virtual GuiButton_IStyleController, public virtual GuiSelectableButton::IStyleController
				{
				public:
					GuiSelectableButton_IStyleController(Ptr<IValueInterfaceProxy> _proxy)
						:GuiControl_IStyleController(_proxy)
						,GuiButton_IStyleController(_proxy)
					{
					}

					static GuiSelectableButton::IStyleController* Create(Ptr<IValueInterfaceProxy> _proxy)
					{
						return new GuiSelectableButton_IStyleController(_proxy);
					}

					void SetSelected(bool value)override
					{
						INVOKE_INTERFACE_PROXY(SetSelected, value);
					}
				};

				class GuiScroll_IStyleController : public virtual GuiControl_IStyleController, public virtual GuiScroll::IStyleController
				{
				public:
					GuiScroll_IStyleController(Ptr<IValueInterfaceProxy> _proxy)
						:GuiControl_IStyleController(_proxy)
					{
					}

					static GuiScroll::IStyleController* Create(Ptr<IValueInterfaceProxy> _proxy)
					{
						return new GuiScroll_IStyleController(_proxy);
					}

					void SetCommandExecutor(GuiScroll::ICommandExecutor* value)override
					{
						INVOKE_INTERFACE_PROXY(SetCommandExecutor, value);
					}

					void SetTotalSize(vint value)override
					{
						INVOKE_INTERFACE_PROXY(SetTotalSize, value);
					}

					void SetPageSize(vint value)override
					{
						INVOKE_INTERFACE_PROXY(SetPageSize, value);
					}

					void SetPosition(vint value)override
					{
						INVOKE_INTERFACE_PROXY(SetPosition, value);
					}
				};
			}
#pragma warning(pop)

/***********************************************************************
Type Loader
***********************************************************************/

			extern bool						LoadGuiControlsTypes();
		}
	}
}

#endif