/***********************************************************************
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
			IMPL_TYPE_INFO(gacstudio::user_interface::MainWindow)
			IMPL_TYPE_INFO(gacstudio::user_interface::NewFileWindow)
			IMPL_TYPE_INFO(gacstudio::user_interface::NewProjectWindow)

			BEGIN_CLASS_MEMBER(gacstudio::user_interface::MainWindow)
				CLASS_MEMBER_BASE(vl::presentation::controls::GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(gacstudio::user_interface::MainWindow*(), NO_PARAMETER)

				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileExit_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileNewFile_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileNewProject_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileOpenFile_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileOpenProject_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileSaveAll_Executed, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(commandFileSave_Executed, vl::presentation::compositions::GuiEventArgs)
			END_CLASS_MEMBER(gacstudio::user_interface::MainWindow)

			BEGIN_CLASS_MEMBER(gacstudio::user_interface::NewFileWindow)
				CLASS_MEMBER_BASE(vl::presentation::controls::GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(gacstudio::user_interface::NewFileWindow*(), NO_PARAMETER)

				CLASS_MEMBER_GUIEVENT_HANDLER(buttonCancel_Clicked, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(buttonCreate_Clicked, vl::presentation::compositions::GuiEventArgs)
			END_CLASS_MEMBER(gacstudio::user_interface::NewFileWindow)

			BEGIN_CLASS_MEMBER(gacstudio::user_interface::NewProjectWindow)
				CLASS_MEMBER_BASE(vl::presentation::controls::GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(gacstudio::user_interface::NewProjectWindow*(), NO_PARAMETER)

				CLASS_MEMBER_GUIEVENT_HANDLER(buttonBrowse_Clicked, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(buttonCancel_Clicked, vl::presentation::compositions::GuiEventArgs)
				CLASS_MEMBER_GUIEVENT_HANDLER(buttonCreate_Clicked, vl::presentation::compositions::GuiEventArgs)
			END_CLASS_MEMBER(gacstudio::user_interface::NewProjectWindow)

			class GacStudioUIResourceLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					ADD_TYPE_INFO(gacstudio::user_interface::MainWindow)
					ADD_TYPE_INFO(gacstudio::user_interface::NewFileWindow)
					ADD_TYPE_INFO(gacstudio::user_interface::NewProjectWindow)
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

