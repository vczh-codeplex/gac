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

class MainWindowInstance : public GuiInstance<GuiWindow>
{
protected:
	GuiTextList*					listResources;
	GuiButton*						buttonShow;

	void ShowWindowInResource(const WString& name)
	{
		auto scope = LoadInstance(GetResource(), L"XmlWindowDemos/" + name + L"/MainWindowResource");
		auto window = UnboxValue<GuiWindow*>(scope->rootInstance);

		window->ForceCalculateSizeImmediately();
		window->MoveToScreenCenter();
		window->Show();

		window->WindowClosed.AttachLambda([=](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			GetApplication()->InvokeInMainThread([=]()
			{
				delete window;
			});
		});
	}

	void listResources_SelectionChanged(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		buttonShow->SetEnabled(listResources->GetSelectedItems().Count() == 1);
	}

	void listResources_ItemLeftButtonDoubleClick(GuiGraphicsComposition* sender, GuiItemMouseEventArgs& arguments)
	{
		ShowWindowInResource(listResources->GetItems()[arguments.itemIndex]->GetText());
	}

	void buttonShow_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		vint itemIndex = listResources->GetSelectedItems()[0];
		ShowWindowInResource(listResources->GetItems()[itemIndex]->GetText());
	}
public:
	MainWindowInstance(Ptr<GuiResource> resource)
		:GuiInstance(resource, L"XmlWindowDemos/MainWindow/MainWindowResource")
	{
		GUI_INSTANCE_REFERENCE(listResources);
		GUI_INSTANCE_REFERENCE(buttonShow);

		FOREACH(Ptr<GuiResourceFolder>, folder, GetResource()->GetFolderByPath(L"XmlWindowDemos/")->GetFolders())
		{
			if (folder->GetName() != L"MainWindow")
			{
				listResources->GetItems().Add(new list::TextItem(folder->GetName()));
			}
		}
		
		listResources->SelectionChanged.AttachMethod(this, &MainWindowInstance::listResources_SelectionChanged);
		listResources->ItemLeftButtonDoubleClick.AttachMethod(this, &MainWindowInstance::listResources_ItemLeftButtonDoubleClick);
		buttonShow->Clicked.AttachMethod(this, &MainWindowInstance::buttonShow_Clicked);

		GetInstance()->ForceCalculateSizeImmediately();
		GetInstance()->MoveToScreenCenter();
	}

	~MainWindowInstance()
	{
		delete GetInstance();
	}
};

/*
Type Loader:
	GuiListView
	GuiComboBoxListControl

Features:
	Component Xml Layout	: Type Loaders, Virtual Type Loaders, Constructor Parameters, Object Inception (e.g. ViewModel object), Object Schema
	Expression				: "bind" Binder
	Data Binding			: Data structure declaration
	Scripting				: Event subscription, code behind
	Control Template		: Template definition and referencing
	Data Service and Query
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
	{
		FileStream fileStream(L"Instance.txt", FileStream::WriteOnly);
		BomEncoder encoder(BomEncoder::Utf16);
		EncoderStream encoderStream(fileStream, encoder);
		StreamWriter writer(encoderStream);
		LogInstanceLoaderManager(writer);
	}
#endif
	UnitTestInGuiMain();

	auto resource=GuiResource::LoadFromXml(L"..\\GacUISrcCodepackedTest\\Resources\\XmlWindowResource.xml");
	MainWindowInstance instance(resource);
	GetApplication()->Run(instance.GetInstance());
}