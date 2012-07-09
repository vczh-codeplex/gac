#include "..\..\Public\Source\GacUI.h"
#include <Windows.h>

using namespace vl::collections;
using namespace vl::stream;

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

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
	GuiMenuButton*					menuFilePrint;
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
	GuiMenuButton*					menuFormatColor;

	void InitializeMenuBar()
	{
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
				menuFileNew->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				stackItem->AddChild(menuFileNew->GetBoundsComposition());
			}
			{
				GuiStackItemComposition* stackItem=new GuiStackItemComposition;
				menuStack->AddChild(stackItem);

				menuFileOpen=g::NewMenuItemButton();
				menuFileOpen->SetText(L"Open...");
				menuFileOpen->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				stackItem->AddChild(menuFileOpen->GetBoundsComposition());
			}
			{
				GuiStackItemComposition* stackItem=new GuiStackItemComposition;
				menuStack->AddChild(stackItem);

				menuFileSave=g::NewMenuItemButton();
				menuFileSave->SetText(L"Save");
				menuFileSave->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				stackItem->AddChild(menuFileSave->GetBoundsComposition());
			}
			{
				GuiStackItemComposition* stackItem=new GuiStackItemComposition;
				menuStack->AddChild(stackItem);

				menuFileSaveAs=g::NewMenuItemButton();
				menuFileSaveAs->SetText(L"Save As...");
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

				menuFilePrint=g::NewMenuItemButton();
				menuFilePrint->SetText(L"Print...");
				menuFilePrint->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				stackItem->AddChild(menuFilePrint->GetBoundsComposition());
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
				menuEditUndo->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				stackItem->AddChild(menuEditUndo->GetBoundsComposition());
			}
			{
				GuiStackItemComposition* stackItem=new GuiStackItemComposition;
				menuStack->AddChild(stackItem);

				menuEditRedo=g::NewMenuItemButton();
				menuEditRedo->SetText(L"Redo");
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
				menuEditCut->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				stackItem->AddChild(menuEditCut->GetBoundsComposition());
			}
			{
				GuiStackItemComposition* stackItem=new GuiStackItemComposition;
				menuStack->AddChild(stackItem);

				menuEditCopy=g::NewMenuItemButton();
				menuEditCopy->SetText(L"Copy");
				menuEditCopy->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				stackItem->AddChild(menuEditCopy->GetBoundsComposition());
			}
			{
				GuiStackItemComposition* stackItem=new GuiStackItemComposition;
				menuStack->AddChild(stackItem);

				menuEditPaste=g::NewMenuItemButton();
				menuEditPaste->SetText(L"Paste");
				menuEditPaste->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				stackItem->AddChild(menuEditPaste->GetBoundsComposition());
			}
			{
				GuiStackItemComposition* stackItem=new GuiStackItemComposition;
				menuStack->AddChild(stackItem);

				menuEditDelete=g::NewMenuItemButton();
				menuEditDelete->SetText(L"Delete");
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
				menuEditGoto->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				stackItem->AddChild(menuEditGoto->GetBoundsComposition());
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
			{
				GuiStackItemComposition* stackItem=new GuiStackItemComposition;
				menuStack->AddChild(stackItem);

				menuFormatColor=g::NewMenuItemButton();
				menuFormatColor->SetText(L"Color...");
				menuFormatColor->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				stackItem->AddChild(menuFormatColor->GetBoundsComposition());
			}
		}
	}

