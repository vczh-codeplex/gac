/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GacUI::Native Window::Windows Implementation

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWS_SERVICESIMPL_WINDOWSDIALOGSERVICE
#define VCZH_PRESENTATION_WINDOWS_SERVICESIMPL_WINDOWSDIALOGSERVICE

#include "..\..\GuiNativeWindow.h"
#include <windows.h>

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
			class WindowsDialogService : public INativeDialogService
			{
				typedef HWND (*HandleRetriver)(INativeWindow*);
			protected:
				HandleRetriver									handleRetriver;

			public:
				WindowsDialogService(HandleRetriver _handleRetriver);

				MessageBoxButtonsOutput			ShowMessageBox(INativeWindow* window, const WString& text, const WString& title, MessageBoxButtonsInput buttons, MessageBoxDefaultButton defaultButton, MessageBoxIcons icon, MessageBoxModalOptions modal)override;
			};
		}
	}
}

#endif