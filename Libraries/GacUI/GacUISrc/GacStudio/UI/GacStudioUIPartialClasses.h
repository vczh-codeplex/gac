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
	class ISaveItemAction;
	class IAddFileItemAction;
	class IOpenInEditorItemAction;
	class IRenameItemAction;
	class IRemoveItemAction;
	class IMacroEnvironment;
	class ITextTemplate;
	class IEditorFactoryModel;
	class ISolutionItemModel;
	class IFileModel;
	class IFolderModel;
	class IProjectModel;
	class ISolutionModel;
	class IFileFactoryModel;
	class IProjectFactoryModel;
	class IStudioNewFileModel;
	class StudioFileReference;
	class IStudioAddExistingFilesModel;
	class IStudioModel;
}
namespace ui
{
	class AboutWindow;
	class AddExistingFilesWindow;
	class MainWindow;
	class NewFileWindow;
	class NewProjectWindow;
	class RenameFileWindow;

}
namespace vm
{
	class ISaveItemAction : public virtual vl::reflection::IDescriptable, public vl::reflection::Description<ISaveItemAction>
	{
	public:

		virtual void Save() = 0;
	};

	class IAddFileItemAction : public virtual vl::reflection::IDescriptable, public vl::reflection::Description<IAddFileItemAction>
	{
	public:

		virtual vl::collections::LazyList<vl::Ptr<vm::ISaveItemAction>> AddFile(vl::Ptr<vm::IFileModel> file) = 0;
	};

	class IOpenInEditorItemAction : public virtual vl::reflection::IDescriptable, public vl::reflection::Description<IOpenInEditorItemAction>
	{
	public:
	};

	class IRenameItemAction : public virtual vl::reflection::IDescriptable, public vl::reflection::Description<IRenameItemAction>
	{
	public:

		virtual vl::WString GetRenameablePart() = 0;
		virtual vl::WString PreviewRename(vl::WString newName) = 0;
		virtual vl::collections::LazyList<vl::Ptr<vm::ISaveItemAction>> Rename(vl::WString newName) = 0;
	};

	class IRemoveItemAction : public virtual vl::reflection::IDescriptable, public vl::reflection::Description<IRemoveItemAction>
	{
	public:

		virtual vl::collections::LazyList<vl::Ptr<vm::ISaveItemAction>> Remove() = 0;
	};

	class IMacroEnvironment : public virtual vl::reflection::IDescriptable, public vl::reflection::Description<IMacroEnvironment>
	{
	public:

		virtual vm::IMacroEnvironment* GetParent() = 0;

		virtual bool HasMacro(vl::WString name, bool inherit) = 0;
		virtual vl::WString GetMacroValue(vl::WString name, bool inherit) = 0;
	};

	class ITextTemplate : public virtual vl::reflection::IDescriptable, public vl::reflection::Description<ITextTemplate>
	{
	public:

		virtual vl::WString Generate(vl::Ptr<vm::IMacroEnvironment> macroEnvironment) = 0;
	};

	class IEditorFactoryModel : public virtual vl::reflection::IDescriptable, public vl::reflection::Description<IEditorFactoryModel>
	{
	public:

		virtual vl::WString GetName() = 0;

		virtual vl::WString GetId() = 0;
	};

	class ISolutionItemModel : public virtual vl::reflection::IDescriptable, public vl::reflection::Description<ISolutionItemModel>
	{
	public:

		virtual vm::ISolutionItemModel* GetParent() = 0;

		virtual vl::Ptr<vl::presentation::GuiImageData> GetImage() = 0;
		vl::Event<void()> ImageChanged;

		virtual vl::WString GetName() = 0;
		vl::Event<void()> NameChanged;

		virtual vl::Ptr<vl::reflection::description::IValueObservableList> GetChildren() = 0;

		virtual vl::WString GetFilePath() = 0;
		vl::Event<void()> FilePathChanged;

		virtual vl::WString GetFileDirectory() = 0;
		vl::Event<void()> FileDirectoryChanged;

		virtual vl::vint32_t GetErrorCount() = 0;
		vl::Event<void()> ErrorCountChanged;

		virtual vl::WString GetErrorText(vl::vint32_t index) = 0;
	};

