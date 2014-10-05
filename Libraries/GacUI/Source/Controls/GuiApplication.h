/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
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

/***********************************************************************
Application
***********************************************************************/

			/// <summary>Represents an GacUI application, for window management and asynchronized operation supporting. Use [M:vl.presentation.controls.GetApplication] to access the instance of this class.</summary>
			class GuiApplication : public Object, private INativeControllerListener, public Description<GuiApplication>
			{
				friend void GuiApplicationInitialize();
				friend class GuiWindow;
				friend class GuiPopup;
				friend class Ptr<GuiApplication>;
			private:
				void											InvokeClipboardNotify(compositions::GuiGraphicsComposition* composition, compositions::GuiEventArgs& arguments);
				void											LeftButtonDown(Point position)override;
				void											LeftButtonUp(Point position)override;
				void											RightButtonDown(Point position)override;
				void											RightButtonUp(Point position)override;
				void											ClipboardUpdated()override;
			protected:
				GuiWindow*										mainWindow;
				GuiWindow*										sharedTooltipOwnerWindow;
				GuiControl*										sharedTooltipOwner;
				GuiTooltip*										sharedTooltipControl;
				bool											sharedTooltipHovering;
				bool											sharedTooltipClosing;
				collections::List<GuiWindow*>					windows;
				collections::SortedList<GuiPopup*>				openingPopups;

				GuiApplication();
				~GuiApplication();

				void											RegisterWindow(GuiWindow* window);
				void											UnregisterWindow(GuiWindow* window);
				void											RegisterPopupOpened(GuiPopup* popup);
				void											RegisterPopupClosed(GuiPopup* popup);
				void											OnMouseDown(Point location);
				void											TooltipMouseEnter(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void											TooltipMouseLeave(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
			public:
				/// <summary>Run a <see cref="GuiWindow"/> as the main window and show it. This function can only be called once in the entry point. When the main window is closed or hiden, the Run function will finished, and the application should prepare for finalization.</summary>
				/// <param name="_mainWindow">The main window.</param>
				void											Run(GuiWindow* _mainWindow);
				/// <summary>Get the main window.</summary>
				/// <returns>The main window.</returns>
				GuiWindow*										GetMainWindow();
				/// <summary>Get all created <see cref="GuiWindow"/> instances. This contains normal windows, popup windows, menus, or other types of windows that inherits from <see cref="GuiWindow"/>.</summary>
				/// <returns>All created <see cref="GuiWindow"/> instances.</returns>
				const collections::List<GuiWindow*>&			GetWindows();
				/// <summary>Get the <see cref="GuiWindow"/> instance that the mouse cursor are directly in.</summary>
				/// <returns>The <see cref="GuiWindow"/> instance that the mouse cursor are directly in.</returns>
				/// <param name="location">The mouse cursor.</param>
				GuiWindow*										GetWindow(Point location);
				/// <summary>Show a tooltip.</summary>
				/// <param name="owner">The control that owns this tooltip temporary.</param>
				/// <param name="tooltip">The control as the tooltip content. This control is not owned by the tooltip. User should manually release this control if no longer needed (usually when the application exit).</param>
				/// <param name="preferredContentWidth">The preferred content width for this tooltip.</param>
				/// <param name="location">The relative location to specify the left-top position of the tooltip.</param>
				void											ShowTooltip(GuiControl* owner, GuiControl* tooltip, vint preferredContentWidth, Point location);
				/// <summary>Close the tooltip</summary>
				void											CloseTooltip();
				/// <summary>Get the tooltip owner. When the tooltip closed, it returns null.</summary>
				/// <returns>The tooltip owner.</returns>
				GuiControl*										GetTooltipOwner();
				/// <summary>Get the file path of the current executable.</summary>
				/// <returns>The file path of the current executable.</returns>
				WString											GetExecutablePath();
				/// <summary>Get the folder of the current executable.</summary>
				/// <returns>The folder of the current executable.</returns>
				WString											GetExecutableFolder();

				/// <summary>Test is the current thread the main thread for GUI.</summary>
				/// <returns>Returns true if the current thread is the main thread for GUI.</returns>
				bool											IsInMainThread();
				/// <summary>Invoke a specified function asynchronously.</summary>
				/// <param name="proc">The specified function.</param>
				void											InvokeAsync(const Func<void()>& proc);
				/// <summary>Invoke a specified function in the main thread.</summary>
				/// <param name="proc">The specified function.</param>
				void											InvokeInMainThread(const Func<void()>& proc);
				/// <summary>Invoke a specified function in the main thread and wait for the function to complete or timeout.</summary>
				/// <returns>Return true if the function complete. Return false if the function has not completed during a specified period of time.</returns>
				/// <param name="proc">The specified function.</param>
				/// <param name="milliseconds">The specified period of time to wait. Set to -1 (default value) to wait forever until the function completed.</param>
				bool											InvokeInMainThreadAndWait(const Func<void()>& proc, vint milliseconds=-1);
				/// <summary>Delay execute a specified function with an specified argument asynchronisly.</summary>
				/// <returns>The Delay execution controller for this task.</returns>
				/// <param name="proc">The specified function.</param>
				/// <param name="milliseconds">Time to delay.</param>
				Ptr<INativeDelay>								DelayExecute(const Func<void()>& proc, vint milliseconds);
				/// <summary>Delay execute a specified function with an specified argument in the main thread.</summary>
				/// <returns>The Delay execution controller for this task.</returns>
				/// <param name="proc">The specified function.</param>
				/// <param name="milliseconds">Time to delay.</param>
				Ptr<INativeDelay>								DelayExecuteInMainThread(const Func<void()>& proc, vint milliseconds);
				/// <summary>Run the specified function in the main thread. If the caller is in the main thread, then run the specified function directly.</summary>
				/// <param name="proc">The specified function.</param>
				void											RunGuiTask(const Func<void()>& proc);

				template<typename T>
				T RunGuiValue(const Func<T()>& proc)
				{
					T result;
					RunGuiTask([&result, &proc]()
					{
						result=proc();
					});
					return result;
				}

				template<typename T>
				void InvokeLambdaInMainThread(const T& proc)
				{
					InvokeInMainThread(Func<void()>(proc));
				}
				
				template<typename T>
				bool InvokeLambdaInMainThreadAndWait(const T& proc, vint milliseconds=-1)
				{
					return InvokeInMainThreadAndWait(Func<void()>(proc), milliseconds);
				}
			};

/***********************************************************************
Plugin
***********************************************************************/

			/// <summary>Represents a plugin for the gui.</summary>
			class IGuiPlugin : public IDescriptable, public Description<IGuiPlugin>
			{
			public:
				/// <summary>Called when the plugin manager want to load this plugin.</summary>
				virtual void									Load()=0;
				/// <summary>Called after the plugin manager loaded all plugins.</summary>
				virtual void									AfterLoad()=0;
				/// <summary>Called when the plugin manager want to unload this plugin.</summary>
				virtual void									Unload()=0;
			};

			/// <summary>Represents a plugin manager.</summary>
			class IGuiPluginManager : public IDescriptable, public Description<IGuiPluginManager>
			{
			public:
				/// <summary>Add a plugin.</summary>
				/// <param name="plugin">The plugin.</param>
				virtual void									AddPlugin(Ptr<IGuiPlugin> plugin)=0;
				/// <summary>Load all plugins.</summary>
				virtual void									Load()=0;
				/// <summary>Unload all plugins.</summary>
				virtual void									Unload()=0;
				/// <summary>Test if all plugins are loaded or not.</summary>
				/// <returns>Returns true if all plugins are loaded.</returns>
				virtual bool									IsLoaded()=0;
			};

/***********************************************************************
Helper Functions
***********************************************************************/

			/// <summary>Get the global <see cref="GuiApplication"/> object.</summary>
			/// <returns>The global <see cref="GuiApplication"/> object.</returns>
			extern GuiApplication*								GetApplication();

			/// <summary>Get the global <see cref="IGuiPluginManager"/> object.</summary>
			/// <returns>The global <see cref="GuiApplication"/> object.</returns>
			extern IGuiPluginManager*							GetPluginManager();

			/// <summary>Destroy the global <see cref="IGuiPluginManager"/> object.</summary>
			extern void											DestroyPluginManager();
		}
	}
}

extern void GuiApplicationMain();

#define GUI_VALUE(x) vl::presentation::controls::GetApplication()->RunGuiValue(LAMBDA([&](){return (x);}))
#define GUI_RUN(x) vl::presentation::controls::GetApplication()->RunGuiTask([=](){x})

#define GUI_REGISTER_PLUGIN(TYPE)\
	class GuiRegisterPluginClass_##TYPE\
	{\
	public:\
		GuiRegisterPluginClass_##TYPE()\
		{\
			vl::presentation::controls::GetPluginManager()->AddPlugin(new TYPE);\
		}\
	} instance_GuiRegisterPluginClass_##TYPE;\

#endif