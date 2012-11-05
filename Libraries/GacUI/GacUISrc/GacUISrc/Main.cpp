#include "..\..\Source\GacUI.h"
#include "..\..\Source\NativeWindow\Windows\Direct2D\WinDirect2DApplication.h"
#include "..\..\Source\NativeWindow\Windows\GDI\WinGDIApplication.h"
#include "..\..\..\..\Common\Source\Stream\FileStream.h"
#include "..\..\..\..\Common\Source\Stream\CharFormat.h"
#include "..\..\..\..\Common\Source\Stream\Accessor.h"

using namespace vl::collections;
using namespace vl::stream;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsGDIRenderer();
}

namespace test
{
	class TestWindow : public GuiWindow
	{
	protected:
		void BuildRichTextContent()
		{
			FileStream fileStream(L"..\\GacUISrcCodepackedTest\\Resources\\document.txt", FileStream::ReadOnly);
			BomDecoder decoder;
			DecoderStream decoderStream(fileStream, decoder);
			StreamReader reader(decoderStream);
			List<WString> lines;
			while(!reader.IsEnd())
			{
				lines.Add(reader.ReadLine());
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