#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "..\..\Source\GacUI.h"
#include "..\..\Source\Reflection\GuiReflectionEvents.h"
#include "..\..\..\..\Common\Source\Stream\FileStream.h"
#include "..\..\..\..\Common\Source\Stream\CharFormat.h"
#include "..\..\..\..\Common\Source\Stream\Accessor.h"
#include <Windows.h>

using namespace vl::parsing::xml;
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

namespace test
{
	class TestWindow : public GuiWindow
	{
	private:
	public:
		TestWindow()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		{
			SetText(GetApplication()->GetExecutableFolder());
			SetClientSize(Size(440, 280));
			GetContainerComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			MoveToScreenCenter();
		}

		~TestWindow()
		{
		}
	};

	void LogReflection()
	{
		FileStream fileStream(L"Reflection.txt", FileStream::WriteOnly);
		BomEncoder encoder(BomEncoder::Utf16);
		EncoderStream encoderStream(fileStream, encoder);
		StreamWriter writer(encoderStream);
		LogTypeManager(writer);
	}
}
using namespace test;

void GuiMain()
{
	LogReflection();
#ifndef VCZH_DEBUG_NO_REFLECTION
	Value currentTheme=Value::InvokeStatic(L"presentation::theme::ITheme", L"GetCurrentTheme");
	Value windowStyle=currentTheme.Invoke(L"CreateWindowStyle");
	Value window=Value::Create(L"presentation::controls::GuiWindow", (Value::xs(), windowStyle));

	Value clientSize=Value::From(L"x:320 y:240", GetTypeDescriptor(L"presentation::Size"));
	window.SetProperty(L"Text", WString(L"Window By Reflection!"));
	window.SetProperty(L"ClientSize", clientSize);
	window.GetProperty(L"ContainerComposition").SetProperty(L"PreferredMinSize", clientSize);
	window.Invoke(L"MoveToScreenCenter");

	Value buttonStyle=currentTheme.Invoke(L"CreateButtonStyle");
	Value button=Value::Create(L"presentation::controls::GuiButton", (Value::xs(), buttonStyle));
	button.GetProperty(L"BoundsComposition").SetProperty(L"AlignmentToParent", Value::From(L"left:60 top:60 right:60 bottom:60", GetTypeDescriptor(L"presentation::Margin")));
	button.SetProperty(L"Text", WString(L"Click Me!"));
	window.Invoke(L"AddChild", (Value::xs(), button));

	Value handler=BoxParameter<Func<void(GuiGraphicsComposition*, GuiEventArgs*)>>(
		LAMBDA([&button](GuiGraphicsComposition*, GuiEventArgs*)
		{
			button.SetProperty(L"Text", WString(L"You clicked the button!"));
		}));
	button.AttachEvent(L"Clicked", handler);

	Value application=Value::InvokeStatic(L"presentation::controls::GuiApplication", L"GetApplication");
	application.Invoke(L"Run", (Value::xs(), window));
	window.DeleteRawPtr();
#endif
}