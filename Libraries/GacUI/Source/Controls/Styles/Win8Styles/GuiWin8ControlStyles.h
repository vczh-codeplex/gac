/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control Styles::Windows8 Styles

Clases:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_WIN8STYLES_GUIWin8CONTROLSTYLES
#define VCZH_PRESENTATION_CONTROLS_WIN8STYLES_GUIWin8CONTROLSTYLES

#include "GuiWin8StylesCommon.h"

namespace vl
{
	namespace presentation
	{
		namespace win8
		{

/***********************************************************************
Container
***********************************************************************/

			/// <summary>Empty style. Nothing but a color filled the whole control.</summary>
			class Win8EmptyStyle : public Object, public virtual controls::GuiControl::IStyleController, public Description<Win8EmptyStyle>
			{
			protected:
				compositions::GuiBoundsComposition*			boundsComposition;
			public:
				/// <summary>Create the style with a specified color.</summary>
				/// <param name="color">The specified color.</param>
				Win8EmptyStyle(Color color);
				~Win8EmptyStyle();

				compositions::GuiBoundsComposition*			GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*		GetContainerComposition()override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
			};

			/// <summary>Window style (Windows 7). Using the Windows 7 window background color to fill the whold control</summary>
			class Win8WindowStyle : public virtual controls::GuiWindow::DefaultBehaviorStyleController, public Description<Win8WindowStyle>
			{
			protected:
				compositions::GuiBoundsComposition*			boundsComposition;
			public:
				/// <summary>Create the style.</summary>
				Win8WindowStyle();
				~Win8WindowStyle();

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