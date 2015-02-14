#include "SolutionModel.h"
#include "StudioModel.h"

using namespace vl::parsing::xml;
using namespace vl::filesystem;
using namespace vl::stream;
using namespace vl::reflection::description;
using namespace vl::presentation;

namespace vm
{
	WString GetDisplayNameFromFilePath(WString filePath, WString extension)
	{
		auto index = INVLOC.FindLast(filePath, L"\\", Locale::None);
		if (index.key == -1)
		{
			return filePath;
		}

		WString fileName = filePath.Sub(index.key + 1, filePath.Length() - index.key - 1);
		if (fileName.Length() >= extension.Length() && fileName.Right(extension.Length()) == extension)
		{
			return fileName.Left(fileName.Length() - extension.Length());
		}
		else
		{
			return fileName;
		}
	}
	
	WString GetDisplayNamePreviewFromFilePath(WString filePath, WString extension, WString newName)
	{
		auto index = INVLOC.FindLast(filePath, L"\\", Locale::None);
		if (index.key == -1)
		{
			return newName;
		}

		WString fileName = filePath.Sub(index.key + 1, filePath.Length() - index.key - 1);
		if (fileName.Length() >= extension.Length() && fileName.Right(extension.Length()) == extension)
		{
			return filePath.Left(index.key + 1) + newName + extension;
		}
		else
		{
			return filePath.Left(index.key + 1) + newName;
		}
	}

	ProjectItem* GetOwnerProject(ISolutionItemModel* item)
	{
		auto current = item;
		while (current)
		{
			if (auto project = dynamic_cast<ProjectItem*>(current))
			{
				return project;
			}
			current = current->GetParent();
		}
		return nullptr;
	}

/***********************************************************************
FileMacroEnvironment
***********************************************************************/

		FileMacroEnvironment::FileMacroEnvironment(IFileModel* _fileModel)
			:fileModel(_fileModel)
		{
		}

		FileMacroEnvironment::~FileMacroEnvironment()
		{
		}

		IMacroEnvironment* FileMacroEnvironment::GetParent()
		{
			return nullptr;
		}

		bool FileMacroEnvironment::HasMacro(WString name, bool inherit)
		{
			return
				name == L"FILENAME" ||
				name == L"FILEEXT" ||
				name == L"FILEDIR" ||
				name == L"FILEPATH"
				;
		}

		WString FileMacroEnvironment::GetMacroValue(WString name, bool inherit)
		{
			if (name == L"FILENAME")
			{
				auto ext = fileModel->GetFileFactory()->GetDefaultFileExt();
				auto dir = fileModel->GetFileDirectory();
				auto path = fileModel->GetFilePath();
				path = path.Right(path.Length() - dir.Length() - 1);

				if (path.Length() >= ext.Length() && path.Right(ext.Length()) == ext)
				{
					return path.Left(path.Length() - ext.Length());
				}
				else
				{
					return path;
				}
			}
			else if (name == L"FILEEXT")
			{
				auto ext = fileModel->GetFileFactory()->GetDefaultFileExt();
				auto path = fileModel->GetFilePath();
				if (path.Length() >= ext.Length() && path.Right(ext.Length()) == ext)
				{
					return ext;
				}
				else
				{
					return L"";
				}
			}
			else if (name == L"FILEDIR")
			{
				return fileModel->GetFileDirectory();
			}
			else if (name == L"FILEPATH")
			{
				return fileModel->GetFilePath();
			}
			else
			{
				return L"";
			}
		}

/***********************************************************************
FileItem
***********************************************************************/

	FileItem::FileItem(IStudioModel* _studioModel, Ptr<IFileFactoryModel> _fileFactory, WString _filePath, bool _unsupported)
		:studioModel(_studioModel)
		, fileFactory(_fileFactory)
		, filePath(_filePath)
		, unsupported(_unsupported)
		, parent(0)
	{
		if (unsupported)
		{
			errors.Add(L"This file is not supported.");
		}
	}

