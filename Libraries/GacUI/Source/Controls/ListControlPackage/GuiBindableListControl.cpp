#include "GuiBindableListControl.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
GuiBindableTextList
***********************************************************************/

			// GuiListControl::IItemPrimaryTextView
			// GuiListControl::IItemPrimaryTextView
			GuiListControl::IItemProvider* CreateTextListDataSource(const GuiBindableTextList::DataBindingConfig& config)
			{
				return 0;
			}

			GuiBindableTextList::GuiBindableTextList(IStyleProvider* _styleProvider, list::TextItemStyleProvider::ITextItemStyleProvider* _itemStyleProvider, const GuiBindableTextList::DataBindingConfig& config)
				:GuiVirtualTextList(_styleProvider, _itemStyleProvider, CreateTextListDataSource(config))
			{
			}

			GuiBindableTextList::~GuiBindableTextList()
			{
			}

/***********************************************************************
GuiBindableListView
***********************************************************************/

			//ListViewItemStyleProvider::IListViewItemView
			// ListViewColumnItemArranger::IColumnItemView
			// GuiListControl::IItemPrimaryTextView
			GuiListControl::IItemProvider* CreateListViewDataSource(const GuiBindableListView::DataBindingConfig& config)
			{
				return 0;
			}

			GuiBindableListView::GuiBindableListView(IStyleProvider* _styleProvider, const GuiBindableListView::DataBindingConfig& config)
				:GuiVirtualListView(_styleProvider, CreateListViewDataSource(config))
			{
			}

			GuiBindableListView::~GuiBindableListView()
			{
			}

/***********************************************************************
GuiBindableTreeView
***********************************************************************/

			// tree::ITreeViewItemView
			// tree:;INodeItemPrimaryTextView
			Ptr<tree::INodeRootProvider> CreateTreeViewDataSource(const GuiBindableTreeView::DataBindingConfig& config)
			{
				return 0;
			}

			GuiBindableTreeView::GuiBindableTreeView(IStyleProvider* _styleProvider, const GuiBindableTreeView::DataBindingConfig& config)
				:GuiVirtualTreeView(_styleProvider, CreateTreeViewDataSource(config))
			{
			}

			GuiBindableTreeView::~GuiBindableTreeView()
			{
			}

/***********************************************************************
GuiBindableDataGrid
***********************************************************************/

			list::IDataProvider* CreateDataGridDataSource(const GuiBindableDataGrid::DataBindingConfig& config)
			{
				return 0;
			}

			GuiBindableDataGrid::GuiBindableDataGrid(IStyleProvider* _styleProvider, const GuiBindableDataGrid::DataBindingConfig& config)
				:GuiVirtualDataGrid(_styleProvider, CreateDataGridDataSource(config))
			{
			}

			GuiBindableDataGrid::~GuiBindableDataGrid()
			{
			}
		}
	}
}