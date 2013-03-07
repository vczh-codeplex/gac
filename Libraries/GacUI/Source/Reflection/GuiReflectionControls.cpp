#include "GuiReflectionControls.h"

namespace vl
{
	namespace reflection
	{
		namespace description
		{
			using namespace collections;
			using namespace parsing;
			using namespace parsing::tabling;
			using namespace parsing::xml;
			using namespace stream;

			GUIREFLECTIONCONTROLS_TYPELIST(IMPL_TYPE_INFO)

/***********************************************************************
External Functions
***********************************************************************/

			Ptr<IValueReadonlyList> GuiTab_GetPages(GuiTab* thisObject)
			{
				return new ValueReadonlyListWrapper<const List<GuiTabPage*>*>(&thisObject->GetPages());
			}

/***********************************************************************
Type Declaration
***********************************************************************/

#define _ ,

#define CONTROL_CONSTRUCTOR_CONTROLLER(CONTROL)\
	CLASS_MEMBER_CONSTRUCTOR(CONTROL*(CONTROL::IStyleController*), {L"styleController"})

#define CONTROL_CONSTRUCTOR_PROVIDER(CONTROL)\
	CLASS_MEMBER_CONSTRUCTOR(CONTROL*(CONTROL::IStyleProvider*), {L"styleProvider"})

#define INTERFACE_EXTERNALCTOR(CONTROL, INTERFACE)\
	CLASS_MEMBER_EXTERNALCTOR(CONTROL::INTERFACE*(Ptr<IValueInterfaceProxy>), {L"proxy"}, &interface_proxy::CONTROL##_##INTERFACE::Create)

			BEGIN_CLASS_MEMBER(GuiLabel)
				CLASS_MEMBER_BASE(GuiControl)
				CONTROL_CONSTRUCTOR_CONTROLLER(GuiLabel)

				CLASS_MEMBER_PROPERTY_FAST(TextColor)
			END_CLASS_MEMBER(GuiLabel)

			BEGIN_CLASS_MEMBER(GuiLabel::IStyleController)
				CLASS_MEMBER_BASE(GuiControl::IStyleController)
				INTERFACE_EXTERNALCTOR(GuiLabel, IStyleController)

				CLASS_MEMBER_METHOD(GetDefaultTextColor, NO_PARAMETER)
				CLASS_MEMBER_METHOD(SetTextColor, {L"value"})
			END_CLASS_MEMBER(GuiLabel::IStyleController)

			BEGIN_CLASS_MEMBER(GuiButton)
				CLASS_MEMBER_BASE(GuiControl)
				CONTROL_CONSTRUCTOR_CONTROLLER(GuiButton)

				CLASS_MEMBER_PROPERTY_FAST(ClickOnMouseUp)
			END_CLASS_MEMBER(GuiButton)

			BEGIN_ENUM_ITEM(GuiButton::ControlState)
				ENUM_ITEM_NAMESPACE(GuiButton)
				ENUM_NAMESPACE_ITEM(Normal)
				ENUM_NAMESPACE_ITEM(Active)
				ENUM_NAMESPACE_ITEM(Pressed)
			END_ENUM_ITEM(GuiButton::ControlState)

			BEGIN_CLASS_MEMBER(GuiButton::IStyleController)
				CLASS_MEMBER_BASE(GuiControl::IStyleController)
				INTERFACE_EXTERNALCTOR(GuiButton, IStyleController)

				CLASS_MEMBER_METHOD(Transfer, {L"value"})
			END_CLASS_MEMBER(GuiButton::IStyleController)

			BEGIN_CLASS_MEMBER(GuiSelectableButton)
				CLASS_MEMBER_BASE(GuiButton)
				CONTROL_CONSTRUCTOR_CONTROLLER(GuiSelectableButton)

				CLASS_MEMBER_PROPERTY_FAST(GroupController)
				CLASS_MEMBER_PROPERTY_FAST(AutoSelection)
				CLASS_MEMBER_PROPERTY_FAST(Selected)
			END_CLASS_MEMBER(GuiSelectableButton)

			BEGIN_CLASS_MEMBER(GuiSelectableButton::IStyleController)
				CLASS_MEMBER_BASE(GuiButton::IStyleController)
				INTERFACE_EXTERNALCTOR(GuiSelectableButton, IStyleController)

				CLASS_MEMBER_METHOD(SetSelected, {L"value"})
			END_CLASS_MEMBER(GuiSelectableButton::IStyleController)

			BEGIN_CLASS_MEMBER(GuiSelectableButton::GroupController)
				CLASS_MEMBER_BASE(GuiComponent)

				CLASS_MEMBER_METHOD(Attach, {L"button"})
				CLASS_MEMBER_METHOD(Detach, {L"button"})
				CLASS_MEMBER_METHOD(OnSelectedChanged, {L"button"})
			END_CLASS_MEMBER(GuiSelectableButton::GroupController)

			BEGIN_CLASS_MEMBER(GuiSelectableButton::MutexGroupController)
				CLASS_MEMBER_BASE(GuiSelectableButton::GroupController)
				CLASS_MEMBER_CONSTRUCTOR(GuiSelectableButton::MutexGroupController*(), NO_PARAMETER)
			END_CLASS_MEMBER(GuiSelectableButton::MutexGroupController)

			BEGIN_CLASS_MEMBER(GuiScroll)
				CLASS_MEMBER_BASE(GuiControl)
				CONTROL_CONSTRUCTOR_CONTROLLER(GuiScroll)

				CLASS_MEMBER_PROPERTY_FAST(TotalSize)
				CLASS_MEMBER_PROPERTY_FAST(PageSize)
				CLASS_MEMBER_PROPERTY_FAST(Position)
				CLASS_MEMBER_PROPERTY_FAST(SmallMove)
				CLASS_MEMBER_PROPERTY_FAST(BigMove)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(MinPosition)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(MaxPosition)
			END_CLASS_MEMBER(GuiScroll)

			BEGIN_CLASS_MEMBER(GuiScroll::ICommandExecutor)
				CLASS_MEMBER_METHOD(SmallDecrease, NO_PARAMETER)
				CLASS_MEMBER_METHOD(SmallIncrease, NO_PARAMETER)
				CLASS_MEMBER_METHOD(BigDecrease, NO_PARAMETER)
				CLASS_MEMBER_METHOD(BigIncrease, NO_PARAMETER)
				CLASS_MEMBER_METHOD(SetTotalSize, {L"value"})
				CLASS_MEMBER_METHOD(SetPageSize, {L"value"})
				CLASS_MEMBER_METHOD(SetPosition, {L"value"})
			END_CLASS_MEMBER(GuiScroll::ICommandExecutor)

			BEGIN_CLASS_MEMBER(GuiScroll::IStyleController)
				CLASS_MEMBER_BASE(GuiControl::IStyleController)
				INTERFACE_EXTERNALCTOR(GuiScroll, IStyleController)

				CLASS_MEMBER_METHOD(SetCommandExecutor, {L"value"})
				CLASS_MEMBER_METHOD(SetTotalSize, {L"value"})
				CLASS_MEMBER_METHOD(SetPageSize, {L"value"})
				CLASS_MEMBER_METHOD(SetPosition, {L"value"})
			END_CLASS_MEMBER(GuiScroll::IStyleController)

			BEGIN_CLASS_MEMBER(GuiTabPage)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Container)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(OwnerTab)
				CLASS_MEMBER_PROPERTY_FAST(Text)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Selected)
			END_CLASS_MEMBER(GuiTabPage)

