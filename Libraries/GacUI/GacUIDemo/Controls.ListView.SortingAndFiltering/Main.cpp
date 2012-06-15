#include "..\CommonLibrary\FileSystemInformation.h"

using namespace vl::collections;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsDirect2DRenderer();
}

/***********************************************************************
SortingAndFilteringWindow
***********************************************************************/

class SortingAndFilteringWindow : public GuiWindow
{
private:
	GuiListView*					listView;
	List<Ptr<FileProperties>>		fileProperties;

	Ptr<list::ListViewItem> CreateFileItem(Ptr<FileProperties> file)
	{
		Ptr<list::ListViewItem> item=new list::ListViewItem;

		// Get large icon.
		item->largeImage=file->GetBigIcon();
		// Get small icon.
		item->smallImage=file->GetSmallIcon();
		// Get display name
		item->text=file->GetDisplayName();
		// Get type name
		item->subItems.Add(file->GetTypeName());
		// Get last write time
		item->subItems.Add(FileTimeToString(file->GetLastWriteTime()));
		// Get file size
		item->subItems.Add(FileSizeToString(file->GetSize()));

		return item;
	}
public:
	SortingAndFilteringWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		this->SetText(L"Controls.ListView.SortingAndFiltering");
		
		listView=g::NewListViewBigIcon();
		listView->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		listView->SetHorizontalAlwaysVisible(false);
		listView->SetVerticalAlwaysVisible(false);
		listView->SetMultiSelect(true);
		listView->GetBoundsComposition()->SetAlignmentToParent(Margin(4, 4, 4, 4));
		listView->GetItems().GetColumns().Add(new list::ListViewColumn(L"Name", 230));
		listView->GetItems().GetColumns().Add(new list::ListViewColumn(L"Type", 120));
		listView->GetItems().GetColumns().Add(new list::ListViewColumn(L"Date", 120));
		listView->GetItems().GetColumns().Add(new list::ListViewColumn(L"Size", 120));
		listView->ChangeItemStyle(new list::ListViewDetailContentProvider);
		this->AddChild(listView);
		
		{
			// Enumerate all directories and files in the Windows directory.
			WString directory=GetWindowsDirectory();
			List<WString> directories;
			List<WString> files;
			SearchDirectoriesAndFiles(directory, directories, files);
			FOREACH(WString, file, directories.Wrap()>>Concat(files.Wrap()))
			{
				fileProperties.Add(new FileProperties(directory+L"\\"+file));
			}
		}
		{
			// Fill data
			FOREACH(Ptr<FileProperties>, file, fileProperties.Wrap())
			{
				listView->GetItems().Add(CreateFileItem(file));
			}
		}

		// set the preferred minimum client size
		this->GetBoundsComposition()->SetPreferredMinSize(Size(640, 480));
		// call this to calculate the size immediately if any indirect content in the table changes
		// so that the window can calcaulte its correct size before calling the MoveToScreenCenter()
		this->ForceCalculateSizeImmediately();
		// move to the screen center
		this->MoveToScreenCenter();
	}
};

/***********************************************************************
GuiMain
***********************************************************************/

void GuiMain()
{
	GuiWindow* window=new SortingAndFilteringWindow;
	GetApplication()->Run(window);
	delete window;
}