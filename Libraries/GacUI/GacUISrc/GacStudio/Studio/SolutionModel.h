/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacStudio::SolutionModel

Interfaces:
***********************************************************************/

#ifndef GACSTUDIO_STUDIO_SOLUTIONMODEL
#define GACSTUDIO_STUDIO_SOLUTIONMODEL

#include "..\UI\GacStudioUI.h"

using namespace vl::collections;

namespace vm
{
	class ProjectItem : public Object, public virtual IProjectModel
	{
	protected:
		list::ObservableList<Ptr<ISolutionItemModel>>	children;
		Ptr<IProjectFactoryModel>						projectFactory;
		WString											filePath;
		bool											isSaved;
		collections::List<WString>						errors;
		bool											unsupported;

	public:
		ProjectItem(Ptr<IProjectFactoryModel> _projectFactory, WString _filePath, bool _unsupported = false);
		~ProjectItem();
		
		Ptr<IProjectFactoryModel>						GetProjectFactory()override;
		bool											OpenProject()override;
		bool											SaveProject(bool saveContainingFiles)override;
		bool											NewProject()override;

		Ptr<GuiImageData>								GetImage()override;
		WString											GetName()override;
		Ptr<description::IValueObservableList>			GetChildren()override;

		bool											GetIsFileItem()override;
		WString											GetFilePath()override;
		bool											GetIsSaved()override;
		vint											GetErrorCount()override;
		WString											GetErrorText(vint index)override;
		bool											OpenFileItem()override;
		bool											SaveFileItem()override;
	};

	class SolutionItem : public Object, public virtual ISolutionModel
	{
	protected:
		list::ObservableList<Ptr<IProjectModel>>		projects;
		Ptr<IProjectFactoryModel>						projectFactory;
		WString											filePath;
		bool											isSaved;
		collections::List<WString>						errors;

	public:
		SolutionItem(Ptr<IProjectFactoryModel> _projectFactory, WString _filePath);
		~SolutionItem();
		
		bool											OpenSolution()override;
		bool											SaveSolution(bool saveContainingProjects)override;
		bool											NewSolution()override;
		bool											AddProject(Ptr<IProjectModel> project)override;

		Ptr<GuiImageData>								GetImage()override;
		WString											GetName()override;
		Ptr<description::IValueObservableList>			GetChildren()override;

		bool											GetIsFileItem()override;
		WString											GetFilePath()override;
		bool											GetIsSaved()override;
		vint											GetErrorCount()override;
		WString											GetErrorText(vint index)override;
		bool											OpenFileItem()override;
		bool											SaveFileItem()override;
	};
}

#endif