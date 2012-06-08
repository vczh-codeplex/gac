/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GacUI::Native Window::Windows Implementation

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWS_SERVICESIMPL_WINDOWSCALLBACKSERVICE
#define VCZH_PRESENTATION_WINDOWS_SERVICESIMPL_WINDOWSCALLBACKSERVICE

#include "..\..\GuiNativeWindow.h"
#include <windows.h>

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
			class WindowsCallbackService : public Object, public INativeCallbackService
			{
			protected:
				collections::List<INativeControllerListener*>	listeners;

			public:
				WindowsCallbackService();
				bool											InstallListener(INativeControllerListener* listener);
				bool											UninstallListener(INativeControllerListener* listener);

				void											InvokeMouseHook(WPARAM message, Point location);
				void											InvokeGlobalTimer();
				void											InvokeClipboardUpdated();
				void											InvokeNativeWindowCreated(INativeWindow* window);
				void											InvokeNativeWindowDestroyed(INativeWindow* window);
			};
		}
	}
}

#endif