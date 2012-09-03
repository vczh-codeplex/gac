#include "..\..\Public\Source\GacUIIncludes.h"
#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsDirect2DRenderer();
}

/***********************************************************************
CustomTemplateWindowStyle
***********************************************************************/

class CustomTemplateWindowStyle : public GuiWindow::IStyleController
{
public:
	GuiBoundsComposition* GetBoundsComposition()override
	{
		return 0;
	}

	GuiGraphicsComposition* GetContainerComposition()override
	{
		return 0;
	}

	void SetFocusableComposition(compositions::GuiGraphicsComposition* value)override
	{
	}

	void SetText(const WString& value)override
	{
	}

	void SetFont(const FontProperties& value)override
	{
	}

	void SetVisuallyEnabled(bool value)override
	{
	}

	void InitializeNativeWindowProperties(GuiWindow* window)override
	{
		// remove all native windows features
		window->SetMinimizedBox(false);
		window->SetMaximizedBox(false);
		window->SetTitleBar(false);
		window->SetSizeBox(false);
	}
};

/***********************************************************************
CustomTemplateWindow
***********************************************************************/

class CustomTemplateWindow : public GuiWindow
{
private:
public:
	CustomTemplateWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		this->SetText(L"Template.Window.CustomizedBorder");

		// set the preferred minimum client size
		this->GetBoundsComposition()->SetPreferredMinSize(Size(640, 480));
		// call this to calculate the size immediately if any indirect content in the table changes
		// so that the window can calcaulte its correct size before calling the MoveToScreenCenter()
		this->ForceCalculateSizeImmediately();
		// move to the screen center
		this->MoveToScreenCenter();
	}

	~CustomTemplateWindow()
	{
	}
};

/***********************************************************************
GuiMain
***********************************************************************/

void GuiMain()
{
	GuiWindow* window=new CustomTemplateWindow;
	GetApplication()->Run(window);
	delete window;
}