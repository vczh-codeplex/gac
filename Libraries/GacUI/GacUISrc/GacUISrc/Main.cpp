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

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsGDIRenderer();
}

namespace test
{
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
	};

	class TestWindow : public GuiWindow
	{
	protected:
		Array<Ptr<DocumentFragment>>			fragments;
	protected:
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

		void BuildRichTextContent()
		{
			WString rawDocument;
			{
				FileStream fileStream(L"..\\GacUISrcCodepackedTest\\Resources\\document.txt", FileStream::ReadOnly);
				Utf8Decoder decoder;
				DecoderStream decoderStream(fileStream, decoder);
				StreamReader reader(decoderStream);
				rawDocument=reader.ReadToEnd();
			}

			Regex regex(L"<(<tag>s)>(<font>[^:]+):(<bold>[^:]+):(<color>[^:]+):(<size>[^:]+):(<text>/.*?)<//s>|<(<tag>p)//>");
			RegexMatch::List matches;
			regex.Search(rawDocument, matches);

			fragments.Resize(matches.Count());
			for(int i=0;i<matches.Count();i++)
			{
				Ptr<RegexMatch> match=matches[i];
				Ptr<DocumentFragment> fragment=new DocumentFragment;
				fragments[i]=fragment;
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
	public:
		TestWindow()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		{
			SetText(L"GacUISrc Test Application");
			SetClientSize(Size(640, 480));
			GetBoundsComposition()->SetPreferredMinSize(Size(640, 480));
			MoveToScreenCenter();
			BuildRichTextContent();
		}
	};
}
using namespace test;

void GuiMain()
{
	TestWindow window;
	GetApplication()->Run(&window);
}