#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "..\..\Source\GacUI.h"
#include "..\..\..\Workflow\Source\Analyzer\WfAnalyzer.h"
#include "..\..\Source\Resources\GuiParserManager.h"
#include "..\..\Source\Reflection\GuiInstanceLoader.h"
#include "..\..\Source\Reflection\TypeDescriptors\GuiReflectionControls.h"
#include "..\..\Source\Reflection\TypeDescriptors\GuiReflectionEvents.h"
#include "..\..\Source\Reflection\TypeDescriptors\GuiReflectionTemplates.h"
#include <Windows.h>

using namespace vl::collections;
using namespace vl::stream;
using namespace vl::reflection::description;
using namespace vl::parsing;
using namespace vl::parsing::xml;
using namespace vl::workflow;
using namespace vl::workflow::analyzer;
using namespace vl::workflow::runtime;
using namespace vl::presentation::controls::list;
using namespace vl::presentation::templates;

//#define GUI_GRAPHICS_RENDERER_GDI
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

#ifndef VCZH_DEBUG_NO_REFLECTION

namespace demos
{
	class Data : public Object, public Description<Data>
	{
	public:
		WString					name;
		WString					organization;
		WString					title;

		Data()
		{
		}

		Data(const WString& _name, const WString& _organization, const WString& _title)
			:name(_name)
			, organization(_organization)
			, title(_title)
		{
		}
	};

	template<typename TImpl>
	class MainWindow_ : public GuiWindow, public GuiInstancePartialClass<GuiWindow>, public Description<TImpl>
	{
	protected:
		Ptr<IValueEnumerable> ViewModel_;

		void InitializeComponents(Ptr<IValueEnumerable> ViewModel)
		{
			ViewModel_ = ViewModel;
			InitializeFromResource();
		}
	public:
		MainWindow_()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
			,GuiInstancePartialClass<GuiWindow>(L"demos::MainWindow")
		{
		}

		Ptr<IValueEnumerable> GetViewModel()
		{
			return ViewModel_;
		}
	};

	class MainWindow : public MainWindow_<MainWindow>
	{
	public:
		MainWindow(Ptr<IValueEnumerable> ViewModel)
		{
			InitializeComponents(ViewModel);
		}
	};
}

namespace vl
{
	namespace reflection
	{
		namespace description
		{
#define DEMO_TYPES(F)\
			F(demos::Data)\
			F(demos::MainWindow)\

			DEMO_TYPES(DECL_TYPE_INFO)
			DEMO_TYPES(IMPL_CPP_TYPE_INFO)

			BEGIN_CLASS_MEMBER(demos::Data)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<demos::Data>(), NO_PARAMETER)

				CLASS_MEMBER_FIELD(name)
				CLASS_MEMBER_FIELD(organization)
				CLASS_MEMBER_FIELD(title)
			END_CLASS_MEMBER(demos::Data)

			BEGIN_CLASS_MEMBER(demos::MainWindow)
				CLASS_MEMBER_BASE(GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(demos::MainWindow*(Ptr<IValueEnumerable>), {L"ViewModel"})

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ViewModel)
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
		}
	}
}

#endif

extern void UnitTestInGuiMain();

void GuiMain()
{
#ifndef VCZH_DEBUG_NO_REFLECTION
	{
		List<WString> errors;
		auto resource = GuiResource::LoadFromXml(L"..\\GacUISrcCodepackedTest\\Resources\\XmlWindowResource.xml", errors);
		GetInstanceLoaderManager()->SetResource(L"Demo", resource);
	}

	auto manager = GetInstanceLoaderManager();
	auto key = GlobalStringKey::Get(L"demos::MainWindow");
	auto td = manager->GetTypeDescriptorForType(key);
	IGuiInstanceLoader::TypeInfo typeInfo(key, td);

	ObservableList<Ptr<demos::Data>> viewModel;
	//viewModel.Add(new demos::Data(L"Kula", L"Opera", L"CAO (Chief Algorithm Officer)"));
	//viewModel.Add(new demos::Data(L"JeffChen", L"Tencent", L"Principal Fisher"));
	//viewModel.Add(new demos::Data(L"MiliMeow", L"Microsoft", L"Software Engineer"));
	//viewModel.Add(new demos::Data(L"Skogkatt", L"360", L"Sponsor"));

	demos::MainWindow window(viewModel.GetWrapper());
	auto loader = GetInstanceLoaderManager()->GetLoader(key);
	auto scope = loader->InitializeInstance(typeInfo, Value::From(&window));
	CHECK_ERROR(scope->errors.Count() == 0, L"");

	window.ForceCalculateSizeImmediately();
	window.MoveToScreenCenter();
	GetApplication()->Run(&window);
#endif
}