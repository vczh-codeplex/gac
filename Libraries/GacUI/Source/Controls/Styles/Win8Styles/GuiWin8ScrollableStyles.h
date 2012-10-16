/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control Styles::Windows7 Styles

Clases:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_WIN8STYLES_GUIWIN7SCROLLABLESTYLES
#define VCZH_PRESENTATION_CONTROLS_WIN8STYLES_GUIWIN7SCROLLABLESTYLES

#include "GuiWin8StylesCommon.h"
#include "GuiWin8ButtonStyles.h"

namespace vl
{
	namespace presentation
	{
		namespace win8
		{

/***********************************************************************
Scroll
***********************************************************************/
			
			/// <summary>Scroll bar handle button style (Windows 8).</summary>
			class Win8ScrollHandleButtonStyle : public Win8ButtonStyleBase, public Description<Win8ScrollHandleButtonStyle>
			{
			protected:
				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool selected)override;
			public:
				/// <summary>Create the style.</summary>
				Win8ScrollHandleButtonStyle();
				~Win8ScrollHandleButtonStyle();
			};
			
			/// <summary>Scroll bar arrow button style (Windows 8).</summary>
			class Win8ScrollArrowButtonStyle : public Win8ButtonStyleBase, public Description<Win8ScrollArrowButtonStyle>
			{
			protected:
				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool selected)override;
			public:
				/// <summary>Create the style.</summary>
				Win8ScrollArrowButtonStyle();
				~Win8ScrollArrowButtonStyle();
			};
			
			/// <summary>Scroll bar style (Windows 8).</summary>
			class Win8ScrollStyle : public common_styles::CommonScrollStyle, public Description<Win8ScrollStyle>
			{
			public:
				static const int							DefaultSize=15;
				static const int							ArrowSize=8;
			protected:
				controls::GuiButton::IStyleController*		CreateDecreaseButtonStyle(Direction direction)override;
				controls::GuiButton::IStyleController*		CreateIncreaseButtonStyle(Direction direction)override;
				controls::GuiButton::IStyleController*		CreateHandleButtonStyle(Direction direction)override;
				compositions::GuiBoundsComposition*			InstallBackground(compositions::GuiBoundsComposition* boundsComposition, Direction direction)override;
			public:
				/// <summary>Create the style using a specified direction.</summary>
				/// <param name="_direction">The specified direction</param>
				Win8ScrollStyle(Direction _direction);
				~Win8ScrollStyle();
			};

/***********************************************************************
ScrollView
***********************************************************************/
			
			/// <summary>Scroll view style (Windows 8).</summary>
			class Win8ScrollViewProvider : public Object, public virtual controls::GuiScrollView::IStyleProvider, public Description<Win8ScrollViewProvider>
			{
			public:
				/// <summary>Create the style.</summary>
				Win8ScrollViewProvider();
				~Win8ScrollViewProvider();

				void										AssociateStyleController(controls::GuiControl::IStyleController* controller)override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;

				controls::GuiScroll::IStyleController*		CreateHorizontalScrollStyle()override;
				controls::GuiScroll::IStyleController*		CreateVerticalScrollStyle()override;
				int											GetDefaultScrollSize()override;
				compositions::GuiGraphicsComposition*		InstallBackground(compositions::GuiBoundsComposition* boundsComposition)override;
			};
		}
	}
}

#endif