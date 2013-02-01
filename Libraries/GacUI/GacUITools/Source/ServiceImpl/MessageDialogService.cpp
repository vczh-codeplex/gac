#include "MessageDialogService.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
MessageDialogService
***********************************************************************/

		void MessageDialogService::ShowMessage(const WString& message)
		{
			INativeWindow* window=GetCurrentController()->WindowService()->GetMainWindow();
			GetCurrentController()->DialogService()->ShowMessageBox(
				window,
				message,
				window->GetTitle()
				);
		}
	}
}