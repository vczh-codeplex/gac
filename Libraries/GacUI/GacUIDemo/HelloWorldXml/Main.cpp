#include "UI\HelloWorld.h"
#include <Windows.h>

using namespace vl::reflection::description;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsDirect2DRenderer();
}

void GuiMain()
{
	GetInstanceLoaderManager()->SetResource(L"Resource", GuiResource::LoadFromXml(L"..\\Resources\\HelloWorld.xml"));
	helloworld::MainWindow window;
	window.ForceCalculateSizeImmediately();
	window.MoveToScreenCenter();
	GetApplication()->Run(&window);
}