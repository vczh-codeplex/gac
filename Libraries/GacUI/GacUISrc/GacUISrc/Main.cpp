#include "..\..\Source\GacUI.h"
#include "..\..\Source\NativeWindow\Windows\Direct2D\WinDirect2DApplication.h"
#include "..\..\Source\NativeWindow\Windows\GDI\WinGDIApplication.h"
#include "..\..\..\..\Common\Source\Stream\FileStream.h"
#include "..\..\..\..\Common\Source\Stream\CharFormat.h"
#include "..\..\..\..\Common\Source\Stream\Accessor.h"
#include "..\..\..\..\Common\Source\Regex\Regex.h"

using namespace vl::collections;
using namespace vl::stream;
using namespace vl::regex;
using namespace vl::presentation::windows;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsGDIRenderer();
}

namespace test
{

/***********************************************************************
DocumentFragment
***********************************************************************/

	class DocumentFragment : public Object
	{
	public:
		bool				paragraph;
		WString				font;
		bool				bold;
		Color				color;
		int					size;
		WString				text;

		DocumentFragment()
			:paragraph(false)
			,bold(false)
			,size(0)
		{
		}

		DocumentFragment(Ptr<DocumentFragment> prototype, const WString& _text)
			:paragraph(prototype->paragraph)
			,font(prototype->font)
			,bold(prototype->bold)
			,color(prototype->color)
			,size(prototype->size)
			,text(_text)
		{
		}
	};

	int ConvertHex(wchar_t c)
	{
		if(L'a'<=c && c<=L'f') return c-L'a'+10;
		if(L'A'<=c && c<=L'F') return c-L'A'+10;
		if(L'0'<=c && c<=L'9') return c-L'0';
		return 0;
	}

	Color ConvertColor(const WString& colorString)
	{
		return Color(
			ConvertHex(colorString[1])*16+ConvertHex(colorString[2]),
			ConvertHex(colorString[3])*16+ConvertHex(colorString[4]),
			ConvertHex(colorString[5])*16+ConvertHex(colorString[6])
			);
	}

	void BuildDocumentFragments(const WString& fileName, List<Ptr<DocumentFragment>>& fragments)
	{
		fragments.Clear();
		WString rawDocument;
		{
			FileStream fileStream(fileName, FileStream::ReadOnly);
			Utf8Decoder decoder;
			DecoderStream decoderStream(fileStream, decoder);
			StreamReader reader(decoderStream);
			rawDocument=reader.ReadToEnd();
		}

		Regex regex(L"<(<tag>s)>(<font>[^:]+):(<bold>[^:]+):(<color>[^:]+):(<size>[^:]+):(<text>/.*?)<//s>|<(<tag>p)//>");
		RegexMatch::List matches;
		regex.Search(rawDocument, matches);
		
		for(int i=0;i<matches.Count();i++)
		{
			Ptr<RegexMatch> match=matches[i];
			Ptr<DocumentFragment> fragment=new DocumentFragment;
			fragments.Add(fragment);
			if(match->Groups()[L"tag"][0].Value()==L"p")
			{
				fragment->paragraph=true;
			}
			else
			{
				WString font=match->Groups()[L"tag"][0].Value();
				WString bold=match->Groups()[L"bold"][0].Value();
				WString color=match->Groups()[L"color"][0].Value();
				WString size=match->Groups()[L"size"][0].Value();
				WString text=match->Groups()[L"text"][0].Value();

				fragment->font=font;
				fragment->bold=bold==L"true";
				fragment->size=wtoi(size);
				fragment->color=ConvertColor(color);
				fragment->text=text;
			}
		}
	}

/***********************************************************************
ScriptFragment
***********************************************************************/

	class ScriptLine
	{
	public:
		List<Ptr<DocumentFragment>>		documentFragments;

		void BuildUniscribeData(WinDC* dc)
		{
		}
	};

	class ScriptParagraph
	{
	public:
		List<Ptr<ScriptLine>>			lines;
	};

	void BuildScriptParagraphs(List<Ptr<DocumentFragment>>& fragments, List<Ptr<ScriptParagraph>>& script)
	{
		script.Clear();
		Regex regex(L"\r\n");
		Ptr<ScriptParagraph> currentParagraph;
		Ptr<ScriptLine> currentLine;

		FOREACH(Ptr<DocumentFragment>, fragment, fragments.Wrap())
		{
			if(!currentParagraph)
			{
				currentParagraph=new ScriptParagraph;
				script.Add(currentParagraph);
			}
			
			if(fragment->paragraph)
			{
				currentParagraph=0;
				currentLine=0;
			}
			else
			{
				RegexMatch::List matches;
				regex.Split(fragment->text, true, matches);
				for(int i=0;i<matches.Count();i++)
				{
					Ptr<RegexMatch> match=matches[i];
					if(i>0)
					{
						currentLine=0;
					}
					if(!currentLine)
					{
						currentLine=new ScriptLine;
						currentParagraph->lines.Add(currentLine);
					}
					currentLine->documentFragments.Add(new DocumentFragment(fragment, match->Result().Value()));
				}
			}
		}

		HDC hdc=CreateCompatibleDC(NULL);
		WinProxyDC dc;
		dc.Initialize(hdc);
		FOREACH(Ptr<ScriptParagraph>, paragraph, script.Wrap())
		{
			FOREACH(Ptr<ScriptLine>, line, paragraph->lines.Wrap())
			{
				line->BuildUniscribeData(&dc);
			}
		}
		DeleteDC(hdc);
	}

/***********************************************************************
TestWindow
***********************************************************************/

	class TestWindow : public GuiWindow
	{
	protected:
		List<Ptr<ScriptParagraph>>		script;
	public:
		TestWindow()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		{
			SetText(L"GacUISrc Test Application");
			SetClientSize(Size(640, 480));
			GetBoundsComposition()->SetPreferredMinSize(Size(640, 480));
			MoveToScreenCenter();

			GetApplication()->InvokeAsync([=]()
			{
				GetApplication()->InvokeInMainThreadAndWait([=]()
				{
					SetText(L"GuiUISrc Test Application (executing BuildDocumentFragments() ...)");
				});
				List<Ptr<DocumentFragment>> fragments;
				BuildDocumentFragments(L"..\\GacUISrcCodepackedTest\\Resources\\document.txt", fragments);
				GetApplication()->InvokeInMainThreadAndWait([=]()
				{
					SetText(L"GuiUISrc Test Application (executing BuildScriptParagraphs() ...)");
				});
				BuildScriptParagraphs(fragments, script);
				GetApplication()->InvokeInMainThreadAndWait([=]()
				{
					SetText(L"GacUISrc Test Application (ready)");
				});
			});
		}
	};
}
using namespace test;

void GuiMain()
{
	TestWindow window;
	GetApplication()->Run(&window);
}