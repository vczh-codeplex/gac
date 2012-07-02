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

			bool WindowsDialogService::ShowColorDialog(INativeWindow* window, Color& selection, bool selected, ColorDialogCustomColorOptions customColorOptions, Color* customColors)
			{
				CHOOSECOLOR chooseColor;
				ZeroMemory(&chooseColor, sizeof(chooseColor));
				COLORREF customColorsBuffer[16]={0};
				if(customColors)
				{
					for(int i=0;i<sizeof(customColorsBuffer)/sizeof(*customColorsBuffer);i++)
					{
						customColorsBuffer[i]=RGB(customColors[i].r, customColors[i].g, customColors[i].b);
					}
				}

				chooseColor.lStructSize=sizeof(chooseColor);
				chooseColor.hwndOwner=handleRetriver(window);
				chooseColor.rgbResult=RGB(selection.r, selection.g, selection.b);
				chooseColor.lpCustColors=customColorsBuffer;
				chooseColor.Flags=CC_ANYCOLOR;
				
#define MAP(A, B) case A: chooseColor.Flags|=(B); break
				switch(customColorOptions)
				{
					MAP(CustomColorDisabled, CC_PREVENTFULLOPEN);
					MAP(CustomColorOpened, CC_FULLOPEN);
				}
#undef MAP
				if(selected)
				{
					chooseColor.Flags|=CC_RGBINIT;
				}

				BOOL result=ChooseColor(&chooseColor);
				if(result)
				{
					selection=Color(GetRValue(chooseColor.rgbResult), GetGValue(chooseColor.rgbResult), GetBValue(chooseColor.rgbResult));
					if(customColors)
					{
						for(int i=0;i<sizeof(customColorsBuffer)/sizeof(*customColorsBuffer);i++)
						{
							COLORREF color=customColorsBuffer[i];
							customColors[i]=Color(GetRValue(color), GetGValue(color), GetBValue(color));
						}
					}
				}
				return result!=FALSE;
			}
		}
	}
}