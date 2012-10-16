#include "GuiWin8ScrollableStyles.h"

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
Win8ScrollHandleButtonStyle
***********************************************************************/

			void Win8ScrollHandleButtonStyle::TransferInternal(GuiButton::ControlState value, bool enabled, bool selected)
			{
				Win8ButtonColors targetColor;
				if(enabled)
				{
					switch(value)
					{
					case GuiButton::Normal:
						targetColor=Win8ButtonColors::ScrollHandleNormal();
						break;
					case GuiButton::Active:
						targetColor=Win8ButtonColors::ScrollHandleActive();
						break;
					case GuiButton::Pressed:
						targetColor=Win8ButtonColors::ScrollHandlePressed();
						break;
					}
				}
				else
				{
					targetColor=Win8ButtonColors::ScrollHandleDisabled();
				}
				transferringAnimation->Transfer(targetColor);
			}

			Win8ScrollHandleButtonStyle::Win8ScrollHandleButtonStyle()
				:Win8ButtonStyleBase(Win8ButtonColors::ScrollHandleNormal(), Alignment::Center, Alignment::Center)
			{
			}

			Win8ScrollHandleButtonStyle::~Win8ScrollHandleButtonStyle()
			{
			}

/***********************************************************************
Win8ScrollArrowButtonStyle
***********************************************************************/

			void Win8ScrollArrowButtonStyle::TransferInternal(GuiButton::ControlState value, bool enabled, bool selected)
			{
				Win8ButtonColors targetColor;
				if(enabled)
				{
					switch(value)
					{
					case GuiButton::Normal:
						targetColor=Win8ButtonColors::ScrollArrowNormal();
						break;
					case GuiButton::Active:
						targetColor=Win8ButtonColors::ScrollArrowActive();
						break;
					case GuiButton::Pressed:
						targetColor=Win8ButtonColors::ScrollArrowPressed();
						break;
					}
				}
				else
				{
					targetColor=Win8ButtonColors::ScrollArrowDisabled();
				}
				transferringAnimation->Transfer(targetColor);
			}

			void Win8ScrollArrowButtonStyle::AfterApplyColors(const Win8ButtonColors& colors)
			{
				Win8ButtonStyleBase::AfterApplyColors(colors);
				arrowElement->SetBorderColor(colors.textColor);
				arrowElement->SetBackgroundColor(colors.textColor);
			}

			Win8ScrollArrowButtonStyle::Win8ScrollArrowButtonStyle(common_styles::CommonScrollStyle::Direction direction, bool increaseButton)
				:Win8ButtonStyleBase(Win8ButtonColors::ScrollArrowNormal(), Alignment::Center, Alignment::Center)
			{
				switch(direction)
				{
				case common_styles::CommonScrollStyle::Horizontal:
					if(increaseButton)
					{
						GetContainerComposition()->AddChild(CommonFragmentBuilder::BuildRightArrow(arrowElement));
					}
					else
					{
						GetContainerComposition()->AddChild(CommonFragmentBuilder::BuildLeftArrow(arrowElement));
					}
					break;
				case common_styles::CommonScrollStyle::Vertical:
					if(increaseButton)
					{
						GetContainerComposition()->AddChild(CommonFragmentBuilder::BuildDownArrow(arrowElement));
					}
					else
					{
						GetContainerComposition()->AddChild(CommonFragmentBuilder::BuildUpArrow(arrowElement));
					}
					break;
				}
			}

			Win8ScrollArrowButtonStyle::~Win8ScrollArrowButtonStyle()
			{
			}

