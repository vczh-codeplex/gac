/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control Styles::Windows7 Styles

Clases:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUIWIN7STYLES
#define VCZH_PRESENTATION_CONTROLS_GUIWIN7STYLES

#include "GuiThemeStyleFactory.h"

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

				controls::GuiWindow::IStyleController*								CreateWindowStyle()override;
				controls::GuiLabel::IStyleController*								CreateLabelStyle()override;
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