	FileItem::~FileItem()
	{
	}

	WString FileItem::GetRenameablePart()
	{
		return GetDisplayNameFromFilePath(filePath, fileFactory->GetDefaultFileExt());
	}

	WString FileItem::PreviewRename(WString newName)
	{
		return GetDisplayNamePreviewFromFilePath(filePath, fileFactory->GetDefaultFileExt(), newName);
	}

	bool FileItem::Rename(WString newName)
	{
		throw 0;
	}

	bool FileItem::Remove()
	{
		if (auto project = GetOwnerProject(this))
		{
			return project->RemoveFileItem(this);
		}
		return false;
	}

	Ptr<IFileFactoryModel> FileItem::GetFileFactory()
	{
		return fileFactory;
	}

	bool FileItem::OpenFile()
	{
		throw 0;
	}

	bool FileItem::SaveFile()
	{
		return true;
	}

	bool FileItem::NewFileAndSave()
	{
		if (fileFactory->GetTextTemplate())
		{
			auto env = MakePtr<FileMacroEnvironment>(this);
			WString text = fileFactory->GetTextTemplate()->Generate(env);

			FileStream fileStream(filePath, FileStream::WriteOnly);
			if (!fileStream.IsAvailable()) return false;
			BomEncoder encoder(BomEncoder::Utf16);
			EncoderStream encoderStream(fileStream, encoder);
			StreamWriter writer(encoderStream);
			writer.WriteString(text);
			return true;
		}
		return false;
	}

	ISolutionItemModel* FileItem::GetParent()
	{
		return parent;
	}

	Ptr<GuiImageData> FileItem::GetImage()
	{
		return fileFactory->GetSmallImage();
	}

	WString FileItem::GetName()
	{
		return GetDisplayNameFromFilePath(filePath, fileFactory->GetDefaultFileExt());
	}

	Ptr<description::IValueObservableList> FileItem::GetChildren()
	{
		return children.GetWrapper();
	}

	WString FileItem::GetFilePath()
	{
		return filePath;
	}

	WString FileItem::GetFileDirectory()
	{
		return FilePath(filePath).GetFolder().GetFullPath();
	}

	vint FileItem::GetErrorCount()
	{
		return errors.Count();
	}

	WString FileItem::GetErrorText(vint index)
	{
		return 0 <= index && index < errors.Count() ? errors[index] : L"";
	}

/***********************************************************************
FolderItemBase
***********************************************************************/

	void FolderItemBase::ClearInternal()
	{
		children.Clear();
		folderNames.Clear();
		fileNames.Clear();
	}

	bool FolderItemBase::AddFileItemInternal(const wchar_t* filePath, Ptr<IFileModel> fileItem)
	{
		auto delimiter = wcschr(filePath, FilePath::Delimiter);
		if (delimiter)
		{
			auto name = WString(filePath, (vint)(delimiter - filePath));
			auto key = wupper(name);
			Ptr<FolderItem> folder;
			auto index = folderNames.IndexOf(key);

			if (index == -1)
			{
				index = folderNames.Add(key);
				auto solutionItem = dynamic_cast<ISolutionItemModel*>(this);
				auto folderPath = (FilePath(solutionItem->GetFileDirectory()) / name).GetFullPath();
				folder = MakePtr<FolderItem>(solutionItem, folderPath);
				children.Insert(index, folder);
			}
			else
			{
				folder = children[index].Cast<FolderItem>();
			}

			return folder->AddFileItemInternal(delimiter + 1, fileItem);
		}
		else
		{
			auto key = wupper(filePath);
			if (fileNames.Contains(key))
			{
				return false;
			}
			auto index = fileNames.Add(key);
			children.Insert(folderNames.Count() + index, fileItem);

			if (auto file = fileItem.Cast<FileItem>())
			{
				file->parent = dynamic_cast<ISolutionItemModel*>(this);
			}
			return true;
		}
	}

