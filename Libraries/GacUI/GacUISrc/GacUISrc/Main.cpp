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
	GuiComboBoxListControl*			comboEditMode;
	GuiButton*						buttonBold;
	GuiButton*						buttonUnbold;
	GuiButton*						buttonImage;
	GuiButton*						buttonHyperlink;
	GuiButton*						buttonUnhyperlink;
	Ptr<GuiImageData>				image;
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
		table->SetRowsAndColumns(2, 7);
		table->SetRowOption(0, GuiCellOption::MinSizeOption());
		table->SetRowOption(1, GuiCellOption::PercentageOption(1.0));
		table->SetColumnOption(0, GuiCellOption::AbsoluteOption(80));
		table->SetColumnOption(1, GuiCellOption::MinSizeOption());
		table->SetColumnOption(2, GuiCellOption::MinSizeOption());
		table->SetColumnOption(3, GuiCellOption::MinSizeOption());
		table->SetColumnOption(4, GuiCellOption::MinSizeOption());
		table->SetColumnOption(5, GuiCellOption::MinSizeOption());
		table->SetColumnOption(6, GuiCellOption::PercentageOption(1.0));
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(0, 0, 1, 1);

			GuiTextList* listEditMode=g::NewTextList();
			listEditMode->GetItems().Add(new list::TextItem(L"ViewOnly"));
			listEditMode->GetItems().Add(new list::TextItem(L"Selectable"));
			listEditMode->GetItems().Add(new list::TextItem(L"Editable"));

			comboEditMode=g::NewComboBox(listEditMode);
			comboEditMode->SetSelectedIndex(2);
			comboEditMode->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			comboEditMode->SelectedIndexChanged.AttachLambda([this](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
			{
				switch(comboEditMode->GetSelectedIndex())
				{
				case 0:
					viewer->SetEditMode(GuiDocumentViewer::ViewOnly);
					break;
				case 1:
					viewer->SetEditMode(GuiDocumentViewer::Selectable);
					break;
				case 2:
					viewer->SetEditMode(GuiDocumentViewer::Editable);
					break;
				}
			});
			cell->AddChild(comboEditMode->GetBoundsComposition());
		}
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(0, 1, 1, 1);

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
			cell->SetSite(0, 2, 1, 1);

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
			cell->SetSite(0, 3, 1, 1);

			buttonImage=g::NewButton();
			buttonImage->SetText(L"Insert Image");
			buttonImage->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			buttonImage->Clicked.AttachLambda([this](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
			{
				TextPos begin=viewer->GetCaretBegin();
				TextPos end=viewer->GetCaretEnd();
				viewer->EditImage(begin, end, image);
			});
			cell->AddChild(buttonImage->GetBoundsComposition());
		}
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(0, 4, 1, 1);

			buttonHyperlink=g::NewButton();
			buttonHyperlink->SetText(L"SetHyperlink");
			buttonHyperlink->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			buttonHyperlink->Clicked.AttachLambda([this](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
			{
			});
			cell->AddChild(buttonHyperlink->GetBoundsComposition());
		}
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(0, 5, 1, 1);

			buttonUnhyperlink=g::NewButton();
			buttonUnhyperlink->SetText(L"Unset Hyperlink");
			buttonUnhyperlink->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			buttonUnhyperlink->Clicked.AttachLambda([this](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
			{
			});
			cell->AddChild(buttonUnhyperlink->GetBoundsComposition());
		}
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(1, 0, 1, 7);

			viewer=g::NewDocumentViewer();
			viewer->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			viewer->SetEditMode(GuiDocumentViewer::Editable);
			viewer->ActiveHyperlinkExecuted.AttachLambda([this](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
			{
				WString reference=viewer->GetActiveHyperlinkReference();
				GetCurrentController()->DialogService()->ShowMessageBox(
					GetNativeWindow(),
					reference,
					L"Hyperlink"
					);
			});
			cell->AddChild(viewer->GetBoundsComposition());
		}
		GetContainerComposition()->AddChild(table);

		{
			Ptr<GuiResource> resource=GuiResource::LoadFromXml(L"..\\GacUISrcCodepackedTest\\Resources\\XmlResource.xml");
			image=resource->GetImageByPath(L"Images/_Save.png");
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