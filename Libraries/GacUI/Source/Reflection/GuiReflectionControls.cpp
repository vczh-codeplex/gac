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
			using namespace tree;
			using namespace text;

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

			TextItemProvider* GuiTextList_GetItemProvider(GuiTextList* thisObject)
			{
				return &thisObject->GetItems();
			}

			Ptr<IValueList> GuiTextList_GetItems(GuiTextList* thisObject)
			{
				return new ValueListWrapper<ListProvider<Ptr<TextItem>>*>(&thisObject->GetItems());
			}

			Ptr<IValueReadonlyList> ListViewItemStyleProvider_GetCreatedItemStyles(ListViewItemStyleProvider* thisObject)
			{
				return new ValueReadonlyListWrapper<const List<GuiListControl::IItemStyleController*>*>(&thisObject->GetCreatedItemStyles());
			}

			ListViewItemStyleProvider::IListViewItemContent* ListViewItemStyleProvider_GetItemContent(ListViewItemStyleProvider* thisObject, GuiListControl::IItemStyleController* itemStyleController)
			{
				return thisObject->GetItemContent<ListViewItemStyleProvider::IListViewItemContent>(itemStyleController);
			}

			Ptr<IValueList> ListViewItem_GetSubItems(ListViewItem* thisObject)
			{
				return new ValueListWrapper<List<WString>*>(&thisObject->subItems);
			}

			Ptr<IValueList> GuiListView_GetDataColumns(GuiListView* thisObject)
			{
				return new ValueListWrapper<ListViewDataColumns*>(&thisObject->GetItems().GetDataColumns());
			}

			Ptr<IValueList> GuiListView_GetColumns(GuiListView* thisObject)
			{
				return new ValueListWrapper<ListViewColumns*>(&thisObject->GetItems().GetColumns());
			}

			Ptr<IValueList> GuiListView_GetItems(GuiListView* thisObject)
			{
				return new ValueListWrapper<ListViewItemProvider*>(&thisObject->GetItems());
			}

			Ptr<IValueList> MemoryNodeProvider_GetChildren(MemoryNodeProvider* thisObject)
			{
				return new ValueListWrapper<MemoryNodeProvider::NodeCollection*>(&thisObject->Children());
			}

			Ptr<TreeViewItemRootProvider> GuiTreeView_GetNodes(GuiTreeView* thisObject)
			{
				return thisObject->Nodes();
			}

			Ptr<IValueList> GuiToolstripMenu_GetToolstripItems(GuiToolstripMenu* thisObject)
			{
				return new ValueListWrapper<GuiToolstripCollection*>(&thisObject->GetToolstripItems());
			}

			Ptr<IValueList> GuiToolstripMenuBar_GetToolstripItems(GuiToolstripMenuBar* thisObject)
			{
				return new ValueListWrapper<GuiToolstripCollection*>(&thisObject->GetToolstripItems());
			}

			Ptr<IValueList> GuiToolstripToolbar_GetToolstripItems(GuiToolstripToolbar* thisObject)
			{
				return new ValueListWrapper<GuiToolstripCollection*>(&thisObject->GetToolstripItems());
			}

			Ptr<IValueReadonlyList> GuiTextBoxColorizerBase_GetColors(GuiTextBoxColorizerBase* thisObject)
			{
				return new ValueReadonlyListWrapper<const Array<ColorEntry>*>(&thisObject->GetColors());
			}

			Ptr<IValueList> GuiTextBoxRegexColorizer_GetTokenRegexes(GuiTextBoxRegexColorizer* thisObject)
			{
				return new ValueListWrapper<List<WString>*>(&thisObject->GetTokenRegexes());
			}

			Ptr<IValueList> GuiTextBoxRegexColorizer_GetTokenColors(GuiTextBoxRegexColorizer* thisObject)
			{
				return new ValueListWrapper<List<ColorEntry>*>(&thisObject->GetTokenColors());
			}

			Ptr<IValueList> GuiTextBoxRegexColorizer_GetExtraTokenColors(GuiTextBoxRegexColorizer* thisObject)
			{
				return new ValueListWrapper<List<ColorEntry>*>(&thisObject->GetExtraTokenColors());
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
				CLASS_MEMBER_BASE(IDescriptable)
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
				CLASS_MEMBER_BASE(IDescriptable)
				INTERFACE_EXTERNALCTOR(GuiListControl, IItemProviderCallback)

				CLASS_MEMBER_METHOD(OnAttached, {L"provider"})
				CLASS_MEMBER_METHOD(OnItemModified, {L"start" _ L"count" _ L"newCount"})
			END_CLASS_MEMBER(GuiListControl::IItemProviderCallback)

			BEGIN_CLASS_MEMBER(GuiListControl::IItemArrangerCallback)
				CLASS_MEMBER_BASE(IDescriptable)
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
				CLASS_MEMBER_BASE(IDescriptable)
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
				CLASS_MEMBER_BASE(IDescriptable)
				INTERFACE_EXTERNALCTOR(GuiListControl, IItemProvider)

				CLASS_MEMBER_METHOD(AttachCallback, {L"value"})
				CLASS_MEMBER_METHOD(DetachCallback, {L"value"})
				CLASS_MEMBER_METHOD(Count, NO_PARAMETER)
				CLASS_MEMBER_METHOD(RequestView, {L"identifier"})
				CLASS_MEMBER_METHOD(ReleaseView, {L"view"})
			END_CLASS_MEMBER(GuiListControl::IItemProvider)

			BEGIN_CLASS_MEMBER(GuiListControl::IItemStyleController)
				CLASS_MEMBER_BASE(IDescriptable)
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
				CLASS_MEMBER_BASE(IDescriptable)
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
				CLASS_MEMBER_BASE(IDescriptable)
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

			BEGIN_CLASS_MEMBER(ItemProviderBase)
				CLASS_MEMBER_BASE(GuiListControl::IItemProvider)
			END_CLASS_MEMBER(ItemProviderBase)

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

			BEGIN_CLASS_MEMBER(TextItem)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<TextItem>(), NO_PARAMETER)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<TextItem>(const WString&), {L"text"})
				CLASS_MEMBER_CONSTRUCTOR(Ptr<TextItem>(const WString&, bool), {L"text" _ L"checked"})

				CLASS_MEMBER_PROPERTY_READONLY_FAST(Text)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Checked)
			END_CLASS_MEMBER(TextItem)

			BEGIN_CLASS_MEMBER(TextItemProvider)
				CLASS_MEMBER_BASE(ItemProviderBase)

				CLASS_MEMBER_METHOD(SetText, {L"itemIndex" _ L"value"})
				CLASS_MEMBER_METHOD(SetChecked, {L"itemIndex" _ L"value"})
			END_CLASS_MEMBER(TextItemProvider)

			BEGIN_CLASS_MEMBER(GuiVirtualTextList)
				CLASS_MEMBER_BASE(GuiSelectableListControl)
				CLASS_MEMBER_CONSTRUCTOR(GuiVirtualTextList*(GuiSelectableListControl::IStyleProvider* _ TextItemStyleProvider::ITextItemStyleProvider* _ GuiListControl::IItemProvider*), {L"styleProvider" _ L"itemStyleProvider" _ L"itemProvider"})

				CLASS_MEMBER_METHOD(ChangeItemStyle, {L"itemStyleProvider"})
			END_CLASS_MEMBER(GuiVirtualTextList)

			BEGIN_CLASS_MEMBER(GuiTextList)
				CLASS_MEMBER_BASE(GuiVirtualTextList)
				CLASS_MEMBER_CONSTRUCTOR(GuiTextList*(GuiSelectableListControl::IStyleProvider* _ TextItemStyleProvider::ITextItemStyleProvider*), {L"styleProvider" _ L"itemStyleProvider"})

				CLASS_MEMBER_EXTERNALMETHOD(GetItemProvider, NO_PARAMETER, TextItemProvider*(GuiTextList::*)(), &GuiTextList_GetItemProvider)
				CLASS_MEMBER_EXTERNALMETHOD(GetItems, NO_PARAMETER, Ptr<IValueList>(GuiTextList::*)(), &GuiTextList_GetItems)
			END_CLASS_MEMBER(GuiTextList)

			BEGIN_CLASS_MEMBER(ListViewItemStyleProviderBase)
				CLASS_MEMBER_BASE(GuiSelectableListControl::IItemStyleProvider)
			END_CLASS_MEMBER(ListViewItemStyleProviderBase)

			BEGIN_CLASS_MEMBER(ListViewItemStyleProviderBase::ListViewItemStyleController)
				CLASS_MEMBER_BASE(ItemStyleControllerBase)
				CLASS_MEMBER_CONSTRUCTOR(ListViewItemStyleProviderBase::ListViewItemStyleController*(ListViewItemStyleProviderBase*), {L"provider"})

				CLASS_MEMBER_PROPERTY_FAST(Selected)
			END_CLASS_MEMBER(ListViewItemStyleProviderBase::ListViewItemStyleController)

			BEGIN_CLASS_MEMBER(GuiListViewColumnHeader)
				CLASS_MEMBER_BASE(GuiMenuButton)
				CONTROL_CONSTRUCTOR_CONTROLLER(GuiListViewColumnHeader)

				CLASS_MEMBER_PROPERTY_FAST(ColumnSortingState)
			END_CLASS_MEMBER(GuiListViewColumnHeader)

			BEGIN_ENUM_ITEM(GuiListViewColumnHeader::ColumnSortingState)
				ENUM_ITEM_NAMESPACE(GuiListViewColumnHeader)
				ENUM_NAMESPACE_ITEM(NotSorted)
				ENUM_NAMESPACE_ITEM(Ascending)
				ENUM_NAMESPACE_ITEM(Descending)
			END_ENUM_ITEM(GuiListViewColumnHeader::ColumnSortingState)

			BEGIN_CLASS_MEMBER(GuiListViewColumnHeader::IStyleController)
				CLASS_MEMBER_BASE(GuiMenuButton::IStyleController)
				INTERFACE_EXTERNALCTOR(GuiListViewColumnHeader, IStyleController)

				CLASS_MEMBER_METHOD(SetColumnSortingState, {L"value"})
			END_CLASS_MEMBER(GuiListViewColumnHeader::IStyleController)

			BEGIN_CLASS_MEMBER(GuiListViewBase)
				CLASS_MEMBER_BASE(GuiSelectableListControl)
				CLASS_MEMBER_CONSTRUCTOR(GuiListViewBase*(GuiListViewBase::IStyleProvider* _ GuiListControl::IItemProvider*), {L"styleProvider" _ L"itemProvider"})

				CLASS_MEMBER_METHOD(GetListViewStyleProvider, NO_PARAMETER)
			END_CLASS_MEMBER(GuiListViewBase)

			BEGIN_CLASS_MEMBER(GuiListViewBase::IStyleProvider)
				CLASS_MEMBER_BASE(GuiSelectableListControl::IStyleProvider)
				INTERFACE_EXTERNALCTOR(GuiListViewBase, IStyleProvider)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(PrimaryTextColor)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(SecondaryTextColor)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ItemSeparatorColor)

				CLASS_MEMBER_METHOD(CreateItemBackground, NO_PARAMETER)
				CLASS_MEMBER_METHOD(CreateColumnStyle, NO_PARAMETER)
			END_CLASS_MEMBER(GuiListViewBase::IStyleProvider)

			BEGIN_CLASS_MEMBER(ListViewItemStyleProvider)
				CLASS_MEMBER_BASE(ListViewItemStyleProviderBase)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<ListViewItemStyleProvider>(ListViewItemStyleProvider::IListViewItemContentProvider*), {L"itemContentProvider"})

				CLASS_MEMBER_EXTERNALMETHOD(GetCreatedItemStyles, NO_PARAMETER, Ptr<IValueReadonlyList>(ListViewItemStyleProvider::*)(), &ListViewItemStyleProvider_GetCreatedItemStyles)
				CLASS_MEMBER_PROPERTY_READONLY(CreatedItemStyles, GetCreatedItemStyles)
				CLASS_MEMBER_METHOD(IsItemStyleAttachedToListView, {L"itemStyle"})
				CLASS_MEMBER_EXTERNALMETHOD(GetItemContent, {L"itemStyleController"}, ListViewItemStyleProvider::IListViewItemContent*(ListViewItemStyleProvider::*)(GuiListControl::IItemStyleController*), &ListViewItemStyleProvider_GetItemContent)
			END_CLASS_MEMBER(ListViewItemStyleProvider)

			BEGIN_CLASS_MEMBER(ListViewItemStyleProvider::IListViewItemView)
				CLASS_MEMBER_BASE(GuiListControl::IItemPrimaryTextView)
				INTERFACE_EXTERNALCTOR(ListViewItemStyleProvider, IListViewItemView)
				INTERFACE_IDENTIFIER(ListViewItemStyleProvider::IListViewItemView)

				CLASS_MEMBER_METHOD(GetSmallImage, {L"itemIndex"})
				CLASS_MEMBER_METHOD(GetLargeImage, {L"itemIndex"})
				CLASS_MEMBER_METHOD(GetText, {L"itemIndex"})
				CLASS_MEMBER_METHOD(GetSubItem, {L"itemIndex" _ L"index"})
				CLASS_MEMBER_METHOD(GetDataColumnCount, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetDataColumn, {L"index"})
				CLASS_MEMBER_METHOD(GetColumnCount, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetColumnText, {L"index"})
			END_CLASS_MEMBER(ListViewItemStyleProvider::IListViewItemView)

			BEGIN_CLASS_MEMBER(ListViewItemStyleProvider::IListViewItemContent)
				CLASS_MEMBER_BASE(IDescriptable)
				INTERFACE_EXTERNALCTOR(ListViewItemStyleProvider, IListViewItemContent)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ContentComposition)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(BackgroundDecorator)
				
				CLASS_MEMBER_METHOD(Install, {L"styleProvider" _ L"view" _ L"itemIndex"})
			END_CLASS_MEMBER(ListViewItemStyleProvider::IListViewItemContent)

			BEGIN_CLASS_MEMBER(ListViewItemStyleProvider::IListViewItemContentProvider)
				CLASS_MEMBER_BASE(IDescriptable)
				INTERFACE_EXTERNALCTOR(ListViewItemStyleProvider, IListViewItemContentProvider)

				CLASS_MEMBER_METHOD(CreatePreferredCoordinateTransformer, NO_PARAMETER)
				CLASS_MEMBER_METHOD(CreatePreferredArranger, NO_PARAMETER)
				CLASS_MEMBER_METHOD(CreateItemContent, {L"font"})
				CLASS_MEMBER_METHOD(AttachListControl, {L"value"})
				CLASS_MEMBER_METHOD(DetachListControl, NO_PARAMETER)
			END_CLASS_MEMBER(ListViewItemStyleProvider::IListViewItemContentProvider)

			BEGIN_CLASS_MEMBER(ListViewItemStyleProvider::ListViewContentItemStyleController)
				CLASS_MEMBER_BASE(ListViewItemStyleProviderBase::ListViewItemStyleController)
				CLASS_MEMBER_CONSTRUCTOR(ListViewItemStyleProvider::ListViewContentItemStyleController*(ListViewItemStyleProvider*), {L"provider"})

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ItemContent)

				CLASS_MEMBER_METHOD(Install, {L"view" _ L"itemIndex"})
			END_CLASS_MEMBER(ListViewItemStyleProvider::ListViewContentItemStyleController)

			BEGIN_CLASS_MEMBER(ListViewBigIconContentProvider)
				CLASS_MEMBER_BASE(ListViewItemStyleProvider::IListViewItemContentProvider)
				CLASS_MEMBER_CONSTRUCTOR(ListViewBigIconContentProvider*(Size), {L"iconSize"})
			END_CLASS_MEMBER(ListViewBigIconContentProvider)

			BEGIN_CLASS_MEMBER(ListViewSmallIconContentProvider)
				CLASS_MEMBER_BASE(ListViewItemStyleProvider::IListViewItemContentProvider)
				CLASS_MEMBER_CONSTRUCTOR(ListViewSmallIconContentProvider*(Size), {L"iconSize"})
			END_CLASS_MEMBER(ListViewSmallIconContentProvider)

			BEGIN_CLASS_MEMBER(ListViewListContentProvider)
				CLASS_MEMBER_BASE(ListViewItemStyleProvider::IListViewItemContentProvider)
				CLASS_MEMBER_CONSTRUCTOR(ListViewListContentProvider*(Size), {L"iconSize"})
			END_CLASS_MEMBER(ListViewListContentProvider)

			BEGIN_CLASS_MEMBER(ListViewTileContentProvider)
				CLASS_MEMBER_BASE(ListViewItemStyleProvider::IListViewItemContentProvider)
				CLASS_MEMBER_CONSTRUCTOR(ListViewTileContentProvider*(Size), {L"iconSize"})
			END_CLASS_MEMBER(ListViewTileContentProvider)

			BEGIN_CLASS_MEMBER(ListViewInformationContentProvider)
				CLASS_MEMBER_BASE(ListViewItemStyleProvider::IListViewItemContentProvider)
				CLASS_MEMBER_CONSTRUCTOR(ListViewInformationContentProvider*(Size), {L"iconSize"})
			END_CLASS_MEMBER(ListViewInformationContentProvider)

			BEGIN_CLASS_MEMBER(ListViewColumnItemArranger)
				CLASS_MEMBER_BASE(FixedHeightItemArranger)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<ListViewColumnItemArranger>(), NO_PARAMETER)
			END_CLASS_MEMBER(ListViewColumnItemArranger)

			BEGIN_CLASS_MEMBER(ListViewColumnItemArranger::IColumnItemViewCallback)
				CLASS_MEMBER_METHOD(OnColumnChanged, NO_PARAMETER)
			END_CLASS_MEMBER(ListViewColumnItemArranger::IColumnItemViewCallback)

			BEGIN_CLASS_MEMBER(ListViewColumnItemArranger::IColumnItemView)
				CLASS_MEMBER_BASE(IDescriptable)
				INTERFACE_EXTERNALCTOR(ListViewColumnItemArranger, IColumnItemView)
				INTERFACE_IDENTIFIER(ListViewColumnItemArranger::IColumnItemView)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ColumnCount)

				CLASS_MEMBER_METHOD(AttachCallback, {L"value"})
				CLASS_MEMBER_METHOD(DetachCallback, {L"value"})
				CLASS_MEMBER_METHOD(GetColumnText, {L"index"})
				CLASS_MEMBER_METHOD(GetColumnSize, {L"index"})
				CLASS_MEMBER_METHOD(SetColumnSize, {L"index" _ L"value"})
				CLASS_MEMBER_METHOD(GetDropdownPopup, {L"index"})
				CLASS_MEMBER_METHOD(GetSortingState, {L"index"})
			END_CLASS_MEMBER(ListViewColumnItemArranger::IColumnItemView)

			BEGIN_CLASS_MEMBER(ListViewDetailContentProvider)
				CLASS_MEMBER_BASE(ListViewItemStyleProvider::IListViewItemContentProvider)
				CLASS_MEMBER_CONSTRUCTOR(ListViewDetailContentProvider*(Size), {L"iconSize"})
			END_CLASS_MEMBER(ListViewDetailContentProvider)

			BEGIN_CLASS_MEMBER(ListViewItem)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<ListViewItem>(), NO_PARAMETER)

				CLASS_MEMBER_FIELD(smallImage)
				CLASS_MEMBER_FIELD(largeImage)
				CLASS_MEMBER_FIELD(text)

				CLASS_MEMBER_EXTERNALMETHOD(GetSubItems, NO_PARAMETER, Ptr<IValueList>(ListViewItem::*)(), &ListViewItem_GetSubItems)
				CLASS_MEMBER_PROPERTY_READONLY(subItems, GetSubItems)
			END_CLASS_MEMBER(ListViewItem)

			BEGIN_CLASS_MEMBER(ListViewColumn)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<ListViewColumn>(), NO_PARAMETER)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<ListViewColumn>(const WString&), {L"text"})
				CLASS_MEMBER_CONSTRUCTOR(Ptr<ListViewColumn>(const WString&, vint), {L"text" _ L"size"})

				CLASS_MEMBER_FIELD(text)
				CLASS_MEMBER_FIELD(size)
				CLASS_MEMBER_FIELD(dropdownPopup)
				CLASS_MEMBER_FIELD(sortingState)
			END_CLASS_MEMBER(ListViewColumn)

			BEGIN_CLASS_MEMBER(GuiVirtualListView)
				CLASS_MEMBER_BASE(GuiListViewBase)
				CLASS_MEMBER_CONSTRUCTOR(GuiVirtualListView*(GuiVirtualListView::IStyleProvider* _ GuiListControl::IItemProvider*), {L"styleProvider" _ L"itemProvider"})

				CLASS_MEMBER_METHOD(ChangeItemStyle, {L"contentProvider"})
			END_CLASS_MEMBER(GuiVirtualListView)

			BEGIN_CLASS_MEMBER(GuiListView)
				CLASS_MEMBER_BASE(GuiVirtualListView)
				CONTROL_CONSTRUCTOR_PROVIDER(GuiListView)

				CLASS_MEMBER_EXTERNALMETHOD(GetDataColumns, NO_PARAMETER, Ptr<IValueList>(GuiListView::*)(), &GuiListView_GetDataColumns)
				CLASS_MEMBER_EXTERNALMETHOD(GetColumns, NO_PARAMETER, Ptr<IValueList>(GuiListView::*)(), &GuiListView_GetColumns)
				CLASS_MEMBER_EXTERNALMETHOD(GetItems, NO_PARAMETER, Ptr<IValueList>(GuiListView::*)(), &GuiListView_GetItems)
			END_CLASS_MEMBER(GuiListView)

			BEGIN_CLASS_MEMBER(IGuiMenuService)
				INTERFACE_IDENTIFIER(IGuiMenuService)

				CLASS_MEMBER_METHOD(GetParentMenuService, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetPreferredDirection, NO_PARAMETER)
				CLASS_MEMBER_METHOD(IsActiveState, NO_PARAMETER)
				CLASS_MEMBER_METHOD(IsSubMenuActivatedByMouseDown, NO_PARAMETER)
				CLASS_MEMBER_METHOD(MenuItemExecuted, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetOpeningMenu, NO_PARAMETER)
				CLASS_MEMBER_METHOD(MenuOpened, {L"menu"})
				CLASS_MEMBER_METHOD(MenuClosed, {L"menu"})
			END_CLASS_MEMBER(IGuiMenuService)

			BEGIN_ENUM_ITEM(IGuiMenuService::Direction)
				ENUM_ITEM_NAMESPACE(IGuiMenuService)
				ENUM_NAMESPACE_ITEM(Horizontal)
				ENUM_NAMESPACE_ITEM(Vertical)
			END_ENUM_ITEM(IGuiMenuService::Direction)

			BEGIN_CLASS_MEMBER(GuiMenu)
				CLASS_MEMBER_BASE(GuiPopup)
				CLASS_MEMBER_CONSTRUCTOR(GuiMenu*(GuiMenu::IStyleController* _ GuiControl*), {L"styleController" _ L"owner"})

				CLASS_MEMBER_METHOD(UpdateMenuService, NO_PARAMETER)
				CLASS_MEMBER_METHOD(QueryService, {L"identifier"})
			END_CLASS_MEMBER(GuiMenu)

			BEGIN_CLASS_MEMBER(GuiMenuBar)
				CLASS_MEMBER_BASE(GuiControl)
				CONTROL_CONSTRUCTOR_CONTROLLER(GuiMenuBar)
			END_CLASS_MEMBER(GuiMenuBar)

			BEGIN_CLASS_MEMBER(GuiMenuButton)
				CLASS_MEMBER_BASE(GuiButton)
				CONTROL_CONSTRUCTOR_CONTROLLER(GuiMenuButton)

				CLASS_MEMBER_PROPERTY_FAST(Image)
				CLASS_MEMBER_PROPERTY_FAST(ShortcutText)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(SubMenu)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(OwnedSubMenu)
				CLASS_MEMBER_PROPERTY_FAST(SubMenuOpening)
				CLASS_MEMBER_PROPERTY_FAST(PreferredMenuClientSize)

				CLASS_MEMBER_METHOD(IsSubMenuExists, NO_PARAMETER)
				CLASS_MEMBER_METHOD(CreateSubMenu, {L"subMenuStyleController"})
				CLASS_MEMBER_METHOD(SetSubMenu, {L"value" _ L"owned"})
			END_CLASS_MEMBER(GuiMenuButton)

			BEGIN_CLASS_MEMBER(GuiMenuButton::IStyleController)
				CLASS_MEMBER_BASE(GuiButton::IStyleController)
				INTERFACE_EXTERNALCTOR(GuiMenuButton, IStyleController)

				CLASS_MEMBER_METHOD(CreateSubMenuStyleController, NO_PARAMETER)
				CLASS_MEMBER_METHOD(SetSubMenuExisting, {L"value"})
				CLASS_MEMBER_METHOD(SetSubMenuOpening, {L"value"})
				CLASS_MEMBER_METHOD(GetSubMenuHost, NO_PARAMETER)
				CLASS_MEMBER_METHOD(SetImage, {L"value"})
				CLASS_MEMBER_METHOD(SetShortcutText, {L"value"})
				CLASS_MEMBER_METHOD(GetMeasuringSource, NO_PARAMETER)
			END_CLASS_MEMBER(GuiMenuButton::IStyleController)

			BEGIN_CLASS_MEMBER(INodeProviderCallback)
				CLASS_MEMBER_BASE(IDescriptable)

				CLASS_MEMBER_METHOD(OnAttached, {L"provider"})
				CLASS_MEMBER_METHOD(OnBeforeItemModified, {L"parentNode" _ L"start" _ L"count" _ L"newCount"})
				CLASS_MEMBER_METHOD(OnAfterItemModified, {L"parentNode" _ L"start" _ L"count" _ L"newCount"})
				CLASS_MEMBER_METHOD(OnItemExpanded, {L"node"})
				CLASS_MEMBER_METHOD(OnItemCollapsed, {L"node"})
			END_CLASS_MEMBER(INodeProviderCallback)

			BEGIN_CLASS_MEMBER(INodeProvider)
				CLASS_MEMBER_BASE(IDescriptable)
				INTERFACE_EXTERNALCTOR(tree, INodeProvider)

				CLASS_MEMBER_PROPERTY_FAST(Expanding)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ChildCount)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Parent)

				CLASS_MEMBER_METHOD(CalculateTotalVisibleNodes, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetChild, {L"index"})
				CLASS_MEMBER_METHOD(Increase, NO_PARAMETER)
				CLASS_MEMBER_METHOD(Release, NO_PARAMETER)
			END_CLASS_MEMBER(INodeProvider)

			BEGIN_CLASS_MEMBER(INodeRootProvider)
				CLASS_MEMBER_BASE(IDescriptable)
				INTERFACE_EXTERNALCTOR(tree, INodeRootProvider)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(RootNode)

				CLASS_MEMBER_METHOD(CanGetNodeByVisibleIndex, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetNodeByVisibleIndex, {L"index"})
				CLASS_MEMBER_METHOD(AttachCallback, {L"value"})
				CLASS_MEMBER_METHOD(DetachCallback, {L"value"})
				CLASS_MEMBER_METHOD(RequestView, {L"identifier"})
				CLASS_MEMBER_METHOD(ReleaseView, {L"value"})
			END_CLASS_MEMBER(INodeRootProvider)

			BEGIN_CLASS_MEMBER(INodeItemView)
				CLASS_MEMBER_BASE(GuiListControl::IItemPrimaryTextView)
				INTERFACE_EXTERNALCTOR(tree, INodeItemView)
				INTERFACE_IDENTIFIER(INodeItemView)

				CLASS_MEMBER_METHOD(RequestNode, {L"index"})
				CLASS_MEMBER_METHOD(ReleaseNode, {L"node"})
				CLASS_MEMBER_METHOD(CalculateNodeVisibilityIndex, {L"node"})
			END_CLASS_MEMBER(INodeItemView)

			BEGIN_CLASS_MEMBER(INodeItemPrimaryTextView)
				CLASS_MEMBER_BASE(IDescriptable)
				INTERFACE_EXTERNALCTOR(tree, INodeItemPrimaryTextView)
				INTERFACE_IDENTIFIER(INodeItemPrimaryTextView)

				CLASS_MEMBER_METHOD(GetPrimaryTextViewText, {L"node"})
			END_CLASS_MEMBER(INodeItemPrimaryTextView)

			BEGIN_CLASS_MEMBER(NodeItemProvider)
				CLASS_MEMBER_BASE(ItemProviderBase)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<NodeItemProvider>(Ptr<INodeRootProvider>), {L"root"})

				CLASS_MEMBER_PROPERTY_READONLY_FAST(Root)
			END_CLASS_MEMBER(NodeItemProvider)

			BEGIN_CLASS_MEMBER(INodeItemStyleController)
				CLASS_MEMBER_BASE(GuiListControl::IItemStyleController)
				INTERFACE_EXTERNALCTOR(tree, INodeItemStyleController)

				CLASS_MEMBER_METHOD(GetNodeStyleProvider, NO_PARAMETER)
			END_CLASS_MEMBER(INodeItemStyleController)

			BEGIN_CLASS_MEMBER(INodeItemStyleProvider)
				CLASS_MEMBER_BASE(IDescriptable)
				INTERFACE_EXTERNALCTOR(tree, INodeItemStyleProvider)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(BindedItemStyleProvider)

				CLASS_MEMBER_METHOD(BindItemStyleProvider, {L"styleProvider"})
				CLASS_MEMBER_METHOD(AttachListControl, {L"value"})
				CLASS_MEMBER_METHOD(DetachListControl, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetItemStyleId, {L"node"})
				CLASS_MEMBER_METHOD(CreateItemStyle, {L"styleId"})
				CLASS_MEMBER_METHOD(DestroyItemStyle, {L"style"})
				CLASS_MEMBER_METHOD(Install, {L"style" _ L"node"})
				CLASS_MEMBER_METHOD(SetStyleSelected, {L"style" _ L"value"})
			END_CLASS_MEMBER(INodeItemStyleProvider)

			BEGIN_CLASS_MEMBER(NodeItemStyleProvider)
				CLASS_MEMBER_BASE(GuiSelectableListControl::IItemStyleProvider)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<NodeItemStyleProvider>(Ptr<INodeItemStyleProvider>), {L"provider"})
			END_CLASS_MEMBER(NodeItemStyleProvider)

			BEGIN_CLASS_MEMBER(IMemoryNodeData)
				CLASS_MEMBER_BASE(IDescriptable)
				INTERFACE_EXTERNALCTOR(tree, IMemoryNodeData)
			END_CLASS_MEMBER(IMemoryNodeData)

			BEGIN_CLASS_MEMBER(MemoryNodeProvider)
				CLASS_MEMBER_BASE(INodeProvider)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<MemoryNodeProvider>(), NO_PARAMETER)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<MemoryNodeProvider>(Ptr<IMemoryNodeData>), {L"data"})

				CLASS_MEMBER_PROPERTY_FAST(Data);

				CLASS_MEMBER_METHOD(NotifyDataModified, NO_PARAMETER)
				CLASS_MEMBER_EXTERNALMETHOD(GetChildren, NO_PARAMETER, Ptr<IValueList>(MemoryNodeProvider::*)(), &MemoryNodeProvider_GetChildren)
				CLASS_MEMBER_PROPERTY_READONLY(Children, GetChildren)
			END_CLASS_MEMBER(MemoryNodeProvider)

			BEGIN_CLASS_MEMBER(NodeRootProviderBase)
				CLASS_MEMBER_BASE(INodeRootProvider)
			END_CLASS_MEMBER(NodeRootProviderBase)

			BEGIN_CLASS_MEMBER(MemoryNodeRootProvider)
				CLASS_MEMBER_BASE(NodeRootProviderBase)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<MemoryNodeRootProvider>(), NO_PARAMETER)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(RootNode)

				CLASS_MEMBER_METHOD(GetMemoryNode, {L"node"})
			END_CLASS_MEMBER(MemoryNodeRootProvider)

			BEGIN_CLASS_MEMBER(GuiVirtualTreeListControl)
				CLASS_MEMBER_BASE(GuiSelectableListControl)
				CLASS_MEMBER_CONSTRUCTOR(GuiVirtualTreeListControl*(GuiVirtualTreeListControl::IStyleProvider* _ Ptr<INodeRootProvider>), {L"styleProvider" _ L"rootNodeProvider"})

				CLASS_MEMBER_PROPERTY_READONLY_FAST(NodeItemView)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(NodeRootProvider)
				CLASS_MEMBER_PROPERTY_FAST(NodeStyleProvider)
			END_CLASS_MEMBER(GuiVirtualTreeListControl)

			BEGIN_CLASS_MEMBER(ITreeViewItemView)
				CLASS_MEMBER_BASE(INodeItemPrimaryTextView)
				INTERFACE_EXTERNALCTOR(tree, ITreeViewItemView)
				INTERFACE_IDENTIFIER(ITreeViewItemView)

				CLASS_MEMBER_METHOD(GetNodeImage, {L"node"})
				CLASS_MEMBER_METHOD(GetNodeText, {L"node"})
			END_CLASS_MEMBER(ITreeViewItemView)

			BEGIN_CLASS_MEMBER(TreeViewItem)
				CLASS_MEMBER_BASE(IMemoryNodeData)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<TreeViewItem>(), NO_PARAMETER)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<TreeViewItem>(const Ptr<GuiImageData>&, const WString&), {L"image" _ L"text"})

				CLASS_MEMBER_FIELD(image)
				CLASS_MEMBER_FIELD(text)
			END_CLASS_MEMBER(TreeViewItem)

			BEGIN_CLASS_MEMBER(TreeViewItemRootProvider)
				CLASS_MEMBER_BASE(MemoryNodeRootProvider)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<TreeViewItemRootProvider>(), NO_PARAMETER)

				CLASS_MEMBER_METHOD(GetTreeViewData, {L"node"})
				CLASS_MEMBER_METHOD(SetTreeViewData, {L"node" _ L"value"})
				CLASS_MEMBER_METHOD(UpdateTreeViewData, {L"node"})
			END_CLASS_MEMBER(TreeViewItemRootProvider)

			BEGIN_CLASS_MEMBER(GuiVirtualTreeView)
				CLASS_MEMBER_BASE(GuiVirtualTreeListControl)
				CLASS_MEMBER_CONSTRUCTOR(GuiVirtualTreeView*(GuiVirtualTreeView::IStyleProvider* _ Ptr<INodeRootProvider>), {L"styleProvider" _ L"rootNodeProvider"})

				CLASS_MEMBER_PROPERTY_READONLY_FAST(TreeViewStyleProvider)
			END_CLASS_MEMBER(GuiVirtualTreeView)

			BEGIN_CLASS_MEMBER(GuiVirtualTreeView::IStyleProvider)
				CLASS_MEMBER_BASE(GuiVirtualTreeListControl::IStyleProvider)
				INTERFACE_EXTERNALCTOR(GuiVirtualTreeView, IStyleProvider)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(TextColor)

				CLASS_MEMBER_METHOD(CreateItemBackground, NO_PARAMETER)
				CLASS_MEMBER_METHOD(CreateItemExpandingDecorator, NO_PARAMETER)
			END_CLASS_MEMBER(GuiVirtualTreeView::IStyleProvider)

			BEGIN_CLASS_MEMBER(GuiTreeView)
				CLASS_MEMBER_BASE(GuiVirtualTreeView)
				CONTROL_CONSTRUCTOR_PROVIDER(GuiTreeView)

				CLASS_MEMBER_EXTERNALMETHOD(GetNodes, NO_PARAMETER, Ptr<TreeViewItemRootProvider>(GuiTreeView::*)(), &GuiTreeView_GetNodes)
				CLASS_MEMBER_PROPERTY_READONLY(Nodes, GetNodes)
			END_CLASS_MEMBER(GuiTreeView)

			BEGIN_CLASS_MEMBER(TreeViewNodeItemStyleProvider)
				CLASS_MEMBER_BASE(INodeItemStyleProvider)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<TreeViewNodeItemStyleProvider>(), NO_PARAMETER)
			END_CLASS_MEMBER(TreeViewNodeItemStyleProvider)

			BEGIN_CLASS_MEMBER(GuiComboBoxBase)
				CLASS_MEMBER_BASE(GuiButton)
				CONTROL_CONSTRUCTOR_CONTROLLER(GuiComboBoxBase)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(Popup)

				CLASS_MEMBER_METHOD(ShowPopup, NO_PARAMETER)
			END_CLASS_MEMBER(GuiComboBoxBase)

			BEGIN_CLASS_MEMBER(GuiComboBoxBase::ICommandExecutor)
				CLASS_MEMBER_BASE(IDescriptable)
				
				CLASS_MEMBER_METHOD(ShowPopup, NO_PARAMETER)
				CLASS_MEMBER_METHOD(SelectItem, NO_PARAMETER)
			END_CLASS_MEMBER(GuiComboBoxBase::ICommandExecutor)

			BEGIN_CLASS_MEMBER(GuiComboBoxBase::IStyleController)
				CLASS_MEMBER_BASE(GuiButton::IStyleController)
				INTERFACE_EXTERNALCTOR(GuiComboBoxBase, IStyleController)
				
				CLASS_MEMBER_METHOD(SetCommandExecutor, {L"value"})
				CLASS_MEMBER_METHOD(OnClicked, NO_PARAMETER)
				CLASS_MEMBER_METHOD(OnPopupOpened, NO_PARAMETER)
				CLASS_MEMBER_METHOD(OnPopupClosed, NO_PARAMETER)
				CLASS_MEMBER_METHOD(OnItemSelected, NO_PARAMETER)
				CLASS_MEMBER_METHOD(CreatePopupStyle, NO_PARAMETER)
			END_CLASS_MEMBER(GuiComboBoxBase::IStyleController)

			BEGIN_CLASS_MEMBER(GuiComboBoxListControl)
				CLASS_MEMBER_BASE(GuiComboBoxBase)
				CLASS_MEMBER_CONSTRUCTOR(GuiComboBoxListControl*(GuiComboBoxListControl::IStyleController* _ GuiSelectableListControl*), {L"styleController" _ L"containedListControl"})

				CLASS_MEMBER_PROPERTY_FAST(Font)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ContainedListControl)
				CLASS_MEMBER_PROPERTY_FAST(SelectedIndex)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ItemProvider)
			END_CLASS_MEMBER(GuiComboBoxListControl)

			BEGIN_CLASS_MEMBER(GuiToolstripCommand)
				CLASS_MEMBER_BASE(GuiComponent)
				CLASS_MEMBER_CONSTRUCTOR(GuiToolstripCommand*(), NO_PARAMETER)

				CLASS_MEMBER_PROPERTY_FAST(Image)
				CLASS_MEMBER_PROPERTY_FAST(Text)
				CLASS_MEMBER_PROPERTY_FAST(Shortcut)
				CLASS_MEMBER_PROPERTY_FAST(Enabled)
			END_CLASS_MEMBER(GuiToolstripCommand)

			BEGIN_CLASS_MEMBER(GuiToolstripMenu)
				CLASS_MEMBER_BASE(GuiMenu)
				CLASS_MEMBER_CONSTRUCTOR(GuiToolstripMenu*(GuiToolstripMenu::IStyleController* _ GuiControl*), {L"styleController" _ L"owner"})

				CLASS_MEMBER_EXTERNALMETHOD(GetToolstripItems, NO_PARAMETER, Ptr<IValueList>(GuiToolstripMenu::*)(), &GuiToolstripMenu_GetToolstripItems)
				CLASS_MEMBER_PROPERTY_READONLY(ToolstripItems, GetToolstripItems)
			END_CLASS_MEMBER(GuiToolstripMenu)

			BEGIN_CLASS_MEMBER(GuiToolstripMenuBar)
				CLASS_MEMBER_BASE(GuiMenuBar)
				CONTROL_CONSTRUCTOR_CONTROLLER(GuiToolstripMenuBar)

				CLASS_MEMBER_EXTERNALMETHOD(GetToolstripItems, NO_PARAMETER, Ptr<IValueList>(GuiToolstripMenuBar::*)(), &GuiToolstripMenuBar_GetToolstripItems)
				CLASS_MEMBER_PROPERTY_READONLY(ToolstripItems, GetToolstripItems)
			END_CLASS_MEMBER(GuiToolstripMenuBar)

			BEGIN_CLASS_MEMBER(GuiToolstripToolbar)
				CLASS_MEMBER_BASE(GuiControl)
				CONTROL_CONSTRUCTOR_CONTROLLER(GuiToolstripToolbar)

				CLASS_MEMBER_EXTERNALMETHOD(GetToolstripItems, NO_PARAMETER, Ptr<IValueList>(GuiToolstripToolbar::*)(), &GuiToolstripToolbar_GetToolstripItems)
				CLASS_MEMBER_PROPERTY_READONLY(ToolstripItems, GetToolstripItems)
			END_CLASS_MEMBER(GuiToolstripToolbar)

			BEGIN_CLASS_MEMBER(GuiToolstripButton)
				CLASS_MEMBER_BASE(GuiMenuButton)
				CONTROL_CONSTRUCTOR_CONTROLLER(GuiToolstripToolbar)

				CLASS_MEMBER_PROPERTY_FAST(Command)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ToolstripSubMenu)

				CLASS_MEMBER_METHOD(CreateToolstripSubMenu, {L"subMenuStyleController"})
			END_CLASS_MEMBER(GuiToolstripButton)

			BEGIN_CLASS_MEMBER(GuiDocumentCommonInterface)
				CLASS_MEMBER_PROPERTY_FAST(Document)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ActiveHyperlinkId)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ActiveHyperlinkReference)

				CLASS_MEMBER_METHOD(NotifyParagraphUpdated, {L"index"})
			END_CLASS_MEMBER(GuiDocumentCommonInterface)

			BEGIN_CLASS_MEMBER(GuiDocumentViewer)
				CLASS_MEMBER_BASE(GuiScrollContainer)
				CLASS_MEMBER_BASE(GuiDocumentCommonInterface)
				CONTROL_CONSTRUCTOR_PROVIDER(GuiDocumentViewer)
			END_CLASS_MEMBER(GuiDocumentViewer)

			BEGIN_CLASS_MEMBER(GuiDocumentLabel)
				CLASS_MEMBER_BASE(GuiControl)
				CLASS_MEMBER_BASE(GuiDocumentCommonInterface)
				CONTROL_CONSTRUCTOR_CONTROLLER(GuiDocumentLabel)
			END_CLASS_MEMBER(GuiDocumentLabel)

			BEGIN_CLASS_MEMBER(GuiTextBoxCommonInterface)
				CLASS_MEMBER_PROPERTY_FAST(Readonly)
				CLASS_MEMBER_PROPERTY_FAST(SelectionText)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(CaretBegin)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(CaretEnd)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(CaretSmall)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(CaretLarge)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(RowHeight)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(MaxWidth)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(MaxHeight)
				CLASS_MEMBER_PROPERTY_FAST(Colorizer)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Modified)

				CLASS_MEMBER_METHOD(CanCut, NO_PARAMETER)
				CLASS_MEMBER_METHOD(CanCopy, NO_PARAMETER)
				CLASS_MEMBER_METHOD(CanPaste, NO_PARAMETER)
				CLASS_MEMBER_METHOD(Cut, NO_PARAMETER)
				CLASS_MEMBER_METHOD(Copy, NO_PARAMETER)
				CLASS_MEMBER_METHOD(Paste, NO_PARAMETER)
				CLASS_MEMBER_METHOD(SelectAll, NO_PARAMETER)
				CLASS_MEMBER_METHOD(Select, {L"begin" _ L"end"})
				CLASS_MEMBER_METHOD(GetRowText, {L"row"})
				CLASS_MEMBER_METHOD(GetFragmentText, {L"start" _ L"end"})
				CLASS_MEMBER_METHOD(GetRowWidth, {L"row"})
				CLASS_MEMBER_METHOD(GetTextPosFromPoint, {L"point"})
				CLASS_MEMBER_METHOD(GetPointFromTextPos, {L"pos"})
				CLASS_MEMBER_METHOD(GetRectFromTextPos, {L"pos"})
				CLASS_MEMBER_METHOD(GetNearestTextPos, {L"point"})
				CLASS_MEMBER_METHOD(CanUndo, NO_PARAMETER)
				CLASS_MEMBER_METHOD(CanRedo, NO_PARAMETER)
				CLASS_MEMBER_METHOD(ClearUndoRedo, NO_PARAMETER)
				CLASS_MEMBER_METHOD(NotifyModificationSaved, NO_PARAMETER)
				CLASS_MEMBER_METHOD(Undo, NO_PARAMETER)
				CLASS_MEMBER_METHOD(Redo, NO_PARAMETER)
			END_CLASS_MEMBER(GuiTextBoxCommonInterface)

			BEGIN_CLASS_MEMBER(GuiTextBoxColorizerBase)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(LexerStartState)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ContextStartState)

				CLASS_MEMBER_METHOD(RestartColorizer, NO_PARAMETER)
				CLASS_MEMBER_EXTERNALMETHOD(GetColors, NO_PARAMETER, Ptr<IValueReadonlyList>(GuiTextBoxColorizerBase::*)(), &GuiTextBoxColorizerBase_GetColors)
				CLASS_MEMBER_PROPERTY_READONLY(Colors, GetColors)
			END_CLASS_MEMBER(GuiTextBoxColorizerBase)

			BEGIN_CLASS_MEMBER(GuiTextBoxRegexColorizer)
				CLASS_MEMBER_BASE(GuiTextBoxColorizerBase)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<GuiTextBoxRegexColorizer>(), NO_PARAMETER)

				CLASS_MEMBER_PROPERTY_FAST(DefaultColor)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ExtraTokenIndexStart)

				CLASS_MEMBER_EXTERNALMETHOD(GetTokenRegexes, NO_PARAMETER, Ptr<IValueList>(GuiTextBoxRegexColorizer::*)(), &GuiTextBoxRegexColorizer_GetTokenRegexes)
				CLASS_MEMBER_PROPERTY_READONLY(TokenRegexes, GetTokenRegexes)
				CLASS_MEMBER_EXTERNALMETHOD(GetTokenColors, NO_PARAMETER, Ptr<IValueList>(GuiTextBoxRegexColorizer::*)(), &GuiTextBoxRegexColorizer_GetTokenColors)
				CLASS_MEMBER_PROPERTY_READONLY(TokenColors, GetTokenColors)
				CLASS_MEMBER_EXTERNALMETHOD(GetExtraTokenColors, NO_PARAMETER, Ptr<IValueList>(GuiTextBoxRegexColorizer::*)(), &GuiTextBoxRegexColorizer_GetExtraTokenColors)
				CLASS_MEMBER_PROPERTY_READONLY(ExtraTokenColors, GetExtraTokenColors)
				CLASS_MEMBER_METHOD(AddToken, {L"regex" _ L"color"})
				CLASS_MEMBER_METHOD(AddExtraToken, {L"color"})
				CLASS_MEMBER_METHOD(Setup, NO_PARAMETER)
			END_CLASS_MEMBER(GuiTextBoxRegexColorizer)

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
