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
			{
				SetAutoSizeForText(false);
				SetTransparentWhenInactive(transparent);
				SetTransparentWhenDisabled(transparent);
			}

			Win7ToolstripButtonStyle::~Win7ToolstripButtonStyle()
			{
			}

/***********************************************************************
Win7ToolstripDropdownButtonStyle
***********************************************************************/

			void Win7ToolstripDropdownButtonStyle::TransferInternal(GuiButton::ControlState value, bool enabled, bool opening)
			{
				Win7ButtonColors targetColor;
				bool active=false;
				if(enabled)
				{
					if(opening)
					{
						targetColor=Win7ButtonColors::MenuItemButtonNormalActive();
						active=true;
					}
					else
					{
						switch(value)
						{
						case GuiButton::Normal:
							targetColor=Win7ButtonColors::MenuItemButtonNormal();
							break;
						case GuiButton::Active:
						case GuiButton::Pressed:
							targetColor=Win7ButtonColors::MenuItemButtonNormalActive();
							active=true;
							break;
						}
					}
				}
				else
				{
					switch(value)
					{
					case GuiButton::Normal:
						targetColor=Win7ButtonColors::MenuItemButtonDisabled();
						break;
					case GuiButton::Active:
					case GuiButton::Pressed:
						targetColor=Win7ButtonColors::MenuItemButtonDisabledActive();
						active=true;
						break;
					}
				}
				elements.Apply(targetColor);
				elements.SetActive(active);
			}

			Win7ToolstripDropdownButtonStyle::Win7ToolstripDropdownButtonStyle()
				:controlStyle(GuiButton::Normal)
				,isVisuallyEnabled(true)
				,isOpening(false)
			{
				elements=Win7MenuItemButtonElements::Create();
				elements.Apply(Win7ButtonColors::MenuItemButtonNormal());
			}

			Win7ToolstripDropdownButtonStyle::~Win7ToolstripDropdownButtonStyle()
			{
			}

			compositions::GuiBoundsComposition* Win7ToolstripDropdownButtonStyle::GetBoundsComposition()
			{
				return elements.mainComposition;
			}

			compositions::GuiGraphicsComposition* Win7ToolstripDropdownButtonStyle::GetContainerComposition()
			{
				return elements.mainComposition;
			}

			void Win7ToolstripDropdownButtonStyle::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
			}

			void Win7ToolstripDropdownButtonStyle::SetText(const WString& value)
			{
				elements.textElement->SetText(value);
			}

			void Win7ToolstripDropdownButtonStyle::SetFont(const FontProperties& value)
			{
				Win7SetFont(elements.textElement, elements.textComposition, value);
			}

			void Win7ToolstripDropdownButtonStyle::SetVisuallyEnabled(bool value)
			{
				if(isVisuallyEnabled!=value)
				{
					isVisuallyEnabled=value;
					TransferInternal(controlStyle, isVisuallyEnabled, isOpening);
				}
			}

			controls::GuiMenu::IStyleController* Win7ToolstripDropdownButtonStyle::CreateSubMenuStyleController()
			{
				return new Win7MenuStyle;
			}

			void Win7ToolstripDropdownButtonStyle::SetSubMenuExisting(bool value)
			{
				elements.SetSubMenuExisting(value);
			}

			void Win7ToolstripDropdownButtonStyle::SetSubMenuOpening(bool value)
			{
				if(isOpening!=value)
				{
					isOpening=value;
					TransferInternal(controlStyle, isVisuallyEnabled, isOpening);
				}
			}

			controls::GuiButton* Win7ToolstripDropdownButtonStyle::GetSubMenuHost()
			{
				return 0;
			}

			void Win7ToolstripDropdownButtonStyle::SetImage(Ptr<controls::GuiImageData> value)
			{
				elements.imageElement->SetImage(value->GetImage(), value->GetFrameIndex());
			}

			void Win7ToolstripDropdownButtonStyle::Transfer(GuiButton::ControlState value)
			{
				if(controlStyle!=value)
				{
					controlStyle=value;
					TransferInternal(controlStyle, isVisuallyEnabled, isOpening);
				}
			}

