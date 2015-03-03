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
			IMPL_CPP_TYPE_INFO(vm::IAddFileItemAction)
			IMPL_CPP_TYPE_INFO(vm::IEditorContentFactoryModel)
			IMPL_CPP_TYPE_INFO(vm::IEditorContentModel)
			IMPL_CPP_TYPE_INFO(vm::IEditorFactoryModel)
			IMPL_CPP_TYPE_INFO(vm::IEditorFileContentModel)
			IMPL_CPP_TYPE_INFO(vm::IEditorModel)
			IMPL_CPP_TYPE_INFO(vm::IFileFactoryModel)
			IMPL_CPP_TYPE_INFO(vm::IFileModel)
			IMPL_CPP_TYPE_INFO(vm::IFolderModel)
			IMPL_CPP_TYPE_INFO(vm::IMacroEnvironment)
			IMPL_CPP_TYPE_INFO(vm::IOpenInEditorItemAction)
			IMPL_CPP_TYPE_INFO(vm::IProjectFactoryModel)
			IMPL_CPP_TYPE_INFO(vm::IProjectModel)
			IMPL_CPP_TYPE_INFO(vm::IRemoveItemAction)
			IMPL_CPP_TYPE_INFO(vm::IRenameItemAction)
			IMPL_CPP_TYPE_INFO(vm::ISaveItemAction)
			IMPL_CPP_TYPE_INFO(vm::ISolutionItemModel)
			IMPL_CPP_TYPE_INFO(vm::ISolutionModel)
			IMPL_CPP_TYPE_INFO(vm::IStudioAddExistingFilesModel)
			IMPL_CPP_TYPE_INFO(vm::IStudioModel)
			IMPL_CPP_TYPE_INFO(vm::IStudioNewFileModel)
			IMPL_CPP_TYPE_INFO(vm::ITextTemplate)
			IMPL_CPP_TYPE_INFO(vm::StudioFileReference)
			IMPL_CPP_TYPE_INFO(ui::AboutWindow)
			IMPL_CPP_TYPE_INFO(ui::AddExistingFilesWindow)
			IMPL_CPP_TYPE_INFO(ui::FileReferenceFactoryColumnTemplate)
			IMPL_CPP_TYPE_INFO(ui::FileReferenceFactoryEditorTemplate)
			IMPL_CPP_TYPE_INFO(ui::FileReferenceFolderColumnTemplate)
			IMPL_CPP_TYPE_INFO(ui::FileReferenceNameColumnTemplate)
			IMPL_CPP_TYPE_INFO(ui::MainWindow)
			IMPL_CPP_TYPE_INFO(ui::NewFileWindow)
			IMPL_CPP_TYPE_INFO(ui::NewProjectWindow)
			IMPL_CPP_TYPE_INFO(ui::RenameFileWindow)

			BEGIN_CLASS_MEMBER(vm::IAddFileItemAction)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_METHOD(HasFile, { L"fileName" });
				CLASS_MEMBER_METHOD(AddFile, { L"file" });
			END_CLASS_MEMBER(vm::IAddFileItemAction)

			BEGIN_CLASS_MEMBER(vm::IEditorContentFactoryModel)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Name)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Id)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(BaseContentFactory)
				CLASS_MEMBER_METHOD(CreateContent, NO_PARAMETER);
			END_CLASS_MEMBER(vm::IEditorContentFactoryModel)

			BEGIN_CLASS_MEMBER(vm::IEditorContentModel)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ContentFactory)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(BaseContent)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(SubContent)
				CLASS_MEMBER_EVENT(PersistedContentChanged)
				CLASS_MEMBER_PROPERTY_EVENT_READONLY_FAST(PersistedContent, PersistedContentChanged)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(EditingContent)
				CLASS_MEMBER_EVENT(CurrentEditorChanged)
				CLASS_MEMBER_PROPERTY_EVENT_READONLY_FAST(CurrentEditor, CurrentEditorChanged)
				CLASS_MEMBER_METHOD(Persist, { L"content" });
				CLASS_MEMBER_METHOD(BeginEdit, { L"editor" });
				CLASS_MEMBER_METHOD(EndEdit, NO_PARAMETER);
			END_CLASS_MEMBER(vm::IEditorContentModel)

			BEGIN_CLASS_MEMBER(vm::IEditorFactoryModel)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Name)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Id)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(RequiredContentFactory)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(EditingContentFactory)
				CLASS_MEMBER_METHOD(CreateEditor, NO_PARAMETER);
			END_CLASS_MEMBER(vm::IEditorFactoryModel)

			BEGIN_CLASS_MEMBER(vm::IEditorFileContentModel)
				CLASS_MEMBER_BASE(vm::IEditorContentModel)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(FileName)
				CLASS_MEMBER_METHOD(LoadFile, { L"fileName" });
				CLASS_MEMBER_METHOD(RenameFile, { L"fileName" });
			END_CLASS_MEMBER(vm::IEditorFileContentModel)

			BEGIN_CLASS_MEMBER(vm::IEditorModel)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(EditorFactory)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(EditorAction)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(EditorControl)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(EditingContent)
				CLASS_MEMBER_METHOD(Open, { L"content" });
				CLASS_MEMBER_METHOD(Save, NO_PARAMETER);
				CLASS_MEMBER_METHOD(Close, NO_PARAMETER);
			END_CLASS_MEMBER(vm::IEditorModel)

			BEGIN_CLASS_MEMBER(vm::IFileFactoryModel)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Image)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(SmallImage)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Name)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Category)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Description)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Id)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(DefaultFileExt)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(TextTemplate)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ContentFactory)
			END_CLASS_MEMBER(vm::IFileFactoryModel)

			BEGIN_CLASS_MEMBER(vm::IFileModel)
				CLASS_MEMBER_BASE(vm::ISolutionItemModel)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(FileFactory)
				CLASS_MEMBER_METHOD(OpenFile, NO_PARAMETER);
				CLASS_MEMBER_METHOD(InitializeFileAndSave, NO_PARAMETER);
			END_CLASS_MEMBER(vm::IFileModel)

			BEGIN_CLASS_MEMBER(vm::IFolderModel)
				CLASS_MEMBER_BASE(vm::ISolutionItemModel)
			END_CLASS_MEMBER(vm::IFolderModel)

			BEGIN_CLASS_MEMBER(vm::IMacroEnvironment)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Parent)
				CLASS_MEMBER_METHOD(HasMacro, { L"name" _ L"inherit" });
				CLASS_MEMBER_METHOD(GetMacroValue, { L"name" _ L"inherit" });
			END_CLASS_MEMBER(vm::IMacroEnvironment)

			BEGIN_CLASS_MEMBER(vm::IOpenInEditorItemAction)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_METHOD(OpenEditor, { L"editorFactory" });
				CLASS_MEMBER_METHOD(CloseEditor, NO_PARAMETER);
			END_CLASS_MEMBER(vm::IOpenInEditorItemAction)

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
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ProjectFactory)
				CLASS_MEMBER_METHOD(OpenProject, NO_PARAMETER);
				CLASS_MEMBER_METHOD(InitializeProjectAndSave, NO_PARAMETER);
			END_CLASS_MEMBER(vm::IProjectModel)

			BEGIN_CLASS_MEMBER(vm::IRemoveItemAction)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_METHOD(Remove, NO_PARAMETER);
			END_CLASS_MEMBER(vm::IRemoveItemAction)

			BEGIN_CLASS_MEMBER(vm::IRenameItemAction)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_METHOD(GetRenameablePart, NO_PARAMETER);
				CLASS_MEMBER_METHOD(PreviewRename, { L"newName" });
				CLASS_MEMBER_METHOD(Rename, { L"newName" });
			END_CLASS_MEMBER(vm::IRenameItemAction)

			BEGIN_CLASS_MEMBER(vm::ISaveItemAction)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_METHOD(Save, NO_PARAMETER);
			END_CLASS_MEMBER(vm::ISaveItemAction)

			BEGIN_CLASS_MEMBER(vm::ISolutionItemModel)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Parent)
				CLASS_MEMBER_EVENT(ImageChanged)
				CLASS_MEMBER_PROPERTY_EVENT_READONLY_FAST(Image, ImageChanged)
				CLASS_MEMBER_EVENT(NameChanged)
				CLASS_MEMBER_PROPERTY_EVENT_READONLY_FAST(Name, NameChanged)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Children)
				CLASS_MEMBER_EVENT(FilePathChanged)
				CLASS_MEMBER_PROPERTY_EVENT_READONLY_FAST(FilePath, FilePathChanged)
				CLASS_MEMBER_EVENT(FileDirectoryChanged)
				CLASS_MEMBER_PROPERTY_EVENT_READONLY_FAST(FileDirectory, FileDirectoryChanged)
				CLASS_MEMBER_EVENT(ErrorCountChanged)
				CLASS_MEMBER_PROPERTY_EVENT_READONLY_FAST(ErrorCount, ErrorCountChanged)
				CLASS_MEMBER_METHOD(GetErrorText, { L"index" });
			END_CLASS_MEMBER(vm::ISolutionItemModel)

			BEGIN_CLASS_MEMBER(vm::ISolutionModel)
				CLASS_MEMBER_BASE(vm::ISolutionItemModel)
				CLASS_MEMBER_METHOD(OpenSolution, NO_PARAMETER);
				CLASS_MEMBER_METHOD(NewSolution, NO_PARAMETER);
				CLASS_MEMBER_METHOD(AddProject, { L"project" });
				CLASS_MEMBER_METHOD(RemoveProject, { L"project" });
			END_CLASS_MEMBER(vm::ISolutionModel)

			BEGIN_CLASS_MEMBER(vm::IStudioAddExistingFilesModel)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(SelectedFiles)
				CLASS_MEMBER_PROPERTY_FAST(CurrentFileName)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(FilteredFileFactories)
				CLASS_MEMBER_METHOD(AddFiles, { L"fileNames" });
				CLASS_MEMBER_METHOD(RemoveFiles, { L"indices" });
			END_CLASS_MEMBER(vm::IStudioAddExistingFilesModel)

			BEGIN_CLASS_MEMBER(vm::IStudioModel)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ProjectFactories)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(FileFactories)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ContentFactories)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(EditorFactories)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(RootSolutionItem)
				CLASS_MEMBER_EVENT(OpenedSolutionChanged)
				CLASS_MEMBER_PROPERTY_EVENT_READONLY_FAST(OpenedSolution, OpenedSolutionChanged)
				CLASS_MEMBER_EVENT(WorkingItemChanged)
				CLASS_MEMBER_PROPERTY_EVENT_READONLY_FAST(WorkingItem, WorkingItemChanged)
				CLASS_MEMBER_EVENT(WorkingProjectChanged)
				CLASS_MEMBER_PROPERTY_EVENT_READONLY_FAST(WorkingProject, WorkingProjectChanged)
				CLASS_MEMBER_EVENT(WorkingDirectoryChanged)
				CLASS_MEMBER_PROPERTY_EVENT_READONLY_FAST(WorkingDirectory, WorkingDirectoryChanged)
				CLASS_MEMBER_METHOD(CreateNewFileModel, NO_PARAMETER);
				CLASS_MEMBER_METHOD(CreateAddExistingFilesModel, { L"action" });
				CLASS_MEMBER_METHOD(NotifySelectedSolutionItem, { L"solutionItem" });
				CLASS_MEMBER_METHOD(GetProjectFactory, { L"id" });
				CLASS_MEMBER_METHOD(GetFileFactory, { L"id" });
				CLASS_MEMBER_METHOD(GetEditorFactory, { L"id" });
				CLASS_MEMBER_METHOD(OpenSolution, { L"filePath" });
				CLASS_MEMBER_METHOD(NewSolution, { L"filePath" });
				CLASS_MEMBER_METHOD(CloseSolution, NO_PARAMETER);
				CLASS_MEMBER_METHOD(AddNewProject, { L"createNewSolution" _ L"projectFactory" _ L"projectName" _ L"solutionDirectory" _ L"solutionName" });
				CLASS_MEMBER_METHOD(AddNewFile, { L"action" _ L"fileFactory" _ L"fileDirectory" _ L"fileName" });
				CLASS_MEMBER_METHOD(AddExistingFiles, { L"action" _ L"files" });
				CLASS_MEMBER_METHOD(RenameFile, { L"action" _ L"solutionItem" _ L"newName" });
				CLASS_MEMBER_METHOD(RemoveFile, { L"action" _ L"solutionItem" });
				CLASS_MEMBER_METHOD(OpenBrowser, { L"url" });
				CLASS_MEMBER_METHOD(PromptError, { L"message" });
				CLASS_MEMBER_METHOD(SafeExecute, { L"procedure" });
				CLASS_MEMBER_METHOD(ExecuteSaveItems, { L"saveItems" });
			END_CLASS_MEMBER(vm::IStudioModel)

			BEGIN_CLASS_MEMBER(vm::IStudioNewFileModel)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(FileFilters)
				CLASS_MEMBER_PROPERTY_FAST(SelectedFileFilter)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(FilteredFileFactories)
			END_CLASS_MEMBER(vm::IStudioNewFileModel)

			BEGIN_CLASS_MEMBER(vm::ITextTemplate)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_METHOD(Generate, { L"macroEnvironment" });
			END_CLASS_MEMBER(vm::ITextTemplate)

			BEGIN_CLASS_MEMBER(vm::StudioFileReference)
				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<vm::StudioFileReference>(), NO_PARAMETER)
				CLASS_MEMBER_FIELD(name)
				CLASS_MEMBER_FIELD(folder)
				CLASS_MEMBER_FIELD(fileFactory)
			END_CLASS_MEMBER(vm::StudioFileReference)

			BEGIN_CLASS_MEMBER(ui::AboutWindow)
				CLASS_MEMBER_BASE(vl::presentation::controls::GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(ui::AboutWindow*(Ptr<vm::IStudioModel>), { L"ViewModel" })

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ViewModel)
			END_CLASS_MEMBER(ui::AboutWindow)

			BEGIN_CLASS_MEMBER(ui::AddExistingFilesWindow)
				CLASS_MEMBER_BASE(vl::presentation::controls::GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(ui::AddExistingFilesWindow*(Ptr<vm::IStudioModel>, Ptr<vm::IStudioAddExistingFilesModel>, Ptr<vm::IAddFileItemAction>), { L"ViewModel" _ L"OperationModel" _ L"Action" })

				CLASS_MEMBER_GUIEVENT_HANDLER(buttonAdd_Clicked, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(buttonCancel_Clicked, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(buttonOK_Clicked, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(buttonRemove_Clicked, vl::presentation::compositions::GuiEventArgs)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ViewModel)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(OperationModel)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Action)
			END_CLASS_MEMBER(ui::AddExistingFilesWindow)

			BEGIN_CLASS_MEMBER(ui::FileReferenceFactoryColumnTemplate)
				CLASS_MEMBER_BASE(vl::presentation::templates::GuiGridVisualizerTemplate)
				CLASS_MEMBER_CONSTRUCTOR(ui::FileReferenceFactoryColumnTemplate*(), NO_PARAMETER)
			END_CLASS_MEMBER(ui::FileReferenceFactoryColumnTemplate)

			BEGIN_CLASS_MEMBER(ui::FileReferenceFactoryEditorTemplate)
				CLASS_MEMBER_BASE(vl::presentation::templates::GuiGridEditorTemplate)
				CLASS_MEMBER_CONSTRUCTOR(ui::FileReferenceFactoryEditorTemplate*(Ptr<vm::IStudioAddExistingFilesModel>), { L"OperationModel" })

				CLASS_MEMBER_PROPERTY_READONLY_FAST(OperationModel)
			END_CLASS_MEMBER(ui::FileReferenceFactoryEditorTemplate)

			BEGIN_CLASS_MEMBER(ui::FileReferenceFolderColumnTemplate)
				CLASS_MEMBER_BASE(vl::presentation::templates::GuiGridVisualizerTemplate)
				CLASS_MEMBER_CONSTRUCTOR(ui::FileReferenceFolderColumnTemplate*(), NO_PARAMETER)
			END_CLASS_MEMBER(ui::FileReferenceFolderColumnTemplate)

			BEGIN_CLASS_MEMBER(ui::FileReferenceNameColumnTemplate)
				CLASS_MEMBER_BASE(vl::presentation::templates::GuiGridVisualizerTemplate)
				CLASS_MEMBER_CONSTRUCTOR(ui::FileReferenceNameColumnTemplate*(), NO_PARAMETER)
			END_CLASS_MEMBER(ui::FileReferenceNameColumnTemplate)

			BEGIN_CLASS_MEMBER(ui::MainWindow)
				CLASS_MEMBER_BASE(vl::presentation::controls::GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(ui::MainWindow*(Ptr<vm::IStudioModel>), { L"ViewModel" })

				CLASS_MEMBER_GUIEVENT_HANDLER(cellSplitter_leftButtonDown, vl::presentation::compositions::GuiMouseEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(cellSplitter_leftButtonUp, vl::presentation::compositions::GuiMouseEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(cellSplitter_mouseMove, vl::presentation::compositions::GuiMouseEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileAddExistingFiles_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileAddNewFile_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileCloseSolution_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileExit_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileNewProject_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileOpenProject_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileOpenWith_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileOpen_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileRemove_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileRename_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileSaveAll_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileSave_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandHelpAbout_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(treeViewSolutionItem_NodeRightButtonUp, vl::presentation::compositions::GuiNodeMouseEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(treeViewSolutionItem_SelectionChanged, vl::presentation::compositions::GuiEventArgs)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ViewModel)
			END_CLASS_MEMBER(ui::MainWindow)

			BEGIN_CLASS_MEMBER(ui::NewFileWindow)
				CLASS_MEMBER_BASE(vl::presentation::controls::GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(ui::NewFileWindow*(Ptr<vm::IStudioModel>, Ptr<vm::IStudioNewFileModel>, Ptr<vm::IAddFileItemAction>), { L"ViewModel" _ L"OperationModel" _ L"Action" })

				CLASS_MEMBER_GUIEVENT_HANDLER(buttonCancel_Clicked, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(buttonCreate_Clicked, vl::presentation::compositions::GuiEventArgs)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ViewModel)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(OperationModel)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Action)
			END_CLASS_MEMBER(ui::NewFileWindow)

			BEGIN_CLASS_MEMBER(ui::NewProjectWindow)
				CLASS_MEMBER_BASE(vl::presentation::controls::GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(ui::NewProjectWindow*(Ptr<vm::IStudioModel>), { L"ViewModel" })

				CLASS_MEMBER_GUIEVENT_HANDLER(buttonBrowse_Clicked, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(buttonCancel_Clicked, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(buttonCreate_Clicked, vl::presentation::compositions::GuiEventArgs)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ViewModel)
			END_CLASS_MEMBER(ui::NewProjectWindow)

			BEGIN_CLASS_MEMBER(ui::RenameFileWindow)
				CLASS_MEMBER_BASE(vl::presentation::controls::GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(ui::RenameFileWindow*(Ptr<vm::IStudioModel>, Ptr<vm::ISolutionItemModel>, Ptr<vm::IRenameItemAction>), { L"ViewModel" _ L"SolutionItem" _ L"Action" })

				CLASS_MEMBER_GUIEVENT_HANDLER(buttonCancel_Clicked, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(buttonRename_Clicked, vl::presentation::compositions::GuiEventArgs)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ViewModel)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(SolutionItem)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Action)
			END_CLASS_MEMBER(ui::RenameFileWindow)

			#undef _

			class GacStudioUIResourceLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					ADD_TYPE_INFO(vm::IAddFileItemAction)
					ADD_TYPE_INFO(vm::IEditorContentFactoryModel)
					ADD_TYPE_INFO(vm::IEditorContentModel)
					ADD_TYPE_INFO(vm::IEditorFactoryModel)
					ADD_TYPE_INFO(vm::IEditorFileContentModel)
					ADD_TYPE_INFO(vm::IEditorModel)
					ADD_TYPE_INFO(vm::IFileFactoryModel)
					ADD_TYPE_INFO(vm::IFileModel)
					ADD_TYPE_INFO(vm::IFolderModel)
					ADD_TYPE_INFO(vm::IMacroEnvironment)
					ADD_TYPE_INFO(vm::IOpenInEditorItemAction)
					ADD_TYPE_INFO(vm::IProjectFactoryModel)
					ADD_TYPE_INFO(vm::IProjectModel)
					ADD_TYPE_INFO(vm::IRemoveItemAction)
					ADD_TYPE_INFO(vm::IRenameItemAction)
					ADD_TYPE_INFO(vm::ISaveItemAction)
					ADD_TYPE_INFO(vm::ISolutionItemModel)
					ADD_TYPE_INFO(vm::ISolutionModel)
					ADD_TYPE_INFO(vm::IStudioAddExistingFilesModel)
					ADD_TYPE_INFO(vm::IStudioModel)
					ADD_TYPE_INFO(vm::IStudioNewFileModel)
					ADD_TYPE_INFO(vm::ITextTemplate)
					ADD_TYPE_INFO(vm::StudioFileReference)
					ADD_TYPE_INFO(ui::AboutWindow)
					ADD_TYPE_INFO(ui::AddExistingFilesWindow)
					ADD_TYPE_INFO(ui::FileReferenceFactoryColumnTemplate)
					ADD_TYPE_INFO(ui::FileReferenceFactoryEditorTemplate)
					ADD_TYPE_INFO(ui::FileReferenceFolderColumnTemplate)
					ADD_TYPE_INFO(ui::FileReferenceNameColumnTemplate)
					ADD_TYPE_INFO(ui::MainWindow)
					ADD_TYPE_INFO(ui::NewFileWindow)
					ADD_TYPE_INFO(ui::NewProjectWindow)
					ADD_TYPE_INFO(ui::RenameFileWindow)
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

