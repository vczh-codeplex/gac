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
			AddChild(stringGrid);

			ForceCalculateSizeImmediately();
			MoveToScreenCenter();
			Initialize();
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