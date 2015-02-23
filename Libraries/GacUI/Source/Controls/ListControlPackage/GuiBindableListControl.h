/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUIBINDABLELISTCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUIBINDABLELISTCONTROLS

#include "GuiTextListControls.h"
#include "GuiListViewControls.h"
#include "GuiTreeViewControls.h"
#include "GuiDataGridControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
GuiBindableTextList
***********************************************************************/

			/// <summary>A bindable Text list control.</summary>
			class GuiBindableTextList : public GuiVirtualTextList, public Description<GuiBindableTextList>
			{
			protected:
				class ItemSource
					: public list::ItemProviderBase
					, protected GuiListControl::IItemBindingView
					, protected list::TextItemStyleProvider::ITextItemView
				{
				protected:
					Ptr<EventHandler>								itemChangedEventHandler;
					Ptr<description::IValueReadonlyList>			itemSource;

				public:
					WString											textProperty;
					WString											checkedProperty;

				public:
					ItemSource(Ptr<description::IValueEnumerable> _itemSource);
					~ItemSource();

					description::Value								Get(vint index);
					void											UpdateBindingProperties();
					
					// ===================== GuiListControl::IItemProvider =====================

					vint											Count()override;
					IDescriptable*									RequestView(const WString& identifier)override;
					void											ReleaseView(IDescriptable* view)override;
					
					// ===================== GuiListControl::IItemBindingView =====================

					description::Value								GetBindingValue(vint itemIndex)override;
					
					// ===================== GuiListControl::IItemPrimaryTextView =====================

					WString											GetPrimaryTextViewText(vint itemIndex)override;
					bool											ContainsPrimaryText(vint itemIndex)override;
					
					// ===================== list::TextItemStyleProvider::ITextItemView =====================

					WString											GetText(vint itemIndex)override;
					bool											GetChecked(vint itemIndex)override;
					void											SetCheckedSilently(vint itemIndex, bool value)override;
				};

			protected:
				ItemSource*											itemSource;

			public:
				/// <summary>Create a bindable Text list control.</summary>
				/// <param name="_styleProvider">The style provider for this control.</param>
				/// <param name="_itemStyleProvider">The item style provider callback for this control.</param>
				/// <param name="_itemSource">The item source.</param>
				GuiBindableTextList(IStyleProvider* _styleProvider, list::TextItemStyleProvider::ITextItemStyleProvider* _itemStyleProvider, Ptr<description::IValueEnumerable> _itemSource);
				~GuiBindableTextList();
				
				/// <summary>Text property name changed event.</summary>
				compositions::GuiNotifyEvent						TextPropertyChanged;
				/// <summary>Checked property name changed event.</summary>
				compositions::GuiNotifyEvent						CheckedPropertyChanged;
				
				/// <summary>Get the text property name to get the item text from an item.</summary>
				/// <returns>The text property name.</returns>
				const WString&										GetTextProperty();
				/// <summary>Set the text property name to get the item text from an item.</summary>
				/// <param name="value">The text property name.</param>
				void												SetTextProperty(const WString& value);
				
				/// <summary>Get the checked property name to get the check state from an item.</summary>
				/// <returns>The checked property name.</returns>
				const WString&										GetCheckedProperty();
				/// <summary>Set the checked property name to get the check state from an item.</summary>
				/// <param name="value">The checked property name.</param>
				void												SetCheckedProperty(const WString& value);

				/// <summary>Get the selected item.</summary>
				/// <returns>Returns the selected item. If there are multiple selected items, or there is no selected item, null will be returned.</returns>
				description::Value									GetSelectedItem();
			};

