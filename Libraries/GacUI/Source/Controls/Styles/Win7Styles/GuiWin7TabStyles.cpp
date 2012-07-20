#include "GuiWin7TabStyles.h"

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
Win7TabPageHeaderStyle
***********************************************************************/

			void Win7TabPageHeaderStyle::TransferInternal(GuiButton::ControlState value, bool enabled, bool selected)
			{
				if(selected)
				{
					transferringAnimation->Transfer(Win7ButtonColors::TabPageHeaderSelected());
				}
				else
				{
					switch(value)
					{
					case GuiButton::Normal:
						transferringAnimation->Transfer(Win7ButtonColors::TabPageHeaderNormal());
						break;
					case GuiButton::Active:
					case GuiButton::Pressed:
						transferringAnimation->Transfer(Win7ButtonColors::TabPageHeaderActive());
						break;
					}
				}
			}

			Win7TabPageHeaderStyle::Win7TabPageHeaderStyle()
				:Win7ButtonStyleBase(true, false, Win7ButtonColors::TabPageHeaderNormal(), Alignment::Left, Alignment::Center)
			{
				transferringAnimation->SetEnableAnimation(false);
				{
					Margin margin=elements.backgroundComposition->GetAlignmentToParent();
					margin.bottom=0;
					elements.backgroundComposition->SetAlignmentToParent(margin);
				}
				{
					Margin margin=elements.gradientComposition->GetAlignmentToParent();
					margin.bottom=0;
					elements.gradientComposition->SetAlignmentToParent(margin);
				}
			}

			Win7TabPageHeaderStyle::~Win7TabPageHeaderStyle()
			{
			}

			void Win7TabPageHeaderStyle::SetFont(const FontProperties& value)
			{
				Win7ButtonStyleBase::SetFont(value);
				Margin margin=elements.textComposition->GetMargin();
				margin.left*=2;
				margin.right*=2;
				elements.textComposition->SetMargin(margin);
			}