	class IFileModel : public virtual vm::ISolutionItemModel, public vl::reflection::Description<IFileModel>
	{
	public:

		virtual vl::Ptr<vm::IFileFactoryModel> GetFileFactory() = 0;

		virtual void OpenFile() = 0;
		virtual void InitializeFileAndSave() = 0;
	};

	class IFolderModel : public virtual vm::ISolutionItemModel, public vl::reflection::Description<IFolderModel>
	{
	public:
	};

	class IProjectModel : public virtual vm::ISolutionItemModel, public vl::reflection::Description<IProjectModel>
	{
	public:

		virtual vl::Ptr<vm::IProjectFactoryModel> GetProjectFactory() = 0;

		virtual void OpenProject() = 0;
		virtual void InitializeProjectAndSave() = 0;
	};

	class ISolutionModel : public virtual vm::ISolutionItemModel, public vl::reflection::Description<ISolutionModel>
	{
	public:

		virtual void OpenSolution() = 0;
		virtual void NewSolution() = 0;
		virtual void AddProject(vl::Ptr<vm::IProjectModel> project) = 0;
		virtual void RemoveProject(vl::Ptr<vm::IProjectModel> project) = 0;
	};

	class IFileFactoryModel : public virtual vl::reflection::IDescriptable, public vl::reflection::Description<IFileFactoryModel>
	{
	public:

		virtual vl::Ptr<vl::presentation::GuiImageData> GetImage() = 0;

		virtual vl::Ptr<vl::presentation::GuiImageData> GetSmallImage() = 0;

		virtual vl::WString GetName() = 0;

		virtual vl::WString GetCategory() = 0;

		virtual vl::WString GetDescription() = 0;

		virtual vl::WString GetId() = 0;

		virtual vl::WString GetDefaultFileExt() = 0;

		virtual vl::Ptr<vm::ITextTemplate> GetTextTemplate() = 0;
	};

	class IProjectFactoryModel : public virtual vl::reflection::IDescriptable, public vl::reflection::Description<IProjectFactoryModel>
	{
	public:

		virtual vl::Ptr<vl::presentation::GuiImageData> GetImage() = 0;

		virtual vl::Ptr<vl::presentation::GuiImageData> GetSmallImage() = 0;

		virtual vl::WString GetName() = 0;

		virtual vl::WString GetDescription() = 0;

		virtual vl::WString GetId() = 0;

		virtual vl::collections::LazyList<vl::Ptr<vm::IProjectFactoryModel>> GetChildren() = 0;
	};

	class IStudioNewFileModel : public virtual vl::reflection::IDescriptable, public vl::reflection::Description<IStudioNewFileModel>
	{
	public:

		virtual vl::Ptr<vm::IProjectFactoryModel> GetFileFilters() = 0;

		virtual vl::Ptr<vm::IProjectFactoryModel> GetSelectedFileFilter() = 0;
		virtual void SetSelectedFileFilter(vl::Ptr<vm::IProjectFactoryModel> value) = 0;

		virtual vl::Ptr<vl::reflection::description::IValueObservableList> GetFilteredFileFactories() = 0;
	};

	class StudioFileReference : public vl::Object, public vl::reflection::Description<StudioFileReference>
	{
	public:
		vl::WString fileName;
		vl::Ptr<vm::IFileFactoryModel> fileFactory;
	};

	class IStudioAddExistingFilesModel : public virtual vl::reflection::IDescriptable, public vl::reflection::Description<IStudioAddExistingFilesModel>
	{
	public:

		virtual vl::Ptr<vl::reflection::description::IValueObservableList> GetSelectedFiles() = 0;

		virtual vl::WString GetCurrentFileName() = 0;
		virtual void SetCurrentFileName(vl::WString value) = 0;

		virtual vl::Ptr<vl::reflection::description::IValueObservableList> GetFilteredFileFactories() = 0;

		virtual void AddFiles(vl::collections::LazyList<vl::WString> fileNames) = 0;
		virtual void RemoveFiles(vl::collections::LazyList<vl::vint32_t> indices) = 0;
	};

	class IStudioModel : public virtual vl::reflection::IDescriptable, public vl::reflection::Description<IStudioModel>
	{
	public:

		virtual vl::collections::LazyList<vl::Ptr<vm::IProjectFactoryModel>> GetProjectFactories() = 0;