	bool FolderItemBase::RemoveFileItemInternal(const wchar_t* filePath, Ptr<IFileModel> fileItem)
	{
		auto delimiter = wcschr(filePath, FilePath::Delimiter);
		if (delimiter)
		{
			auto name = WString(filePath, (vint)(delimiter - filePath));
			auto key = wupper(name);
			auto index = folderNames.IndexOf(key);
			if (index == -1)
			{
				return false;
			}

			auto folder = children[index].Cast<FolderItem>();
			if (folder->RemoveFileItemInternal(delimiter + 1, fileItem))
			{
				folderNames.RemoveAt(index);
				children.RemoveAt(index);
			}
		}
		else
		{
			auto key = wupper(filePath);
			auto index = fileNames.IndexOf(key);
			if (index != -1)
			{
				fileNames.RemoveAt(index);
				children.RemoveAt(folderNames.Count() + index);

				if (auto file = fileItem.Cast<FileItem>())
				{
					file->parent = nullptr;
				}
			}
		}
		return children.Count() == 0;
	}

	FolderItemBase::FolderItemBase()
	{
	}

	FolderItemBase::~FolderItemBase()
	{
	}

/***********************************************************************
FolderItem
***********************************************************************/

	FolderItem::FolderItem(ISolutionItemModel* _parent, WString _filePath)
		:parent(_parent)
		, filePath(_filePath)
	{
		image = GetInstanceLoaderManager()->GetResource(L"GacStudioUI")->GetImageByPath(L"FileImages/FolderSmall.png");
	}

	FolderItem::~FolderItem()
	{
	}
	
	bool FolderItem::AddFile(Ptr<IFileModel> file)
	{
		if (auto project = GetOwnerProject(this))
		{
			return project->AddFileItem(file);
		}
		return false;
	}

	WString FolderItem::GetRenameablePart()
	{
		return GetDisplayNameFromFilePath(filePath, L"");
	}

	WString FolderItem::PreviewRename(WString newName)
	{
		return GetDisplayNamePreviewFromFilePath(filePath, L"", newName);
	}

	bool FolderItem::Rename(WString newName)
	{
		throw 0;
	}

	bool FolderItem::Remove()
	{
		auto project = GetOwnerProject(this);
		if (!project)
		{
			return false;
		}

		List<Ptr<IFileModel>> files;
		List<FolderItem*> items;
		items.Add(this);
		vint current = 0;

		while (current < items.Count())
		{
			auto folder = items[current++];
			FOREACH(Ptr<ISolutionItemModel>, item, folder->children)
			{
				if (auto subFolder = item.Cast<FolderItem>())
				{
					items.Add(subFolder.Obj());
				}
				else if (auto fileItem = item.Cast<IFileModel>())
				{
					files.Add(fileItem);
				}
			}
		}

		FOREACH(Ptr<IFileModel>, fileItem, files)
		{
			if (!project->RemoveFileItem(fileItem))
			{
				return false;
			}
		}

		return true;
	}

	ISolutionItemModel* FolderItem::GetParent()
	{
		return parent;
	}

	Ptr<GuiImageData> FolderItem::GetImage()
	{
		return image;
	}

	WString FolderItem::GetName()
	{
		return GetDisplayNameFromFilePath(filePath, L"");
	}

	Ptr<description::IValueObservableList> FolderItem::GetChildren()
	{
		return children.GetWrapper();
	}

	WString FolderItem::GetFilePath()
	{
		return L"";
	}

	WString FolderItem::GetFileDirectory()
	{
		return filePath;
	}

	vint FolderItem::GetErrorCount()
	{
		return 0;
	}

	WString FolderItem::GetErrorText(vint index)
	{
		return L"";
	}

/***********************************************************************
ProjectItem
***********************************************************************/

