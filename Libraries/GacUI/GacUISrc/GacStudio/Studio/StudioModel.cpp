#include "StudioModel.h"
#include "SolutionModel.h"
#include <Windows.h>

using namespace vl::stream;
using namespace vl::reflection::description;
using namespace vl::parsing::xml;
using namespace vl::filesystem;

namespace vm
{

/***********************************************************************
TextTemplate
***********************************************************************/

	TextTemplate::TextTemplate(const WString& content)
	{
		const wchar_t* reading = content.Buffer();
		while (*reading)
		{
			auto begin = wcsstr(reading, L"{$");
			if (!begin)
			{
				auto item = MakePtr<TextTemplateItem>();
				item->isMacro = false;
				item->content = reading;
				items.Add(item);
				break;
			}

			if (begin > reading)
			{
				auto item = MakePtr<TextTemplateItem>();
				item->isMacro = false;
				item->content = WString(reading, (vint)(begin - reading));
				items.Add(item);
			}

			auto end = wcsstr(begin, L"}");
			if (!end)
			{
				auto item = MakePtr<TextTemplateItem>();
				item->isMacro = false;
				item->content = begin;
				items.Add(item);
				break;
			}
			
			auto item = MakePtr<TextTemplateItem>();
			item->isMacro = true;
			item->content = WString(begin + 2, (vint)(end - begin - 2));
			items.Add(item);
			reading = end + 1;
		}
	}

	TextTemplate::~TextTemplate()
	{
	}

	WString TextTemplate::Generate(Ptr<IMacroEnvironment> macroEnvironment)
	{
		MemoryStream stream;
		{
			StreamWriter writer(stream);
			FOREACH(Ptr<TextTemplateItem>, item, items)
			{
				if (item->isMacro)
				{
					writer.WriteString(macroEnvironment->GetMacroValue(item->content, true));
				}
				else
				{
					writer.WriteString(item->content);
				}
			}
		}
		stream.SeekFromBegin(0);
		{
			StreamReader reader(stream);
			return reader.ReadToEnd();
		}
	}

/***********************************************************************
FileFactoryModel
***********************************************************************/

	FileFactoryModel::FileFactoryModel(WString _imageUrl, WString _smallImageUrl, WString _name, WString _category, WString _description, WString _id, WString _ext, Ptr<ITextTemplate> _textTemplate)
		:name(_name)
		, category(_category)
		, description(_description)
		, id(_id)
		, ext(_ext)
		, textTemplate(_textTemplate)
	{
		image = GetInstanceLoaderManager()->GetResource(L"GacStudioUI")->GetImageByPath(_imageUrl);
		smallImage = GetInstanceLoaderManager()->GetResource(L"GacStudioUI")->GetImageByPath(_smallImageUrl);
	}

	FileFactoryModel::FileFactoryModel(Ptr<GuiImageData> _image, Ptr<GuiImageData> _smallImage, WString _id)
		:image(_image)
		, smallImage(_smallImage)
		, name(L"Unsupported file \"" + id + L"\"")
		, id(_id)
	{
	}

	FileFactoryModel::~FileFactoryModel()
	{
	}

	Ptr<GuiImageData> FileFactoryModel::GetImage()
	{
		return image;
	}

	Ptr<GuiImageData> FileFactoryModel::GetSmallImage()
	{
		return smallImage;
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

	WString FileFactoryModel::GetDefaultFileExt()
	{
		return ext;
	}

	Ptr<ITextTemplate> FileFactoryModel::GetTextTemplate()
	{
		return textTemplate;
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

	ISolutionItemModel* RootSolutionItemModel::GetParent()
	{
		return nullptr;
	}

	Ptr<GuiImageData> RootSolutionItemModel::GetImage()
	{
		return nullptr;
	}

	WString RootSolutionItemModel::GetName()
	{
		return L"";
	}

	Ptr<description::IValueObservableList> RootSolutionItemModel::GetChildren()
	{
		return children.GetWrapper();
	}

	WString RootSolutionItemModel::GetFilePath()
	{
		return L"";
	}

	WString RootSolutionItemModel::GetFileDirectory()
	{
		return L"";
	}

	vint RootSolutionItemModel::GetErrorCount()
	{
		return 0;
	}

	WString RootSolutionItemModel::GetErrorText(vint index)
	{
		return L"";
	}

	Ptr<ISolutionModel> RootSolutionItemModel::GetSolution()
	{
		if (children.Count() == 0) return nullptr;
		return children[0].Cast<ISolutionModel>();
	}

	void RootSolutionItemModel::SetSolution(Ptr<ISolutionModel> solution)
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
				auto smallImage = XmlGetValue(XmlGetElement(xml, L"SmallImage"));
				auto display = XmlGetValue(XmlGetElement(xml, L"Display"));
				auto description = XmlGetValue(XmlGetElement(xml, L"Description"));
				auto category = XmlGetValue(XmlGetElement(xml, L"Category"));
				auto ext = XmlGetValue(XmlGetElement(xml, L"DefaultFileExt"));

				Ptr<ITextTemplate> textTemplate;
				if (auto element = XmlGetElement(xml, L"TextTemplate"))
				{
					textTemplate = new TextTemplate(XmlGetValue(element));
				}

				configFiles.Add(id, new FileFactoryModel(image, smallImage, display, category, description, id, ext, textTemplate));
			}
		}

