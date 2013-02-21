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
		GuiButton*					tooltipButton;
		GuiDocumentLabel*			tooltipLabel;
		Ptr<DocumentModel>			tooltipDocument;

		void tooltipLabel_ActiveHyperlinkExecuted(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			GetCurrentController()->DialogService()->ShowMessageBox(
				GetNativeWindow(),
				tooltipLabel->GetActiveHyperlinkReference(),
				GetText());
		}
	public:
		TestWindow()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		{
			SetText(GetApplication()->GetExecutableFolder());
			SetClientSize(Size(440, 280));
			GetContainerComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			MoveToScreenCenter();

			tooltipButton=g::NewButton();
			tooltipButton->SetText(L"This is a button with tooltip.");
			tooltipButton->GetBoundsComposition()->SetAlignmentToParent(Margin(100, 100, 100, 100));
			AddChild(tooltipButton);
			{
				tooltipLabel=g::NewDocumentLabel();
				tooltipLabel->ActiveHyperlinkExecuted.AttachMethod(this, &TestWindow::tooltipLabel_ActiveHyperlinkExecuted);
				tooltipButton->SetTooltipControl(tooltipLabel);
				tooltipButton->SetTooltipWidth(200);

				Ptr<GuiResource> resource=GuiResource::LoadFromXml(L"..\\GacUISrcCodepackedTest\\Resources\\XmlResource.xml");
				tooltipDocument=resource->GetValueByPath(L"TooltipDocument").Cast<DocumentModel>();
				tooltipLabel->SetDocument(tooltipDocument);
			}
		}

		~TestWindow()
		{
		}
	};
}
using namespace test;

void GuiMain()
{
	TestWindow window;
	GetApplication()->Run(&window);
}