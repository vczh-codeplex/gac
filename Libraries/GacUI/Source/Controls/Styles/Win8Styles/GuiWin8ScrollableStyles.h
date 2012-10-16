/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control Styles::Windows7 Styles

Clases:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_WIN8STYLES_GUIWin8SCROLLABLESTYLES
#define VCZH_PRESENTATION_CONTROLS_WIN8STYLES_GUIWin8SCROLLABLESTYLES

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
				elements::GuiPolygonElement*				arrowElement;

				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool selected)override;
				void										AfterApplyColors(const Win8ButtonColors& colors)override;
			public:
				/// <summary>Create the style.</summary>
				Win8ScrollArrowButtonStyle(common_styles::CommonScrollStyle::Direction direction, bool increaseButton);
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

/***********************************************************************
TextBox
***********************************************************************/
			
			class Win8TextBoxBackground : public Object, public Description<Win8TextBoxBackground>
			{
			protected:
				DEFINE_TRANSFERRING_ANIMATION(Win8TextBoxColors, Win8TextBoxBackground)
					
				elements::GuiSolidBorderElement*			borderElement;
				elements::GuiSolidBackgroundElement*		backgroundElement;
				compositions::GuiGraphicsComposition*		focusableComposition;
				bool										isMouseEnter;
				bool										isFocused;
				bool										isVisuallyEnabled;
				Ptr<TransferringAnimation>					transferringAnimation;
				controls::GuiControl::IStyleController*		styleController;
				elements::GuiColorizedTextElement*			textElement;

				void										UpdateStyle();
				void										Apply(const Win8TextBoxColors& colors);

				void										OnBoundsMouseEnter(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnBoundsMouseLeave(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnBoundsGotFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnBoundsLostFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
			public:
				Win8TextBoxBackground();
				~Win8TextBoxBackground();
				
				void										AssociateStyleController(controls::GuiControl::IStyleController* controller);
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value);
				void										SetVisuallyEnabled(bool value);
				compositions::GuiGraphicsComposition*		InstallBackground(compositions::GuiBoundsComposition* boundsComposition);
				void										InitializeTextElement(elements::GuiColorizedTextElement* _textElement);
			};
			
			/// <summary>Multiline text box style (Windows 7).</summary>
			class Win8MultilineTextBoxProvider : public Win8ScrollViewProvider, public Description<Win8MultilineTextBoxProvider>
			{
			protected:
				Win8TextBoxBackground						background;
				controls::GuiControl::IStyleController*		styleController;
			public:
				/// <summary>Create the style.</summary>
				Win8MultilineTextBoxProvider();
				~Win8MultilineTextBoxProvider();
				
				void										AssociateStyleController(controls::GuiControl::IStyleController* controller)override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetVisuallyEnabled(bool value)override;
				compositions::GuiGraphicsComposition*		InstallBackground(compositions::GuiBoundsComposition* boundsComposition)override;
			};
			
			/// <summary>Singleline text box style (Windows 7).</summary>
			class Win8SinglelineTextBoxProvider : public Object, public virtual controls::GuiSinglelineTextBox::IStyleProvider, public Description<Win8SinglelineTextBoxProvider>
			{
			protected:
				Win8TextBoxBackground						background;
				controls::GuiControl::IStyleController*		styleController;
			public:
				/// <summary>Create the style.</summary>
				Win8SinglelineTextBoxProvider();
				~Win8SinglelineTextBoxProvider();

				void										AssociateStyleController(controls::GuiControl::IStyleController* controller)override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
				compositions::GuiGraphicsComposition*		InstallBackground(compositions::GuiBoundsComposition* boundsComposition)override;
			};
		}
	}
}

#endif