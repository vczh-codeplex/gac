#include "WindowsInputService.h"

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
			bool WinIsKeyPressing(vint code)
			{
				return (GetKeyState((int)code)&0xF0)!=0;
			}

			bool WinIsKeyToggled(vint code)
			{
				return (GetKeyState((int)code)&0x0F)!=0;
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
				
			bool WindowsInputService::IsKeyPressing(vint code)
			{
				return WinIsKeyPressing(code);
			}

			bool WindowsInputService::IsKeyToggled(vint code)
			{
				return WinIsKeyToggled(code);
			}

			WString WindowsInputService::GetKeyName(vint code)
			{
				wchar_t name[256]={0};
				vint scanCode=MapVirtualKey((int)code, MAPVK_VK_TO_VSC)<<16;
				switch(code)
				{
				case VK_INSERT:
				case VK_DELETE:
				case VK_HOME:
				case VK_END:
				case VK_PRIOR:
				case VK_NEXT:
				case VK_LEFT:
				case VK_RIGHT:
				case VK_UP:
				case VK_DOWN:
					scanCode|=1<<24;
					break;
				}
				GetKeyNameText((int)scanCode, name, sizeof(name)/sizeof(*name));
				return name[0]?name:L"?";
			}
		}
	}
}