/***********************************************************************
Win8ScrollStyle
***********************************************************************/

			controls::GuiButton::IStyleController* Win8ScrollStyle::CreateDecreaseButtonStyle(Direction direction)
			{
				Win8ScrollArrowButtonStyle* decreaseButtonStyle=new Win8ScrollArrowButtonStyle(direction, false);
				return decreaseButtonStyle;
			}

			controls::GuiButton::IStyleController* Win8ScrollStyle::CreateIncreaseButtonStyle(Direction direction)
			{
				Win8ScrollArrowButtonStyle* increaseButtonStyle=new Win8ScrollArrowButtonStyle(direction, true);
				return increaseButtonStyle;
			}

			controls::GuiButton::IStyleController* Win8ScrollStyle::CreateHandleButtonStyle(Direction direction)
			{
				Win8ScrollHandleButtonStyle* handleButtonStyle=new Win8ScrollHandleButtonStyle;
				return handleButtonStyle;
			}

			compositions::GuiBoundsComposition* Win8ScrollStyle::InstallBackground(compositions::GuiBoundsComposition* boundsComposition, Direction direction)
			{
				GuiBoundsComposition* containerComposition=0;
				{
					GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
					element->SetColor(Color(255, 255, 255));
					
					GuiBoundsComposition* composition=new GuiBoundsComposition;
					switch(direction)
					{
					case Horizontal:
						{
							composition->SetPreferredMinSize(Size(0, 1));
							composition->SetAlignmentToParent(Margin(0, 0, 0, -1));
						}
						break;
					case Vertical:
						{
							composition->SetPreferredMinSize(Size(1, 0));
							composition->SetAlignmentToParent(Margin(0, 0, -1, 0));
						}
						break;
					}
					composition->SetOwnedElement(element);
					boundsComposition->AddChild(composition);
				}
				{
					GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
					element->SetColor(Win8GetSystemWindowColor());
					
					GuiBoundsComposition* composition=new GuiBoundsComposition;
					switch(direction)
					{
					case Horizontal:
						{
							composition->SetAlignmentToParent(Margin(0, 1, 0, 0));
						}
						break;
					case Vertical:
						{
							composition->SetAlignmentToParent(Margin(1, 0, 0, 0));
						}
						break;
					}
					composition->SetOwnedElement(element);
					boundsComposition->AddChild(composition);
					containerComposition=composition;
				}

				return containerComposition;
			}

			Win8ScrollStyle::Win8ScrollStyle(Direction _direction)
				:CommonScrollStyle(_direction)
			{
				BuildStyle(DefaultSize, ArrowSize);
			}

			Win8ScrollStyle::~Win8ScrollStyle()
			{
			}

/***********************************************************************
Win8ScrollViewProvider
***********************************************************************/

			Win8ScrollViewProvider::Win8ScrollViewProvider()
			{
			}

			Win8ScrollViewProvider::~Win8ScrollViewProvider()
			{
			}

			void Win8ScrollViewProvider::AssociateStyleController(controls::GuiControl::IStyleController* controller)
			{
			}
			
			void Win8ScrollViewProvider::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
			}

			void Win8ScrollViewProvider::SetText(const WString& value)
			{
			}

			void Win8ScrollViewProvider::SetFont(const FontProperties& value)
			{
			}

			void Win8ScrollViewProvider::SetVisuallyEnabled(bool value)
			{
			}

			controls::GuiScroll::IStyleController* Win8ScrollViewProvider::CreateHorizontalScrollStyle()
			{
				return new Win8ScrollStyle(Win8ScrollStyle::Horizontal);
			}

			controls::GuiScroll::IStyleController* Win8ScrollViewProvider::CreateVerticalScrollStyle()
			{
				return new Win8ScrollStyle(Win8ScrollStyle::Vertical);
			}

			int Win8ScrollViewProvider::GetDefaultScrollSize()
			{
				return Win8ScrollStyle::DefaultSize;
			}

			compositions::GuiGraphicsComposition* Win8ScrollViewProvider::InstallBackground(compositions::GuiBoundsComposition* boundsComposition)
			{
				GuiSolidBorderElement* border=GuiSolidBorderElement::Create();
				border->SetColor(Win8GetSystemBorderColor());
				boundsComposition->SetOwnedElement(border);
				boundsComposition->SetInternalMargin(Margin(1, 1, 1, 1));
				
				GuiSolidBackgroundElement* background=GuiSolidBackgroundElement::Create();
				background->SetColor(Win8GetSystemWindowColor());

				GuiBoundsComposition* backgroundComposition=new GuiBoundsComposition;
				boundsComposition->AddChild(backgroundComposition);
				backgroundComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
				backgroundComposition->SetOwnedElement(background);

				return boundsComposition;
			}

