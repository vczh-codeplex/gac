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
				backgroundColor.a=a;
			}

			Win8ButtonColors Win8ButtonColors::Blend(const Win8ButtonColors& c1, const Win8ButtonColors& c2, int ratio, int total)
			{
				if(ratio<0) ratio=0;
				else if(ratio>total) ratio=total;

				Win8ButtonColors result;
				result.borderColor=BlendColor(c1.borderColor, c2.borderColor, ratio, total);
				result.backgroundColor=BlendColor(c1.backgroundColor, c2.backgroundColor, ratio, total);
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
					Color(232, 232, 232),
					Win8GetSystemTextColor(true),
				};
				return colors;
			}

			Win8ButtonColors Win8ButtonColors::ButtonActive()
			{
				Win8ButtonColors colors=
				{
					Color(126, 180, 234),
					Color(232, 242, 252),
					Win8GetSystemTextColor(true),
				};
				return colors;
			}

			Win8ButtonColors Win8ButtonColors::ButtonPressed()
			{
				Win8ButtonColors colors=
				{
					Color(217, 217, 217),
					Color(239, 239, 239),
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
					GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
					button.backgroundElement=element;

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
				backgroundElement->SetColor(colors.backgroundColor);
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