/***********************************************************************
Win7TabStyle
***********************************************************************/

			void Win7TabStyle::OnHeaderButtonClicked(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				if(commandExecutor)
				{
					int index=headerButtons.IndexOf(dynamic_cast<GuiSelectableButton*>(sender->GetAssociatedControl()));
					if(index!=-1)
					{
						commandExecutor->ShowTab(index);
					}
				}
			}

			void Win7TabStyle::OnTabHeaderBoundsChanged(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				int height=headerOverflowButton->GetBoundsComposition()->GetBounds().Height();
				headerOverflowButton->GetBoundsComposition()->SetBounds(Rect(Point(0, 0), Size(height, 0)));
				UpdateHeaderOverflowButtonVisibility();
			}

			void Win7TabStyle::OnHeaderOverflowButtonClicked(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				for(int i=headerOverflowMenuStack->GetStackItems().Count()-1;i>=0;i--)
				{
					GuiStackItemComposition* item=headerOverflowMenuStack->GetStackItems()[i];
					GuiControl* button=item->Children()[0]->GetAssociatedControl();

					headerOverflowMenuStack->RemoveChild(item);
					item->RemoveChild(button->GetBoundsComposition());
					delete button;
					delete item;
				}

				for(int i=0;i<headerButtons.Count();i++)
				{
					GuiStackItemComposition* item=new GuiStackItemComposition;
					headerOverflowMenuStack->AddChild(item);

					GuiMenuButton* button=new GuiMenuButton(new Win7MenuItemButtonStyle);
					button->SetText(headerButtons[i]->GetText());
					button->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
					button->Clicked.AttachMethod(this, &Win7TabStyle::OnHeaderOverflowMenuButtonClicked);
					item->AddChild(button->GetBoundsComposition());
				}

				headerOverflowMenu->SetClientSize(Size(0, 0));
				headerOverflowMenu->ShowPopup(headerOverflowButton, true);
			}

			void Win7TabStyle::OnHeaderOverflowMenuButtonClicked(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				int index=headerOverflowMenuStack->GetStackItems().IndexOf(dynamic_cast<GuiStackItemComposition*>(sender->GetParent()));
				if(index!=-1)
				{
					commandExecutor->ShowTab(index);
				}
			}

			void Win7TabStyle::UpdateHeaderOverflowButtonVisibility()
			{
				headerOverflowButton->SetVisible(tabHeaderComposition->IsStackItemClipped());
			}

			void Win7TabStyle::UpdateHeaderZOrder()
			{
				int itemCount=tabHeaderComposition->GetStackItems().Count();
				int childCount=tabHeaderComposition->Children().Count();
				for(int i=0;i<itemCount;i++)
				{
					GuiStackItemComposition* item=tabHeaderComposition->GetStackItems()[i];
					if(headerButtons[i]->GetSelected())
					{
						tabHeaderComposition->MoveChild(item, childCount-1);
						item->SetExtraMargin(Margin(2, 2, 2, 0));
					}
					else
					{
						item->SetExtraMargin(Margin(0, 0, 0, 0));
					}
				}
				tabHeaderComposition->MoveChild(tabContentTopLineComposition, childCount-2);
			}

			Win7TabStyle::Win7TabStyle()
				:commandExecutor(0)
			{
				boundsComposition=new GuiTableComposition;
				boundsComposition->SetRowsAndColumns(2, 1);
				boundsComposition->SetRowOption(0, GuiCellOption::MinSizeOption());
				boundsComposition->SetRowOption(1, GuiCellOption::PercentageOption(1.0));
				boundsComposition->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
				{
					GuiCellComposition* cell=new GuiCellComposition;
					boundsComposition->AddChild(cell);
					cell->SetSite(0, 0, 1, 1);

					tabHeaderComposition=new GuiStackComposition;
					tabHeaderComposition->SetExtraMargin(Margin(2, 2, 2, 0));
					tabHeaderComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
					tabHeaderComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
					tabHeaderComposition->BoundsChanged.AttachMethod(this, &Win7TabStyle::OnTabHeaderBoundsChanged);
					cell->AddChild(tabHeaderComposition);

					headerOverflowButton=new GuiButton(new Win7ButtonStyle);
					headerOverflowButton->GetContainerComposition()->AddChild(common_styles::CommonFragmentBuilder::BuildDownArrow(headerOverflowArrowElement));
					headerOverflowButton->SetVisible(false);
					headerOverflowButton->GetBoundsComposition()->SetAlignmentToParent(Margin(-1, 0, 0, 0));
					headerOverflowButton->Clicked.AttachMethod(this, &Win7TabStyle::OnHeaderOverflowButtonClicked);
					cell->AddChild(headerOverflowButton->GetBoundsComposition());
				}
				{
					GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
					element->SetColor(Win7ButtonColors::TabPageHeaderNormal().borderColor);

					tabContentTopLineComposition=new GuiBoundsComposition;
					tabContentTopLineComposition->SetOwnedElement(element);
					tabContentTopLineComposition->SetAlignmentToParent(Margin(0, -1, 0, 0));
					tabContentTopLineComposition->SetPreferredMinSize(Size(0, 1));
					tabHeaderComposition->AddChild(tabContentTopLineComposition);
				}
				{
					GuiCellComposition* cell=new GuiCellComposition;
					boundsComposition->AddChild(cell);
					cell->SetSite(1, 0, 1, 1);

					containerComposition=new GuiBoundsComposition;
					containerComposition->SetAlignmentToParent(Margin(1, 0, 1, 1));
					containerComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
					cell->AddChild(containerComposition);

					{
						GuiSolidBorderElement* element=GuiSolidBorderElement::Create();
						element->SetColor(Win7ButtonColors::TabPageHeaderNormal().borderColor);
						cell->SetOwnedElement(element);
					}
					{
						GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
						element->SetColor(Win7GetSystemTabContentColor());
						containerComposition->SetOwnedElement(element);
					}
				}
				{
					headerOverflowMenu=new GuiMenu(new Win7MenuStyle, 0);
					headerOverflowMenuStack=new GuiStackComposition;
					headerOverflowMenuStack->SetDirection(GuiStackComposition::Vertical);
					headerOverflowMenuStack->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
					headerOverflowMenuStack->SetAlignmentToParent(Margin(0, 0, 0, 0));
					headerOverflowMenu->GetContainerComposition()->AddChild(headerOverflowMenuStack);
				}

				headerController=new GuiSelectableButton::MutexGroupController;
			}

			Win7TabStyle::~Win7TabStyle()
			{
				delete headerOverflowMenu;
			}

			compositions::GuiBoundsComposition* Win7TabStyle::GetBoundsComposition()
			{
				return boundsComposition;
			}

			compositions::GuiGraphicsComposition* Win7TabStyle::GetContainerComposition()
			{
				return containerComposition;
			}

			void Win7TabStyle::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
			}

			void Win7TabStyle::SetText(const WString& value)
			{
			}

			void Win7TabStyle::SetFont(const FontProperties& value)
			{
				headerFont=value;
			}

			void Win7TabStyle::SetVisuallyEnabled(bool value)
			{
			}

			void Win7TabStyle::SetCommandExecutor(controls::GuiTab::ICommandExecutor* value)
			{
				commandExecutor=value;
			}

			void Win7TabStyle::InsertTab(int index)
			{
				GuiSelectableButton* button=new GuiSelectableButton(new Win7TabPageHeaderStyle);
				button->SetAutoSelection(false);
				button->SetFont(headerFont);
				button->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				button->SetGroupController(headerController.Obj());
				button->Clicked.AttachMethod(this, &Win7TabStyle::OnHeaderButtonClicked);

				GuiStackItemComposition* item=new GuiStackItemComposition;
				item->AddChild(button->GetBoundsComposition());
				tabHeaderComposition->InsertStackItem(index, item);
				headerButtons.Insert(index, button);
				UpdateHeaderZOrder();
			}

			void Win7TabStyle::SetTabText(int index, const WString& value)
			{
				headerButtons[index]->SetText(value);
				UpdateHeaderOverflowButtonVisibility();
			}

			void Win7TabStyle::RemoveTab(int index)
			{
				GuiStackItemComposition* item=tabHeaderComposition->GetStackItems()[index];
				GuiSelectableButton* button=headerButtons[index];

				tabHeaderComposition->RemoveChild(item);
				item->RemoveChild(button->GetBoundsComposition());
				headerButtons.RemoveAt(index);

				delete item;
				delete button;
				UpdateHeaderOverflowButtonVisibility();
			}

			void Win7TabStyle::MoveTab(int oldIndex, int newIndex)
			{
				GuiStackItemComposition* item=tabHeaderComposition->GetStackItems()[oldIndex];
				tabHeaderComposition->RemoveChild(item);
				tabHeaderComposition->InsertStackItem(newIndex, item);

				GuiSelectableButton* button=headerButtons[oldIndex];
				headerButtons.RemoveAt(oldIndex);
				headerButtons.Insert(newIndex, button);
				UpdateHeaderZOrder();
				UpdateHeaderOverflowButtonVisibility();
			}

			void Win7TabStyle::SetSelectedTab(int index)
			{
				headerButtons[index]->SetSelected(true);
				UpdateHeaderZOrder();
				UpdateHeaderOverflowButtonVisibility();
			}

			controls::GuiControl::IStyleController* Win7TabStyle::CreateTabPageStyleController()
			{
				GuiControl::IStyleController* style=new Win7EmptyStyle(Win7GetSystemTabContentColor());
				style->GetBoundsComposition()->SetAlignmentToParent(Margin(2, 2, 2, 2));
				return style;
			}
		}
	}
}