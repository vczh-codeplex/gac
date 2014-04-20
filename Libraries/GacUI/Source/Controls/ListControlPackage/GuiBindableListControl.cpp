#include "GuiBindableListControl.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
GuiBindableTextList::ItemSource
***********************************************************************/

			GuiBindableTextList::ItemSource::ItemSource(Ptr<description::IValueEnumerable> itemSource)
			{
				throw 0;
			}

			GuiBindableTextList::ItemSource::~ItemSource()
			{
				throw 0;
			}
					
			// ===================== GuiListControl::IItemProvider =====================

			bool GuiBindableTextList::ItemSource::AttachCallback(IItemProviderCallback* value)
			{
				throw 0;
			}
			
			bool GuiBindableTextList::ItemSource::DetachCallback(IItemProviderCallback* value)
			{
				throw 0;
			}
			
			vint GuiBindableTextList::ItemSource::Count()
			{
				throw 0;
			}
			
			IDescriptable* GuiBindableTextList::ItemSource::RequestView(const WString& identifier)
			{
				throw 0;
			}
			
			void GuiBindableTextList::ItemSource::ReleaseView(IDescriptable* view)
			{
				throw 0;
			}
					
			// ===================== GuiListControl::IItemPrimaryTextView =====================

			WString GuiBindableTextList::ItemSource::GetPrimaryTextViewText(vint itemIndex)
			{
				throw 0;
			}
			
			bool GuiBindableTextList::ItemSource::ContainsPrimaryText(vint itemIndex)
			{
				throw 0;
			}
					
			// ===================== list::TextItemStyleProvider::ITextItemView =====================

			WString GuiBindableTextList::ItemSource::GetText(vint itemIndex)
			{
				throw 0;
			}
			
			bool GuiBindableTextList::ItemSource::GetChecked(vint itemIndex)
			{
				throw 0;
			}
			
			void GuiBindableTextList::ItemSource::SetCheckedSilently(vint itemIndex, bool value)
			{
				throw 0;
			}

/***********************************************************************
GuiBindableTextList
***********************************************************************/

			GuiBindableTextList::GuiBindableTextList(IStyleProvider* _styleProvider, list::TextItemStyleProvider::ITextItemStyleProvider* _itemStyleProvider, Ptr<description::IValueEnumerable> itemSource)
				:GuiVirtualTextList(_styleProvider, _itemStyleProvider, new ItemSource(itemSource))
			{
			}

			GuiBindableTextList::~GuiBindableTextList()
			{
			}

