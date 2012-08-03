/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GacUI::Control Styles::Windows7 Styles

Clases:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_WIN7STYLES_GUIWIN7SCROLLABLESTYLES
#define VCZH_PRESENTATION_CONTROLS_WIN7STYLES_GUIWIN7SCROLLABLESTYLES

#include "GuiWin7StylesCommon.h"
#include "GuiWin7ButtonStyles.h"

namespace vl
{
	namespace presentation
	{
		namespace win7
		{

/***********************************************************************
Scroll
***********************************************************************/
			
			/// <summary>Scroll bar style (Windows 7).</summary>
			class Win7ScrollStyle : public common_styles::CommonScrollStyle, public Description<Win7ScrollStyle>
			{
			public:
				static const int							DefaultSize=18;
				static const int							ArrowSize=10;
			protected:
				controls::GuiButton::IStyleController*		CreateDecreaseButtonStyle(Direction direction);
				controls::GuiButton::IStyleController*		CreateIncreaseButtonStyle(Direction direction);
				controls::GuiButton::IStyleController*		CreateHandleButtonStyle(Direction direction);
				void										InstallBackground(compositions::GuiGraphicsComposition* boundsComposition, Direction direction)override;
			public:
				/// <summary>Create the style using a specified direction.</summary>
				/// <param name="_direction">The specified direction</param>
				Win7ScrollStyle(Direction _direction);
				~Win7ScrollStyle();
			};
			
			/// <summary>Tracker (slide bar) style (Windows 7).</summary>
			class Win7TrackStyle : public common_styles::CommonTrackStyle, public Description<Win7TrackStyle>
			{
			public:
				static const int							TrackThickness=4;
				static const int							TrackPadding=8;
				static const int							HandleLong=21;
				static const int							HandleShort=10;

			protected:
				controls::GuiButton::IStyleController*		CreateHandleButtonStyle(Direction direction)override;
				void										InstallBackground(compositions::GuiGraphicsComposition* boundsComposition, Direction direction)override;
				void										InstallTrack(compositions::GuiGraphicsComposition* trackComposition, Direction direction)override;
			public:
				/// <summary>Create the style using a specified direction.</summary>
				/// <param name="_direction">The specified direction</param>
				Win7TrackStyle(Direction _direction);
				~Win7TrackStyle();
			};

			/// <summary>Progress bar style (Windows 7).</summary>
			class Win7ProgressBarStyle : public Object, public virtual controls::GuiScroll::IStyleController, public Description<Win7ProgressBarStyle>
			{
			protected:
				int											totalSize;
				int											pageSize;
				int											position;
				compositions::GuiBoundsComposition*			boundsComposition;
				compositions::GuiBoundsComposition*			containerComposition;
				compositions::GuiPartialViewComposition*	progressComposition;

				void										UpdateProgressBar();
				void										FillProgressColors(compositions::GuiGraphicsComposition* parent, Color g1, Color g2, Color g3, Color g4, Color g5);
			public:
				Win7ProgressBarStyle();
				~Win7ProgressBarStyle();

				compositions::GuiBoundsComposition*			GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*		GetContainerComposition()override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
				void										SetCommandExecutor(controls::GuiScroll::ICommandExecutor* value)override;
				void										SetTotalSize(int value)override;
				void										SetPageSize(int value)override;
				void										SetPosition(int value)override;
			};

/***********************************************************************
ScrollView
***********************************************************************/
			
			/// <summary>Scroll view style (Windows 7).</summary>
			class Win7ScrollViewProvider : public Object, public virtual controls::GuiScrollView::IStyleProvider, public Description<Win7ScrollViewProvider>
			{
			public:
				/// <summary>Create the style.</summary>
				Win7ScrollViewProvider();
				~Win7ScrollViewProvider();

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
			
			class Win7TextBoxBackground : public Object, public Description<Win7TextBoxBackground>
			{
			protected:
				DEFINE_TRANSFERRING_ANIMATION(Win7TextBoxColors, Win7TextBoxBackground)
					
				elements::GuiRoundBorderElement*			borderElement;
				elements::GuiSolidBackgroundElement*		backgroundElement;
				compositions::GuiGraphicsComposition*		focusableComposition;
				bool										isMouseEnter;
				bool										isFocused;
				bool										isVisuallyEnabled;
				Ptr<TransferringAnimation>					transferringAnimation;
				controls::GuiControl::IStyleController*		styleController;
				elements::GuiColorizedTextElement*			textElement;

				void										UpdateStyle();
				void										Apply(const Win7TextBoxColors& colors);

				void										OnBoundsMouseEnter(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnBoundsMouseLeave(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnBoundsGotFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnBoundsLostFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
			public:
				Win7TextBoxBackground();
				~Win7TextBoxBackground();
				
				void										AssociateStyleController(controls::GuiControl::IStyleController* controller);
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value);
				void										SetVisuallyEnabled(bool value);
				compositions::GuiGraphicsComposition*		InstallBackground(compositions::GuiBoundsComposition* boundsComposition);
				void										InitializeTextElement(elements::GuiColorizedTextElement* _textElement);
			};
			
			/// <summary>Multiline text box style (Windows 7).</summary>
			class Win7MultilineTextBoxProvider : public Win7ScrollViewProvider, public Description<Win7MultilineTextBoxProvider>
			{
			protected:
				Win7TextBoxBackground						background;
				controls::GuiControl::IStyleController*		styleController;
			public:
				/// <summary>Create the style.</summary>
				Win7MultilineTextBoxProvider();
				~Win7MultilineTextBoxProvider();
				
				void										AssociateStyleController(controls::GuiControl::IStyleController* controller)override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetVisuallyEnabled(bool value)override;
				compositions::GuiGraphicsComposition*		InstallBackground(compositions::GuiBoundsComposition* boundsComposition)override;
			};
			
			/// <summary>Singleline text box style (Windows 7).</summary>
			class Win7SinglelineTextBoxProvider : public Object, public virtual controls::GuiSinglelineTextBox::IStyleProvider, public Description<Win7SinglelineTextBoxProvider>
			{
			protected:
				Win7TextBoxBackground						background;
				controls::GuiControl::IStyleController*		styleController;
			public:
				/// <summary>Create the style.</summary>
				Win7SinglelineTextBoxProvider();
				~Win7SinglelineTextBoxProvider();

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