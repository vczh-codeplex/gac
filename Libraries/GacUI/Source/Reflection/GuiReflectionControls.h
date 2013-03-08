/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Reflection: Basic

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_REFLECTION_GUIREFLECTIONCONTROLS
#define VCZH_PRESENTATION_REFLECTION_GUIREFLECTIONCONTROLS

#include "GuiReflectionCompositions.h"

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
			F(presentation::controls::GuiListControl)\
			F(presentation::controls::GuiListControl::IItemProviderCallback)\
			F(presentation::controls::GuiListControl::IItemArrangerCallback)\
			F(presentation::controls::GuiListControl::IItemPrimaryTextView)\
			F(presentation::controls::GuiListControl::KeyDirection)\
			F(presentation::controls::GuiListControl::IItemProvider)\
			F(presentation::controls::GuiListControl::IItemStyleController)\
			F(presentation::controls::GuiListControl::IItemStyleProvider)\
			F(presentation::controls::GuiListControl::IItemArranger)\
			F(presentation::controls::GuiListControl::IItemCoordinateTransformer)\
			F(presentation::controls::GuiSelectableListControl)\
			F(presentation::controls::GuiSelectableListControl::IItemStyleProvider)\
			F(presentation::controls::list::DefaultItemCoordinateTransformer)\
			F(presentation::controls::list::AxisAlignedItemCoordinateTransformer)\
			F(presentation::controls::list::AxisAlignedItemCoordinateTransformer::Alignment)\
			F(presentation::controls::list::RangedItemArrangerBase)\
			F(presentation::controls::list::FixedHeightItemArranger)\
			F(presentation::controls::list::FixedSizeMultiColumnItemArranger)\
			F(presentation::controls::list::FixedHeightMultiColumnItemArranger)\
			F(presentation::controls::list::ItemStyleControllerBase)\
			F(presentation::controls::list::TextItemStyleProvider)\
			F(presentation::controls::list::TextItemStyleProvider::ITextItemStyleProvider)\
			F(presentation::controls::list::TextItemStyleProvider::ITextItemView)\
			F(presentation::controls::list::TextItemStyleProvider::TextItemStyleController)\
			F(presentation::controls::list::TextItem)\
			F(presentation::controls::list::TextItemProvider)\
			F(presentation::controls::GuiVirtualTextList)\
			F(presentation::controls::GuiTextList)\
			F(presentation::controls::list::ListViewItemStyleProviderBase)\
			F(presentation::controls::list::ListViewItemStyleProviderBase::ListViewItemStyleController)\
			F(presentation::controls::GuiListViewColumnHeader)\
			F(presentation::controls::GuiListViewColumnHeader::ColumnSortingState)\
			F(presentation::controls::GuiListViewColumnHeader::IStyleController)\
			F(presentation::controls::GuiListViewBase)\
			F(presentation::controls::GuiListViewBase::IStyleProvider)\
			F(presentation::controls::list::ListViewItemStyleProvider)\
			F(presentation::controls::list::ListViewItemStyleProvider::IListViewItemView)\
			F(presentation::controls::list::ListViewItemStyleProvider::IListViewItemContent)\
			F(presentation::controls::list::ListViewItemStyleProvider::IListViewItemContentProvider)\
			F(presentation::controls::list::ListViewItemStyleProvider::ListViewContentItemStyleController)\
			F(presentation::controls::list::ListViewBigIconContentProvider)\
			F(presentation::controls::list::ListViewSmallIconContentProvider)\
			F(presentation::controls::list::ListViewListContentProvider)\
			F(presentation::controls::list::ListViewTileContentProvider)\
			F(presentation::controls::list::ListViewInformationContentProvider)\
			F(presentation::controls::list::ListViewColumnItemArranger)\
			F(presentation::controls::list::ListViewColumnItemArranger::IColumnItemViewCallback)\
			F(presentation::controls::list::ListViewColumnItemArranger::IColumnItemView)\
			F(presentation::controls::list::ListViewDetailContentProvider)\
			F(presentation::controls::list::ListViewItem)\
			F(presentation::controls::list::ListViewColumn)\
			F(presentation::controls::GuiVirtualListView)\
			F(presentation::controls::GuiListView)\
			F(presentation::controls::IGuiMenuService)\
			F(presentation::controls::IGuiMenuService::Direction)\
			F(presentation::controls::GuiMenu)\
			F(presentation::controls::GuiMenuBar)\
			F(presentation::controls::GuiMenuButton)\
			F(presentation::controls::GuiMenuButton::IStyleController)\

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
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetDefaultTextColor);
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
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(CreateTabPageStyleController);
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
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(CreateHorizontalScrollStyle);
					}

					GuiScroll::IStyleController* CreateVerticalScrollStyle()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(CreateVerticalScrollStyle);
					}

					vint GetDefaultScrollSize()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetDefaultScrollSize);
					}

					compositions::GuiGraphicsComposition* InstallBackground(compositions::GuiBoundsComposition* boundsComposition)override
					{
						return INVOKEGET_INTERFACE_PROXY(InstallBackground, boundsComposition);
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
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetMaximizedBox);
					}

					void SetMaximizedBox(bool visible)override
					{
						INVOKE_INTERFACE_PROXY(SetMaximizedBox, visible);
					}

					bool GetMinimizedBox()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetMinimizedBox);
					}

					void SetMinimizedBox(bool visible)override
					{
						INVOKE_INTERFACE_PROXY(SetMinimizedBox, visible);
					}

					bool GetBorder()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetBorder);
					}

					void SetBorder(bool visible)override
					{
						INVOKE_INTERFACE_PROXY(SetBorder, visible);
					}

					bool GetSizeBox()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetSizeBox);
					}

					void SetSizeBox(bool visible)override
					{
						INVOKE_INTERFACE_PROXY(SetSizeBox, visible);
					}

					bool GetIconVisible()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetIconVisible);
					}

					void SetIconVisible(bool visible)override
					{
						INVOKE_INTERFACE_PROXY(SetIconVisible, visible);
					}

					bool GetTitleBar()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetTitleBar);
					}

					void SetTitleBar(bool visible)override
					{
						INVOKE_INTERFACE_PROXY(SetTitleBar, visible);
					}
				};

				class GuiListControl_IItemProviderCallback : public Object, public virtual GuiListControl::IItemProviderCallback
				{
				protected:
					Ptr<IValueInterfaceProxy>			proxy;
				public:
					GuiListControl_IItemProviderCallback(Ptr<IValueInterfaceProxy> _proxy)
						:proxy(_proxy)
					{
					}

					static Ptr<GuiListControl::IItemProviderCallback> Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new GuiListControl_IItemProviderCallback(proxy);
					}

					void OnAttached(GuiListControl::IItemProvider* provider)override
					{
						INVOKE_INTERFACE_PROXY(OnAttached, provider);
					}

					void OnItemModified(vint start, vint count, vint newCount)override
					{
						INVOKE_INTERFACE_PROXY(OnItemModified, start, count, newCount);
					}
				};

				class GuiListControl_IItemPrimaryTextView : public Object, public virtual GuiListControl::IItemPrimaryTextView
				{
				protected:
					Ptr<IValueInterfaceProxy>			proxy;
				public:
					GuiListControl_IItemPrimaryTextView(Ptr<IValueInterfaceProxy> _proxy)
						:proxy(_proxy)
					{
					}

					static Ptr<GuiListControl::IItemPrimaryTextView> Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new GuiListControl_IItemPrimaryTextView(proxy);
					}

					WString GetPrimaryTextViewText(vint itemIndex)override
					{
						return INVOKEGET_INTERFACE_PROXY(GetPrimaryTextViewText, itemIndex);
					}

					bool ContainsPrimaryText(vint itemIndex)override
					{
						return INVOKEGET_INTERFACE_PROXY(ContainsPrimaryText, itemIndex);
					}
				};

				class GuiListControl_IItemProvider : public Object, public virtual GuiListControl::IItemProvider
				{
				protected:
					Ptr<IValueInterfaceProxy>			proxy;
				public:
					GuiListControl_IItemProvider(Ptr<IValueInterfaceProxy> _proxy)
						:proxy(_proxy)
					{
					}

					static GuiListControl::IItemProvider* Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new GuiListControl_IItemProvider(proxy);
					}

					bool AttachCallback(GuiListControl::IItemProviderCallback* value)override
					{
						return INVOKEGET_INTERFACE_PROXY(AttachCallback, value);
					}

					bool DetachCallback(GuiListControl::IItemProviderCallback* value)override
					{
						return INVOKEGET_INTERFACE_PROXY(DetachCallback, value);
					}

					vint Count()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(Count);
					}

					IDescriptable* RequestView(const WString& identifier)override
					{
						return INVOKEGET_INTERFACE_PROXY(RequestView, identifier);
					}

					void ReleaseView(IDescriptable* view)override
					{
						INVOKE_INTERFACE_PROXY(ReleaseView, view);
					}
				};

				class GuiListControl_IItemStyleController : public Object, public virtual GuiListControl::IItemStyleController
				{
				protected:
					Ptr<IValueInterfaceProxy>			proxy;
				public:
					GuiListControl_IItemStyleController(Ptr<IValueInterfaceProxy> _proxy)
						:proxy(_proxy)
					{
					}

					static GuiListControl::IItemStyleController* Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new GuiListControl_IItemStyleController(proxy);
					}

					GuiListControl::IItemStyleProvider* GetStyleProvider()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetStyleProvider);
					}

					vint GetItemStyleId()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetItemStyleId);
					}

					compositions::GuiBoundsComposition* GetBoundsComposition()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetBoundsComposition);
					}

					bool IsCacheable()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(IsCacheable);
					}

					bool IsInstalled()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(IsInstalled);
					}

					void OnInstalled()override
					{
						INVOKE_INTERFACE_PROXY_NOPARAM(OnInstalled);
					}

					void OnUninstalled()override
					{
						INVOKE_INTERFACE_PROXY_NOPARAM(OnUninstalled);
					}
				};

				class GuiListControl_IItemStyleProvider : public Object, public virtual GuiListControl::IItemStyleProvider
				{
				protected:
					Ptr<IValueInterfaceProxy>			proxy;
				public:
					GuiListControl_IItemStyleProvider(Ptr<IValueInterfaceProxy> _proxy)
						:proxy(_proxy)
					{
					}

					static Ptr<GuiListControl::IItemStyleProvider> Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new GuiListControl_IItemStyleProvider(proxy);
					}

					void AttachListControl(GuiListControl* value)override
					{
						INVOKE_INTERFACE_PROXY(AttachListControl, value);
					}

					void DetachListControl()override
					{
						INVOKE_INTERFACE_PROXY_NOPARAM(DetachListControl);
					}

					vint GetItemStyleId(vint itemIndex)override
					{
						return INVOKEGET_INTERFACE_PROXY(GetItemStyleId, itemIndex);
					}

					GuiListControl::IItemStyleController* CreateItemStyle(vint styleId)override
					{
						return INVOKEGET_INTERFACE_PROXY(CreateItemStyle, styleId);
					}

					void DestroyItemStyle(GuiListControl::IItemStyleController* style)override
					{
						INVOKE_INTERFACE_PROXY(DestroyItemStyle, style);
					}

					void Install(GuiListControl::IItemStyleController* style, vint itemIndex)override
					{
						INVOKE_INTERFACE_PROXY(Install, style, itemIndex);
					}
				};

				class GuiListControl_IItemArranger : public virtual GuiListControl_IItemProviderCallback, public virtual GuiListControl::IItemArranger
				{
				public:
					GuiListControl_IItemArranger(Ptr<IValueInterfaceProxy> _proxy)
						:GuiListControl_IItemProviderCallback(_proxy)
					{
					}

					static Ptr<GuiListControl::IItemArranger> Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new GuiListControl_IItemArranger(proxy);
					}

					void AttachListControl(GuiListControl* value)override
					{
						INVOKE_INTERFACE_PROXY(AttachListControl, value);
					}

					void DetachListControl()override
					{
						INVOKE_INTERFACE_PROXY(DetachListControl);
					}

					GuiListControl::IItemArrangerCallback* GetCallback()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetCallback);
					}

					void SetCallback(GuiListControl::IItemArrangerCallback* value)override
					{
						INVOKE_INTERFACE_PROXY(SetCallback, value);
					}

					Size GetTotalSize()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetTotalSize);
					}

					GuiListControl::IItemStyleController* GetVisibleStyle(vint itemIndex)override
					{
						return INVOKEGET_INTERFACE_PROXY(GetVisibleStyle, itemIndex);
					}

					vint GetVisibleIndex(GuiListControl::IItemStyleController* style)override
					{
						return INVOKEGET_INTERFACE_PROXY(GetVisibleIndex, style);
					}

					void OnViewChanged(Rect bounds)override
					{
						INVOKE_INTERFACE_PROXY(OnViewChanged, bounds);
					}

					vint FindItem(vint itemIndex, GuiListControl::KeyDirection key)override
					{
						return INVOKEGET_INTERFACE_PROXY(FindItem, itemIndex, key);
					}

					bool EnsureItemVisible(vint itemIndex)override
					{
						return INVOKEGET_INTERFACE_PROXY(EnsureItemVisible, itemIndex);
					}
				};

				class GuiListControl_IItemCoordinateTransformer : public Object, public virtual GuiListControl::IItemCoordinateTransformer
				{
				protected:
					Ptr<IValueInterfaceProxy>			proxy;
				public:
					GuiListControl_IItemCoordinateTransformer(Ptr<IValueInterfaceProxy> _proxy)
						:proxy(_proxy)
					{
					}

					static Ptr<GuiListControl::IItemCoordinateTransformer> Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new GuiListControl_IItemCoordinateTransformer(proxy);
					}

					Size RealSizeToVirtualSize(Size size)override
					{
						return INVOKEGET_INTERFACE_PROXY(RealSizeToVirtualSize, size);
					}

					Size VirtualSizeToRealSize(Size size)override
					{
						return INVOKEGET_INTERFACE_PROXY(VirtualSizeToRealSize, size);
					}

					Point RealPointToVirtualPoint(Size realFullSize, Point point)override
					{
						return INVOKEGET_INTERFACE_PROXY(RealPointToVirtualPoint, realFullSize, point);
					}

					Point VirtualPointToRealPoint(Size realFullSize, Point point)override
					{
						return INVOKEGET_INTERFACE_PROXY(VirtualPointToRealPoint, realFullSize, point);
					}

					Rect RealRectToVirtualRect(Size realFullSize, Rect rect)override
					{
						return INVOKEGET_INTERFACE_PROXY(RealRectToVirtualRect, realFullSize, rect);
					}

					Rect VirtualRectToRealRect(Size realFullSize, Rect rect)override
					{
						return INVOKEGET_INTERFACE_PROXY(VirtualRectToRealRect, realFullSize, rect);
					}

					Margin RealMarginToVirtualMargin(Margin margin)override
					{
						return INVOKEGET_INTERFACE_PROXY(RealMarginToVirtualMargin, margin);
					}

					Margin VirtualMarginToRealMargin(Margin margin)override
					{
						return INVOKEGET_INTERFACE_PROXY(VirtualMarginToRealMargin, margin);
					}

					GuiListControl::KeyDirection RealKeyDirectionToVirtualKeyDirection(GuiListControl::KeyDirection key)override
					{
						return INVOKEGET_INTERFACE_PROXY(RealKeyDirectionToVirtualKeyDirection, key);
					}
				};

				class GuiSelectableListControl_IItemStyleProvider : public virtual GuiListControl_IItemStyleProvider, public virtual GuiSelectableListControl::IItemStyleProvider
				{
				public:
					GuiSelectableListControl_IItemStyleProvider(Ptr<IValueInterfaceProxy> proxy)
						:GuiListControl_IItemStyleProvider(proxy)
					{
					}

					static Ptr<GuiSelectableListControl::IItemStyleProvider> Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new GuiSelectableListControl_IItemStyleProvider(proxy);
					}

					void SetStyleSelected(GuiListControl::IItemStyleController* style, bool value)override
					{
						INVOKE_INTERFACE_PROXY(SetStyleSelected, style, value);
					}
				};

				class TextItemStyleProvider_ITextItemStyleProvider : public Object, public virtual list::TextItemStyleProvider::ITextItemStyleProvider
				{
				protected:
					Ptr<IValueInterfaceProxy>			proxy;
				public:
					TextItemStyleProvider_ITextItemStyleProvider(Ptr<IValueInterfaceProxy> _proxy)
						:proxy(_proxy)
					{
					}

					static list::TextItemStyleProvider::ITextItemStyleProvider* Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new TextItemStyleProvider_ITextItemStyleProvider(proxy);
					}

					GuiSelectableButton::IStyleController* CreateBackgroundStyleController()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(CreateBackgroundStyleController);
					}

					GuiSelectableButton::IStyleController* CreateBulletStyleController()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(CreateBulletStyleController);
					}
				};

				class TextItemStyleProvider_ITextItemView : public virtual GuiListControl_IItemPrimaryTextView, public virtual list::TextItemStyleProvider::ITextItemView
				{
				public:
					TextItemStyleProvider_ITextItemView(Ptr<IValueInterfaceProxy> _proxy)
						:GuiListControl_IItemPrimaryTextView(_proxy)
					{
					}

					static Ptr<list::TextItemStyleProvider::ITextItemView> Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new TextItemStyleProvider_ITextItemView(proxy);
					}

					WString GetText(vint itemIndex)override
					{
						return INVOKEGET_INTERFACE_PROXY(GetText, itemIndex);
					}

					bool GetChecked(vint itemIndex)override
					{
						return INVOKEGET_INTERFACE_PROXY(GetChecked, itemIndex);
					}

					void SetCheckedSilently(vint itemIndex, bool value)override
					{
						INVOKE_INTERFACE_PROXY(SetCheckedSilently, itemIndex, value);
					}
				};

				class GuiListViewBase_IStyleProvider : public virtual GuiScrollView_IStyleProvider, public virtual GuiListViewBase::IStyleProvider
				{
				public:
					GuiListViewBase_IStyleProvider(Ptr<IValueInterfaceProxy> _proxy)
						:GuiControl_IStyleProvider(_proxy)
						,GuiScrollView_IStyleProvider(_proxy)
					{
					}

					static GuiListViewBase::IStyleProvider* Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new GuiListViewBase_IStyleProvider(proxy);
					}

					GuiSelectableButton::IStyleController* CreateItemBackground()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(CreateItemBackground);
					}

					GuiListViewColumnHeader::IStyleController* CreateColumnStyle()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(CreateColumnStyle);
					}

					Color GetPrimaryTextColor()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetPrimaryTextColor);
					}

					Color GetSecondaryTextColor()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetSecondaryTextColor);
					}

					Color GetItemSeparatorColor()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetItemSeparatorColor);
					}
				};

				class ListViewItemStyleProvider_IListViewItemView : public virtual GuiListControl_IItemPrimaryTextView, public virtual list::ListViewItemStyleProvider::IListViewItemView
				{
				public:
					ListViewItemStyleProvider_IListViewItemView(Ptr<IValueInterfaceProxy> _proxy)
						:GuiListControl_IItemPrimaryTextView(_proxy)
					{
					}

					static Ptr<list::ListViewItemStyleProvider::IListViewItemView> Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new ListViewItemStyleProvider_IListViewItemView(proxy);
					}

					Ptr<GuiImageData> GetSmallImage(vint itemIndex)override
					{
						return INVOKEGET_INTERFACE_PROXY(GetSmallImage, itemIndex);
					}

					Ptr<GuiImageData> GetLargeImage(vint itemIndex)override
					{
						return INVOKEGET_INTERFACE_PROXY(GetLargeImage, itemIndex);
					}

					WString GetText(vint itemIndex)override
					{
						return INVOKEGET_INTERFACE_PROXY(GetText, itemIndex);
					}

					WString GetSubItem(vint itemIndex, vint index)override
					{
						return INVOKEGET_INTERFACE_PROXY(GetSubItem, itemIndex, index);
					}

					vint GetDataColumnCount()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetDataColumnCount);
					}

					vint GetDataColumn(vint index)override
					{
						return INVOKEGET_INTERFACE_PROXY(GetDataColumn, index);
					}

					vint GetColumnCount()override
					{
						return INVOKEGET_INTERFACE_PROXY(GetColumnCount);
					}

					WString GetColumnText(vint index)override
					{
						return INVOKEGET_INTERFACE_PROXY(GetColumnText, index);
					}
				};

				class ListViewItemStyleProvider_IListViewItemContent : public Object, public virtual list::ListViewItemStyleProvider::IListViewItemContent
				{
				protected:
					Ptr<IValueInterfaceProxy>			proxy;
				public:
					ListViewItemStyleProvider_IListViewItemContent(Ptr<IValueInterfaceProxy> _proxy)
						:proxy(_proxy)
					{
					}

					static list::ListViewItemStyleProvider::IListViewItemContent* Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new ListViewItemStyleProvider_IListViewItemContent(proxy);
					}

					compositions::GuiBoundsComposition* GetContentComposition()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetContentComposition);
					}

					compositions::GuiBoundsComposition* GetBackgroundDecorator()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetBackgroundDecorator);
					}

					void Install(GuiListViewBase::IStyleProvider* styleProvider, list::ListViewItemStyleProvider::IListViewItemView* view, vint itemIndex)override
					{
						INVOKE_INTERFACE_PROXY(Install, styleProvider, view, itemIndex);
					}
				};

				class ListViewItemStyleProvider_IListViewItemContentProvider : public Object, public virtual list::ListViewItemStyleProvider::IListViewItemContentProvider
				{
				protected:
					Ptr<IValueInterfaceProxy>			proxy;
				public:
					ListViewItemStyleProvider_IListViewItemContentProvider(Ptr<IValueInterfaceProxy> _proxy)
						:proxy(_proxy)
					{
					}

					static list::ListViewItemStyleProvider::IListViewItemContentProvider* Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new ListViewItemStyleProvider_IListViewItemContentProvider(proxy);
					}

					GuiListControl::IItemCoordinateTransformer* CreatePreferredCoordinateTransformer()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(CreatePreferredCoordinateTransformer);
					}

					GuiListControl::IItemArranger* CreatePreferredArranger()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(CreatePreferredArranger);
					}

					list::ListViewItemStyleProvider::IListViewItemContent* CreateItemContent(const FontProperties& font)override
					{
						return INVOKEGET_INTERFACE_PROXY(CreateItemContent, font);
					}

					void AttachListControl(GuiListControl* value)override
					{
						INVOKE_INTERFACE_PROXY(AttachListControl, value);
					}

					void DetachListControl()override
					{
						INVOKE_INTERFACE_PROXY_NOPARAM(DetachListControl);
					}
				};

				class ListViewColumnItemArranger_IColumnItemView : public Object, public virtual list::ListViewColumnItemArranger::IColumnItemView
				{
				protected:
					Ptr<IValueInterfaceProxy>			proxy;
				public:
					ListViewColumnItemArranger_IColumnItemView(Ptr<IValueInterfaceProxy> _proxy)
						:proxy(_proxy)
					{
					}

					static Ptr<list::ListViewColumnItemArranger::IColumnItemView> Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new ListViewColumnItemArranger_IColumnItemView(proxy);
					}

					bool AttachCallback(list::ListViewColumnItemArranger::IColumnItemViewCallback* value)override
					{
						return INVOKEGET_INTERFACE_PROXY(AttachCallback, value);
					}

					bool DetachCallback(list::ListViewColumnItemArranger::IColumnItemViewCallback* value)override
					{
						return INVOKEGET_INTERFACE_PROXY(DetachCallback, value);
					}

					vint GetColumnCount()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetColumnCount);
					}

					WString GetColumnText(vint index)override
					{
						return INVOKEGET_INTERFACE_PROXY(GetColumnText, index);
					}

					vint GetColumnSize(vint index)override
					{
						return INVOKEGET_INTERFACE_PROXY(GetColumnSize, index);
					}

					void SetColumnSize(vint index, vint value)override
					{
						INVOKE_INTERFACE_PROXY(SetColumnSize, index, value);
					}

					GuiMenu* GetDropdownPopup(vint index)override
					{
						return INVOKEGET_INTERFACE_PROXY(GetDropdownPopup, index);
					}

					GuiListViewColumnHeader::ColumnSortingState GetSortingState(vint index)override
					{
						return INVOKEGET_INTERFACE_PROXY(GetSortingState, index);
					}
				};

				class GuiMenuButton_IStyleController : public virtual GuiButton_IStyleController, public virtual GuiMenuButton::IStyleController
				{
				public:
					GuiMenuButton_IStyleController(Ptr<IValueInterfaceProxy> _proxy)
						:GuiControl_IStyleController(_proxy)
						,GuiButton_IStyleController(_proxy)
					{
					}

					static GuiMenuButton::IStyleController* Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new GuiMenuButton_IStyleController(proxy);
					}

					GuiMenu::IStyleController* CreateSubMenuStyleController()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(CreateSubMenuStyleController);
					}

					void SetSubMenuExisting(bool value)override
					{
						INVOKE_INTERFACE_PROXY(SetSubMenuExisting, value);
					}

					void SetSubMenuOpening(bool value)override
					{
						INVOKE_INTERFACE_PROXY(SetSubMenuOpening, value);
					}

					GuiButton* GetSubMenuHost()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetSubMenuHost);
					}

					void SetImage(Ptr<GuiImageData> value)override
					{
						INVOKE_INTERFACE_PROXY(SetImage, value);
					}

					void SetShortcutText(const WString& value)override
					{
						INVOKE_INTERFACE_PROXY(SetShortcutText, value);
					}

					compositions::GuiSubComponentMeasurer::IMeasuringSource* GetMeasuringSource()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetMeasuringSource);
					}
				};

				class GuiListViewColumnHeader_IStyleController : public virtual GuiMenuButton_IStyleController, public virtual GuiListViewColumnHeader::IStyleController
				{
				public:
					GuiListViewColumnHeader_IStyleController(Ptr<IValueInterfaceProxy> _proxy)
						:GuiControl_IStyleController(_proxy)
						,GuiButton_IStyleController(_proxy)
						,GuiMenuButton_IStyleController(_proxy)
					{
					}

					static GuiListViewColumnHeader::IStyleController* Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new GuiListViewColumnHeader_IStyleController(proxy);
					}

					void SetColumnSortingState(GuiListViewColumnHeader::ColumnSortingState value)override
					{
						INVOKE_INTERFACE_PROXY(SetColumnSortingState, value);
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