/***********************************************************************
Win8TextBoxBackground
***********************************************************************/

#define HOST_GETTER_BY_FOCUSABLE_COMPOSITION(STYLE) (style->focusableComposition->GetRelatedGraphicsHost())

			IMPLEMENT_TRANSFERRING_ANIMATION_BASE(Win8TextBoxColors, Win8TextBoxBackground, HOST_GETTER_BY_FOCUSABLE_COMPOSITION)
			{
				colorCurrent=Win8TextBoxColors::Blend(colorBegin, colorEnd, currentPosition, totalLength);
				style->Apply(colorCurrent);
			}

			void Win8TextBoxBackground::UpdateStyle()
			{
				if(!isVisuallyEnabled)
				{
					transferringAnimation->Transfer(Win8TextBoxColors::Disabled());
				}
				else if(isFocused)
				{
					transferringAnimation->Transfer(Win8TextBoxColors::Focused());
				}
				else if(isMouseEnter)
				{
					transferringAnimation->Transfer(Win8TextBoxColors::Active());
				}
				else
				{
					transferringAnimation->Transfer(Win8TextBoxColors::Normal());
				}
			}

			void Win8TextBoxBackground::Apply(const Win8TextBoxColors& colors)
			{
				borderElement->SetColor(colors.borderColor);
				backgroundElement->SetColor(colors.backgroundColor);
			}

			void Win8TextBoxBackground::OnBoundsMouseEnter(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				isMouseEnter=true;
				UpdateStyle();
			}

			void Win8TextBoxBackground::OnBoundsMouseLeave(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				isMouseEnter=false;
				UpdateStyle();
			}

			void Win8TextBoxBackground::OnBoundsGotFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				isFocused=true;
				UpdateStyle();
			}

			void Win8TextBoxBackground::OnBoundsLostFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				isFocused=false;
				UpdateStyle();
			}

			Win8TextBoxBackground::Win8TextBoxBackground()
				:backgroundElement(0)
				,borderElement(0)
				,focusableComposition(0)
				,isMouseEnter(false)
				,isFocused(false)
				,isVisuallyEnabled(false)
				,styleController(0)
				,textElement(0)
			{
				transferringAnimation=new TransferringAnimation(this, Win8TextBoxColors::Normal());
			}

			Win8TextBoxBackground::~Win8TextBoxBackground()
			{
				transferringAnimation->Disable();
			}

			void Win8TextBoxBackground::AssociateStyleController(controls::GuiControl::IStyleController* controller)
			{
				styleController=controller;
			}
			
			void Win8TextBoxBackground::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
				focusableComposition=value;
				focusableComposition->GetEventReceiver()->mouseEnter.AttachMethod(this, &Win8TextBoxBackground::OnBoundsMouseEnter);
				focusableComposition->GetEventReceiver()->mouseLeave.AttachMethod(this, &Win8TextBoxBackground::OnBoundsMouseLeave);
				focusableComposition->GetEventReceiver()->gotFocus.AttachMethod(this, &Win8TextBoxBackground::OnBoundsGotFocus);
				focusableComposition->GetEventReceiver()->lostFocus.AttachMethod(this, &Win8TextBoxBackground::OnBoundsLostFocus);
			}

			void Win8TextBoxBackground::SetVisuallyEnabled(bool value)
			{
				isVisuallyEnabled=value;
				UpdateStyle();
			}

			compositions::GuiGraphicsComposition* Win8TextBoxBackground::InstallBackground(compositions::GuiBoundsComposition* boundsComposition)
			{
				{
					GuiSolidBackgroundElement* background=GuiSolidBackgroundElement::Create();
					background->SetColor(Color(255, 255, 255));

					GuiBoundsComposition* backgroundComposition=new GuiBoundsComposition;
					boundsComposition->AddChild(backgroundComposition);
					backgroundComposition->SetAlignmentToParent(Margin(1, 1, 1, 1));
					backgroundComposition->SetOwnedElement(background);
				}
				{
					GuiSolidBackgroundElement* background=GuiSolidBackgroundElement::Create();
					background->SetColor(Color(255, 255, 255));

					GuiBoundsComposition* backgroundComposition=new GuiBoundsComposition;
					boundsComposition->AddChild(backgroundComposition);
					backgroundComposition->SetAlignmentToParent(Margin(2, 2, 2, 2));
					backgroundComposition->SetOwnedElement(background);
					backgroundElement=background;
				}
				{
					GuiSolidBorderElement* border=GuiSolidBorderElement::Create();
					border->SetColor(Win8GetSystemBorderColor());
					borderElement=border;

					GuiBoundsComposition* borderComposition=new GuiBoundsComposition;
					boundsComposition->AddChild(borderComposition);
					borderComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
					borderComposition->SetOwnedElement(border);
				}
				Apply(Win8TextBoxColors::Normal());
				{
					GuiBoundsComposition* containerComposition=new GuiBoundsComposition;
					boundsComposition->AddChild(containerComposition);
					containerComposition->SetAlignmentToParent(Margin(2, 2, 2, 2));
					return containerComposition;
				}
			}

			void Win8TextBoxBackground::InitializeTextElement(elements::GuiColorizedTextElement* _textElement)
			{
				textElement=_textElement;

				Array<text::ColorEntry> colors;
				colors.Resize(1);
				{
					colors[0]=Win8GetTextBoxTextColor();
				}
				textElement->SetColors(colors);
				textElement->SetCaretColor(Color(0, 0, 0));
			}

