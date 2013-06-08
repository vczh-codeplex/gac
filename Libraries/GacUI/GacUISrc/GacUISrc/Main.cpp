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
	
	class FileNameColumnProvider : public list::StrongTypedColumnProvider<Ptr<FileProperties>, WString>
	{
	protected:

		void GetCellData(const Ptr<FileProperties>& rowData, WString& cellData)override
		{
			cellData=INVLOC.ToUpper(rowData->GetDisplayName());
		}
	public:
		FileNameColumnProvider(list::StrongTypedDataProvider<Ptr<FileProperties>>* _dataProvider)
			:StrongTypedColumnProvider(_dataProvider)
		{
		}

		WString GetCellText(vint row)override
		{
			Ptr<FileProperties> rowData;
			dataProvider->GetRowData(row, rowData);
			return rowData->GetDisplayName();
		}
	};
	
	class FileTypeColumnProvider : public list::StrongTypedColumnProvider<Ptr<FileProperties>, WString>
	{
	protected:

		void GetCellData(const Ptr<FileProperties>& rowData, WString& cellData)override
		{
			cellData=rowData->GetTypeName();
		}
	public:
		FileTypeColumnProvider(list::StrongTypedDataProvider<Ptr<FileProperties>>* _dataProvider)
			:StrongTypedColumnProvider(_dataProvider)
		{
		}
	};
	
	class FileDateColumnProvider : public list::StrongTypedColumnProvider<Ptr<FileProperties>, unsigned __int64>
	{
	protected:

		void GetCellData(const Ptr<FileProperties>& rowData, unsigned __int64& cellData)override
		{
			FILETIME ft=rowData->GetLastWriteTime();
			LARGE_INTEGER li;
			li.HighPart=ft.dwHighDateTime;
			li.LowPart=ft.dwLowDateTime;
			cellData=li.QuadPart;
		}
	public:
		FileDateColumnProvider(list::StrongTypedDataProvider<Ptr<FileProperties>>* _dataProvider)
			:StrongTypedColumnProvider(_dataProvider)
		{
		}

		WString GetCellText(vint row)override
		{
			Ptr<FileProperties> rowData;
			dataProvider->GetRowData(row, rowData);
			return FileTimeToString(rowData->GetLastWriteTime());
		}
	};
	
	class FileSizeColumnProvider : public list::StrongTypedColumnProvider<Ptr<FileProperties>, signed __int64>
	{
	protected:

		void GetCellData(const Ptr<FileProperties>& rowData, signed __int64& cellData)override
		{
			if(rowData->IsDirectory())
			{
				cellData=-1;
			}
			else
			{
				cellData=(signed __int64)rowData->GetSize().QuadPart;
			}
		}
	public:
		FileSizeColumnProvider(list::StrongTypedDataProvider<Ptr<FileProperties>>* _dataProvider)
			:StrongTypedColumnProvider(_dataProvider)
		{
		}

		WString GetCellText(vint row)override
		{
			Ptr<FileProperties> rowData;
			dataProvider->GetRowData(row, rowData);
			if(rowData->IsDirectory())
			{
				return L"";
			}
			else
			{
				return FileSizeToString(rowData->GetSize());
			}
		}
	};

	class DataProvider : public list::StrongTypedDataProvider<Ptr<FileProperties>>
	{
	protected:
		List<Ptr<FileProperties>>				fileProperties;
	public:
		DataProvider()
		{
			AddSortableStrongTypedColumn<WString>(L"Name", new FileNameColumnProvider(this))
				->SetSize(240);

			AddSortableStrongTypedColumn<unsigned __int64>(L"Data modified", new FileDateColumnProvider(this))
				->SetSize(120);

			AddSortableStrongTypedColumn<WString>(L"Type", new FileTypeColumnProvider(this))
				->SetSize(160);

			AddSortableStrongTypedColumn<signed __int64>(L"Size", new FileSizeColumnProvider(this))
				->SetSize(100);

			List<WString> directories, files;
			WString path=GetWindowsDirectory();
			SearchDirectoriesAndFiles(path, directories, files);
			FOREACH(WString, fileName, directories)
			{
				fileProperties.Add(new FileProperties(path+L"\\"+fileName));
			}
			FOREACH(WString, fileName, files)
			{
				fileProperties.Add(new FileProperties(path+L"\\"+fileName));
			}
		}

		void GetRowData(vint row, Ptr<FileProperties>& rowData)override
		{
			rowData=fileProperties[row];
		}

		vint GetRowCount()override
		{
			return fileProperties.Count();
		}

		Ptr<GuiImageData> GetRowLargeImage(vint row)override
		{
			return fileProperties[row]->GetBigIcon();
		}

		Ptr<GuiImageData> GetRowSmallImage(vint row)override
		{
			return fileProperties[row]->GetSmallIcon();
		}
	};

	class TestWindow : public GuiWindow
	{
	private:
		GuiVirtualDataGrid*					dataGrid;
	public:
		TestWindow()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		{
			SetText(GetApplication()->GetExecutableFolder());
			GetContainerComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			GetContainerComposition()->SetPreferredMinSize(Size(1048, 576));
			ForceCalculateSizeImmediately();
			MoveToScreenCenter();

			dataGrid=new GuiVirtualDataGrid(GetCurrentTheme()->CreateListViewStyle(), new DataProvider);
			dataGrid->GetBoundsComposition()->SetAlignmentToParent(Margin(5, 5, 5, 5));
			dataGrid->SetHorizontalAlwaysVisible(false);
			dataGrid->SetVerticalAlwaysVisible(false);
			AddChild(dataGrid);
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