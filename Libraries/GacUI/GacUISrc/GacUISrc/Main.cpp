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

	GuiWindow window(GetCurrentTheme()->CreateWindowStyle());
	auto loader = GetInstanceLoaderManager()->GetLoader(key);
	auto scope = loader->InitializeInstance(typeInfo, Value::From(&window));
	CHECK_ERROR(scope->errors.Count() == 0, L"");

	window.ForceCalculateSizeImmediately();
	window.MoveToScreenCenter();
	GetApplication()->Run(&window);
#endif
}