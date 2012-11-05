#include "..\..\Public\Source\GacUI.h"
#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsGDIRenderer();
}

class GDIWindow : public GuiWindow
{
public:
	GDIWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		SetText(L"Rendering.RawAPI.GDI");
		SetClientSize(Size(640, 480));
		GetBoundsComposition()->SetPreferredMinSize(Size(640, 480));
		MoveToScreenCenter();
	}
};

void GuiMain()
{
	GDIWindow window;
	GetApplication()->Run(&window);
}