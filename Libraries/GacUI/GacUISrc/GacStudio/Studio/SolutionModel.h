/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacStudio::SolutionModel

Interfaces:
***********************************************************************/

#ifndef GACSTUDIO_STUDIO_SOLUTIONMODEL
#define GACSTUDIO_STUDIO_SOLUTIONMODEL

#include "..\UI\GacStudioUI.h"

using namespace vl::collections;

namespace vm
{
	class FileMacroEnvironment : public Object, public virtual IMacroEnvironment
	{
	protected:
		IFileModel*										fileModel;
	public:
		FileMacroEnvironment(IFileModel* _fileModel);
		~FileMacroEnvironment();

		IMacroEnvironment*								GetParent()override;
		bool											HasMacro(WString name, bool inherit)override;
		WString											GetMacroValue(WString name, bool inherit)override;
	};

	class FolderItemBase;

	class FileItem : public Object, public virtual IFileModel, public Description<FileItem>
		, public virtual IRenameItemAction
		, public virtual IRemoveItemAction
	{
		friend class FolderItemBase;
	protected:
		IStudioModel*									studioModel;
		list::ObservableList<Ptr<ISolutionItemModel>>	children;
		Ptr<IFileFactoryModel>							fileFactory;
		WString											filePath;
		collections::List<WString>						errors;
		bool											unsupported;
		ISolutionItemModel*								parent;

	public:
		FileItem(IStudioModel* _studioModel, Ptr<IFileFactoryModel> _fileFactory, WString _filePath, bool _unsupported = false);
		~FileItem();

		// --------------------------- action
		WString											GetRenameablePart()override;
		WString											PreviewRename(WString newName)override;
		bool											Rename(WString newName)override;
		bool											Remove()override;

		// --------------------------- feature
		Ptr<IFileFactoryModel>							GetFileFactory()override;
		bool											OpenFile()override;
		bool											SaveFile()override;
		bool											NewFileAndSave()override;

		// --------------------------- solution item
		ISolutionItemModel*								GetParent()override;
		Ptr<GuiImageData>								GetImage()override;
		WString											GetName()override;
		Ptr<description::IValueObservableList>			GetChildren()override;
		WString											GetFilePath()override;
		WString											GetFileDirectory()override;
		vint											GetErrorCount()override;
		WString											GetErrorText(vint index)override;
	};

	class FolderItemBase : public Object
	{
	protected:
		list::ObservableList<Ptr<ISolutionItemModel>>	children;
		collections::SortedList<WString>				folderNames;
		collections::SortedList<WString>				fileNames;

		void											ClearInternal();
		bool											AddFileItemInternal(const wchar_t* filePath, Ptr<IFileModel> fileItem);
		bool											RemoveFileItemInternal(const wchar_t* filePath, Ptr<IFileModel> fileItem);
	public:
		FolderItemBase();
		~FolderItemBase();
	};

	class FolderItem : public FolderItemBase, public virtual IFolderModel, public Description<FolderItem>
		, public virtual IAddFileItemAction
		, public virtual IRenameItemAction
		, public virtual IRemoveItemAction
	{
	protected:
		ISolutionItemModel*								parent;
		Ptr<GuiImageData>								image;
		WString											filePath;

	public:
		FolderItem(ISolutionItemModel* _parent, WString _filePath);
		~FolderItem();
		
		// --------------------------- action
		bool											AddFile(Ptr<IFileModel> file)override;
		WString											GetRenameablePart()override;
		WString											PreviewRename(WString newName)override;
		bool											Rename(WString newName)override;
		bool											Remove()override;
		
		// --------------------------- solution item
		ISolutionItemModel*								GetParent()override;
		Ptr<GuiImageData>								GetImage()override;
		WString											GetName()override;
		Ptr<description::IValueObservableList>			GetChildren()override;
		WString											GetFilePath()override;
		WString											GetFileDirectory()override;
		vint											GetErrorCount()override;
		WString											GetErrorText(vint index)override;
	};

	class ProjectItem : public FolderItemBase, public virtual IProjectModel, public Description<ProjectItem>
		, public virtual IAddFileItemAction
		, public virtual IRenameItemAction
		, public virtual IRemoveItemAction
	{
		friend class FileItem;
		friend class FolderItem;
	protected:
		IStudioModel*									studioModel;
		Ptr<IProjectFactoryModel>						projectFactory;
		WString											filePath;
		List<Ptr<IFileModel>>							fileItems;
		collections::List<WString>						errors;
		bool											unsupported;

		WString											GetNormalizedRelativePath(Ptr<IFileModel> fileItem);
		bool											AddFileItem(Ptr<IFileModel> fileItem);
		bool											RemoveFileItem(Ptr<IFileModel> fileItem);
	public:
		ProjectItem(IStudioModel* _studioModel, Ptr<IProjectFactoryModel> _projectFactory, WString _filePath, bool _unsupported = false);
		~ProjectItem();
		
		// --------------------------- action
		bool											AddFile(Ptr<IFileModel> file)override;
		WString											GetRenameablePart()override;
		WString											PreviewRename(WString newName)override;
		bool											Rename(WString newName)override;
		bool											Remove()override;
		
		// --------------------------- feature
		Ptr<IProjectFactoryModel>						GetProjectFactory()override;
		bool											OpenProject()override;
		bool											SaveProject(bool saveContainingFiles)override;
		bool											NewProjectAndSave()override;
		
		// --------------------------- solution item
		ISolutionItemModel*								GetParent()override;
		Ptr<GuiImageData>								GetImage()override;
		WString											GetName()override;
		Ptr<description::IValueObservableList>			GetChildren()override;
		WString											GetFilePath()override;
		WString											GetFileDirectory()override;
		vint											GetErrorCount()override;
		WString											GetErrorText(vint index)override;
	};

	class SolutionItem : public Object, public virtual ISolutionModel, public Description<SolutionItem>
	{
	protected:
		IStudioModel*									studioModel;
		list::ObservableList<Ptr<IProjectModel>>		projects;
		Ptr<IProjectFactoryModel>						projectFactory;
		WString											filePath;
		collections::List<WString>						errors;

	public:
		SolutionItem(IStudioModel* _studioModel, Ptr<IProjectFactoryModel> _projectFactory, WString _filePath);
		~SolutionItem();
		
		// --------------------------- action
		bool											OpenSolution()override;
		bool											SaveSolution(bool saveContainingProjects)override;
		bool											NewSolution()override;
		bool											AddProject(Ptr<IProjectModel> project)override;
		bool											RemoveProject(Ptr<IProjectModel> project)override;
		
		// --------------------------- solution item
		ISolutionItemModel*								GetParent()override;
		Ptr<GuiImageData>								GetImage()override;
		WString											GetName()override;
		Ptr<description::IValueObservableList>			GetChildren()override;
		WString											GetFilePath()override;
		WString											GetFileDirectory()override;
		vint											GetErrorCount()override;
		WString											GetErrorText(vint index)override;
	};
}

namespace vl
{
	namespace reflection
	{
		namespace description
		{
			DECL_TYPE_INFO(vm::FileItem)
			DECL_TYPE_INFO(vm::FolderItem)
			DECL_TYPE_INFO(vm::ProjectItem)
			DECL_TYPE_INFO(vm::SolutionItem)
		}
	}
}

#endif