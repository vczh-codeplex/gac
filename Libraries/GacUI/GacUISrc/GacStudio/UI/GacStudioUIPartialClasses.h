﻿/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
GacUI::Partial Classes

This file is generated by: Vczh GacUI Resource Code Generator
************************************************************************
DO NOT MODIFY
***********************************************************************/

#ifndef VCZH_GACUI_RESOURCE_CODE_GENERATOR_GacStudioUI_PARTIAL_CLASSES
#define VCZH_GACUI_RESOURCE_CODE_GENERATOR_GacStudioUI_PARTIAL_CLASSES

#include "..\..\..\Public\Source\GacUIReflection.h"

namespace vm
{
	class ISolutionItemModel : public virtual vl::reflection::IDescriptable, public vl::reflection::Description<ISolutionItemModel>
	{
	public:

		virtual Ptr<presentation::GuiImageData> GetImage() = 0;
		vl::Event<void()> ImageChanged;

		virtual WString GetName() = 0;
		vl::Event<void()> NameChanged;

		virtual Ptr<description::IValueObservableList> GetChildren() = 0;

		virtual bool GetIsFileItem() = 0;

		virtual WString GetFilePath() = 0;
		vl::Event<void()> FilePathChanged;

		virtual bool GetIsSaved() = 0;
		vl::Event<void()> IsSavedChanged;

		virtual vint GetErrorCount() = 0;
		vl::Event<void()> ErrorCountChanged;

		virtual WString GetErrorText(vint index) = 0;
		virtual bool OpenFileItem() = 0;
		virtual bool SaveFileItem() = 0;
	};
}

namespace vm
{
	class IFileFactoryModel : public virtual vl::reflection::IDescriptable, public vl::reflection::Description<IFileFactoryModel>
	{
	public:

		virtual Ptr<presentation::GuiImageData> GetImage() = 0;

		virtual WString GetName() = 0;

		virtual WString GetCategory() = 0;

		virtual WString GetDescription() = 0;

		virtual WString GetId() = 0;

		virtual WString GetDefaultFileExt() = 0;

		virtual bool GenerateFile(Ptr<vm::ISolutionItemModel> , WString filePath) = 0;
	};
}

namespace vm
{
	class IProjectFactoryModel : public virtual vl::reflection::IDescriptable, public vl::reflection::Description<IProjectFactoryModel>
	{
	public:

		virtual Ptr<presentation::GuiImageData> GetImage() = 0;

		virtual Ptr<presentation::GuiImageData> GetSmallImage() = 0;

		virtual WString GetName() = 0;

		virtual WString GetDescription() = 0;

		virtual WString GetId() = 0;

		virtual collections::LazyList<Ptr<vm::IProjectFactoryModel>> GetChildren() = 0;
	};
}

namespace vm
{
	class IFileModel : public virtual vm::ISolutionItemModel, public vl::reflection::Description<IFileModel>
	{
	public:
	};
}

namespace vm
{
	class IFolderModel : public virtual vm::ISolutionItemModel, public vl::reflection::Description<IFolderModel>
	{
	public:
	};
}

namespace vm
{
	class IProjectModel : public virtual vm::ISolutionItemModel, public vl::reflection::Description<IProjectModel>
	{
	public:

		virtual Ptr<vm::IProjectFactoryModel> GetProjectFactory() = 0;
		virtual bool OpenProject() = 0;
		virtual bool SaveProject(bool saveContainingFiles) = 0;
		virtual bool NewProject() = 0;
	};
}

namespace vm
{
	class ISolutionModel : public virtual vm::ISolutionItemModel, public vl::reflection::Description<ISolutionModel>
	{
	public:

		virtual bool OpenSolution() = 0;
		virtual bool SaveSolution(bool saveContainingProjects) = 0;
		virtual bool NewSolution() = 0;
		virtual bool AddProject(Ptr<vm::IProjectModel> project) = 0;
	};
}

namespace vm
{
	class IStudioModel : public virtual vl::reflection::IDescriptable, public vl::reflection::Description<IStudioModel>
	{
	public:

		virtual collections::LazyList<Ptr<vm::IProjectFactoryModel>> GetProjectModels() = 0;

		virtual Ptr<presentation::description::IValueObservableList> GetFileModels() = 0;

		virtual Ptr<vm::IProjectFactoryModel> GetFileFilters() = 0;

