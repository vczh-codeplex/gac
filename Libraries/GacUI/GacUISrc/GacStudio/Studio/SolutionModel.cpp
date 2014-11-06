#include "SolutionModel.h"
#include <Windows.h>

using namespace vl::reflection::description;

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
WindowResourceFileFactory
***********************************************************************/

	class WindowResourceFileFactory : public FileFactoryModelBase
	{
	public:
		WindowResourceFileFactory()
			:FileFactoryModelBase(
				L"FileImages/Dialog.png",
				L"Window Resource",
				L"GACUI_RESOURCE_PROJECT",
				L"Create a window for your GacUI program.",
				L"GACUI_WINDOW_RESOURCE_FILE")
		{
		}
	};

/***********************************************************************
ControlResourceFileFactory
***********************************************************************/

	class ControlResourceFileFactory : public FileFactoryModelBase
	{
	public:
		ControlResourceFileFactory()
			:FileFactoryModelBase(
				L"FileImages/Control.png",
				L"Control Resource",
				L"GACUI_RESOURCE_PROJECT",
				L"Create a control for your GacUI program.",
				L"GACUI_CONTROL_RESOURCE_FILE")
		{
		}
	};

/***********************************************************************
TemplateResourceFileFactory
***********************************************************************/

	class TemplateResourceFileFactory : public FileFactoryModelBase
	{
	public:
		TemplateResourceFileFactory()
			:FileFactoryModelBase(
				L"FileImages/Template.png",
				L"Control Template",
				L"GACUI_RESOURCE_PROJECT",
				L"Create a control template for your GacUI program.",
				L"GACUI_TEMPLATE_RESOURCE_FILE")
		{
		}
	};

/***********************************************************************
ImageFileFactory
***********************************************************************/

	class ImageFileFactory : public FileFactoryModelBase
	{
	public:
		ImageFileFactory()
			:FileFactoryModelBase(
				L"FileImages/Image.png",
				L"Image Resource",
				L"GACUI_RESOURCE_PROJECT",
				L"Create an image resource for your GacUI program.",
				L"GACUI_IMAGE_RESOURCE_FILE")
		{
		}
	};

/***********************************************************************
TextFileFactory
***********************************************************************/

	class TextFileFactory : public FileFactoryModelBase
	{
	public:
		TextFileFactory()
			:FileFactoryModelBase(
				L"FileImages/Document.png",
				L"Text Resource",
				L"GACUI_RESOURCE_PROJECT",
				L"Create a text resource for your GacUI program.",
				L"GACUI_TEXT_RESOURCE_FILE")
		{
		}
	};

/***********************************************************************
RichTextFileFactory
***********************************************************************/

	class RichTextFileFactory : public FileFactoryModelBase
	{
	public:
		RichTextFileFactory()
			:FileFactoryModelBase(
				L"FileImages/RichDocument.png",
				L"Rich Text Resource",
				L"GACUI_RESOURCE_PROJECT",
				L"Create a rich text resource for your GacUI program.",
				L"GACUI_RICHTEXT_RESOURCE_FILE")
		{
		}
	};

/***********************************************************************
XmlFileFactory
***********************************************************************/

	class XmlFileFactory : public FileFactoryModelBase
	{
	public:
		XmlFileFactory()
			:FileFactoryModelBase(
				L"FileImages/XmlDocument.png",
				L"XML Resource",
				L"GACUI_RESOURCE_PROJECT",
				L"Create an xml resource for your GacUI program.",
				L"GACUI_XML_RESOURCE_FILE")
		{
		}
	};

/***********************************************************************
ParserFileFactory
***********************************************************************/

	class ParserFileFactory : public FileFactoryModelBase
	{
	public:
		ParserFileFactory()
			:FileFactoryModelBase(
				L"FileImages/Parser.png",
				L"Parser",
				L"PARSER_PROJECT",
				L"Create a parser from a grammar.",
				L"PARSER_FILE")
		{
		}
	};

/***********************************************************************
ParserTestFileFactory
***********************************************************************/

	class ParserTestFileFactory : public FileFactoryModelBase
	{
	public:
		ParserTestFileFactory()
			:FileFactoryModelBase(
				L"FileImages/Parser.png",
				L"Parser Test",
				L"PARSER_PROJECT",
				L"Create a parser test.",
				L"PARSER_TEST_FILE")
		{
		}
	};

/***********************************************************************
SqlDatabaseFileFactory
***********************************************************************/

	class SqlDatabaseFileFactory : public FileFactoryModelBase
	{
	public:
		SqlDatabaseFileFactory()
			:FileFactoryModelBase(
				L"FileImages/SQLDatabase.png",
				L"SQL Database",
				L"HERODB_PROJECT",
				L"Create a SQL Database in a HeroDB database instance.",
				L"HERODB_SQL_DATABASE")
		{
		}
	};

