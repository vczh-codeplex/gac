#include "GuiWin7ControlStyles.h"

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
Win7EmptyStyle
***********************************************************************/

			Win7EmptyStyle::Win7EmptyStyle(Color color)
			{
				GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
				element->SetColor(color);
				
				boundsComposition=new GuiBoundsComposition;
				boundsComposition->SetOwnedElement(element);
			}

			Win7EmptyStyle::~Win7EmptyStyle()
			{
			}

			compositions::GuiBoundsComposition* Win7EmptyStyle::GetBoundsComposition()
			{
				return boundsComposition;
			}

			compositions::GuiGraphicsComposition* Win7EmptyStyle::GetContainerComposition()
			{
				return boundsComposition;
			}

			void Win7EmptyStyle::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
			}

			void Win7EmptyStyle::SetText(const WString& value)
			{
			}

			void Win7EmptyStyle::SetFont(const FontProperties& value)
			{
			}

			void Win7EmptyStyle::SetVisuallyEnabled(bool value)
			{
			}

/***********************************************************************
Win7WindowStyle
***********************************************************************/

			Win7WindowStyle::Win7WindowStyle()
			{
				GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
				element->SetColor(Win7GetSystemWindowColor());
				
				boundsComposition=new GuiBoundsComposition;
				boundsComposition->SetOwnedElement(element);
			}

			Win7WindowStyle::~Win7WindowStyle()
			{
			}

			compositions::GuiBoundsComposition* Win7WindowStyle::GetBoundsComposition()
			{
				return boundsComposition;
			}

			compositions::GuiGraphicsComposition* Win7WindowStyle::GetContainerComposition()
			{
				return boundsComposition;
			}

			void Win7WindowStyle::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
			}

			void Win7WindowStyle::SetText(const WString& value)
			{
			}

			void Win7WindowStyle::SetFont(const FontProperties& value)
			{
			}

			void Win7WindowStyle::SetVisuallyEnabled(bool value)
			{
			}

/***********************************************************************
Win7LabelStyle
***********************************************************************/

			Win7LabelStyle::Win7LabelStyle()
			{
				textElement=GuiSolidLabelElement::Create();
				textElement->SetColor(GetDefaultTextColor());
				
				boundsComposition=new GuiBoundsComposition;
				boundsComposition->SetOwnedElement(textElement);
				boundsComposition->SetMinSizeLimitation(GuiBoundsComposition::LimitToElement);
			}

			Win7LabelStyle::~Win7LabelStyle()
			{
			}

			compositions::GuiBoundsComposition* Win7LabelStyle::GetBoundsComposition()
			{
				return boundsComposition;
			}

			compositions::GuiGraphicsComposition* Win7LabelStyle::GetContainerComposition()
			{
				return boundsComposition;
			}

			void Win7LabelStyle::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
			}

			void Win7LabelStyle::SetText(const WString& value)
			{
				textElement->SetText(value);
			}

			void Win7LabelStyle::SetFont(const FontProperties& value)
			{
				textElement->SetFont(value);
			}

			void Win7LabelStyle::SetVisuallyEnabled(bool value)
			{
			}

			Color Win7LabelStyle::GetDefaultTextColor()
			{
				return Win7GetSystemTextColor(true);
			}

			void Win7LabelStyle::SetTextColor(Color value)
			{
				textElement->SetColor(value);
			}

/***********************************************************************
Win7GroupBoxStyle
***********************************************************************/
			
			IMPLEMENT_TRANSFERRING_ANIMATION(Color, Win7GroupBoxStyle)
			{
				colorCurrent=BlendColor(colorBegin, colorEnd, currentPosition, totalLength);
				style->textElement->SetColor(colorCurrent);
			}

			void Win7GroupBoxStyle::SetMargins(int fontSize)
			{
				fontSize+=4;
				int half=fontSize/2;
				sinkBorderComposition->SetAlignmentToParent(Margin(0, half, 1, 1));
				raisedBorderComposition->SetAlignmentToParent(Margin(1, half+1, 0, 0));
				containerComposition->SetAlignmentToParent(Margin(2, fontSize, 2, 2));
				textBackgroundComposition->SetAlignmentToParent(Margin(half, 2, -1, -1));
			}

			Win7GroupBoxStyle::Win7GroupBoxStyle()
			{
				boundsComposition=new GuiBoundsComposition;
				{
					GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
					element->SetColor(Win7GetSystemWindowColor());

					boundsComposition->SetOwnedElement(element);
					boundsComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
				}

				sinkBorderComposition=new GuiBoundsComposition;
				{
					GuiRoundBorderElement* element=GuiRoundBorderElement::Create();
					element->SetRadius(3);
					element->SetColor(Color(213, 223, 229));

					sinkBorderComposition->SetOwnedElement(element);
					boundsComposition->AddChild(sinkBorderComposition);
				}

				raisedBorderComposition=new GuiBoundsComposition;
				{
					GuiRoundBorderElement* element=GuiRoundBorderElement::Create();
					element->SetRadius(3);
					element->SetColor(Color(255, 255, 255));

					raisedBorderComposition->SetOwnedElement(element);
					boundsComposition->AddChild(raisedBorderComposition);
				}

				textBackgroundComposition=new GuiBoundsComposition;
				{
					GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
					element->SetColor(Win7GetSystemWindowColor());

					textBackgroundComposition->SetOwnedElement(element);
					textBackgroundComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
					boundsComposition->AddChild(textBackgroundComposition);
				}

				textComposition=new GuiBoundsComposition;
				{
					GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
					element->SetColor(Win7GetSystemTextColor(true));
					textElement=element;

					textComposition->SetOwnedElement(element);
					textComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
					textComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
					textBackgroundComposition->AddChild(textComposition);
				}

				containerComposition=new GuiBoundsComposition;
				{
					boundsComposition->AddChild(containerComposition);
				}

				SetMargins(0);
				transferringAnimation=new TransferringAnimation(this, Win7GetSystemTextColor(true));
			}

			Win7GroupBoxStyle::~Win7GroupBoxStyle()
			{
				transferringAnimation->Disable();
			}

			compositions::GuiBoundsComposition* Win7GroupBoxStyle::GetBoundsComposition()
			{
				return boundsComposition;
			}

			compositions::GuiGraphicsComposition* Win7GroupBoxStyle::GetContainerComposition()
			{
				return containerComposition;
			}

			void Win7GroupBoxStyle::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
			}

			void Win7GroupBoxStyle::SetText(const WString& value)
			{
				textElement->SetText(value);
			}

			void Win7GroupBoxStyle::SetFont(const FontProperties& value)
			{
				textElement->SetFont(value);
				SetMargins(value.size);
			}

			void Win7GroupBoxStyle::SetVisuallyEnabled(bool value)
			{
				if(value)
				{
					transferringAnimation->Transfer(Win7GetSystemTextColor(true));
				}
				else
				{
					transferringAnimation->Transfer(Win7GetSystemTextColor(false));
				}
			}
		}
	}
}