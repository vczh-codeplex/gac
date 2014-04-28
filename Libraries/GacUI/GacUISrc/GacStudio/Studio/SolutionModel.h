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
		Ptr<GuiImageData>			image;
		WString						name;
		WString						category;
		WString						description;
		WString						id;

	public:
		FileFactoryModelBase(WString _imageUrl, WString _name, WString _category, WString _description, WString _id);
		~FileFactoryModelBase();

		Ptr<GuiImageData>			GetImage()override;
		WString						GetName()override;
		WString						GetCategory()override;
		WString						GetDescription()override;
		WString						GetId()override;
	};

	class ProjectFactoryModelBase : public Object, public virtual IProjectFactoryModel
	{
	protected:
		Ptr<GuiImageData>			image;
		WString						name;
		WString						description;
		WString						id;

	public:
		ProjectFactoryModelBase(WString _imageUrl, WString _name, WString _description, WString _id);
		~ProjectFactoryModelBase();

		Ptr<GuiImageData>			GetImage()override;
		WString						GetName()override;
		WString						GetDescription()override;
		WString						GetId()override;
	};

	class StudioModel : public Object, public virtual IStudioModel
	{
	protected:
		List<Ptr<IProjectFactoryModel>>			projectFactories;
		List<Ptr<IFileFactoryModel>>			fileFactories;

	public:
		StudioModel();
		~StudioModel();

		LazyList<Ptr<IProjectFactoryModel>>		GetProjectModels()override;
		LazyList<Ptr<IFileFactoryModel>>		GetFileModels(WString category)override;
	};
}

#endif