private:
	WString							fileName;
	bool							enableModifyFlag;
	bool							modified;

	void UpdateWindowTitle()
	{
		if(fileName==L"")
		{
			this->SetText(L"Controls.Toolstrip.TextEditor[UNTITLED]");
		}
		else
		{
			this->SetText(L"Controls.Toolstrip.TextEditor["+fileName+L"]");
		}
	}

	bool TryOpen()
	{
		if(PromptSaveUnchanged())
		{
			List<WString> selectionFileNames;
			int selectionFilterIndex=0;
			if(GetCurrentController()->DialogService()->ShowFileDialog(
				this->GetNativeWindow(),
				selectionFileNames,
				selectionFilterIndex,
				INativeDialogService::FileDialogOpen,
				L"Open a text document",
				L"",
				L"",
				L".txt",
				L"Text Files(*.txt)|*.txt|All Files(*.*)|*.*",
				(INativeDialogService::FileDialogOptions)
					( INativeDialogService::FileDialogAddToRecent
					| INativeDialogService::FileDialogDereferenceLinks
					| INativeDialogService::FileDialogDirectoryMustExist
					| INativeDialogService::FileDialogFileMustExist
					)
				))
			{
				fileName=selectionFileNames[0];
				FileStream fileStream(fileName, FileStream::ReadOnly);
				BomDecoder decoder;
				DecoderStream decoderStream(fileStream, decoder);
				StreamReader reader(decoderStream);
				textBox->SetText(reader.ReadToEnd());
				textBox->Select(TextPos(0, 0), TextPos(0, 0));
				modified=false;
				UpdateWindowTitle();
				return true;
			}
		}
		return true;
	}

	bool TrySave(bool ignoreCurrentFileName)
	{
		if(fileName==L"" || ignoreCurrentFileName)
		{
			List<WString> selectionFileNames;
			int selectionFilterIndex=0;
			if(GetCurrentController()->DialogService()->ShowFileDialog(
				this->GetNativeWindow(),
				selectionFileNames,
				selectionFilterIndex,
				INativeDialogService::FileDialogSave,
				L"Save a text document",
				L"",
				L"",
				L".txt",
				L"Text Files(*.txt)|*.txt|All Files(*.*)|*.*",
				(INativeDialogService::FileDialogOptions)
					( INativeDialogService::FileDialogAddToRecent
					| INativeDialogService::FileDialogDereferenceLinks
					| INativeDialogService::FileDialogDirectoryMustExist
					| INativeDialogService::FileDialogPromptOverwriteFile
					)
				))
			{
				fileName=selectionFileNames[0];
			}
			else
			{
				return false;
			}
		}
		{
			FileStream fileStream(fileName, FileStream::WriteOnly);
			BomEncoder encoder(BomEncoder::Utf8);
			EncoderStream encoderStream(fileStream, encoder);
			StreamWriter writer(encoderStream);
			writer.WriteString(textBox->GetText());
			modified=false;
			UpdateWindowTitle();
		}
		return true;
	}

	bool TryNew()
	{
		if(PromptSaveUnchanged())
		{
			fileName=L"";
			textBox->SetText(L"");
			modified=false;
			UpdateWindowTitle();
			return true;
		}
		else
		{
			return false;
		}
	}

	bool PromptSaveUnchanged()
	{
		if(modified)
		{
			INativeDialogService::MessageBoxButtonsOutput result=GetCurrentController()->DialogService()->ShowMessageBox(
				this->GetNativeWindow(),
				L"The document is modified. Do you want to save it before continue?",
				this->GetText(),
				INativeDialogService::DisplayYesNoCancel,
				INativeDialogService::DefaultFirst,
				INativeDialogService::IconQuestion
				);
			switch(result)
			{
			case INativeDialogService::SelectYes:
				return TrySave(false);
			case INativeDialogService::SelectNo:
				return true;
			default:
				return false;
			}
		}
		else
		{
			return true;
		}
	}

	void window_WindowClosing(GuiGraphicsComposition* sender, GuiRequestEventArgs& arguments)
	{
		if(!PromptSaveUnchanged())
		{
			arguments.cancel=true;
		}
	}

	void textBox_TextChanged(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		if(enableModifyFlag)
		{
			modified=true;
		}
	}

	void menuFileNew_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		TryNew();
	}

	void menuFileOpen_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		TryOpen();
	}

	void menuFileSave_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		TrySave(false);
	}

	void menuFileSaveAs_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		TrySave(true);
	}

	void menuFilePrint_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
	}

	void menuFileExit_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		this->Close();
	}

	void menuEditUndo_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
	}

	void menuEditRedo_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
	}

	void menuEditCut_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
	}

	void menuEditCopy_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
	}

	void menuEditPaste_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
	}

	void menuEditDelete_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
	}

	void menuEditSelect_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
	}

	void menuEditGoto_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
	}

	void menuFormatFont_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
	}

	void menuFormatColor_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
	}
public:
	TextEditorWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		,enableModifyFlag(true)
		,modified(false)
	{
		UpdateWindowTitle();

		this->WindowClosing.AttachMethod(this, &TextEditorWindow::window_WindowClosing);

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

			// bind menu events
			menuFileNew->Clicked.AttachMethod(this, &TextEditorWindow::menuFileNew_Clicked);
			menuFileOpen->Clicked.AttachMethod(this, &TextEditorWindow::menuFileOpen_Clicked);
			menuFileSave->Clicked.AttachMethod(this, &TextEditorWindow::menuFileSave_Clicked);
			menuFileSaveAs->Clicked.AttachMethod(this, &TextEditorWindow::menuFileSaveAs_Clicked);
			menuFilePrint->Clicked.AttachMethod(this, &TextEditorWindow::menuFilePrint_Clicked);
			menuFileExit->Clicked.AttachMethod(this, &TextEditorWindow::menuFileExit_Clicked);
			menuEditUndo->Clicked.AttachMethod(this, &TextEditorWindow::menuEditUndo_Clicked);
			menuEditRedo->Clicked.AttachMethod(this, &TextEditorWindow::menuEditRedo_Clicked);
			menuEditCut->Clicked.AttachMethod(this, &TextEditorWindow::menuEditCut_Clicked);
			menuEditCopy->Clicked.AttachMethod(this, &TextEditorWindow::menuEditCopy_Clicked);
			menuEditPaste->Clicked.AttachMethod(this, &TextEditorWindow::menuEditPaste_Clicked);
			menuEditDelete->Clicked.AttachMethod(this, &TextEditorWindow::menuEditDelete_Clicked);
			menuEditSelect->Clicked.AttachMethod(this, &TextEditorWindow::menuEditSelect_Clicked);
			menuEditGoto->Clicked.AttachMethod(this, &TextEditorWindow::menuEditGoto_Clicked);
			menuFormatFont->Clicked.AttachMethod(this, &TextEditorWindow::menuFormatFont_Clicked);
			menuFormatColor->Clicked.AttachMethod(this, &TextEditorWindow::menuFormatColor_Clicked);
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
			textBox->TextChanged.AttachMethod(this, &TextEditorWindow::textBox_TextChanged);
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