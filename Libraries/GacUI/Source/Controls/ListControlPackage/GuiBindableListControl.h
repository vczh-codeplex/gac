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
				struct DataBindingConfig
				{
					Ptr<description::IValueEnumerable>		itemSource;
					WString									textProperty;
					WString									checkedProperty;
				};

				GuiBindableTextList(IStyleProvider* _styleProvider, list::TextItemStyleProvider::ITextItemStyleProvider* _itemStyleProvider, const DataBindingConfig& config);
				~GuiBindableTextList();
			};

/***********************************************************************
GuiBindableListView
***********************************************************************/

			class GuiBindableListView : public GuiVirtualListView
			{
			public:
				struct DataBindingConfig
				{
					Ptr<description::IValueEnumerable>		itemSource;
				};

				GuiBindableListView(IStyleProvider* _styleProvider, const DataBindingConfig& config);
				~GuiBindableListView();
			};

/***********************************************************************
GuiBindableTreeView
***********************************************************************/

			class GuiBindableTreeView : public GuiVirtualTreeView
			{
			public:
				struct DataBindingConfig
				{
					Ptr<description::IValueEnumerable>		itemSource;
				};

				GuiBindableTreeView(IStyleProvider* _styleProvider, const DataBindingConfig& config);
				~GuiBindableTreeView();
			};

/***********************************************************************
GuiBindableDataGrid
***********************************************************************/

			class GuiBindableDataGrid : public GuiVirtualDataGrid
			{
			public:
				struct DataBindingConfig
				{
					Ptr<description::IValueEnumerable>		itemSource;
				};

				GuiBindableDataGrid(IStyleProvider* _styleProvider, const DataBindingConfig& config);
				~GuiBindableDataGrid();
			};
		}
	}
}

#endif