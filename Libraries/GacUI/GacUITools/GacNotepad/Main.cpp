#include "..\..\Public\Source\GacUI.h"

using namespace vl::collections;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsDirect2DRenderer();
}

class MainWindow : public GuiWindow
{
public:
	MainWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		this->SetText(L"GacUI Notepad (www.gaclib.net)");

		this->GetBoundsComposition()->SetPreferredMinSize(Size(640, 480));
		this->ForceCalculateSizeImmediately();
		this->MoveToScreenCenter();
	}

	~MainWindow()
	{
	}
};

/***********************************************************************
GuiMain
***********************************************************************/

void GuiMain()
{
	MainWindow window;
	GetApplication()->Run(&window);
}