#include "StudioModel.h"
#include "SolutionModel.h"
#include <Windows.h>

using namespace vl::reflection::description;
using namespace vl::parsing::xml;

namespace vm
{
/***********************************************************************
FileFactoryModel
***********************************************************************/

	FileFactoryModel::FileFactoryModel(WString _imageUrl, WString _name, WString _category, WString _description, WString _id)
		:name(_name)
		, category(_category)
		, description(_description)
		, id(_id)
	{
		image = GetInstanceLoaderManager()->GetResource(L"GacStudioUI")->GetImageByPath(_imageUrl);
	}

	FileFactoryModel::~FileFactoryModel()
	{
	}

	Ptr<GuiImageData> FileFactoryModel::GetImage()
	{
		return image;
	}

	WString FileFactoryModel::GetName()
	{
		return name;
	}

	WString FileFactoryModel::GetCategory()
	{
		return category;
	}

	WString FileFactoryModel::GetDescription()
	{
		return description;
	}

	WString FileFactoryModel::GetId()
	{
		return id;
	}

/***********************************************************************
ProjectFactoryModel
***********************************************************************/
	
	ProjectFactoryModel::ProjectFactoryModel(WString _imageUrl, WString _smallImageUrl, WString _name, WString _description, WString _id)
		:name(_name)
		, description(_description)
		, id(_id)
	{
		if (_imageUrl != L"")
		{
			image = GetInstanceLoaderManager()->GetResource(L"GacStudioUI")->GetImageByPath(_imageUrl);
		}
		if (_smallImageUrl != L"")
		{
			smallImage = GetInstanceLoaderManager()->GetResource(L"GacStudioUI")->GetImageByPath(_smallImageUrl);
		}
	}

	ProjectFactoryModel::ProjectFactoryModel(Ptr<GuiImageData> _image, Ptr<GuiImageData> _smallImage, WString _id)
		:image(_image)
		, smallImage(_smallImage)
		, name(L"Unsupported project \"" + id + L"\"")
		, id(_id)
	{
	}

	ProjectFactoryModel::~ProjectFactoryModel()
	{
	}

	Ptr<GuiImageData> ProjectFactoryModel::GetImage()
	{
		return image;
	}

	Ptr<GuiImageData> ProjectFactoryModel::GetSmallImage()
	{
		return smallImage;
	}

	WString ProjectFactoryModel::GetName()
	{
		return name;
	}

	WString ProjectFactoryModel::GetDescription()
	{
		return description;
	}

	WString ProjectFactoryModel::GetId()
	{
		return id;
	}

	LazyList<Ptr<IProjectFactoryModel>> ProjectFactoryModel::GetChildren()
	{
		return MakePtr<List<Ptr<IProjectFactoryModel>>>();
	}

/***********************************************************************
AllFileFactoryFilterModel
***********************************************************************/
	
	FileFactoryFilterModel::FileFactoryFilterModel()
		:ProjectFactoryModel(L"", L"ProjectImages/AllSmall.png", L"All", L"", L"")
	{
	}

	FileFactoryFilterModel::~FileFactoryFilterModel()
	{
	}

	void FileFactoryFilterModel::AddChild(Ptr<ProjectFactoryModel> child)
	{
		children.Add(child);
	}

	LazyList<Ptr<IProjectFactoryModel>> FileFactoryFilterModel::GetChildren()
	{
		return From(children).Cast<IProjectFactoryModel>();
	}

/***********************************************************************
RootSolutionItemModel
***********************************************************************/

	Ptr<GuiImageData> RootSolutionItemModel::GetImage()
	{
		return 0;
	}

	WString RootSolutionItemModel::GetName()
	{
		return L"";
	}

	Ptr<description::IValueObservableList> RootSolutionItemModel::GetChildren()
	{
		return children.GetWrapper();
	}

	bool RootSolutionItemModel::GetIsFileItem()
	{
		return false;
	}

	WString RootSolutionItemModel::GetFilePath()
	{
		return L"";
	}

	bool RootSolutionItemModel::GetIsSaved()
	{
		throw true;
	}

	vint RootSolutionItemModel::GetErrorCount()
	{
		return 0;
	}

	WString RootSolutionItemModel::GetErrorText(vint index)
	{
		return L"";
	}

	bool RootSolutionItemModel::OpenFileItem()
	{
		return false;
	}

	bool RootSolutionItemModel::SaveFileItem()
	{
		return false;
	}

	Ptr<ISolutionItemModel> RootSolutionItemModel::GetSolution()
	{
		if (children.Count() == 0)return 0;
		return children[0];
	}

	void RootSolutionItemModel::SetSolution(Ptr<ISolutionItemModel> solution)
	{
		children.Clear();
		if (solution)
		{
			children.Add(solution);
		}
	}

/***********************************************************************
StudioModel
***********************************************************************/

