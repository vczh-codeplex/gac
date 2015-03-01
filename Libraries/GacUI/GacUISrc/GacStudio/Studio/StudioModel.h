/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GacStudio::StudioModel

Interfaces:
***********************************************************************/

#ifndef GACSTUDIO_STUDIO_STUDIOMODEL
#define GACSTUDIO_STUDIO_STUDIOMODEL

#include "..\UI\GacStudioUI.h"

using namespace vl::collections;

namespace vm
{
	class TextTemplateItem : public Object
	{
	public:
		bool											isMacro;
		WString											content;
	};

	class TextTemplate : public Object, public virtual ITextTemplate
	{
	protected:
		collections::List<Ptr<TextTemplateItem>>		items;

	public:
		TextTemplate(const WString& content);
		~TextTemplate();

		WString											Generate(Ptr<IMacroEnvironment> macroEnvironment)override;
	};

	class FileFactoryModel : public Object, public virtual IFileFactoryModel
	{
	protected:
		Ptr<GuiImageData>								image;
		Ptr<GuiImageData>								smallImage;
		WString											name;
		WString											category;
		WString											description;
		WString											id;
		WString											ext;
		Ptr<ITextTemplate>								textTemplate;

	public:
		FileFactoryModel(WString _imageUrl, WString _smallImageUrl, WString _name, WString _category, WString _description, WString _id, WString _ext, Ptr<ITextTemplate> _textTemplate);
		FileFactoryModel(Ptr<GuiImageData> _image, Ptr<GuiImageData> _smallImage, WString _id);
		~FileFactoryModel();

		Ptr<GuiImageData>								GetImage()override;
		Ptr<GuiImageData>								GetSmallImage()override;
		WString											GetName()override;
		WString											GetCategory()override;
		WString											GetDescription()override;
		WString											GetId()override;
		WString											GetDefaultFileExt()override;
		Ptr<ITextTemplate>								GetTextTemplate()override;
	};

	class ProjectFactoryModel : public Object, public virtual IProjectFactoryModel
	{
	protected:
		Ptr<GuiImageData>								image;
		Ptr<GuiImageData>								smallImage;
		WString											name;
		WString											description;
		WString											id;

	public:
		ProjectFactoryModel(WString _imageUrl, WString _smallImageUrl, WString _name, WString _description, WString _id);
		ProjectFactoryModel(Ptr<GuiImageData> _image, Ptr<GuiImageData> _smallImage, WString _id);
		~ProjectFactoryModel();

		Ptr<GuiImageData>								GetImage()override;
		Ptr<GuiImageData>								GetSmallImage()override;
		WString											GetName()override;
		WString											GetDescription()override;
		WString											GetId()override;
		LazyList<Ptr<IProjectFactoryModel>>				GetChildren()override;
	};

	class FileFactoryFilterModel : public ProjectFactoryModel
	{
	protected:
		List<Ptr<ProjectFactoryModel>>					children;

	public:
		FileFactoryFilterModel();
		~FileFactoryFilterModel();

		void											AddChild(Ptr<ProjectFactoryModel> child);
		LazyList<Ptr<IProjectFactoryModel>>				GetChildren()override;
	};

	class RootSolutionItemModel : public Object, public virtual ISolutionItemModel
	{
	protected:
		list::ObservableList<Ptr<ISolutionItemModel>>	children;

	public:
		ISolutionItemModel*								GetParent()override;
		Ptr<GuiImageData>								GetImage()override;
		WString											GetName()override;
		Ptr<description::IValueObservableList>			GetChildren()override;

		WString											GetFilePath()override;
		WString											GetFileDirectory()override;
		vint											GetErrorCount()override;
		WString											GetErrorText(vint index)override;
		
		Ptr<ISolutionModel>								GetSolution();
		void											SetSolution(Ptr<ISolutionModel> solution);
	};

	class StudioNewFileModel : public Object, public virtual IStudioNewFileModel
	{
	protected:
		IStudioModel*									studioModel;
		Ptr<FileFactoryFilterModel>						fileFilters;
		Ptr<IProjectFactoryModel>						selectedFileFilter;
		list::ObservableList<Ptr<IFileFactoryModel>>	filteredFileFactories;

	public:
		StudioNewFileModel(IStudioModel* _studioModel, Ptr<ProjectFactoryModel> solutionProjectFactory);
		~StudioNewFileModel();
		
