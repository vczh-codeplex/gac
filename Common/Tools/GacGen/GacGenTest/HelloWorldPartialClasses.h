/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
GacUI::Partial Classes

本文件使用Vczh GacUI Resource Code Generator工具自动生成
************************************************************************
DO NOT MODIFY
***********************************************************************/

#ifndef VCZH_GACUI_RESOURCE_CODE_GENERATOR_HelloWorld_PARTIAL_CLASSES
#define VCZH_GACUI_RESOURCE_CODE_GENERATOR_HelloWorld_PARTIAL_CLASSES

#include "..\..\..\..\Libraries\GacUI\Public\Source\GacUIReflection.h"

namespace demos
{
	template<typename TImpl>
	class CalculatorControl_ : public vl::presentation::controls::GuiCustomControl, public vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiCustomControl>, public vl::reflection::Description<TImpl>
	{
	protected:
		vl::presentation::controls::GuiSinglelineTextBox* textBox1;
		vl::presentation::controls::GuiSinglelineTextBox* textBox2;
		vl::presentation::controls::GuiSinglelineTextBox* textBox3;

		void InitializeComponents()
		{
			if (InitializeFromResource())
			{
				GUI_INSTANCE_REFERENCE(textBox1);
				GUI_INSTANCE_REFERENCE(textBox2);
				GUI_INSTANCE_REFERENCE(textBox3);
			}
		}
	public:
		CalculatorControl_()
			:vl::presentation::controls::GuiCustomControl(vl::presentation::theme::GetCurrentTheme()->CreateCustomControlStyle())
			,vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiCustomControl>(L"demos::CalculatorControl")
			,textBox1(0)
			,textBox2(0)
			,textBox3(0)
		{
		}
	};

	class CalculatorControl;
}

namespace demos
{
	template<typename TImpl>
	class MainWindow_ : public vl::presentation::controls::GuiWindow, public vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>, public vl::reflection::Description<TImpl>
	{
	protected:
		demos::CalculatorControl* calculator;
		vl::presentation::controls::GuiMultilineTextBox* textBoxError;

		void InitializeComponents()
		{
			if (InitializeFromResource())
			{
				GUI_INSTANCE_REFERENCE(calculator);
				GUI_INSTANCE_REFERENCE(textBoxError);
			}
		}
	public:
		MainWindow_()
			:vl::presentation::controls::GuiWindow(vl::presentation::theme::GetCurrentTheme()->CreateWindowStyle())
			,vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>(L"demos::MainWindow")
			,calculator(0)
			,textBoxError(0)
		{
		}
	};

	class MainWindow;
}

namespace vl
{
	namespace reflection
	{
		namespace description
		{
			DECL_TYPE_INFO(demos::CalculatorControl)
			DECL_TYPE_INFO(demos::MainWindow)
		}
	}
}

/*
CalculatorControl.h :
namespace demos
{
	class CalculatorControl : public CalculatorControl_<CalculatorControl>
	{
		friend class CalculatorControl_<CalculatorControl>;
		friend struct vl::reflection::description::CustomTypeDescriptorSelector<CalculatorControl>;
	protected:

		// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)
		// #endregion CLASS_MEMBER_GUIEVENT_HANDLER
	public:
		CalculatorControl();
	};
}


CalculatorControl.cpp :
namespace demos
{
	// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)

	// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

	CalculatorControl::CalculatorControl()
	{
		InitializeComponents();
	}
}


MainWindow.h :
namespace demos
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


MainWindow.cpp :
namespace demos
{
	// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)

	// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

	MainWindow::MainWindow()
	{
		InitializeComponents();
	}
}


*/

#endif
