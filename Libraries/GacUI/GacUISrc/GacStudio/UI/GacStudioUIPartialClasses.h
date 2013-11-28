/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
GacUI::Partial Classes

本文件使用Vczh GacUI Resource Code Generator工具自动生成
************************************************************************
DO NOT MODIFY
***********************************************************************/

#ifndef VCZH_GACUI_RESOURCE_CODE_GENERATOR_GacStudioUI_PARTIAL_CLASSES
#define VCZH_GACUI_RESOURCE_CODE_GENERATOR_GacStudioUI_PARTIAL_CLASSES

#include "..\..\..\Public\Source\GacUIReflection.h"

namespace gacstudio
{
	namespace user_interface
	{
		template<typename TImpl>
		class MainWindow_ : public vl::presentation::controls::GuiWindow, public vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>, public vl::reflection::Description<TImpl>
		{
		protected:
			vl::presentation::controls::GuiToolstripCommand* commandFileExit;
			vl::presentation::controls::GuiToolstripCommand* commandFileNew;
			vl::presentation::controls::GuiToolstripCommand* commandFileOpen;
			vl::presentation::controls::GuiToolstripCommand* commandFileSave;
			vl::presentation::controls::GuiToolstripCommand* commandFileSaveAll;

		void InitializeComponents()
			{
				if (InitializeFromResource())
				{
					GUI_INSTANCE_REFERENCE(commandFileExit);
					GUI_INSTANCE_REFERENCE(commandFileNew);
					GUI_INSTANCE_REFERENCE(commandFileOpen);
					GUI_INSTANCE_REFERENCE(commandFileSave);
					GUI_INSTANCE_REFERENCE(commandFileSaveAll);
				}
			}
		public:
			MainWindow_()
				:vl::presentation::controls::GuiWindow(vl::presentation::theme::GetCurrentTheme()->CreateWindowStyle())
				,vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>(L"gacstudio::user_interface::MainWindow")
				,commandFileExit(0)
				,commandFileNew(0)
				,commandFileOpen(0)
				,commandFileSave(0)
				,commandFileSaveAll(0)
			{
			}
		};

		class MainWindow;
	}
}

namespace vl
{
	namespace reflection
	{
		namespace description
		{
			DECL_TYPE_INFO(gacstudio::user_interface::MainWindow)
		}
	}
}

/*
GsMainWindow.h :
namespace gacstudio
{
	namespace user_interface
	{
		class MainWindow : public MainWindow_<MainWindow>
		{
			friend class MainWindow_<MainWindow>;
			friend struct vl::reflection::description::CustomTypeDescriptorSelector<MainWindow>;
		protected:

			// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)
			// #endregion CLASS_MEMBER_GUIEVENT_HANDLER
		public:
			MainWindow();
		};
	}
}


GsMainWindow.cpp :
namespace gacstudio
{
	namespace user_interface
	{
		// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)

		// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

		MainWindow::MainWindow()
		{
			InitializeComponents();
		}
	}
}


*/

#endif
