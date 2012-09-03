/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUIWINDOWCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUIWINDOWCONTROLS

#include "GuiBasicControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
Control Host
***********************************************************************/

			/// <summary>
			/// Represents a control that host by a <see cref="INativeWindow"/>.
			/// </summary>
			class GuiControlHost : public GuiControl, private INativeWindowListener, public Description<GuiControlHost>
			{
			protected:
				compositions::GuiGraphicsHost*			host;
				collections::List<GuiComponent*>		components;

			private:
				void									Enabled()override;
				void									Disabled()override;
				void									GotFocus()override;
				void									LostFocus()override;
				void									Activated()override;
				void									Deactivated()override;
				void									Opened()override;
				void									Closing(bool& cancel)override;
				void									Closed()override;
				void									Destroying()override;
			public:
				/// <summary>Create a control with a specified style controller.</summary>
				/// <param name="_styleController">The style controller.</param>
				GuiControlHost(GuiControl::IStyleController* _styleController);
				~GuiControlHost();
				
				/// <summary>Window got focus event.</summary>
				compositions::GuiNotifyEvent			WindowGotFocus;
				/// <summary>Window lost focus event.</summary>
				compositions::GuiNotifyEvent			WindowLostFocus;
				/// <summary>Window activated event.</summary>
				compositions::GuiNotifyEvent			WindowActivated;
				/// <summary>Window deactivated event.</summary>
				compositions::GuiNotifyEvent			WindowDeactivated;
				/// <summary>Window opened event.</summary>
				compositions::GuiNotifyEvent			WindowOpened;
				/// <summary>Window closing event.</summary>
				compositions::GuiRequestEvent			WindowClosing;
				/// <summary>Window closed event.</summary>
				compositions::GuiNotifyEvent			WindowClosed;
				/// <summary>Window destroying event.</summary>
				compositions::GuiNotifyEvent			WindowDestroying;

				/// <summary>Get the internal <see cref="compositions::GuiGraphicsHost"/> object to host the window content.</summary>
				/// <returns>The internal <see cref="compositions::GuiGraphicsHost"/> object to host the window content.</returns>
				compositions::GuiGraphicsHost*			GetGraphicsHost();
				/// <summary>Get the main composition to host the window content.</summary>
				/// <returns>The main composition to host the window content.</returns>
				compositions::GuiGraphicsComposition*	GetMainComposition();
				/// <summary>Get the internal <see cref="INativeWindow"/> object to host the content.</summary>
				/// <returns>The the internal <see cref="INativeWindow"/> object to host the content.</returns>
				INativeWindow*							GetNativeWindow();
				/// <summary>Set the internal <see cref="INativeWindow"/> object to host the content.</summary>
				/// <param name="window">The the internal <see cref="INativeWindow"/> object to host the content.</param>
				void									SetNativeWindow(INativeWindow* window);
				/// <summary>Force to calculate layout and size immediately</summary>
				void									ForceCalculateSizeImmediately();
				/// <summary>Render the window content.</summary>
				void									Render();
				
				/// <summary>Test is the window enabled.</summary>
				/// <returns>Returns true if the window is enabled.</returns>
				bool									GetEnabled()override;
				/// <summary>Enable or disable the window.</summary>
				/// <param name="value">Set to true to enable the window.</param>
				void									SetEnabled(bool value)override;
				/// <summary>Test is the window focused.</summary>
				/// <returns>Returns true if the window is focused.</returns>
				bool									GetFocused();
				/// <summary>Focus the window.</summary>
				void									SetFocused();
				/// <summary>Test is the window activated.</summary>
				/// <returns>Returns true if the window is activated.</returns>
				bool									GetActivated();
				/// <summary>Activate the window.</summary>
				void									SetActivated();
				/// <summary>Test is the window icon shown in the task bar.</summary>
				/// <returns>Returns true if the window is icon shown in the task bar.</returns>
				bool									GetShowInTaskBar();
				/// <summary>Show or hide the window icon in the task bar.</summary>
				/// <param name="value">Set to true to show the window icon in the task bar.</param>
				void									SetShowInTaskBar(bool value);
				/// <summary>Test is the window allowed to be activated.</summary>
				/// <returns>Returns true if the window is allowed to be activated.</returns>
				bool									GetEnabledActivate();
				/// <summary>Allow or forbid the window to be activated.</summary>
				/// <param name="value">Set to true to allow the window to be activated.</param>
				void									SetEnabledActivate(bool value);
				
				/// <summary>
				/// Test is the maximize box visible.
				/// </summary>
				/// <returns>Returns true if the maximize box is visible.</returns>
				bool									GetMaximizedBox();
				/// <summary>
				/// Make the maximize box visible or invisible.
				/// </summary>
				/// <param name="visible">True to make the maximize box visible.</param>
				void									SetMaximizedBox(bool visible);
				/// <summary>
				/// Test is the minimize box visible.
				/// </summary>
				/// <returns>Returns true if the minimize box is visible.</returns>
				bool									GetMinimizedBox();
				/// <summary>
				/// Make the minimize box visible or invisible.
				/// </summary>
				/// <param name="visible">True to make the minimize box visible.</param>
				void									SetMinimizedBox(bool visible);
				/// <summary>
				/// Test is the border visible.
				/// </summary>
				/// <returns>Returns true if the border is visible.</returns>
				bool									GetBorder();
				/// <summary>
				/// Make the border visible or invisible.
				/// </summary>
				/// <param name="visible">True to make the border visible.</param>
				void									SetBorder(bool visible);
				/// <summary>
				/// Test is the size box visible.
				/// </summary>
				/// <returns>Returns true if the size box is visible.</returns>
				bool									GetSizeBox();
				/// <summary>
				/// Make the size box visible or invisible.
				/// </summary>
				/// <param name="visible">True to make the size box visible.</param>
				void									SetSizeBox(bool visible);
				/// <summary>
				/// Test is the icon visible.
				/// </summary>
				/// <returns>Returns true if the icon is visible.</returns>
				bool									GetIconVisible();
				/// <summary>
				/// Make the icon visible or invisible.
				/// </summary>
				/// <param name="visible">True to make the icon visible.</param>
				void									SetIconVisible(bool visible);
				/// <summary>
				/// Test is the title bar visible.
				/// </summary>
				/// <returns>Returns true if the title bar is visible.</returns>
				bool									GetTitleBar();
				/// <summary>
				/// Make the title bar visible or invisible.
				/// </summary>
				/// <param name="visible">True to make the title bar visible.</param>
				void									SetTitleBar(bool visible);
				/// <summary>
				/// Test is the window always on top of the desktop.
				/// </summary>
				/// <returns>Returns true if the window is always on top of the desktop.</returns>
				bool									GetTopMost();
				/// <summary>
				/// Make the window always or never on top of the desktop.
				/// </summary>
				/// <param name="topmost">True to make the window always  on top of the desktop.</param>
				void									SetTopMost(bool topmost);

				/// <summary>Add a component. When this control host is disposing, all attached components will be deleted.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				bool									AddComponent(GuiComponent* component);
				/// <summary>Remove a component.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				bool									RemoveComponent(GuiComponent* component);
				/// <summary>Test does the window contain the component.</summary>
				/// <returns>Returns true if the window contains the component.</returns>
				/// <param name="component">The component to test.</param>
				bool									ContainsComponent(GuiComponent* component);
				/// <summary>Get the <see cref="compositions::IGuiShortcutKeyManager"/> attached with this control host.</summary>
				/// <returns>The shortcut key manager.</returns>
				compositions::IGuiShortcutKeyManager*	GetShortcutKeyManager();
				/// <summary>Attach or detach the <see cref="compositions::IGuiShortcutKeyManager"/> associated with this control host. When this control host is disposing, the associated shortcut key manager will be deleted if exists.</summary>
				/// <param name="value">The shortcut key manager. Set to null to detach the previous shortcut key manager from this control host.</param>
				void									SetShortcutKeyManager(compositions::IGuiShortcutKeyManager* value);

				/// <summary>Get the client size of the window.</summary>
				/// <returns>The client size of the window.</returns>
				Size									GetClientSize();
				/// <summary>Set the client size of the window.</summary>
				/// <param name="value">The client size of the window.</param>
				void									SetClientSize(Size value);
				/// <summary>Get the bounds of the window in screen space.</summary>
				/// <returns>The bounds of the window.</returns>
				Rect									GetBounds();
				/// <summary>Set the bounds of the window in screen space.</summary>
				/// <param name="value">The bounds of the window.</param>
				void									SetBounds(Rect value);
				GuiControlHost*							GetRelatedControlHost()override;
				const WString&							GetText()override;
				void									SetText(const WString& value)override;

				/// <summary>Get the screen that contains the window.</summary>
				/// <returns>The screen that contains the window.</returns>
				INativeScreen*							GetRelatedScreen();
				/// <summary>
				/// Show the window.
				/// </summary>
				void									Show();
				/// <summary>
				/// Show the window without activation.
				/// </summary>
				void									ShowDeactivated();
				/// <summary>
				/// Restore the window.
				/// </summary>
				void									ShowRestored();
				/// <summary>
				/// Maximize the window.
				/// </summary>
				void									ShowMaximized();
				/// <summary>
				/// Minimize the window.
				/// </summary>
				void									ShowMinimized();
				/// <summary>
				/// Hide the window.
				/// </summary>
				void									Hide();
				/// <summary>
				/// Close the window and destroy the internal <see cref="INativeWindow"/> object.
				/// </summary>
				void									Close();
				/// <summary>Test is the window opened.</summary>
				/// <returns>Returns true if the window is opened.</returns>
				bool									GetOpening();
			};

