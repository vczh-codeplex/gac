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

			Win8ScrollArrowButtonStyle::Win8ScrollArrowButtonStyle()
				:Win8ButtonStyleBase(Win8ButtonColors::ScrollArrowNormal(), Alignment::Center, Alignment::Center)
			{
			}

			Win8ScrollArrowButtonStyle::~Win8ScrollArrowButtonStyle()
			{
			}

/***********************************************************************
Win8ScrollStyle
***********************************************************************/

			controls::GuiButton::IStyleController* Win8ScrollStyle::CreateDecreaseButtonStyle(Direction direction)
			{
				Win8ScrollArrowButtonStyle* decreaseButtonStyle=new Win8ScrollArrowButtonStyle;
				return decreaseButtonStyle;
			}

			controls::GuiButton::IStyleController* Win8ScrollStyle::CreateIncreaseButtonStyle(Direction direction)
			{
				Win8ScrollArrowButtonStyle* increaseButtonStyle=new Win8ScrollArrowButtonStyle;
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
		}
	}
}