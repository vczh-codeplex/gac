/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control Styles::Windows7 Styles

Clases:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_WIN7STYLES_GUIWIN7CONTROLSTYLES
#define VCZH_PRESENTATION_CONTROLS_WIN7STYLES_GUIWIN7CONTROLSTYLES

#include "GuiWin7StylesCommon.h"

namespace vl
{
	namespace presentation
	{
		namespace win7
		{

/***********************************************************************
Container
***********************************************************************/

			/// <summary>Empty style. Nothing but a color filled the whole control.</summary>
			class Win7EmptyStyle : public Object, public virtual controls::GuiControl::IStyleController, public Description<Win7EmptyStyle>
			{
			protected:
				compositions::GuiBoundsComposition*			boundsComposition;
			public:
				/// <summary>Create the style with a specified color.</summary>
				/// <param name="color">The specified color.</param>
				Win7EmptyStyle(Color color);
				~Win7EmptyStyle();

				compositions::GuiBoundsComposition*			GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*		GetContainerComposition()override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
			};

			/// <summary>Window style (Windows 7). Using the Windows 7 window background color to fill the whold control</summary>
			class Win7WindowStyle : public virtual controls::GuiWindow::IStyleController, public Description<Win7WindowStyle>
			{
			protected:
				compositions::GuiBoundsComposition*			boundsComposition;
			public:
				/// <summary>Create the style.</summary>
				Win7WindowStyle();
				~Win7WindowStyle();

				compositions::GuiBoundsComposition*			GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*		GetContainerComposition()override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
				void										InitializeNativeWindowProperties(controls::GuiWindow* window)override;
			};

			/// <summary>Label style. A label contains a text, and the label will automatically resize to fit the text.</summary>
			class Win7LabelStyle : public Object, public virtual controls::GuiLabel::IStyleController, public Description<Win7LabelStyle>
			{
			protected:
				compositions::GuiBoundsComposition*			boundsComposition;
				elements::GuiSolidLabelElement*				textElement;
			public:
				/// <summary>Create the style.</summary>
				Win7LabelStyle();
				~Win7LabelStyle();

				compositions::GuiBoundsComposition*			GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*		GetContainerComposition()override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
				Color										GetDefaultTextColor()override;
				void										SetTextColor(Color value)override;
			};
			
			/// <summary>Group box style (Windows 7).</summary>
			class Win7GroupBoxStyle : public Object, public virtual controls::GuiControl::IStyleController, public Description<Win7GroupBoxStyle>
			{
			protected:
				DEFINE_TRANSFERRING_ANIMATION(Color, Win7GroupBoxStyle)

				compositions::GuiBoundsComposition*			boundsComposition;
				compositions::GuiBoundsComposition*			sinkBorderComposition;
				compositions::GuiBoundsComposition*			raisedBorderComposition;
				compositions::GuiBoundsComposition*			textComposition;
				compositions::GuiBoundsComposition*			textBackgroundComposition;
				compositions::GuiBoundsComposition*			containerComposition;
				elements::GuiSolidLabelElement*				textElement;
				Ptr<TransferringAnimation>					transferringAnimation;

				void										SetMargins(int fontSize);
			public:
				/// <summary>Create the style.</summary>
				Win7GroupBoxStyle();
				~Win7GroupBoxStyle();

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