	StudioModel::StudioModel()
	{
		Dictionary<WString, Ptr<ProjectFactoryModel>> configProjects;
		Dictionary<WString, Ptr<FileFactoryModel>> configFiles;
		{
			auto resources = GetInstanceLoaderManager()->GetResource(L"GacStudioUI");

			auto projects = resources->GetFolderByPath(L"Config/Projects/");
			FOREACH(Ptr<GuiResourceItem>, item, projects->GetItems())
			{
				auto xml = item->AsXml()->rootElement;
				auto id = item->GetName();
				auto image = XmlGetValue(XmlGetElement(xml, L"Image"));
				auto smallImage = XmlGetValue(XmlGetElement(xml, L"SmallImage"));
				auto display = XmlGetValue(XmlGetElement(xml, L"Display"));
				auto description = XmlGetValue(XmlGetElement(xml, L"Description"));
				configProjects.Add(id, new ProjectFactoryModel(image, smallImage, display, description, id));
			}

			auto files = resources->GetFolderByPath(L"Config/Files/");
			FOREACH(Ptr<GuiResourceItem>, item, files->GetItems())
			{
				auto xml = item->AsXml()->rootElement;
				auto id = item->GetName();
				auto image = XmlGetValue(XmlGetElement(xml, L"Image"));
				auto display = XmlGetValue(XmlGetElement(xml, L"Display"));
				auto description = XmlGetValue(XmlGetElement(xml, L"Description"));
				auto category = XmlGetValue(XmlGetElement(xml, L"Category"));
				configFiles.Add(id, new FileFactoryModel(image, display, category, description, id));
			}
		}
		auto allProjects = new FileFactoryFilterModel;
		FOREACH(Ptr<ProjectFactoryModel>, project, configProjects.Values())
		{
			allProjects->AddChild(project);
		}

		fileFilters = new FileFactoryFilterModel;
		fileFilters->AddChild(allProjects);

		openingSolution = new RootSolutionItemModel;
		
		FOREACH(Ptr<FileFactoryModel>, file, configFiles.Values())
		{
			fileFactories.Add(file);
			filteredFileFactories.Add(file);
		}
	}

	StudioModel::~StudioModel()
	{
	}

	LazyList<Ptr<IProjectFactoryModel>> StudioModel::GetProjectModels()
	{
		return fileFilters->GetChildren().First()->GetChildren();
	}

	Ptr<IValueObservableList> StudioModel::GetFileModels()
	{
		return filteredFileFactories.GetWrapper();
	}

	Ptr<IProjectFactoryModel> StudioModel::GetFileFilters()
	{
		return fileFilters;
	}

	WString StudioModel::GetFileCategory()
	{
		return fileCategory;
	}

	void StudioModel::SetFileCategory(WString value)
	{
		fileCategory = value;
		LazyList<Ptr<IFileFactoryModel>> source;
		if (fileCategory == L"")
		{
			source = fileFactories;
		}
		else
		{
			source = From(fileFactories)
				.Where([=](Ptr<IFileFactoryModel> model)
				{
					return model->GetCategory() == fileCategory;
				});
		}
		CopyFrom(filteredFileFactories, source);
	}

	Ptr<ISolutionItemModel> StudioModel::GetOpeningSolution()
	{
		return openingSolution;
	}

	bool StudioModel::GetHasOpeningSolution()
	{
		return openingSolution->GetSolution();
	}

	bool StudioModel::OpenSolution(WString filePath)
	{
		// EnsureAllOpeningFilesSaved();
		auto solution = MakePtr<SolutionItem>(fileFilters, filePath);
		if (!solution->OpenSolution()) return false;
		openingSolution->SetSolution(solution);
		return true;
	}

	bool StudioModel::SaveSolution()
	{
		// EnsureAllOpeningFilesSaved();
		auto solution = openingSolution->GetSolution().Cast<SolutionItem>();
		if (!solution) return false;
		return solution->SaveSolution();
	}

	bool StudioModel::NewSolution(WString filePath)
	{
		// EnsureAllOpeningFilesSaved();
		auto solution = MakePtr<SolutionItem>(fileFilters, filePath);
		if (!solution->NewSolution()) return false;
		openingSolution->SetSolution(solution);
		return true;
	}

	bool StudioModel::CloseSolution()
	{
		// EnsureAllOpeningFilesSaved();
		openingSolution->SetSolution(0);
		return true;
	}

	bool StudioModel::AddProject(Ptr<vm::IProjectFactoryModel> projectFactory, WString projectName)
	{
		return false;
	}

	void StudioModel::OpenBrowser(WString url)
	{
		ShellExecute(NULL, NULL, url.Buffer(), NULL, NULL, SHOW_FULLSCREEN);
	}
}