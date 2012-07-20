/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control Styles::Windows7 Styles

Clases:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_WIN7STYLES_GUIWIN7STYLESCOMMON
#define VCZH_PRESENTATION_CONTROLS_WIN7STYLES_GUIWIN7STYLESCOMMON

#include "..\GuiCommonStyles.h"

namespace vl
{
	namespace presentation
	{
		namespace win7
		{

/***********************************************************************
Button Configuration
***********************************************************************/
			
			struct Win7ButtonColors
			{
				Color										borderColor;
				Color										backgroundColor;
				Color										g1;
				Color										g2;
				Color										g3;
				Color										g4;
				Color										textColor;
				Color										bulletLight;
				Color										bulletDark;

				bool operator==(const Win7ButtonColors& colors)
				{
					return
						borderColor == colors.borderColor &&
						backgroundColor == colors.backgroundColor &&
						g1 == colors.g1 &&
						g2 == colors.g2 &&
						g3 == colors.g3 &&
						g4 == colors.g4 &&
						textColor == colors.textColor &&
						bulletLight == colors.bulletLight &&
						bulletDark == colors.bulletDark;
				}

				bool operator!=(const Win7ButtonColors& colors)
				{
					return !(*this==colors);
				}

				void										SetAlphaWithoutText(unsigned char a);

				static Win7ButtonColors						Blend(const Win7ButtonColors& c1, const Win7ButtonColors& c2, int ratio, int total);

				static Win7ButtonColors						ButtonNormal();
				static Win7ButtonColors						ButtonActive();
				static Win7ButtonColors						ButtonPressed();
				static Win7ButtonColors						ButtonDisabled();
				
				static Win7ButtonColors						ItemNormal();
				static Win7ButtonColors						ItemActive();
				static Win7ButtonColors						ItemSelected();
				static Win7ButtonColors						ItemDisabled();
				
				static Win7ButtonColors						CheckedNormal(bool selected);
				static Win7ButtonColors						CheckedActive(bool selected);
				static Win7ButtonColors						CheckedPressed(bool selected);
				static Win7ButtonColors						CheckedDisabled(bool selected);

				static Win7ButtonColors						ToolstripButtonNormal();
				static Win7ButtonColors						ToolstripButtonActive();
				static Win7ButtonColors						ToolstripButtonPressed();
				static Win7ButtonColors						ToolstripButtonDisabled();

				static Win7ButtonColors						MenuBarButtonNormal();
				static Win7ButtonColors						MenuBarButtonActive();
				static Win7ButtonColors						MenuBarButtonPressed();
				static Win7ButtonColors						MenuBarButtonDisabled();

				static Win7ButtonColors						MenuItemButtonNormal();
				static Win7ButtonColors						MenuItemButtonNormalActive();
				static Win7ButtonColors						MenuItemButtonDisabled();
				static Win7ButtonColors						MenuItemButtonDisabledActive();

				static Win7ButtonColors						TabPageHeaderNormal();
				static Win7ButtonColors						TabPageHeaderActive();
				static Win7ButtonColors						TabPageHeaderSelected();
			};

			struct Win7ButtonElements
			{
				elements::GuiSolidBorderElement*			rectBorderElement;
				elements::GuiRoundBorderElement*			roundBorderElement;
				elements::GuiSolidBackgroundElement*		backgroundElement;
				elements::GuiGradientBackgroundElement*		topGradientElement;
				elements::GuiGradientBackgroundElement*		bottomGradientElement;
				elements::GuiSolidLabelElement*				textElement;
				compositions::GuiBoundsComposition*			textComposition;
				compositions::GuiBoundsComposition*			mainComposition;
				compositions::GuiBoundsComposition*			backgroundComposition;
				compositions::GuiTableComposition*			gradientComposition;

				static Win7ButtonElements					Create(bool verticalGradient, bool roundBorder, Alignment::Type horizontal=Alignment::Center, Alignment::Type vertical=Alignment::Center);
				void										Apply(const Win7ButtonColors& colors);
			};

			struct Win7CheckedButtonElements
			{
				elements::GuiSolidBorderElement*			borderElement;
				elements::GuiSolidBackgroundElement*		backgroundElement;
				elements::GuiGradientBackgroundElement*		outerGradientElement;
				elements::GuiGradientBackgroundElement*		innerGradientElement;
				elements::GuiSolidLabelElement*				textElement;
				elements::GuiSolidLabelElement*				bulletTextElement;
				elements::GuiSolidBackgroundElement*		bulletBackgroundElement;
				compositions::GuiBoundsComposition*			textComposition;
				compositions::GuiBoundsComposition*			mainComposition;

				static Win7CheckedButtonElements			Create(elements::ElementShape::Type shape, bool backgroundVisible);
				void										Apply(const Win7ButtonColors& colors);
			};

			struct Win7MenuItemButtonElements
			{
				elements::GuiRoundBorderElement*			borderElement;
				elements::GuiSolidBackgroundElement*		backgroundElement;
				elements::GuiGradientBackgroundElement*		gradientElement;
				elements::Gui3DSplitterElement*				splitterElement;
				compositions::GuiCellComposition*			splitterComposition;
				elements::GuiImageFrameElement*				imageElement;
				elements::GuiSolidLabelElement*				textElement;
				compositions::GuiBoundsComposition*			textComposition;
				elements::GuiPolygonElement*				subMenuArrowElement;
				compositions::GuiGraphicsComposition*		subMenuArrowComposition;
				compositions::GuiBoundsComposition*			mainComposition;

