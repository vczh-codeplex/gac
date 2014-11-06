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
	class FileFactoryModelBase : public Object, public virtual IFileFactoryModel
	{
	protected:
		Ptr<GuiImageData>								image;
		WString											name;
		WString											category;
		WString											description;
		WString											id;

	public:
		FileFactoryModelBase(WString _imageUrl, WString _name, WString _category, WString _description, WString _id);
		~FileFactoryModelBase();

		Ptr<GuiImageData>								GetImage()override;
		WString											GetName()override;
		WString											GetCategory()override;
		WString											GetDescription()override;
		WString											GetId()override;
	};

	class ProjectFactoryModelBase : public Object, public virtual IProjectFactoryModel, public virtual IFileFactoryFilterModel
	{
	protected:
		Ptr<GuiImageData>								image;
		Ptr<GuiImageData>								smallImage;
		WString											name;
		WString											description;
		WString											id;

	public:
		ProjectFactoryModelBase(WString _imageUrl, WString _smallImageUrl, WString _name, WString _description, WString _id);
		~ProjectFactoryModelBase();

		Ptr<GuiImageData>								GetImage()override;
		Ptr<GuiImageData>								GetFilterImage()override;
		WString											GetName()override;
		WString											GetDescription()override;
		WString											GetId()override;
		LazyList<Ptr<IFileFactoryFilterModel>>			GetChildren()override;
	};

	class AllFileFactoryFilterModel : public Object, public virtual IFileFactoryFilterModel
	{
	protected:
		Ptr<GuiImageData>								smallImage;
		List<Ptr<ProjectFactoryModelBase>>				projectFactories;

	public:
		AllFileFactoryFilterModel();
		~AllFileFactoryFilterModel();

		Ptr<GuiImageData>								GetFilterImage()override;
		WString											GetName()override;
		WString											GetId()override;
		LazyList<Ptr<IFileFactoryFilterModel>>			GetChildren()override;
	};

	class StudioModel : public Object, public virtual IStudioModel
	{
	protected:
		Ptr<AllFileFactoryFilterModel>					allProjects;
		List<Ptr<IFileFactoryModel>>					fileFactories;
		WString											fileCategory;
		list::ObservableList<Ptr<IFileFactoryModel>>	filteredFileFactories;
		list::ObservableList<Ptr<ISolutionItemModel>>	openingSolution;

	public:
		StudioModel();
		~StudioModel();

		LazyList<Ptr<IProjectFactoryModel>>				GetProjectModels()override;
		Ptr<description::IValueObservableList>			GetFileModels()override;

		WString											GetFileCategory()override;
		void											SetFileCategory(WString value)override;

		LazyList<Ptr<IFileFactoryFilterModel>>			GetFileFilters()override;
		Ptr<description::IValueObservableList>			GetOpeningSolution()override;

		void											OpenBrowser(WString url)override;
	};
}

#endif