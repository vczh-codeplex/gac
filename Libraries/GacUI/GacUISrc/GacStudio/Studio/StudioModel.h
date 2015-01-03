/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacStudio::StudioModel

Interfaces:
***********************************************************************/

#ifndef GACSTUDIO_STUDIO_STUDIOMODEL
#define GACSTUDIO_STUDIO_STUDIOMODEL

#include "..\UI\GacStudioUI.h"

using namespace vl::collections;

namespace vm
{
	class TextTemplateItem : public Object
	{
	public:
		bool											isMacro;
		WString											content;
	};

	class TextTemplate : public Object, public virtual ITextTemplate
	{
	protected:
		collections::List<Ptr<TextTemplateItem>>		items;

	public:
		TextTemplate(const WString& content);
		~TextTemplate();

		WString											Generate(Ptr<IMacroEnvironment> macroEnvironment)override;
	};

	class FileFactoryModel : public Object, public virtual IFileFactoryModel
	{
	protected:
		Ptr<GuiImageData>								image;
		Ptr<GuiImageData>								smallImage;
		WString											name;
		WString											category;
		WString											description;
		WString											id;
		WString											ext;
		Ptr<ITextTemplate>								textTemplate;

	public:
		FileFactoryModel(WString _imageUrl, WString _smallImageUrl, WString _name, WString _category, WString _description, WString _id, WString _ext, Ptr<ITextTemplate> _textTemplate);
		FileFactoryModel(Ptr<GuiImageData> _image, Ptr<GuiImageData> _smallImage, WString _id);
		~FileFactoryModel();

		Ptr<GuiImageData>								GetImage()override;
		Ptr<GuiImageData>								GetSmallImage()override;
		WString											GetName()override;
		WString											GetCategory()override;
		WString											GetDescription()override;
		WString											GetId()override;
		WString											GetDefaultFileExt()override;
		Ptr<ITextTemplate>								GetTextTemplate()override;
	};

	class ProjectFactoryModel : public Object, public virtual IProjectFactoryModel
	{
	protected:
		Ptr<GuiImageData>								image;
		Ptr<GuiImageData>								smallImage;
		WString											name;
		WString											description;
		WString											id;

	public:
		ProjectFactoryModel(WString _imageUrl, WString _smallImageUrl, WString _name, WString _description, WString _id);
		ProjectFactoryModel(Ptr<GuiImageData> _image, Ptr<GuiImageData> _smallImage, WString _id);
		~ProjectFactoryModel();

		Ptr<GuiImageData>								GetImage()override;
		Ptr<GuiImageData>								GetSmallImage()override;
		WString											GetName()override;
		WString											GetDescription()override;
		WString											GetId()override;
		LazyList<Ptr<IProjectFactoryModel>>				GetChildren()override;
	};

	class FileFactoryFilterModel : public ProjectFactoryModel
	{
	protected:
		List<Ptr<ProjectFactoryModel>>					children;

	public:
		FileFactoryFilterModel();
		~FileFactoryFilterModel();

		void											AddChild(Ptr<ProjectFactoryModel> child);
		LazyList<Ptr<IProjectFactoryModel>>				GetChildren()override;
	};

	class RootSolutionItemModel : public Object, public virtual ISolutionItemModel
	{
	protected:
		list::ObservableList<Ptr<ISolutionItemModel>>	children;

	public:
		ISolutionItemModel*								GetParent()override;
		Ptr<GuiImageData>								GetImage()override;
		WString											GetName()override;
		Ptr<description::IValueObservableList>			GetChildren()override;

		bool											GetIsFileItem()override;
		WString											GetFilePath()override;
		WString											GetFileDirectory()override;
		bool											GetIsSaved()override;
		vint											GetErrorCount()override;
		WString											GetErrorText(vint index)override;
		bool											OpenFileItem()override;
		bool											SaveFileItem()override;
		
		Ptr<ISolutionModel>								GetSolution();
		void											SetSolution(Ptr<ISolutionModel> solution);
	};

	class StudioModel : public Object, public virtual IStudioModel
	{
	protected:
		Ptr<FileFactoryFilterModel>						solutionProjectFactory;
		Ptr<FileFactoryFilterModel>						fileFilters;
		List<Ptr<IFileFactoryModel>>					fileFactories;
		WString											fileCategory;
		list::ObservableList<Ptr<IFileFactoryModel>>	filteredFileFactories;
		Ptr<RootSolutionItemModel>						rootSolutionItem;
		Ptr<ISolutionItemModel>							selectedSolutionItem;

	public:
		StudioModel();
		~StudioModel();

		LazyList<Ptr<IProjectFactoryModel>>				GetProjectModels()override;
		Ptr<description::IValueObservableList>			GetFileModels()override;
		Ptr<IProjectFactoryModel>						GetFileFilters()override;

		WString											GetFileCategory()override;
		void											SetFileCategory(WString value)override;

		Ptr<ISolutionItemModel>							GetRootSolutionItem()override;
		Ptr<ISolutionModel>								GetOpenedSolution()override;
		void											NotifySelectedSolutionItem(Ptr<ISolutionItemModel> selectedItem)override;
		Ptr<ISolutionItemModel>							GetWorkingItem()override;
		Ptr<IProjectModel>								GetWorkingProject()override;
		WString											GetWorkingDirectory()override;

		Ptr<IProjectFactoryModel>						GetProjectFactory(WString id)override;
		Ptr<IFileFactoryModel>							GetFileFactory(WString id)override;

		bool											OpenSolution(WString filePath)override;
		bool											SaveSolution()override;
		bool											NewSolution(WString filePath)override;
		bool											CloseSolution()override;
		Ptr<IProjectModel>								AddProject(Ptr<IProjectFactoryModel> projectFactory, WString filePath)override;
		Ptr<IFileModel>									AddFile(Ptr<IProjectModel> project, Ptr<IFileFactoryModel> fileFactory, WString filePath)override;

		void											OpenBrowser(WString url)override;
		void											PromptError(WString message)override;
	};
}

#endif