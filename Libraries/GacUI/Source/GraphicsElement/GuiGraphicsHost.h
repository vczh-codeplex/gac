/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Graphics Composition Host

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSHOST
#define VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSHOST

#include "..\GraphicsComposition\GuiGraphicsComposition.h"

namespace vl
{
	namespace presentation
	{
		namespace compositions
		{

/***********************************************************************
Animation
***********************************************************************/

			/// <summary>
			/// Represents an animation. Use [M:vl.presentation.compositions.GuiGraphicsHost.GetAnimationManager] to access this object.
			/// </summary>
			class IGuiGraphicsAnimation : public virtual IDescriptable, public Description<IGuiGraphicsAnimation>
			{
			public:
				/// <summary>Get the total length.</summary>
				/// <returns>The total length.</returns>
				virtual int						GetTotalLength()=0;
				/// <summary>Get the current position. Value in [0, TotalLength-1]. This function doesn't return a internal state. It return the suggested current position at the moment this function is called.</summary>
				/// <returns>The current position.</returns>
				virtual int						GetCurrentPosition()=0;
				/// <summary>Display a state in the animation with the specified current position and the specified total length.</summary>
				/// <param name="currentPosition">The current position. When this function is called by [T:vl.presentation.compositions.GuiGraphicsAnimationManager], this value comes from <see cref="IGuiGraphicsAnimation::GetCurrentPosition"/>.</param>
				/// <param name="totalLength">The total length. When this function is called by [T:vl.presentation.compositions.GuiGraphicsAnimationManager], this value comes from <see cref="IGuiGraphicsAnimation::GetTotalLength"/>.</param>
				virtual void					Play(int currentPosition, int totalLength)=0;
				/// <summary>Stop the animation.</summary>
				virtual void					Stop()=0;
			};

			/// <summary>
			/// Animation manager.
			/// </summary>
			class GuiGraphicsAnimationManager : public Object
			{
				typedef collections::List<Ptr<IGuiGraphicsAnimation>>		AnimationList;
			protected:
				AnimationList					playingAnimations;
			public:
				GuiGraphicsAnimationManager();
				~GuiGraphicsAnimationManager();

				/// <summary>Add a new animation.</summary>
				/// <param name="animation">The new animation to add.</param>
				void							AddAnimation(Ptr<IGuiGraphicsAnimation> animation);
				/// <summary>Test is the animation manager contains any alive animation.</summary>
				/// <returns>Returns true if the animation manager contains any alive animation.</returns>
				bool							HasAnimation();
				/// <summary>Play all alive animations. Any animation that comes to the end will be removed.</summary>
				void							Play();
			};

/***********************************************************************
Host
***********************************************************************/

			/// <summary>
			/// GuiGraphicsHost hosts an <see cref="GuiWindowComposition"/> in an <see cref="INativeWindow"/>. The composition will fill the whole window.
			/// </summary>
			class GuiGraphicsHost : public Object, private INativeWindowListener, private INativeControllerListener, public Description<GuiGraphicsHost>
			{
				typedef collections::List<GuiGraphicsComposition*>		CompositionList;
			public:
				static const unsigned __int64	CaretInterval=500;
			protected:
				INativeWindow*					nativeWindow;
				GuiWindowComposition*			windowComposition;
				GuiGraphicsComposition*			focusedComposition;
				Size							previousClientSize;
				Size							minSize;
				Point							caretPoint;
				unsigned __int64				lastCaretTime;

				GuiGraphicsAnimationManager		animationManager;
				GuiGraphicsComposition*			mouseCaptureComposition;
				CompositionList					mouseEnterCompositions;

				void							DisconnectCompositionInternal(GuiGraphicsComposition* composition);

				void							MouseCapture(const NativeWindowMouseInfo& info);
				void							MouseUncapture(const NativeWindowMouseInfo& info);
				void							OnCharInput(const NativeWindowCharInfo& info, GuiGraphicsComposition* composition, GuiCharEvent GuiGraphicsEventReceiver::* eventReceiverEvent);
				void							OnKeyInput(const NativeWindowKeyInfo& info, GuiGraphicsComposition* composition, GuiKeyEvent GuiGraphicsEventReceiver::* eventReceiverEvent);
				void							RaiseMouseEvent(GuiMouseEventArgs& arguments, GuiGraphicsComposition* composition, GuiMouseEvent GuiGraphicsEventReceiver::* eventReceiverEvent);
				void							OnMouseInput(const NativeWindowMouseInfo& info, GuiMouseEvent GuiGraphicsEventReceiver::* eventReceiverEvent);
				