		virtual vl::collections::LazyList<vl::Ptr<vm::IFileFactoryModel>> GetFileFactories() = 0;

		virtual vl::collections::LazyList<vl::Ptr<vm::IEditorFactoryModel>> GetEditorFactories() = 0;

		virtual vl::Ptr<vm::ISolutionItemModel> GetRootSolutionItem() = 0;

		virtual vl::Ptr<vm::ISolutionModel> GetOpenedSolution() = 0;
		vl::Event<void()> OpenedSolutionChanged;

		virtual vl::Ptr<vm::ISolutionItemModel> GetWorkingItem() = 0;
		vl::Event<void()> WorkingItemChanged;

		virtual vl::Ptr<vm::IProjectModel> GetWorkingProject() = 0;
		vl::Event<void()> WorkingProjectChanged;

		virtual vl::WString GetWorkingDirectory() = 0;
		vl::Event<void()> WorkingDirectoryChanged;

		virtual vl::Ptr<vm::IStudioNewFileModel> CreateNewFileModel() = 0;
		virtual vl::Ptr<vm::IStudioAddExistingFilesModel> CreateAddExistingFilesModel() = 0;
		virtual void NotifySelectedSolutionItem(vl::Ptr<vm::ISolutionItemModel> solutionItem) = 0;
		virtual vl::Ptr<vm::IProjectFactoryModel> GetProjectFactory(vl::WString id) = 0;
		virtual vl::Ptr<vm::IFileFactoryModel> GetFileFactory(vl::WString id) = 0;
		virtual vl::Ptr<vm::IEditorFactoryModel> GetEditorFactory(vl::WString id) = 0;
		virtual void OpenSolution(vl::WString filePath) = 0;
		virtual void NewSolution(vl::WString filePath) = 0;
		virtual void CloseSolution() = 0;
		virtual vl::Ptr<vm::IProjectModel> AddNewProject(bool createNewSolution, vl::Ptr<vm::IProjectFactoryModel> projectFactory, vl::WString projectName, vl::WString solutionDirectory, vl::WString solutionName) = 0;
		virtual vl::Ptr<vm::IFileModel> AddNewFile(vl::Ptr<vm::IAddFileItemAction> action, vl::Ptr<vm::IProjectModel> project, vl::Ptr<vm::IFileFactoryModel> fileFactory, vl::WString fileDirectory, vl::WString fileName) = 0;
		virtual void RenameFile(vl::Ptr<vm::IRenameItemAction> action, vl::Ptr<vm::ISolutionItemModel> solutionItem, vl::WString newName) = 0;
		virtual void RemoveFile(vl::Ptr<vm::IRemoveItemAction> action, vl::Ptr<vm::ISolutionItemModel> solutionItem) = 0;
		virtual void OpenBrowser(vl::WString url) = 0;
		virtual void PromptError(vl::WString message) = 0;
		virtual bool SafeExecute(vl::Func<void()> procedure) = 0;
		virtual void ExecuteSaveItems(vl::collections::LazyList<vl::Ptr<vm::ISaveItemAction>> saveItems) = 0;
	};

}
namespace ui
{
	template<typename TImpl>
	class AboutWindow_ : public vl::presentation::controls::GuiWindow, public vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>, public vl::reflection::Description<TImpl>
	{
		friend struct vl::reflection::description::CustomTypeDescriptorSelector<TImpl>;
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

	template<typename TImpl>
	class AddExistingFilesWindow_ : public vl::presentation::controls::GuiWindow, public vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>, public vl::reflection::Description<TImpl>
	{
		friend struct vl::reflection::description::CustomTypeDescriptorSelector<TImpl>;
	private:
		Ptr<vm::IStudioModel> ViewModel_;
		Ptr<vm::IStudioAddExistingFilesModel> OperationModel_;
		Ptr<vm::IAddFileItemAction> Action_;
	protected:
		vl::presentation::controls::GuiButton* buttonAdd;
		vl::presentation::controls::GuiButton* buttonCancel;
		vl::presentation::controls::GuiButton* buttonOK;
		vl::presentation::controls::GuiButton* buttonRemove;
		vl::presentation::controls::GuiBindableDataGrid* dataGridFiles;
		vl::presentation::controls::GuiOpenFileDialog* dialogOpen;

