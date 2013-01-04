#include "WindowsScreenService.h"

namespace vl
{
	namespace presentation
	{
		namespace windows
		{

/***********************************************************************
WindowsScreen
***********************************************************************/

			WindowsScreen::WindowsScreen()
			{
				monitor=NULL;
			}

			Rect WindowsScreen::GetBounds()
			{
				MONITORINFOEX info;
				info.cbSize=sizeof(MONITORINFOEX);
				GetMonitorInfo(monitor, &info);
				return Rect(info.rcMonitor.left, info.rcMonitor.top, info.rcMonitor.right, info.rcMonitor.bottom);
			}

			Rect WindowsScreen::GetClientBounds()
			{
				MONITORINFOEX info;
				info.cbSize=sizeof(MONITORINFOEX);
				GetMonitorInfo(monitor, &info);
				return Rect(info.rcWork.left, info.rcWork.top, info.rcWork.right, info.rcWork.bottom);
			}

			WString WindowsScreen::GetName()
			{
				MONITORINFOEX info;
				info.cbSize=sizeof(MONITORINFOEX);
				GetMonitorInfo(monitor, &info);
					
				wchar_t buffer[sizeof(info.szDevice)/sizeof(*info.szDevice)+1];
				memset(buffer, 0, sizeof(buffer));
				memcpy(buffer, info.szDevice, sizeof(info.szDevice));
				return buffer;
			}

			bool WindowsScreen::IsPrimary()
			{
				MONITORINFOEX info;
				info.cbSize=sizeof(MONITORINFOEX);
				GetMonitorInfo(monitor, &info);
				return info.dwFlags==MONITORINFOF_PRIMARY;
			}

/***********************************************************************
WindowsScreenService
***********************************************************************/

			WindowsScreenService::WindowsScreenService(HandleRetriver _handleRetriver)
				:handleRetriver(_handleRetriver)
			{
			}

			BOOL CALLBACK WindowsScreenService::MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
			{
				MonitorEnumProcData* data=(MonitorEnumProcData*)dwData;
				if(data->currentScreen==data->screenService->screens.Count())
				{
					data->screenService->screens.Add(new WindowsScreen());
				}
				data->screenService->screens[data->currentScreen]->monitor=hMonitor;
				data->currentScreen++;
				return TRUE;
			}

			void WindowsScreenService::RefreshScreenInformation()
			{
				for(vint i=0;i<screens.Count();i++)
				{
					screens[i]->monitor=NULL;
				}
				MonitorEnumProcData data;
				data.screenService=this;
				data.currentScreen=0;
				EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)(&data));
			}
				
			vint WindowsScreenService::GetScreenCount()
			{
				RefreshScreenInformation();
				return GetSystemMetrics(SM_CMONITORS);
			}

			INativeScreen* WindowsScreenService::GetScreen(vint index)
			{
				RefreshScreenInformation();
				return screens[index].Obj();
			}

			INativeScreen* WindowsScreenService::GetScreen(INativeWindow* window)
			{
				RefreshScreenInformation();
				HWND hwnd=handleRetriver(window);
				if(hwnd)
				{
					HMONITOR monitor=MonitorFromWindow(hwnd, MONITOR_DEFAULTTONULL);
					if(monitor!=NULL)
					{
						for(vint i=0;i<screens.Count();i++)
						{
							if(screens[i]->monitor==monitor)
							{
								return screens[i].Obj();
							}
						}
					}
				}
				return 0;
			}
		}
	}
}