	WString ProjectItem::GetNormalizedRelativePath(Ptr<IFileModel> fileItem)
	{
		auto projectFolder = FilePath(filePath).GetFolder();
		auto filePath = FilePath(fileItem->GetFilePath());
		auto relativePath = projectFolder.GetRelativePathFor(filePath);
		if (relativePath.Length() >= 2 && relativePath[0] == L'.' && relativePath[1] == FilePath::Delimiter)
		{
			return relativePath.Right(relativePath.Length() - 2);
		}
		else
		{
			return relativePath;
		}
	}

	bool ProjectItem::AddFileItem(Ptr<IFileModel> fileItem)
	{
		if (fileItems.Contains(fileItem.Obj())) return false;
		auto path = GetNormalizedRelativePath(fileItem);
		if (!AddFileItemInternal(path.Buffer(), fileItem))
		{
			return false;
		}
		fileItems.Add(fileItem);
		return true;
	}

	bool ProjectItem::RemoveFileItem(Ptr<IFileModel> fileItem)
	{
		auto index = fileItems.IndexOf(fileItem.Obj());
		if (index == -1)
		{
			return false;
		}
		auto path = GetNormalizedRelativePath(fileItem);
		RemoveFileItemInternal(path.Buffer(), fileItem);
		fileItems.RemoveAt(index);
		return true;
	}

	ProjectItem::ProjectItem(IStudioModel* _studioModel, Ptr<IProjectFactoryModel> _projectFactory, WString _filePath, bool _unsupported)
		:studioModel(_studioModel)
		, projectFactory(_projectFactory)
		, filePath(_filePath)
		, unsupported(_unsupported)
	{
		if (unsupported)
		{
			errors.Add(L"This project is not supported.");
		}
	}

	ProjectItem::~ProjectItem()
	{
	}

	bool ProjectItem::AddFile(Ptr<IFileModel> file)
	{
		return AddFileItem(file);
	}

	WString ProjectItem::GetRenameablePart()
	{
		return GetDisplayNameFromFilePath(filePath, L".gacproj.xml");
	}

	WString ProjectItem::PreviewRename(WString newName)
	{
		return GetDisplayNamePreviewFromFilePath(filePath, L".gacproj.xml", newName);
	}

	bool ProjectItem::Rename(WString newName)
	{
		throw 0;
	}

	bool ProjectItem::Remove()
	{
		return studioModel->GetOpenedSolution()->RemoveProject(this);
	}

	Ptr<IProjectFactoryModel> ProjectItem::GetProjectFactory()
	{
		return projectFactory;
	}
		
	bool ProjectItem::OpenProject()
	{
		if (unsupported) return false;
		errors.Clear();

		auto projectFolder = FilePath(filePath).GetFolder();
		Ptr<XmlDocument> xml;
		{
			FileStream fileStream(filePath, FileStream::ReadOnly);
			if (!fileStream.IsAvailable())
			{
				errors.Add(L"Failed to read \"" + filePath + L"\".");
				ErrorCountChanged();
			}
			BomDecoder decoder;
			DecoderStream decoderStream(fileStream, decoder);
			StreamReader reader(decoderStream);

			auto parser = GetParserManager()->GetParser<XmlDocument>(L"XML");
			if (!parser) return false;

			xml = parser->TypedParse(reader.ReadToEnd(), errors);
		}

		if (xml)
		{
			ClearInternal();
			fileItems.Clear();

			auto attFactoryId = XmlGetAttribute(xml->rootElement, L"Factory");
			if (!attFactoryId)
			{
				errors.Add(L"Attribute \"Factory\" is missing.");
			}
			else if (attFactoryId->value.value != projectFactory->GetId())
			{
				errors.Add(L"This project is not matched with the solution specification.");
				unsupported = true;
			}
			
			FOREACH(Ptr<XmlElement>, xmlProject, XmlGetElements(xml->rootElement, L"GacStudioFile"))
			{
				auto attFactoryId = XmlGetAttribute(xmlProject, L"Factory");
				auto attFilePath = XmlGetAttribute(xmlProject, L"FilePath");
				if (!attFactoryId)
				{
					errors.Add(L"Attribute \"Factory\" is missing.");
				}
				if (!attFilePath)
				{
					errors.Add(L"Attribute \"FilePath\" is missing.");
				}

				if (attFactoryId && attFilePath)
				{
					auto factoryId = attFactoryId->value.value;
					auto filePath = attFilePath->value.value;
					auto factory = studioModel->GetFileFactory(factoryId);
					bool unsupported = false;
					if (!factory)
					{
						unsupported = true;
						factory = new FileFactoryModel(projectFactory->GetImage(), projectFactory->GetSmallImage(), factoryId);
						errors.Add(L"Unrecognizable project factory id \"" + factoryId + L"\".");
					}
					auto file = new FileItem(studioModel, factory, (projectFolder / filePath).GetFullPath(), unsupported);
					AddFileItem(file);
				}
			}
		}

		ErrorCountChanged();
		return true;
	}