/***********************************************************************
GuiBindableListView
***********************************************************************/
			
			/// <summary>A bindable List view control.</summary>
			class GuiBindableListView : public GuiVirtualListView, public Description<GuiBindableListView>
			{
			protected:
				class ItemSource;
			public:
				/// <summary>List view data column container.</summary>
				class ListViewDataColumns : public list::ItemsBase<vint>
				{
					friend class ItemSource;
				protected:
					ItemSource*										itemProvider;

					void NotifyUpdateInternal(vint start, vint count, vint newCount)override;
				public:
					/// <summary>Create a container.</summary>
					ListViewDataColumns();
					~ListViewDataColumns();
				};
				
				/// <summary>List view column container.</summary>
				class ListViewColumns : public list::ItemsBase<Ptr<list::ListViewColumn>>
				{
					friend class ItemSource;
				protected:
					ItemSource*										itemProvider;

					void NotifyUpdateInternal(vint start, vint count, vint newCount)override;
				public:
					/// <summary>Create a container.</summary>
					ListViewColumns();
					~ListViewColumns();
				};

			protected:
				class ItemSource
					: public list::ItemProviderBase
					, protected GuiListControl::IItemBindingView
					, protected virtual list::ListViewItemStyleProvider::IListViewItemView
					, protected virtual list::ListViewColumnItemArranger::IColumnItemView
				{
					friend class ListViewDataColumns;
					friend class ListViewColumns;
					typedef collections::List<list::ListViewColumnItemArranger::IColumnItemViewCallback*>		ColumnItemViewCallbackList;
				protected:
					ListViewDataColumns								dataColumns;
					ListViewColumns									columns;
					ColumnItemViewCallbackList						columnItemViewCallbacks;
					Ptr<EventHandler>								itemChangedEventHandler;
					Ptr<description::IValueReadonlyList>			itemSource;

				public:
					WString											largeImageProperty;
					WString											smallImageProperty;

				public:
					ItemSource(Ptr<description::IValueEnumerable> _itemSource);
					~ItemSource();
					
					description::Value								Get(vint index);
					void											UpdateBindingProperties();
					bool											NotifyUpdate(vint start, vint count);
					ListViewDataColumns&							GetDataColumns();
					ListViewColumns&								GetColumns();
					
					// ===================== GuiListControl::IItemProvider =====================

					vint											Count()override;
					IDescriptable*									RequestView(const WString& identifier)override;
					void											ReleaseView(IDescriptable* view)override;
					
					// ===================== GuiListControl::IItemBindingView =====================

					description::Value								GetBindingValue(vint itemIndex)override;

					// ===================== GuiListControl::IItemPrimaryTextView =====================

					WString											GetPrimaryTextViewText(vint itemIndex)override;
					bool											ContainsPrimaryText(vint itemIndex)override;

					// ===================== list::ListViewItemStyleProvider::IListViewItemView =====================

					Ptr<GuiImageData>								GetSmallImage(vint itemIndex)override;
					Ptr<GuiImageData>								GetLargeImage(vint itemIndex)override;
					WString											GetText(vint itemIndex)override;
					WString											GetSubItem(vint itemIndex, vint index)override;
					vint											GetDataColumnCount()override;
					vint											GetDataColumn(vint index)override;

					// ===================== list::ListViewColumnItemArranger::IColumnItemView =====================
						
					bool											AttachCallback(list::ListViewColumnItemArranger::IColumnItemViewCallback* value)override;
					bool											DetachCallback(list::ListViewColumnItemArranger::IColumnItemViewCallback* value)override;
					vint											GetColumnCount()override;
					WString											GetColumnText(vint index)override;
					vint											GetColumnSize(vint index)override;
					void											SetColumnSize(vint index, vint value)override;
					GuiMenu*										GetDropdownPopup(vint index)override;
					GuiListViewColumnHeader::ColumnSortingState		GetSortingState(vint index)override;
				};

			protected:
				ItemSource*											itemSource;

			public:
				/// <summary>Create a bindable List view control.</summary>
				/// <param name="_styleProvider">The style provider for this control.</param>
				/// <param name="_itemSource">The item source.</param>
				GuiBindableListView(IStyleProvider* _styleProvider, Ptr<description::IValueEnumerable> _itemSource);
				~GuiBindableListView();

				/// <summary>Get all data columns indices in columns.</summary>
				/// <returns>All data columns indices in columns.</returns>
				ListViewDataColumns&								GetDataColumns();
				/// <summary>Get all columns.</summary>
				/// <returns>All columns.</returns>
				ListViewColumns&									GetColumns();
				
				/// <summary>Large image property name changed event.</summary>
				compositions::GuiNotifyEvent						LargeImagePropertyChanged;
				/// <summary>Small image property name changed event.</summary>
				compositions::GuiNotifyEvent						SmallImagePropertyChanged;
				
				/// <summary>Get the large image property name to get the large image from an item.</summary>
				/// <returns>The large image property name.</returns>
				const WString&										GetLargeImageProperty();
				/// <summary>Set the large image property name to get the large image from an item.</summary>
				/// <param name="value">The large image property name.</param>
				void												SetLargeImageProperty(const WString& value);
				
				/// <summary>Get the small image property name to get the small image from an item.</summary>
				/// <returns>The small image property name.</returns>
				const WString&										GetSmallImageProperty();
				/// <summary>Set the small image property name to get the small image from an item.</summary>
				/// <param name="value">The small image property name.</param>
				void												SetSmallImageProperty(const WString& value);

				/// <summary>Get the selected item.</summary>
				/// <returns>Returns the selected item. If there are multiple selected items, or there is no selected item, null will be returned.</returns>
				description::Value									GetSelectedItem();
			};