/***********************************************************************
GuiBindableListView::ItemSource
***********************************************************************/

			GuiBindableListView::ItemSource::ItemSource(Ptr<description::IValueEnumerable> itemSource)
			{
				throw 0;
			}

			GuiBindableListView::ItemSource::~ItemSource()
			{
				throw 0;
			}

			// ===================== GuiListControl::IItemProvider =====================

			bool GuiBindableListView::ItemSource::AttachCallback(IItemProviderCallback* value)
			{
				throw 0;
			}

			bool GuiBindableListView::ItemSource::DetachCallback(IItemProviderCallback* value)
			{
				throw 0;
			}

			vint GuiBindableListView::ItemSource::Count()
			{
				throw 0;
			}

			IDescriptable* GuiBindableListView::ItemSource::RequestView(const WString& identifier)
			{
				throw 0;
			}

			void GuiBindableListView::ItemSource::ReleaseView(IDescriptable* view)
			{
				throw 0;
			}

			// ===================== GuiListControl::IItemPrimaryTextView =====================

			WString GuiBindableListView::ItemSource::GetPrimaryTextViewText(vint itemIndex)
			{
				throw 0;
			}

			bool GuiBindableListView::ItemSource::ContainsPrimaryText(vint itemIndex)
			{
				throw 0;
			}

			// ===================== list::ListViewItemStyleProvider::IListViewItemView =====================

			Ptr<GuiImageData> GuiBindableListView::ItemSource::GetSmallImage(vint itemIndex)
			{
				throw 0;
			}

			Ptr<GuiImageData> GuiBindableListView::ItemSource::GetLargeImage(vint itemIndex)
			{
				throw 0;
			}

			WString GuiBindableListView::ItemSource::GetText(vint itemIndex)
			{
				throw 0;
			}

			WString GuiBindableListView::ItemSource::GetSubItem(vint itemIndex, vint index)
			{
				throw 0;
			}

			vint GuiBindableListView::ItemSource::GetDataColumnCount()
			{
				throw 0;
			}

			vint GuiBindableListView::ItemSource::GetDataColumn(vint index)
			{
				throw 0;
			}

			// ===================== list::ListViewColumnItemArranger::IColumnItemView =====================
						
			bool GuiBindableListView::ItemSource::AttachCallback(list::ListViewColumnItemArranger::IColumnItemViewCallback* value)
			{
				throw 0;
			}

			bool GuiBindableListView::ItemSource::DetachCallback(list::ListViewColumnItemArranger::IColumnItemViewCallback* value)
			{
				throw 0;
			}

			vint GuiBindableListView::ItemSource::GetColumnCount()
			{
				throw 0;
			}

			WString GuiBindableListView::ItemSource::GetColumnText(vint index)
			{
				throw 0;
			}

			vint GuiBindableListView::ItemSource::GetColumnSize(vint index)
			{
				throw 0;
			}

			void GuiBindableListView::ItemSource::SetColumnSize(vint index, vint value)
			{
				throw 0;
			}

			GuiMenu* GuiBindableListView::ItemSource::GetDropdownPopup(vint index)
			{
				throw 0;
			}

			GuiListViewColumnHeader::ColumnSortingState GuiBindableListView::ItemSource::GetSortingState(vint index)
			{
				throw 0;
			}

/***********************************************************************
GuiBindableListView
***********************************************************************/

			GuiBindableListView::GuiBindableListView(IStyleProvider* _styleProvider, Ptr<description::IValueEnumerable> itemSource)
				:GuiVirtualListView(_styleProvider, new ItemSource(itemSource))
			{
			}

			GuiBindableListView::~GuiBindableListView()
			{
			}

/***********************************************************************
GuiBindableTreeView::ItemSource
***********************************************************************/

			GuiBindableTreeView::ItemSource::ItemSource(Ptr<description::IValueEnumerable> itemSource)
			{
				throw 0;
			}

			GuiBindableTreeView::ItemSource::~ItemSource()
			{
				throw 0;
			}

			// ===================== tree::INodeRootProvider =====================

			tree::INodeProvider* GuiBindableTreeView::ItemSource::GetRootNode()
			{
				throw 0;
			}

			bool GuiBindableTreeView::ItemSource::CanGetNodeByVisibleIndex()
			{
				throw 0;
			}

			tree::INodeProvider* GuiBindableTreeView::ItemSource::GetNodeByVisibleIndex(vint index)
			{
				throw 0;
			}

			bool GuiBindableTreeView::ItemSource::AttachCallback(tree::INodeProviderCallback* value)
			{
				throw 0;
			}

			bool GuiBindableTreeView::ItemSource::DetachCallback(tree::INodeProviderCallback* value)
			{
				throw 0;
			}

			IDescriptable* GuiBindableTreeView::ItemSource::RequestView(const WString& identifier)
			{
				throw 0;
			}

			void GuiBindableTreeView::ItemSource::ReleaseView(IDescriptable* view)
			{
				throw 0;
			}

			// ===================== tree::INodeItemPrimaryTextView =====================

			WString GuiBindableTreeView::ItemSource::GetPrimaryTextViewText(tree::INodeProvider* node)
			{
				throw 0;
			}

			// ===================== tree::ITreeViewItemView =====================

			Ptr<GuiImageData> GuiBindableTreeView::ItemSource::GetNodeImage(tree::INodeProvider* node)
			{
				throw 0;
			}

			WString GuiBindableTreeView::ItemSource::GetNodeText(tree::INodeProvider* node)
			{
				throw 0;
			}

