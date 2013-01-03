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
	class GrammarParser : public Object
	{
	public:

	protected:

	};

	class GrammarColorizer : public GuiTextBoxRegexColorizer
	{
	protected:
		Ptr<ParsingRestrictParser>				grammarParser;
		volatile bool							finalizing;

		SpinLock								parsingTreeLock;
		Ptr<ParsingTreeNode>					parsingTreeNode;

		SpinLock								parsingTextLock;
		WString									parsingText;
		volatile bool							isParsingRunning;
		SpinLock								parsingRunningEvent;

		void ParsingProcInternal()
		{
			while(true)
			{
				WString currentParsingText;
				{
					SpinLock::Scope scope(parsingTextLock);
					currentParsingText=parsingText;
					parsingText=L"";
				}
				if(currentParsingText==L"")
				{
					isParsingRunning=false;
					break;
				}

				ParsingError error;
				Ptr<ParsingTreeNode> node=grammarParser->Parse(currentParsingText, L"ParserDecl", error);
				if(node)
				{
					node->InitializeQueryCache();
				}
				{
					SpinLock::Scope scope(parsingTreeLock);
					parsingTreeNode=node;
					node=0;
				}
				RestartColorizer();
			}
			parsingRunningEvent.Leave();
		}

		static void ParsingProc(void* argument)
		{
			((GrammarColorizer*)argument)->ParsingProcInternal();
		}

		void InitializeColorizer()
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

		void InitializeParser()
		{
			grammarParser=CreateBootstrapParser();
		}
	public:
		GrammarColorizer()
			:isParsingRunning(false)
			,finalizing(false)
		{
			InitializeColorizer();
			InitializeParser();
		}

		~GrammarColorizer()
		{
			finalizing=true;
			parsingRunningEvent.Enter();
			parsingRunningEvent.Leave();
		}

		void SubmitCurrentText(const wchar_t* text)
		{
			{
				// copy the text because this is a cross thread accessible data
				SpinLock::Scope scope(parsingTextLock);
				parsingText=text;
			}
			if(!isParsingRunning)
			{
				isParsingRunning=true;
				parsingRunningEvent.Enter();
				ThreadPoolLite::Queue(&ParsingProc, this);
			}
		}
	};

	class TestWindow : public GuiWindow
	{
	protected:
		GuiMultilineTextBox*					textBoxGrammar;
		Ptr<GrammarColorizer>					colorizer;

		void textBoxGrammar_TextChanged(GuiGraphicsComposition* composition, GuiEventArgs& arguments)
		{
			WString text=textBoxGrammar->GetText();
			colorizer->SubmitCurrentText(text.Buffer());
		}
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
			colorizer=new GrammarColorizer;
			textBoxGrammar->SetColorizer(colorizer);
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
	};
}
using namespace test;

void GuiMain()
{
	TestWindow window;
	GetApplication()->Run(&window);
}