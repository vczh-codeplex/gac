/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Native Window::Windows Implementation

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWS_SERVICESIMPL_WINDOWSDIALOGSERVICE
#define VCZH_PRESENTATION_WINDOWS_SERVICESIMPL_WINDOWSDIALOGSERVICE

#include "..\..\GuiNativeWindow.h"

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
			class WindowsDialogService : public INativeDialogService
			{
			public:
				MessageBoxButtonsOutput			ShowMessageBox(INativeWindow* window, const WString& text, const WString& title, MessageBoxButtonsInput buttons, MessageBoxDefaultButton defaultButton, MessageBoxIcons icon, MessageBoxModelOptions model)override;
			};
		}
	}
}

#endif