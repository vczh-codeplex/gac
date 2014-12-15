#include "SolutionModel.h"
#include "StudioModel.h"

using namespace vl::parsing::xml;
using namespace vl::filesystem;
using namespace vl::stream;
using namespace vl::reflection::description;
using namespace vl::presentation;

namespace vm
{
	WString GetDisplayNameFromFilePath(WString filePath)
	{
		auto index = INVLOC.FindLast(filePath, L"\\", Locale::None);
		if (index.key == -1)
		{
			return filePath;
		}
		else
		{
			return filePath.Sub(index.key + 1, filePath.Length() - index.key - 1);
		}
	}

/***********************************************************************
ProjectItem
***********************************************************************/

	ProjectItem::ProjectItem(Ptr<IProjectFactoryModel> _projectFactory, WString _filePath, bool _unsupported)
		:projectFactory(_projectFactory)
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
		auto solutionFolder = FilePath(filePath).GetFolder();
		auto xml = MakePtr<XmlDocument>();
		auto xmlSolution = MakePtr<XmlElement>();
		xmlSolution->name.value = L"GacStudioProject";
		xml->rootElement = xmlSolution;
		{
			XmlElementWriter xmlWriter(xmlSolution);
			xmlWriter.Attribute(L"Factory", projectFactory->GetId());
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
			List<ISolutionItemModel*> content;
			content.Add(this);
			vint start = 0;
			while (start < content.Count())
			{
				auto items = content[start]->GetChildren();
				for (vint i = 0; i < items->GetCount(); i++)
				{
					auto item = UnboxValue<ISolutionItemModel*>(items->Get(i));
					if (item->GetIsFileItem())
					{
						item->SaveFileItem();
					}
					content.Add(item);
				}
				start++;
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
		return false;
	}

	Ptr<GuiImageData> ProjectItem::GetImage()
	{
		return projectFactory->GetSmallImage();
	}

	WString ProjectItem::GetName()
	{
		return GetDisplayNameFromFilePath(filePath);
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

	SolutionItem::SolutionItem(Ptr<IProjectFactoryModel> _projectFactory, WString _filePath)
		:projectFactory(_projectFactory)
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
					auto factory=projectFactory
						->GetChildren()
						.Where([=](Ptr<IProjectFactoryModel> factory)
						{
							return factory->GetId() == factoryId;
						})
						.First();
					bool unsupported = false;
					if (!factory)
					{
						unsupported = true;
						factory = new ProjectFactoryModel(projectFactory->GetImage(), projectFactory->GetSmallImage(), factoryId);
						errors.Add(L"Unrecognizable project factory id \"" + factoryId + L"\".");
					}
					auto project = new ProjectItem(factory, (solutionFolder / projectPath).GetFullPath(), unsupported);
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

	bool SolutionItem::AddProject(Ptr<ProjectItem> project)
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

	Ptr<GuiImageData> SolutionItem::GetImage()
	{
		return projectFactory->GetSmallImage();
	}

	WString SolutionItem::GetName()
	{
		return GetDisplayNameFromFilePath(filePath);
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