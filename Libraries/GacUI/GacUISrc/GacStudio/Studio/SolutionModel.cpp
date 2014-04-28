#include "SolutionModel.h"

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
				L"Create a parser for your GacUI program.",
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
				L"Create a parser test for your GacUI program.",
				L"PARSER_TEST_FILE")
		{
		}
	};

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

		fileFactories.Add(new WindowResourceFileFactory);
		fileFactories.Add(new ControlResourceFileFactory);
		fileFactories.Add(new ImageFileFactory);
		fileFactories.Add(new TextFileFactory);
		fileFactories.Add(new RichTextFileFactory);
		fileFactories.Add(new XmlFileFactory);
		fileFactories.Add(new ParserFileFactory);
		fileFactories.Add(new ParserTestFileFactory);

		CopyFrom(filteredFileFactories, fileFactories);
	}

	StudioModel::~StudioModel()
	{
	}

	LazyList<Ptr<IProjectFactoryModel>> StudioModel::GetProjectModels()
	{
		return projectFactories;
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
}