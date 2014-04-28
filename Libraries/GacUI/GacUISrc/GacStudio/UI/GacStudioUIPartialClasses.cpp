﻿/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
GacUI::Partial Classes

本文件使用Vczh GacUI Resource Code Generator工具自动生成
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
			IMPL_TYPE_INFO(vm::IProjectFactoryModel)
			IMPL_TYPE_INFO(vm::IStudioModel)
			IMPL_TYPE_INFO(ui::MainWindow)
			IMPL_TYPE_INFO(ui::NewFileWindow)
			IMPL_TYPE_INFO(ui::NewProjectWindow)

			BEGIN_CLASS_MEMBER(vm::IFileFactoryModel)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Image)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Name)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Category)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Description)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Id)
			END_CLASS_MEMBER(vm::IFileFactoryModel)

			BEGIN_CLASS_MEMBER(vm::IProjectFactoryModel)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Image)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Name)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Description)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Id)
			END_CLASS_MEMBER(vm::IProjectFactoryModel)

			BEGIN_CLASS_MEMBER(vm::IStudioModel)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_METHOD(GetProjectModels, NO_PARAMETER);
				CLASS_MEMBER_METHOD(GetFileModels, { L"category" });
			END_CLASS_MEMBER(vm::IStudioModel)

			BEGIN_CLASS_MEMBER(ui::MainWindow)
				CLASS_MEMBER_BASE(vl::presentation::controls::GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(ui::MainWindow*(Ptr<vm::IStudioModel>), { L"ViewModel" })

				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileExit_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileNewFile_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileNewProject_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileOpenFile_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileOpenProject_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileSaveAll_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileSave_Executed, vl::presentation::compositions::GuiEventArgs)

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
					ADD_TYPE_INFO(vm::IProjectFactoryModel)
					ADD_TYPE_INFO(vm::IStudioModel)
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

