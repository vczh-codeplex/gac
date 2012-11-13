#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "..\..\Source\GacUI.h"
#include "..\..\..\..\Common\Source\Stream\FileStream.h"
#include "..\..\..\..\Common\Source\Stream\MemoryStream.h"
#include "..\..\..\..\Common\Source\Stream\CharFormat.h"
#include "..\..\..\..\Common\Source\Stream\Accessor.h"
#include "..\..\..\..\Common\Source\Regex\Regex.h"

#include <Windows.h>

using namespace vl::collections;
using namespace vl::stream;
using namespace vl::regex;

#define GUI_GRAPHICS_RENDERER_GDI

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


/***********************************************************************
DocumentLoader
***********************************************************************/

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

	Ptr<text::DocumentModel> BuildDocumentModel(const WString& fileName)
	{
		HDC dc=CreateCompatibleDC(NULL);
		int dpi=GetDeviceCaps(dc, LOGPIXELSY);
		DeleteDC(dc);
		Ptr<text::DocumentModel> document=new text::DocumentModel;

		WString rawDocument;
		{
			FileStream fileStream(fileName, FileStream::ReadOnly);
			BomDecoder decoder;
			DecoderStream decoderStream(fileStream, decoder);
			StreamReader reader(decoderStream);
			rawDocument=reader.ReadToEnd();
		}

		Regex regexTag(L"<(<tag>s)>(<font>[^:]+):(<bold>[^:]+):(<color>[^:]+):(<size>[^:]+):(<text>/.*?)<//s>|<(<tag>p)//>");
		Regex regexLine(L"\r\n");
		RegexMatch::List matches;
		regexTag.Search(rawDocument, matches);

		Ptr<text::DocumentParagraph> paragraph=0;
		Ptr<text::DocumentLine> line=0;
		
		for(int i=0;i<matches.Count();i++)
		{
			Ptr<RegexMatch> match=matches[i];
			if(match->Groups()[L"tag"][0].Value()==L"p")
			{
				paragraph=0;
				line=0;
			}
			else
			{
				FontProperties fontStyle;
				Color fontColor;
				RegexMatch::List lines;
				{
					WString font=match->Groups()[L"font"][0].Value();
					WString bold=match->Groups()[L"bold"][0].Value();
					WString color=match->Groups()[L"color"][0].Value();
					WString size=match->Groups()[L"size"][0].Value();
					WString text=match->Groups()[L"text"][0].Value();

					fontStyle.fontFamily=font;
					fontStyle.bold=bold==L"true";
					fontStyle.size=(int)(wtof(size)*dpi/72);
					fontColor=ConvertColor(color);
					regexLine.Split(text, true, lines);
				}

				for(int j=0;j<lines.Count();j++)
				{
					WString lineText=lines[j]->Result().Value();
					if(!paragraph)
					{
						paragraph=new text::DocumentParagraph;
						document->paragraphs.Add(paragraph);
					}
					if(!line || j>0)
					{
						line=new text::DocumentLine;
						paragraph->lines.Add(line);
					}

					Ptr<text::DocumentRun> run=new text::DocumentRun;
					run->style=fontStyle;
					run->color=fontColor;
					run->text=lineText;
					line->runs.Add(run);
				}
			}
		}

		return document;
	}

/***********************************************************************
TestWindow
***********************************************************************/

	class TestWindow : public GuiWindow
	{
	protected:
		GuiScrollContainer*				scriptDocumentView;

	public:
		TestWindow()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		{
			SetText(L"GacUISrc Test Application");
			SetClientSize(Size(640, 480));
			GetBoundsComposition()->SetPreferredMinSize(Size(320, 240));
			MoveToScreenCenter();
			{
				scriptDocumentView=new GuiScrollContainer(GetCurrentTheme()->CreateMultilineTextBoxStyle());
				scriptDocumentView->SetExtendToFullWidth(true);
				scriptDocumentView->SetHorizontalAlwaysVisible(false);
				scriptDocumentView->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				scriptDocumentView->GetBoundsComposition()->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetSystemCursor(INativeCursor::LargeWaiting));
				GetContainerComposition()->AddChild(scriptDocumentView->GetBoundsComposition());
			}
			GetApplication()->InvokeAsync([=]()
			{
				Ptr<text::DocumentModel> document=BuildDocumentModel(L"..\\GacUISrcCodepackedTest\\Resources\\document2.txt");
				GetApplication()->InvokeInMainThreadAndWait([=]()
				{
					scriptDocumentView->GetBoundsComposition()->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetDefaultSystemCursor());
					GuiDocumentElement* element=GuiDocumentElement::Create();
					element->SetDocument(document);

					GuiBoundsComposition* composition=new GuiBoundsComposition;
					composition->SetOwnedElement(element);
					composition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
					composition->SetAlignmentToParent(Margin(10, 10, 10, 10));
					scriptDocumentView->GetContainerComposition()->AddChild(composition);
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