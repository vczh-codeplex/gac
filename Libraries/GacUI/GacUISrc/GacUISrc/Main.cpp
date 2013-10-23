#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "..\..\Source\GacUI.h"
#include <Windows.h>

using namespace vl::stream;
using namespace vl::parsing::tabling;
using namespace vl::parsing;

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
XmlGrammarColorizer
***********************************************************************/

class XmlGrammarColorizer : public GuiGrammarColorizer
{
public:
	XmlGrammarColorizer()
		:GuiGrammarColorizer(CreateAutoRecoverParser(xml::XmlLoadTable()), L"XDocument")
	{
		SetColor(L"Boundary", Color(0, 0, 255));
		SetColor(L"Comment", Color(0, 128, 0));
		SetColor(L"TagName", Color(163, 21, 21));
		SetColor(L"AttName", Color(255, 0, 0));
		SetColor(L"AttValue", Color(128, 0, 255));
		EndSetColors();
	}
};

/***********************************************************************
TextBoxColorizerWindow
***********************************************************************/

class TextBoxColorizerWindow : public GuiWindow
{
protected:
	GuiMultilineTextBox*					textBoxEditor;

public:
	TextBoxColorizerWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		SetText(L"Editor.Colorizer.Xml");
		SetClientSize(Size(800, 600));

		textBoxEditor=g::NewMultilineTextBox();
		textBoxEditor->SetVerticalAlwaysVisible(false);
		textBoxEditor->SetHorizontalAlwaysVisible(false);
		textBoxEditor->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		textBoxEditor->SetColorizer(new XmlGrammarColorizer);
		this->GetBoundsComposition()->AddChild(textBoxEditor->GetBoundsComposition());

		LoadTextFile(L"..\\GacUISrcCodepackedTest\\Resources\\XmlResource.xml", text);
		textBoxEditor->SetText(text);
		textBoxEditor->Select(TextPos(), TextPos());

		// set the preferred minimum client 600
		this->GetBoundsComposition()->SetPreferredMinSize(Size(800, 600));
		// call this to calculate the size immediately if any indirect content in the table changes
		// so that the window can calcaulte its correct size before calling the MoveToScreenCenter()
		this->ForceCalculateSizeImmediately();
		// move to the screen center
		this->MoveToScreenCenter();
	}

	~TextBoxColorizerWindow()
	{
	}
};

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
	TextBoxColorizerWindow window;
	GetApplication()->Run(&window);
}