/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control Styles::Windows7 Styles

Clases:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_WIN7STYLES_GUIWIN8BUTTONSTYLES
#define VCZH_PRESENTATION_CONTROLS_WIN7STYLES_GUIWIN8BUTTONSTYLES

#include "GuiWin8StylesCommon.h"

namespace vl
{
	namespace presentation
	{
		namespace win8
		{

/***********************************************************************
Button
***********************************************************************/
			
			/// <summary>The base class of all button style implementations. (Windows 8)</summary>
			class Win8ButtonStyleBase : public Object, public virtual controls::GuiSelectableButton::IStyleController, public Description<Win8ButtonStyleBase>
			{
			protected:
				DEFINE_TRANSFERRING_ANIMATION(Win8ButtonColors, Win8ButtonStyleBase)

				Win8ButtonElements							elements;
				Ptr<TransferringAnimation>					transferringAnimation;
				controls::GuiButton::ControlState			controlStyle;
				bool										isVisuallyEnabled;
				bool										isSelected;
				bool										transparentWhenInactive;
				bool										transparentWhenDisabled;

				virtual void								TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool selected)=0;
			public:
				/// <summary>Create the style.</summary>
				/// <param name="initialColor">Set to true to set the initial colors of all components for a button.</param>
				/// <param name="horizontal">Horizontal alignment for text.</param>
				/// <param name="vertical">Vertical alignment for text.</param>
				Win8ButtonStyleBase(const Win8ButtonColors& initialColor, Alignment::Type horizontal, Alignment::Type vertical);
				~Win8ButtonStyleBase();

				compositions::GuiBoundsComposition*			GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*		GetContainerComposition()override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
				void										SetSelected(bool value)override;
				void										Transfer(controls::GuiButton::ControlState value)override;

				/// <summary>Get the transparent style for the inactive state.</summary>
				/// <returns>Returns true if the background is not transparent for the inactive state.</returns>
				bool										GetTransparentWhenInactive();
				/// <summary>Set the transparent style for the inactive state.</summary>
				/// <param name="value">Set to troe true to make the background not transparent for the inactive state.</param>
				void										SetTransparentWhenInactive(bool value);
				/// <summary>Get the transparent style for the disabled state.</summary>
				/// <returns>Returns true if the background is not transparent for the disabled state.</returns>
				bool										GetTransparentWhenDisabled();
				/// <summary>Set the transparent style for the inactive state.</summary>
				/// <param name="value">Set to troe true to make the background not transparent for the disabled state.</param>
				void										SetTransparentWhenDisabled(bool value);
				/// <summary>Get the automatically size changing state of the button.</summary>
				/// <returns>Returns true if the style automatically changes its size if the text changed.</returns>
				bool										GetAutoSizeForText();
				/// <summary>Set the automatically size changing state of the button.</summary>
				/// <param name="value">Set to true to make the style automatically changes its size if the text changed.</param>
				void										SetAutoSizeForText(bool value);
			};
			
			/// <summary>Button style (Windows 8).</summary>
			class Win8ButtonStyle : public Win8ButtonStyleBase, public Description<Win8ButtonStyle>
			{
			protected:
				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool selected)override;
			public:
				/// <summary>Create the style.</summary>
				Win8ButtonStyle();
				~Win8ButtonStyle();
			};
		}
	}
}

#endif