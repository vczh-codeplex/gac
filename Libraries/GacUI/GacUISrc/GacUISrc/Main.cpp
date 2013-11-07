#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "..\..\Source\GacUI.h"
#include "..\..\Source\Reflection\GuiInstanceLoader.h"
#include <Windows.h>

using namespace vl::collections;
using namespace vl::stream;
using namespace vl::reflection::description;

#define GUI_GRAPHICS_RENDERER_DIRECT2D

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
#ifdef GUI_GRAPHICS_RENDERER_GDI
	int result=SetupWindowsGDIRenderer();
#endif
#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D
	int result=SetupWindowsDirect2DRenderer();
#endif

#if _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return result;
}

extern void UnitTestInGuiMain();

/***********************************************************************
TestWindow
***********************************************************************/

class TestWindow : public GuiWindow
{
protected:
	Ptr<GuiResource>				resource;
	GuiTextList*					listResources;

	void listResources_ItemDoubleClicked(GuiGraphicsComposition* sender, GuiItemMouseEventArgs& arguments)
	{
		WString selectedResource = listResources->GetItems()[arguments.itemIndex]->GetText();
		if (auto scope = LoadInstance(resource, L"XmlWindowDemos/"+selectedResource+L"/MainWindowResource"))
		{
			GuiWindow* window=dynamic_cast<GuiWindow*>(scope->rootInstance.GetRawPtr());
			if(window)
			{
				window->ForceCalculateSizeImmediately();
				window->MoveToScreenCenter();
				window->Show();

				window->WindowClosed.AttachLambda([window](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
				{
					GetApplication()->InvokeInMainThread([window]()
					{
						delete window;
					});
				});
			}
		}
	}
public:
	TestWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		SetText(L"Xml Window Loading");
		SetClientSize(Size(200, 300));
		SetMinimizedBox(false);
		SetMaximizedBox(false);
		SetSizeBox(false);

		resource=GuiResource::LoadFromXml(L"..\\GacUISrcCodepackedTest\\Resources\\XmlWindowResource.xml");

		listResources = g::NewTextList();
		listResources->SetVerticalAlwaysVisible(false);
		listResources->SetHorizontalAlwaysVisible(false);
		listResources->GetBoundsComposition()->SetAlignmentToParent(Margin(5, 5, 5, 5));
		listResources->ItemLeftButtonDoubleClick.AttachMethod(this, &TestWindow::listResources_ItemDoubleClicked);
		this->GetBoundsComposition()->AddChild(listResources->GetBoundsComposition());

		FOREACH(Ptr<GuiResourceFolder>, folder, resource->GetFolderByPath(L"XmlWindowDemos/")->GetFolders())
		{
			listResources->GetItems().Add(new list::TextItem(folder->GetName()));
		}

		// set the preferred minimum client 600
		this->GetBoundsComposition()->SetPreferredMinSize(Size(200, 300));
		// call this to calculate the size immediately if any indirect content in the table changes
		// so that the window can calcaulte its correct size before calling the MoveToScreenCenter()
		this->ForceCalculateSizeImmediately();
		// move to the screen center
		this->MoveToScreenCenter();
	}
};

/*
Type Loader:
	GuiListView
	GuiComboBoxListControl
	GuiToolstripMenu

Features:
	Component Xml Layout	: Type Loaders, Virtual Type Loaders
	Expression				: "bind" Binder
	Data Binding			: Data structure declaration
	Scripting				: Event subscription, code behind
	Control Template		: Template definition and referencing
*/

void GuiMain()
{
#ifndef VCZH_DEBUG_NO_REFLECTION
	{
		FileStream fileStream(L"Reflection.txt", FileStream::WriteOnly);
		BomEncoder encoder(BomEncoder::Utf16);
		EncoderStream encoderStream(fileStream, encoder);
		StreamWriter writer(encoderStream);
		LogTypeManager(writer);
	}
#endif
	UnitTestInGuiMain();
	TestWindow window;
	GetApplication()->Run(&window);
}