		Ptr<IProjectFactoryModel>						GetFileFilters()override;
		Ptr<IProjectFactoryModel>						GetSelectedFileFilter()override;
		void											SetSelectedFileFilter(Ptr<IProjectFactoryModel> value)override;
		Ptr<description::IValueObservableList>			GetFilteredFileFactories()override;
	};

	class StudioAddExistingFilesModel : public Object, public virtual IStudioAddExistingFilesModel
	{
	protected:
		IStudioModel*									studioModel;
		Ptr<IAddFileItemAction>							action;
		WString											currentFileName;
		list::ObservableList<Ptr<StudioFileReference>>	selectedFiles;
		list::ObservableList<Ptr<IFileFactoryModel>>	filteredFileFactories;

		void											GetAcceptableFileFactories(const WString& fileName, list::ObservableList<Ptr<IFileFactoryModel>>& fileFactories);
	public:
		StudioAddExistingFilesModel(IStudioModel* _studioModel, Ptr<IAddFileItemAction> _action);
		~StudioAddExistingFilesModel();

		Ptr<description::IValueObservableList>			GetSelectedFiles()override;
		WString											GetCurrentFileName()override;
		void											SetCurrentFileName(WString value)override;
		Ptr<description::IValueObservableList>			GetFilteredFileFactories()override;

		void											AddFiles(LazyList<WString> fileNames)override;
		void											RemoveFiles(LazyList<vint32_t> indices)override;
	};

	class StudioModel : public Object, public virtual IStudioModel
	{
	protected:
		Ptr<FileFactoryFilterModel>						solutionProjectFactory;
		List<Ptr<IFileFactoryModel>>					fileFactories;
		List<Ptr<IEditorFactoryModel>>					editorFactories;
		Ptr<RootSolutionItemModel>						rootSolutionItem;
		Ptr<ISolutionItemModel>							selectedSolutionItem;

	public:
		StudioModel();
		~StudioModel();

		LazyList<Ptr<IProjectFactoryModel>>				GetProjectFactories()override;
		LazyList<Ptr<IFileFactoryModel>>				GetFileFactories()override;
		LazyList<Ptr<IEditorFactoryModel>>				GetEditorFactories()override;

		Ptr<IStudioNewFileModel>						CreateNewFileModel()override;
		Ptr<IStudioAddExistingFilesModel>				CreateAddExistingFilesModel(Ptr<IAddFileItemAction> action)override;

		Ptr<ISolutionItemModel>							GetRootSolutionItem()override;
		Ptr<ISolutionModel>								GetOpenedSolution()override;
		void											NotifySelectedSolutionItem(Ptr<ISolutionItemModel> selectedItem)override;
		Ptr<ISolutionItemModel>							GetWorkingItem()override;
		Ptr<IProjectModel>								GetWorkingProject()override;
		WString											GetWorkingDirectory()override;

		Ptr<IProjectFactoryModel>						GetProjectFactory(WString id)override;
		Ptr<IFileFactoryModel>							GetFileFactory(WString id)override;
		Ptr<IEditorFactoryModel>						GetEditorFactory(WString id)override;

		void											OpenSolution(WString filePath)override;
		void											NewSolution(WString filePath)override;
		void											CloseSolution()override;
		vl::Ptr<vm::IProjectModel>						AddNewProject(bool createNewSolution, vl::Ptr<vm::IProjectFactoryModel> projectFactory, vl::WString projectName, vl::WString solutionDirectory, vl::WString solutionName)override;
		vl::Ptr<vm::IFileModel>							AddNewFile(vl::Ptr<vm::IAddFileItemAction> action, vl::Ptr<vm::IProjectModel> project, vl::Ptr<vm::IFileFactoryModel> fileFactory, vl::WString fileDirectory, vl::WString fileName)override;
		void											RenameFile(vl::Ptr<vm::IRenameItemAction> action, vl::Ptr<vm::ISolutionItemModel> solutionItem, vl::WString newName)override;
		void											RemoveFile(vl::Ptr<vm::IRemoveItemAction> action, vl::Ptr<vm::ISolutionItemModel> solutionItem)override;

		void											OpenBrowser(WString url)override;
		void											PromptError(WString message)override;
		bool											SafeExecute(vl::Func<void()> procedure)override;
		void											ExecuteSaveItems(vl::collections::LazyList<vl::Ptr<vm::ISaveItemAction>> saveItems)override;
	};
}

#endif