		void InitializeComponents(Ptr<vm::IStudioModel> ViewModel, Ptr<vm::IStudioAddExistingFilesModel> OperationModel, Ptr<vm::IAddFileItemAction> Action)
		{
			ViewModel_ = ViewModel;
			OperationModel_ = OperationModel;
			Action_ = Action;
			if (InitializeFromResource())
			{
				GUI_INSTANCE_REFERENCE(buttonAdd);
				GUI_INSTANCE_REFERENCE(buttonCancel);
				GUI_INSTANCE_REFERENCE(buttonOK);
				GUI_INSTANCE_REFERENCE(buttonRemove);
				GUI_INSTANCE_REFERENCE(dataGridFiles);
				GUI_INSTANCE_REFERENCE(dialogOpen);
			}
			else
			{
				ViewModel_ = 0;
				OperationModel_ = 0;
				Action_ = 0;
			}
		}
	public:
		AddExistingFilesWindow_()
			:vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>(L"ui::AddExistingFilesWindow")
			,vl::presentation::controls::GuiWindow(vl::presentation::theme::GetCurrentTheme()->CreateWindowStyle())
			,buttonAdd(0)
			,buttonCancel(0)
			,buttonOK(0)
			,buttonRemove(0)
			,dataGridFiles(0)
			,dialogOpen(0)
		{
		}

		Ptr<vm::IStudioModel> GetViewModel()
		{
			return ViewModel_;
		}

		Ptr<vm::IStudioAddExistingFilesModel> GetOperationModel()
		{
			return OperationModel_;
		}

		Ptr<vm::IAddFileItemAction> GetAction()
		{
			return Action_;
		}
	};

	template<typename TImpl>
	class MainWindow_ : public vl::presentation::controls::GuiWindow, public vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>, public vl::reflection::Description<TImpl>
	{
		friend struct vl::reflection::description::CustomTypeDescriptorSelector<TImpl>;
	private:
		Ptr<vm::IStudioModel> ViewModel_;
	protected:
		vl::presentation::compositions::GuiCellComposition* cellSplitter;
		vl::presentation::controls::GuiToolstripCommand* commandFileAddExistingFiles;
		vl::presentation::controls::GuiToolstripCommand* commandFileAddNewFile;
		vl::presentation::controls::GuiToolstripCommand* commandFileCloseSolution;
		vl::presentation::controls::GuiToolstripCommand* commandFileExit;
		vl::presentation::controls::GuiToolstripCommand* commandFileNewProject;
		vl::presentation::controls::GuiToolstripCommand* commandFileOpen;
		vl::presentation::controls::GuiToolstripCommand* commandFileOpenProject;
		vl::presentation::controls::GuiToolstripCommand* commandFileOpenWith;
		vl::presentation::controls::GuiToolstripCommand* commandFileRemove;
		vl::presentation::controls::GuiToolstripCommand* commandFileRename;
		vl::presentation::controls::GuiToolstripCommand* commandFileSave;
		vl::presentation::controls::GuiToolstripCommand* commandFileSaveAll;
		vl::presentation::controls::GuiToolstripCommand* commandHelpAbout;
		vl::presentation::compositions::GuiTableComposition* tableMain;
		vl::presentation::controls::GuiToolstripMenu* toolstripMenuSolutionItem;
		vl::presentation::controls::GuiBindableTreeView* treeViewSolutionItem;

