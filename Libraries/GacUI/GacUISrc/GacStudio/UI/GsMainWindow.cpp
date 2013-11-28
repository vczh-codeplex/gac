/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
GacUI::MainWindow

本文件使用Vczh GacUI Resource Code Generator工具自动生成
***********************************************************************/

#include "GacStudioUI.h"

namespace gacstudio
{
	namespace user_interface
	{
		// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)

		void MainWindow::commandFileExit_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
		{
		}

		void MainWindow::commandFileNewFile_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
		{
		}

		void MainWindow::commandFileNewProject_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
		{
			SetEnabled(false);
			auto window = new NewProjectWindow;
			window->ForceCalculateSizeImmediately();
			window->MoveToScreenCenter();
			window->GetNativeWindow()->SetParent(GetNativeWindow());
			window->WindowClosed.AttachLambda([=](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
			{
				GetApplication()->InvokeInMainThread([=]()
				{
					window->GetNativeWindow()->SetParent(0);
					SetEnabled(true);
					SetActivated();
					delete window;
				});
			});
			window->Show();
		}

		void MainWindow::commandFileOpenFile_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
		{
		}

		void MainWindow::commandFileOpenProject_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
		{
		}

		void MainWindow::commandFileSaveAll_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
		{
		}

		void MainWindow::commandFileSave_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
		{
		}

		// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

		MainWindow::MainWindow()
		{
			InitializeComponents();
		}
	}
}





