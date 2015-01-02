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

/***********************************************************************
FileItem
***********************************************************************/

	FileItem::FileItem(IStudioModel* _studioModel, Ptr<IFileFactoryModel> _fileFactory, WString _filePath, bool _unsupported)
		:studioModel(_studioModel)
		, fileFactory(_fileFactory)
		, filePath(_filePath)
		, isSaved(false)
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
		throw 0;
	}

	bool FileItem::NewFile()
	{
		throw 0;
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
		return GetDisplayNameFromFilePath(filePath, L"");
	}

	Ptr<description::IValueObservableList> FileItem::GetChildren()
	{
		return children.GetWrapper();
	}

	bool FileItem::GetIsFileItem()
	{
		return true;
	}

	WString FileItem::GetFilePath()
	{
		return filePath;
	}

	WString FileItem::GetFileDirectory()
	{
		return FilePath(filePath).GetFolder().GetFullPath();
	}

	bool FileItem::GetIsSaved()
	{
		if (unsupported) return true;
		throw 0;
	}

	vint FileItem::GetErrorCount()
	{
		return errors.Count();
	}

	WString FileItem::GetErrorText(vint index)
	{
		return 0 <= index && index < errors.Count() ? errors[index] : L"";
	}

	bool FileItem::OpenFileItem()
	{
		if (unsupported) return true;
		throw 0;
	}

	bool FileItem::SaveFileItem()
	{
		if (unsupported) return true;
		throw 0;
	}

/***********************************************************************
FolderItem
***********************************************************************/

	FolderItem::FolderItem(ISolutionItemModel* _parent, WString _filePath)
		:filePath(_filePath)
		, parent(_parent)
	{
		image = GetInstanceLoaderManager()->GetResource(L"GacStudioUI")->GetImageByPath(L"FileImages/FolderSmall.png");
	}

	FolderItem::~FolderItem()
	{
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

	bool FolderItem::GetIsFileItem()
	{
		return false;
	}

	WString FolderItem::GetFilePath()
	{
		return filePath;
	}

	WString FolderItem::GetFileDirectory()
	{
		return filePath;
	}

	bool FolderItem::GetIsSaved()
	{
		return false;
	}

	vint FolderItem::GetErrorCount()
	{
		return 0;
	}

	WString FolderItem::GetErrorText(vint index)
	{
		return L"";
	}

	bool FolderItem::OpenFileItem()
	{
		return false;
	}

	bool FolderItem::SaveFileItem()
	{
		return false;
	}

/***********************************************************************
ProjectItem
***********************************************************************/

	void ProjectItem::AddFileItem(Ptr<IFileModel> fileItem)
	{
	}

	ProjectItem::ProjectItem(IStudioModel* _studioModel, Ptr<IProjectFactoryModel> _projectFactory, WString _filePath, bool _unsupported)
		:studioModel(_studioModel)
		, projectFactory(_projectFactory)
		, filePath(_filePath)
		, isSaved(false)
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
			children.Clear();
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
			
			fileItems.Clear();
			children.Clear();
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

		if (!isSaved)
		{
			isSaved = true;
			IsSavedChanged();
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
				fileItem->SaveFileItem();
			}
		}
		if (!isSaved)
		{
			isSaved = true;
			IsSavedChanged();
		}
		return true;
	}

	bool ProjectItem::NewProject()
	{
		if (unsupported) return false;
		children.Clear();
		if (isSaved)
		{
			isSaved = false;
			IsSavedChanged();
		}
		return true;
	}

	bool ProjectItem::AddFile(Ptr<IFileModel> file)
	{
		if (fileItems.Contains(file.Obj())) return false;
		AddFileItem(file);
		if (!isSaved)
		{
			isSaved = false;
			IsSavedChanged();
		}
		return true;
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

	bool ProjectItem::GetIsFileItem()
	{
		return !unsupported;
	}

	WString ProjectItem::GetFilePath()
	{
		return filePath;
	}

	WString ProjectItem::GetFileDirectory()
	{
		return FilePath(filePath).GetFolder().GetFullPath();
	}

	bool ProjectItem::GetIsSaved()
	{
		if (unsupported) return true;
		throw 0;
	}

	vint ProjectItem::GetErrorCount()
	{
		return errors.Count();
	}

	WString ProjectItem::GetErrorText(vint index)
	{
		return 0 <= index && index < errors.Count() ? errors[index] : L"";
	}

	bool ProjectItem::OpenFileItem()
	{
		return OpenProject();
	}

	bool ProjectItem::SaveFileItem()
	{
		return SaveProject(false);
	}

/***********************************************************************
SolutionItem
***********************************************************************/

	SolutionItem::SolutionItem(IStudioModel* _studioModel, Ptr<IProjectFactoryModel> _projectFactory, WString _filePath)
		:studioModel(_studioModel)
		, projectFactory(_projectFactory)
		, filePath(_filePath)
		, isSaved(false)
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
				}
			}
		}

		if (!isSaved)
		{
			isSaved = true;
			IsSavedChanged();
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
		if (!isSaved)
		{
			isSaved = true;
			IsSavedChanged();
		}
		return true;
	}

	bool SolutionItem::NewSolution()
	{
		projects.Clear();
		if (isSaved)
		{
			isSaved = false;
			IsSavedChanged();
		}
		return true;
	}

	bool SolutionItem::AddProject(Ptr<IProjectModel> project)
	{
		if (projects.Contains(project.Obj())) return false;
		projects.Add(project);
		if (!isSaved)
		{
			isSaved = false;
			IsSavedChanged();
		}
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

	bool SolutionItem::GetIsFileItem()
	{
		return true;
	}

	WString SolutionItem::GetFilePath()
	{
		return filePath;
	}

	WString SolutionItem::GetFileDirectory()
	{
		return FilePath(filePath).GetFolder().GetFullPath();
	}

	bool SolutionItem::GetIsSaved()
	{
		return isSaved;
	}

	vint SolutionItem::GetErrorCount()
	{
		return errors.Count();
	}

	WString SolutionItem::GetErrorText(vint index)
	{
		return 0 <= index && index < errors.Count() ? errors[index] : L"";
	}

	bool SolutionItem::OpenFileItem()
	{
		return OpenSolution();
	}

	bool SolutionItem::SaveFileItem()
	{
		return SaveSolution(false);
	}
}