/***********************************************************************
GuiBindableTreeView
***********************************************************************/
			
			/// <summary>A bindable Tree view control.</summary>
			class GuiBindableTreeView : public GuiVirtualTreeView, public Description<GuiBindableTreeView>
			{
			protected:
				class ItemSource;

				class ItemSourceNode
					: public Object
					, public virtual tree::INodeProvider
				{
					friend class ItemSource;
					typedef collections::List<Ptr<ItemSourceNode>>	NodeList;
				protected:
					description::Value								itemSource;
					ItemSource*										rootProvider;
					ItemSourceNode*									parent;
					tree::INodeProviderCallback*					callback;
					bool											expanding = false;

					Ptr<EventHandler>								itemChangedEventHandler;
					Ptr<description::IValueReadonlyList>			childrenVirtualList;
					NodeList										children;

					void											PrepareChildren();
					void											UnprepareChildren();
				public:
					ItemSourceNode(const description::Value& _itemSource, ItemSourceNode* _parent);
					ItemSourceNode(const description::Value& _itemSource, ItemSource* _rootProvider);
					~ItemSourceNode();

					description::Value								GetItemSource();

					// ===================== tree::INodeProvider =====================

					bool											GetExpanding()override;
					void											SetExpanding(bool value)override;
					vint											CalculateTotalVisibleNodes()override;

					vint											GetChildCount()override;
					tree::INodeProvider*							GetParent()override;
					tree::INodeProvider*							GetChild(vint index)override;
					void											Increase()override;
					void											Release()override;
				};

				class ItemSource
					: public tree::NodeRootProviderBase
					, protected virtual tree::INodeItemBindingView
					, protected virtual tree::ITreeViewItemView
				{
					friend class ItemSourceNode;
				public:
					WString											textProperty;
					WString											imageProperty;
					WString											childrenProperty;
					Ptr<ItemSourceNode>								rootNode;

				public:
					ItemSource(const description::Value& _itemSource);
					~ItemSource();

					void											UpdateBindingProperties(bool updateChildrenProperty);

					// ===================== tree::INodeRootProvider =====================

					tree::INodeProvider*							GetRootNode()override;
					IDescriptable*									RequestView(const WString& identifier)override;
					void											ReleaseView(IDescriptable* view)override;

					// ===================== tree::INodeItemBindingView =====================

					description::Value								GetBindingValue(tree::INodeProvider* node)override;

					// ===================== tree::INodeItemPrimaryTextView =====================

					WString											GetPrimaryTextViewText(tree::INodeProvider* node)override;

					// ===================== tree::ITreeViewItemView =====================

					Ptr<GuiImageData>								GetNodeImage(tree::INodeProvider* node)override;
					WString											GetNodeText(tree::INodeProvider* node)override;
				};

			protected:
				ItemSource*											itemSource;

			public:
				/// <summary>Create a bindable Tree view control.</summary>
				/// <param name="_styleProvider">The style provider for this control.</param>
				/// <param name="_itemSource">The item source.</param>
				GuiBindableTreeView(IStyleProvider* _styleProvider, const description::Value& _itemSource);
				~GuiBindableTreeView();
				
				/// <summary>Text property name changed event.</summary>
				compositions::GuiNotifyEvent						TextPropertyChanged;
				/// <summary>Image property name changed event.</summary>
				compositions::GuiNotifyEvent						ImagePropertyChanged;
				/// <summary>Children property name changed event.</summary>
				compositions::GuiNotifyEvent						ChildrenPropertyChanged;
				
				/// <summary>Get the text property name to get the item text from an item.</summary>
				/// <returns>The text property name.</returns>
				const WString&										GetTextProperty();
				/// <summary>Set the text property name to get the item text from an item.</summary>
				/// <param name="value">The text property name.</param>
				void												SetTextProperty(const WString& value);
				
				/// <summary>Get the image property name to get the image from an item.</summary>
				/// <returns>The image property name.</returns>
				const WString&										GetImageProperty();
				/// <summary>Set the image property name to get the image from an item.</summary>
				/// <param name="value">The image property name.</param>
				void												SetImageProperty(const WString& value);
				
				/// <summary>Get the children property name to get the children from an item.</summary>
				/// <returns>The children property name.</returns>
				const WString&										GetChildrenProperty();
				/// <summary>Set the children property name to get the children from an item.</summary>
				/// <param name="value">The children property name.</param>
				void												SetChildrenProperty(const WString& value);

				/// <summary>Get the selected item.</summary>
				/// <returns>Returns the selected item. If there are multiple selected items, or there is no selected item, null will be returned.</returns>
				description::Value									GetSelectedItem();
			};