				static Win7MenuItemButtonElements			Create();
				void										Apply(const Win7ButtonColors& colors);
				void										SetActive(bool value);
				void										SetSubMenuExisting(bool value);
			};

			struct Win7TextBoxColors
			{
				Color										borderColor;
				Color										backgroundColor;

				bool operator==(const Win7TextBoxColors& colors)
				{
					return
						borderColor == colors.borderColor &&
						backgroundColor == colors.backgroundColor;
				}

				bool operator!=(const Win7TextBoxColors& colors)
				{
					return !(*this==colors);
				}

				static Win7TextBoxColors					Blend(const Win7TextBoxColors& c1, const Win7TextBoxColors& c2, int ratio, int total);

				static Win7TextBoxColors					Normal();
				static Win7TextBoxColors					Active();
				static Win7TextBoxColors					Focused();
				static Win7TextBoxColors					Disabled();
			};

/***********************************************************************
Helper Functions
***********************************************************************/
			
			extern Color									BlendColor(Color c1, Color c2, int currentPosition, int totalLength);
			extern int										Win7GetColorAnimationLength();
			extern Color									Win7GetSystemWindowColor();
			extern Color									Win7GetSystemTabContentColor();
			extern Color									Win7GetSystemBorderColor();
			extern Color									Win7GetSystemBorderSinkColor();
			extern Color									Win7GetSystemBorderRaiseColor();
			extern Color									Win7GetSystemTextColor(bool enabled);
			extern void										Win7SetFont(elements::GuiSolidLabelElement* element, compositions::GuiBoundsComposition* composition, const FontProperties& fontProperties);
			extern void										Win7CreateSolidLabelElement(elements::GuiSolidLabelElement*& element, compositions::GuiBoundsComposition*& composition, Alignment::Type horizontal, Alignment::Type vertical);
			extern elements::text::ColorEntry				Win7GetTextBoxTextColor();

/***********************************************************************
Animation
***********************************************************************/

#define DEFINE_TRANSFERRING_ANIMATION(TSTATE, TSTYLECONTROLLER)\
				class TransferringAnimation : public compositions::GuiTimeBasedAnimation\
				{\
				protected:\
					TSTATE									colorBegin;\
					TSTATE									colorEnd;\
					TSTATE									colorCurrent;\
					TSTYLECONTROLLER*						style;\
					bool									stopped;\
					bool									disabled;\
					bool									enableAnimation;\
					void									PlayInternal(int currentPosition, int totalLength);\
				public:\
					TransferringAnimation(TSTYLECONTROLLER* _style, const TSTATE& begin);\
					void									Disable();\
					void									Play(int currentPosition, int totalLength)override;\
					void									Stop()override;\
					bool									GetEnableAnimation();\
					void									SetEnableAnimation(bool value);\
					void									Transfer(const TSTATE& end);\
				};\

/***********************************************************************
Animation Implementation
***********************************************************************/

#define DEFAULT_TRANSFERRING_ANIMATION_HOST_GETTER(STYLE) (STYLE->GetBoundsComposition()->GetRelatedGraphicsHost())

#define IMPLEMENT_TRANSFERRING_ANIMATION_BASE(TSTATE, TSTYLECONTROLLER, HOST_GETTER)\
			TSTYLECONTROLLER::TransferringAnimation::TransferringAnimation(TSTYLECONTROLLER* _style, const TSTATE& begin)\
				:GuiTimeBasedAnimation(0)\
				,colorBegin(begin)\
				,colorEnd(begin)\
				,colorCurrent(begin)\
				,style(_style)\
				,stopped(true)\
				,disabled(false)\
				,enableAnimation(true)\
			{\
			}\
			void TSTYLECONTROLLER::TransferringAnimation::Disable()\
			{\
				disabled=true;\
			}\
			void TSTYLECONTROLLER::TransferringAnimation::Play(int currentPosition, int totalLength)\
			{\
				if(!disabled)\
				{\
					PlayInternal(currentPosition, totalLength);\
				}\
			}\
			void TSTYLECONTROLLER::TransferringAnimation::Stop()\
			{\
				stopped=true;\
			}\
			bool TSTYLECONTROLLER::TransferringAnimation::GetEnableAnimation()\
			{\
				return enableAnimation;\
			}\
			void TSTYLECONTROLLER::TransferringAnimation::SetEnableAnimation(bool value)\
			{\
				enableAnimation=value;\
			}\
			void TSTYLECONTROLLER::TransferringAnimation::Transfer(const TSTATE& end)\
			{\
				if(colorEnd!=end)\
				{\
					GuiGraphicsHost* host=HOST_GETTER(style);\
					if(enableAnimation && host)\
					{\
						Restart(Win7GetColorAnimationLength());\
						if(stopped)\
						{\
							colorBegin=colorEnd;\
							colorEnd=end;\
							host->GetAnimationManager()->AddAnimation(style->transferringAnimation);\
							stopped=false;\
						}\
						else\
						{\
							colorBegin=colorCurrent;\
							colorEnd=end;\
						}\
					}\
					else\
					{\
						colorBegin=end;\
						colorEnd=end;\
						colorCurrent=end;\
						Play(1, 1);\
					}\
				}\
			}\
			void TSTYLECONTROLLER::TransferringAnimation::PlayInternal(int currentPosition, int totalLength)\

#define IMPLEMENT_TRANSFERRING_ANIMATION(TSTATE, TSTYLECONTROLLER)\
	IMPLEMENT_TRANSFERRING_ANIMATION_BASE(TSTATE, TSTYLECONTROLLER, DEFAULT_TRANSFERRING_ANIMATION_HOST_GETTER)

		}
	}
}

#endif