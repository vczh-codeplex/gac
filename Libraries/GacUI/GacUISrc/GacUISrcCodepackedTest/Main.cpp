#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "..\..\Public\Source\GacUIIncludes.h"
#include <Windows.h>

#define GUI_GRAPHICS_RENDERER_DIRECT2D

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
#ifdef GUI_GRAPHICS_RENDERER_GDI
	return SetupWindowsGDIRenderer();
#endif
#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D
	return SetupWindowsDirect2DRenderer();
#endif

#if _DEBUG
	_CrtDumpMemoryLeaks();
#endif
}

extern void SetupBasicWindow(GuiControlHost* controlHost, GuiControl* container);
extern void SetupTextBoxWindow(GuiControlHost* controlHost, GuiControl* container);
extern void SetupTabPageListControlWindow(GuiControlHost* controlHost, GuiControl* container);
extern void SetupTabPageToolstripWindow(GuiControlHost* controlHost, GuiControl* container);
extern void SetupDialogWindow(GuiControlHost* controlHost, GuiControl* container);
extern void SetupRibbonWindow(GuiControlHost* controlHost, GuiControl* container);

/***********************************************************************
SetupWindow
***********************************************************************/

void SetupTabPageWindow(GuiControlHost* controlHost, GuiControl* container)
{
	container->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	GuiTab* tab=new GuiTab(new win7::Win7TabStyle);
	tab->GetBoundsComposition()->SetAlignmentToParent(Margin(6, 6, 6, 6));
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"Basic");
		SetupBasicWindow(controlHost, page->GetContainer());
	}
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"Text Box");
		SetupTextBoxWindow(controlHost, page->GetContainer());
	}
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"List Control");
		SetupTabPageListControlWindow(controlHost, page->GetContainer());
	}
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"Toolstrip");
		SetupTabPageToolstripWindow(controlHost, page->GetContainer());
	}
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"Dialogs");
		SetupDialogWindow(controlHost, page->GetContainer());
	}
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"Ribbon (not completed)");
		SetupRibbonWindow(controlHost, page->GetContainer());
	}
	container->GetContainerComposition()->AddChild(tab->GetBoundsComposition());
	tab->MovePage(tab->GetPages()[3], 2);
}

void GuiMain()
{
	GuiWindow window(new win7::Win7WindowStyle);
#ifdef GUI_GRAPHICS_RENDERER_GDI
	window.SetText(L"Vczh GUI Demo (GDI)");
#endif
#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D
	window.SetText(L"Vczh GUI Demo (Direct2D)");
#endif
	SetupTabPageWindow(&window, &window);

	window.SetClientSize(Size(800, 600));
	INativeScreen* screen=window.GetRelatedScreen();
	Rect windowBounds=window.GetBounds();
	Rect screenBounds=screen->GetClientBounds();
	window.SetBounds(Rect(
		Point(
			screenBounds.Left()+(screenBounds.Width()-windowBounds.Width())/2,
			screenBounds.Top()+(screenBounds.Height()-windowBounds.Height())/2
			),
		windowBounds.GetSize()
		));

	GetApplication()->Run(&window);
}