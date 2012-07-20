#include "GuiWin7ToolstripStyles.h"

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
Win7ToolstripButtonStyle
***********************************************************************/

			void Win7ToolstripButtonStyle::TransferInternal(GuiButton::ControlState value, bool enabled, bool selected)
			{
				Win7ButtonColors targetColor;
				if(enabled)
				{
					if(isOpening)
					{
						value=GuiButton::Pressed;
					}
					switch(value)
					{
					case GuiButton::Normal:
						targetColor=Win7ButtonColors::ToolstripButtonNormal();
						if(transparentWhenInactive)
						{
							targetColor.SetAlphaWithoutText(0);
						}
						break;
					case GuiButton::Active:
						targetColor=Win7ButtonColors::ToolstripButtonActive();
						break;
					case GuiButton::Pressed:
						targetColor=Win7ButtonColors::ToolstripButtonPressed();
						break;
					}
				}
				else
				{
					targetColor=Win7ButtonColors::ToolstripButtonDisabled();
					if(transparentWhenDisabled)
					{
						targetColor.SetAlphaWithoutText(0);
					}
				}
				transferringAnimation->Transfer(targetColor);
			}

			Win7ToolstripButtonStyle::Win7ToolstripButtonStyle(bool transparent)
				:Win7ButtonStyleBase(true, true, Win7ButtonColors::ToolstripButtonNormal(), Alignment::Center, Alignment::Center)
				,isOpening(false)
			{
				SetAutoSizeForText(false);
				SetTransparentWhenInactive(transparent);
				SetTransparentWhenDisabled(transparent);
			}

			Win7ToolstripButtonStyle::~Win7ToolstripButtonStyle()
			{
			}

			controls::GuiMenu::IStyleController* Win7ToolstripButtonStyle::CreateSubMenuStyleController()
			{
				return new Win7MenuStyle;
			}

			void Win7ToolstripButtonStyle::SetSubMenuExisting(bool value)
			{
			}

			void Win7ToolstripButtonStyle::SetSubMenuOpening(bool value)
			{
				if(isOpening!=value)
				{
					isOpening=value;
					TransferInternal(controlStyle, isVisuallyEnabled, isSelected);
				}
			}

			controls::GuiButton* Win7ToolstripButtonStyle::GetSubMenuHost()
			{
				return 0;
			}

			void Win7ToolstripButtonStyle::SetImage(Ptr<controls::GuiImageData> value)
			{
			}

			void Win7ToolstripButtonStyle::Transfer(controls::GuiButton::ControlState value)
			{
				Win7ButtonStyleBase::Transfer(value);
			}

/***********************************************************************
Win7ToolstripDropdownButtonStyle
***********************************************************************/

			Win7ToolstripDropdownButtonStyle::Win7ToolstripDropdownButtonStyle(bool transparent)
				:Win7ToolstripButtonStyle(transparent)
			{
			}

			Win7ToolstripDropdownButtonStyle::~Win7ToolstripDropdownButtonStyle()
			{
			}

/***********************************************************************
Win7ToolstripSplitButtonStyle
***********************************************************************/

			Win7ToolstripSplitButtonStyle::Win7ToolstripSplitButtonStyle(bool transparent)
				:Win7ToolstripButtonStyle(transparent)
			{
			}

			Win7ToolstripSplitButtonStyle::~Win7ToolstripSplitButtonStyle()
			{
			}

/***********************************************************************
Win7ToolstripSplitterStyle
***********************************************************************/

			Win7ToolstripSplitterStyle::Win7ToolstripSplitterStyle()
			{
				Color dark=Win7GetSystemBorderSinkColor();
				Color bright=Win7GetSystemBorderRaiseColor();

				GuiBoundsComposition* bounds=new GuiBoundsComposition;
				bounds->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
				bounds->SetAlignmentToParent(Margin(0, 0, 0, 0));
				boundsComposition=bounds;

				GuiSolidBackgroundElement* backgroundElement=GuiSolidBackgroundElement::Create();
				bounds->SetOwnedElement(backgroundElement);
				backgroundElement->SetColor(Win7GetSystemWindowColor());

				GuiBoundsComposition* splitterComposition=new GuiBoundsComposition;
				bounds->AddChild(splitterComposition);
				splitterComposition->SetAlignmentToParent(Margin(3, 3, 3, 3));

				Gui3DSplitterElement* splitterElement=Gui3DSplitterElement::Create();
				splitterComposition->SetOwnedElement(splitterElement);
				splitterElement->SetColors(dark, bright);
				splitterElement->SetDirection(Gui3DSplitterElement::Vertical);
			}

			Win7ToolstripSplitterStyle::~Win7ToolstripSplitterStyle()
			{
			}

			compositions::GuiBoundsComposition* Win7ToolstripSplitterStyle::GetBoundsComposition()
			{
				return boundsComposition;
			}

			compositions::GuiGraphicsComposition* Win7ToolstripSplitterStyle::GetContainerComposition()
			{
				return boundsComposition;
			}

			void Win7ToolstripSplitterStyle::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
			}

			void Win7ToolstripSplitterStyle::SetText(const WString& value)
			{
			}

			void Win7ToolstripSplitterStyle::SetFont(const FontProperties& value)
			{
			}

			void Win7ToolstripSplitterStyle::SetVisuallyEnabled(bool value)
			{
			}
		}
	}
}