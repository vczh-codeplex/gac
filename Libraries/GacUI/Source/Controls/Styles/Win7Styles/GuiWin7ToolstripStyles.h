/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control Styles::Windows7 Styles

Clases:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_WIN7STYLES_GUIWIN7TOOLSTRIPSTYLES
#define VCZH_PRESENTATION_CONTROLS_WIN7STYLES_GUIWIN7TOOLSTRIPSTYLES

#include "GuiWin7MenuStyles.h"

namespace vl
{
	namespace presentation
	{
		namespace win7
		{

/***********************************************************************
Toolstrip Button
***********************************************************************/

			/// <summary>Toolstrip button style (Windows 7).</summary>
			class Win7ToolstripButtonStyle : public Object, public virtual controls::GuiMenuButton::IStyleController, public Description<Win7ToolstripButtonStyle>
			{
			public:
				enum ButtonStyle
				{
					CommandButton,
					DropdownButton,
					SplitButton,
				};
			protected:
				DEFINE_TRANSFERRING_ANIMATION(Win7ButtonColors, Win7ToolstripButtonStyle)

				Win7ButtonElements							elements;
				Ptr<TransferringAnimation>					transferringAnimation;
				controls::GuiButton::ControlState			controlStyle;
				bool										isVisuallyEnabled;
				bool										isOpening;
				elements::GuiImageFrameElement*				imageElement;
				compositions::GuiBoundsComposition*			imageComposition;
				ButtonStyle									buttonStyle;

				virtual void								TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool menuOpening);
			public:
				/// <summary>Create the style.</summary>
				/// <param name="transparent">Set to true to make the background disappear when the button is not in an active state.</param>
				Win7ToolstripButtonStyle(bool transparent, ButtonStyle _buttonStyle);
				~Win7ToolstripButtonStyle();
				
				compositions::GuiBoundsComposition*			GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*		GetContainerComposition()override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
				controls::GuiMenu::IStyleController*		CreateSubMenuStyleController()override;
				void										SetSubMenuExisting(bool value)override;
				void										SetSubMenuOpening(bool value)override;
				controls::GuiButton*						GetSubMenuHost()override;
				void										SetImage(Ptr<controls::GuiImageData> value)override;
				void										Transfer(controls::GuiButton::ControlState value)override;
			};

			/// <summary>Toolstrip splitter style (Windows 7).</summary>
			class Win7ToolstripSplitterStyle : public Object, public virtual controls::GuiControl::IStyleController, public Description<Win7ToolstripSplitterStyle>
			{
			protected:
				compositions::GuiBoundsComposition*			boundsComposition;
			public:
				/// <summary>Create the style.</summary>
				Win7ToolstripSplitterStyle();
				~Win7ToolstripSplitterStyle();

				compositions::GuiBoundsComposition*			GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*		GetContainerComposition()override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
			};
		}
	}
}

#endif