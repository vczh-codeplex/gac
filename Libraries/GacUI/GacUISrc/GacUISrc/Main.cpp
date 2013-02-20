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

		Point						tooltipLocation;
		Ptr<GuiTooltip>				tooltip;

		void tooltipLabel_ActiveHyperlinkExecuted(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			GetCurrentController()->DialogService()->ShowMessageBox(
				GetNativeWindow(),
				tooltipLabel->GetActiveHyperlinkReference(),
				GetText());
		}

		void tooltipButton_MouseMove(GuiGraphicsComposition* sender, GuiMouseEventArgs& arguments)
		{
			tooltipLocation.x=arguments.x+5;
			tooltipLocation.y=arguments.y+5;
		}

		void tooltipButton_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			tooltip->SetClientSize(Size(0, 0));
			tooltip->ShowPopup(tooltipButton, tooltipLocation);
		}
	public:
		TestWindow()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		{
			SetText(GetApplication()->GetExecutableFolder());
			SetClientSize(Size(640, 480));
			GetContainerComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			MoveToScreenCenter();

			tooltipButton=g::NewButton();
			tooltipButton->SetText(L"This is a button with tooltip.");
			tooltipButton->GetBoundsComposition()->SetAlignmentToParent(Margin(200, 200, 200, 200));
			tooltipButton->GetEventReceiver()->mouseMove.AttachMethod(this, &TestWindow::tooltipButton_MouseMove);
			tooltipButton->Clicked.AttachMethod(this, &TestWindow::tooltipButton_Clicked);
			AddChild(tooltipButton);

			tooltip=new GuiTooltip(GetCurrentTheme()->CreateTooltipStyle());
			tooltip->SetPrefferedContentWidth(200);

			tooltipLabel=g::NewDocumentLabel();
			tooltipLabel->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			tooltipLabel->ActiveHyperlinkExecuted.AttachMethod(this, &TestWindow::tooltipLabel_ActiveHyperlinkExecuted);
			tooltip->AddChild(tooltipLabel);

			Ptr<GuiResource> resource=GuiResource::LoadFromXml(L"..\\GacUISrcCodepackedTest\\Resources\\XmlResource.xml");
			tooltipDocument=resource->GetValueByPath(L"TooltipDocument").Cast<DocumentModel>();
			tooltipLabel->SetDocument(tooltipDocument);
		}
	};
}
using namespace test;

void GuiMain()
{
	TestWindow window;
	GetApplication()->Run(&window);
}