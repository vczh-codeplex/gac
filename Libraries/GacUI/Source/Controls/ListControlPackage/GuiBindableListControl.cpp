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

			GuiBindableTextList::GuiBindableTextList(IStyleProvider* _styleProvider, list::TextItemStyleProvider::ITextItemStyleProvider* _itemStyleProvider)
				:GuiVirtualTextList(_styleProvider, _itemStyleProvider, 0)
			{
			}

			GuiBindableTextList::~GuiBindableTextList()
			{
			}

/***********************************************************************
GuiBindableListView
***********************************************************************/

			GuiBindableListView::GuiBindableListView(IStyleProvider* _styleProvider)
				:GuiVirtualListView(_styleProvider, 0)
			{
			}

			GuiBindableListView::~GuiBindableListView()
			{
			}

/***********************************************************************
GuiBindableTreeView
***********************************************************************/

			GuiBindableTreeView::GuiBindableTreeView(IStyleProvider* _styleProvider)
				:GuiVirtualTreeView(_styleProvider, 0)
			{
			}

			GuiBindableTreeView::~GuiBindableTreeView()
			{
			}

/***********************************************************************
GuiBindableDataGrid
***********************************************************************/

			GuiBindableDataGrid::GuiBindableDataGrid(IStyleProvider* _styleProvider)
				:GuiVirtualDataGrid(_styleProvider, (list::IDataProvider*)0)
			{
			}

			GuiBindableDataGrid::~GuiBindableDataGrid()
			{
			}
		}
	}
}