#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "..\..\Source\GacUI.h"
#include "..\..\..\..\Common\Source\Stream\FileStream.h"
#include "..\..\..\..\Common\Source\Stream\Accessor.h"
#include "..\..\..\..\Common\Source\Stream\CharFormat.h"
#include "..\..\..\..\Common\Source\Parsing\ParsingTable.h"
#include <Windows.h>

using namespace vl::collections;
using namespace vl::stream;
using namespace vl::regex;
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
}

namespace test
{
	class GrammarColorizer : public GuiTextBoxRegexColorizer
	{
	public:
		GrammarColorizer()
		{
			text::ColorEntry entry=GetCurrentTheme()->GetDefaultTextBoxColorEntry();
			SetDefaultColor(entry);

			entry.normal.text=Color(163, 21, 21);
			AddToken(L"\"([^\\\\\"]|\\\\/.)*\"", entry);

			entry.normal.text=Color(0, 0, 255);
			AddToken(L"class|enum|token|discardtoken|rule|as|with", entry);

			Setup();
		}
	};

	class TestWindow : public GuiWindow
	{
	protected:
		GuiMultilineTextBox*				textBoxGrammar;
	public:
		TestWindow()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		{
			SetText(L"GacUISrc Test Application");
			SetClientSize(Size(640, 480));
			GetBoundsComposition()->SetPreferredMinSize(Size(320, 240));
			MoveToScreenCenter();

			textBoxGrammar=g::NewMultilineTextBox();
			textBoxGrammar->GetBoundsComposition()->SetAlignmentToParent(Margin(3, 3, 3, 3));
			GetBoundsComposition()->AddChild(textBoxGrammar->GetBoundsComposition());
			textBoxGrammar->SetColorizer(new GrammarColorizer);

			{
				FileStream fileStream(L"..\\GacUISrcCodepackedTest\\Resources\\CalculatorDefinition.txt", FileStream::ReadOnly);
				BomDecoder decoder;
				DecoderStream decoderStream(fileStream, decoder);
				StreamReader reader(decoderStream);
				textBoxGrammar->SetText(reader.ReadToEnd());
				textBoxGrammar->Select(TextPos(), TextPos());
			}
		}
	};
}
using namespace test;

void GuiMain()
{
	TestWindow window;
	GetApplication()->Run(&window);
}