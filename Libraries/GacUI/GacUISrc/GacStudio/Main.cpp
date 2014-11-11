#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "UI\GacStudioUI.h"
#include "Studio\StudioModel.h"
#include <Windows.h>

using namespace vl::collections;
using namespace ui;
using namespace vm;

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
	List<WString> errors;
	auto resource = GuiResource::LoadFromXml(L"..\\GacStudio\\UI\\Resources.xml", errors);
	GetInstanceLoaderManager()->SetResource(L"GacStudioUI", resource);
	MainWindow window(new StudioModel);
	window.ForceCalculateSizeImmediately();
	window.MoveToScreenCenter();
	GetApplication()->Run(&window);
}