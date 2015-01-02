﻿/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
GacUI::Partial Classes

This file is generated by: Vczh GacUI Resource Code Generator
************************************************************************
DO NOT MODIFY
***********************************************************************/

#include "GacStudioUI.h"

namespace vl
{
	namespace reflection
	{
		namespace description
		{
			#define _ ,
			IMPL_TYPE_INFO(vm::IFileFactoryModel)
			IMPL_TYPE_INFO(vm::IFileModel)
			IMPL_TYPE_INFO(vm::IFolderModel)
			IMPL_TYPE_INFO(vm::IProjectFactoryModel)
			IMPL_TYPE_INFO(vm::IProjectModel)
			IMPL_TYPE_INFO(vm::ISolutionItemModel)
			IMPL_TYPE_INFO(vm::ISolutionModel)
			IMPL_TYPE_INFO(vm::IStudioModel)
			IMPL_TYPE_INFO(ui::AboutWindow)
			IMPL_TYPE_INFO(ui::MainWindow)
			IMPL_TYPE_INFO(ui::NewFileWindow)
			IMPL_TYPE_INFO(ui::NewProjectWindow)

			BEGIN_CLASS_MEMBER(vm::IFileFactoryModel)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Image)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(SmallImage)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Name)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Category)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Description)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Id)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(DefaultFileExt)
				CLASS_MEMBER_METHOD(GenerateFile, { L"" _ L"filePath" });
			END_CLASS_MEMBER(vm::IFileFactoryModel)

			BEGIN_CLASS_MEMBER(vm::IFileModel)
				CLASS_MEMBER_BASE(vm::ISolutionItemModel)
				CLASS_MEMBER_METHOD(GetFileFactory, NO_PARAMETER);
				CLASS_MEMBER_METHOD(OpenFile, NO_PARAMETER);
				CLASS_MEMBER_METHOD(SaveFile, NO_PARAMETER);
				CLASS_MEMBER_METHOD(NewFile, NO_PARAMETER);
			END_CLASS_MEMBER(vm::IFileModel)

			BEGIN_CLASS_MEMBER(vm::IFolderModel)
				CLASS_MEMBER_BASE(vm::ISolutionItemModel)
			END_CLASS_MEMBER(vm::IFolderModel)

			BEGIN_CLASS_MEMBER(vm::IProjectFactoryModel)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Image)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(SmallImage)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Name)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Description)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Id)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Children)
			END_CLASS_MEMBER(vm::IProjectFactoryModel)

			BEGIN_CLASS_MEMBER(vm::IProjectModel)
				CLASS_MEMBER_BASE(vm::ISolutionItemModel)
				CLASS_MEMBER_METHOD(GetProjectFactory, NO_PARAMETER);
				CLASS_MEMBER_METHOD(OpenProject, NO_PARAMETER);
				CLASS_MEMBER_METHOD(SaveProject, { L"saveContainingFiles" });
				CLASS_MEMBER_METHOD(NewProject, NO_PARAMETER);
				CLASS_MEMBER_METHOD(AddFile, { L"file" });
			END_CLASS_MEMBER(vm::IProjectModel)

			BEGIN_CLASS_MEMBER(vm::ISolutionItemModel)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_EVENT(ImageChanged)
				CLASS_MEMBER_PROPERTY_EVENT_READONLY_FAST(Image, ImageChanged)
				CLASS_MEMBER_EVENT(NameChanged)
				CLASS_MEMBER_PROPERTY_EVENT_READONLY_FAST(Name, NameChanged)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Children)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(IsFileItem)
				CLASS_MEMBER_EVENT(FilePathChanged)
				CLASS_MEMBER_PROPERTY_EVENT_READONLY_FAST(FilePath, FilePathChanged)
				CLASS_MEMBER_EVENT(FileDirectoryChanged)
				CLASS_MEMBER_PROPERTY_EVENT_READONLY_FAST(FileDirectory, FileDirectoryChanged)
				CLASS_MEMBER_EVENT(IsSavedChanged)
				CLASS_MEMBER_PROPERTY_EVENT_READONLY_FAST(IsSaved, IsSavedChanged)
				CLASS_MEMBER_EVENT(ErrorCountChanged)
				CLASS_MEMBER_PROPERTY_EVENT_READONLY_FAST(ErrorCount, ErrorCountChanged)
				CLASS_MEMBER_METHOD(GetErrorText, { L"index" });
				CLASS_MEMBER_METHOD(OpenFileItem, NO_PARAMETER);
				CLASS_MEMBER_METHOD(SaveFileItem, NO_PARAMETER);
			END_CLASS_MEMBER(vm::ISolutionItemModel)

			BEGIN_CLASS_MEMBER(vm::ISolutionModel)
				CLASS_MEMBER_BASE(vm::ISolutionItemModel)
				CLASS_MEMBER_METHOD(OpenSolution, NO_PARAMETER);
				CLASS_MEMBER_METHOD(SaveSolution, { L"saveContainingProjects" });
				CLASS_MEMBER_METHOD(NewSolution, NO_PARAMETER);
				CLASS_MEMBER_METHOD(AddProject, { L"project" });
			END_CLASS_MEMBER(vm::ISolutionModel)

			BEGIN_CLASS_MEMBER(vm::IStudioModel)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ProjectModels)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(FileModels)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(FileFilters)
				CLASS_MEMBER_PROPERTY_FAST(FileCategory)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(RootSolutionItem)
				CLASS_MEMBER_EVENT(OpenedSolutionChanged)
				CLASS_MEMBER_PROPERTY_EVENT_READONLY_FAST(OpenedSolution, OpenedSolutionChanged)
				CLASS_MEMBER_METHOD(GetProjectFactory, { L"id" });
				CLASS_MEMBER_METHOD(GetFileFactory, { L"id" });
				CLASS_MEMBER_METHOD(OpenSolution, { L"filePath" });
				CLASS_MEMBER_METHOD(SaveSolution, NO_PARAMETER);
				CLASS_MEMBER_METHOD(NewSolution, { L"filePath" });
				CLASS_MEMBER_METHOD(CloseSolution, NO_PARAMETER);
				CLASS_MEMBER_METHOD(AddProject, { L"projectFactory" _ L"filePath" });
				CLASS_MEMBER_METHOD(AddFile, { L"project" _ L"fileFactory" _ L"filePath" });
				CLASS_MEMBER_METHOD(OpenBrowser, { L"url" });
				CLASS_MEMBER_METHOD(PromptError, { L"message" });
			END_CLASS_MEMBER(vm::IStudioModel)

			BEGIN_CLASS_MEMBER(ui::AboutWindow)
				CLASS_MEMBER_BASE(vl::presentation::controls::GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(ui::AboutWindow*(Ptr<vm::IStudioModel>), { L"ViewModel" })

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ViewModel)
			END_CLASS_MEMBER(ui::AboutWindow)

			BEGIN_CLASS_MEMBER(ui::MainWindow)
				CLASS_MEMBER_BASE(vl::presentation::controls::GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(ui::MainWindow*(Ptr<vm::IStudioModel>), { L"ViewModel" })

				CLASS_MEMBER_GUIEVENT_HANDLER(cellSplitter_leftButtonDown, vl::presentation::compositions::GuiMouseEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(cellSplitter_leftButtonUp, vl::presentation::compositions::GuiMouseEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(cellSplitter_mouseMove, vl::presentation::compositions::GuiMouseEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileExit_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileNewFile_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileNewProject_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileOpenFile_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileOpenProject_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileSaveAll_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileSave_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandHelpAbout_Executed, vl::presentation::compositions::GuiEventArgs)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ViewModel)
			END_CLASS_MEMBER(ui::MainWindow)

			BEGIN_CLASS_MEMBER(ui::NewFileWindow)
				CLASS_MEMBER_BASE(vl::presentation::controls::GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(ui::NewFileWindow*(Ptr<vm::IStudioModel>), { L"ViewModel" })

				CLASS_MEMBER_GUIEVENT_HANDLER(buttonCancel_Clicked, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(buttonCreate_Clicked, vl::presentation::compositions::GuiEventArgs)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ViewModel)
			END_CLASS_MEMBER(ui::NewFileWindow)

			BEGIN_CLASS_MEMBER(ui::NewProjectWindow)
				CLASS_MEMBER_BASE(vl::presentation::controls::GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(ui::NewProjectWindow*(Ptr<vm::IStudioModel>), { L"ViewModel" })

				CLASS_MEMBER_GUIEVENT_HANDLER(buttonBrowse_Clicked, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(buttonCancel_Clicked, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(buttonCreate_Clicked, vl::presentation::compositions::GuiEventArgs)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ViewModel)
			END_CLASS_MEMBER(ui::NewProjectWindow)

			#undef _

			class GacStudioUIResourceLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					ADD_TYPE_INFO(vm::IFileFactoryModel)
					ADD_TYPE_INFO(vm::IFileModel)
					ADD_TYPE_INFO(vm::IFolderModel)
					ADD_TYPE_INFO(vm::IProjectFactoryModel)
					ADD_TYPE_INFO(vm::IProjectModel)
					ADD_TYPE_INFO(vm::ISolutionItemModel)
					ADD_TYPE_INFO(vm::ISolutionModel)
					ADD_TYPE_INFO(vm::IStudioModel)
					ADD_TYPE_INFO(ui::AboutWindow)
					ADD_TYPE_INFO(ui::MainWindow)
					ADD_TYPE_INFO(ui::NewFileWindow)
					ADD_TYPE_INFO(ui::NewProjectWindow)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};

			class GacStudioUIResourcePlugin : public Object, public vl::presentation::controls::IGuiPlugin
			{
			public:
				void Load()override
				{
					GetGlobalTypeManager()->AddTypeLoader(new GacStudioUIResourceLoader);
				}

				void AfterLoad()override
				{
				}

				void Unload()override
				{
				}
			};
			GUI_REGISTER_PLUGIN(GacStudioUIResourcePlugin)
		}
	}
}

