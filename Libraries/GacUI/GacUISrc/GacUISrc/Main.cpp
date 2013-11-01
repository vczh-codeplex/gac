#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "..\..\Source\GacUI.h"
#include "..\..\Source\Reflection\GuiInstanceLoader.h"
#include <Windows.h>

using namespace vl::stream;
using namespace vl::parsing::tabling;
using namespace vl::parsing;
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
	GuiDocumentViewer*					viewer;

public:
	TestWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		SetText(L"Editor.Colorizer.Xml");
		SetClientSize(Size(640, 480));

		viewer=g::NewDocumentViewer();
		viewer->SetVerticalAlwaysVisible(false);
		viewer->SetHorizontalAlwaysVisible(false);
		viewer->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		this->GetBoundsComposition()->AddChild(viewer->GetBoundsComposition());

		Ptr<GuiResource> resource=GuiResource::LoadFromXml(L"..\\GacUISrcCodepackedTest\\Resources\\XmlWindowResource.xml");
		Ptr<DocumentModel> document=resource->GetDocumentByPath(L"XmlDoc.xml");
		viewer->SetDocument(document);

		// set the preferred minimum client 600
		this->GetBoundsComposition()->SetPreferredMinSize(Size(640, 480));
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
		description::LogTypeManager(writer);
	}
#endif
	UnitTestInGuiMain();
	//TestWindow window;
	//GetApplication()->Run(&window);
	
	Ptr<GuiResource> resource=GuiResource::LoadFromXml(L"..\\GacUISrcCodepackedTest\\Resources\\XmlWindowResource.xml");
	Ptr<GuiInstanceContext> context=resource->GetValueByPath(L"Controls/DocumentWindowResource").Cast<GuiInstanceContext>();
	if(context)
	{
		Value windowInstance=LoadInstance(context, new GuiResourcePathResolver(resource, resource->GetWorkingDirectory()));
		GuiWindow* window=dynamic_cast<GuiWindow*>(windowInstance.GetRawPtr());
		if(window)
		{
			window->ForceCalculateSizeImmediately();
			window->MoveToScreenCenter();
			GetApplication()->Run(window);
		}
		windowInstance.DeleteRawPtr();
	}
}