/***********************************************************************
Win8MultilineTextBoxProvider
***********************************************************************/

			Win8MultilineTextBoxProvider::Win8MultilineTextBoxProvider()
				:styleController(0)
			{
			}

			Win8MultilineTextBoxProvider::~Win8MultilineTextBoxProvider()
			{
			}

			void Win8MultilineTextBoxProvider::AssociateStyleController(controls::GuiControl::IStyleController* controller)
			{
				styleController=controller;
				background.AssociateStyleController(controller);
			}
			
			void Win8MultilineTextBoxProvider::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
				background.SetFocusableComposition(value);
				GuiMultilineTextBox::StyleController* textBoxController=dynamic_cast<GuiMultilineTextBox::StyleController*>(styleController);
				if(textBoxController)
				{
					background.InitializeTextElement(textBoxController->GetTextElement());
				}
			}

			void Win8MultilineTextBoxProvider::SetVisuallyEnabled(bool value)
			{
				background.SetVisuallyEnabled(value);
			}

			compositions::GuiGraphicsComposition* Win8MultilineTextBoxProvider::InstallBackground(compositions::GuiBoundsComposition* boundsComposition)
			{
				return background.InstallBackground(boundsComposition);
			}

/***********************************************************************
Win8SinglelineTextBoxProvider
***********************************************************************/

			Win8SinglelineTextBoxProvider::Win8SinglelineTextBoxProvider()
				:styleController(0)
			{
			}

			Win8SinglelineTextBoxProvider::~Win8SinglelineTextBoxProvider()
			{
			}

			void Win8SinglelineTextBoxProvider::AssociateStyleController(controls::GuiControl::IStyleController* controller)
			{
				styleController=controller;
				background.AssociateStyleController(controller);
			}
			
			void Win8SinglelineTextBoxProvider::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
				background.SetFocusableComposition(value);
				GuiSinglelineTextBox::StyleController* textBoxController=dynamic_cast<GuiSinglelineTextBox::StyleController*>(styleController);
				background.InitializeTextElement(textBoxController->GetTextElement());
			}

			void Win8SinglelineTextBoxProvider::SetText(const WString& value)
			{
			}

			void Win8SinglelineTextBoxProvider::SetFont(const FontProperties& value)
			{
			}

			void Win8SinglelineTextBoxProvider::SetVisuallyEnabled(bool value)
			{
				background.SetVisuallyEnabled(value);
			}

			compositions::GuiGraphicsComposition* Win8SinglelineTextBoxProvider::InstallBackground(compositions::GuiBoundsComposition* boundsComposition)
			{
				return background.InstallBackground(boundsComposition);
			}
		}
	}
}