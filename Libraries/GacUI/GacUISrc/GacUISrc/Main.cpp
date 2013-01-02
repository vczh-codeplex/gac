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
using namespace vl::parsing::tabling;

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

			AddToken(L"[a-zA-Z_]/w*", GetDefaultColor());
			
			entry.normal.text=Color(43, 145, 175);
			AddExtraToken(entry);

			Setup();
		}
	};

	class TestWindow : public GuiWindow
	{
	protected:
		GuiMultilineTextBox*					textBoxGrammar;
		Ptr<ParsingRestrictParser>				grammarParser;

		SpinLock								parsingTreeLock;
		Ptr<ParsingTreeNode>					parsingTreeNode;

		SpinLock								parsingTextLock;
		WString									parsingText;
		volatile bool							isParsingRunning;
		SpinLock								parsingRunningEvent;

		static void ParsingProc(void* argument)
		{
			TestWindow* window=(TestWindow*)argument;
			while(true)
			{
				WString currentParsingText;
				{
					SpinLock::Scope scope(window->parsingTextLock);
					currentParsingText=window->parsingText;
					window->parsingText=L"";
				}
				if(currentParsingText==L"")
				{
					window->isParsingRunning=false;
					break;
				}

				ParsingError error;
				Ptr<ParsingTreeNode> node=window->grammarParser->Parse(currentParsingText, L"ParserDecl", error);
				if(node)
				{
					node->InitializeQueryCache();
				}
				{
					SpinLock::Scope scope(window->parsingTreeLock);
					window->parsingTreeNode=node;
					node=0;
				}

				GetApplication()->InvokeInMainThread([=]()
				{
					window->textBoxGrammar->GetColorizer()->RestartColorizer();
				});
			}
			window->parsingRunningEvent.Leave();
		}

		void textBoxGrammar_TextChanged(GuiGraphicsComposition* composition, GuiEventArgs& arguments)
		{
			{
				// copy the text because this is a cross thread accessible data
				SpinLock::Scope scope(parsingTextLock);
				parsingText=textBoxGrammar->GetText().Buffer();
			}
			if(!isParsingRunning)
			{
				isParsingRunning=true;
				parsingRunningEvent.Enter();
				ThreadPoolLite::Queue(&ParsingProc, this);
			}
		}
	public:
		TestWindow()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
			,isParsingRunning(false)
		{
			grammarParser=CreateBootstrapParser();

			SetText(L"GacUISrc Test Application");
			SetClientSize(Size(640, 480));
			GetBoundsComposition()->SetPreferredMinSize(Size(320, 240));
			MoveToScreenCenter();

			textBoxGrammar=g::NewMultilineTextBox();
			textBoxGrammar->GetBoundsComposition()->SetAlignmentToParent(Margin(3, 3, 3, 3));
			GetBoundsComposition()->AddChild(textBoxGrammar->GetBoundsComposition());
			textBoxGrammar->SetColorizer(new GrammarColorizer);
			textBoxGrammar->TextChanged.AttachMethod(this, &TestWindow::textBoxGrammar_TextChanged);

			{
				FileStream fileStream(L"..\\GacUISrcCodepackedTest\\Resources\\CalculatorDefinition.txt", FileStream::ReadOnly);
				BomDecoder decoder;
				DecoderStream decoderStream(fileStream, decoder);
				StreamReader reader(decoderStream);
				textBoxGrammar->SetText(reader.ReadToEnd());
				textBoxGrammar->Select(TextPos(), TextPos());
			}
		}

		~TestWindow()
		{
			parsingRunningEvent.Enter();
			parsingRunningEvent.Leave();
		}
	};
}
using namespace test;

void GuiMain()
{
	TestWindow window;
	GetApplication()->Run(&window);
}