/***********************************************************************
GuiBindableTreeView
***********************************************************************/

			GuiBindableTreeView::GuiBindableTreeView(IStyleProvider* _styleProvider, Ptr<description::IValueEnumerable> itemSource)
				:GuiVirtualTreeView(_styleProvider, new ItemSource(itemSource))
			{
			}

			GuiBindableTreeView::~GuiBindableTreeView()
			{
			}

/***********************************************************************
GuiBindableDataGrid::ItemSource
***********************************************************************/

			GuiBindableDataGrid::ItemSource::ItemSource(Ptr<description::IValueEnumerable> itemSource)
			{
				throw 0;
			}

			GuiBindableDataGrid::ItemSource::~ItemSource()
			{
				throw 0;
			}

			// ===================== list::IDataProvider =====================

			void GuiBindableDataGrid::ItemSource::SetCommandExecutor(list::IDataProviderCommandExecutor* value)
			{
				throw 0;
			}

			vint GuiBindableDataGrid::ItemSource::GetColumnCount()
			{
				throw 0;
			}

			WString GuiBindableDataGrid::ItemSource::GetColumnText(vint column)
			{
				throw 0;
			}

			vint GuiBindableDataGrid::ItemSource::GetColumnSize(vint column)
			{
				throw 0;
			}

			void GuiBindableDataGrid::ItemSource::SetColumnSize(vint column, vint value)
			{
				throw 0;
			}

			GuiMenu* GuiBindableDataGrid::ItemSource::GetColumnPopup(vint column)
			{
				throw 0;
			}

			bool GuiBindableDataGrid::ItemSource::IsColumnSortable(vint column)
			{
				throw 0;
			}

			void GuiBindableDataGrid::ItemSource::SortByColumn(vint column, bool ascending)
			{
				throw 0;
			}

			vint GuiBindableDataGrid::ItemSource::GetSortedColumn()
			{
				throw 0;
			}

			bool GuiBindableDataGrid::ItemSource::IsSortOrderAscending()
			{
				throw 0;
			}

			vint GuiBindableDataGrid::ItemSource::GetRowCount()
			{
				throw 0;
			}

			Ptr<GuiImageData> GuiBindableDataGrid::ItemSource::GetRowLargeImage(vint row)
			{
				throw 0;
			}

			Ptr<GuiImageData> GuiBindableDataGrid::ItemSource::GetRowSmallImage(vint row)
			{
				throw 0;
			}

			vint GuiBindableDataGrid::ItemSource::GetCellSpan(vint row, vint column)
			{
				throw 0;
			}

			WString GuiBindableDataGrid::ItemSource::GetCellText(vint row, vint column)
			{
				throw 0;
			}

			list::IDataVisualizerFactory* GuiBindableDataGrid::ItemSource::GetCellDataVisualizerFactory(vint row, vint column)
			{
				throw 0;
			}

			void GuiBindableDataGrid::ItemSource::VisualizeCell(vint row, vint column, list::IDataVisualizer* dataVisualizer)
			{
				throw 0;
			}

			list::IDataEditorFactory* GuiBindableDataGrid::ItemSource::GetCellDataEditorFactory(vint row, vint column)
			{
				throw 0;
			}

			void GuiBindableDataGrid::ItemSource::BeforeEditCell(vint row, vint column, list::IDataEditor* dataEditor)
			{
				throw 0;
			}

			void GuiBindableDataGrid::ItemSource::SaveCellData(vint row, vint column, list::IDataEditor* dataEditor)
			{
				throw 0;
			}

/***********************************************************************
GuiBindableDataGrid
***********************************************************************/

			GuiBindableDataGrid::GuiBindableDataGrid(IStyleProvider* _styleProvider, Ptr<description::IValueEnumerable> itemSource)
				:GuiVirtualDataGrid(_styleProvider, new ItemSource(itemSource))
			{
			}

			GuiBindableDataGrid::~GuiBindableDataGrid()
			{
			}
		}
	}
}