		virtual WString GetFileCategory() = 0;
		virtual void SetFileCategory(WString value) = 0;

		virtual Ptr<ISolutionItemModel> GetOpeningSolution() = 0;

		virtual WString GetOpeningSolutionPath() = 0;

		virtual bool GetHasOpeningSolution() = 0;
		vl::Event<void()> HasOpeningSolutionChanged;

		virtual bool OpenSolution(WString filePath) = 0;
		virtual bool SaveSolution() = 0;
		virtual bool NewSolution(WString filePath) = 0;
		virtual bool CloseSolution() = 0;
		virtual bool AddProject(Ptr<vm::IProjectFactoryModel> projectFactory, WString filePath) = 0;
		virtual void OpenBrowser(WString url) = 0;
		virtual void PromptError(WString message) = 0;
	};
}

namespace ui
{
	template<typename TImpl>
	class AboutWindow_ : public vl::presentation::controls::GuiWindow, public vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>, public vl::reflection::Description<TImpl>
	{
	private:
		Ptr<vm::IStudioModel> ViewModel_;
	protected:
		vl::presentation::controls::GuiDocumentViewer* documentViewer;
		vl::presentation::controls::GuiWindow* self;

		void InitializeComponents(Ptr<vm::IStudioModel> ViewModel)
		{
			ViewModel_ = ViewModel;
			if (InitializeFromResource())
			{
				GUI_INSTANCE_REFERENCE(documentViewer);
				GUI_INSTANCE_REFERENCE(self);
			}
			else
			{
				ViewModel_ = 0;
			}
		}
	public:
		AboutWindow_()
			:vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>(L"ui::AboutWindow")
			,vl::presentation::controls::GuiWindow(vl::presentation::theme::GetCurrentTheme()->CreateWindowStyle())
			,documentViewer(0)
			,self(0)
		{
		}

		Ptr<vm::IStudioModel> GetViewModel()
		{
			return ViewModel_;
		}
	};

	class AboutWindow;
}

namespace ui
{
	template<typename TImpl>
	class MainWindow_ : public vl::presentation::controls::GuiWindow, public vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>, public vl::reflection::Description<TImpl>
	{
	private:
		Ptr<vm::IStudioModel> ViewModel_;
	protected:
		vl::presentation::compositions::GuiCellComposition* cellSplitter;
		vl::presentation::controls::GuiToolstripCommand* commandFileExit;
		vl::presentation::controls::GuiToolstripCommand* commandFileNewFile;
		vl::presentation::controls::GuiToolstripCommand* commandFileNewProject;
		vl::presentation::controls::GuiToolstripCommand* commandFileOpenFile;
		vl::presentation::controls::GuiToolstripCommand* commandFileOpenProject;
		vl::presentation::controls::GuiToolstripCommand* commandFileSave;
		vl::presentation::controls::GuiToolstripCommand* commandFileSaveAll;
		vl::presentation::controls::GuiToolstripCommand* commandHelpAbout;
		vl::presentation::compositions::GuiTableComposition* tableMain;

