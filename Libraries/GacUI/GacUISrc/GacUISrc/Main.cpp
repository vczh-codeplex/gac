#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "..\..\Source\GacUI.h"
#ifndef VCZH_DEBUG_NO_REFLECTION
#include "..\..\Source\Reflection\GuiInstanceLoader.h"
#include "..\..\Source\Reflection\TypeDescriptors\GuiReflectionControls.h"
#include "..\..\Source\Reflection\TypeDescriptors\GuiReflectionEvents.h"
#endif
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
	int result = SetupWindowsDirect2DRenderer();
#endif

#if _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return result;
}

extern void UnitTestInGuiMain();

#ifndef VCZH_DEBUG_NO_REFLECTION
template<typename TImpl>
class MainWindow_ : public GuiWindow, public GuiInstancePartialClass<GuiWindow>, public Description<TImpl>
{
protected:
	GuiTextList*					listResources;
	GuiButton*						buttonShow;

	void InitializeComponents()
	{
		if (InitializeFromResource())
		{
			GUI_INSTANCE_REFERENCE(listResources);
			GUI_INSTANCE_REFERENCE(buttonShow);
		}
	}
public:
	MainWindow_()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		, GuiInstancePartialClass<GuiWindow>(L"demos::MainWindow")
		, listResources(0)
		, buttonShow(0)
	{
	}
};

class MainWindow : public MainWindow_<MainWindow>
{
	friend class MainWindow_<MainWindow>;
	friend struct vl::reflection::description::CustomTypeDescriptorSelector<MainWindow>;
protected:

	void ShowWindowInResource(const WString& name)
	{
		auto resource = GetInstanceLoaderManager()->GetResource(L"Resource");
		auto scope = LoadInstance(resource, L"XmlWindowDemos/" + name + L"/MainWindowResource");
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
	MainWindow()
	{
		InitializeComponents();
	}
};

namespace vl
{
	namespace reflection
	{
		namespace description
		{

			DECL_TYPE_INFO(MainWindow)
			IMPL_TYPE_INFO_RENAME(MainWindow, demos::MainWindow)

			BEGIN_CLASS_MEMBER(MainWindow)
				CLASS_MEMBER_BASE(GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(MainWindow*(), NO_PARAMETER)

				CLASS_MEMBER_GUIEVENT_HANDLER(listResources_SelectionChanged, GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(listResources_ItemLeftButtonDoubleClick, GuiItemMouseEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(buttonShow_Clicked, GuiEventArgs)
			END_CLASS_MEMBER(MainWindow)

			class ResourceLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					ADD_TYPE_INFO(MainWindow)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};

			class ResourcePlugin : public Object, public IGuiPlugin
			{
			public:
				void Load()override
				{
					GetGlobalTypeManager()->AddTypeLoader(new ResourceLoader);
				}

				void AfterLoad()override
				{
				}

				void Unload()override
				{
				}
			};
			GUI_REGISTER_PLUGIN(ResourcePlugin)
		}
	}
}
#else
class MainWindow : public GuiWindow
{
public:
	MainWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		SetText(L"MainWindow");
		SetClientSize(Size(640, 480));
		GetContainerComposition()->SetPreferredMinSize(Size(640, 480));
	}
};
#endif

/*
Data Binding:
	GuiSelectableListControl
	GuiVirtualTextList
	GuiVirtualListView
	GuiVirtualTreeView
	GuiVirtualDataGrid

Features:
	Component Xml Layout	: Object Inception, Object Schema
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

#ifndef VCZH_DEBUG_NO_REFLECTION
	GetInstanceLoaderManager()->SetResource(L"Resource", GuiResource::LoadFromXml(L"..\\GacUISrcCodepackedTest\\Resources\\XmlWindowResource.xml"));
#endif
	MainWindow window;
	window.ForceCalculateSizeImmediately();
	window.MoveToScreenCenter();
	GetApplication()->Run(&window);
}