#include "WindowsInputService.h"

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
			bool WinIsKeyPressing(int code)
			{
				return (GetKeyState(code)&0xF0)!=0;
			}

			bool WinIsKeyToggled(int code)
			{
				return (GetKeyState(code)&0x0F)!=0;
			}

/***********************************************************************
WindowsInputService
***********************************************************************/

			WindowsInputService::WindowsInputService(HOOKPROC _mouseProc)
				:ownerHandle(NULL)
				,mouseHook(NULL)
				,isTimerEnabled(false)
				,mouseProc(_mouseProc)
			{
			}

			void WindowsInputService::SetOwnerHandle(HWND handle)
			{
				ownerHandle=handle;
			}

			void WindowsInputService::StartHookMouse()
			{
				if(!IsHookingMouse())
				{
					mouseHook=SetWindowsHookEx(WH_MOUSE_LL, mouseProc, NULL, NULL);
				}
			}

			void WindowsInputService::StopHookMouse()
			{
				if(IsHookingMouse())
				{
					UnhookWindowsHookEx(mouseHook);
					mouseHook=NULL;
				}
			}

			bool WindowsInputService::IsHookingMouse()
			{
				return mouseHook!=NULL;
			}

			void WindowsInputService::StartTimer()
			{
				if(!IsTimerEnabled())
				{
					SetTimer(ownerHandle, 1, 16, NULL);
					isTimerEnabled=true;
				}
			}

			void WindowsInputService::StopTimer()
			{
				if(IsTimerEnabled())
				{
					KillTimer(ownerHandle, 1);
					isTimerEnabled=false;
				}
			}

			bool WindowsInputService::IsTimerEnabled()
			{
				return isTimerEnabled;
			}
				
			bool WindowsInputService::IsKeyPressing(int code)
			{
				return WinIsKeyPressing(code);
			}

			bool WindowsInputService::IsKeyToggled(int code)
			{
				return WinIsKeyToggled(code);
			}
		}
	}
}