#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "..\..\Source\GacUI.h"
#include "..\..\..\..\Common\Source\Stream\FileStream.h"
#include "..\..\..\..\Common\Source\Stream\CharFormat.h"
#include "..\..\..\..\Common\Source\Stream\Accessor.h"
#include <Windows.h>

using namespace vl::parsing::xml;
using namespace vl::stream;

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

namespace test
{
	class TestWindow : public GuiWindow
	{
	private:
		GuiDocumentLabel*				documentControl;

		void documentViewer_ActiveHyperlinkExecuted(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			GetCurrentController()->DialogService()->ShowMessageBox(
				GetNativeWindow(),
				documentControl->GetActiveHyperlinkReference(),
				GetText());
		}
	public:
		TestWindow()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		{
			SetText(GetApplication()->GetExecutableFolder());
			SetClientSize(Size(640, 520));
			GetBoundsComposition()->SetPreferredMinSize(Size(320, 240));
			GetContainerComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			MoveToScreenCenter();

			documentControl=g::NewDocumentLabel();
			documentControl->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			documentControl->ActiveHyperlinkExecuted.AttachMethod(this, &TestWindow::documentViewer_ActiveHyperlinkExecuted);
			AddChild(documentControl);

			Ptr<GuiResource> resource=GuiResource::LoadFromXml(L"..\\GacUISrcCodepackedTest\\Resources\\XmlResource.xml");
			Ptr<DocumentModel> document=resource->GetValueByPath(L"XmlDoc.xml").Cast<DocumentModel>();
			{
				Ptr<XmlDocument> xml=document->SaveToXml();
				FileStream fileStream(L"XmlDoc.xml", FileStream::WriteOnly);
				BomEncoder encoder(BomEncoder::Utf8);
				EncoderStream encoderStream(fileStream, encoder);
				StreamWriter writer(encoderStream);
				XmlPrint(xml, writer);
			}
			documentControl->SetDocument(document);
		}
	};
}
using namespace test;

void GuiMain()
{
	TestWindow window;
	GetApplication()->Run(&window);
}