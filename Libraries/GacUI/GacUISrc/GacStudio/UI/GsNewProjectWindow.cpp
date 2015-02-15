/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
GacUI::NewProjectWindow

本文件使用Vczh GacUI Resource Code Generator工具自动生成
***********************************************************************/

#include "GacStudioUI.h"
#include "..\Studio\SolutionModel.h"

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
			model->PromptError(L"Failed to add a project.");
			Close();
		}

		try
		{
			auto solutionFolder =
				comboSolution->GetSelectedIndex() == 0
				? Folder(FilePath(textBoxLocation->GetText()) / textBoxSolutionName->GetText())
				: Folder(FilePath(model->GetOpenedSolution()->GetFileDirectory()))
				;
			auto solutionPath = solutionFolder.GetFilePath() / (textBoxSolutionName->GetText() + L".gacsln.xml");
			auto projectFolder = Folder(solutionFolder.GetFilePath() / textBoxProjectName->GetText());
			auto projectPath = projectFolder.GetFilePath() / (textBoxProjectName->GetText() + L".gacproj.xml");

			if (comboSolution->GetSelectedIndex() == 0)
			{
				if (!solutionFolder.Create(true))
				{
					model->PromptError(L"Failed to create empty folder \"" + solutionFolder.GetFilePath().GetFullPath() + L"\".");
					return;
				}
			}
			if (!projectFolder.Create(true))
			{
				model->PromptError(L"Failed to create empty folder \"" + projectFolder.GetFilePath().GetFullPath() + L"\".");
				return;
			}

			if (comboSolution->GetSelectedIndex() == 0)
			{
				model->NewSolution(solutionPath.GetFullPath());
			}

			auto projectItem = model->CreateProjectModel(projectFactory, projectPath.GetFullPath());
			model->GetOpenedSolution()->AddProject(projectItem);
			model->GetOpenedSolution()->SaveSolution(false);
		}
		catch (const vm::StudioException& ex)
		{
			if (ex.IsNonConfigError())
			{
				Close();
			}
		}
	}

	// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

	NewProjectWindow::NewProjectWindow(Ptr<vm::IStudioModel> ViewModel)
	{
		InitializeComponents(ViewModel);
	}
}
