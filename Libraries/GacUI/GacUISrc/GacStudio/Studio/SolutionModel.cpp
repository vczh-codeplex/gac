#include "SolutionModel.h"

namespace vm
{
/***********************************************************************
FileFactoryModelBase
***********************************************************************/

	FileFactoryModelBase::FileFactoryModelBase(WString _imageUrl, WString _name, WString _category, WString _description, WString _id)
		:name(_name)
		, category(_category)
		, description(_description)
		, id(_id)
	{
		image = GetInstanceLoaderManager()->GetResource(L"GacStudioUI")->GetImageByPath(_imageUrl);
	}

	FileFactoryModelBase::~FileFactoryModelBase()
	{

	}

	Ptr<GuiImageData> FileFactoryModelBase::GetImage()
	{
		return image;
	}

	WString FileFactoryModelBase::GetName()
	{
		return name;
	}

	WString FileFactoryModelBase::GetCategory()
	{
		return category;
	}

	WString FileFactoryModelBase::GetDescription()
	{
		return description;
	}

	WString FileFactoryModelBase::GetId()
	{
		return id;
	}

/***********************************************************************
ProjectFactoryModelBase
***********************************************************************/
	
	ProjectFactoryModelBase::ProjectFactoryModelBase(WString _imageUrl, WString _name, WString _description, WString _id)
		:name(_name)
		, description(_description)
		, id(_id)
	{
		image = GetInstanceLoaderManager()->GetResource(L"GacStudioUI")->GetImageByPath(_imageUrl);
	}

	ProjectFactoryModelBase::~ProjectFactoryModelBase()
	{

	}

	Ptr<GuiImageData> ProjectFactoryModelBase::GetImage()
	{
		return image;
	}

	WString ProjectFactoryModelBase::GetName()
	{
		return name;
	}

	WString ProjectFactoryModelBase::GetDescription()
	{
		return description;
	}

	WString ProjectFactoryModelBase::GetId()
	{
		return id;
	}

/***********************************************************************
ResourceProjectFactory
***********************************************************************/

	class ResourceProjectFactory : public ProjectFactoryModelBase
	{
	public:
		ResourceProjectFactory()
			:ProjectFactoryModelBase(
				L"ProjectImages/Dialog.png",
				L"GacUI Resource",
				L"Create an embedded resource for your GacUI program.",
				L"RESOURCE_PROJECT")
		{
		}
	};

/***********************************************************************
ParserProjectFactory
***********************************************************************/

	class ParserProjectFactory : public ProjectFactoryModelBase
	{
	public:
		ParserProjectFactory()
			:ProjectFactoryModelBase(
				L"ProjectImages/Parser.png",
				L"Parser",
				L"Create a parser for your GacUI program.",
				L"PARSER_PROJECT")
		{
		}
	};

/***********************************************************************
StudioModel
***********************************************************************/

	StudioModel::StudioModel()
	{
		projectFactories.Add(new ResourceProjectFactory);
		projectFactories.Add(new ParserProjectFactory);
	}

	StudioModel::~StudioModel()
	{
	}

	LazyList<Ptr<IProjectFactoryModel>> StudioModel::GetProjectModels()
	{
		return projectFactories;
	}

	LazyList<Ptr<IFileFactoryModel>> StudioModel::GetFileModels(WString category)
	{
		if (category == L"")
		{
			return fileFactories;
		}
		else
		{
			return From(fileFactories)
				.Where([=](Ptr<IFileFactoryModel> model)
				{
					return model->GetCategory() == category;
				});
		}
	}
}