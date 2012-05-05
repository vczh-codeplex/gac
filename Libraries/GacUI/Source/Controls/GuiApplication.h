/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Application Framework

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUIAPPLICATION
#define VCZH_PRESENTATION_CONTROLS_GUIAPPLICATION

#include "GuiWindowControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			/// <summary>Represents an GacUI application, for window management and asynchronized operation supporting. Use [M:vl.presentation.controls.GetApplication] to access the instance of this class.</summary>
			class GuiApplication : public Object, private INativeControllerListener
			{
				friend void GuiApplicationInitialize();
				friend class GuiWindow;
				friend class Ptr<GuiApplication>;
			private:
				void											LeftButtonDown(Point position)override;
				void											LeftButtonUp(Point position)override;
				void											RightButtonDown(Point position)override;
				void											RightButtonUp(Point position)override;
				void											ClipboardUpdated()override;
			protected:
				GuiWindow*										mainWindow;
				collections::List<GuiWindow*>					windows;

				GuiApplication();
				~GuiApplication();

				void											RegisterWindow(GuiWindow* window);
				void											UnregisterWindow(GuiWindow* window);
				void											OnMouseDown(Point location);
			public:
				/// <summary>Run a <see cref="GuiWindow"/> as the main window and show it. This function can only be called once in the entry point. When the main window is closed or hiden, the Run function will finished, and the application should prepare for finalization.</summary>
				/// <param name="_mainWindow">The main window.</param>
				void											Run(GuiWindow* _mainWindow);
				/// <summary>Get all created <see cref="GuiWindow"/> instances. This contains normal windows, popup windows, menus, or other types of windows that inherits from <see cref="GuiWindow"/>.</summary>
				/// <returns>All created <see cref="GuiWindow"/> instances.</returns>
				const collections::IReadonlyList<GuiWindow*>&	GetWindows();
				/// <summary>Get the <see cref="GuiWindow"/> instance that the mouse cursor are directly in.</summary>
				/// <returns>The <see cref="GuiWindow"/> instance that the mouse cursor are directly in.</returns>
				/// <param name="location">The mouse cursor.</param>
				GuiWindow*										GetWindow(Point location);

				/// <summary>Test is the current thread the main thread for GUI.</summary>
				/// <returns>Returns true if the current thread is the main thread for GUI.</returns>
				bool											IsInMainThread();
				/// <summary>Invoke a specified function with an specified argument in the main thread.</summary>
				/// <param name="proc">The specified function.</param>
				/// <param name="argument">The specified argument.</param>
				void											InvokeInMainThread(INativeAsyncService::AsyncTaskProc* proc, void* argument);
				/// <summary>Invoke a specified function with an specified argument in the main thread and wait for the function to complete or timeout.</summary>
				/// <returns>Return true if the function complete. Return false if the function has not completed during a specified period of time.</returns>
				/// <param name="proc">The specified function.</param>
				/// <param name="argument">The specified argument.</param>
				/// <param name="milliseconds">The specified period of time to wait. Set to -1 (default value) to wait forever until the function completed.</param>
				bool											InvokeInMainThreadAndWait(INativeAsyncService::AsyncTaskProc* proc, void* argument, int milliseconds=-1);
				/// <summary>Invoke a specified function in the main thread.</summary>
				/// <param name="proc">The specified function.</param>
				void											InvokeInMainThread(const Func<void()>& proc);
				/// <summary>Invoke a specified function in the main thread and wait for the function to complete or timeout.</summary>
				/// <returns>Return true if the function complete. Return false if the function has not completed during a specified period of time.</returns>
				/// <param name="proc">The specified function.</param>
				/// <param name="milliseconds">The specified period of time to wait. Set to -1 (default value) to wait forever until the function completed.</param>
				bool											InvokeInMainThreadAndWait(const Func<void()>& proc, int milliseconds=-1);

				template<typename T>
				void InvokeLambdaInMainThread(const T& proc)
				{
					InvokeInMainThread(Func<void()>(proc));
				}
				
				template<typename T>
				bool InvokeLambdaInMainThreadAndWait(const T& proc, int milliseconds=-1)
				{
					return InvokeInMainThreadAndWait(Func<void()>(proc), milliseconds);
				}
			};

			/// <summary>Get the global <see cref="GuiApplication"/> object.</summary>
			/// <returns>The global <see cref="GuiApplication"/> object.</returns>
			extern GuiApplication*								GetApplication();
		}
	}
}

extern void GuiApplicationMain();

#endif