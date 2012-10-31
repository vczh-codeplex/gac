#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "..\..\..\..\Libraries\GacUI\Public\Source\GacUIIncludes.h"
#include "..\NestleConsole\NestleSDK.h"

#define GUI_GRAPHICS_RENDERER_DIRECT2D

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	int result=SetupWindowsDirect2DRenderer();

#if _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return result;
}

class NestleWindow : public GuiWindow
{
public:
	NestleWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		SetText(L"Vczh Nestle Client v1.0");
		SetClientSize(Size(800, 600));
		GetBoundsComposition()->SetPreferredMinSize(Size(400, 300));
		MoveToScreenCenter();
	}
};

void GuiMain()
{
	win8::Win8Theme theme;
	SetCurrentTheme(&theme);

	NestleWindow window;
	GetApplication()->Run(&window);
}