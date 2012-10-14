/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control Styles::Windows8 Styles

Clases:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_WIN8STYLES_GUIWIN7STYLESCOMMON
#define VCZH_PRESENTATION_CONTROLS_WIN8STYLES_GUIWIN7STYLESCOMMON

#include "..\GuiCommonStyles.h"

namespace vl
{
	namespace presentation
	{
		namespace win8
		{

/***********************************************************************
Button Configuration
***********************************************************************/
			
			struct Win8ButtonColors
			{
				Color										borderColor;
				Color										g1;
				Color										g2;
				Color										textColor;
				Color										bullet;

				bool operator==(const Win8ButtonColors& colors)
				{
					return
						borderColor == colors.borderColor &&
						g1 == colors.g1 &&
						g2 == colors.g2 &&
						textColor == colors.textColor &&
						bullet == colors.bullet;
				}

				bool operator!=(const Win8ButtonColors& colors)
				{
					return !(*this==colors);
				}

				void										SetAlphaWithoutText(unsigned char a);

				static Win8ButtonColors						Blend(const Win8ButtonColors& c1, const Win8ButtonColors& c2, int ratio, int total);

				static Win8ButtonColors						ButtonNormal();
				static Win8ButtonColors						ButtonActive();
				static Win8ButtonColors						ButtonPressed();
				static Win8ButtonColors						ButtonDisabled();
				
				static Win8ButtonColors						CheckedNormal(bool selected);
				static Win8ButtonColors						CheckedActive(bool selected);
				static Win8ButtonColors						CheckedPressed(bool selected);
				static Win8ButtonColors						CheckedDisabled(bool selected);
			};

			struct Win8ButtonElements
			{
				elements::GuiSolidBorderElement*			rectBorderElement;
				elements::GuiGradientBackgroundElement*		backgroundElement;
				elements::GuiSolidLabelElement*				textElement;
				compositions::GuiBoundsComposition*			textComposition;
				compositions::GuiBoundsComposition*			mainComposition;
				compositions::GuiBoundsComposition*			backgroundComposition;

				static Win8ButtonElements					Create(Alignment::Type horizontal=Alignment::Center, Alignment::Type vertical=Alignment::Center);
				void										Apply(const Win8ButtonColors& colors);
			};

			struct Win8CheckedButtonElements
			{
				elements::GuiSolidBorderElement*			bulletBorderElement;
				elements::GuiGradientBackgroundElement*		bulletBackgroundElement;
				elements::GuiSolidLabelElement*				bulletCheckElement;
				elements::GuiSolidBackgroundElement*		bulletRadioElement;
				elements::GuiSolidLabelElement*				textElement;
				compositions::GuiBoundsComposition*			textComposition;
				compositions::GuiBoundsComposition*			mainComposition;

				static Win8CheckedButtonElements			Create(elements::ElementShape::Type shape, bool backgroundVisible);
				void										Apply(const Win8ButtonColors& colors);
			};

/***********************************************************************
Helper Functions
***********************************************************************/
			
			extern Color									Win8GetSystemWindowColor();
			extern Color									Win8GetSystemTextColor(bool enabled);
			extern void										Win8SetFont(elements::GuiSolidLabelElement* element, compositions::GuiBoundsComposition* composition, const FontProperties& fontProperties);
			extern void										Win8CreateSolidLabelElement(elements::GuiSolidLabelElement*& element, compositions::GuiBoundsComposition*& composition, Alignment::Type horizontal, Alignment::Type vertical);
		}
	}
}

#endif