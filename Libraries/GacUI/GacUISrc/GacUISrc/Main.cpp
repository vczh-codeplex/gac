#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "..\..\Source\GacUI.h"
#include "..\..\..\Workflow\Source\Analyzer\WfAnalyzer.h"
#include "..\..\Source\Reflection\GuiInstanceLoader.h"
#include "..\..\Source\Reflection\TypeDescriptors\GuiReflectionControls.h"
#include "..\..\Source\Reflection\TypeDescriptors\GuiReflectionEvents.h"
#include <Windows.h>

using namespace vl::collections;
using namespace vl::stream;
using namespace vl::reflection::description;
using namespace vl::parsing;
using namespace vl::workflow;
using namespace vl::workflow::analyzer;
using namespace vl::workflow::runtime;

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

//template<typename TImpl>
//class MainWindow_ : public GuiWindow, public GuiInstancePartialClass<GuiWindow>, public Description<TImpl>
//{
//protected:
//	GuiTextList*					listResources;
//	GuiButton*						buttonShow;
//
//	void InitializeComponents()
//	{
//		if (InitializeFromResource())
//		{
//			GUI_INSTANCE_REFERENCE(listResources);
//			GUI_INSTANCE_REFERENCE(buttonShow);
//		}
//	}
//public:
//	MainWindow_()
//		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
//		, GuiInstancePartialClass<GuiWindow>(L"demos::MainWindow")
//		, listResources(0)
//		, buttonShow(0)
//	{
//	}
//};
//
//class MainWindow : public MainWindow_<MainWindow>
//{
//	friend class MainWindow_<MainWindow>;
//	friend struct vl::reflection::description::CustomTypeDescriptorSelector<MainWindow>;
//protected:
//
//	void ShowWindowInResource(const WString& name)
//	{
//		auto resource = GetInstanceLoaderManager()->GetResource(L"Resource");
//		auto scope = LoadInstance(resource, L"XmlWindowDemos/" + name + L"/MainWindowResource");
//		auto window = UnboxValue<GuiWindow*>(scope->rootInstance);
//
//		window->ForceCalculateSizeImmediately();
//		window->MoveToScreenCenter();
//		window->Show();
//
//		window->WindowClosed.AttachLambda([=](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
//		{
//			GetApplication()->InvokeInMainThread([=]()
//			{
//				delete window;
//			});
//		});
//	}
//
//	void listResources_SelectionChanged(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
//	{
//		buttonShow->SetEnabled(listResources->GetSelectedItems().Count() == 1);
//	}
//
//	void listResources_ItemLeftButtonDoubleClick(GuiGraphicsComposition* sender, GuiItemMouseEventArgs& arguments)
//	{
//		ShowWindowInResource(listResources->GetItems()[arguments.itemIndex]->GetText());
//	}
//
//	void buttonShow_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
//	{
//		vint itemIndex = listResources->GetSelectedItems()[0];
//		ShowWindowInResource(listResources->GetItems()[itemIndex]->GetText());
//	}
//public:
//	MainWindow()
//	{
//		InitializeComponents();
//	}
//};
//
//namespace vl
//{
//	namespace reflection
//	{
//		namespace description
//		{
//
//			DECL_TYPE_INFO(MainWindow)
//			IMPL_TYPE_INFO_RENAME(MainWindow, demos::MainWindow)
//
//			BEGIN_CLASS_MEMBER(MainWindow)
//				CLASS_MEMBER_BASE(GuiWindow)
//				CLASS_MEMBER_CONSTRUCTOR(MainWindow*(), NO_PARAMETER)
//
//				CLASS_MEMBER_GUIEVENT_HANDLER(listResources_SelectionChanged, GuiEventArgs)
//				CLASS_MEMBER_GUIEVENT_HANDLER(listResources_ItemLeftButtonDoubleClick, GuiItemMouseEventArgs)
//				CLASS_MEMBER_GUIEVENT_HANDLER(buttonShow_Clicked, GuiEventArgs)
//			END_CLASS_MEMBER(MainWindow)
//
//			class ResourceLoader : public Object, public ITypeLoader
//			{
//			public:
//				void Load(ITypeManager* manager)
//				{
//					ADD_TYPE_INFO(MainWindow)
//				}
//
//				void Unload(ITypeManager* manager)
//				{
//				}
//			};
//
//			class ResourcePlugin : public Object, public IGuiPlugin
//			{
//			public:
//				void Load()override
//				{
//					GetGlobalTypeManager()->AddTypeLoader(new ResourceLoader);
//				}
//
//				void AfterLoad()override
//				{
//				}
//
//				void Unload()override
//				{
//				}
//			};
//			GUI_REGISTER_PLUGIN(ResourcePlugin)
//		}
//	}
//}


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
	List<Ptr<ParsingError>> errors;
	List<WString> codes;
	codes.Add(LR"workflow(
module helloworld;
using system::*;
using system::reflection::*;
using presentation::*;
using presentation::controls::*;
using presentation::compositions::*;

func AddRow(table : GuiTableComposition*, row : int, title : string) : GuiSinglelineTextBox*
{
	{
		var cell = new GuiCellComposition*();
		table.AddChild(cell);
		cell.SetSite(row, 0, 1, 1);

		var label = new GuiLabel*();
		label.Text = title;
		label.BoundsComposition.AlignmentToParent = cast Margin "left:0 top:0 right:0 bottom:0";
		cell.AddChild(label.BoundsComposition);
	}
	{
		var cell = new GuiCellComposition*();
		table.AddChild(cell);
		cell.SetSite(row, 1, 1, 1);

		var textBox = new GuiSinglelineTextBox*();
		textBox.Text = 0;
		textBox.BoundsComposition.AlignmentToParent = cast Margin "left:0 top:0 right:0 bottom:0";
		textBox.BoundsComposition.PreferredMinSize = cast Size $"x:0 y:$(textBox.Font.size * 2)";
		cell.AddChild(textBox.BoundsComposition);
		return textBox;
	}
}

var subscription : Subscription^ = null;

func CreateWindow() : GuiWindow*
{
	var window = new GuiWindow*();
	window.Text = "Scriptable GacUI!";
	window.ContainerComposition.PreferredMinSize = cast Size "x:300 y:200";

	var table = new GuiTableComposition*();
	window.ContainerComposition.AddChild(table);
	table.AlignmentToParent = cast Margin "left:0 top:0 right:0 bottom:0";
	table.CellPadding = 5;
	table.SetRowsAndColumns(4, 2);

	for(row in range[0, 2])
	{
		table.SetRowOption(row, cast GuiCellOption "composeType:MinSize");
	}
	table.SetColumnOption(0, cast GuiCellOption "composeType:MinSize");
	table.SetColumnOption(1, cast GuiCellOption "composeType:Percentage percentage:1.0");

	var textBox1 = AddRow(table, 0, "A = ");
	var textBox2 = AddRow(table, 1, "B = ");
	var textBox3 = AddRow(table, 2, "A + B = ");
	textBox3.Readonly = true;

	subscription = bind($"$(cast int (textBox1.Text) + cast int (textBox2.Text))" ?? "<error>");
	subscription.Subscribe(func(value : object):void
	{
		textBox3.Text = cast string value;
	});

	window.ForceCalculateSizeImmediately();
	window.MoveToScreenCenter();
	return window;
}

func CleanUp():void
{
	subscription.Close();
	subscription = null;
}

)workflow");

	WfLoadTypes();
	auto table = WfLoadTable();
	auto assembly = Compile(table, codes, errors);
	auto globalContext = MakePtr<WfRuntimeGlobalContext>(assembly);
	auto threadContext = MakePtr<WfRuntimeThreadContext>(globalContext);

	auto initializeFunction = LoadFunction<void()>(globalContext, L"<initialize>");
	auto createWindowFunction = LoadFunction<GuiWindow*()>(globalContext, L"CreateWindow");
	auto cleanUpFunction = LoadFunction<void()>(globalContext, L"CleanUp");

	initializeFunction();
	Ptr<GuiWindow> window = createWindowFunction();
	GetApplication()->Run(window.Obj());
	cleanUpFunction();
}