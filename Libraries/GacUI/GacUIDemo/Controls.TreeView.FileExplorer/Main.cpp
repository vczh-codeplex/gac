#include "..\CommonLibrary\FileSystemInformation.h"

using namespace vl::collections;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsDirect2DRenderer();
}

/***********************************************************************
SortingAndFilteringWindow
***********************************************************************/

class FileExplorerWindow : public GuiWindow
{
private:
	GuiTreeView*					treeView;

	void InitializeFileSystem()
	{
		wchar_t drives[1024];
		GetLogicalDriveStrings(sizeof(drives), drives);

		wchar_t* reading=drives;
		while(true)
		{
			WString drive=reading;
			if(drive==L"")
			{
				break;
			}
			else
			{
				Ptr<tree::TreeViewItem> item=new tree::TreeViewItem;
				item->text=drive;
				treeView->Nodes()->Children().Add(new tree::MemoryNodeProvider(item));
				reading+=drive.Length()+1;
			}
		}
	}
public:
	FileExplorerWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		this->SetText(L"Controls.TreeView.FileExplorerWindow");
		
		// Create list view control to display a folder
		treeView=g::NewTreeView();
		treeView->SetHorizontalAlwaysVisible(false);
		treeView->SetVerticalAlwaysVisible(false);
		treeView->GetBoundsComposition()->SetAlignmentToParent(Margin(4, 4, 4, 4));
		this->AddChild(treeView);

		InitializeFileSystem();

		// set the preferred minimum client size
		this->GetBoundsComposition()->SetPreferredMinSize(Size(640, 480));
		// call this to calculate the size immediately if any indirect content in the table changes
		// so that the window can calcaulte its correct size before calling the MoveToScreenCenter()
		this->ForceCalculateSizeImmediately();
		// move to the screen center
		this->MoveToScreenCenter();
	}

	~FileExplorerWindow()
	{
	}
};

/***********************************************************************
GuiMain
***********************************************************************/

void GuiMain()
{
	GuiWindow* window=new FileExplorerWindow;
	GetApplication()->Run(window);
	delete window;
}