		solutionProjectFactory = new FileFactoryFilterModel;
		FOREACH(Ptr<ProjectFactoryModel>, project, configProjects.Values())
		{
			solutionProjectFactory->AddChild(project);
		}

		fileFilters = new FileFactoryFilterModel;
		fileFilters->AddChild(solutionProjectFactory);

		rootSolutionItem = new RootSolutionItemModel;
		
		FOREACH(Ptr<FileFactoryModel>, file, configFiles.Values())
		{
			fileFactories.Add(file);
			filteredFileFactories.Add(file);
		}
	}

	StudioModel::~StudioModel()
	{
	}

	LazyList<Ptr<IProjectFactoryModel>> StudioModel::GetProjectFactories()
	{
		return fileFilters->GetChildren().First()->GetChildren();
	}

	LazyList<Ptr<IFileFactoryModel>> StudioModel::GetFileFactories()
	{
		return From(fileFactories);
	}

	LazyList<Ptr<IEditorFactoryModel>> StudioModel::GetEditorFactories()
	{
		return From(editorFactories);
	}

	Ptr<IProjectFactoryModel> StudioModel::GetFileFilters()
	{
		return fileFilters;
	}

	Ptr<IProjectFactoryModel> StudioModel::GetSelectedFileFilter()
	{
		return selectedFileFilter;
	}

	void StudioModel::SetSelectedFileFilter(Ptr<IProjectFactoryModel> value)
	{
		selectedFileFilter = value;
		LazyList<Ptr<IFileFactoryModel>> source;
		if (!selectedFileFilter || selectedFileFilter->GetId() == L"")
		{
			source = fileFactories;
		}
		else
		{
			source = From(fileFactories)
				.Where([=](Ptr<IFileFactoryModel> model)
				{
					return model->GetCategory() == selectedFileFilter->GetId();
				});
		}
		CopyFrom(filteredFileFactories, source);
	}

	Ptr<IValueObservableList> StudioModel::GetFilteredFileFactories()
	{
		return filteredFileFactories.GetWrapper();
	}

	Ptr<ISolutionItemModel> StudioModel::GetRootSolutionItem()
	{
		return rootSolutionItem;
	}

	Ptr<ISolutionModel> StudioModel::GetOpenedSolution()
	{
		return rootSolutionItem->GetSolution();
	}

	void StudioModel::NotifySelectedSolutionItem(Ptr<ISolutionItemModel> selectedItem)
	{
		selectedSolutionItem = selectedItem;
		WorkingItemChanged();
		WorkingProjectChanged();
		WorkingDirectoryChanged();
	}

	Ptr<ISolutionItemModel> StudioModel::GetWorkingItem()
	{
		return selectedSolutionItem;
	}

	Ptr<IProjectModel> StudioModel::GetWorkingProject()
	{
		auto item = selectedSolutionItem.Obj();
		while (item)
		{
			if (auto projectModel = dynamic_cast<IProjectModel*>(item))
			{
				return projectModel;
			}
			item = item->GetParent();
		}
		return nullptr;
	}

	WString StudioModel::GetWorkingDirectory()
	{
		return selectedSolutionItem ? selectedSolutionItem->GetFileDirectory() : L"";
	}

	Ptr<IProjectFactoryModel> StudioModel::GetProjectFactory(WString id)
	{
		return GetProjectFactories()
			.Where([=](Ptr<IProjectFactoryModel> model)
			{
				return model->GetId() == id;
			})
		.First(nullptr);
	}

	Ptr<IFileFactoryModel> StudioModel::GetFileFactory(WString id)
	{
		return From(fileFactories)
			.Where([=](Ptr<IFileFactoryModel> model)
			{
				return model->GetId() == id;
			})
		.First(nullptr);
	}

	Ptr<IEditorFactoryModel> StudioModel::GetEditorFactory(WString id)
	{
		return From(editorFactories)
			.Where([=](Ptr<IEditorFactoryModel> model)
			{
				return model->GetId() == id;
			})
		.First(nullptr);
	}

	void StudioModel::OpenSolution(WString filePath)
	{
		// EnsureAllOpeningFilesSaved();
		auto solution = MakePtr<SolutionItem>(this, solutionProjectFactory, filePath);
		solution->OpenSolution();
		rootSolutionItem->SetSolution(solution);
		OpenedSolutionChanged();
	}

	void StudioModel::SaveSolution()
	{
		// EnsureAllOpeningFilesSaved();
		auto solution = rootSolutionItem->GetSolution();
		if (!solution)
		{
			throw StudioException(L"Internal error: Solution does not exist.", true);
		}
		return solution->SaveSolution(true);
	}

	void StudioModel::NewSolution(WString filePath)
	{
		// EnsureAllOpeningFilesSaved();
		auto solution = MakePtr<SolutionItem>(this, solutionProjectFactory, filePath);
		solution->NewSolution();
		rootSolutionItem->SetSolution(solution);
		OpenedSolutionChanged();
	}

	void StudioModel::CloseSolution()
	{
		// EnsureAllOpeningFilesSaved();
		rootSolutionItem->SetSolution(0);
		OpenedSolutionChanged();
	}

	vl::Ptr<vm::IProjectModel> StudioModel::AddNewProject(bool createNewSolution, vl::Ptr<vm::IProjectFactoryModel> projectFactory, vl::WString projectName, vl::WString solutionDirectory, vl::WString solutionName)
	{
		if (!projectFactory)
		{
			throw StudioException(L"Failed to add a project.", true);
		}

		auto solutionFolder =
			createNewSolution
			? Folder(FilePath(solutionDirectory) / solutionName)
			: Folder(FilePath(GetOpenedSolution()->GetFileDirectory()))
			;
		auto solutionPath = solutionFolder.GetFilePath() / (solutionName + L".gacsln.xml");
		auto projectFolder = Folder(solutionFolder.GetFilePath() / projectName);
		auto projectPath = projectFolder.GetFilePath() / (projectName + L".gacproj.xml");

		if (createNewSolution)
		{
			if (!solutionFolder.Create(true))
			{
				throw StudioException(L"Failed to create empty folder \"" + solutionFolder.GetFilePath().GetFullPath() + L"\".", false);
			}
		}
		if (!projectFolder.Create(true))
		{
			throw StudioException(L"Failed to create empty folder \"" + projectFolder.GetFilePath().GetFullPath() + L"\".", false);
		}

		if (createNewSolution)
		{
			NewSolution(solutionPath.GetFullPath());
		}
		
		auto projectItem = MakePtr<ProjectItem>(this, projectFactory, projectPath.GetFullPath());
		projectItem->NewProjectAndSave();
		GetOpenedSolution()->AddProject(projectItem);
		GetOpenedSolution()->SaveSolution(false);

		return projectItem;
	}

	vl::Ptr<vm::IFileModel> StudioModel::AddNewFile(vl::Ptr<vm::IAddFileItemAction> action, vl::Ptr<vm::IProjectModel> project, vl::Ptr<vm::IFileFactoryModel> fileFactory, vl::WString fileDirectory, vl::WString fileName)
	{
		if (!project || !fileFactory)
		{
			throw StudioException(L"Failed to add a file.", true);
		}
		
		auto fileFolder = Folder(fileDirectory);
		auto filePath = fileFolder.GetFilePath() / (fileName + fileFactory->GetDefaultFileExt());

		if (!fileFolder.Exists() && !fileFolder.Create(true))
		{
			throw StudioException(L"Failed to create folder \"" + fileFolder.GetFilePath().GetFullPath() + L"\".", true);
		}
		
		auto fileItem = MakePtr<FileItem>(this, fileFactory, filePath.GetFullPath());
		fileItem->NewFileAndSave();
		action->AddFile(fileItem);
		project->SaveProject(false);

		return fileItem;
	}

	void StudioModel::OpenBrowser(WString url)
	{
		ShellExecute(NULL, NULL, url.Buffer(), NULL, NULL, SHOW_FULLSCREEN);
	}

	void StudioModel::PromptError(WString message)
	{
		auto mainWindow = GetApplication()->GetMainWindow();
		GetCurrentController()->DialogService()->ShowMessageBox(
			mainWindow->GetNativeWindow(),
			message,
			mainWindow->GetText(),
			INativeDialogService::DisplayOK,
			INativeDialogService::DefaultFirst,
			INativeDialogService::IconError
			);
	}

	bool StudioModel::SafeExecute(vl::Func<void()> procedure)
	{
		try
		{
			procedure();
			return true;
		}
		catch (const vm::StudioException& ex)
		{
			PromptError(ex.Message());
			return ex.IsNonConfigError();
		}
	}
}