﻿/***********************************************************************
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

namespace vm
{
	class IFileFactoryModel : public virtual vl::reflection::IDescriptable, public vl::reflection::Description<IFileFactoryModel>
	{
	public:

		virtual Ptr<presentation::GuiImageData> GetImage() = 0;
		virtual void SetImage(Ptr<presentation::GuiImageData> value) = 0;

		virtual WString GetName() = 0;
		virtual void SetName(WString value) = 0;

		virtual WString GetCategory() = 0;
		virtual void SetCategory(WString value) = 0;

		virtual WString GetDescription() = 0;
		virtual void SetDescription(WString value) = 0;

		virtual WString GetId() = 0;
		virtual void SetId(WString value) = 0;
	};
}

namespace vm
{
	class IProjectFactoryModel : public virtual vl::reflection::IDescriptable, public vl::reflection::Description<IProjectFactoryModel>
	{
	public:

		virtual Ptr<presentation::GuiImageData> GetImage() = 0;
		virtual void SetImage(Ptr<presentation::GuiImageData> value) = 0;

		virtual WString GetName() = 0;
		virtual void SetName(WString value) = 0;

		virtual WString GetDescription() = 0;
		virtual void SetDescription(WString value) = 0;

		virtual WString GetId() = 0;
		virtual void SetId(WString value) = 0;
	};
}

namespace vm
{
	class IStudioModel : public virtual vl::reflection::IDescriptable, public vl::reflection::Description<IStudioModel>
	{
	public:

		virtual collections::LazyList<Ptr<vm::IFileFactoryModel>> GetProjectModels() = 0;
		virtual collections::LazyList<Ptr<vm::IFileFactoryModel>> GetFileModels(WString category) = 0;
	};
}

namespace ui
{
	template<typename TImpl>
	class MainWindow_ : public vl::presentation::controls::GuiWindow, public vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>, public vl::reflection::Description<TImpl>
	{
	private:
		Ptr<vm::IStudioModel> ViewModel_;
	protected:
		vl::presentation::controls::GuiToolstripCommand* commandFileExit;
		vl::presentation::controls::GuiToolstripCommand* commandFileNewFile;
		vl::presentation::controls::GuiToolstripCommand* commandFileNewProject;
		vl::presentation::controls::GuiToolstripCommand* commandFileOpenFile;
		vl::presentation::controls::GuiToolstripCommand* commandFileOpenProject;
		vl::presentation::controls::GuiToolstripCommand* commandFileSave;
		vl::presentation::controls::GuiToolstripCommand* commandFileSaveAll;

		void InitializeComponents(Ptr<vm::IStudioModel> ViewModel)
		{
			ViewModel_ = ViewModel;
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
			else
			{
				ViewModel_ = 0;
			}
		}
	public:
		MainWindow_()
			:vl::presentation::controls::GuiWindow(vl::presentation::theme::GetCurrentTheme()->CreateWindowStyle())
			,vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>(L"ui::MainWindow")
			,commandFileExit(0)
			,commandFileNewFile(0)
			,commandFileNewProject(0)
			,commandFileOpenFile(0)
			,commandFileOpenProject(0)
			,commandFileSave(0)
			,commandFileSaveAll(0)
		{
		}

		Ptr<vm::IStudioModel> GetViewModel()
		{
			return ViewModel_;
		}
	};

	class MainWindow;
}

namespace ui
{
	template<typename TImpl>
	class NewFileWindow_ : public vl::presentation::controls::GuiWindow, public vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>, public vl::reflection::Description<TImpl>
	{
	private:
		Ptr<vm::IStudioModel> ViewModel_;
	protected:
		vl::presentation::controls::GuiButton* buttonCancel;
		vl::presentation::controls::GuiButton* buttonCreate;
		vl::presentation::controls::GuiListView* listViewFileTemplate;
		vl::presentation::controls::GuiSinglelineTextBox* textBoxLocation;
		vl::presentation::controls::GuiSinglelineTextBox* textBoxProjectName;
		vl::presentation::controls::GuiTreeView* treeViewProjectTemplate;

		void InitializeComponents(Ptr<vm::IStudioModel> ViewModel)
		{
			ViewModel_ = ViewModel;
			if (InitializeFromResource())
			{
				GUI_INSTANCE_REFERENCE(buttonCancel);
				GUI_INSTANCE_REFERENCE(buttonCreate);
				GUI_INSTANCE_REFERENCE(listViewFileTemplate);
				GUI_INSTANCE_REFERENCE(textBoxLocation);
				GUI_INSTANCE_REFERENCE(textBoxProjectName);
				GUI_INSTANCE_REFERENCE(treeViewProjectTemplate);
			}
			else
			{
				ViewModel_ = 0;
			}
		}
	public:
		NewFileWindow_()
			:vl::presentation::controls::GuiWindow(vl::presentation::theme::GetCurrentTheme()->CreateWindowStyle())
			,vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>(L"ui::NewFileWindow")
			,buttonCancel(0)
			,buttonCreate(0)
			,listViewFileTemplate(0)
			,textBoxLocation(0)
			,textBoxProjectName(0)
			,treeViewProjectTemplate(0)
		{
		}

		Ptr<vm::IStudioModel> GetViewModel()
		{
			return ViewModel_;
		}
	};

	class NewFileWindow;
}

namespace ui
{
	template<typename TImpl>
	class NewProjectWindow_ : public vl::presentation::controls::GuiWindow, public vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>, public vl::reflection::Description<TImpl>
	{
	private:
		Ptr<vm::IStudioModel> ViewModel_;
	protected:
		vl::presentation::controls::GuiButton* buttonBrowse;
		vl::presentation::controls::GuiButton* buttonCancel;
		vl::presentation::controls::GuiButton* buttonCreate;
		vl::presentation::controls::GuiComboBoxListControl* comboSolution;
		vl::presentation::controls::GuiListView* listViewProjectTemplate;
		vl::presentation::controls::GuiSinglelineTextBox* textBoxLocation;
		vl::presentation::controls::GuiSinglelineTextBox* textBoxProjectName;
		vl::presentation::controls::GuiSinglelineTextBox* textBoxSolutionName;

		void InitializeComponents(Ptr<vm::IStudioModel> ViewModel)
		{
			ViewModel_ = ViewModel;
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
			else
			{
				ViewModel_ = 0;
			}
		}
	public:
		NewProjectWindow_()
			:vl::presentation::controls::GuiWindow(vl::presentation::theme::GetCurrentTheme()->CreateWindowStyle())
			,vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>(L"ui::NewProjectWindow")
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

		Ptr<vm::IStudioModel> GetViewModel()
		{
			return ViewModel_;
		}
	};

	class NewProjectWindow;
}

namespace vl
{
	namespace reflection
	{
		namespace description
		{
			DECL_TYPE_INFO(vm::IFileFactoryModel)
			DECL_TYPE_INFO(vm::IProjectFactoryModel)
			DECL_TYPE_INFO(vm::IStudioModel)
			DECL_TYPE_INFO(ui::MainWindow)
			DECL_TYPE_INFO(ui::NewFileWindow)
			DECL_TYPE_INFO(ui::NewProjectWindow)
		}
	}
}

/*
GsMainWindow.h :
namespace ui
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
		MainWindow(Ptr<vm::IStudioModel> ViewModel);
	};
}


GsMainWindow.cpp :
namespace ui
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

	MainWindow::MainWindow(Ptr<vm::IStudioModel> ViewModel)
	{
		InitializeComponents(ViewModel);
	}
}


GsNewFileWindow.h :
namespace ui
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
		NewFileWindow(Ptr<vm::IStudioModel> ViewModel);
	};
}


GsNewFileWindow.cpp :
namespace ui
{
	// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)

	void NewFileWindow::buttonCancel_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	void NewFileWindow::buttonCreate_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

	NewFileWindow::NewFileWindow(Ptr<vm::IStudioModel> ViewModel)
	{
		InitializeComponents(ViewModel);
	}
}


GsNewProjectWindow.h :
namespace ui
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
		NewProjectWindow(Ptr<vm::IStudioModel> ViewModel);
	};
}


GsNewProjectWindow.cpp :
namespace ui
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

	NewProjectWindow::NewProjectWindow(Ptr<vm::IStudioModel> ViewModel)
	{
		InitializeComponents(ViewModel);
	}
}


*/

#endif
