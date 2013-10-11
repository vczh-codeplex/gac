#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "FileSystemInformation.h"
#include "..\..\Source\GraphicsElement\WindowsDirect2D\GuiGraphicsWindowsDirect2D.h"
#include "..\..\Source\Reflection\GuiReflectionBasic.h"
#include "..\..\..\..\Common\Source\Parsing\Xml\ParsingXml.h"
#include "..\..\..\..\Common\Source\Stream\MemoryStream.h"

using namespace vl::stream;
using namespace vl::collections;
using namespace vl::regex;
using namespace vl::parsing;
using namespace vl::parsing::tabling;
using namespace vl::parsing::xml;

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

class RichTextWindow : public GuiWindow
{
protected:
	GuiButton*						buttonBold;
	GuiButton*						buttonUnbold;
	GuiDocumentViewer*				viewer;

public:
	RichTextWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		SetText(L"GacUISrc Test Application");
		SetClientSize(Size(640, 480));

		GuiTableComposition* table=new GuiTableComposition;
		table->SetAlignmentToParent(Margin(0, 0, 0, 0));
		table->SetCellPadding(5);
		table->SetRowsAndColumns(2, 3);
		table->SetRowOption(0, GuiCellOption::MinSizeOption());
		table->SetRowOption(1, GuiCellOption::PercentageOption(1.0));
		table->SetColumnOption(0, GuiCellOption::MinSizeOption());
		table->SetColumnOption(1, GuiCellOption::MinSizeOption());
		table->SetColumnOption(2, GuiCellOption::PercentageOption(1.0));
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(0, 0, 1, 1);

			buttonBold=g::NewButton();
			buttonBold->SetText(L"Bold");
			buttonBold->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			buttonBold->Clicked.AttachLambda([this](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
			{
				TextPos begin=viewer->GetCaretBegin();
				TextPos end=viewer->GetCaretEnd();
				Ptr<DocumentStyleProperties> style=new DocumentStyleProperties;
				style->bold=true;
				viewer->EditStyle(begin, end, style);
			});
			cell->AddChild(buttonBold->GetBoundsComposition());
		}
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(0, 1, 1, 1);

			buttonUnbold=g::NewButton();
			buttonUnbold->SetText(L"Unbold");
			buttonUnbold->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			buttonUnbold->Clicked.AttachLambda([this](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
			{
				TextPos begin=viewer->GetCaretBegin();
				TextPos end=viewer->GetCaretEnd();
				Ptr<DocumentStyleProperties> style=new DocumentStyleProperties;
				style->bold=false;
				viewer->EditStyle(begin, end, style);
			});
			cell->AddChild(buttonUnbold->GetBoundsComposition());
		}
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(1, 0, 1, 3);

			viewer=g::NewDocumentViewer();
			viewer->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			viewer->SetEditMode(GuiDocumentViewer::Editable);
			cell->AddChild(viewer->GetBoundsComposition());
		}
		GetContainerComposition()->AddChild(table);

		{
			Ptr<GuiResource> resource=GuiResource::LoadFromXml(L"..\\GacUISrcCodepackedTest\\Resources\\XmlResource.xml");
			Ptr<DocumentModel> document=resource->GetValueByPath(L"XmlDoc.xml").Cast<DocumentModel>();
			viewer->SetDocument(document);
		}

		// set the preferred minimum client 600
		this->GetBoundsComposition()->SetPreferredMinSize(Size(640, 480));
		// call this to calculate the size immediately if any indirect content in the table changes
		// so that the window can calcaulte its correct size before calling the MoveToScreenCenter()
		this->ForceCalculateSizeImmediately();
		// move to the screen center
		this->MoveToScreenCenter();
	}

	~RichTextWindow()
	{
	}
};

extern void UnitTestInGuiMain();

void GuiMain()
{
	//{
	//	FileStream fileStream(L"Reflection.txt", FileStream::WriteOnly);
	//	BomEncoder encoder(BomEncoder::Utf16);
	//	EncoderStream encoderStream(fileStream, encoder);
	//	StreamWriter writer(encoderStream);
	//	description::LogTypeManager(writer);
	//}
	UnitTestInGuiMain();
	RichTextWindow window;
	GetApplication()->Run(&window);
}