#include "..\..\Public\Source\GacUI.h"
#include <Windows.h>

using namespace vl::presentation::description;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsDirect2DRenderer();
}

void GuiMain()
{
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
	button.GetProperty(L"BoundsComposition").SetProperty(L"AlignmentToParent", Value::From(L"left:100 top:100 right:100 bottom:100", GetTypeDescriptor(L"presentation::Margin")));
	button.SetProperty(L"Text", WString(L"Click Me!"));
	window.Invoke(L"AddChild", (Value::xs(), button));

	Value application=Value::InvokeStatic(L"presentation::controls::GuiApplication", L"GetApplication");
	application.Invoke(L"Run", (Value::xs(), window));
	window.DeleteRawPtr();
}