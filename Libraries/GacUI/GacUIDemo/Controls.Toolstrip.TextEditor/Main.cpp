#include "..\..\Public\Source\GacUIIncludes.h"
#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsDirect2DRenderer();
}

class TextEditorWindow : public GuiWindow
{
private:
	GuiMenuBar*						menuBar;
	GuiMultilineTextBox*			textBox;
	GuiMenuButton*					menuFile;
	GuiMenuButton*					menuFileNew;
	GuiMenuButton*					menuFileOpen;
	GuiMenuButton*					menuFileSave;
	GuiMenuButton*					menuFileSaveAs;
	GuiMenuButton*					menuFileExit;
	GuiMenuButton*					menuEdit;
	GuiMenuButton*					menuEditUndo;
	GuiMenuButton*					menuEditRedo;
	GuiMenuButton*					menuEditCut;
	GuiMenuButton*					menuEditCopy;
	GuiMenuButton*					menuEditPaste;
	GuiMenuButton*					menuEditDelete;
	GuiMenuButton*					menuEditSelect;
	GuiMenuButton*					menuEditGoto;
	GuiMenuButton*					menuFormat;
	GuiMenuButton*					menuFormatFont;
public:
	TextEditorWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		this->SetText(L"Controls.Toolstrip.TextEditor");

		// create a table to place a menu bar and a text box
		GuiTableComposition* table=new GuiTableComposition;
		table->SetCellPadding(0);
		table->SetAlignmentToParent(Margin(0, 0, 0, 0));
		table->SetRowsAndColumns(2, 1);
		table->SetRowOption(0, GuiCellOption::MinSizeOption());
		table->SetRowOption(1, GuiCellOption::PercentageOption(1.0));
		table->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
		this->GetContainerComposition()->AddChild(table);

		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(0, 0, 1, 1);

			// create the menu bar
			menuBar=g::NewMenuBar();
			menuBar->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			menuBar->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			cell->AddChild(menuBar->GetBoundsComposition());

			// create a stack to place menu bar buttons from left to right
			GuiStackComposition* menuStack=new GuiStackComposition;
			menuStack->SetDirection(GuiStackComposition::Horizontal);
			menuStack->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			menuBar->GetContainerComposition()->AddChild(menuStack);

