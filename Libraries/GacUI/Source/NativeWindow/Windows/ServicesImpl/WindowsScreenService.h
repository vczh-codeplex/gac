/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Native Window::Windows Implementation

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWS_SERVICESIMPL_WINDOWSSCREENSERVICE
#define VCZH_PRESENTATION_WINDOWS_SERVICESIMPL_WINDOWSSCREENSERVICE

#include "..\..\GuiNativeWindow.h"
#include <windows.h>

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
			class WindowsScreen : public Object, public INativeScreen
			{
				friend class WindowsScreenService;
			protected:
				HMONITOR										monitor;
			public:
				WindowsScreen();
				Rect											GetBounds();
				Rect											GetClientBounds();
				WString											GetName();
				bool											IsPrimary();
			};

			class WindowsScreenService : public Object, public INativeScreenService
			{
				typedef HWND (*HandleRetriver)(INativeWindow*);
			protected:
				collections::List<Ptr<WindowsScreen>>			screens;
				HandleRetriver									handleRetriver;
			public:

				struct MonitorEnumProcData
				{
					WindowsScreenService*	screenService;
					int						currentScreen;
				};

				WindowsScreenService(HandleRetriver _handleRetriver);

				static BOOL CALLBACK							MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
				void											RefreshScreenInformation();
				int												GetScreenCount();
				INativeScreen*									GetScreen(int index);
				INativeScreen*									GetScreen(INativeWindow* window);
			};
		}
	}
}

#endif