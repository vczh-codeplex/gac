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
				Color										backgroundColor;
				Color										textColor;
				Color										bulletLight;
				Color										bulletDark;

				bool operator==(const Win8ButtonColors& colors)
				{
					return
						borderColor == colors.borderColor &&
						backgroundColor == colors.backgroundColor &&
						textColor == colors.textColor &&
						bulletLight == colors.bulletLight &&
						bulletDark == colors.bulletDark;
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
			};

			struct Win8ButtonElements
			{
				elements::GuiSolidBorderElement*			rectBorderElement;
				elements::GuiSolidBackgroundElement*		backgroundElement;
				elements::GuiSolidLabelElement*				textElement;
				compositions::GuiBoundsComposition*			textComposition;
				compositions::GuiBoundsComposition*			mainComposition;
				compositions::GuiBoundsComposition*			backgroundComposition;

				static Win8ButtonElements					Create(Alignment::Type horizontal=Alignment::Center, Alignment::Type vertical=Alignment::Center);
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