			// create sub menus
			{
				GuiStackItemComposition* stackItem=new GuiStackItemComposition;
				menuStack->AddChild(stackItem);

				menuFile=g::NewMenuBarButton();
				menuFile->SetText(L"File");
				menuFile->CreateSubMenu();
				stackItem->AddChild(menuFile->GetBoundsComposition());

				GuiStackComposition* menuStack=new GuiStackComposition;
				menuStack->SetDirection(GuiStackComposition::Vertical);
				menuStack->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
				menuFile->GetSubMenu()->GetContainerComposition()->AddChild(menuStack);
				{
					GuiStackItemComposition* stackItem=new GuiStackItemComposition;
					menuStack->AddChild(stackItem);

					menuFileNew=g::NewMenuItemButton();
					menuFileNew->SetText(L"New");
					stackItem->AddChild(menuFileNew->GetBoundsComposition());
				}
				{
					GuiStackItemComposition* stackItem=new GuiStackItemComposition;
					menuStack->AddChild(stackItem);

					menuFileOpen=g::NewMenuItemButton();
					menuFileOpen->SetText(L"Open...");
					stackItem->AddChild(menuFileOpen->GetBoundsComposition());
				}
				{
					GuiStackItemComposition* stackItem=new GuiStackItemComposition;
					menuStack->AddChild(stackItem);

					menuFileSave=g::NewMenuItemButton();
					menuFileSave->SetText(L"Save");
					stackItem->AddChild(menuFileSave->GetBoundsComposition());
				}
				{
					GuiStackItemComposition* stackItem=new GuiStackItemComposition;
					menuStack->AddChild(stackItem);

					menuFileSaveAs=g::NewMenuItemButton();
					menuFileSaveAs->SetText(L"Save As...");
					stackItem->AddChild(menuFileSaveAs->GetBoundsComposition());
				}
				{
					GuiStackItemComposition* stackItem=new GuiStackItemComposition;
					menuStack->AddChild(stackItem);

					GuiControl* separator=g::NewMenuSplitter();
					stackItem->AddChild(separator->GetBoundsComposition());
				}
				{
					GuiStackItemComposition* stackItem=new GuiStackItemComposition;
					menuStack->AddChild(stackItem);

					menuFileExit=g::NewMenuItemButton();
					menuFileExit->SetText(L"Exit");
					stackItem->AddChild(menuFileExit->GetBoundsComposition());
				}
			}
			{
				GuiStackItemComposition* stackItem=new GuiStackItemComposition;
				menuStack->AddChild(stackItem);

				menuEdit=g::NewMenuBarButton();
				menuEdit->SetText(L"Edit");
				menuEdit->CreateSubMenu();
				stackItem->AddChild(menuEdit->GetBoundsComposition());

				GuiStackComposition* menuStack=new GuiStackComposition;
				menuStack->SetDirection(GuiStackComposition::Vertical);
				menuStack->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
				menuEdit->GetSubMenu()->GetContainerComposition()->AddChild(menuStack);
				{
					GuiStackItemComposition* stackItem=new GuiStackItemComposition;
					menuStack->AddChild(stackItem);

					menuEditUndo=g::NewMenuItemButton();
					menuEditUndo->SetText(L"Undo");
					stackItem->AddChild(menuEditUndo->GetBoundsComposition());
				}
				{
					GuiStackItemComposition* stackItem=new GuiStackItemComposition;
					menuStack->AddChild(stackItem);

					menuEditRedo=g::NewMenuItemButton();
					menuEditRedo->SetText(L"Redo");
					stackItem->AddChild(menuEditRedo->GetBoundsComposition());
				}
				{
					GuiStackItemComposition* stackItem=new GuiStackItemComposition;
					menuStack->AddChild(stackItem);

					GuiControl* separator=g::NewMenuSplitter();
					stackItem->AddChild(separator->GetBoundsComposition());
				}
				{
					GuiStackItemComposition* stackItem=new GuiStackItemComposition;
					menuStack->AddChild(stackItem);

					menuEditCut=g::NewMenuItemButton();
					menuEditCut->SetText(L"Cut");
					stackItem->AddChild(menuEditCut->GetBoundsComposition());
				}
				{
					GuiStackItemComposition* stackItem=new GuiStackItemComposition;
					menuStack->AddChild(stackItem);

					menuEditCopy=g::NewMenuItemButton();
					menuEditCopy->SetText(L"Copy");
					stackItem->AddChild(menuEditCopy->GetBoundsComposition());
				}
				{
					GuiStackItemComposition* stackItem=new GuiStackItemComposition;
					menuStack->AddChild(stackItem);

					menuEditPaste=g::NewMenuItemButton();
					menuEditPaste->SetText(L"Paste");
					stackItem->AddChild(menuEditPaste->GetBoundsComposition());
				}
				{
					GuiStackItemComposition* stackItem=new GuiStackItemComposition;
					menuStack->AddChild(stackItem);

					menuEditDelete=g::NewMenuItemButton();
					menuEditDelete->SetText(L"Delete");
					stackItem->AddChild(menuEditDelete->GetBoundsComposition());
				}
				{
					GuiStackItemComposition* stackItem=new GuiStackItemComposition;
					menuStack->AddChild(stackItem);

					GuiControl* separator=g::NewMenuSplitter();
					stackItem->AddChild(separator->GetBoundsComposition());
				}
				{
					GuiStackItemComposition* stackItem=new GuiStackItemComposition;
					menuStack->AddChild(stackItem);

					menuEditSelect=g::NewMenuItemButton();
					menuEditSelect->SetText(L"Select All");
					stackItem->AddChild(menuEditSelect->GetBoundsComposition());
				}
				{
					GuiStackItemComposition* stackItem=new GuiStackItemComposition;
					menuStack->AddChild(stackItem);

					GuiControl* separator=g::NewMenuSplitter();
					stackItem->AddChild(separator->GetBoundsComposition());
				}
				{
					GuiStackItemComposition* stackItem=new GuiStackItemComposition;
					menuStack->AddChild(stackItem);

					menuEditGoto=g::NewMenuItemButton();
					menuEditGoto->SetText(L"Go To...");
					stackItem->AddChild(menuEditGoto->GetBoundsComposition());
				}
			}
			{
				GuiStackItemComposition* stackItem=new GuiStackItemComposition;
				menuStack->AddChild(stackItem);

				menuFormat=g::NewMenuBarButton();
				menuFormat->SetText(L"Format");
				menuFormat->CreateSubMenu();
				stackItem->AddChild(menuFormat->GetBoundsComposition());

				GuiStackComposition* menuStack=new GuiStackComposition;
				menuStack->SetDirection(GuiStackComposition::Vertical);
				menuStack->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
				menuFormat->GetSubMenu()->GetContainerComposition()->AddChild(menuStack);
				{
					GuiStackItemComposition* stackItem=new GuiStackItemComposition;
					menuStack->AddChild(stackItem);

					menuFormatFont=g::NewMenuItemButton();
					menuFormatFont->SetText(L"Font...");
					stackItem->AddChild(menuFormatFont->GetBoundsComposition());
				}
			}
		}

		// create the text box
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(1, 0, 1, 1);
			cell->SetInternalMargin(Margin(2, 2, 2, 2));

			// create the menu bar
			textBox=g::NewMultilineTextBox();
			textBox->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			cell->AddChild(textBox->GetBoundsComposition());
		}

		// set the preferred minimum client size
		this->GetBoundsComposition()->SetPreferredMinSize(Size(640, 480));
		// call this to calculate the size immediately if any indirect content in the table changes
		// so that the window can calcaulte its correct size before calling the MoveToScreenCenter()
		this->ForceCalculateSizeImmediately();
		// move to the screen center
		this->MoveToScreenCenter();
	}

	~TextEditorWindow()
	{
	}
};

void GuiMain()
{
	GuiWindow* window=new TextEditorWindow();
	GetApplication()->Run(window);
	delete window;
}