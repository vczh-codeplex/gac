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
		GuiDateComboBox*					dateComboBox;
		GuiDatePicker*						datePicker;
	public:
		TestWindow()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		{
			SetText(GetApplication()->GetExecutableFolder());
			GetContainerComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			GetContainerComposition()->SetPreferredMinSize(Size(320, 240));

			dateComboBox=g::NewDateComboBox();
			dateComboBox->GetDatePicker()->SetDateLocale(Locale(L"en-US"));
			dateComboBox->SelectedDateChanged.AttachLambda([=](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
			{
				SetText(L"GuiDateComboBox: "+dateComboBox->GetText());
			});

			datePicker=g::NewDatePicker();
			datePicker->DateChanged.AttachLambda([=](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
			{
				SetText(L"GuiDatePicker: "+datePicker->GetText());
			});

			GuiTableComposition* table=new GuiTableComposition;
			GetBoundsComposition()->AddChild(table);
			table->SetAlignmentToParent(Margin(0, 0, 0, 0));
			table->SetRowsAndColumns(4, 3);
			table->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
			table->SetRowOption(1, GuiCellOption::MinSizeOption());
			table->SetRowOption(2, GuiCellOption::MinSizeOption());
			table->SetRowOption(3, GuiCellOption::PercentageOption(0.5));
			table->SetColumnOption(0, GuiCellOption::PercentageOption(0.5));
			table->SetColumnOption(1, GuiCellOption::MinSizeOption());
			table->SetColumnOption(2, GuiCellOption::PercentageOption(0.5));
			{
				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetSite(1, 1, 1, 1);

				datePicker->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				cell->AddChild(datePicker->GetBoundsComposition());
			}
			{
				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetSite(2, 1, 1, 1);

				dateComboBox->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				cell->AddChild(dateComboBox->GetBoundsComposition());
			}

			ForceCalculateSizeImmediately();
			MoveToScreenCenter();
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