	bool ProjectItem::SaveProject(bool saveContainingFiles)
	{
		if (unsupported) return false;
		auto projectFolder = FilePath(filePath).GetFolder();
		auto xml = MakePtr<XmlDocument>();
		auto xmlSolution = MakePtr<XmlElement>();
		xmlSolution->name.value = L"GacStudioProject";
		xml->rootElement = xmlSolution;
		{
			XmlElementWriter xmlWriter(xmlSolution);
			xmlWriter.Attribute(L"Factory", projectFactory->GetId());

			FOREACH(Ptr<IFileModel>, fileItem, fileItems)
			{
				xmlWriter
					.Element(L"GacStudioFile")
					.Attribute(L"Factory", fileItem->GetFileFactory()->GetId())
					.Attribute(L"FilePath", projectFolder.GetRelativePathFor(fileItem->GetFilePath()))
					;
			}
		}
		{
			FileStream fileStream(filePath, FileStream::WriteOnly);
			if (!fileStream.IsAvailable())
			{
				errors.Add(L"Failed to write \"" + filePath + L"\".");
				ErrorCountChanged();
				return false;
			}
			BomEncoder encoder(BomEncoder::Utf16);
			EncoderStream encoderStream(fileStream, encoder);
			StreamWriter writer(encoderStream);
			XmlPrint(xml, writer);
		}
		if (saveContainingFiles)
		{
			FOREACH(Ptr<IFileModel>, fileItem, fileItems)
			{
				fileItem->SaveFile();
			}
		}
		return true;
	}

	bool ProjectItem::NewProjectAndSave()
	{
		if (unsupported) return false;
		ClearInternal();
		fileItems.Clear();
		return SaveProject(false);
	}

	ISolutionItemModel* ProjectItem::GetParent()
	{
		return studioModel->GetOpenedSolution().Obj();
	}

	Ptr<GuiImageData> ProjectItem::GetImage()
	{
		return projectFactory->GetSmallImage();
	}

	WString ProjectItem::GetName()
	{
		return GetDisplayNameFromFilePath(filePath, L".gacproj.xml");
	}

	Ptr<description::IValueObservableList> ProjectItem::GetChildren()
	{
		return children.GetWrapper();
	}

	WString ProjectItem::GetFilePath()
	{
		return filePath;
	}

	WString ProjectItem::GetFileDirectory()
	{
		return FilePath(filePath).GetFolder().GetFullPath();
	}

	vint ProjectItem::GetErrorCount()
	{
		return errors.Count();
	}

	WString ProjectItem::GetErrorText(vint index)
	{
		return 0 <= index && index < errors.Count() ? errors[index] : L"";
	}

/***********************************************************************
SolutionItem
***********************************************************************/

