#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "UI\GacStudioUI.h"
#include <Windows.h>

using namespace gacstudio::user_interface;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	int result =  SetupWindowsDirect2DRenderer();
#if _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return result;
}

void GuiMain()
{
	GetInstanceLoaderManager()->SetResource(L"GacStudioUI", GuiResource::LoadFromXml(L"..\\GacStudio\\UI\\Resources.xml"));
	MainWindow window;
	window.ForceCalculateSizeImmediately();
	window.MoveToScreenCenter();
	GetApplication()->Run(&window);
}