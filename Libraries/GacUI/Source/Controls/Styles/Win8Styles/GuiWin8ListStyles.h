/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GacUI::Control Styles::Windows8 Styles

Clases:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_WIN8STYLES_GUIWin8LISTSTYLES
#define VCZH_PRESENTATION_CONTROLS_WIN8STYLES_GUIWin8LISTSTYLES

#include "GuiWin8StylesCommon.h"
#include "GuiWin8ButtonStyles.h"
#include "GuiWin8ScrollableStyles.h"
#include "GuiWin8ControlStyles.h"
#include "GuiWin8MenuStyles.h"

namespace vl
{
	namespace presentation
	{
		namespace win8
		{

/***********************************************************************
List Control Buttons
***********************************************************************/
			
			/// <summary>Selectable item style (Windows 8). Generally for list box item background.</summary>
			class Win8SelectableItemStyle : public Win8ButtonStyleBase, public Description<Win8SelectableItemStyle>
			{
			protected:
				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool selected)override;
			public:
				/// <summary>Create the style.</summary>
				Win8SelectableItemStyle();
				~Win8SelectableItemStyle();
			};

/***********************************************************************
List
***********************************************************************/
			
			/// <summary>Text list style (Windows 8).</summary>
			class Win8TextListProvider : public Object, public virtual controls::list::TextItemStyleProvider::ITextItemStyleProvider, public Description<Win8TextListProvider>
			{
			public:
				/// <summary>Create the style.</summary>
				Win8TextListProvider();
				~Win8TextListProvider();

				controls::GuiSelectableButton::IStyleController*		CreateBackgroundStyleController()override;
				controls::GuiSelectableButton::IStyleController*		CreateBulletStyleController()override;
			};
			
			/// <summary>Check box text list style (Windows 8).</summary>
			class Win8CheckTextListProvider : public Win8TextListProvider, public Description<Win8CheckTextListProvider>
			{
			public:
				/// <summary>Create the style.</summary>
				Win8CheckTextListProvider();
				~Win8CheckTextListProvider();

				controls::GuiSelectableButton::IStyleController*		CreateBulletStyleController()override;
			};
			
			/// <summary>Radio button text list style (Windows 8).</summary>
			class Win8RadioTextListProvider : public Win8TextListProvider, public Description<Win8RadioTextListProvider>
			{
			public:
				/// <summary>Create the style.</summary>
				Win8RadioTextListProvider();
				~Win8RadioTextListProvider();

				controls::GuiSelectableButton::IStyleController*		CreateBulletStyleController()override;
			};

#pragma warning(push)
#pragma warning(disable:4250)
			/// <summary>List view style (Windows 7).</summary>
			class Win8ListViewProvider : public Win8MultilineTextBoxProvider, public virtual controls::GuiListView::IStyleProvider, public Description<Win8ListViewProvider>
			{
			public:
				/// <summary>Create the style.</summary>
				Win8ListViewProvider();
				~Win8ListViewProvider();

				controls::GuiSelectableButton::IStyleController*		CreateItemBackground()override;
				controls::GuiListViewColumnHeader::IStyleController*	CreateColumnStyle()override;
				Color													GetPrimaryTextColor()override;
				Color													GetSecondaryTextColor()override;
				Color													GetItemSeparatorColor()override;
			};
			
			/// <summary>Tree view style (Windows 7).</summary>
			class Win8TreeViewProvider : public Win8MultilineTextBoxProvider, public virtual controls::GuiTreeView::IStyleProvider, public Description<Win8TreeViewProvider>
			{
			public:
				/// <summary>Create the style.</summary>
				Win8TreeViewProvider();
				~Win8TreeViewProvider();

				controls::GuiSelectableButton::IStyleController*		CreateItemBackground()override;
				controls::GuiSelectableButton::IStyleController*		CreateItemExpandingDecorator()override;
				Color													GetTextColor()override;
			};
#pragma warning(pop)
		}
	}
}

#endif