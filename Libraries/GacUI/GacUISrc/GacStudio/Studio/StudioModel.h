/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacStudio::StudioModel

Interfaces:
***********************************************************************/

#ifndef GACSTUDIO_STUDIO_STUDIOMODEL
#define GACSTUDIO_STUDIO_STUDIOMODEL

#include "..\UI\GacStudioUI.h"

using namespace vl::collections;

namespace vm
{
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

	public:
		FileFactoryModel(WString _imageUrl, WString _smallImageUrl, WString _name, WString _category, WString _description, WString _id, WString _ext);
		FileFactoryModel(Ptr<GuiImageData> _image, Ptr<GuiImageData> _smallImage, WString _id);
		~FileFactoryModel();

		Ptr<GuiImageData>								GetImage()override;
		Ptr<GuiImageData>								GetSmallImage()override;
		WString											GetName()override;
		WString											GetCategory()override;
		WString											GetDescription()override;
		WString											GetId()override;
		WString											GetDefaultFileExt()override;
		bool											GenerateFile(Ptr<vm::ISolutionItemModel> project, WString filePath)override;
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
		Ptr<GuiImageData>								GetImage()override;
		WString											GetName()override;
		Ptr<description::IValueObservableList>			GetChildren()override;

		bool											GetIsFileItem()override;
		WString											GetFilePath()override;
		bool											GetIsSaved()override;
		vint											GetErrorCount()override;
		WString											GetErrorText(vint index)override;
		bool											OpenFileItem()override;
		bool											SaveFileItem()override;
		
		Ptr<ISolutionItemModel>							GetSolution();
		void											SetSolution(Ptr<ISolutionItemModel> solution);
	};

	class StudioModel : public Object, public virtual IStudioModel
	{
	protected:
		Ptr<FileFactoryFilterModel>						solutionProjectFactory;
		Ptr<FileFactoryFilterModel>						fileFilters;
		List<Ptr<IFileFactoryModel>>					fileFactories;
		WString											fileCategory;
		list::ObservableList<Ptr<IFileFactoryModel>>	filteredFileFactories;
		Ptr<RootSolutionItemModel>						openingSolution;

	public:
		StudioModel();
		~StudioModel();

		LazyList<Ptr<IProjectFactoryModel>>				GetProjectModels()override;
		Ptr<description::IValueObservableList>			GetFileModels()override;
		Ptr<IProjectFactoryModel>						GetFileFilters()override;

		WString											GetFileCategory()override;
		void											SetFileCategory(WString value)override;

		Ptr<ISolutionItemModel>							GetOpeningSolution()override;
		WString											GetOpeningSolutionPath()override;
		bool											GetHasOpeningSolution()override;

		Ptr<IProjectFactoryModel>						GetProjectFactory(WString id)override;
		Ptr<IFileFactoryModel>							GetFileFactory(WString id)override;

		bool											OpenSolution(WString filePath)override;
		bool											SaveSolution()override;
		bool											NewSolution(WString filePath)override;
		bool											CloseSolution()override;
		bool											AddProject(Ptr<vm::IProjectFactoryModel> projectFactory, WString filePath)override;

		void											OpenBrowser(WString url)override;
		void											PromptError(WString message)override;
	};
}

#endif