/***********************************************************************
Win7ToolstripSplitButtonStyle
***********************************************************************/

			void Win7ToolstripSplitButtonStyle::TransferInternal(GuiButton::ControlState value, bool enabled, bool opening)
			{
				Win7ButtonColors targetColor;
				bool active=false;
				if(enabled)
				{
					if(opening)
					{
						targetColor=Win7ButtonColors::MenuItemButtonNormalActive();
						active=true;
					}
					else
					{
						switch(value)
						{
						case GuiButton::Normal:
							targetColor=Win7ButtonColors::MenuItemButtonNormal();
							break;
						case GuiButton::Active:
						case GuiButton::Pressed:
							targetColor=Win7ButtonColors::MenuItemButtonNormalActive();
							active=true;
							break;
						}
					}
				}
				else
				{
					switch(value)
					{
					case GuiButton::Normal:
						targetColor=Win7ButtonColors::MenuItemButtonDisabled();
						break;
					case GuiButton::Active:
					case GuiButton::Pressed:
						targetColor=Win7ButtonColors::MenuItemButtonDisabledActive();
						active=true;
						break;
					}
				}
				elements.Apply(targetColor);
				elements.SetActive(active);
			}

			Win7ToolstripSplitButtonStyle::Win7ToolstripSplitButtonStyle()
				:controlStyle(GuiButton::Normal)
				,isVisuallyEnabled(true)
				,isOpening(false)
			{
				elements=Win7MenuItemButtonElements::Create();
				elements.Apply(Win7ButtonColors::MenuItemButtonNormal());
			}

			Win7ToolstripSplitButtonStyle::~Win7ToolstripSplitButtonStyle()
			{
			}

			compositions::GuiBoundsComposition* Win7ToolstripSplitButtonStyle::GetBoundsComposition()
			{
				return elements.mainComposition;
			}

			compositions::GuiGraphicsComposition* Win7ToolstripSplitButtonStyle::GetContainerComposition()
			{
				return elements.mainComposition;
			}

			void Win7ToolstripSplitButtonStyle::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
			}

			void Win7ToolstripSplitButtonStyle::SetText(const WString& value)
			{
				elements.textElement->SetText(value);
			}

			void Win7ToolstripSplitButtonStyle::SetFont(const FontProperties& value)
			{
				Win7SetFont(elements.textElement, elements.textComposition, value);
			}

			void Win7ToolstripSplitButtonStyle::SetVisuallyEnabled(bool value)
			{
				if(isVisuallyEnabled!=value)
				{
					isVisuallyEnabled=value;
					TransferInternal(controlStyle, isVisuallyEnabled, isOpening);
				}
			}

			controls::GuiMenu::IStyleController* Win7ToolstripSplitButtonStyle::CreateSubMenuStyleController()
			{
				return new Win7MenuStyle;
			}

			void Win7ToolstripSplitButtonStyle::SetSubMenuExisting(bool value)
			{
				elements.SetSubMenuExisting(value);
			}

			void Win7ToolstripSplitButtonStyle::SetSubMenuOpening(bool value)
			{
				if(isOpening!=value)
				{
					isOpening=value;
					TransferInternal(controlStyle, isVisuallyEnabled, isOpening);
				}
			}

			controls::GuiButton* Win7ToolstripSplitButtonStyle::GetSubMenuHost()
			{
				return 0;
			}

			void Win7ToolstripSplitButtonStyle::SetImage(Ptr<controls::GuiImageData> value)
			{
				elements.imageElement->SetImage(value->GetImage(), value->GetFrameIndex());
			}

			void Win7ToolstripSplitButtonStyle::Transfer(GuiButton::ControlState value)
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