#include "GuiThemeStyleFactory.h"

namespace vl
{
	namespace presentation
	{
		namespace theme
		{
			ITheme* currentTheme=0;

			ITheme* GetCurrentTheme()
			{
				return currentTheme;
			}

			void SetCurrentTheme(ITheme* theme)
			{
				currentTheme=theme;
			}

			namespace g
			{
				controls::GuiWindow* NewWindow()
				{
					return new controls::GuiWindow(GetCurrentTheme()->CreateWindowStyle());
				}

				controls::GuiLabel* NewLabel()
				{
					return new controls::GuiLabel(GetCurrentTheme()->CreateLabelStyle());
				}

				controls::GuiScrollContainer* NewScrollContainer()
				{
					return new controls::GuiScrollContainer(GetCurrentTheme()->CreateScrollContainerStyle());
				}

				controls::GuiControl* NewGroupBox()
				{
					return new controls::GuiControl(GetCurrentTheme()->CreateGroupBoxStyle());
				}

				controls::GuiTab* NewTab()
				{
					return new controls::GuiTab(GetCurrentTheme()->CreateTabStyle());
				}

				controls::GuiComboBoxListControl* NewComboBox(controls::GuiSelectableListControl* containedListControl)
				{
					return new controls::GuiComboBoxListControl(GetCurrentTheme()->CreateComboBoxStyle(), containedListControl);
				}

				controls::GuiMultilineTextBox* NewMultilineTextBox()
				{
					return new controls::GuiMultilineTextBox(GetCurrentTheme()->CreateMultilineTextBoxStyle());
				}

				controls::GuiSinglelineTextBox* NewTextBox()
				{
					return new controls::GuiSinglelineTextBox(GetCurrentTheme()->CreateTextBoxStyle());
				}

				controls::GuiDocumentViewer* NewDocumentViewer()
				{
					return new controls::GuiDocumentViewer(GetCurrentTheme()->CreateDocumentViewerStyle());
				}

				controls::GuiListView* NewListViewBigIcon()
				{
					controls::GuiListView* listview=new controls::GuiListView(GetCurrentTheme()->CreateListViewStyle());
					listview->ChangeItemStyle(new controls::list::ListViewBigIconContentProvider);
					return listview;
				}

				controls::GuiListView* NewListViewSmallIcon()
				{
					controls::GuiListView* listview=new controls::GuiListView(GetCurrentTheme()->CreateListViewStyle());
					listview->ChangeItemStyle(new controls::list::ListViewSmallIconContentProvider);
					return listview;
				}

				controls::GuiListView* NewListViewList()
				{
					controls::GuiListView* listview=new controls::GuiListView(GetCurrentTheme()->CreateListViewStyle());
					listview->ChangeItemStyle(new controls::list::ListViewListContentProvider);
					return listview;
				}

				controls::GuiListView* NewListViewDetail()
				{
					controls::GuiListView* listview=new controls::GuiListView(GetCurrentTheme()->CreateListViewStyle());
					listview->ChangeItemStyle(new controls::list::ListViewDetailContentProvider);
					return listview;
				}

				controls::GuiListView* NewListViewTile()
				{
					controls::GuiListView* listview=new controls::GuiListView(GetCurrentTheme()->CreateListViewStyle());
					listview->ChangeItemStyle(new controls::list::ListViewTileContentProvider);
					return listview;
				}

				controls::GuiListView* NewListViewInformation()
				{
					controls::GuiListView* listview=new controls::GuiListView(GetCurrentTheme()->CreateListViewStyle());
					listview->ChangeItemStyle(new controls::list::ListViewInformationContentProvider);
					return listview;
				}

				controls::GuiTreeView* NewTreeView()
				{
					return new controls::GuiTreeView(GetCurrentTheme()->CreateTreeViewStyle());
				}

				controls::GuiToolstripMenu* NewMenu(controls::GuiControl* owner)
				{
					return new controls::GuiToolstripMenu(GetCurrentTheme()->CreateMenuStyle(), owner);
				}

