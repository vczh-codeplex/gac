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

namespace gacstudio
{
	namespace user_interface
	{
		template<typename TImpl>
		class NewFileWindow_ : public vl::presentation::controls::GuiWindow, public vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>, public vl::reflection::Description<TImpl>
		{
		protected:
			vl::presentation::controls::GuiButton* buttonCancel;
			vl::presentation::controls::GuiButton* buttonCreate;
			vl::presentation::controls::GuiListView* listViewFileTemplate;
			vl::presentation::controls::GuiSinglelineTextBox* textBoxLocation;
			vl::presentation::controls::GuiSinglelineTextBox* textBoxProjectName;
			vl::presentation::controls::GuiTreeView* treeViewProjectTemplate;

		void InitializeComponents()
			{
				if (InitializeFromResource())
				{
					GUI_INSTANCE_REFERENCE(buttonCancel);
					GUI_INSTANCE_REFERENCE(buttonCreate);
					GUI_INSTANCE_REFERENCE(listViewFileTemplate);
					GUI_INSTANCE_REFERENCE(textBoxLocation);
					GUI_INSTANCE_REFERENCE(textBoxProjectName);
					GUI_INSTANCE_REFERENCE(treeViewProjectTemplate);
				}
			}
		public:
			NewFileWindow_()
				:vl::presentation::controls::GuiWindow(vl::presentation::theme::GetCurrentTheme()->CreateWindowStyle())
				,vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>(L"gacstudio::user_interface::NewFileWindow")
				,buttonCancel(0)
				,buttonCreate(0)
				,listViewFileTemplate(0)
				,textBoxLocation(0)
				,textBoxProjectName(0)
				,treeViewProjectTemplate(0)
			{
			}
		};

		class NewFileWindow;
	}
}

namespace gacstudio
{
	namespace user_interface
	{
		template<typename TImpl>
		class NewProjectWindow_ : public vl::presentation::controls::GuiWindow, public vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>, public vl::reflection::Description<TImpl>
		{
		protected:
			vl::presentation::controls::GuiButton* buttonBrowse;
			vl::presentation::controls::GuiButton* buttonCancel;
			vl::presentation::controls::GuiButton* buttonCreate;
			vl::presentation::controls::GuiComboBoxListControl* comboSolution;
			vl::presentation::controls::GuiListView* listViewProjectTemplate;
			vl::presentation::controls::GuiSinglelineTextBox* textBoxLocation;
			vl::presentation::controls::GuiSinglelineTextBox* textBoxProjectName;
			vl::presentation::controls::GuiSinglelineTextBox* textBoxSolutionName;

		void InitializeComponents()
			{
				if (InitializeFromResource())
				{
					GUI_INSTANCE_REFERENCE(buttonBrowse);
					GUI_INSTANCE_REFERENCE(buttonCancel);
					GUI_INSTANCE_REFERENCE(buttonCreate);
					GUI_INSTANCE_REFERENCE(comboSolution);
					GUI_INSTANCE_REFERENCE(listViewProjectTemplate);
					GUI_INSTANCE_REFERENCE(textBoxLocation);
					GUI_INSTANCE_REFERENCE(textBoxProjectName);
					GUI_INSTANCE_REFERENCE(textBoxSolutionName);
				}
			}
		public:
			NewProjectWindow_()
				:vl::presentation::controls::GuiWindow(vl::presentation::theme::GetCurrentTheme()->CreateWindowStyle())
				,vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>(L"gacstudio::user_interface::NewProjectWindow")
				,buttonBrowse(0)
				,buttonCancel(0)
				,buttonCreate(0)
				,comboSolution(0)
				,listViewProjectTemplate(0)
				,textBoxLocation(0)
				,textBoxProjectName(0)
				,textBoxSolutionName(0)
			{
			}
		};

		class NewProjectWindow;
	}
}

namespace vl
{
	namespace reflection
	{
		namespace description
		{
			DECL_TYPE_INFO(gacstudio::user_interface::MainWindow)
			DECL_TYPE_INFO(gacstudio::user_interface::NewFileWindow)
			DECL_TYPE_INFO(gacstudio::user_interface::NewProjectWindow)
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


GsNewFileWindow.h :
namespace gacstudio
{
	namespace user_interface
	{
		class NewFileWindow : public NewFileWindow_<NewFileWindow>
		{
			friend class NewFileWindow_<NewFileWindow>;
			friend struct vl::reflection::description::CustomTypeDescriptorSelector<NewFileWindow>;
		protected:

			// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)
			void buttonCancel_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
			void buttonCreate_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
			// #endregion CLASS_MEMBER_GUIEVENT_HANDLER
		public:
			NewFileWindow();
		};
	}
}


GsNewFileWindow.cpp :
namespace gacstudio
{
	namespace user_interface
	{
		// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)

		void NewFileWindow::buttonCancel_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
		{
		}

		void NewFileWindow::buttonCreate_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
		{
		}

		// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

		NewFileWindow::NewFileWindow()
		{
			InitializeComponents();
		}
	}
}


GsNewProjectWindow.h :
namespace gacstudio
{
	namespace user_interface
	{
		class NewProjectWindow : public NewProjectWindow_<NewProjectWindow>
		{
			friend class NewProjectWindow_<NewProjectWindow>;
			friend struct vl::reflection::description::CustomTypeDescriptorSelector<NewProjectWindow>;
		protected:

			// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)
			void buttonBrowse_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
			void buttonCancel_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
			void buttonCreate_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
			// #endregion CLASS_MEMBER_GUIEVENT_HANDLER
		public:
			NewProjectWindow();
		};
	}
}


GsNewProjectWindow.cpp :
namespace gacstudio
{
	namespace user_interface
	{
		// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)

		void NewProjectWindow::buttonBrowse_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
		{
		}

		void NewProjectWindow::buttonCancel_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
		{
		}

		void NewProjectWindow::buttonCreate_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
		{
		}

		// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

		NewProjectWindow::NewProjectWindow()
		{
			InitializeComponents();
		}
	}
}


*/

#endif
