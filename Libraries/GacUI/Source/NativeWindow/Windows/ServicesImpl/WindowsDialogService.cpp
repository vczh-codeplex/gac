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

			WindowsDialogService::WindowsDialogService(HandleRetriver _handleRetriver)
				:handleRetriver(_handleRetriver)
			{
			}

			INativeDialogService::MessageBoxButtonsOutput WindowsDialogService::ShowMessageBox(
				INativeWindow* window,
				const WString& text,
				const WString& title,
				MessageBoxButtonsInput buttons,
				MessageBoxDefaultButton defaultButton,
				MessageBoxIcons icon,
				MessageBoxModalOptions modal)
			{
				WString realTitle=title;
				if(title==L"" && window!=0)
				{
					realTitle=window->GetTitle();
				}
				HWND hWnd=handleRetriver(window);
				LPCTSTR lpText=text.Buffer();
				LPCTSTR lpCaption=realTitle.Buffer();
				UINT uType=0;
				

#define MAP(A, B) case A: uType|=(B); break
				switch(buttons)
				{
					MAP(DisplayOK, MB_OK);
					MAP(DisplayOKCancel, MB_OKCANCEL);
					MAP(DisplayYesNo, MB_YESNO);
					MAP(DisplayYesNoCancel, MB_YESNOCANCEL);
					MAP(DisplayRetryCancel, MB_RETRYCANCEL);
					MAP(DisplayAbortRetryIgnore, MB_ABORTRETRYIGNORE);
					MAP(DisplayCancelTryAgainContinue, MB_CANCELTRYCONTINUE);
				}
				switch(defaultButton)
				{
					MAP(DefaultFirst, MB_DEFBUTTON1);
					MAP(DefaultSecond, MB_DEFBUTTON2);
					MAP(DefaultThird, MB_DEFBUTTON3);
				}
				switch(icon)
				{
					MAP(IconError, MB_ICONERROR);
					MAP(IconQuestion, MB_ICONQUESTION);
					MAP(IconWarning, MB_ICONWARNING);
					MAP(IconInformation, MB_ICONINFORMATION);
				}
				switch(modal)
				{
					MAP(ModalWindow, MB_APPLMODAL);
					MAP(ModalSystem, MB_SYSTEMMODAL);
					MAP(ModalTask, MB_TASKMODAL);
				}
#undef MAP

				int result=MessageBox(hWnd, lpText, lpCaption, uType);
				switch(result)
				{
				case IDABORT: return SelectAbort;
				case IDCANCEL: return SelectCancel;
				case IDCONTINUE: return SelectContinue;
				case IDIGNORE: return SelectIgnore;
				case IDNO: return SelectNo;
				case IDOK: return SelectOK;
				case IDRETRY: return SelectRetry;
				case IDTRYAGAIN: return SelectTryAgain;
				case IDYES: return SelectYes;
				default: return SelectOK;
				}
			}
		}
	}
}