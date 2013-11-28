/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
GacUI::Partial Classes

本文件使用Vczh GacUI Resource Code Generator工具自动生成
************************************************************************
DO NOT MODIFY
***********************************************************************/

#ifndef VCZH_GACUI_RESOURCE_CODE_GENERATOR_GacStudioUI_PARTIAL_CLASSES
#define VCZH_GACUI_RESOURCE_CODE_GENERATOR_GacStudioUI_PARTIAL_CLASSES

#include "..\..\..\Public\Source\GacUIReflection.h"

namespace gacstudio
{
	namespace user_interface
	{
		template<typename TImpl>
		class MainWindow_ : public vl::presentation::controls::GuiWindow, public vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>, public vl::reflection::Description<TImpl>
		{
		protected:
			vl::presentation::controls::GuiToolstripCommand* commandFileExit;
			vl::presentation::controls::GuiToolstripCommand* commandFileNewFile;
			vl::presentation::controls::GuiToolstripCommand* commandFileNewProject;
			vl::presentation::controls::GuiToolstripCommand* commandFileOpenFile;
			vl::presentation::controls::GuiToolstripCommand* commandFileOpenProject;
			vl::presentation::controls::GuiToolstripCommand* commandFileSave;
			vl::presentation::controls::GuiToolstripCommand* commandFileSaveAll;

		void InitializeComponents()
			{
				if (InitializeFromResource())
				{
					GUI_INSTANCE_REFERENCE(commandFileExit);
					GUI_INSTANCE_REFERENCE(commandFileNewFile);
					GUI_INSTANCE_REFERENCE(commandFileNewProject);
					GUI_INSTANCE_REFERENCE(commandFileOpenFile);
					GUI_INSTANCE_REFERENCE(commandFileOpenProject);
					GUI_INSTANCE_REFERENCE(commandFileSave);
					GUI_INSTANCE_REFERENCE(commandFileSaveAll);
				}
			}
		public:
			MainWindow_()
				:vl::presentation::controls::GuiWindow(vl::presentation::theme::GetCurrentTheme()->CreateWindowStyle())
				,vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>(L"gacstudio::user_interface::MainWindow")
				,commandFileExit(0)
				,commandFileNewFile(0)
				,commandFileNewProject(0)
				,commandFileOpenFile(0)
				,commandFileOpenProject(0)
				,commandFileSave(0)
				,commandFileSaveAll(0)
			{
			}
		};

		class MainWindow;
	}
}

namespace vl
{
	namespace reflection
	{
		namespace description
		{
			DECL_TYPE_INFO(gacstudio::user_interface::MainWindow)
		}
	}
}

/*
GsMainWindow.h :
namespace gacstudio
{
	namespace user_interface
	{
		class MainWindow : public MainWindow_<MainWindow>
		{
			friend class MainWindow_<MainWindow>;
			friend struct vl::reflection::description::CustomTypeDescriptorSelector<MainWindow>;
		protected:

			// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)
			void commandFileExit_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
			void commandFileNewFile_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
			void commandFileNewProject_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
			void commandFileOpenFile_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
			void commandFileOpenProject_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
			void commandFileSaveAll_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
			void commandFileSave_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
			// #endregion CLASS_MEMBER_GUIEVENT_HANDLER
		public:
			MainWindow();
		};
	}
}


GsMainWindow.cpp :
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


*/

#endif
