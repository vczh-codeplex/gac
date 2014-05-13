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
#include "..\..\Source\Reflection\TypeDescriptors\GuiReflectionTemplates.h"
#include <Windows.h>

using namespace vl::collections;
using namespace vl::stream;
using namespace vl::reflection::description;
using namespace vl::parsing;
using namespace vl::workflow;
using namespace vl::workflow::analyzer;
using namespace vl::workflow::runtime;
using namespace vl::presentation::controls::list;
using namespace vl::presentation::templates;

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

/*
Data Binding: IValueObservableList/ItemTemplate
	GuiSelectableListControl
	GuiVirtualTextList
	GuiVirtualListView
	GuiVirtualTreeView
	GuiVirtualDataGrid
	GuiTab
	GuiComboBoxListControl

Features:
	View Model				: View model interface declaration
	Data Binding			: Data structure declaration
	Control Template		: Interface methods to events, or prepare predefined classes
	Workflow as Code Behind
	Data Service and Query
*/

#ifndef VCZH_DEBUG_NO_REFLECTION

namespace demos
{
// -------- WILL BE GENERATED BY "Vczh GacUI Resource Code Generator" -------- //

	template<typename TImpl>
	class MainWindow_ : public GuiWindow, public GuiInstancePartialClass<GuiWindow>, public Description<TImpl>
	{
	protected:

		void InitializeComponents()
		{
			if (InitializeFromResource())
			{
			}
			else
			{
			}
		}
	public:
		MainWindow_()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
			,GuiInstancePartialClass<GuiWindow>(L"demos::MainWindow")
		{
		}
	};

// -------- WILL BE GENERATED BY "Vczh GacUI Resource Code Generator" -------- //
}

namespace demos
{
	class MainWindow : public MainWindow_<MainWindow>
	{
	public:
		MainWindow()
		{
			InitializeComponents();
		}
	};
}

namespace vl
{
	namespace reflection
	{
		namespace description
		{
// -------- WILL BE GENERATED BY "Vczh GacUI Resource Code Generator" -------- //

#define DEMO_TYPES(F)\
			F(demos::MainWindow)

			DEMO_TYPES(DECL_TYPE_INFO)
			DEMO_TYPES(IMPL_TYPE_INFO)

			BEGIN_CLASS_MEMBER(demos::MainWindow)
				CLASS_MEMBER_BASE(GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(demos::MainWindow*(), NO_PARAMETER)
			END_CLASS_MEMBER(demos::MainWindow)

			class DemoResourceLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					DEMO_TYPES(ADD_TYPE_INFO)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};

			class DemoResourcePlugin : public Object, public IGuiPlugin
			{
			public:
				void Load()override
				{
					GetGlobalTypeManager()->AddTypeLoader(new DemoResourceLoader);
				}

				void AfterLoad()override
				{
				}

				void Unload()override
				{
				}
			};
			GUI_REGISTER_PLUGIN(DemoResourcePlugin)

// -------- WILL BE GENERATED BY "Vczh GacUI Resource Code Generator" -------- //
		}
	}
}

#endif

void GuiMain()
{
#ifndef VCZH_DEBUG_NO_REFLECTION
	UnitTestInGuiMain();
	//{
	//	FileStream fileStream(L"Reflection.txt", FileStream::WriteOnly);
	//	BomEncoder encoder(BomEncoder::Utf16);
	//	EncoderStream encoderStream(fileStream, encoder);
	//	StreamWriter writer(encoderStream);
	//	LogTypeManager(writer);
	//}
	//{
	//	FileStream fileStream(L"Instance.txt", FileStream::WriteOnly);
	//	BomEncoder encoder(BomEncoder::Utf16);
	//	EncoderStream encoderStream(fileStream, encoder);
	//	StreamWriter writer(encoderStream);
	//	LogInstanceLoaderManager(writer);
	//}

	List<WString> errors;
	auto resource = GuiResource::LoadFromXml(L"..\\GacUISrcCodepackedTest\\Resources\\XmlWindowResourceDataBinding.xml", errors);
	GetInstanceLoaderManager()->SetResource(L"Demo", resource);
	demos::MainWindow window;

	auto scope = window.GetScope().Obj();
	CopyFrom(errors, scope->errors, true);

	window.ForceCalculateSizeImmediately();
	window.MoveToScreenCenter();
	GetApplication()->Run(&window);
#endif
}