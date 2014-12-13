/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GacStudio::SolutionModel

Interfaces:
***********************************************************************/

#ifndef GACSTUDIO_STUDIO_SOLUTIONMODEL
#define GACSTUDIO_STUDIO_SOLUTIONMODEL

#include "..\UI\GacStudioUI.h"

using namespace vl::collections;

namespace vm
{
	class ProjectItem : public Object, public virtual ISolutionItemModel
	{
	protected:
		list::ObservableList<Ptr<ISolutionItemModel>>	children;
		Ptr<IProjectFactoryModel>						projectFactory;
		WString											filePath;

	public:
		ProjectItem(Ptr<IProjectFactoryModel> _projectFactory, WString _filePath);
		~ProjectItem();
		
		Ptr<IProjectFactoryModel>						GetProjectFactory();
		bool											OpenProject();
		bool											SaveProject();
		bool											NewProject();

		Ptr<GuiImageData>								GetImage()override;
		WString											GetName()override;
		Ptr<description::IValueObservableList>			GetChildren()override;

		bool											GetIsFileItem()override;
		WString											GetFilePath()override;
		bool											GetIsSaved()override;
		bool											OpenFileItem()override;
		bool											SaveFileItem()override;
	};

	class SolutionItem : public Object, public virtual ISolutionItemModel
	{
	protected:
		list::ObservableList<Ptr<ISolutionItemModel>>			projects;
		Ptr<IProjectFactoryModel>						projectFactory;
		WString											filePath;
		bool											isSaved;

	public:
		SolutionItem(Ptr<IProjectFactoryModel> _projectFactory, WString _filePath);
		~SolutionItem();
		
		bool											OpenSolution();
		bool											SaveSolution();
		bool											NewSolution();

		Ptr<GuiImageData>								GetImage()override;
		WString											GetName()override;
		Ptr<description::IValueObservableList>			GetChildren()override;

		bool											GetIsFileItem()override;
		WString											GetFilePath()override;
		bool											GetIsSaved()override;
		bool											OpenFileItem()override;
		bool											SaveFileItem()override;
	};
}

#endif