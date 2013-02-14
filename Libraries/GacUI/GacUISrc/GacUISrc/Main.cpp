#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "..\..\Source\GacUI.h"
#include <Windows.h>

#define GUI_GRAPHICS_RENDERER_DIRECT2D

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
#ifdef GUI_GRAPHICS_RENDERER_GDI
	int result=SetupWindowsGDIRenderer();
#endif
#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D
	int result=SetupWindowsDirect2DRenderer();
#endif
	_CrtDumpMemoryLeaks();
	return result;
}

namespace test
{
	class TestWindow : public GuiWindow
	{
	private:
		GuiDocumentViewer*				documentViewer;

	public:
		TestWindow()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		{
			SetText(GetApplication()->GetExecutableFolder());
			SetClientSize(Size(640, 480));
			GetBoundsComposition()->SetPreferredMinSize(Size(320, 240));
			MoveToScreenCenter();

			documentViewer=g::NewDocumentViewer();
			documentViewer->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			AddChild(documentViewer);

			Ptr<GuiResource> resource=GuiResource::LoadFromXml(L"..\\GacUISrcCodepackedTest\\Resources\\XmlResource.xml");
			Ptr<DocumentModel> document=resource->GetValueByPath(L"XmlDoc.xml").Cast<DocumentModel>();
			documentViewer->SetDocument(document);
		}
	};
}
using namespace test;

void GuiMain()
{
	TestWindow window;
	GetApplication()->Run(&window);
}