		void InitializeComponents(Ptr<vm::IStudioModel> ViewModel)
		{
			ViewModel_ = ViewModel;
			if (InitializeFromResource())
			{
				GUI_INSTANCE_REFERENCE(cellSplitter);
				GUI_INSTANCE_REFERENCE(commandFileExit);
				GUI_INSTANCE_REFERENCE(commandFileNewFile);
				GUI_INSTANCE_REFERENCE(commandFileNewProject);
				GUI_INSTANCE_REFERENCE(commandFileOpenFile);
				GUI_INSTANCE_REFERENCE(commandFileOpenProject);
				GUI_INSTANCE_REFERENCE(commandFileSave);
				GUI_INSTANCE_REFERENCE(commandFileSaveAll);
				GUI_INSTANCE_REFERENCE(commandHelpAbout);
				GUI_INSTANCE_REFERENCE(tableMain);
			}
			else
			{
				ViewModel_ = 0;
			}
		}
	public:
		MainWindow_()
			:vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>(L"ui::MainWindow")
			,vl::presentation::controls::GuiWindow(vl::presentation::theme::GetCurrentTheme()->CreateWindowStyle())
			,cellSplitter(0)
			,commandFileExit(0)
			,commandFileNewFile(0)
			,commandFileNewProject(0)
			,commandFileOpenFile(0)
			,commandFileOpenProject(0)
			,commandFileSave(0)
			,commandFileSaveAll(0)
			,commandHelpAbout(0)
			,tableMain(0)
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
		vl::presentation::controls::GuiBindableListView* listViewFileTemplate;
		vl::presentation::controls::GuiSinglelineTextBox* textBoxLocation;
		vl::presentation::controls::GuiSinglelineTextBox* textBoxProjectName;
		vl::presentation::controls::GuiBindableTreeView* treeViewProjectTemplate;

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
			:vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>(L"ui::NewFileWindow")
			,vl::presentation::controls::GuiWindow(vl::presentation::theme::GetCurrentTheme()->CreateWindowStyle())
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
		vl::presentation::controls::GuiBindableListView* listViewProjectTemplate;
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
			:vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>(L"ui::NewProjectWindow")
			,vl::presentation::controls::GuiWindow(vl::presentation::theme::GetCurrentTheme()->CreateWindowStyle())
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
			DECL_TYPE_INFO(vm::IFileModel)
			DECL_TYPE_INFO(vm::IFolderModel)
			DECL_TYPE_INFO(vm::IProjectFactoryModel)
			DECL_TYPE_INFO(vm::IProjectModel)
			DECL_TYPE_INFO(vm::ISolutionItemModel)
			DECL_TYPE_INFO(vm::ISolutionModel)
			DECL_TYPE_INFO(vm::IStudioModel)
			DECL_TYPE_INFO(ui::AboutWindow)
			DECL_TYPE_INFO(ui::MainWindow)
			DECL_TYPE_INFO(ui::NewFileWindow)
			DECL_TYPE_INFO(ui::NewProjectWindow)
		}
	}
}

/*
GsAboutWindow.h :
namespace ui
{
	class AboutWindow : public ui::AboutWindow_<ui::AboutWindow>
	{
		friend class ui::AboutWindow_<ui::AboutWindow>;
		friend struct vl::reflection::description::CustomTypeDescriptorSelector<ui::AboutWindow>;
	protected:

		// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)
		// #endregion CLASS_MEMBER_GUIEVENT_HANDLER
	public:
		AboutWindow(Ptr<vm::IStudioModel> ViewModel);
	};
}


GsAboutWindow.cpp :
namespace ui
{
	// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)

	// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

	AboutWindow::AboutWindow(Ptr<vm::IStudioModel> ViewModel)
	{
		InitializeComponents(ViewModel);
	}
}


GsMainWindow.h :
namespace ui
{
	class MainWindow : public ui::MainWindow_<ui::MainWindow>
	{
		friend class ui::MainWindow_<ui::MainWindow>;
		friend struct vl::reflection::description::CustomTypeDescriptorSelector<ui::MainWindow>;
	protected:

		// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)
		void cellSplitter_leftButtonDown(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiMouseEventArgs& arguments);
		void cellSplitter_leftButtonUp(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiMouseEventArgs& arguments);
		void cellSplitter_mouseMove(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiMouseEventArgs& arguments);
		void commandFileExit_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void commandFileNewFile_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void commandFileNewProject_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void commandFileOpenFile_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void commandFileOpenProject_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void commandFileSaveAll_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void commandFileSave_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void commandHelpAbout_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		// #endregion CLASS_MEMBER_GUIEVENT_HANDLER
	public:
		MainWindow(Ptr<vm::IStudioModel> ViewModel);
	};
}


GsMainWindow.cpp :
namespace ui
{
	// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)

	void MainWindow::cellSplitter_leftButtonDown(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiMouseEventArgs& arguments)
	{
	}

	void MainWindow::cellSplitter_leftButtonUp(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiMouseEventArgs& arguments)
	{
	}

	void MainWindow::cellSplitter_mouseMove(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiMouseEventArgs& arguments)
	{
	}

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

	void MainWindow::commandHelpAbout_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
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
	class NewFileWindow : public ui::NewFileWindow_<ui::NewFileWindow>
	{
		friend class ui::NewFileWindow_<ui::NewFileWindow>;
		friend struct vl::reflection::description::CustomTypeDescriptorSelector<ui::NewFileWindow>;
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
	class NewProjectWindow : public ui::NewProjectWindow_<ui::NewProjectWindow>
	{
		friend class ui::NewProjectWindow_<ui::NewProjectWindow>;
		friend struct vl::reflection::description::CustomTypeDescriptorSelector<ui::NewProjectWindow>;
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
