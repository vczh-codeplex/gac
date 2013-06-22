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
		GuiStringGrid*						stringGrid;
		GuiToolstripMenu*					contextMenu;

		GuiToolstripButton*					menuInsertRowBefore;
		GuiToolstripButton*					menuInsertRowAfter;
		GuiToolstripButton*					menuRemoveRow;
		GuiToolstripButton*					menuInsertColumnBefore;
		GuiToolstripButton*					menuInsertColumnAfter;
		GuiToolstripButton*					menuRemoveColumn;

		void menuInsertRowBefore_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			vint row=stringGrid->GetSelectedCell().row;
			stringGrid->Grids().InsertRow(row);
			stringGrid->SetSelectedCell(GridPos(row, 0));
		}

		void menuInsertRowAfter_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			vint row=stringGrid->GetSelectedCell().row;
			stringGrid->Grids().InsertRow(row+1);
			stringGrid->SetSelectedCell(GridPos(row+1, 0));
		}

		void menuRemoveRow_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			vint row=stringGrid->GetSelectedCell().row;
			stringGrid->Grids().RemoveRow(row);
		}

		void menuInsertColumnBefore_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			vint column=stringGrid->GetSelectedCell().column;
			stringGrid->Grids().InsertColumn(column, L"New Colunn");
		}

		void menuInsertColumnAfter_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			vint column=stringGrid->GetSelectedCell().column;
			stringGrid->Grids().InsertColumn(column+1, L"New Colunn");
		}

		void menuRemoveColumn_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			vint column=stringGrid->GetSelectedCell().column;
			stringGrid->Grids().RemoveColumn(column);
		}

		void contextMenu_WindowOpened(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			menuInsertRowBefore->SetEnabled(stringGrid->GetSelectedCell().row!=-1);
			menuInsertRowAfter->SetEnabled(stringGrid->GetSelectedCell().row!=-1);
			menuRemoveRow->SetEnabled(stringGrid->GetSelectedCell().row!=-1);
			menuInsertColumnBefore->SetEnabled(stringGrid->GetSelectedCell().column!=-1);
			menuInsertColumnAfter->SetEnabled(stringGrid->GetSelectedCell().column!=-1);
			menuRemoveColumn->SetEnabled(stringGrid->GetSelectedCell().column!=-1);
		}

		void stringGrid_rightButtonUp(GuiGraphicsComposition* sender, GuiMouseEventArgs& arguments)
		{
			contextMenu->ShowPopup(this, Point(arguments.x, arguments.y));
		}

		void Initialize()
		{
			stringGrid->Grids().AppendColumn(L"File Name", 240);
			stringGrid->Grids().AppendColumn(L"Data Modified", 120);
			stringGrid->Grids().AppendColumn(L"Type", 160);
			stringGrid->Grids().AppendColumn(L"Size", 100);

			WString path=GetWindowsDirectory();
			List<WString> files, folders;
			SearchDirectoriesAndFiles(path, folders, files);

			FOREACH(WString, file, files)
			{
				WString fullPath=path+L"\\"+file;
				vint row=stringGrid->Grids().AppendRow();
				stringGrid->Grids().SetGridString(row, 0, GetFileDisplayName(fullPath));
				stringGrid->Grids().SetGridString(row, 1, FileTimeToString(GetFileLastWriteTime(fullPath)));
				stringGrid->Grids().SetGridString(row, 2, GetFileTypeName(fullPath));
				stringGrid->Grids().SetGridString(row, 3, FileSizeToString(GetFileSize(fullPath)));
			}
		}
	public:
		TestWindow()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		{
			SetText(GetApplication()->GetExecutableFolder());
			GetContainerComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			GetContainerComposition()->SetPreferredMinSize(Size(800, 600));

			stringGrid=g::NewStringGrid();
			stringGrid->GetBoundsComposition()->SetAlignmentToParent(Margin(5, 5, 5, 5));
			stringGrid->SetVerticalAlwaysVisible(false);
			stringGrid->SetHorizontalAlwaysVisible(false);
			stringGrid->SetMultiSelect(true);
			stringGrid->GetBoundsComposition()->GetEventReceiver()->rightButtonUp.AttachMethod(this, &TestWindow::stringGrid_rightButtonUp);
			AddChild(stringGrid);

			contextMenu=g::NewMenu(0);
			contextMenu->GetBuilder()
				->Button(0, L"Insert Row Before", &menuInsertRowBefore)
				->Button(0, L"Insert Row After", &menuInsertRowAfter)
				->Button(0, L"Remove This Row", &menuRemoveRow)
				->Splitter()
				->Button(0, L"Insert Column Before...", &menuInsertColumnBefore)
				->Button(0, L"Insert Column After...", &menuInsertColumnAfter)
				->Button(0, L"Remove This Column", &menuRemoveColumn)
				;
			contextMenu->WindowOpened.AttachMethod(this, &TestWindow::contextMenu_WindowOpened);
			menuInsertRowBefore->Clicked.AttachMethod(this, &TestWindow::menuInsertRowBefore_Clicked);
			menuInsertRowAfter->Clicked.AttachMethod(this, &TestWindow::menuInsertRowAfter_Clicked);
			menuRemoveRow->Clicked.AttachMethod(this, &TestWindow::menuRemoveRow_Clicked);
			menuInsertColumnBefore->Clicked.AttachMethod(this, &TestWindow::menuInsertColumnBefore_Clicked);
			menuInsertColumnAfter->Clicked.AttachMethod(this, &TestWindow::menuInsertColumnAfter_Clicked);
			menuRemoveColumn->Clicked.AttachMethod(this, &TestWindow::menuRemoveColumn_Clicked);

			ForceCalculateSizeImmediately();
			MoveToScreenCenter();
			Initialize();
		}

		~TestWindow()
		{
			delete contextMenu;
		}
	};
}
using namespace test;

void GuiMain()
{
	TestWindow window;
	GetApplication()->Run(&window);
}