	SolutionItem::SolutionItem(IStudioModel* _studioModel, Ptr<IProjectFactoryModel> _projectFactory, WString _filePath)
		:studioModel(_studioModel)
		, projectFactory(_projectFactory)
		, filePath(_filePath)
	{
	}

	SolutionItem::~SolutionItem()
	{
	}
		
	bool SolutionItem::OpenSolution()
	{
		projects.Clear();
		errors.Clear();

		auto solutionFolder = FilePath(filePath).GetFolder();
		Ptr<XmlDocument> xml;
		{
			FileStream fileStream(filePath, FileStream::ReadOnly);
			if (!fileStream.IsAvailable())
			{
				errors.Add(L"Failed to read \"" + filePath + L"\".");
				ErrorCountChanged();
			}
			BomDecoder decoder;
			DecoderStream decoderStream(fileStream, decoder);
			StreamReader reader(decoderStream);

			auto parser = GetParserManager()->GetParser<XmlDocument>(L"XML");
			if (!parser) return false;

			xml = parser->TypedParse(reader.ReadToEnd(), errors);
		}

		if (xml)
		{
			projects.Clear();
			FOREACH(Ptr<XmlElement>, xmlProject, XmlGetElements(xml->rootElement, L"GacStudioProject"))
			{
				auto attFactoryId = XmlGetAttribute(xmlProject, L"Factory");
				auto attFilePath = XmlGetAttribute(xmlProject, L"FilePath");
				if (!attFactoryId)
				{
					errors.Add(L"Attribute \"Factory\" is missing.");
				}
				if (!attFilePath)
				{
					errors.Add(L"Attribute \"FilePath\" is missing.");
				}

				if (attFactoryId && attFilePath)
				{
					auto factoryId = attFactoryId->value.value;
					auto projectPath = attFilePath->value.value;
					auto factory = studioModel->GetProjectFactory(factoryId);
					bool unsupported = false;
					if (!factory)
					{
						unsupported = true;
						factory = new ProjectFactoryModel(projectFactory->GetImage(), projectFactory->GetSmallImage(), factoryId);
						errors.Add(L"Unrecognizable project factory id \"" + factoryId + L"\".");
					}
					auto project = new ProjectItem(studioModel, factory, (solutionFolder / projectPath).GetFullPath(), unsupported);
					projects.Add(project);
					project->OpenProject();
				}
			}
		}

		ErrorCountChanged();
		return true;
	}

	bool SolutionItem::SaveSolution(bool saveContainingProjects)
	{
		auto solutionFolder = FilePath(filePath).GetFolder();
		auto xml = MakePtr<XmlDocument>();
		auto xmlSolution = MakePtr<XmlElement>();
		xmlSolution->name.value = L"GacStudioSolution";
		xml->rootElement = xmlSolution;
		{
			XmlElementWriter xmlWriter(xmlSolution);
			FOREACH(Ptr<ProjectItem>, project, From(projects).Cast<ProjectItem>())
			{
				xmlWriter
					.Element(L"GacStudioProject")
					.Attribute(L"Factory", project->GetProjectFactory()->GetId())
					.Attribute(L"FilePath", solutionFolder.GetRelativePathFor(project->GetFilePath()))
					;
			}
		}
		{
			FileStream fileStream(filePath, FileStream::WriteOnly);
			if (!fileStream.IsAvailable())
			{
				errors.Add(L"Failed to write \"" + filePath + L"\".");
				ErrorCountChanged();
				return false;
			}
			BomEncoder encoder(BomEncoder::Utf16);
			EncoderStream encoderStream(fileStream, encoder);
			StreamWriter writer(encoderStream);
			XmlPrint(xml, writer);
		}
		if (saveContainingProjects)
		{
			FOREACH(Ptr<ProjectItem>, project, From(projects).Cast<ProjectItem>())
			{
				project->SaveProject(true);
			}
		}
		return true;
	}

	bool SolutionItem::NewSolution()
	{
		projects.Clear();
		return true;
	}

