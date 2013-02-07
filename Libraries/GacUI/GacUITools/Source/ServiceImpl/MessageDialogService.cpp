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
				window->GetTitle(),
				INativeDialogService::DisplayOK,
				INativeDialogService::DefaultFirst,
				INativeDialogService::IconInformation
				);
		}

		MessageDialogService::MessageResult MessageDialogService::ShowRequest(const WString& message)
		{
			INativeWindow* window=GetCurrentController()->WindowService()->GetMainWindow();
			return GetCurrentController()->DialogService()->ShowMessageBox(
				window,
				message,
				window->GetTitle(),
				INativeDialogService::DisplayOKCancel,
				INativeDialogService::DefaultFirst,
				INativeDialogService::IconQuestion
				);
		}

		MessageDialogService::MessageResult MessageDialogService::ShowQuestion(const WString& message)
		{
			INativeWindow* window=GetCurrentController()->WindowService()->GetMainWindow();
			return GetCurrentController()->DialogService()->ShowMessageBox(
				window,
				message,
				window->GetTitle(),
				INativeDialogService::DisplayYesNo,
				INativeDialogService::DefaultFirst,
				INativeDialogService::IconQuestion
				);
		}

		MessageDialogService::MessageResult MessageDialogService::ShowQuestionWithCancel(const WString& message)
		{
			INativeWindow* window=GetCurrentController()->WindowService()->GetMainWindow();
			return GetCurrentController()->DialogService()->ShowMessageBox(
				window,
				message,
				window->GetTitle(),
				INativeDialogService::DisplayYesNoCancel,
				INativeDialogService::DefaultFirst,
				INativeDialogService::IconQuestion
				);
		}
	}
}