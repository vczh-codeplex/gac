#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "FileSystemInformation.h"
#include "..\..\Source\GraphicsElement\WindowsDirect2D\GuiGraphicsWindowsDirect2D.h"

using namespace vl::stream;
using namespace vl::collections;
using namespace vl::regex;

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

/***********************************************************************
TestWindow
***********************************************************************/

	class TestWindow : public GuiWindow
	{
	private:
		GuiDatePicker*						datePicker;
	public:
		TestWindow()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		{
			SetText(GetApplication()->GetExecutableFolder());
			GetContainerComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			GetContainerComposition()->SetPreferredMinSize(Size(320, 240));
			ForceCalculateSizeImmediately();
			MoveToScreenCenter();

			datePicker=g::NewDatePicker();
			SetText(datePicker->GetText());
			datePicker->DateChanged.AttachLambda([=](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
			{
				SetText(datePicker->GetText());
			});

			GuiTableComposition* table=new GuiTableComposition;
			GetBoundsComposition()->AddChild(table);
			table->SetAlignmentToParent(Margin(0, 0, 0, 0));
			table->SetRowsAndColumns(3, 3);
			table->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
			table->SetRowOption(1, GuiCellOption::MinSizeOption());
			table->SetRowOption(2, GuiCellOption::PercentageOption(0.5));
			table->SetColumnOption(0, GuiCellOption::PercentageOption(0.5));
			table->SetColumnOption(1, GuiCellOption::MinSizeOption());
			table->SetColumnOption(2, GuiCellOption::PercentageOption(0.5));
			{
				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetSite(1, 1, 1, 1);

				cell->AddChild(datePicker->GetBoundsComposition());
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