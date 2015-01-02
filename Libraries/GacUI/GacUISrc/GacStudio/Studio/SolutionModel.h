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
		bool											NewFile()override;
		
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

	class FolderItem : public Object, public virtual IFolderModel
	{
	protected:
		list::ObservableList<Ptr<ISolutionItemModel>>	children;
		Ptr<GuiImageData>								image;
		WString											filePath;
		ISolutionItemModel*								parent;

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

	class ProjectItem : public Object, public virtual IProjectModel
	{
	protected:
		IStudioModel*									studioModel;
		list::ObservableList<Ptr<ISolutionItemModel>>	children;
		Ptr<IProjectFactoryModel>						projectFactory;
		WString											filePath;
		List<Ptr<IFileModel>>								fileItems;
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
		bool											NewProject()override;
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