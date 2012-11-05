#include "..\..\Public\Source\GacUI.h"
#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsDirect2DRenderer();
}

class Direct2DWindow : public GuiWindow
{
public:
	Direct2DWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		SetText(L"Rendering.RawAPI.Direct2D");
		SetClientSize(Size(640, 480));
		GetBoundsComposition()->SetPreferredMinSize(Size(640, 480));
		MoveToScreenCenter();
	}
};

void GuiMain()
{
	Direct2DWindow window;
	GetApplication()->Run(&window);
}