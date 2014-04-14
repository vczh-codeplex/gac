#include "HelloWorld.h"
#include <Windows.h>

using namespace demos;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsDirect2DRenderer();
}

void GuiMain()
{
	GetInstanceLoaderManager()->SetResource(L"CodeplexClient", GuiResource::LoadFromXml(L"XmlWindowResource.xml"));
	MainWindow window;
	window.ForceCalculateSizeImmediately();
	window.MoveToScreenCenter();
	GetApplication()->Run(&window);
}