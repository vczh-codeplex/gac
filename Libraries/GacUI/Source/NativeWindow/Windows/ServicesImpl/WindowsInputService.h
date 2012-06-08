/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Native Window::Windows Implementation

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWS_SERVICESIMPL_WINDOWSINPUTSERVICE
#define VCZH_PRESENTATION_WINDOWS_SERVICESIMPL_WINDOWSINPUTSERVICE

#include "..\..\GuiNativeWindow.h"
#include <windows.h>

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
			class WindowsInputService : public Object, public INativeInputService
			{
			protected:
				HWND								ownerHandle;
				HHOOK								mouseHook;
				bool								isTimerEnabled;
				HOOKPROC							mouseProc;
			public:
				WindowsInputService(HOOKPROC _mouseProc);

				void								SetOwnerHandle(HWND handle);
				void								StartHookMouse();
				void								StopHookMouse();
				bool								IsHookingMouse();
				void								StartTimer();
				void								StopTimer();
				bool								IsTimerEnabled();
				bool								IsKeyPressing(int code);
				bool								IsKeyToggled(int code);
			};

			extern bool								WinIsKeyPressing(int code);
			extern bool								WinIsKeyToggled(int code);
		}
	}
}

#endif