/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
GacUI::MainWindow

本文件使用Vczh GacUI Resource Code Generator工具自动生成
***********************************************************************/

#include "GacStudioUI.h"

using namespace vl::collections;

namespace ui
{
	// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)

	void MainWindow::cellSplitter_leftButtonDown(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiMouseEventArgs& arguments)
	{
		splitterDragging = true;
		splitterPosition = cellSplitter->GetGlobalBounds().Left() + arguments.x;
	}

	void MainWindow::cellSplitter_leftButtonUp(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiMouseEventArgs& arguments)
	{
		splitterDragging = false;
	}

	void MainWindow::cellSplitter_mouseMove(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiMouseEventArgs& arguments)
	{
		if (splitterDragging)
		{
			vint delta = cellSplitter->GetGlobalBounds().Left() + arguments.x - splitterPosition;
			splitterPosition += delta;

			auto option = tableMain->GetColumnOption(0);
			option.absolute += delta;
			tableMain->SetColumnOption(0, option);
			tableMain->ForceCalculateSizeImmediately();
		}
	}

	void MainWindow::commandFileAddExistingFiles_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	void MainWindow::commandFileAddNewFile_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
		auto window = new NewFileWindow(GetViewModel());
		window->ForceCalculateSizeImmediately();
		window->MoveToScreenCenter();
		window->ShowModalAndDelete(this, []()
		{
		});
	}

	void MainWindow::commandFileCloseSolution_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
		GetViewModel()->CloseSolution();
	}

	void MainWindow::commandFileExit_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
		Close();
	}

	void MainWindow::commandFileNewProject_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
		auto window = new NewProjectWindow(GetViewModel());
		window->ForceCalculateSizeImmediately();
		window->MoveToScreenCenter();
		window->ShowModalAndDelete(this, []()
		{
		});
	}

	void MainWindow::commandFileOpenProject_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
		auto model = GetViewModel();
		List<WString> fileNames;
		vint filterIndex = 0;
		if (GetCurrentController()->DialogService()->ShowFileDialog(
			GetNativeWindow(),
			fileNames,
			filterIndex,
			INativeDialogService::FileDialogOpen,
			L"Open Solution",
			L"",
			L"",
			L"gacsln.xml",
			L"Gac Studio Solution (*.gacsln.xml)|*.gacsln.xml",
			(INativeDialogService::FileDialogOptions)(
				INativeDialogService::FileDialogDereferenceLinks |
				INativeDialogService::FileDialogFileMustExist
			)))
		{
			model->OpenSolution(fileNames[0]);
		}
	}

	void MainWindow::commandFileOpenWith_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	void MainWindow::commandFileOpen_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	void MainWindow::commandFileRemove_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	void MainWindow::commandFileRename_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	void MainWindow::commandFileSaveAll_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	void MainWindow::commandFileSave_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	void MainWindow::commandHelpAbout_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
		auto window = new AboutWindow(GetViewModel());
		window->ForceCalculateSizeImmediately();
		window->MoveToScreenCenter();
		window->ShowModalAndDelete(this, []()
		{
		});
	}

	void MainWindow::treeViewSolutionItem_NodeRightButtonUp(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiNodeMouseEventArgs& arguments)
	{
		toolstripMenuSolutionItem->ShowPopup(treeViewSolutionItem, Point(arguments.x, arguments.y));
	}

	void MainWindow::treeViewSolutionItem_SelectionChanged(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
		selectedSolutionItem = description::UnboxValue<Ptr<vm::ISolutionItemModel>>(treeViewSolutionItem->GetSelectedItem());
		bool canAddFile = selectedSolutionItem && !selectedSolutionItem.Cast<vm::ISolutionModel>();
		bool canEditProject = selectedSolutionItem.Cast<vm::IProjectModel>();
		bool canEditFile = selectedSolutionItem.Cast<vm::IFileModel>();
		commandFileAddNewFile->SetEnabled(canAddFile);
		commandFileAddExistingFiles->SetEnabled(canAddFile);
		commandFileOpen->SetEnabled(canEditFile);
		commandFileOpenWith->SetEnabled(canEditFile);
		commandFileRename->SetEnabled(canEditFile || canEditProject);
		commandFileRemove->SetEnabled(canEditFile || canEditProject);
		GetViewModel()->NotifySelectedSolutionItem(selectedSolutionItem);
	}

	// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

	MainWindow::MainWindow(Ptr<vm::IStudioModel> ViewModel)
	{
		InitializeComponents(ViewModel);
		cellSplitter->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetSystemCursor(INativeCursor::SizeWE));
	}
}
