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

	class FileItem : public Object, public virtual IFileModel
	{
	protected:
		IStudioModel*									studioModel;
		list::ObservableList<Ptr<ISolutionItemModel>>	children;
		Ptr<IFileFactoryModel>							fileFactory;
		WString											filePath;
		bool											isSaved;
		collections::List<WString>						errors;
		bool											unsupported;
		ISolutionItemModel*								parent;

	public:
		FileItem(IStudioModel* _studioModel, Ptr<IFileFactoryModel> _fileFactory, WString _filePath, bool _unsupported = false);
		~FileItem();
		
		Ptr<IFileFactoryModel>							GetFileFactory()override;
		bool											OpenFile()override;
		bool											SaveFile()override;
		bool											NewFileAndSave()override;
		bool											RenameFile(WString newName)override;
		bool											RemoveFile()override;
		
		ISolutionItemModel*								GetParent()override;
		Ptr<GuiImageData>								GetImage()override;
		WString											GetName()override;
		Ptr<description::IValueObservableList>			GetChildren()override;

		bool											GetIsFileItem()override;
		WString											GetFilePath()override;
		WString											GetFileDirectory()override;
		bool											GetIsSaved()override;
		vint											GetErrorCount()override;
		WString											GetErrorText(vint index)override;
		bool											OpenFileItem()override;
		bool											SaveFileItem()override;
	};

	class FolderItemBase : public Object
	{
	protected:
		list::ObservableList<Ptr<ISolutionItemModel>>	children;
		collections::SortedList<WString>				folderNames;
		collections::SortedList<WString>				fileNames;

		void											ClearInternal();
		void											AddFileItemInternal(const wchar_t* filePath, Ptr<IFileModel> fileItem);
	public:
		FolderItemBase();
		~FolderItemBase();
	};

	class FolderItem : public FolderItemBase, public virtual IFolderModel
	{
	protected:
		ISolutionItemModel*								parent;
		Ptr<GuiImageData>								image;
		WString											filePath;

	public:
		FolderItem(ISolutionItemModel* _parent, WString _filePath);
		~FolderItem();
		
		ISolutionItemModel*								GetParent()override;
		Ptr<GuiImageData>								GetImage()override;
		WString											GetName()override;
		Ptr<description::IValueObservableList>			GetChildren()override;

		bool											GetIsFileItem()override;
		WString											GetFilePath()override;
		WString											GetFileDirectory()override;
		bool											GetIsSaved()override;
		vint											GetErrorCount()override;
		WString											GetErrorText(vint index)override;
		bool											OpenFileItem()override;
		bool											SaveFileItem()override;
	};

	class ProjectItem : public FolderItemBase, public virtual IProjectModel
	{
	protected:
		IStudioModel*									studioModel;
		Ptr<IProjectFactoryModel>						projectFactory;
		WString											filePath;
		List<Ptr<IFileModel>>							fileItems;
		bool											isSaved;
		collections::List<WString>						errors;
		bool											unsupported;

		void											AddFileItem(Ptr<IFileModel> fileItem);
	public:
		ProjectItem(IStudioModel* _studioModel, Ptr<IProjectFactoryModel> _projectFactory, WString _filePath, bool _unsupported = false);
		~ProjectItem();
		
		Ptr<IProjectFactoryModel>						GetProjectFactory()override;
		bool											OpenProject()override;
		bool											SaveProject(bool saveContainingFiles)override;
		bool											NewProjectAndSave()override;
		bool											RenameProject(WString newName)override;
		bool											RemoveProject()override;
		bool											AddFile(Ptr<IFileModel> file)override;
		
		ISolutionItemModel*								GetParent()override;
		Ptr<GuiImageData>								GetImage()override;
		WString											GetName()override;
		Ptr<description::IValueObservableList>			GetChildren()override;

		bool											GetIsFileItem()override;
		WString											GetFilePath()override;
		WString											GetFileDirectory()override;
		bool											GetIsSaved()override;
		vint											GetErrorCount()override;
		WString											GetErrorText(vint index)override;
		bool											OpenFileItem()override;
		bool											SaveFileItem()override;
	};

	class SolutionItem : public Object, public virtual ISolutionModel
	{
	protected:
		IStudioModel*									studioModel;
		list::ObservableList<Ptr<IProjectModel>>		projects;
		Ptr<IProjectFactoryModel>						projectFactory;
		WString											filePath;
		bool											isSaved;
		collections::List<WString>						errors;

	public:
		SolutionItem(IStudioModel* _studioModel, Ptr<IProjectFactoryModel> _projectFactory, WString _filePath);
		~SolutionItem();
		
		bool											OpenSolution()override;
		bool											SaveSolution(bool saveContainingProjects)override;
		bool											NewSolution()override;
		bool											AddProject(Ptr<IProjectModel> project)override;

		ISolutionItemModel*								GetParent()override;
		Ptr<GuiImageData>								GetImage()override;
		WString											GetName()override;
		Ptr<description::IValueObservableList>			GetChildren()override;

		bool											GetIsFileItem()override;
		WString											GetFilePath()override;
		WString											GetFileDirectory()override;
		bool											GetIsSaved()override;
		vint											GetErrorCount()override;
		WString											GetErrorText(vint index)override;
		bool											OpenFileItem()override;
		bool											SaveFileItem()override;
	};
}

#endif