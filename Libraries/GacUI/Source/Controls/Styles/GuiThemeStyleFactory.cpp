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

				controls::GuiControl* NewLabel()
				{
					return new controls::GuiControl(GetCurrentTheme()->CreateLabelStyle());
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

				controls::GuiMenu* NewMenu(controls::GuiControl* owner)
				{
					return new controls::GuiMenu(GetCurrentTheme()->CreateMenuStyle(), owner);
				}

				controls::GuiMenuBar* NewMenuBar()
				{
					return new controls::GuiMenuBar(GetCurrentTheme()->CreateMenuBarStyle());
				}

				controls::GuiControl* NewMenuSplitter()
				{
					return new controls::GuiControl(GetCurrentTheme()->CreateMenuSplitterStyle());
				}

				controls::GuiMenuButton* NewMenuBarButton()
				{
					return new controls::GuiMenuButton(GetCurrentTheme()->CreateMenuBarButtonStyle());
				}

				controls::GuiMenuButton* NewMenuItemButton()
				{
					return new controls::GuiMenuButton(GetCurrentTheme()->CreateMenuItemButtonStyle());
				}

				controls::GuiButton* NewButton()
				{
					return new controls::GuiButton(GetCurrentTheme()->CreateButtonStyle());
				}

				controls::GuiSelectableButton* NewToolstripButton()
				{
					return new controls::GuiSelectableButton(GetCurrentTheme()->CreateToolstripButtonStyle());
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
					return new controls::GuiScroll(GetCurrentTheme()->CreateHTrackerStyle());
				}

				controls::GuiScroll* NewVTracker()
				{
					return new controls::GuiScroll(GetCurrentTheme()->CreateVTrackerStyle());
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