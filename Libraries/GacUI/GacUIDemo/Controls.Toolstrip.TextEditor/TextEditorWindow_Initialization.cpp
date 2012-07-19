#include "TextEditorWindow.h"

void SetImage(GuiMenuButton* menuItem, const WString& imagePath)
{
	menuItem->SetImage(new GuiImageData(GetCurrentController()->ImageService()->CreateImageFromFile(imagePath), 0));
}

void TextEditorWindow::InitializeMenuBar()
{
	WString resourceFolder;
	{
		wchar_t exePath[1024]={0};
		DWORD len=GetModuleFileName(NULL, exePath, 1024);
		while(exePath[--len]!=L'\\');
		exePath[len+1]=0;
		wcscat_s(exePath, L"..\\Resources\\");
		resourceFolder=exePath;
	}

	menuBar=g::NewMenuBar();
	menuBar->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	menuBar->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));

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
		menuFile->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
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
			SetImage(menuFileNew, resourceFolder+L"_New.png");
			menuFileNew->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			stackItem->AddChild(menuFileNew->GetBoundsComposition());
		}
		{
			GuiStackItemComposition* stackItem=new GuiStackItemComposition;
			menuStack->AddChild(stackItem);

			menuFileOpen=g::NewMenuItemButton();
			menuFileOpen->SetText(L"Open...");
			SetImage(menuFileOpen, resourceFolder+L"_Open.png");
			menuFileOpen->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			stackItem->AddChild(menuFileOpen->GetBoundsComposition());
		}
		{
			GuiStackItemComposition* stackItem=new GuiStackItemComposition;
			menuStack->AddChild(stackItem);

			menuFileSave=g::NewMenuItemButton();
			menuFileSave->SetText(L"Save");
			SetImage(menuFileSave, resourceFolder+L"_Save.png");
			menuFileSave->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			stackItem->AddChild(menuFileSave->GetBoundsComposition());
		}
		{
			GuiStackItemComposition* stackItem=new GuiStackItemComposition;
			menuStack->AddChild(stackItem);

			menuFileSaveAs=g::NewMenuItemButton();
			menuFileSaveAs->SetText(L"Save As...");
			SetImage(menuFileSaveAs, resourceFolder+L"_SaveAs.png");
			menuFileSaveAs->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
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
			menuFileExit->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			stackItem->AddChild(menuFileExit->GetBoundsComposition());
		}
	}
	{
		GuiStackItemComposition* stackItem=new GuiStackItemComposition;
		menuStack->AddChild(stackItem);

		menuEdit=g::NewMenuBarButton();
		menuEdit->SetText(L"Edit");
		menuEdit->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
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
			SetImage(menuEditUndo, resourceFolder+L"_Undo.png");
			menuEditUndo->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			stackItem->AddChild(menuEditUndo->GetBoundsComposition());
		}
		{
			GuiStackItemComposition* stackItem=new GuiStackItemComposition;
			menuStack->AddChild(stackItem);

			menuEditRedo=g::NewMenuItemButton();
			menuEditRedo->SetText(L"Redo");
			SetImage(menuEditRedo, resourceFolder+L"_Redo.png");
			menuEditRedo->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
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
			SetImage(menuEditCut, resourceFolder+L"_Cut.png");
			menuEditCut->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			stackItem->AddChild(menuEditCut->GetBoundsComposition());
		}
		{
			GuiStackItemComposition* stackItem=new GuiStackItemComposition;
			menuStack->AddChild(stackItem);

			menuEditCopy=g::NewMenuItemButton();
			menuEditCopy->SetText(L"Copy");
			SetImage(menuEditCopy, resourceFolder+L"_Copy.png");
			menuEditCopy->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			stackItem->AddChild(menuEditCopy->GetBoundsComposition());
		}
		{
			GuiStackItemComposition* stackItem=new GuiStackItemComposition;
			menuStack->AddChild(stackItem);

			menuEditPaste=g::NewMenuItemButton();
			menuEditPaste->SetText(L"Paste");
			SetImage(menuEditPaste, resourceFolder+L"_Paste.png");
			menuEditPaste->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			stackItem->AddChild(menuEditPaste->GetBoundsComposition());
		}
		{
			GuiStackItemComposition* stackItem=new GuiStackItemComposition;
			menuStack->AddChild(stackItem);

			menuEditDelete=g::NewMenuItemButton();
			menuEditDelete->SetText(L"Delete");
			SetImage(menuEditDelete, resourceFolder+L"_Delete.png");
			menuEditDelete->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
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
			menuEditSelect->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			stackItem->AddChild(menuEditSelect->GetBoundsComposition());
		}
	}
	{
		GuiStackItemComposition* stackItem=new GuiStackItemComposition;
		menuStack->AddChild(stackItem);

		menuFormat=g::NewMenuBarButton();
		menuFormat->SetText(L"Format");
		menuFormat->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
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
			menuFormatFont->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			stackItem->AddChild(menuFormatFont->GetBoundsComposition());
		}
	}
}

void TextEditorWindow::InitializeComponents()
{

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
		InitializeMenuBar();
		cell->AddChild(menuBar->GetBoundsComposition());
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