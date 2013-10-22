#include "DocumentEditorWindow.h"

using namespace vl::collections;
using namespace vl::stream;

/******************************************************************************
File Operation
******************************************************************************/

void DocumentEditorWindow::UpdateWindowTitle()
{
	if(fileName==L"")
	{
		this->SetText(L"Editor.Toolstrip.Document[UNTITLED]");
	}
	else
	{
		this->SetText(L"Editor.Toolstrip.Document["+fileName+L"]");
	}
}

bool DocumentEditorWindow::TryOpen()
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
			L"Open a GacUI Rich Document",
			L"",
			L"",
			L".xml",
			L"GacUI Rich Document Files(*.xml)|*.xml|All Files(*.*)|*.*",
			(INativeDialogService::FileDialogOptions)
				( INativeDialogService::FileDialogAddToRecent
				| INativeDialogService::FileDialogDereferenceLinks
				| INativeDialogService::FileDialogDirectoryMustExist
				| INativeDialogService::FileDialogFileMustExist
				)
			))
		{
			fileName=selectionFileNames[0];
			Ptr<DocumentModel> model=DocumentModel::LoadFromXml(fileName);
			textBox->SetDocument(model);
			textBox->SetCaret(TextPos(0, 0), TextPos(0, 0));
			return true;
		}
	}
	return true;
}

bool DocumentEditorWindow::TrySave(bool ignoreCurrentFileName)
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
			L"Save a GacUI Rich Document",
			L"",
			L"",
			L".xml",
			L"GacUI Rich Document Files(*.xml)|*.xml|All Files(*.*)|*.*",
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
		textBox->GetDocument()->SaveToXml(fileName);
		textBox->NotifyModificationSaved();
	}
	return true;
}

bool DocumentEditorWindow::TryNew()
{
	if(PromptSaveUnchanged())
	{
		fileName=L"";
		textBox->SetText(L"");
		textBox->ClearUndoRedo();
		textBox->NotifyModificationSaved();
		return true;
	}
	else
	{
		return false;
	}
}

