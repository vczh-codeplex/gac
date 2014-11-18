#include "GuiWin8ListStyles.h"
#include "../Win7Styles/GuiWin7ListStyles.h"

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
Win8DropDownComboBoxStyle
***********************************************************************/

			void Win8DropDownComboBoxStyle::TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool selected)
			{
				Win8ButtonColors targetColor;
				if(enabled)
				{
					if(selected) value=GuiButton::Pressed;
					switch(value)
					{
					case GuiButton::Normal:
						targetColor=Win8ButtonColors::ButtonNormal();
						break;
					case GuiButton::Active:
						targetColor=Win8ButtonColors::ButtonActive();
						break;
					case GuiButton::Pressed:
						targetColor=Win8ButtonColors::ButtonPressed();
						break;
					}
				}
				else
				{
					targetColor=Win8ButtonColors::ButtonDisabled();
				}
				transferringAnimation->Transfer(targetColor);
			}

			void Win8DropDownComboBoxStyle::AfterApplyColors(const Win8ButtonColors& colors)
			{
				Win8ButtonStyle::AfterApplyColors(colors);
				dropDownElement->SetBorderColor(colors.textColor);
				dropDownElement->SetBackgroundColor(colors.textColor);
			}

			Win8DropDownComboBoxStyle::Win8DropDownComboBoxStyle()
				:commandExecutor(0)
			{
				table=new GuiTableComposition;
				table->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
				table->SetAlignmentToParent(Margin(0, 0, 0, 0));
				table->SetRowsAndColumns(3, 2);
				table->SetRowOption(0, GuiCellOption::PercentageOption(1.0));
				table->SetRowOption(1, GuiCellOption::MinSizeOption());
				table->SetRowOption(2, GuiCellOption::PercentageOption(1.0));
				table->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
				table->SetColumnOption(1, GuiCellOption::MinSizeOption());
				elements.textComposition->AddChild(table);
				elements.textComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);

				textComposition=new GuiCellComposition;
				table->AddChild(textComposition);
				textComposition->SetSite(1, 0, 1, 1);

				Ptr<IGuiGraphicsElement> element=elements.textComposition->GetOwnedElement();
				elements.textComposition->SetOwnedElement(0);
				textComposition->SetOwnedElement(element);
				elements.textElement->SetEllipse(true);
				elements.textElement->SetAlignments(Alignment::Left, Alignment::Center);

				dropDownElement=common_styles::CommonFragmentBuilder::BuildDownArrow();

				dropDownComposition=new GuiCellComposition;
				table->AddChild(dropDownComposition);
				dropDownComposition->SetSite(1, 1, 1, 1);
				dropDownComposition->SetOwnedElement(dropDownElement);
				dropDownComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
				dropDownComposition->SetMargin(Margin(3, 0, 3, 0));
			}

			Win8DropDownComboBoxStyle::~Win8DropDownComboBoxStyle()
			{
			}

			compositions::GuiGraphicsComposition* Win8DropDownComboBoxStyle::GetContainerComposition()
			{
				return textComposition;
			}

			GuiMenu::IStyleController* Win8DropDownComboBoxStyle::CreateSubMenuStyleController()
			{
				return new Win8MenuStyle;
			}

			void Win8DropDownComboBoxStyle::SetSubMenuExisting(bool value)
			{
			}

			void Win8DropDownComboBoxStyle::SetSubMenuOpening(bool value)
			{
				SetSelected(value);
			}

			GuiButton* Win8DropDownComboBoxStyle::GetSubMenuHost()
			{
				return 0;
			}

			void Win8DropDownComboBoxStyle::SetImage(Ptr<GuiImageData> value)
			{
			}

			void Win8DropDownComboBoxStyle::SetShortcutText(const WString& value)
			{
			}

			void Win8DropDownComboBoxStyle::SetCommandExecutor(controls::GuiComboBoxBase::ICommandExecutor* value)
			{
				commandExecutor=value;
			}

			void Win8DropDownComboBoxStyle::OnItemSelected()
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

			Color Win8TextListProvider::GetTextColor()
			{
				return Win8GetSystemTextColor(true);
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