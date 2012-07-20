#include "TextEditorWindow.h"

void SetImage(GuiMenuButton* menuItem, const WString& imagePath)
{
	menuItem->SetImage(new GuiImageData(GetCurrentController()->ImageService()->CreateImageFromFile(imagePath), 0));
}

WString GetResourceFolder()
{
	wchar_t exePath[1024]={0};
	DWORD len=GetModuleFileName(NULL, exePath, 1024);
	while(exePath[--len]!=L'\\');
	exePath[len+1]=0;
	wcscat_s(exePath, L"..\\Resources\\");
	return exePath;
}

void TextEditorWindow::InitializeMenuBar()
{
	WString resourceFolder=GetResourceFolder();

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

void TextEditorWindow::InitializeToolBar()
{
	WString resourceFolder=GetResourceFolder();

	toolbarStack=new GuiStackComposition;
	toolbarStack->SetDirection(GuiStackComposition::Horizontal);
	toolbarStack->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	toolbarStack->SetAlignmentToParent(Margin(0, 0, 0, 0));
	{
		GuiStackItemComposition* stackItem=new GuiStackItemComposition;
		toolbarStack->AddChild(stackItem);

		toolbarNew=g::NewToolstripButton();
		SetImage(toolbarNew, resourceFolder+L"_New.png");
		toolbarNew->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		stackItem->AddChild(toolbarNew->GetBoundsComposition());
	}
	{
		GuiStackItemComposition* stackItem=new GuiStackItemComposition;
		toolbarStack->AddChild(stackItem);

		toolbarOpen=g::NewToolstripButton();
		SetImage(toolbarOpen, resourceFolder+L"_Open.png");
		toolbarOpen->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		stackItem->AddChild(toolbarOpen->GetBoundsComposition());
	}
	{
		GuiStackItemComposition* stackItem=new GuiStackItemComposition;
		toolbarStack->AddChild(stackItem);

		toolbarSave=g::NewToolstripButton();
		SetImage(toolbarSave, resourceFolder+L"_Save.png");
		toolbarSave->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		stackItem->AddChild(toolbarSave->GetBoundsComposition());
	}
	{
		GuiStackItemComposition* stackItem=new GuiStackItemComposition;
		toolbarStack->AddChild(stackItem);

		toolbarSaveAs=g::NewToolstripButton();
		SetImage(toolbarSaveAs, resourceFolder+L"_SaveAs.png");
		toolbarSaveAs->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		stackItem->AddChild(toolbarSaveAs->GetBoundsComposition());
	}
	{
		GuiStackItemComposition* stackItem=new GuiStackItemComposition;
		toolbarStack->AddChild(stackItem);

		GuiControl* splitter=g::NewToolstripSplitter();
		splitter->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		stackItem->AddChild(splitter->GetBoundsComposition());
	}
	{
		GuiStackItemComposition* stackItem=new GuiStackItemComposition;
		toolbarStack->AddChild(stackItem);

		toolbarUndo=g::NewToolstripButton();
		SetImage(toolbarUndo, resourceFolder+L"_Undo.png");
		toolbarUndo->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		stackItem->AddChild(toolbarUndo->GetBoundsComposition());
	}
	{
		GuiStackItemComposition* stackItem=new GuiStackItemComposition;
		toolbarStack->AddChild(stackItem);

		toolbarRedo=g::NewToolstripButton();
		SetImage(toolbarRedo, resourceFolder+L"_Redo.png");
		toolbarRedo->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		stackItem->AddChild(toolbarRedo->GetBoundsComposition());
	}
	{
		GuiStackItemComposition* stackItem=new GuiStackItemComposition;
		toolbarStack->AddChild(stackItem);

		GuiControl* splitter=g::NewToolstripSplitter();
		splitter->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		stackItem->AddChild(splitter->GetBoundsComposition());
	}
	{
		GuiStackItemComposition* stackItem=new GuiStackItemComposition;
		toolbarStack->AddChild(stackItem);

		toolbarCut=g::NewToolstripButton();
		SetImage(toolbarCut, resourceFolder+L"_Cut.png");
		toolbarCut->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		stackItem->AddChild(toolbarCut->GetBoundsComposition());
	}
	{
		GuiStackItemComposition* stackItem=new GuiStackItemComposition;
		toolbarStack->AddChild(stackItem);

		toolbarCopy=g::NewToolstripButton();
		SetImage(toolbarCopy, resourceFolder+L"_Copy.png");
		toolbarCopy->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		stackItem->AddChild(toolbarCopy->GetBoundsComposition());
	}
	{
		GuiStackItemComposition* stackItem=new GuiStackItemComposition;
		toolbarStack->AddChild(stackItem);

		toolbarPaste=g::NewToolstripButton();
		SetImage(toolbarPaste, resourceFolder+L"_Paste.png");
		toolbarPaste->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		stackItem->AddChild(toolbarPaste->GetBoundsComposition());
	}
	{
		GuiStackItemComposition* stackItem=new GuiStackItemComposition;
		toolbarStack->AddChild(stackItem);

		toolbarDelete=g::NewToolstripButton();
		SetImage(toolbarDelete, resourceFolder+L"_Delete.png");
		toolbarDelete->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		stackItem->AddChild(toolbarDelete->GetBoundsComposition());
	}
}

void TextEditorWindow::InitializeComponents()
{

	// create a table to place a menu bar and a text box
	GuiTableComposition* table=new GuiTableComposition;
	table->SetCellPadding(0);
	table->SetAlignmentToParent(Margin(0, 0, 0, 0));
	table->SetRowsAndColumns(3, 1);
	table->SetRowOption(0, GuiCellOption::MinSizeOption());
	table->SetRowOption(1, GuiCellOption::MinSizeOption());
	table->SetRowOption(2, GuiCellOption::PercentageOption(1.0));
	table->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
	this->GetContainerComposition()->AddChild(table);

	// create the menu bar
	{
		GuiCellComposition* cell=new GuiCellComposition;
		table->AddChild(cell);
		cell->SetSite(0, 0, 1, 1);

		InitializeMenuBar();
		cell->AddChild(menuBar->GetBoundsComposition());
	}

	// create the tool bar
	{
		GuiCellComposition* cell=new GuiCellComposition;
		table->AddChild(cell);
		cell->SetSite(1, 0, 1, 1);
		cell->SetInternalMargin(Margin(2, 2, 2, 0));

		InitializeToolBar();
		cell->AddChild(toolbarStack);
	}

	// create the text box
	{
		GuiCellComposition* cell=new GuiCellComposition;
		table->AddChild(cell);
		cell->SetSite(2, 0, 1, 1);
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