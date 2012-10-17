#include "GuiWin8ListStyles.h"
#include "..\Win7Styles\GuiWin7ListStyles.h"

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
Win8SelectableItemStyle
***********************************************************************/

			void Win8SelectableItemStyle::TransferInternal(GuiButton::ControlState value, bool enabled, bool selected)
			{
				if(!enabled)
				{
					transferringAnimation->Transfer(Win8ButtonColors::ItemDisabled());
				}
				else if(selected)
				{
					transferringAnimation->Transfer(Win8ButtonColors::ItemSelected());
				}
				else
				{
					switch(value)
					{
					case GuiButton::Normal:
						transferringAnimation->Transfer(Win8ButtonColors::ItemNormal());
						break;
					case GuiButton::Active:
						transferringAnimation->Transfer(Win8ButtonColors::ItemActive());
						break;
					case GuiButton::Pressed:
						transferringAnimation->Transfer(Win8ButtonColors::ItemSelected());
						break;
					}
				}
			}

			Win8SelectableItemStyle::Win8SelectableItemStyle()
				:Win8ButtonStyleBase(Win8ButtonColors::ItemNormal(), Alignment::Left, Alignment::Center)
			{
				transferringAnimation->SetEnableAnimation(false);
			}

			Win8SelectableItemStyle::~Win8SelectableItemStyle()
			{
			}

/***********************************************************************
Win8TextListProvider
***********************************************************************/
			
			Win8TextListProvider::Win8TextListProvider()
			{
			}

			Win8TextListProvider::~Win8TextListProvider()
			{
			}

			controls::GuiSelectableButton::IStyleController* Win8TextListProvider::CreateBackgroundStyleController()
			{
				return new Win8SelectableItemStyle;
			}

			controls::GuiSelectableButton::IStyleController* Win8TextListProvider::CreateBulletStyleController()
			{
				return 0;
			}

/***********************************************************************
Win8CheckTextListProvider
***********************************************************************/

			Win8CheckTextListProvider::Win8CheckTextListProvider()
			{
			}

			Win8CheckTextListProvider::~Win8CheckTextListProvider()
			{
			}

			controls::GuiSelectableButton::IStyleController* Win8CheckTextListProvider::CreateBulletStyleController()
			{
				return new Win8CheckBoxStyle(Win8CheckBoxStyle::CheckBox, false);
			}

/***********************************************************************
Win8RadioTextListProvider
***********************************************************************/

			Win8RadioTextListProvider::Win8RadioTextListProvider()
			{
			}

			Win8RadioTextListProvider::~Win8RadioTextListProvider()
			{
			}

			controls::GuiSelectableButton::IStyleController* Win8RadioTextListProvider::CreateBulletStyleController()
			{
				return new Win8CheckBoxStyle(Win8CheckBoxStyle::RadioButton, false);
			}

/***********************************************************************
Win8ListViewProvider
***********************************************************************/

			Win8ListViewProvider::Win8ListViewProvider()
			{
			}

			Win8ListViewProvider::~Win8ListViewProvider()
			{
			}

			controls::GuiSelectableButton::IStyleController* Win8ListViewProvider::CreateItemBackground()
			{
				return new Win8SelectableItemStyle;
			}

			controls::GuiListViewColumnHeader::IStyleController* Win8ListViewProvider::CreateColumnStyle()
			{
				return new win7::Win7ListViewColumnHeaderStyle;
			}

			Color Win8ListViewProvider::GetPrimaryTextColor()
			{
				return Win8GetSystemTextColor(true);
			}

			Color Win8ListViewProvider::GetSecondaryTextColor()
			{
				return Win8GetSystemTextColor(false);
			}

			Color Win8ListViewProvider::GetItemSeparatorColor()
			{
				return Color(220, 220, 220);
			}

/***********************************************************************
Win8TreeViewProvider
***********************************************************************/

			Win8TreeViewProvider::Win8TreeViewProvider()
			{
			}

			Win8TreeViewProvider::~Win8TreeViewProvider()
			{
			}

			controls::GuiSelectableButton::IStyleController* Win8TreeViewProvider::CreateItemBackground()
			{
				return new Win8SelectableItemStyle;
			}

			controls::GuiSelectableButton::IStyleController* Win8TreeViewProvider::CreateItemExpandingDecorator()
			{
				return new win7::Win7TreeViewExpandingButtonStyle;
			}

			Color Win8TreeViewProvider::GetTextColor()
			{
				return Win8GetSystemTextColor(true);
			}
		}
	}
}