﻿/***********************************************************************
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

		void InitializeComponents()
			{
				if (InitializeFromResource())
				{
				}
			}
		public:
			MainWindow_()
				:vl::presentation::controls::GuiWindow(vl::presentation::theme::GetCurrentTheme()->CreateWindowStyle())
				,vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>(L"gacstudio::user_interface::MainWindow")
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
		// #region CLASS_MEMBER_GUIEVENT_HANDLER

		// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

		MainWindow::MainWindow()
		{
			InitializeComponents();
		}
	}
}


*/

#endif
