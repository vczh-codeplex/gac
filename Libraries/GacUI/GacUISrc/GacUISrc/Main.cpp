#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "..\..\Source\GacUI.h"
#include "..\..\Source\Reflection\GuiInstanceLoader.h"
#include "..\..\Source\Reflection\TypeDescriptors\GuiReflectionControls.h"
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

Ptr<GuiInstanceContextScope> ShowWindowInResource(Ptr<GuiResource> resource, const WString& name, bool show)
{
	if (auto scope = LoadInstance(resource, L"XmlWindowDemos/"+name+L"/MainWindowResource"))
	{
		GuiWindow* window=dynamic_cast<GuiWindow*>(scope->rootInstance.GetRawPtr());
		if(window)
		{
			window->ForceCalculateSizeImmediately();
			window->MoveToScreenCenter();
			if (show)
			{
				window->Show();
			}

			window->WindowClosed.AttachLambda([window](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
			{
				GetApplication()->InvokeInMainThread([window]()
				{
					delete window;
				});
			});

			return scope;
		}
	}
	return 0;
}

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

	auto resource=GuiResource::LoadFromXml(L"..\\GacUISrcCodepackedTest\\Resources\\XmlWindowResource.xml");
	auto scope = ShowWindowInResource(resource, L"MainWindow", false);

	auto listResources = UnboxValue<GuiTextList*>(scope->referenceValues[L"listResources"]);
	FOREACH(Ptr<GuiResourceFolder>, folder, resource->GetFolderByPath(L"XmlWindowDemos/")->GetFolders())
	{
		if (folder->GetName() != L"MainWindow")
		{
			listResources->GetItems().Add(new list::TextItem(folder->GetName()));
		}
	}
	listResources->ItemLeftButtonDoubleClick.AttachLambda([=](GuiGraphicsComposition* sender, GuiItemMouseEventArgs& arguments)
	{
		ShowWindowInResource(resource, listResources->GetItems()[arguments.itemIndex]->GetText(), true);
	});

	auto buttonShow = UnboxValue<GuiButton*>(scope->referenceValues[L"buttonShow"]);
	buttonShow->Clicked.AttachLambda([=](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		if (listResources->GetSelectedItems().Count() == 1)
		{
			vint itemIndex = listResources->GetSelectedItems()[0];
			ShowWindowInResource(resource, listResources->GetItems()[itemIndex]->GetText(), true);
		}
	});

	GetApplication()->Run(UnboxValue<GuiWindow*>(scope->rootInstance));
}