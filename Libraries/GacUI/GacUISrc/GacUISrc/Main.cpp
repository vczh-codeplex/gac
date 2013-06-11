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
	public:
		FileNameColumnProvider(list::StrongTypedDataProvider<Ptr<FileProperties>>* _dataProvider)
			:StrongTypedColumnProvider(_dataProvider)
		{
		}

		void GetCellData(const Ptr<FileProperties>& rowData, WString& cellData)override
		{
			cellData=INVLOC.ToUpper(rowData->GetDisplayName());
		}

		WString GetCellText(vint row)override
		{
			Ptr<FileProperties> rowData;
			dataProvider->GetRowData(row, rowData);
			return rowData->GetDisplayName();
		}
	};
	
	class FileDateColumnProvider : public list::StrongTypedColumnProvider<Ptr<FileProperties>, unsigned __int64>
	{
	public:
		FileDateColumnProvider(list::StrongTypedDataProvider<Ptr<FileProperties>>* _dataProvider)
			:StrongTypedColumnProvider(_dataProvider)
		{
		}

		void GetCellData(const Ptr<FileProperties>& rowData, unsigned __int64& cellData)override
		{
			FILETIME ft=rowData->GetLastWriteTime();
			LARGE_INTEGER li;
			li.HighPart=ft.dwHighDateTime;
			li.LowPart=ft.dwLowDateTime;
			cellData=li.QuadPart;
		}

		WString GetCellText(vint row)override
		{
			Ptr<FileProperties> rowData;
			dataProvider->GetRowData(row, rowData);
			return FileTimeToString(rowData->GetLastWriteTime());
		}

		void VisualizeCell(vint row, list::IDataVisualizer* dataVisualizer)override
		{
			StrongTypedColumnProvider::VisualizeCell(row, dataVisualizer);
			list::NotifyIconDataVisualizer* visualizer=dataVisualizer->GetVisualizer<list::NotifyIconDataVisualizer>();
			visualizer->GetLeftImageElement()->SetImage(dataProvider->GetRowLargeImage(row)->GetImage());
			visualizer->GetRightImageElement()->SetImage(dataProvider->GetRowSmallImage(row)->GetImage());
		}
	};
	
	class FileSizeColumnProvider : public list::StrongTypedColumnProvider<Ptr<FileProperties>, signed __int64>
	{
	public:
		FileSizeColumnProvider(list::StrongTypedDataProvider<Ptr<FileProperties>>* _dataProvider)
			:StrongTypedColumnProvider(_dataProvider)
		{
		}

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
	
	class FileImageColumnProvider : public list::StrongTypedColumnProvider<Ptr<FileProperties>, Ptr<GuiImageData>>
	{
	public:
		FileImageColumnProvider(list::StrongTypedDataProvider<Ptr<FileProperties>>* _dataProvider)
			:StrongTypedColumnProvider(_dataProvider)
		{
		}

		void GetCellData(const Ptr<FileProperties>& rowData, Ptr<GuiImageData>& cellData)override
		{
			cellData=rowData->GetSmallIcon();
		}

		WString GetCellText(vint row)override
		{
			return L"";
		}

		void VisualizeCell(vint row, list::IDataVisualizer* dataVisualizer)override
		{
			StrongTypedColumnProvider::VisualizeCell(row, dataVisualizer);
			list::ImageDataVisualizer* visualizer=dataVisualizer->GetVisualizer<list::ImageDataVisualizer>();
			visualizer->GetImageElement()->SetImage(dataProvider->GetRowLargeImage(row)->GetImage());
		}
	};
	
	class FileTypeColumnProvider : public list::StrongTypedColumnProvider<Ptr<FileProperties>, WString>
	{
	protected:
		class FileTypeFilter : public FilterBase
		{
		protected:
			GuiMenu*							popup;
			GuiTextList*						textList;
			bool								preventEvent;

			bool FilterData(const Ptr<FileProperties>& rowData, const WString& cellData)
			{
				if(textList->GetItems().Count()==0)
				{
					return true;
				}
				else
				{
					return From(textList->GetItems())
						.Skip(1)
						.Where([](Ptr<list::TextItem> item){return item->GetChecked();})
						.Any([=](Ptr<list::TextItem> item){return item->GetText()==cellData;});
				}
			}

			void textList_ItemChecked(GuiGraphicsComposition* sender, GuiItemEventArgs& arguments)
			{
				if(!preventEvent)
				{
					if(arguments.itemIndex==0)
					{
						preventEvent=true;
						bool checked=textList->GetItems()[0]->GetChecked();
						for(vint i=1;i<textList->GetItems().Count();i++)
						{
							textList->GetItems().SetChecked(i, checked);
						}
						preventEvent=false;
					}
					InvokeOnFilterChanged();
				}
			}

			void popup_Opened(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
			{
				if(textList->GetItems().Count()==0)
				{
					textList->GetItems().Add(new list::TextItem(L"(Select All)", true));
					LazyList<WString> columns=Range(0, dataProvider->GetRowCount())
						.Select([this](vint i)->WString
						{
							Ptr<FileProperties> rowData;
							WString cellData;
							dataProvider->GetRowData(i, rowData);
							ownerColumn->GetCellData(rowData, cellData);
							return cellData;
						})
						.Distinct()
						.OrderBy([](const WString& a, const WString& b){return WString::Compare(a, b);});

					FOREACH(WString, item, columns)
					{
						textList->GetItems().Add(new list::TextItem(item, true));
					}
				}
			}
		public:
			FileTypeFilter(FileTypeColumnProvider* columnProvider)
				:FilterBase(columnProvider)
				,popup(0)
				,preventEvent(false)
			{
				textList=g::NewCheckTextList();
				textList->SetHorizontalAlwaysVisible(false);
				textList->SetVerticalAlwaysVisible(false);
				textList->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				textList->GetBoundsComposition()->SetPreferredMinSize(Size(160, 200));
				textList->ItemChecked.AttachMethod(this, &FileTypeFilter::textList_ItemChecked);

				popup=g::NewMenu(0);
				popup->GetContainerComposition()->AddChild(textList->GetBoundsComposition());
				popup->WindowOpened.AttachMethod(this, &FileTypeFilter::popup_Opened);
			}

			~FileTypeFilter()
			{
				delete popup;
			}

			GuiMenu* GetPopup()
			{
				return popup;
			}
		};
	protected:
		Ptr<FileTypeFilter>						filter;

	public:
		FileTypeColumnProvider(list::StrongTypedDataProvider<Ptr<FileProperties>>* _dataProvider)
			:StrongTypedColumnProvider(_dataProvider)
		{
			filter=new FileTypeFilter(this);
			SetInherentFilter(filter);
			SetPopup(filter->GetPopup());
		}

		void GetCellData(const Ptr<FileProperties>& rowData, WString& cellData)override
		{
			cellData=rowData->GetTypeName();
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
				->SetVisualizerFactory(new list::CellBorderDataVisualizer::Factory(new list::ListViewMainColumnDataVisualizer::Factory))
				->SetSize(240);

			AddSortableStrongTypedColumn<unsigned __int64>(L"Data modified", new FileDateColumnProvider(this))
				->SetVisualizerFactory(new list::CellBorderDataVisualizer::Factory(new list::NotifyIconDataVisualizer::Factory(new list::ListViewSubColumnDataVisualizer::Factory)))
				->SetSize(200);

			AddSortableStrongTypedColumn<WString>(L"Type", new FileTypeColumnProvider(this))
				->SetVisualizerFactory(new list::CellBorderDataVisualizer::Factory(new list::HyperlinkDataVisualizer::Factory))
				->SetSize(160);

			AddSortableStrongTypedColumn<signed __int64>(L"Size", new FileSizeColumnProvider(this))
				->SetVisualizerFactory(new list::CellBorderDataVisualizer::Factory(new list::ListViewSubColumnDataVisualizer::Factory))
				->SetSize(100);

			AddSortableStrongTypedColumn<Ptr<GuiImageData>>(L"Big Icon", new FileImageColumnProvider(this))
				->SetVisualizerFactory(new list::CellBorderDataVisualizer::Factory(new list::ImageDataVisualizer::Factory))
				->SetSize(40);

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