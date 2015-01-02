/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
GacUI::NewFileWindow

本文件使用Vczh GacUI Resource Code Generator工具自动生成
***********************************************************************/

#include "GacStudioUI.h"

using namespace vl::filesystem;
using namespace vm;
using namespace vl::reflection::description;

namespace ui
{
	// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)

	void NewFileWindow::buttonCancel_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
		Close();
	}

	void NewFileWindow::buttonCreate_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
		auto model = GetViewModel();
		auto workingProject = model->GetWorkingProject();
		auto fileFactory = UnboxValue<Ptr<IFileFactoryModel>>(listViewFileTemplate->GetSelectedItem());
		if (!workingProject || !fileFactory)
		{
			model->PromptError(L"Failed to add a file.");
			goto CLOSE;
		}

		{
			auto fileFolder = Folder(textBoxLocation->GetText());
			auto filePath = fileFolder.GetFilePath() / (textBoxFileName->GetText() + fileFactory->GetDefaultFileExt());

			if (!fileFolder.Exists() && !fileFolder.Create(true))
			{
				model->PromptError(L"Failed to create folder \"" + fileFolder.GetFilePath().GetFullPath() + L"\".");
				return;
			}

			auto fileItem = model->AddFile(workingProject, fileFactory, filePath.GetFullPath());
			if (!fileItem)
			{
				model->PromptError(L"Failed to add a file of \"" + fileFactory->GetName() + L"\" to project \"" + workingProject->GetName() + L"\".");
				goto CLOSE;
			}
			else if (!fileItem->NewFile())
			{
				model->PromptError(L"Failed to create a file of \"" + fileFactory->GetName() + L"\".");
				// goto CLOSE;
			}
			if (!workingProject->SaveProject(false))
			{
				model->PromptError(L"Failed to save project \"" + workingProject->GetName() + L"\".");
				goto CLOSE;
			}

		}
	CLOSE:
		Close();
	}

	// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

	NewFileWindow::NewFileWindow(Ptr<vm::IStudioModel> ViewModel)
	{
		InitializeComponents(ViewModel);
	}
}