/***********************************************************************
GuiBindableDataGrid
***********************************************************************/
			
			/// <summary>Column object for [T:vl.presentation.controls.GuiBindableDataGrid].</summary>
			class GuiBindableDataColumn : public list::StructuredColummProviderBase
			{
				friend class GuiBindableDataProvider;
			protected:
				Ptr<description::IValueReadonlyList>				itemSource;

				void												SetItemSource(Ptr<description::IValueReadonlyList> _itemSource);
			public:
				GuiBindableDataColumn();
				~GuiBindableDataColumn();

				WString												GetCellText(vint row)override;
			};
			
			/// <summary>Data provider object for [T:vl.presentation.controls.GuiBindableDataGrid].</summary>
			class GuiBindableDataProvider :public list::StructuredDataProviderBase
			{
			protected:
				Ptr<description::IValueReadonlyList>				itemSource;
				Ptr<EventHandler>									itemChangedEventHandler;

			public:
				GuiBindableDataProvider(Ptr<description::IValueEnumerable> _itemSource);
				~GuiBindableDataProvider();

				vint												GetRowCount()override;

				bool												InsertBindableColumn(vint index, Ptr<GuiBindableDataColumn> column);
				bool												AddBindableColumn(Ptr<GuiBindableDataColumn> column);
				bool												RemoveBindableColumn(Ptr<GuiBindableDataColumn> column);
				bool												ClearBindableColumns();
				Ptr<GuiBindableDataColumn>							GetBindableColumn(vint index);
			};
			
			/// <summary>A bindable Data grid control.</summary>
			class GuiBindableDataGrid : public GuiVirtualDataGrid, public Description<GuiBindableDataGrid>
			{
			protected:
				GuiBindableDataProvider*							bindableDataProvider;

			public:
				/// <summary>Create a bindable Data grid control.</summary>
				/// <param name="_styleProvider">The style provider for this control.</param>
				/// <param name="_itemSource">The item source.</param>
				GuiBindableDataGrid(IStyleProvider* _styleProvider, Ptr<description::IValueEnumerable> _itemSource);
				~GuiBindableDataGrid();
				
				/// <summary>Insert a column.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="index">The column index.</param>
				/// <param name="column">The column.</param>
				bool												InsertBindableColumn(vint index, Ptr<GuiBindableDataColumn> column);
				/// <summary>Add a column.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="column">The column.</param>
				bool												AddBindableColumn(Ptr<GuiBindableDataColumn> column);
				/// <summary>Remove a column.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="column">The column.</param>
				bool												RemoveBindableColumn(Ptr<GuiBindableDataColumn> column);
				/// <summary>Clear all columns.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				bool												ClearBindableColumns();
				/// <summary>Get a column.</summary>
				/// <param name="index">The column index.</param>
				/// <returns>Returns the column of a specified index.</returns>
				Ptr<GuiBindableDataColumn>							GetBindableColumn(vint index);

				/// <summary>Get the selected item.</summary>
				/// <returns>Returns the selected item. If there are multiple selected items, or there is no selected item, null will be returned.</returns>
				description::Value									GetSelectedItem();
			};
		}
	}
}

#endif