	bool SolutionItem::AddProject(Ptr<IProjectModel> project)
	{
		if (projects.Contains(project.Obj())) return false;
		projects.Add(project);
		return true;
	}

	bool SolutionItem::RemoveProject(Ptr<IProjectModel> project)
	{
		auto index = projects.IndexOf(project.Obj());
		if (index == -1) return false;
		projects.RemoveAt(index);
		return true;
	}

	ISolutionItemModel* SolutionItem::GetParent()
	{
		return studioModel->GetRootSolutionItem().Obj();
	}

	Ptr<GuiImageData> SolutionItem::GetImage()
	{
		return projectFactory->GetSmallImage();
	}

	WString SolutionItem::GetName()
	{
		return GetDisplayNameFromFilePath(filePath, L".gacsln.xml");
	}

	Ptr<description::IValueObservableList> SolutionItem::GetChildren()
	{
		return projects.GetWrapper();
	}

	WString SolutionItem::GetFilePath()
	{
		return filePath;
	}

	WString SolutionItem::GetFileDirectory()
	{
		return FilePath(filePath).GetFolder().GetFullPath();
	}

	vint SolutionItem::GetErrorCount()
	{
		return errors.Count();
	}

	WString SolutionItem::GetErrorText(vint index)
	{
		return 0 <= index && index < errors.Count() ? errors[index] : L"";
	}
}

namespace vl
{
	namespace reflection
	{
		namespace description
		{
			#define _ ,
			IMPL_CPP_TYPE_INFO(vm::FileItem)
			IMPL_CPP_TYPE_INFO(vm::FolderItem)
			IMPL_CPP_TYPE_INFO(vm::ProjectItem)
			IMPL_CPP_TYPE_INFO(vm::SolutionItem)

			BEGIN_CLASS_MEMBER(vm::FileItem)
				CLASS_MEMBER_BASE(vm::IFileModel)
				CLASS_MEMBER_BASE(vm::IRenameItemAction)
				CLASS_MEMBER_BASE(vm::IRemoveItemAction)
			END_CLASS_MEMBER(vm::FileItem)

			BEGIN_CLASS_MEMBER(vm::FolderItem)
				CLASS_MEMBER_BASE(vm::IFolderModel)
				CLASS_MEMBER_BASE(vm::IAddFileItemAction)
				CLASS_MEMBER_BASE(vm::IRenameItemAction)
				CLASS_MEMBER_BASE(vm::IRemoveItemAction)
			END_CLASS_MEMBER(vm::FolderItem)

			BEGIN_CLASS_MEMBER(vm::ProjectItem)
				CLASS_MEMBER_BASE(vm::IProjectModel)
				CLASS_MEMBER_BASE(vm::IAddFileItemAction)
				CLASS_MEMBER_BASE(vm::IRenameItemAction)
				CLASS_MEMBER_BASE(vm::IRemoveItemAction)
			END_CLASS_MEMBER(vm::ProjectItem)

			BEGIN_CLASS_MEMBER(vm::SolutionItem)
				CLASS_MEMBER_BASE(vm::ISolutionModel)
			END_CLASS_MEMBER(vm::SolutionItem)

			#undef _

			class GacStudioSolutionModelLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					ADD_TYPE_INFO(vm::FileItem)
					ADD_TYPE_INFO(vm::FolderItem)
					ADD_TYPE_INFO(vm::ProjectItem)
					ADD_TYPE_INFO(vm::SolutionItem)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};

			class GacStudioSolutionModelPlugin : public Object, public vl::presentation::controls::IGuiPlugin
			{
			public:
				void Load()override
				{
					GetGlobalTypeManager()->AddTypeLoader(new GacStudioSolutionModelLoader);
				}

				void AfterLoad()override
				{
				}

				void Unload()override
				{
				}
			};
			GUI_REGISTER_PLUGIN(GacStudioSolutionModelPlugin)
		}
	}
}
