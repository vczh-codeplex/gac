/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control Styles::Windows7 Styles

Clases:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUIWIN7STYLES
#define VCZH_PRESENTATION_CONTROLS_GUIWIN7STYLES

#include "Win7Styles\GuiWin7StylesCommon.h"
#include "Win7Styles\GuiWin7ControlStyles.h"
#include "Win7Styles\GuiWin7ButtonStyles.h"
#include "Win7Styles\GuiWin7TabStyles.h"
#include "Win7Styles\GuiWin7MenuStyles.h"
#include "Win7Styles\GuiWin7ToolstripStyles.h"
#include "Win7Styles\GuiWin7ScrollableStyles.h"
#include "Win7Styles\GuiWin7ListStyles.h"

namespace vl
{
	namespace presentation
	{
		namespace win7
		{

/***********************************************************************
Theme
***********************************************************************/

			/// <summary>Windows 7 theme.</summary>
			class Win7Theme : public theme::ITheme
			{
			public:
				/// <summary>Create the theme.</summary>
				Win7Theme();
				~Win7Theme();

				controls::GuiControl::IStyleController*								CreateWindowStyle()override;
				controls::GuiLabel::IStyleController*								CreateLabelStyle()override;
				controls::GuiControl::IStyleController*								CreateGroupBoxStyle()override;
				controls::GuiTab::IStyleController*									CreateTabStyle()override;
				controls::GuiComboBoxBase::IStyleController*						CreateComboBoxStyle()override;
				controls::GuiScrollView::IStyleProvider*							CreateMultilineTextBoxStyle()override;
				controls::GuiSinglelineTextBox::IStyleProvider*						CreateTextBoxStyle()override;
				controls::GuiListView::IStyleProvider*								CreateListViewStyle()override;
				controls::GuiTreeView::IStyleProvider*								CreateTreeViewStyle()override;
				
				controls::GuiToolstripMenu::IStyleController*						CreateMenuStyle()override;
				controls::GuiToolstripMenuBar::IStyleController*					CreateMenuBarStyle()override;
				controls::GuiControl::IStyleController*								CreateMenuSplitterStyle()override;
				controls::GuiToolstripButton::IStyleController*						CreateMenuBarButtonStyle()override;
				controls::GuiToolstripButton::IStyleController*						CreateMenuItemButtonStyle()override;
				controls::GuiToolstripToolbar::IStyleController*					CreateToolbarStyle()override;
				controls::GuiToolstripButton::IStyleController*						CreateToolbarButtonStyle()override;
				controls::GuiToolstripButton::IStyleController*						CreateToolbarDropdownButtonStyle()override;
				controls::GuiToolstripButton::IStyleController*						CreateToolbarSplitButtonStyle()override;
				controls::GuiControl::IStyleController*								CreateToolbarSplitterStyle()override;

				controls::GuiButton::IStyleController*								CreateButtonStyle()override;
				controls::GuiSelectableButton::IStyleController*					CreateCheckBoxStyle()override;
				controls::GuiSelectableButton::IStyleController*					CreateRadioButtonStyle()override;
				
				controls::GuiScroll::IStyleController*								CreateHScrollStyle()override;
				controls::GuiScroll::IStyleController*								CreateVScrollStyle()override;
				controls::GuiScroll::IStyleController*								CreateHTrackerStyle()override;
				controls::GuiScroll::IStyleController*								CreateVTrackerStyle()override;

				controls::GuiScrollView::IStyleProvider*							CreateTextListStyle()override;
				controls::list::TextItemStyleProvider::ITextItemStyleProvider*		CreateTextListItemStyle()override;
				controls::list::TextItemStyleProvider::ITextItemStyleProvider*		CreateCheckTextListItemStyle()override;
				controls::list::TextItemStyleProvider::ITextItemStyleProvider*		CreateRadioTextListItemStyle()override;
			};
		}
	}
}

#endif