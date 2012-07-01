#include "WindowsDialogService.h"

namespace vl
{
	namespace presentation
	{
		namespace windows
		{

/***********************************************************************
WindowsDialogService
***********************************************************************/

			INativeDialogService::MessageBoxButtonsOutput WindowsDialogService::ShowMessageBox(
				INativeWindow* window,
				const WString& text,
				const WString& title,
				MessageBoxButtonsInput buttons,
				MessageBoxDefaultButton defaultButton,
				MessageBoxIcons icon,
				MessageBoxModelOptions model)
			{
				return SelectOK;
			}
		}
	}
}