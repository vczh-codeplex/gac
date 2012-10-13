#include "GuiWin8StylesCommon.h"

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
Win8ButtonColors
***********************************************************************/

			void Win8ButtonColors::SetAlphaWithoutText(unsigned char a)
			{
				borderColor.a=a;
				g1.a=a;
				g2.a=a;
			}

			Win8ButtonColors Win8ButtonColors::Blend(const Win8ButtonColors& c1, const Win8ButtonColors& c2, int ratio, int total)
			{
				if(ratio<0) ratio=0;
				else if(ratio>total) ratio=total;

				Win8ButtonColors result;
				result.borderColor=BlendColor(c1.borderColor, c2.borderColor, ratio, total);
				result.g1=BlendColor(c1.g1, c2.g1, ratio, total);
				result.g2=BlendColor(c1.g2, c2.g2, ratio, total);
				result.textColor=BlendColor(c1.textColor, c2.textColor, ratio, total);
				result.bulletLight=BlendColor(c1.bulletLight, c2.bulletLight, ratio, total);
				result.bulletDark=BlendColor(c1.bulletDark, c2.bulletDark, ratio, total);
				return result;
			}

			//---------------------------------------------------------

			Win8ButtonColors Win8ButtonColors::ButtonNormal()
			{
				Win8ButtonColors colors=
				{
					Color(172, 172, 172),
					Color(239, 239, 239),
					Color(229, 229, 229),
					Win8GetSystemTextColor(true),
				};
				return colors;
			}

			Win8ButtonColors Win8ButtonColors::ButtonActive()
			{
				Win8ButtonColors colors=
				{
					Color(126, 180, 234),
					Color(235, 244, 252),
					Color(220, 236, 252),
					Win8GetSystemTextColor(true),
				};
				return colors;
			}

			Win8ButtonColors Win8ButtonColors::ButtonPressed()
			{
				Win8ButtonColors colors=
				{
					Color(86, 157, 229),
					Color(218, 236, 252),
					Color(196, 224, 252),
					Win8GetSystemTextColor(true),
				};
				return colors;
			}

			Win8ButtonColors Win8ButtonColors::ButtonDisabled()
			{
				Win8ButtonColors colors=
				{
					Color(173, 178, 181),
					Color(252, 252, 252),
					Color(252, 252, 252),
					Win8GetSystemTextColor(false),
				};
				return colors;
			}

/***********************************************************************
Win8ButtonElements
***********************************************************************/

			Win8ButtonElements Win8ButtonElements::Create(Alignment::Type horizontal, Alignment::Type vertical)
			{
				Win8ButtonElements button;
				{
					button.mainComposition=new GuiBoundsComposition;
					button.mainComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
				}
				{
					GuiSolidBorderElement* element=GuiSolidBorderElement::Create();
					button.rectBorderElement=element;

					GuiBoundsComposition* composition=new GuiBoundsComposition;
					button.mainComposition->AddChild(composition);
					composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
					composition->SetOwnedElement(element);
				}
				{
					GuiGradientBackgroundElement* element=GuiGradientBackgroundElement::Create();
					button.backgroundElement=element;
					element->SetDirection(GuiGradientBackgroundElement::Vertical);
					element->SetShape(ElementShape::Rectangle);

					GuiBoundsComposition* composition=new GuiBoundsComposition;
					button.backgroundComposition=composition;
					button.mainComposition->AddChild(composition);
					composition->SetAlignmentToParent(Margin(1, 1, 1, 1));
					composition->SetOwnedElement(element);
				}
				{
					Win8CreateSolidLabelElement(button.textElement, button.textComposition, horizontal, vertical);
					button.mainComposition->AddChild(button.textComposition);
				}
				return button;
			}

			void Win8ButtonElements::Apply(const Win8ButtonColors& colors)
			{
				if(rectBorderElement)
				{
					rectBorderElement->SetColor(colors.borderColor);
				}
				backgroundElement->SetColors(colors.g1, colors.g2);
				textElement->SetColor(colors.textColor);
			}

/***********************************************************************
Helpers
***********************************************************************/

			Color Win8GetSystemWindowColor()
			{
				return Color(240, 240, 240);
			}

			Color Win8GetSystemTextColor(bool enabled)
			{
				return enabled?Color(0, 0, 0):Color(131, 131, 131);
			}

			void Win8SetFont(GuiSolidLabelElement* element, GuiBoundsComposition* composition, const FontProperties& fontProperties)
			{
				int margin=3;
				element->SetFont(fontProperties);
				composition->SetMargin(Margin(margin, margin, margin, margin));
			}

			void Win8CreateSolidLabelElement(GuiSolidLabelElement*& element, GuiBoundsComposition*& composition, Alignment::Type horizontal, Alignment::Type vertical)
			{
				element=GuiSolidLabelElement::Create();
				element->SetAlignments(horizontal, vertical);

				composition=new GuiBoundsComposition;
				composition->SetOwnedElement(element);
				composition->SetMargin(Margin(0, 0, 0, 0));
				composition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
				composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
			}
		}
	}
}