/***********************************************************************
Window
***********************************************************************/

			/// <summary>
			/// Represents a normal window.
			/// </summary>
			class GuiWindow : public GuiControlHost, public Description<GuiWindow>
			{
				friend class GuiApplication;
			protected:
				virtual void							MouseClickedOnOtherWindow(GuiWindow* window);
			public:
				/// <summary>Style controller interface for <see cref="GuiWindow"/>.</summary>
				class IStyleController : virtual public GuiControl::IStyleController, public Description<IStyleController>
				{
				public:
					/// <summary>Initialize visual properties of the window. This callback is for some window template that don't need the standard window border.</summary>
					/// <param name="window">The window to set visual properties.</param>
					virtual void						InitializeNativeWindowProperties(GuiWindow* window)=0;
				};
			public:
				/// <summary>Create a control with a specified style controller.</summary>
				/// <param name="_styleController">The style controller.</param>
				GuiWindow(IStyleController* _styleController);
				~GuiWindow();

				/// <summary>Clipboard updated event.</summary>
				compositions::GuiNotifyEvent			ClipboardUpdated;

				/// <summary>Move the window to the center of the screen. If multiple screens exist, the window move to the screen that contains the biggest part of the window.</summary>
				void									MoveToScreenCenter();
			};
			
			/// <summary>
			/// Represents a popup window. When the mouse click on other window or the desktop, the popup window will be closed automatically.
			/// </summary>
			class GuiPopup : public GuiWindow, public Description<GuiPopup>
			{
			protected:
				void									MouseClickedOnOtherWindow(GuiWindow* window)override;

				void									PopupOpened(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void									PopupClosed(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
			public:
				/// <summary>Create a control with a specified style controller.</summary>
				/// <param name="_styleController">The style controller.</param>
				GuiPopup(IStyleController* _styleController);
				~GuiPopup();

				/// <summary>Test will the whole popup window be in the screen if the popup's left-top position is set to a specified value.</summary>
				/// <returns>Returns true if the whole popup window will be in the screen.</returns>
				/// <param name="location">The specified left-top position.</param>
				bool									IsClippedByScreen(Point location);
				/// <summary>Show the popup window with the left-top position set to a specified value. The position of the popup window will be adjusted to make it totally inside the screen if possible.</summary>
				/// <param name="location">The specified left-top position.</param>
				void									ShowPopup(Point location);
				/// <summary>Show the popup window aligned with a specified control. The position of the popup window will be adjusted to make it totally inside the screen if possible.</summary>
				/// <param name="preferredTopBottomSide">Set to true if the popup window is expected to be opened at the top or bottom side of that control.</param>
				void									ShowPopup(GuiControl* control, bool preferredTopBottomSide);
			};
		}
	}
}

#endif