			BEGIN_CLASS_MEMBER(GuiTab)
				CLASS_MEMBER_BASE(GuiControl)
				CONTROL_CONSTRUCTOR_CONTROLLER(GuiTab)

				CLASS_MEMBER_PROPERTY_FAST(SelectedPage)

				CLASS_MEMBER_METHOD_OVERLOAD(CreatePage, {L"index"}, GuiTabPage*(GuiTab::*)(vint))
				CLASS_MEMBER_METHOD_OVERLOAD(CreatePage, {L"page" _ L"index"}, bool(GuiTab::*)(GuiTabPage* _ vint))
				CLASS_MEMBER_METHOD(RemovePage, {L"value"})
				CLASS_MEMBER_METHOD(MovePage, {L"page" _ L"newIndex"})
				CLASS_MEMBER_EXTERNALMETHOD(GetPages, NO_PARAMETER, Ptr<IValueReadonlyList>(GuiTab::*)(), &GuiTab_GetPages)
			END_CLASS_MEMBER(GuiTab)

			BEGIN_CLASS_MEMBER(GuiTab::ICommandExecutor)
				CLASS_MEMBER_METHOD(ShowTab, {L"index"})
			END_CLASS_MEMBER(GuiTab::ICommandExecutor)

			BEGIN_CLASS_MEMBER(GuiTab::IStyleController)
				CLASS_MEMBER_BASE(GuiControl::IStyleController)
				INTERFACE_EXTERNALCTOR(GuiTab, IStyleController)

				CLASS_MEMBER_METHOD(SetCommandExecutor, {L"value"})
				CLASS_MEMBER_METHOD(InsertTab, {L"index"})
				CLASS_MEMBER_METHOD(SetTabText, {L"index" _ L"value"})
				CLASS_MEMBER_METHOD(RemoveTab, {L"index"})
				CLASS_MEMBER_METHOD(MoveTab, {L"oldIndex" _ L"newIndex"})
				CLASS_MEMBER_METHOD(SetSelectedTab, {L"index"})
				CLASS_MEMBER_METHOD(CreateTabPageStyleController, NO_PARAMETER)
			END_CLASS_MEMBER(GuiTab::IStyleController)

