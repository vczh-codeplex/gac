#include <math.h>
#include "GuiWin7Styles.h"

namespace vl
{
	namespace presentation
	{
		namespace win7
		{
			using namespace collections;
			using namespace elements;
			using namespace compositions;
			using namespace controls;

/***********************************************************************
Win7Theme
***********************************************************************/

			Win7Theme::Win7Theme()
			{
			}

			Win7Theme::~Win7Theme()
			{
			}

			controls::GuiControl::IStyleController* Win7Theme::CreateWindowStyle()
			{
				return new Win7WindowStyle;
			}

			controls::GuiLabel::IStyleController* Win7Theme::CreateLabelStyle()
			{
				return new Win7LabelStyle;
			}

			controls::GuiControl::IStyleController* Win7Theme::CreateGroupBoxStyle()
			{
				return new Win7GroupBoxStyle;
			}

			controls::GuiTab::IStyleController* Win7Theme::CreateTabStyle()
			{
				return new Win7TabStyle;
			}

			controls::GuiComboBoxBase::IStyleController* Win7Theme::CreateComboBoxStyle()
			{
				return new Win7DropDownComboBoxStyle;
			}

			controls::GuiScrollView::IStyleProvider* Win7Theme::CreateMultilineTextBoxStyle()
			{
				return new Win7MultilineTextBoxProvider;
			}

			controls::GuiSinglelineTextBox::IStyleProvider* Win7Theme::CreateTextBoxStyle()
			{
				return new Win7SinglelineTextBoxProvider;
			}

			controls::GuiListView::IStyleProvider* Win7Theme::CreateListViewStyle()
			{
				return new Win7ListViewProvider;
			}

			controls::GuiTreeView::IStyleProvider* Win7Theme::CreateTreeViewStyle()
			{
				return new Win7TreeViewProvider;
			}

			controls::GuiControl::IStyleController* Win7Theme::CreateMenuStyle()
			{
				return new Win7MenuStyle;
			}

			controls::GuiControl::IStyleController* Win7Theme::CreateMenuBarStyle()
			{
				return new Win7MenuBarStyle;
			}

			controls::GuiControl::IStyleController* Win7Theme::CreateMenuSplitterStyle()
			{
				return new Win7MenuSplitterStyle;
			}

			controls::GuiMenuButton::IStyleController* Win7Theme::CreateMenuBarButtonStyle()
			{
				return new Win7MenuBarButtonStyle;
			}

			controls::GuiMenuButton::IStyleController* Win7Theme::CreateMenuItemButtonStyle()
			{
				return new Win7MenuItemButtonStyle;
			}

			controls::GuiButton::IStyleController* Win7Theme::CreateButtonStyle()
			{
				return new Win7ButtonStyle;
			}

			controls::GuiSelectableButton::IStyleController* Win7Theme::CreateCheckBoxStyle()
			{
				return new Win7CheckBoxStyle(Win7CheckBoxStyle::CheckBox);
			}

			controls::GuiSelectableButton::IStyleController* Win7Theme::CreateRadioButtonStyle()
			{
				return new Win7CheckBoxStyle(Win7CheckBoxStyle::RadioButton);
			}

			controls::GuiSelectableButton::IStyleController* Win7Theme::CreateToolstripButtonStyle()
			{
				return new Win7ToolstripButtonStyle(false);
			}

			controls::GuiMenuButton::IStyleController* Win7Theme::CreateToolstripDropdownButtonStyle()
			{
				return new Win7ToolstripDropdownButtonStyle(false);
			}

			controls::GuiMenuButton::IStyleController* Win7Theme::CreateToolstripSplitButtonStyle()
			{
				return new Win7ToolstripSplitButtonStyle(false);
			}

			controls::GuiControl::IStyleController* Win7Theme::CreateToolstripSplitterStyle()
			{
				return new Win7ToolstripSplitterStyle;
			}

			controls::GuiScroll::IStyleController* Win7Theme::CreateHScrollStyle()
			{
				return new Win7ScrollStyle(Win7ScrollStyle::Horizontal);
			}

			controls::GuiScroll::IStyleController* Win7Theme::CreateVScrollStyle()
			{
				return new Win7ScrollStyle(Win7ScrollStyle::Vertical);
			}

			controls::GuiScroll::IStyleController* Win7Theme::CreateHTrackerStyle()
			{
				return new Win7TrackStyle(Win7TrackStyle::Horizontal);
			}

			controls::GuiScroll::IStyleController* Win7Theme::CreateVTrackerStyle()
			{
				return new Win7TrackStyle(Win7TrackStyle::Vertical);
			}

			controls::GuiScrollView::IStyleProvider* Win7Theme::CreateTextListStyle()
			{
				return new Win7MultilineTextBoxProvider;
			}

			controls::list::TextItemStyleProvider::ITextItemStyleProvider* Win7Theme::CreateTextListItemStyle()
			{
				return new Win7TextListProvider;
			}

			controls::list::TextItemStyleProvider::ITextItemStyleProvider* Win7Theme::CreateCheckTextListItemStyle()
			{
				return new Win7CheckTextListProvider;
			}

			controls::list::TextItemStyleProvider::ITextItemStyleProvider* Win7Theme::CreateRadioTextListItemStyle()
			{
				return new Win7RadioTextListProvider;
			}
		}
	}
}