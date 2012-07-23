/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GacUI::Control Styles::Windows7 Styles

Clases:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_WIN7STYLES_GUIWIN7MENUSTYLES
#define VCZH_PRESENTATION_CONTROLS_WIN7STYLES_GUIWIN7MENUSTYLES

#include "GuiWin7StylesCommon.h"

namespace vl
{
	namespace presentation
	{
		namespace win7
		{

/***********************************************************************
Menu Container
***********************************************************************/
			
			/// <summary>Menu style (Windows 7). For the background of a popup menu.</summary>
			class Win7MenuStyle : public Object, public virtual controls::GuiControl::IStyleController, public Description<Win7MenuStyle>
			{
			protected:
				compositions::GuiBoundsComposition*			boundsComposition;
				compositions::GuiBoundsComposition*			containerComposition;
			public:
				/// <summary>Create the style.</summary>
				Win7MenuStyle();
				~Win7MenuStyle();

				compositions::GuiBoundsComposition*			GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*		GetContainerComposition()override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
			};
			
			/// <summary>Menu bar style (Windows 7). For the background of a menu bar.</summary>
			class Win7MenuBarStyle : public Object, public virtual controls::GuiControl::IStyleController, public Description<Win7MenuBarStyle>
			{
			protected:
				compositions::GuiBoundsComposition*			boundsComposition;
			public:
				/// <summary>Create the style.</summary>
				Win7MenuBarStyle();
				~Win7MenuBarStyle();

				compositions::GuiBoundsComposition*			GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*		GetContainerComposition()override;
				void										SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
			};

/***********************************************************************
Menu Button
***********************************************************************/
			
			/// <summary>Menu bar button style (Windows 7). For menu buttons in a menu bar.</summary>
			class Win7MenuBarButtonStyle : public Object, public virtual controls::GuiMenuButton::IStyleController, public Description<Win7MenuBarButtonStyle>
			{
			protected:
				Win7ButtonElements							elements;
				controls::GuiButton::ControlState			controlStyle;
				bool										isVisuallyEnabled;
				bool										isOpening;

				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool opening);
			public:
				/// <summary>Create the style.</summary>
				Win7MenuBarButtonStyle();
				~Win7MenuBarButtonStyle();

				compositions::GuiBoundsComposition*							GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*						GetContainerComposition()override;
				void														SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void														SetText(const WString& value)override;
				void														SetFont(const FontProperties& value)override;
				void														SetVisuallyEnabled(bool value)override;
				controls::GuiMenu::IStyleController*						CreateSubMenuStyleController()override;
				void														SetSubMenuExisting(bool value)override;
				void														SetSubMenuOpening(bool value)override;
				controls::GuiButton*										GetSubMenuHost()override;
				void														SetImage(Ptr<controls::GuiImageData> value)override;
				void														SetShortcutText(const WString& value)override;
				compositions::GuiSubComponentMeasurer::IMeasuringSource*	GetMeasuringSource()override;
				void														Transfer(controls::GuiButton::ControlState value)override;
			};
			
			/// <summary>Menu item button style (Windows 7). For menu buttons in a popup menu.</summary>
			class Win7MenuItemButtonStyle : public Object, public virtual controls::GuiMenuButton::IStyleController, public Description<Win7MenuItemButtonStyle>
			{
			protected:
				class MeasuringSource : public compositions::GuiSubComponentMeasurer::MeasuringSource
				{
				protected:
					Win7MenuItemButtonStyle*				style;
				public:
					MeasuringSource(Win7MenuItemButtonStyle* _style);
					~MeasuringSource();

					void									SubComponentPreferredMinSizeUpdated()override;
				};

				Win7MenuItemButtonElements					elements;
				Ptr<MeasuringSource>						measuringSource;
				controls::GuiButton::ControlState			controlStyle;
				bool										isVisuallyEnabled;
				bool										isOpening;

				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool opening);
			public:
				/// <summary>Create the style.</summary>
				Win7MenuItemButtonStyle();
				~Win7MenuItemButtonStyle();

				compositions::GuiBoundsComposition*							GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*						GetContainerComposition()override;
				void														SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void														SetText(const WString& value)override;
				void														SetFont(const FontProperties& value)override;
				void														SetVisuallyEnabled(bool value)override;
				controls::GuiMenu::IStyleController*						CreateSubMenuStyleController()override;
				void														SetSubMenuExisting(bool value)override;
				void														SetSubMenuOpening(bool value)override;
				controls::GuiButton*										GetSubMenuHost()override;
				void														SetImage(Ptr<controls::GuiImageData> value)override;
				void														SetShortcutText(const WString& value)override;
				compositions::GuiSubComponentMeasurer::IMeasuringSource*	GetMeasuringSource()override;
				void														Transfer(controls::GuiButton::ControlState value)override;
			};
			
			/// <summary>Menu splitter style (Windows 7). For splitters in a popup menu.</summary>
			class Win7MenuSplitterStyle : public Object, public virtual controls::GuiControl::IStyleController, public Description<Win7MenuSplitterStyle>
			{
			protected:
				compositions::GuiBoundsComposition*			boundsComposition;
			public:
				/// <summary>Create the style.</summary>
				Win7MenuSplitterStyle();
				~Win7MenuSplitterStyle();

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