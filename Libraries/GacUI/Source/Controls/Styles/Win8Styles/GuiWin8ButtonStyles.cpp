#include "GuiWin8ButtonStyles.h"

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
Win8ButtonStyleBase
***********************************************************************/

			IMPLEMENT_TRANSFERRING_ANIMATION(Win8ButtonColors, Win8ButtonStyleBase)
			{
				colorCurrent=Win8ButtonColors::Blend(colorBegin, colorEnd, currentPosition, totalLength);
				style->elements.Apply(colorCurrent);
			}

			Win8ButtonStyleBase::Win8ButtonStyleBase(const Win8ButtonColors& initialColor, Alignment::Type horizontal, Alignment::Type vertical)
				:controlStyle(GuiButton::Normal)
				,isVisuallyEnabled(true)
				,isSelected(false)
				,transparentWhenInactive(false)
				,transparentWhenDisabled(false)
			{
				elements=Win8ButtonElements::Create(horizontal, vertical);
				elements.Apply(initialColor);
				transferringAnimation=new TransferringAnimation(this, initialColor);
			}

			Win8ButtonStyleBase::~Win8ButtonStyleBase()
			{
				transferringAnimation->Disable();
			}

			compositions::GuiBoundsComposition* Win8ButtonStyleBase::GetBoundsComposition()
			{
				return elements.mainComposition;
			}

			compositions::GuiGraphicsComposition* Win8ButtonStyleBase::GetContainerComposition()
			{
				return elements.mainComposition;
			}

			void Win8ButtonStyleBase::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
			}

			void Win8ButtonStyleBase::SetText(const WString& value)
			{
				elements.textElement->SetText(value);
			}

			void Win8ButtonStyleBase::SetFont(const FontProperties& value)
			{
				Win8SetFont(elements.textElement, elements.textComposition, value);
			}

			void Win8ButtonStyleBase::SetVisuallyEnabled(bool value)
			{
				if(isVisuallyEnabled!=value)
				{
					isVisuallyEnabled=value;
					TransferInternal(controlStyle, isVisuallyEnabled, isSelected);
				}
			}

			void Win8ButtonStyleBase::SetSelected(bool value)
			{
				if(isSelected!=value)
				{
					isSelected=value;
					TransferInternal(controlStyle, isVisuallyEnabled, isSelected);
				}
			}

			void Win8ButtonStyleBase::Transfer(GuiButton::ControlState value)
			{
				if(controlStyle!=value)
				{
					controlStyle=value;
					TransferInternal(controlStyle, isVisuallyEnabled, isSelected);
				}
			}

			bool Win8ButtonStyleBase::GetTransparentWhenInactive()
			{
				return transparentWhenInactive;
			}

			void Win8ButtonStyleBase::SetTransparentWhenInactive(bool value)
			{
				transparentWhenInactive=value;
				TransferInternal(controlStyle, isVisuallyEnabled, isSelected);
			}

			bool Win8ButtonStyleBase::GetTransparentWhenDisabled()
			{
				return transparentWhenDisabled;
			}

			void Win8ButtonStyleBase::SetTransparentWhenDisabled(bool value)
			{
				transparentWhenDisabled=value;
				TransferInternal(controlStyle, isVisuallyEnabled, isSelected);
			}

			bool Win8ButtonStyleBase::GetAutoSizeForText()
			{
				return elements.textComposition->GetMinSizeLimitation()!=GuiGraphicsComposition::NoLimit;
			}

			void Win8ButtonStyleBase::SetAutoSizeForText(bool value)
			{
				if(value)
				{
					elements.textComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
				}
				else
				{
					elements.textComposition->SetMinSizeLimitation(GuiGraphicsComposition::NoLimit);
				}
			}

/***********************************************************************
Win8ButtonStyle
***********************************************************************/

			void Win8ButtonStyle::TransferInternal(GuiButton::ControlState value, bool enabled, bool selected)
			{
				Win8ButtonColors targetColor;
				if(enabled)
				{
					switch(value)
					{
					case GuiButton::Normal:
						targetColor=Win8ButtonColors::ButtonNormal();
						if(transparentWhenInactive)
						{
							targetColor.SetAlphaWithoutText(0);
						}
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
					if(transparentWhenDisabled)
					{
						targetColor.SetAlphaWithoutText(0);
					}
				}
				transferringAnimation->Transfer(targetColor);
			}

			Win8ButtonStyle::Win8ButtonStyle()
				:Win8ButtonStyleBase(Win8ButtonColors::ButtonNormal(), Alignment::Center, Alignment::Center)
			{
			}

			Win8ButtonStyle::~Win8ButtonStyle()
			{
			}
		}
	}
}