bool DocumentEditorWindow::PromptSaveUnchanged()
{
	if(textBox->GetModified())
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

void DocumentEditorWindow::window_WindowClosing(GuiGraphicsComposition* sender, GuiRequestEventArgs& arguments)
{
	if(!PromptSaveUnchanged())
	{
		arguments.cancel=true;
	}
}

/******************************************************************************
Command Updating
******************************************************************************/

void DocumentEditorWindow::UpdateMenuItems(int commands)
{
	if(commands & ClipboardCommands)
	{
		commandEditPaste->SetEnabled(GetCurrentController()->ClipboardService()->ContainsText());
	}
	if(commands & SelectionCommands)
	{
		commandEditCut->SetEnabled(textBox->CanCut());
		commandEditCopy->SetEnabled(textBox->CanCopy());
		commandEditDelete->SetEnabled(textBox->GetCaretBegin()!=textBox->GetCaretEnd());
	}
	if(commands & UndoRedoCommands)
	{
		commandEditUndo->SetEnabled(textBox->CanUndo());
		commandEditRedo->SetEnabled(textBox->CanRedo());
	}
}

void DocumentEditorWindow::window_ClipboardUpdated(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
	UpdateMenuItems(ClipboardCommands);
}

void DocumentEditorWindow::textBox_SelectionChanged(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
	UpdateMenuItems(SelectionCommands);
}

void DocumentEditorWindow::textBox_TextChanged(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
	UpdateMenuItems(UndoRedoCommands);
}

/******************************************************************************
Menu Handlers
******************************************************************************/

void DocumentEditorWindow::commandFileNew_Executed(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
	TryNew();
	UpdateMenuItems(AllCommands);
	UpdateWindowTitle();
}

void DocumentEditorWindow::commandFileOpen_Executed(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
	TryOpen();
	UpdateMenuItems(AllCommands);
	UpdateWindowTitle();
}

void DocumentEditorWindow::commandFileSave_Executed(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
	TrySave(false);
	UpdateMenuItems(AllCommands);
	UpdateWindowTitle();
}

void DocumentEditorWindow::commandFileSaveAs_Executed(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
	TrySave(true);
	UpdateMenuItems(AllCommands);
	UpdateWindowTitle();
}

void DocumentEditorWindow::commandFileExit_Executed(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
	this->Close();
}

void DocumentEditorWindow::commandEditUndo_Executed(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
	textBox->Undo();
}

void DocumentEditorWindow::commandEditRedo_Executed(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
	textBox->Redo();
}

void DocumentEditorWindow::commandEditCut_Executed(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
	textBox->Cut();
}

void DocumentEditorWindow::commandEditCopy_Executed(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
	textBox->Copy();
}

void DocumentEditorWindow::commandEditPaste_Executed(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
	textBox->Paste();
}

void DocumentEditorWindow::commandEditDelete_Executed(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
	textBox->SetSelectionText(L"");
}

void DocumentEditorWindow::commandEditSelect_Executed(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
	textBox->SelectAll();
}

void DocumentEditorWindow::commandStyleBold_Executed(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
}

void DocumentEditorWindow::commandStyleItalic_Executed(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
}

void DocumentEditorWindow::commandStyleUnderline_Executed(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
}

void DocumentEditorWindow::commandStyleStrikeline_Executed(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
}

void DocumentEditorWindow::commandStyleFont_Executed(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
	FontProperties font;
	Color color;
	if(GetCurrentController()->DialogService()->ShowFontDialog(
		this->GetNativeWindow(),
		font,
		color,
		false,
		false,
		true))
	{
	}
}

void DocumentEditorWindow::commandStyleForeColor_Executed(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
	Color color;
	if(GetCurrentController()->DialogService()->ShowColorDialog(
		this->GetNativeWindow(),
		color))
	{
	}
}

void DocumentEditorWindow::commandStyleBackColor_Executed(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
	Color color;
	if(GetCurrentController()->DialogService()->ShowColorDialog(
		this->GetNativeWindow(),
		color))
	{
	}
}

void DocumentEditorWindow::commandStyleAlignLeft_Executed(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
}

void DocumentEditorWindow::commandStyleAlignCenter_Executed(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
}

void DocumentEditorWindow::commandStyleAlignRight_Executed(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
}

DocumentEditorWindow::DocumentEditorWindow()
	:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
{
	InitializeComponents();
	UpdateWindowTitle();
	UpdateMenuItems(AllCommands);

	this->WindowClosing.AttachMethod(this, &DocumentEditorWindow::window_WindowClosing);
	this->ClipboardUpdated.AttachMethod(this, &DocumentEditorWindow::window_ClipboardUpdated);

	textBox->TextChanged.AttachMethod(this, &DocumentEditorWindow::textBox_TextChanged);
	textBox->SelectionChanged.AttachMethod(this, &DocumentEditorWindow::textBox_SelectionChanged);

	commandFileNew->Executed.AttachMethod(this, &DocumentEditorWindow::commandFileNew_Executed);
	commandFileOpen->Executed.AttachMethod(this, &DocumentEditorWindow::commandFileOpen_Executed);
	commandFileSave->Executed.AttachMethod(this, &DocumentEditorWindow::commandFileSave_Executed);
	commandFileSaveAs->Executed.AttachMethod(this, &DocumentEditorWindow::commandFileSaveAs_Executed);
	commandFileExit->Executed.AttachMethod(this, &DocumentEditorWindow::commandFileExit_Executed);
	commandEditUndo->Executed.AttachMethod(this, &DocumentEditorWindow::commandEditUndo_Executed);
	commandEditRedo->Executed.AttachMethod(this, &DocumentEditorWindow::commandEditRedo_Executed);
	commandEditCut->Executed.AttachMethod(this, &DocumentEditorWindow::commandEditCut_Executed);
	commandEditCopy->Executed.AttachMethod(this, &DocumentEditorWindow::commandEditCopy_Executed);
	commandEditPaste->Executed.AttachMethod(this, &DocumentEditorWindow::commandEditPaste_Executed);
	commandEditDelete->Executed.AttachMethod(this, &DocumentEditorWindow::commandEditDelete_Executed);
	commandEditSelect->Executed.AttachMethod(this, &DocumentEditorWindow::commandEditSelect_Executed);
	commandStyleBold->Executed.AttachMethod(this, &DocumentEditorWindow::commandStyleBold_Executed);
	commandStyleItalic->Executed.AttachMethod(this, &DocumentEditorWindow::commandStyleItalic_Executed);
	commandStyleUnderline->Executed.AttachMethod(this, &DocumentEditorWindow::commandStyleUnderline_Executed);
	commandStyleStrikeline->Executed.AttachMethod(this, &DocumentEditorWindow::commandStyleStrikeline_Executed);
	commandStyleFont->Executed.AttachMethod(this, &DocumentEditorWindow::commandStyleFont_Executed);
	commandStyleForeColor->Executed.AttachMethod(this, &DocumentEditorWindow::commandStyleForeColor_Executed);
	commandStyleBackColor->Executed.AttachMethod(this, &DocumentEditorWindow::commandStyleBackColor_Executed);
	commandStyleAlignLeft->Executed.AttachMethod(this, &DocumentEditorWindow::commandStyleAlignLeft_Executed);
	commandStyleAlignCenter->Executed.AttachMethod(this, &DocumentEditorWindow::commandStyleAlignCenter_Executed);
	commandStyleAlignRight->Executed.AttachMethod(this, &DocumentEditorWindow::commandStyleAlignRight_Executed);
}

DocumentEditorWindow::~DocumentEditorWindow()
{
}