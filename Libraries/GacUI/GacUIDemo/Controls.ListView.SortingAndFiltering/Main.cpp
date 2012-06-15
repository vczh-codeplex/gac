#include "..\..\Public\Source\GacUI.h"
#include <ShlObj.h>

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
		this->AddChild(listView);

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