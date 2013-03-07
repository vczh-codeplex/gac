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
			using namespace list;

			GUIREFLECTIONCONTROLS_TYPELIST(IMPL_TYPE_INFO)

/***********************************************************************
External Functions
***********************************************************************/

			Ptr<IValueReadonlyList> GuiTab_GetPages(GuiTab* thisObject)
			{
				return new ValueReadonlyListWrapper<const List<GuiTabPage*>*>(&thisObject->GetPages());
			}

			Ptr<IValueReadonlyList> GuiApplication_GetWindows(GuiApplication* thisObject)
			{
				return new ValueReadonlyListWrapper<const List<GuiWindow*>*>(&thisObject->GetWindows());
			}

			void GuiApplication_InvokeAsync(GuiApplication* thisObject, Ptr<IValueFunctionProxy> proc)
			{
				thisObject->InvokeAsync([=]()
				{
					proc->Invoke(IValueList::Create());
				});
			}

			void GuiApplication_InvokeInMainThread(GuiApplication* thisObject, Ptr<IValueFunctionProxy> proc)
			{
				thisObject->InvokeInMainThread([=]()
				{
					proc->Invoke(IValueList::Create());
				});
			}

			void GuiApplication_InvokeInMainThreadAndWait(GuiApplication* thisObject, Ptr<IValueFunctionProxy> proc)
			{
				thisObject->InvokeInMainThreadAndWait([=]()
				{
					proc->Invoke(IValueList::Create());
				});
			}

			Ptr<INativeDelay> GuiApplication_DelayExecute(GuiApplication* thisObject, Ptr<IValueFunctionProxy> proc, vint milliseconds)
			{
				return thisObject->DelayExecute([=]()
				{
					proc->Invoke(IValueList::Create());
				}, milliseconds);
			}

			Ptr<INativeDelay> GuiApplication_DelayExecuteInMainThread(GuiApplication* thisObject, Ptr<IValueFunctionProxy> proc, vint milliseconds)
			{
				return thisObject->DelayExecuteInMainThread([=]()
				{
					proc->Invoke(IValueList::Create());
				}, milliseconds);
			}

			Ptr<IValueReadonlyList> GuiSelectableListControl_GetSelectedItem(GuiSelectableListControl* thisObject)
			{
				return new ValueReadonlyListWrapper<const SortedList<vint>*>(&thisObject->GetSelectedItems());
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
	CLASS_MEMBER_EXTERNALCTOR(decltype(interface_proxy::CONTROL##_##INTERFACE::Create(0))(Ptr<IValueInterfaceProxy>), {L"proxy"}, &interface_proxy::CONTROL##_##INTERFACE::Create)

#define INTERFACE_IDENTIFIER(INTERFACE)\
	CLASS_MEMBER_STATIC_EXTERNALMETHOD(GetIdentifier, NO_PARAMETER, WString(*)(), []()->WString{return INTERFACE::Identifier;})

			BEGIN_CLASS_MEMBER(GuiApplication)
				CLASS_MEMBER_STATIC_EXTERNALMETHOD(GetApplication, NO_PARAMETER, GuiApplication*(*)(), &GetApplication)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(MainWindow)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(TooltipOwner)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ExecutablePath)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ExecutableFolder)

				CLASS_MEMBER_EXTERNALMETHOD(GetWindows, NO_PARAMETER, Ptr<IValueReadonlyList>(GuiApplication::*)(), &GuiApplication_GetWindows)
				CLASS_MEMBER_METHOD(ShowTooltip, {L"owner" _ L"tooltip" _ L"preferredContentWidth" _ L"location"})
				CLASS_MEMBER_METHOD(CloseTooltip, NO_PARAMETER)
				CLASS_MEMBER_METHOD(IsInMainThread, NO_PARAMETER)
				CLASS_MEMBER_EXTERNALMETHOD(InvokeAsync, {L"proc"}, void(GuiApplication::*)(Ptr<IValueFunctionProxy>), &GuiApplication_InvokeAsync)
				CLASS_MEMBER_EXTERNALMETHOD(InvokeInMainThread, {L"proc"}, void(GuiApplication::*)(Ptr<IValueFunctionProxy>), &GuiApplication_InvokeInMainThread)
				CLASS_MEMBER_EXTERNALMETHOD(InvokeInMainThreadAndWait, {L"proc"}, void(GuiApplication::*)(Ptr<IValueFunctionProxy>), &GuiApplication_InvokeInMainThreadAndWait)
				CLASS_MEMBER_EXTERNALMETHOD(DelayExecute, {L"proc" _ L"milliseconds"}, Ptr<INativeDelay>(GuiApplication::*)(Ptr<IValueFunctionProxy>, vint), &GuiApplication_DelayExecute)
				CLASS_MEMBER_EXTERNALMETHOD(DelayExecuteInMainThread, {L"proc" _ L"milliseconds"}, Ptr<INativeDelay>(GuiApplication::*)(Ptr<IValueFunctionProxy>, vint), &GuiApplication_DelayExecuteInMainThread)
			END_CLASS_MEMBER(GuiApplication)

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

			BEGIN_CLASS_MEMBER(GuiListControl)
				CLASS_MEMBER_BASE(GuiScrollView)
				CLASS_MEMBER_CONSTRUCTOR(GuiListControl*(GuiListControl::IStyleProvider* _ GuiListControl::IItemProvider* _ bool), {L"styleProvider" _ L"itemProvider" _ L"acceptFocus"})

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ItemProvider)
				CLASS_MEMBER_PROPERTY_FAST(StyleProvider)
				CLASS_MEMBER_PROPERTY_FAST(Arranger)
				CLASS_MEMBER_PROPERTY_FAST(CoordinateTransformer)

				CLASS_MEMBER_METHOD(EnsureItemVisible, {L"itemIndex"})
			END_CLASS_MEMBER(GuiListControl)

			BEGIN_CLASS_MEMBER(GuiListControl::IItemProviderCallback)
				INTERFACE_EXTERNALCTOR(GuiListControl, IItemProviderCallback)

				CLASS_MEMBER_METHOD(OnAttached, {L"provider"})
				CLASS_MEMBER_METHOD(OnItemModified, {L"start" _ L"count" _ L"newCount"})
			END_CLASS_MEMBER(GuiListControl::IItemProviderCallback)

			BEGIN_CLASS_MEMBER(GuiListControl::IItemArrangerCallback)
				CLASS_MEMBER_METHOD(RequestItem, {L"itemIndex"})
				CLASS_MEMBER_METHOD(ReleaseItem, {L"style"})
				CLASS_MEMBER_METHOD(SetViewLocation, {L"value"})
				CLASS_MEMBER_METHOD(GetStylePreferredSize, {L"style"})
				CLASS_MEMBER_METHOD(SetStyleAlignmentToParent, {L"style" _ L"margin"})
				CLASS_MEMBER_METHOD(GetStyleBounds, {L"style"})
				CLASS_MEMBER_METHOD(SetStyleBounds, {L"style" _ L"bounds"})
				CLASS_MEMBER_METHOD(GetContainerComposition, NO_PARAMETER)
				CLASS_MEMBER_METHOD(OnTotalSizeChanged, NO_PARAMETER)
			END_CLASS_MEMBER(GuiListControl::IItemArrangerCallback)

			BEGIN_CLASS_MEMBER(GuiListControl::IItemPrimaryTextView)
				INTERFACE_EXTERNALCTOR(GuiListControl, IItemPrimaryTextView)
				INTERFACE_IDENTIFIER(GuiListControl::IItemPrimaryTextView)

				CLASS_MEMBER_METHOD(GetPrimaryTextViewText, {L"itemIndex"})
				CLASS_MEMBER_METHOD(ContainsPrimaryText, {L"itemIndex"})
			END_CLASS_MEMBER(GuiListControl::IItemPrimaryTextView)

			BEGIN_ENUM_ITEM(GuiListControl::KeyDirection)
				ENUM_ITEM_NAMESPACE(GuiListControl)
				ENUM_NAMESPACE_ITEM(Up)
				ENUM_NAMESPACE_ITEM(Down)
				ENUM_NAMESPACE_ITEM(Left)
				ENUM_NAMESPACE_ITEM(Right)
				ENUM_NAMESPACE_ITEM(Home)
				ENUM_NAMESPACE_ITEM(End)
				ENUM_NAMESPACE_ITEM(PageUp)
				ENUM_NAMESPACE_ITEM(PageDown)
				ENUM_NAMESPACE_ITEM(PageLeft)
				ENUM_NAMESPACE_ITEM(PageRight)
			END_ENUM_ITEM(GuiListControl::KeyDirection)

			BEGIN_CLASS_MEMBER(GuiListControl::IItemProvider)
				INTERFACE_EXTERNALCTOR(GuiListControl, IItemProvider)

				CLASS_MEMBER_METHOD(AttachCallback, {L"value"})
				CLASS_MEMBER_METHOD(DetachCallback, {L"value"})
				CLASS_MEMBER_METHOD(Count, NO_PARAMETER)
				CLASS_MEMBER_METHOD(RequestView, {L"identifier"})
				CLASS_MEMBER_METHOD(ReleaseView, {L"view"})
			END_CLASS_MEMBER(GuiListControl::IItemProvider)

			BEGIN_CLASS_MEMBER(GuiListControl::IItemStyleController)
				INTERFACE_EXTERNALCTOR(GuiListControl, IItemProvider)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(StyleProvider)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ItemStyleId)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(BoundsComposition)

				CLASS_MEMBER_METHOD(IsCacheable, NO_PARAMETER)
				CLASS_MEMBER_METHOD(IsInstalled, NO_PARAMETER)
				CLASS_MEMBER_METHOD(OnInstalled, NO_PARAMETER)
				CLASS_MEMBER_METHOD(OnUninstalled, NO_PARAMETER)
			END_CLASS_MEMBER(GuiListControl::IItemStyleController)

			BEGIN_CLASS_MEMBER(GuiListControl::IItemStyleProvider)
				INTERFACE_EXTERNALCTOR(GuiListControl, IItemStyleProvider)

				CLASS_MEMBER_METHOD(AttachListControl, {L"value"})
				CLASS_MEMBER_METHOD(DetachListControl, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetItemStyleId, {L"itemIndex"})
				CLASS_MEMBER_METHOD(CreateItemStyle, {L"styleId"})
				CLASS_MEMBER_METHOD(DestroyItemStyle, {L"style"})
				CLASS_MEMBER_METHOD(Install, {L"style" _ L"itemIndex"})
			END_CLASS_MEMBER(GuiListControl::IItemStyleProvider)

			BEGIN_CLASS_MEMBER(GuiListControl::IItemArranger)
				CLASS_MEMBER_BASE(GuiListControl::IItemProviderCallback)
				INTERFACE_EXTERNALCTOR(GuiListControl, IItemArranger)

				CLASS_MEMBER_PROPERTY_FAST(Callback)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(TotalSize)
				
				CLASS_MEMBER_METHOD(AttachListControl, {L"value"})
				CLASS_MEMBER_METHOD(DetachListControl, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetVisibleStyle, {L"itemIndex"})
				CLASS_MEMBER_METHOD(GetVisibleIndex, {L"style"})
				CLASS_MEMBER_METHOD(OnViewChanged, {L"bounds"})
				CLASS_MEMBER_METHOD(FindItem, {L"itemIndex" _ L"key"})
				CLASS_MEMBER_METHOD(EnsureItemVisible, {L"itemIndex"})
			END_CLASS_MEMBER(GuiListControl::IItemArranger)

			BEGIN_CLASS_MEMBER(GuiListControl::IItemCoordinateTransformer)
				INTERFACE_EXTERNALCTOR(GuiListControl, IItemCoordinateTransformer)

				CLASS_MEMBER_METHOD(RealSizeToVirtualSize, {L"size"})
				CLASS_MEMBER_METHOD(VirtualSizeToRealSize, {L"size"})
				CLASS_MEMBER_METHOD(RealPointToVirtualPoint, {L"realFullSize" _ L"point"})
				CLASS_MEMBER_METHOD(VirtualPointToRealPoint, {L"realFullSize" _ L"point"})
				CLASS_MEMBER_METHOD(RealRectToVirtualRect, {L"realFullSize" _ L"rect"})
				CLASS_MEMBER_METHOD(VirtualRectToRealRect, {L"realFullSize" _ L"rect"})
				CLASS_MEMBER_METHOD(RealMarginToVirtualMargin, {L"margin"})
				CLASS_MEMBER_METHOD(VirtualMarginToRealMargin, {L"margin"})
				CLASS_MEMBER_METHOD(RealKeyDirectionToVirtualKeyDirection, {L"key"})
			END_CLASS_MEMBER(GuiListControl::IItemCoordinateTransformer)

			BEGIN_CLASS_MEMBER(GuiSelectableListControl)
				CLASS_MEMBER_BASE(GuiListControl)
				CLASS_MEMBER_CONSTRUCTOR(GuiSelectableListControl*(GuiSelectableListControl::IStyleProvider* _ GuiSelectableListControl::IItemProvider*), {L"styleProvider" _ L"itemProvider"})

				CLASS_MEMBER_PROPERTY_FAST(MultiSelect)

				CLASS_MEMBER_EXTERNALMETHOD(GetSelectedItems, NO_PARAMETER, Ptr<IValueReadonlyList>(GuiSelectableListControl::*)(), &GuiSelectableListControl_GetSelectedItem)
				CLASS_MEMBER_METHOD(GetSelected, {L"itemIndex"})
				CLASS_MEMBER_METHOD(SetSelected, {L"itemIndex" _ L"value"})
				CLASS_MEMBER_METHOD(SelectItemsByClick, {L"itemIndex" _ L"ctrl" _ L"shift"})
				CLASS_MEMBER_METHOD(SelectItemsByKey, {L"code" _ L"ctrl" _ L"shift"})
				CLASS_MEMBER_METHOD(ClearSelection, NO_PARAMETER)
			END_CLASS_MEMBER(GuiSelectableListControl)

			BEGIN_CLASS_MEMBER(GuiSelectableListControl::IItemStyleProvider)
				CLASS_MEMBER_BASE(GuiListControl::IItemStyleProvider)
				INTERFACE_EXTERNALCTOR(GuiSelectableListControl, IItemStyleProvider)

				CLASS_MEMBER_METHOD(SetStyleSelected, {L"style" _ L"value"})
			END_CLASS_MEMBER(GuiSelectableListControl::IItemStyleProvider)

			BEGIN_CLASS_MEMBER(DefaultItemCoordinateTransformer)
				CLASS_MEMBER_BASE(GuiListControl::IItemCoordinateTransformer)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<DefaultItemCoordinateTransformer>(), NO_PARAMETER)
			END_CLASS_MEMBER(DefaultItemCoordinateTransformer)

			BEGIN_CLASS_MEMBER(AxisAlignedItemCoordinateTransformer)
				CLASS_MEMBER_BASE(GuiListControl::IItemCoordinateTransformer)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<AxisAlignedItemCoordinateTransformer>(AxisAlignedItemCoordinateTransformer::Alignment), {L"alignment"})

				CLASS_MEMBER_PROPERTY_READONLY_FAST(Alignment)
			END_CLASS_MEMBER(AxisAlignedItemCoordinateTransformer)

			BEGIN_ENUM_ITEM(AxisAlignedItemCoordinateTransformer::Alignment)
				ENUM_ITEM_NAMESPACE(AxisAlignedItemCoordinateTransformer)
				ENUM_NAMESPACE_ITEM(LeftDown)
				ENUM_NAMESPACE_ITEM(RightDown)
				ENUM_NAMESPACE_ITEM(LeftUp)
				ENUM_NAMESPACE_ITEM(RightUp)
				ENUM_NAMESPACE_ITEM(DownLeft)
				ENUM_NAMESPACE_ITEM(DownRight)
				ENUM_NAMESPACE_ITEM(UpLeft)
				ENUM_NAMESPACE_ITEM(UpRight)
			END_ENUM_ITEM(AxisAlignedItemCoordinateTransformer::Alignment)

			BEGIN_CLASS_MEMBER(RangedItemArrangerBase)
				CLASS_MEMBER_BASE(GuiListControl::IItemArranger)
			END_CLASS_MEMBER(RangedItemArrangerBase)

			BEGIN_CLASS_MEMBER(FixedHeightItemArranger)
				CLASS_MEMBER_BASE(RangedItemArrangerBase)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<FixedHeightItemArranger>(), NO_PARAMETER)
			END_CLASS_MEMBER(FixedHeightItemArranger)

			BEGIN_CLASS_MEMBER(FixedSizeMultiColumnItemArranger)
				CLASS_MEMBER_BASE(RangedItemArrangerBase)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<FixedSizeMultiColumnItemArranger>(), NO_PARAMETER)
			END_CLASS_MEMBER(FixedSizeMultiColumnItemArranger)

			BEGIN_CLASS_MEMBER(FixedHeightMultiColumnItemArranger)
				CLASS_MEMBER_BASE(RangedItemArrangerBase)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<FixedHeightMultiColumnItemArranger>(), NO_PARAMETER)
			END_CLASS_MEMBER(FixedHeightMultiColumnItemArranger)

			BEGIN_CLASS_MEMBER(ItemStyleControllerBase)
				CLASS_MEMBER_BASE(GuiListControl::IItemStyleController)
			END_CLASS_MEMBER(ItemStyleControllerBase)

			BEGIN_CLASS_MEMBER(TextItemStyleProvider)
				CLASS_MEMBER_BASE(GuiSelectableListControl::IItemStyleProvider)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<TextItemStyleProvider>(TextItemStyleProvider::ITextItemStyleProvider*), {L"textItemStyleProvider"})
			END_CLASS_MEMBER(TextItemStyleProvider)

			BEGIN_CLASS_MEMBER(TextItemStyleProvider::ITextItemStyleProvider)
				INTERFACE_EXTERNALCTOR(TextItemStyleProvider, ITextItemStyleProvider)

				CLASS_MEMBER_METHOD(CreateBackgroundStyleController, NO_PARAMETER)
				CLASS_MEMBER_METHOD(CreateBulletStyleController, NO_PARAMETER)
			END_CLASS_MEMBER(TextItemStyleProvider::ITextItemStyleProvider)

			BEGIN_CLASS_MEMBER(TextItemStyleProvider::ITextItemView)
				CLASS_MEMBER_BASE(GuiListControl::IItemPrimaryTextView)
				INTERFACE_EXTERNALCTOR(TextItemStyleProvider, ITextItemView)
				INTERFACE_IDENTIFIER(TextItemStyleProvider::ITextItemView)

				CLASS_MEMBER_METHOD(GetText, {L"itemIndex"})
				CLASS_MEMBER_METHOD(GetChecked, {L"itemIndex"})
				CLASS_MEMBER_METHOD(SetCheckedSilently, {L"itemIndex" _ L"value"})
			END_CLASS_MEMBER(TextItemStyleProvider::ITextItemView)

			BEGIN_CLASS_MEMBER(TextItemStyleProvider::TextItemStyleController)
				CLASS_MEMBER_BASE(ItemStyleControllerBase)
				CLASS_MEMBER_CONSTRUCTOR(TextItemStyleProvider::TextItemStyleController*(TextItemStyleProvider*), {L"provider"})

				CLASS_MEMBER_PROPERTY_FAST(Selected)
				CLASS_MEMBER_PROPERTY_FAST(Checked)
				CLASS_MEMBER_PROPERTY_FAST(Text)
			END_CLASS_MEMBER(TextItemStyleProvider::TextItemStyleController)

#undef INTERFACE_IDENTIFIER
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
