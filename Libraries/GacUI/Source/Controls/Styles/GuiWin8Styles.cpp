#include <math.h>
#include "GuiWin8Styles.h"
#include "Win8Styles\GuiWin8StylesCommon.h"
#include "Win8Styles\GuiWin8ControlStyles.h"
#include "Win8Styles\GuiWin8ButtonStyles.h"
//#include "Win8Styles\GuiWin8TabStyles.h"
//#include "Win8Styles\GuiWin8MenuStyles.h"
//#include "Win8Styles\GuiWin8ToolstripStyles.h"
//#include "Win8Styles\GuiWin8ScrollableStyles.h"
//#include "Win8Styles\GuiWin8ListStyles.h"

namespace vl
{
	namespace presentation
	{
		namespace win8
		{
			using namespace collections;
			using namespace elements;
			using namespace compositions;
			using namespace controls;

/***********************************************************************
Win8Theme
***********************************************************************/

			Win8Theme::Win8Theme()
			{
			}

			Win8Theme::~Win8Theme()
			{
			}

			controls::GuiWindow::IStyleController* Win8Theme::CreateWindowStyle()
			{
				return new Win8WindowStyle;
			}

			//controls::GuiLabel::IStyleController* Win8Theme::CreateLabelStyle()
			//{
			//	throw 0;
			//}

			//controls::GuiScrollContainer::IStyleProvider* Win8Theme::CreateScrollContainerStyle()
			//{
			//	throw 0;
			//}

			//controls::GuiControl::IStyleController* Win8Theme::CreateGroupBoxStyle()
			//{
			//	throw 0;
			//}

			//controls::GuiTab::IStyleController* Win8Theme::CreateTabStyle()
			//{
			//	throw 0;
			//}

			//controls::GuiComboBoxBase::IStyleController* Win8Theme::CreateComboBoxStyle()
			//{
			//	throw 0;
			//}

			//controls::GuiScrollView::IStyleProvider* Win8Theme::CreateMultilineTextBoxStyle()
			//{
			//	throw 0;
			//}

			//controls::GuiSinglelineTextBox::IStyleProvider* Win8Theme::CreateTextBoxStyle()
			//{
			//	throw 0;
			//}

			//controls::GuiListView::IStyleProvider* Win8Theme::CreateListViewStyle()
			//{
			//	throw 0;
			//}

			//controls::GuiTreeView::IStyleProvider* Win8Theme::CreateTreeViewStyle()
			//{
			//	throw 0;
			//}

			//elements::text::ColorEntry Win8Theme::GetDefaultTextBoxColorEntry()
			//{
			//	throw 0;
			//}

			//controls::GuiToolstripMenu::IStyleController* Win8Theme::CreateMenuStyle()
			//{
			//	throw 0;
			//}

			//controls::GuiToolstripMenuBar::IStyleController* Win8Theme::CreateMenuBarStyle()
			//{
			//	throw 0;
			//}

			//controls::GuiControl::IStyleController* Win8Theme::CreateMenuSplitterStyle()
			//{
			//	throw 0;
			//}

			//controls::GuiToolstripButton::IStyleController* Win8Theme::CreateMenuBarButtonStyle()
			//{
			//	throw 0;
			//}

			//controls::GuiToolstripButton::IStyleController* Win8Theme::CreateMenuItemButtonStyle()
			//{
			//	throw 0;
			//}

			//controls::GuiControl::IStyleController* Win8Theme::CreateToolbarStyle()
			//{
			//	throw 0;
			//}

			//controls::GuiToolstripButton::IStyleController* Win8Theme::CreateToolbarButtonStyle()
			//{
			//	throw 0;
			//}

			//controls::GuiToolstripButton::IStyleController* Win8Theme::CreateToolbarDropdownButtonStyle()
			//{
			//	throw 0;
			//}

			//controls::GuiToolstripButton::IStyleController* Win8Theme::CreateToolbarSplitButtonStyle()
			//{
			//	throw 0;
			//}

			//controls::GuiControl::IStyleController* Win8Theme::CreateToolbarSplitterStyle()
			//{
			//	throw 0;
			//}

			controls::GuiButton::IStyleController* Win8Theme::CreateButtonStyle()
			{
				return new Win8ButtonStyle;
			}

			controls::GuiSelectableButton::IStyleController* Win8Theme::CreateCheckBoxStyle()
			{
				return new Win8CheckBoxStyle(Win8CheckBoxStyle::CheckBox);
			}

			controls::GuiSelectableButton::IStyleController* Win8Theme::CreateRadioButtonStyle()
			{
				return new Win8CheckBoxStyle(Win8CheckBoxStyle::RadioButton);
			}

			//controls::GuiScroll::IStyleController* Win8Theme::CreateHScrollStyle()
			//{
			//	throw 0;
			//}

			//controls::GuiScroll::IStyleController* Win8Theme::CreateVScrollStyle()
			//{
			//	throw 0;
			//}

			//controls::GuiScroll::IStyleController* Win8Theme::CreateHTrackerStyle()
			//{
			//	throw 0;
			//}

			//controls::GuiScroll::IStyleController* Win8Theme::CreateVTrackerStyle()
			//{
			//	throw 0;
			//}

			//controls::GuiScroll::IStyleController* Win8Theme::CreateProgressBarStyle()
			//{
			//	throw 0;
			//}

			//int Win8Theme::GetScrollDefaultSize()
			//{
			//	throw 0;
			//}

			//int Win8Theme::GetTrackerDefaultSize()
			//{
			//	throw 0;
			//}

			//controls::GuiScrollView::IStyleProvider* Win8Theme::CreateTextListStyle()
			//{
			//	throw 0;
			//}

			//controls::list::TextItemStyleProvider::ITextItemStyleProvider* Win8Theme::CreateTextListItemStyle()
			//{
			//	throw 0;
			//}

			//controls::list::TextItemStyleProvider::ITextItemStyleProvider* Win8Theme::CreateCheckTextListItemStyle()
			//{
			//	throw 0;
			//}

			//controls::list::TextItemStyleProvider::ITextItemStyleProvider* Win8Theme::CreateRadioTextListItemStyle()
			//{
			//	throw 0;
			//}
		}
	}
}