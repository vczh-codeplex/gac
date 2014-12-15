/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
GacUI::NewProjectWindow

本文件使用Vczh GacUI Resource Code Generator工具自动生成
***********************************************************************/

#include "GacStudioUI.h"

using namespace vl::filesystem;
using namespace vm;
using namespace vl::reflection::description;

namespace ui
{
	// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)

	void NewProjectWindow::buttonBrowse_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	void NewProjectWindow::buttonCancel_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
		Close();
	}

	void NewProjectWindow::buttonCreate_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
		auto model = GetViewModel();
		auto projectFactory = UnboxValue<Ptr<IProjectFactoryModel>>(listViewProjectTemplate->GetSelectedItem());
		if (!projectFactory)
		{
			model->PromptError(L"Failed to add a project of \"" + projectFactory->GetName() + L"\".");
			goto CLOSE;
		}

		auto solutionFolder = Folder(FilePath(textBoxLocation->GetText()) / textBoxSolutionName->GetText());
		auto solutionPath = solutionFolder.GetFilePath() / (textBoxSolutionName->GetText() + L".gacsln.xml");
		auto projectFolder = Folder(solutionFolder.GetFilePath() / textBoxProjectName->GetText());
		auto projectPath = projectFolder.GetFilePath() / (textBoxProjectName->GetText() + L".gacproj.xml");

		if (!solutionFolder.Exists() && !solutionFolder.Create(true))
		{
			model->PromptError(L"Failed to create folder \"" + solutionFolder.GetFilePath().GetFullPath() + L"\".");
			goto CLOSE;
		}
		if (!projectFolder.Exists() && !projectFolder.Create(true))
		{
			model->PromptError(L"Failed to create folder \"" + projectFolder.GetFilePath().GetFullPath() + L"\".");
			goto CLOSE;
		}

		if (comboSolution->GetSelectedIndex() == 0)
		{
			if (!model->NewSolution(solutionPath.GetFullPath()))
			{
				goto CLOSE;
			}
		}
		if (!model->AddProject(projectFactory, projectPath.GetFullPath()))
		{
			model->PromptError(L"Failed to add a project of \"" + projectFactory->GetName() + L"\".");
			goto CLOSE;
		}
		if (!model->SaveSolution())
		{
			model->PromptError(L"Failed to save the solution.");
			goto CLOSE;
		}
	CLOSE:
		Close();
	}

	// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

	NewProjectWindow::NewProjectWindow(Ptr<vm::IStudioModel> ViewModel)
	{
		InitializeComponents(ViewModel);
	}
}
