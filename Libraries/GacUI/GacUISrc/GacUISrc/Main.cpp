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
	int result = SetupWindowsDirect2DRenderer();
#endif

#if _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return result;
}

extern void UnitTestInGuiMain();

template<typename TImpl>
class SignInWindow_ : public GuiWindow, public GuiInstancePartialClass<GuiWindow>
{
protected:
	GuiDocumentLabel*				documentLabelTop;
	GuiDocumentLabel*				documentLabelBottom;
	GuiSinglelineTextBox*			textBoxUserName;
	GuiSinglelineTextBox*			textBoxPassword;
	GuiButton*						buttonLogin;

	void InitializeComponents()
	{
		if (InitializeFromResource(L"SignInWindow/MainWindowResource"))
		{
			GUI_INSTANCE_REFERENCE(documentLabelTop);
			GUI_INSTANCE_REFERENCE(documentLabelBottom);
			GUI_INSTANCE_REFERENCE(textBoxUserName);
			GUI_INSTANCE_REFERENCE(textBoxPassword);
			GUI_INSTANCE_REFERENCE(buttonLogin);

			TImpl* impl = dynamic_cast <TImpl*>(this);
			buttonLogin->Clicked.AttachMethod(impl, TImpl::buttonLogin_Clicked);
		}
	}
public:
	SignInWindow_(Ptr<GuiResource> resource)
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		, GuiInstancePartialClass<GuiWindow>(resource)
		, documentLabelTop(0)
		, documentLabelBottom(0)
		, textBoxUserName(0)
		, textBoxPassword(0)
		, buttonLogin(0)
	{
	}
};

class SignInWindow : public SignInWindow_<SignInWindow>
{
	friend class SignInWindow_<SignInWindow>;
protected:

	void buttonLogin_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
	}
public:
	SignInWindow(Ptr<GuiResource> resource)
		:SignInWindow_<SignInWindow>(resource)
	{
	}
};

/*
Data Binding:
GuiSelectableListControl
GuiVirtualTextList
GuiVirtualListView
GuiVirtualTreeView
GuiVirtualDataGrid

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
#endif
	UnitTestInGuiMain();

	auto resource = GuiResource::LoadFromXml(L"..\\GacUISrcCodepackedTest\\Resources\\XmlWindowResource.xml");
	SignInWindow window(resource);
	window.ForceCalculateSizeImmediately();
	window.MoveToScreenCenter();
	GetApplication()->Run(&window);
}