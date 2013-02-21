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

		void tooltipLabel_ActiveHyperlinkExecuted(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			GuiDocumentLabel* label=dynamic_cast<GuiDocumentLabel*>(sender->GetRelatedControl());
			GetCurrentController()->DialogService()->ShowMessageBox(
				GetNativeWindow(),
				label->GetActiveHyperlinkReference(),
				GetText());
		}

		void AddTooltip(GuiControl* control, Ptr<GuiResource> resource, const WString& path, vint width)
		{
			GuiDocumentLabel* tooltipLabel=g::NewDocumentLabel();
			tooltipLabel->ActiveHyperlinkExecuted.AttachMethod(this, &TestWindow::tooltipLabel_ActiveHyperlinkExecuted);
			control->SetTooltipControl(tooltipLabel);
			control->SetTooltipWidth(width);

			Ptr<DocumentModel> tooltipDocument=resource->GetValueByPath(path).Cast<DocumentModel>();
			tooltipLabel->SetDocument(tooltipDocument);
		}
	public:
		TestWindow()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		{
			SetText(GetApplication()->GetExecutableFolder());
			SetClientSize(Size(440, 280));
			GetContainerComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			MoveToScreenCenter();
			Ptr<GuiResource> resource=GuiResource::LoadFromXml(L"..\\GacUISrcCodepackedTest\\Resources\\XmlResource.xml");

			{
				GuiButton* tooltipButton=g::NewButton();
				tooltipButton->SetText(L"This is a button with tooltip.");
				tooltipButton->GetBoundsComposition()->SetAlignmentToParent(Margin(100, 100, 100, 100));
				AddChild(tooltipButton);
				AddTooltip(tooltipButton, resource, L"TooltipDocument", 200);
			}

			const wchar_t* images[]=
			{
				L"Images/_New.png",
				L"Images/_Open.png",
				L"Images/_Save.png",
				L"Images/_SaveAs.png",
			};
			const wchar_t* paths[]=
			{
				L"Tooltips/New",
				L"Tooltips/Open",
				L"Tooltips/Save",
				L"Tooltips/SaveAs",
			};

			GuiToolstripToolbar* toolbar=g::NewToolbar();
			toolbar->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, -1));
			AddChild(toolbar);
			for(vint i=0;i<4;i++)
			{
				GuiToolstripButton* button=g::NewToolbarButton();
				button->SetImage(resource->GetValueByPath(images[i]).Cast<GuiImageData>());
				AddTooltip(button, resource, paths[i], 100);
				toolbar->GetToolstripItems().Add(button);
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