#include "CodeplexClient.h"
#include <Windows.h>

using namespace codeplex_client;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsDirect2DRenderer();
}

void GuiMain()
{
	GetInstanceLoaderManager()->SetResource(L"CodeplexClient", GuiResource::LoadFromXml(L"XmlWindowResource.xml"));
	SignInWindow window;
	window.ForceCalculateSizeImmediately();
	window.MoveToScreenCenter();
	GetApplication()->Run(&window);
}