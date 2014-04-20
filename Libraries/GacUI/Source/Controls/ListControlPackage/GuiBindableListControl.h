/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
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

			class GuiBindableTextList : public GuiVirtualTextList, public Description<GuiBindableTextList>
			{
			protected:
				class ItemSource
					: public Object
					, public virtual GuiListControl::IItemProvider
					, protected list::TextItemStyleProvider::ITextItemView
				{
				public:
					ItemSource(Ptr<description::IValueEnumerable> itemSource);
					~ItemSource();
					
					// ===================== GuiListControl::IItemProvider =====================

					bool								AttachCallback(IItemProviderCallback* value)override;
					bool								DetachCallback(IItemProviderCallback* value)override;
					vint								Count()override;
					IDescriptable*						RequestView(const WString& identifier)override;
					void								ReleaseView(IDescriptable* view)override;
					
					// ===================== GuiListControl::IItemPrimaryTextView =====================

					WString								GetPrimaryTextViewText(vint itemIndex)override;
					bool								ContainsPrimaryText(vint itemIndex)override;
					
					// ===================== list::TextItemStyleProvider::ITextItemView =====================

					WString								GetText(vint itemIndex)override;
					bool								GetChecked(vint itemIndex)override;
					void								SetCheckedSilently(vint itemIndex, bool value)override;
				};
			public:
				GuiBindableTextList(IStyleProvider* _styleProvider, list::TextItemStyleProvider::ITextItemStyleProvider* _itemStyleProvider, Ptr<description::IValueEnumerable> itemSource);
				~GuiBindableTextList();
			};

/***********************************************************************
GuiBindableListView
***********************************************************************/

			class GuiBindableListView : public GuiVirtualListView, public Description<GuiBindableListView>
			{
			protected:
				class ItemSource
					: public Object
					, public virtual GuiListControl::IItemProvider
					, protected virtual list::ListViewItemStyleProvider::IListViewItemView
					, protected virtual list::ListViewColumnItemArranger::IColumnItemView
				{
				public:
					ItemSource(Ptr<description::IValueEnumerable> itemSource);
					~ItemSource();
					
					// ===================== GuiListControl::IItemProvider =====================

					bool								AttachCallback(IItemProviderCallback* value)override;
					bool								DetachCallback(IItemProviderCallback* value)override;
					vint								Count()override;
					IDescriptable*						RequestView(const WString& identifier)override;
					void								ReleaseView(IDescriptable* view)override;

					// ===================== GuiListControl::IItemPrimaryTextView =====================

					WString								GetPrimaryTextViewText(vint itemIndex)override;
					bool								ContainsPrimaryText(vint itemIndex)override;

					// ===================== list::ListViewItemStyleProvider::IListViewItemView =====================

					Ptr<GuiImageData>					GetSmallImage(vint itemIndex)override;
					Ptr<GuiImageData>					GetLargeImage(vint itemIndex)override;
					WString								GetText(vint itemIndex)override;
					WString								GetSubItem(vint itemIndex, vint index)override;
					vint								GetDataColumnCount()override;
					vint								GetDataColumn(vint index)override;

					// ===================== list::ListViewColumnItemArranger::IColumnItemView =====================
						
					bool								AttachCallback(list::ListViewColumnItemArranger::IColumnItemViewCallback* value)override;
					bool								DetachCallback(list::ListViewColumnItemArranger::IColumnItemViewCallback* value)override;
					vint								GetColumnCount()override;
					WString								GetColumnText(vint index)override;
					vint								GetColumnSize(vint index)override;
					void								SetColumnSize(vint index, vint value)override;
					GuiMenu*							GetDropdownPopup(vint index)override;
					GuiListViewColumnHeader::ColumnSortingState			GetSortingState(vint index)override;
				};
			public:
				GuiBindableListView(IStyleProvider* _styleProvider, Ptr<description::IValueEnumerable> itemSource);
				~GuiBindableListView();
			};

/***********************************************************************
GuiBindableTreeView
***********************************************************************/

			class GuiBindableTreeView : public GuiVirtualTreeView, public Description<GuiBindableTreeView>
			{
			protected:
				class ItemSource
					: public Object
					, public virtual tree::INodeRootProvider
					, protected virtual tree::ITreeViewItemView
				{
				public:
					ItemSource(Ptr<description::IValueEnumerable> itemSource);
					~ItemSource();

					// ===================== tree::INodeRootProvider =====================

					tree::INodeProvider*			GetRootNode()override;
					bool							CanGetNodeByVisibleIndex()override;
					tree::INodeProvider*			GetNodeByVisibleIndex(vint index)override;
					bool							AttachCallback(tree::INodeProviderCallback* value)override;
					bool							DetachCallback(tree::INodeProviderCallback* value)override;
					IDescriptable*					RequestView(const WString& identifier)override;
					void							ReleaseView(IDescriptable* view)override;

					// ===================== tree::INodeItemPrimaryTextView =====================

					WString							GetPrimaryTextViewText(tree::INodeProvider* node)override;

					// ===================== tree::ITreeViewItemView =====================

					Ptr<GuiImageData>				GetNodeImage(tree::INodeProvider* node)override;
					WString							GetNodeText(tree::INodeProvider* node)override;
				};
			public:
				GuiBindableTreeView(IStyleProvider* _styleProvider, Ptr<description::IValueEnumerable> itemSource);
				~GuiBindableTreeView();
			};

/***********************************************************************
GuiBindableDataGrid
***********************************************************************/

			class GuiBindableDataGrid : public GuiVirtualDataGrid, public Description<GuiBindableDataGrid>
			{
			protected:
				class ItemSource
					: public Object
					, public virtual list::IDataProvider
				{
				public:
					ItemSource(Ptr<description::IValueEnumerable> itemSource);
					~ItemSource();

					// ===================== list::IDataProvider =====================

					void							SetCommandExecutor(list::IDataProviderCommandExecutor* value)override;
					vint							GetColumnCount()override;
					WString							GetColumnText(vint column)override;
					vint							GetColumnSize(vint column)override;
					void							SetColumnSize(vint column, vint value)override;
					GuiMenu*						GetColumnPopup(vint column)override;
					bool							IsColumnSortable(vint column)override;
					void							SortByColumn(vint column, bool ascending)override;
					vint							GetSortedColumn()override;
					bool							IsSortOrderAscending()override;
					
					vint							GetRowCount()override;
					Ptr<GuiImageData>				GetRowLargeImage(vint row)override;
					Ptr<GuiImageData>				GetRowSmallImage(vint row)override;
					vint							GetCellSpan(vint row, vint column)override;
					WString							GetCellText(vint row, vint column)override;
					list::IDataVisualizerFactory*	GetCellDataVisualizerFactory(vint row, vint column)override;
					void							VisualizeCell(vint row, vint column, list::IDataVisualizer* dataVisualizer)override;
					list::IDataEditorFactory*		GetCellDataEditorFactory(vint row, vint column)override;
					void							BeforeEditCell(vint row, vint column, list::IDataEditor* dataEditor)override;
					void							SaveCellData(vint row, vint column, list::IDataEditor* dataEditor)override;
				};
			public:
				GuiBindableDataGrid(IStyleProvider* _styleProvider, Ptr<description::IValueEnumerable> itemSource);
				~GuiBindableDataGrid();
			};
		}
	}
}

#endif