			BEGIN_CLASS_MEMBER(GuiScrollView)
				CLASS_MEMBER_BASE(GuiControl)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ViewSize)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ViewBounds)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(HorizontalScroll)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(VerticalScroll)
				CLASS_MEMBER_PROPERTY_FAST(HorizontalAlwaysVisible)
				CLASS_MEMBER_PROPERTY_FAST(VerticalAlwaysVisible)

				CLASS_MEMBER_METHOD(CalculateView, NO_PARAMETER)
			END_CLASS_MEMBER(GuiScrollView)

			BEGIN_CLASS_MEMBER(GuiScrollView::IStyleProvider)
				CLASS_MEMBER_BASE(GuiControl::IStyleProvider)
				INTERFACE_EXTERNALCTOR(GuiScrollView, IStyleProvider)

				CLASS_MEMBER_METHOD(CreateHorizontalScrollStyle, NO_PARAMETER)
				CLASS_MEMBER_METHOD(CreateVerticalScrollStyle, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetDefaultScrollSize, NO_PARAMETER)
				CLASS_MEMBER_METHOD(InstallBackground, {L"boundsComposition"})
			END_CLASS_MEMBER(GuiScrollView::IStyleProvider)

			BEGIN_CLASS_MEMBER(GuiScrollContainer)
				CLASS_MEMBER_BASE(GuiScrollView)
				CONTROL_CONSTRUCTOR_PROVIDER(GuiScrollContainer)

				CLASS_MEMBER_PROPERTY_FAST(ExtendToFullWidth)
			END_CLASS_MEMBER(GuiScrollContainer)

			BEGIN_CLASS_MEMBER(GuiWindow)
				CLASS_MEMBER_BASE(GuiControlHost)
				CONTROL_CONSTRUCTOR_CONTROLLER(GuiWindow)

				CLASS_MEMBER_PROPERTY_FAST(MaximizedBox)
				CLASS_MEMBER_PROPERTY_FAST(MinimizedBox)
				CLASS_MEMBER_PROPERTY_FAST(Border)
				CLASS_MEMBER_PROPERTY_FAST(SizeBox)
				CLASS_MEMBER_PROPERTY_FAST(IconVisible)
				CLASS_MEMBER_PROPERTY_FAST(TitleBar)

				CLASS_MEMBER_METHOD(MoveToScreenCenter, NO_PARAMETER)
			END_CLASS_MEMBER(GuiWindow)

			BEGIN_CLASS_MEMBER(GuiWindow::IStyleController)
				CLASS_MEMBER_BASE(GuiControl::IStyleController)
				INTERFACE_EXTERNALCTOR(GuiWindow, IStyleController)

				CLASS_MEMBER_PROPERTY_FAST(MaximizedBox)
				CLASS_MEMBER_PROPERTY_FAST(MinimizedBox)
				CLASS_MEMBER_PROPERTY_FAST(Border)
				CLASS_MEMBER_PROPERTY_FAST(SizeBox)
				CLASS_MEMBER_PROPERTY_FAST(IconVisible)
				CLASS_MEMBER_PROPERTY_FAST(TitleBar)
			END_CLASS_MEMBER(GuiWindow::IStyleController)

			BEGIN_CLASS_MEMBER(GuiPopup)
				CLASS_MEMBER_BASE(GuiWindow)
				CONTROL_CONSTRUCTOR_CONTROLLER(GuiPopup)

				CLASS_MEMBER_METHOD(IsClippedByScreen, {L"location"})
				CLASS_MEMBER_METHOD_OVERLOAD(ShowPopup, {L"location"}, void(GuiPopup::*)(Point))
				CLASS_MEMBER_METHOD_OVERLOAD(ShowPopup, {L"control" _ L"location"}, void(GuiPopup::*)(GuiControl* _ Point))
				CLASS_MEMBER_METHOD_OVERLOAD(ShowPopup, {L"control" _ L"preferredTopBottomSide"}, void(GuiPopup::*)(GuiControl* _ bool))
			END_CLASS_MEMBER(GuiPopup)

			BEGIN_CLASS_MEMBER(GuiTooltip)
				CLASS_MEMBER_BASE(GuiPopup)
				CONTROL_CONSTRUCTOR_CONTROLLER(GuiPopup)
				
				CLASS_MEMBER_PROPERTY_FAST(PreferredContentWidth)
				CLASS_MEMBER_PROPERTY_FAST(TemporaryContentControl)
			END_CLASS_MEMBER(GuiTooltip)

#undef CONTROL_CONSTRUCTOR_CONTROLLER
#undef INTERFACE_EXTERNALCTOR
#undef _

/***********************************************************************
Type Loader
***********************************************************************/

			class GuiControlsTypeLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					GUIREFLECTIONCONTROLS_TYPELIST(ADD_TYPE_INFO)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};

			bool LoadGuiControlsTypes()
			{
				ITypeManager* manager=GetGlobalTypeManager();
				if(manager)
				{
					Ptr<ITypeLoader> loader=new GuiControlsTypeLoader;
					return manager->AddTypeLoader(loader);
				}
				return false;
			}
		}
	}
}