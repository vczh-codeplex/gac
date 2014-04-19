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

			class GuiBindableTextList : public GuiVirtualTextList
			{
			public:
				GuiBindableTextList(IStyleProvider* _styleProvider, list::TextItemStyleProvider::ITextItemStyleProvider* _itemStyleProvider);
				~GuiBindableTextList();
			};

/***********************************************************************
GuiBindableListView
***********************************************************************/

			class GuiBindableListView : public GuiVirtualListView
			{
			public:
				GuiBindableListView(IStyleProvider* _styleProvider);
				~GuiBindableListView();
			};

/***********************************************************************
GuiBindableTreeView
***********************************************************************/

			class GuiBindableTreeView : public GuiVirtualTreeView
			{
			public:
				GuiBindableTreeView(IStyleProvider* _styleProvider);
				~GuiBindableTreeView();
			};

/***********************************************************************
GuiBindableDataGrid
***********************************************************************/

			class GuiBindableDataGrid : public GuiVirtualDataGrid
			{
			public:
				GuiBindableDataGrid(IStyleProvider* _styleProvider);
				~GuiBindableDataGrid();
			};
		}
	}
}

#endif