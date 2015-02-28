﻿/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
GacUI::AddExistingFilesWindow

This file is generated by: Vczh GacUI Resource Code Generator
***********************************************************************/

#include "GacStudioUI.h"

namespace ui
{
	// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)

	void AddExistingFilesWindow::buttonAdd_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
		if (dialogOpen->ShowDialog())
		{
			GetOperationModel()->AddFiles(dialogOpen->GetFileNames());
		}
	}

	void AddExistingFilesWindow::buttonCancel_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	void AddExistingFilesWindow::buttonOK_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	void AddExistingFilesWindow::buttonRemove_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
		GetOperationModel()->RemoveFiles(dataGridFiles->GetSelectedItems());
	}

	// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

	AddExistingFilesWindow::AddExistingFilesWindow(Ptr<vm::IStudioModel> ViewModel, Ptr<vm::IStudioAddExistingFilesModel> OperationModel, Ptr<vm::IAddFileItemAction> Action)
	{
		InitializeComponents(ViewModel, OperationModel, Action);
	}
}