			private:
				void							Moving(Rect& bounds, bool fixSizeOnly)override;
				void							Moved()override;

				void							LeftButtonDown(const NativeWindowMouseInfo& info)override;
				void							LeftButtonUp(const NativeWindowMouseInfo& info)override;
				void							LeftButtonDoubleClick(const NativeWindowMouseInfo& info)override;
				void							RightButtonDown(const NativeWindowMouseInfo& info)override;
				void							RightButtonUp(const NativeWindowMouseInfo& info)override;
				void							RightButtonDoubleClick(const NativeWindowMouseInfo& info)override;
				void							MiddleButtonDown(const NativeWindowMouseInfo& info)override;
				void							MiddleButtonUp(const NativeWindowMouseInfo& info)override;
				void							MiddleButtonDoubleClick(const NativeWindowMouseInfo& info)override;
				void							HorizontalWheel(const NativeWindowMouseInfo& info)override;
				void							VerticalWheel(const NativeWindowMouseInfo& info)override;
				void							MouseMoving(const NativeWindowMouseInfo& info)override;
				void							MouseEntered()override;
				void							MouseLeaved()override;

				void							KeyDown(const NativeWindowKeyInfo& info)override;
				void							KeyUp(const NativeWindowKeyInfo& info)override;
				void							SysKeyDown(const NativeWindowKeyInfo& info)override;
				void							SysKeyUp(const NativeWindowKeyInfo& info)override;
				void							Char(const NativeWindowCharInfo& info)override;

				void							GlobalTimer()override;
			public:
				GuiGraphicsHost();
				~GuiGraphicsHost();

				/// <summary>Get the associated window.</summary>
				/// <returns>The associated window.</returns>
				INativeWindow*					GetNativeWindow();
				/// <summary>Associate a window. A <see cref="GuiWindowComposition"/> will fill and appear in the window.</summary>
				/// <param name="_nativeWindow">The window to associated.</param>
				void							SetNativeWindow(INativeWindow* _nativeWindow);
				/// <summary>Get the main <see cref="GuiWindowComposition"/>. If a window is associated, everything that put into the main composition will be shown in the window.</summary>
				/// <returns>The main compositoin.</returns>
				GuiGraphicsComposition*			GetMainComposition();
				/// <summary>Render the main composition and all content to the associated window.</summary>
				void							Render();

				/// <summary>Set the focus composition. A focused composition will receive keyboard messages.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="composition">The composition to set focus. This composition should be or in the main composition.</param>
				bool							SetFocus(GuiGraphicsComposition* composition);
				/// <summary>Get the focus composition. A focused composition will receive keyboard messages.</summary>
				/// <returns>The focus composition.</returns>
				GuiGraphicsComposition*			GetFocusedComposition();
				/// <summary>Get the caret point. A caret point is the position to place the edit box of the activated input method editor.</summary>
				/// <returns>The caret point.</returns>
				Point							GetCaretPoint();
				/// <summary>Set the caret point. A caret point is the position to place the edit box of the activated input method editor.</summary>
				/// <param name="value">The caret point.</param>
				/// <param name="referenceComposition">The point space. If this argument is null, the "value" argument will use the point space of the client area in the main composition.</param>
				void							SetCaretPoint(Point value, GuiGraphicsComposition* referenceComposition=0);

				/// <summary>Get the animation manager.</summary>
				/// <returns>The animation manager.</returns>
				GuiGraphicsAnimationManager*	GetAnimationManager();
				/// <summary>Notify that a composition is going to disconnect from this graphics host. Generally this happens when a composition's parent line changes.</summary>
				/// <param name="composition">The composition to disconnect</param>
				void							DisconnectComposition(GuiGraphicsComposition* composition);
			};

/***********************************************************************
Helpers
***********************************************************************/
			
			/// <summary>
			/// Represents a time based animation.
			/// </summary>
			class GuiTimeBasedAnimation : public IGuiGraphicsAnimation, public Description<GuiTimeBasedAnimation>
			{
			protected:
				unsigned __int64				startTime;
				int								length;
			public:
				/// <summary>Create an animation with a specified length in milliseconds.</summary>
				/// <param name="totalMilliseconds">The specified length in milliseconds.</param>
				GuiTimeBasedAnimation(int totalMilliseconds);
				~GuiTimeBasedAnimation();

				/// <summary>Restart an animation with a specified length in milliseconds.</summary>
				/// <param name="totalMilliseconds">The specified length in milliseconds. If the value is -1, it will use the previous length.</param>
				void							Restart(int totalMilliseconds=-1);
				int								GetTotalLength()override;
				int								GetCurrentPosition()override;
			};
		}
	}
}

#endif