		void InitializeComponents(Ptr<vm::IStudioModel> ViewModel)
		{
			ViewModel_ = ViewModel;
			if (InitializeFromResource())
			{
				GUI_INSTANCE_REFERENCE(cellSplitter);
				GUI_INSTANCE_REFERENCE(commandFileAddExistingFiles);
				GUI_INSTANCE_REFERENCE(commandFileAddNewFile);
				GUI_INSTANCE_REFERENCE(commandFileCloseSolution);
				GUI_INSTANCE_REFERENCE(commandFileExit);
				GUI_INSTANCE_REFERENCE(commandFileNewProject);
				GUI_INSTANCE_REFERENCE(commandFileOpen);
				GUI_INSTANCE_REFERENCE(commandFileOpenProject);
				GUI_INSTANCE_REFERENCE(commandFileOpenWith);
				GUI_INSTANCE_REFERENCE(commandFileRemove);
				GUI_INSTANCE_REFERENCE(commandFileRename);
				GUI_INSTANCE_REFERENCE(commandFileSave);
				GUI_INSTANCE_REFERENCE(commandFileSaveAll);
				GUI_INSTANCE_REFERENCE(commandHelpAbout);
				GUI_INSTANCE_REFERENCE(tableMain);
				GUI_INSTANCE_REFERENCE(toolstripMenuSolutionItem);
				GUI_INSTANCE_REFERENCE(treeViewSolutionItem);
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
			,commandFileAddExistingFiles(0)
			,commandFileAddNewFile(0)
			,commandFileCloseSolution(0)
			,commandFileExit(0)
			,commandFileNewProject(0)
			,commandFileOpen(0)
			,commandFileOpenProject(0)
			,commandFileOpenWith(0)
			,commandFileRemove(0)
			,commandFileRename(0)
			,commandFileSave(0)
			,commandFileSaveAll(0)
			,commandHelpAbout(0)
			,tableMain(0)
			,toolstripMenuSolutionItem(0)
			,treeViewSolutionItem(0)
		{
		}

		Ptr<vm::IStudioModel> GetViewModel()
		{
			return ViewModel_;
		}
	};

	template<typename TImpl>
	class NewFileWindow_ : public vl::presentation::controls::GuiWindow, public vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>, public vl::reflection::Description<TImpl>
	{
		friend struct vl::reflection::description::CustomTypeDescriptorSelector<TImpl>;
	private:
		Ptr<vm::IStudioModel> ViewModel_;
		Ptr<vm::IStudioNewFileModel> OperationModel_;
		Ptr<vm::IAddFileItemAction> Action_;
	protected:
		vl::presentation::controls::GuiButton* buttonCancel;
		vl::presentation::controls::GuiButton* buttonCreate;
		vl::presentation::controls::GuiBindableListView* listViewFileTemplate;
		vl::presentation::controls::GuiSinglelineTextBox* textBoxFileName;
		vl::presentation::controls::GuiSinglelineTextBox* textBoxLocation;
		vl::presentation::controls::GuiBindableTreeView* treeViewProjectTemplate;

		void InitializeComponents(Ptr<vm::IStudioModel> ViewModel, Ptr<vm::IStudioNewFileModel> OperationModel, Ptr<vm::IAddFileItemAction> Action)
		{
			ViewModel_ = ViewModel;
			OperationModel_ = OperationModel;
			Action_ = Action;
			if (InitializeFromResource())
			{
				GUI_INSTANCE_REFERENCE(buttonCancel);
				GUI_INSTANCE_REFERENCE(buttonCreate);
				GUI_INSTANCE_REFERENCE(listViewFileTemplate);
				GUI_INSTANCE_REFERENCE(textBoxFileName);
				GUI_INSTANCE_REFERENCE(textBoxLocation);
				GUI_INSTANCE_REFERENCE(treeViewProjectTemplate);
			}
			else
			{
				ViewModel_ = 0;
				OperationModel_ = 0;
				Action_ = 0;
			}
		}
	public:
		NewFileWindow_()
			:vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>(L"ui::NewFileWindow")
			,vl::presentation::controls::GuiWindow(vl::presentation::theme::GetCurrentTheme()->CreateWindowStyle())
			,buttonCancel(0)
			,buttonCreate(0)
			,listViewFileTemplate(0)
			,textBoxFileName(0)
			,textBoxLocation(0)
			,treeViewProjectTemplate(0)
		{
		}

		Ptr<vm::IStudioModel> GetViewModel()
		{
			return ViewModel_;
		}

		Ptr<vm::IStudioNewFileModel> GetOperationModel()
		{
			return OperationModel_;
		}

		Ptr<vm::IAddFileItemAction> GetAction()
		{
			return Action_;
		}
	};

	template<typename TImpl>
	class NewProjectWindow_ : public vl::presentation::controls::GuiWindow, public vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>, public vl::reflection::Description<TImpl>
	{
		friend struct vl::reflection::description::CustomTypeDescriptorSelector<TImpl>;
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

	template<typename TImpl>
	class RenameFileWindow_ : public vl::presentation::controls::GuiWindow, public vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>, public vl::reflection::Description<TImpl>
	{
		friend struct vl::reflection::description::CustomTypeDescriptorSelector<TImpl>;
	private:
		Ptr<vm::IStudioModel> ViewModel_;
		Ptr<vm::ISolutionItemModel> SolutionItem_;
		Ptr<vm::IRenameItemAction> Action_;
	protected:
		vl::presentation::controls::GuiButton* buttonCancel;
		vl::presentation::controls::GuiButton* buttonRename;
		vl::presentation::controls::GuiWindow* self;
		vl::presentation::controls::GuiSinglelineTextBox* textBoxName;
		vl::presentation::controls::GuiSinglelineTextBox* textBoxNew;
		vl::presentation::controls::GuiSinglelineTextBox* textBoxOriginal;

		void InitializeComponents(Ptr<vm::IStudioModel> ViewModel, Ptr<vm::ISolutionItemModel> SolutionItem, Ptr<vm::IRenameItemAction> Action)
		{
			ViewModel_ = ViewModel;
			SolutionItem_ = SolutionItem;
			Action_ = Action;
			if (InitializeFromResource())
			{
				GUI_INSTANCE_REFERENCE(buttonCancel);
				GUI_INSTANCE_REFERENCE(buttonRename);
				GUI_INSTANCE_REFERENCE(self);
				GUI_INSTANCE_REFERENCE(textBoxName);
				GUI_INSTANCE_REFERENCE(textBoxNew);
				GUI_INSTANCE_REFERENCE(textBoxOriginal);
			}
			else
			{
				ViewModel_ = 0;
				SolutionItem_ = 0;
				Action_ = 0;
			}
		}
	public:
		RenameFileWindow_()
			:vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>(L"ui::RenameFileWindow")
			,vl::presentation::controls::GuiWindow(vl::presentation::theme::GetCurrentTheme()->CreateWindowStyle())
			,buttonCancel(0)
			,buttonRename(0)
			,self(0)
			,textBoxName(0)
			,textBoxNew(0)
			,textBoxOriginal(0)
		{
		}

		Ptr<vm::IStudioModel> GetViewModel()
		{
			return ViewModel_;
		}

		Ptr<vm::ISolutionItemModel> GetSolutionItem()
		{
			return SolutionItem_;
		}

		Ptr<vm::IRenameItemAction> GetAction()
		{
			return Action_;
		}
	};

}
namespace vl
{
	namespace reflection
	{
		namespace description
		{
			DECL_TYPE_INFO(vm::IAddFileItemAction)
			DECL_TYPE_INFO(vm::IEditorFactoryModel)
			DECL_TYPE_INFO(vm::IFileFactoryModel)
			DECL_TYPE_INFO(vm::IFileModel)
			DECL_TYPE_INFO(vm::IFolderModel)
			DECL_TYPE_INFO(vm::IMacroEnvironment)
			DECL_TYPE_INFO(vm::IOpenInEditorItemAction)
			DECL_TYPE_INFO(vm::IProjectFactoryModel)
			DECL_TYPE_INFO(vm::IProjectModel)
			DECL_TYPE_INFO(vm::IRemoveItemAction)
			DECL_TYPE_INFO(vm::IRenameItemAction)
			DECL_TYPE_INFO(vm::ISaveItemAction)
			DECL_TYPE_INFO(vm::ISolutionItemModel)
			DECL_TYPE_INFO(vm::ISolutionModel)
			DECL_TYPE_INFO(vm::IStudioAddExistingFilesModel)
			DECL_TYPE_INFO(vm::IStudioModel)
			DECL_TYPE_INFO(vm::IStudioNewFileModel)
			DECL_TYPE_INFO(vm::ITextTemplate)
			DECL_TYPE_INFO(vm::StudioFileReference)
			DECL_TYPE_INFO(ui::AboutWindow)
			DECL_TYPE_INFO(ui::AddExistingFilesWindow)
			DECL_TYPE_INFO(ui::MainWindow)
			DECL_TYPE_INFO(ui::NewFileWindow)
			DECL_TYPE_INFO(ui::NewProjectWindow)
			DECL_TYPE_INFO(ui::RenameFileWindow)

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


GsAddExistingFilesWindow.h :
namespace ui
{
	class AddExistingFilesWindow : public ui::AddExistingFilesWindow_<ui::AddExistingFilesWindow>
	{
		friend class ui::AddExistingFilesWindow_<ui::AddExistingFilesWindow>;
		friend struct vl::reflection::description::CustomTypeDescriptorSelector<ui::AddExistingFilesWindow>;
	protected:

		// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)
		void buttonAdd_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void buttonCancel_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void buttonOK_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void buttonRemove_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		// #endregion CLASS_MEMBER_GUIEVENT_HANDLER
	public:
		AddExistingFilesWindow(Ptr<vm::IStudioModel> ViewModel, Ptr<vm::IStudioAddExistingFilesModel> OperationModel, Ptr<vm::IAddFileItemAction> Action);
	};
}


GsAddExistingFilesWindow.cpp :
namespace ui
{
	// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)

	void AddExistingFilesWindow::buttonAdd_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	void AddExistingFilesWindow::buttonCancel_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	void AddExistingFilesWindow::buttonOK_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	void AddExistingFilesWindow::buttonRemove_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

	AddExistingFilesWindow::AddExistingFilesWindow(Ptr<vm::IStudioModel> ViewModel, Ptr<vm::IStudioAddExistingFilesModel> OperationModel, Ptr<vm::IAddFileItemAction> Action)
	{
		InitializeComponents(ViewModel, OperationModel, Action);
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
		void commandFileAddExistingFiles_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void commandFileAddNewFile_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void commandFileCloseSolution_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void commandFileExit_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void commandFileNewProject_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void commandFileOpenProject_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void commandFileOpenWith_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void commandFileOpen_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void commandFileRemove_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void commandFileRename_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void commandFileSaveAll_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void commandFileSave_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void commandHelpAbout_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void treeViewSolutionItem_NodeRightButtonUp(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiNodeMouseEventArgs& arguments);
		void treeViewSolutionItem_SelectionChanged(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
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

	void MainWindow::commandFileAddExistingFiles_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	void MainWindow::commandFileAddNewFile_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	void MainWindow::commandFileCloseSolution_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	void MainWindow::commandFileExit_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	void MainWindow::commandFileNewProject_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	void MainWindow::commandFileOpenProject_Executed(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
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
	}

	void MainWindow::treeViewSolutionItem_NodeRightButtonUp(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiNodeMouseEventArgs& arguments)
	{
	}

	void MainWindow::treeViewSolutionItem_SelectionChanged(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
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
		NewFileWindow(Ptr<vm::IStudioModel> ViewModel, Ptr<vm::IStudioNewFileModel> OperationModel, Ptr<vm::IAddFileItemAction> Action);
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

	NewFileWindow::NewFileWindow(Ptr<vm::IStudioModel> ViewModel, Ptr<vm::IStudioNewFileModel> OperationModel, Ptr<vm::IAddFileItemAction> Action)
	{
		InitializeComponents(ViewModel, OperationModel, Action);
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


GsRenameFileWindow.h :
namespace ui
{
	class RenameFileWindow : public ui::RenameFileWindow_<ui::RenameFileWindow>
	{
		friend class ui::RenameFileWindow_<ui::RenameFileWindow>;
		friend struct vl::reflection::description::CustomTypeDescriptorSelector<ui::RenameFileWindow>;
	protected:

		// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)
		void buttonCancel_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void buttonRename_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		// #endregion CLASS_MEMBER_GUIEVENT_HANDLER
	public:
		RenameFileWindow(Ptr<vm::IStudioModel> ViewModel, Ptr<vm::ISolutionItemModel> SolutionItem, Ptr<vm::IRenameItemAction> Action);
	};
}


GsRenameFileWindow.cpp :
namespace ui
{
	// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)

	void RenameFileWindow::buttonCancel_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	void RenameFileWindow::buttonRename_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

	RenameFileWindow::RenameFileWindow(Ptr<vm::IStudioModel> ViewModel, Ptr<vm::ISolutionItemModel> SolutionItem, Ptr<vm::IRenameItemAction> Action)
	{
		InitializeComponents(ViewModel, SolutionItem, Action);
	}
}


*/

#endif
