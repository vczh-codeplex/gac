/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
GacUI::Control Styles::Windows8 Styles

Clases:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUIWIN8STYLES
#define VCZH_PRESENTATION_CONTROLS_GUIWIN8STYLES

#include "GuiThemeStyleFactory.h"
#include "GuiWin7Styles.h"

namespace vl
{
	namespace presentation
	{
		namespace win8
		{

/***********************************************************************
Theme
***********************************************************************/

			/// <summary>Windows 8 theme.</summary>
			class Win8Theme : public Object, public theme::ITheme
			{
			public:
				/// <summary>Create the theme.</summary>
				Win8Theme();
				~Win8Theme();

				controls::GuiWindow::IStyleController*								CreateWindowStyle()override;
				controls::GuiCustomControl::IStyleController*						CreateCustomControlStyle()override;
				controls::GuiTooltip::IStyleController*								CreateTooltipStyle()override;
				controls::GuiLabel::IStyleController*								CreateLabelStyle()override;
				controls::GuiLabel::IStyleController*								CreateShortcutKeyStyle()override;
				controls::GuiScrollContainer::IStyleProvider*						CreateScrollContainerStyle()override;
				controls::GuiControl::IStyleController*								CreateGroupBoxStyle()override;
				controls::GuiTab::IStyleController*									CreateTabStyle()override;
				controls::GuiComboBoxBase::IStyleController*						CreateComboBoxStyle()override;
				controls::GuiScrollView::IStyleProvider*							CreateMultilineTextBoxStyle()override;
				controls::GuiSinglelineTextBox::IStyleProvider*						CreateTextBoxStyle()override;
				elements::text::ColorEntry											GetDefaultTextBoxColorEntry()override;
				controls::GuiDocumentViewer::IStyleProvider*						CreateDocumentViewerStyle()override;
				controls::GuiDocumentLabel::IStyleController*						CreateDocumentLabelStyle()override;
				controls::GuiListView::IStyleProvider*								CreateListViewStyle()override;
				controls::GuiTreeView::IStyleProvider*								CreateTreeViewStyle()override;
				controls::GuiSelectableButton::IStyleController*					CreateListItemBackgroundStyle()override;

				controls::GuiToolstripMenu::IStyleController*						CreateMenuStyle()override;
				controls::GuiToolstripMenuBar::IStyleController*					CreateMenuBarStyle()override;
				controls::GuiControl::IStyleController*								CreateMenuSplitterStyle()override;
				controls::GuiToolstripButton::IStyleController*						CreateMenuBarButtonStyle()override;
				controls::GuiToolstripButton::IStyleController*						CreateMenuItemButtonStyle()override;
				controls::GuiToolstripToolBar::IStyleController*					CreateToolBarStyle()override;
				controls::GuiToolstripButton::IStyleController*						CreateToolBarButtonStyle()override;
				controls::GuiToolstripButton::IStyleController*						CreateToolBarDropdownButtonStyle()override;
				controls::GuiToolstripButton::IStyleController*						CreateToolBarSplitButtonStyle()override;
				controls::GuiControl::IStyleController*								CreateToolBarSplitterStyle()override;

				controls::GuiButton::IStyleController*								CreateButtonStyle()override;
				controls::GuiSelectableButton::IStyleController*					CreateCheckBoxStyle()override;
				controls::GuiSelectableButton::IStyleController*					CreateRadioButtonStyle()override;
				controls::GuiDatePicker::IStyleProvider*							CreateDatePickerStyle()override;

				controls::GuiScroll::IStyleController*								CreateHScrollStyle()override;
				controls::GuiScroll::IStyleController*								CreateVScrollStyle()override;
				controls::GuiScroll::IStyleController*								CreateHTrackerStyle()override;
				controls::GuiScroll::IStyleController*								CreateVTrackerStyle()override;
				controls::GuiScroll::IStyleController*								CreateProgressBarStyle()override;
				vint																GetScrollDefaultSize()override;
				vint																GetTrackerDefaultSize()override;

				controls::GuiScrollView::IStyleProvider*							CreateTextListStyle()override;
				controls::list::TextItemStyleProvider::ITextItemStyleProvider*		CreateTextListItemStyle()override;
				controls::list::TextItemStyleProvider::ITextItemStyleProvider*		CreateCheckTextListItemStyle()override;
				controls::list::TextItemStyleProvider::ITextItemStyleProvider*		CreateRadioTextListItemStyle()override;
			};
		}
	}
}

#endif