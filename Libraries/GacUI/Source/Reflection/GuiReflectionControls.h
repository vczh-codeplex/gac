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
			F(presentation::controls::GuiApplication)\
			F(presentation::controls::GuiLabel)\
			F(presentation::controls::GuiLabel::IStyleController)\
			F(presentation::controls::GuiButton)\
			F(presentation::controls::GuiButton::ControlState)\
			F(presentation::controls::GuiButton::IStyleController)\
			F(presentation::controls::GuiSelectableButton)\
			F(presentation::controls::GuiSelectableButton::IStyleController)\
			F(presentation::controls::GuiSelectableButton::GroupController)\
			F(presentation::controls::GuiSelectableButton::MutexGroupController)\
			F(presentation::controls::GuiScroll)\
			F(presentation::controls::GuiScroll::ICommandExecutor)\
			F(presentation::controls::GuiScroll::IStyleController)\
			F(presentation::controls::GuiTabPage)\
			F(presentation::controls::GuiTab)\
			F(presentation::controls::GuiTab::ICommandExecutor)\
			F(presentation::controls::GuiTab::IStyleController)\
			F(presentation::controls::GuiScrollView)\
			F(presentation::controls::GuiScrollView::IStyleProvider)\
			F(presentation::controls::GuiScrollContainer)\
			F(presentation::controls::GuiWindow)\
			F(presentation::controls::GuiWindow::IStyleController)\
			F(presentation::controls::GuiPopup)\
			F(presentation::controls::GuiTooltip)\

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

				class GuiTab_IStyleController : public virtual GuiControl_IStyleController, public virtual GuiTab::IStyleController
				{
				public:
					GuiTab_IStyleController(Ptr<IValueInterfaceProxy> _proxy)
						:GuiControl_IStyleController(_proxy)
					{
					}

					static GuiTab::IStyleController* Create(Ptr<IValueInterfaceProxy> _proxy)
					{
						return new GuiTab_IStyleController(_proxy);
					}

					void SetCommandExecutor(GuiTab::ICommandExecutor* value)override
					{
						INVOKE_INTERFACE_PROXY(SetCommandExecutor, value);
					}

					void InsertTab(vint index)override
					{
						INVOKE_INTERFACE_PROXY(InsertTab, index);
					}

					void SetTabText(vint index, const WString& value)override
					{
						INVOKE_INTERFACE_PROXY(SetTabText, index, value);
					}

					void RemoveTab(vint index)override
					{
						INVOKE_INTERFACE_PROXY(RemoveTab, index);
					}

					void MoveTab(vint oldIndex, vint newIndex)override
					{
						INVOKE_INTERFACE_PROXY(MoveTab, oldIndex, newIndex);
					}

					void SetSelectedTab(vint index)override
					{
						INVOKE_INTERFACE_PROXY(SetSelectedTab, index);
					}

					GuiControl::IStyleController* CreateTabPageStyleController()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GuiControl::IStyleController*, CreateTabPageStyleController);
					}
				};

				class GuiScrollView_IStyleProvider : public virtual GuiControl_IStyleProvider, public virtual GuiScrollView::IStyleProvider
				{
				public:
					GuiScrollView_IStyleProvider(Ptr<IValueInterfaceProxy> _proxy)
						:GuiControl_IStyleProvider(_proxy)
					{
					}

					static GuiScrollView::IStyleProvider* Create(Ptr<IValueInterfaceProxy> _proxy)
					{
						return new GuiScrollView_IStyleProvider(_proxy);
					}

					GuiScroll::IStyleController* CreateHorizontalScrollStyle()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GuiScroll::IStyleController*, CreateHorizontalScrollStyle);
					}

					GuiScroll::IStyleController* CreateVerticalScrollStyle()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GuiScroll::IStyleController*, CreateVerticalScrollStyle);
					}

					vint GetDefaultScrollSize()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(vint, GetDefaultScrollSize);
					}

					compositions::GuiGraphicsComposition* InstallBackground(compositions::GuiBoundsComposition* boundsComposition)override
					{
						return INVOKEGET_INTERFACE_PROXY(GuiGraphicsComposition*, InstallBackground, boundsComposition);
					}
				};

				class GuiWindow_IStyleController : public virtual GuiControl_IStyleController, public virtual GuiWindow::IStyleController
				{
				public:
					GuiWindow_IStyleController(Ptr<IValueInterfaceProxy> _proxy)
						:GuiControl_IStyleController(_proxy)
					{
					}

					static GuiWindow::IStyleController* Create(Ptr<IValueInterfaceProxy> _proxy)
					{
						return new GuiWindow_IStyleController(_proxy);
					}

					void AttachWindow(GuiWindow* _window)override
					{
						INVOKE_INTERFACE_PROXY(ActiveWindow, _window);
					}

					void InitializeNativeWindowProperties()override
					{
						INVOKE_INTERFACE_PROXY_NOPARAM(InitializeNativeWindowProperties);
					}

					bool GetMaximizedBox()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(bool, GetMaximizedBox);
					}

					void SetMaximizedBox(bool visible)override
					{
						INVOKE_INTERFACE_PROXY(SetMaximizedBox, visible);
					}

					bool GetMinimizedBox()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(bool, GetMinimizedBox);
					}

					void SetMinimizedBox(bool visible)override
					{
						INVOKE_INTERFACE_PROXY(SetMinimizedBox, visible);
					}

					bool GetBorder()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(bool, GetBorder);
					}

					void SetBorder(bool visible)override
					{
						INVOKE_INTERFACE_PROXY(SetBorder, visible);
					}

					bool GetSizeBox()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(bool, GetSizeBox);
					}

					void SetSizeBox(bool visible)override
					{
						INVOKE_INTERFACE_PROXY(SetSizeBox, visible);
					}

					bool GetIconVisible()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(bool, GetIconVisible);
					}

					void SetIconVisible(bool visible)override
					{
						INVOKE_INTERFACE_PROXY(SetIconVisible, visible);
					}

					bool GetTitleBar()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(bool, GetTitleBar);
					}

					void SetTitleBar(bool visible)override
					{
						INVOKE_INTERFACE_PROXY(SetTitleBar, visible);
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