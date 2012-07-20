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

			IMPLEMENT_TRANSFERRING_ANIMATION(Win7ButtonColors, Win7ToolstripButtonStyle)
			{
				colorCurrent=Win7ButtonColors::Blend(colorBegin, colorEnd, currentPosition, totalLength);
				style->elements.Apply(colorCurrent);
			}

			void Win7ToolstripButtonStyle::TransferInternal(GuiButton::ControlState value, bool enabled, bool menuOpening)
			{
				Win7ButtonColors targetColor;
				if(enabled)
				{
					if(menuOpening)
					{
						value=GuiButton::Pressed;
					}
					switch(value)
					{
					case GuiButton::Normal:
						targetColor=Win7ButtonColors::ToolstripButtonNormal();
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
				}
				transferringAnimation->Transfer(targetColor);
			}

			Win7ToolstripButtonStyle::Win7ToolstripButtonStyle(bool transparent, ButtonStyle _buttonStyle)
				:controlStyle(GuiButton::Normal)
				,isVisuallyEnabled(true)
				,isOpening(false)
			{
				elements=Win7ButtonElements::Create(true, true, Alignment::Center, Alignment::Center);
				elements.Apply(Win7ButtonColors::ToolstripButtonNormal());
				transferringAnimation=new TransferringAnimation(this, Win7ButtonColors::ToolstripButtonNormal());

				elements.textComposition->SetMinSizeLimitation(GuiGraphicsComposition::NoLimit);
				imageElement=GuiImageFrameElement::Create();
				imageComposition=new GuiBoundsComposition;
				imageComposition->SetOwnedElement(imageElement);
				imageComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
				imageComposition->SetAlignmentToParent(Margin(3, 3, 3, 3));
				GetContainerComposition()->AddChild(imageComposition);
			}

			Win7ToolstripButtonStyle::~Win7ToolstripButtonStyle()
			{
				transferringAnimation->Disable();
			}

			compositions::GuiBoundsComposition* Win7ToolstripButtonStyle::GetBoundsComposition()
			{
				return elements.mainComposition;
			}

			compositions::GuiGraphicsComposition* Win7ToolstripButtonStyle::GetContainerComposition()
			{
				return elements.mainComposition;
			}

			void Win7ToolstripButtonStyle::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
			}

			void Win7ToolstripButtonStyle::SetText(const WString& value)
			{
			}

			void Win7ToolstripButtonStyle::SetFont(const FontProperties& value)
			{
				Win7SetFont(elements.textElement, elements.textComposition, value);
			}

			void Win7ToolstripButtonStyle::SetVisuallyEnabled(bool value)
			{
				if(isVisuallyEnabled!=value)
				{
					isVisuallyEnabled=value;
					TransferInternal(controlStyle, isVisuallyEnabled, isOpening);
				}
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
					TransferInternal(controlStyle, isVisuallyEnabled, isOpening);
				}
			}

			controls::GuiButton* Win7ToolstripButtonStyle::GetSubMenuHost()
			{
				return 0;
			}

			void Win7ToolstripButtonStyle::SetImage(Ptr<controls::GuiImageData> value)
			{
				if(value)
				{
					imageElement->SetImage(value->GetImage(), value->GetFrameIndex());
				}
				else
				{
					imageElement->SetImage(0, 0);
				}
			}

			void Win7ToolstripButtonStyle::Transfer(controls::GuiButton::ControlState value)
			{
				if(controlStyle!=value)
				{
					controlStyle=value;
					TransferInternal(controlStyle, isVisuallyEnabled, isOpening);
				}
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