/***********************************************************************
GraphDatabaseFileFactory
***********************************************************************/

	class GraphDatabaseFileFactory : public FileFactoryModelBase
	{
	public:
		GraphDatabaseFileFactory()
			:FileFactoryModelBase(
				L"FileImages/GraphDatabase.png",
				L"Graph Database",
				L"HERODB_PROJECT",
				L"Create a Graph Database in a HeroDB database instance.",
				L"HERODB_GRAPH DATABASE")
		{
		}
	};

/***********************************************************************
ProjectFactoryModelBase
***********************************************************************/
	
	ProjectFactoryModelBase::ProjectFactoryModelBase(WString _imageUrl, WString _smallImageUrl, WString _name, WString _description, WString _id)
		:name(_name)
		, description(_description)
		, id(_id)
	{
		image = GetInstanceLoaderManager()->GetResource(L"GacStudioUI")->GetImageByPath(_imageUrl);
		smallImage = GetInstanceLoaderManager()->GetResource(L"GacStudioUI")->GetImageByPath(_smallImageUrl);
	}

	ProjectFactoryModelBase::~ProjectFactoryModelBase()
	{
	}

	Ptr<GuiImageData> ProjectFactoryModelBase::GetImage()
	{
		return image;
	}

	Ptr<GuiImageData> ProjectFactoryModelBase::GetFilterImage()
	{
		return smallImage;
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

	LazyList<Ptr<IFileFactoryFilterModel>> ProjectFactoryModelBase::GetChildren()
	{
		return MakePtr<List<Ptr<IFileFactoryFilterModel>>>();
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
				L"ProjectImages/DialogSmall.png",
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
				L"ProjectImages/ParserSmall.png",
				L"Parser",
				L"Create a parser.",
				L"PARSER_PROJECT")
		{
		}
	};

/***********************************************************************
DatabaseProjectFactory
***********************************************************************/

	class DatabaseProjectFactory : public ProjectFactoryModelBase
	{
	public:
		DatabaseProjectFactory()
			:ProjectFactoryModelBase(
				L"ProjectImages/Database.png",
				L"ProjectImages/DatabaseSmall.png",
				L"HeroDB Database",
				L"Create a HeroDB database instance.",
				L"HERODB_PROJECT")
		{
		}
	};

/***********************************************************************
AllFileFactoryFilterModel
***********************************************************************/
	
	AllFileFactoryFilterModel::AllFileFactoryFilterModel()
	{
		smallImage = GetInstanceLoaderManager()->GetResource(L"GacStudioUI")->GetImageByPath(L"ProjectImages/AllSmall.png");
		projectFactories.Add(new ResourceProjectFactory);
		projectFactories.Add(new ParserProjectFactory);
		projectFactories.Add(new DatabaseProjectFactory);
	}

	AllFileFactoryFilterModel::~AllFileFactoryFilterModel()
	{
	}

	Ptr<GuiImageData> AllFileFactoryFilterModel::GetFilterImage()
	{
		return smallImage;
	}

	WString AllFileFactoryFilterModel::GetName()
	{
		return L"All";
	}

	WString AllFileFactoryFilterModel::GetId()
	{
		return L"";
	}

	LazyList<Ptr<IFileFactoryFilterModel>> AllFileFactoryFilterModel::GetChildren()
	{
		return From(projectFactories).Cast<IFileFactoryFilterModel>();
	}

/***********************************************************************
StudioModel
***********************************************************************/

	StudioModel::StudioModel()
	{
		allProjects = new AllFileFactoryFilterModel;

		fileFactories.Add(new WindowResourceFileFactory);
		fileFactories.Add(new ControlResourceFileFactory);
		fileFactories.Add(new TemplateResourceFileFactory);
		fileFactories.Add(new ImageFileFactory);
		fileFactories.Add(new TextFileFactory);
		fileFactories.Add(new RichTextFileFactory);
		fileFactories.Add(new XmlFileFactory);
		fileFactories.Add(new ParserFileFactory);
		fileFactories.Add(new ParserTestFileFactory);
		fileFactories.Add(new SqlDatabaseFileFactory);
		fileFactories.Add(new GraphDatabaseFileFactory);

		CopyFrom(filteredFileFactories, fileFactories);
	}

	StudioModel::~StudioModel()
	{
	}

	LazyList<Ptr<IProjectFactoryModel>> StudioModel::GetProjectModels()
	{
		return allProjects->GetChildren().Cast<IProjectFactoryModel>();
	}

	Ptr<IValueObservableList> StudioModel::GetFileModels()
	{
		return filteredFileFactories.GetWrapper();
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

	LazyList<Ptr<IFileFactoryFilterModel>> StudioModel::GetFileFilters()
	{
		auto list = MakePtr<List<Ptr<IFileFactoryFilterModel>>>();
		list->Add(allProjects);
		return list;
	}

	Ptr<description::IValueObservableList> StudioModel::GetOpeningSolution()
	{
		return openingSolution.GetWrapper();
	}

	void StudioModel::OpenBrowser(WString url)
	{
		ShellExecute(NULL, NULL, url.Buffer(), NULL, NULL, SHOW_FULLSCREEN);
	}
}