				controls::GuiToolstripMenuBar* NewMenuBar()
				{
					return new controls::GuiToolstripMenuBar(GetCurrentTheme()->CreateMenuBarStyle());
				}

				controls::GuiControl* NewMenuSplitter()
				{
					return new controls::GuiControl(GetCurrentTheme()->CreateMenuSplitterStyle());
				}

				controls::GuiToolstripButton* NewMenuBarButton()
				{
					return new controls::GuiToolstripButton(GetCurrentTheme()->CreateMenuBarButtonStyle());
				}

				controls::GuiToolstripButton* NewMenuItemButton()
				{
					return new controls::GuiToolstripButton(GetCurrentTheme()->CreateMenuItemButtonStyle());
				}

				controls::GuiToolstripToolbar* NewToolbar()
				{
					return new controls::GuiToolstripToolbar(GetCurrentTheme()->CreateToolbarStyle());
				}

				controls::GuiToolstripButton* NewToolbarButton()
				{
					return new controls::GuiToolstripButton(GetCurrentTheme()->CreateToolbarButtonStyle());
				}

				controls::GuiToolstripButton* NewToolbarDropdownButton()
				{
					return new controls::GuiToolstripButton(GetCurrentTheme()->CreateToolbarDropdownButtonStyle());
				}

				controls::GuiToolstripButton* NewToolbarSplitButton()
				{
					return new controls::GuiToolstripButton(GetCurrentTheme()->CreateToolbarSplitButtonStyle());
				}

				controls::GuiControl* NewToolbarSplitter()
				{
					return new controls::GuiControl(GetCurrentTheme()->CreateToolbarSplitterStyle());
				}

				controls::GuiButton* NewButton()
				{
					return new controls::GuiButton(GetCurrentTheme()->CreateButtonStyle());
				}

				controls::GuiSelectableButton* NewCheckBox()
				{
					return new controls::GuiSelectableButton(GetCurrentTheme()->CreateCheckBoxStyle());
				}

				controls::GuiSelectableButton* NewRadioButton()
				{
					return new controls::GuiSelectableButton(GetCurrentTheme()->CreateRadioButtonStyle());
				}

				controls::GuiScroll* NewHScroll()
				{
					return new controls::GuiScroll(GetCurrentTheme()->CreateHScrollStyle());
				}

				controls::GuiScroll* NewVScroll()
				{
					return new controls::GuiScroll(GetCurrentTheme()->CreateVScrollStyle());
				}

				controls::GuiScroll* NewHTracker()
				{
					controls::GuiScroll* scroll=new controls::GuiScroll(GetCurrentTheme()->CreateHTrackerStyle());
					scroll->SetPageSize(0);
					return scroll;
				}

				controls::GuiScroll* NewVTracker()
				{
					controls::GuiScroll* scroll=new controls::GuiScroll(GetCurrentTheme()->CreateVTrackerStyle());
					scroll->SetPageSize(0);
					return scroll;
				}
				
				controls::GuiScroll* NewProgressBar()
				{
					controls::GuiScroll* scroll=new controls::GuiScroll(GetCurrentTheme()->CreateProgressBarStyle());
					scroll->SetPageSize(0);
					return scroll;
				}

				controls::GuiTextList* NewTextList()
				{
					return new controls::GuiTextList(GetCurrentTheme()->CreateTextListStyle(), GetCurrentTheme()->CreateTextListItemStyle());
				}

				controls::GuiTextList* NewCheckTextList()
				{
					return new controls::GuiTextList(GetCurrentTheme()->CreateTextListStyle(), GetCurrentTheme()->CreateCheckTextListItemStyle());
				}

				controls::GuiTextList* NewRadioTextList()
				{
					return new controls::GuiTextList(GetCurrentTheme()->CreateTextListStyle(), GetCurrentTheme()->CreateRadioTextListItemStyle());
				}
			}
		}
	}
}