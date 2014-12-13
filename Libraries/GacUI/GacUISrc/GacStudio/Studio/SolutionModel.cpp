#include "SolutionModel.h"

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

	ProjectItem::ProjectItem(Ptr<IProjectFactoryModel> _projectFactory, WString _filePath)
		:projectFactory(_projectFactory)
		, filePath(_filePath)
	{
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
		return false;
	}

	bool ProjectItem::SaveProject()
	{
		return false;
	}

	bool ProjectItem::NewProject()
	{
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
		return true;
	}

	WString ProjectItem::GetFilePath()
	{
		return filePath;
	}

	bool ProjectItem::GetIsSaved()
	{
		throw 0;
	}

	bool ProjectItem::OpenFileItem()
	{
		return OpenProject();
	}

	bool ProjectItem::SaveFileItem()
	{
		return SaveProject();
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
		auto solutionFolder = FilePath(filePath).GetFolder();
		Ptr<XmlDocument> xml;
		{
			FileStream fileStream(filePath, FileStream::ReadOnly);
			BomDecoder decoder;
			DecoderStream decoderStream(fileStream, decoder);
			StreamReader reader(decoderStream);

			auto parser = GetParserManager()->GetParser<XmlDocument>(L"XML");
			if (!parser) return false;

			List<WString> errors;
			xml = parser->TypedParse(reader.ReadToEnd(), errors);
			if (!xml || errors.Count() > 0) return false;
		}

		projects.Clear();
		FOREACH(Ptr<XmlElement>, xmlProject, XmlGetElements(xml->rootElement, L"GacStudioProject"))
		{
			auto factoryId = XmlGetAttribute(xmlProject, L"Factory")->value.value;
			auto filePath = XmlGetAttribute(xmlProject, L"FilePath")->value.value;
			auto factory=projectFactory
				->GetChildren()
				.Where([=](Ptr<IProjectFactoryModel> factory)
				{
					return factory->GetId() == factoryId;
				})
				.First();
			if (factory)
			{
				auto project = new ProjectItem(factory, (solutionFolder / filePath).GetFullPath());
				projects.Add(project);
			}
		}

		if (!isSaved)
		{
			isSaved = true;
			IsSavedChanged();
		}
		return true;
	}

	bool SolutionItem::SaveSolution()
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
			if (!fileStream.IsAvailable()) return false;
			BomEncoder encoder(BomEncoder::Utf16);
			EncoderStream encoderStream(fileStream, encoder);
			StreamWriter writer(encoderStream);
			XmlPrint(xml, writer);
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

	bool SolutionItem::OpenFileItem()
	{
		return OpenSolution();
	}

	bool SolutionItem::SaveFileItem()
	{
		return SaveSolution();
	}
}