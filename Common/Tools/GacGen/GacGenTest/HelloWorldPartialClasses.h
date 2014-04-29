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
	class IControlViewModel : public virtual vl::reflection::IDescriptable, public vl::reflection::Description<IControlViewModel>
	{
	public:

		virtual vint GetFirst() = 0;
		virtual void SetFirst(vint value) = 0;

		virtual vint GetSecond() = 0;
		virtual void SetSecond(vint value) = 0;

		virtual vint GetSum() = 0;
		vl::Event<void()> SumChanged;

		virtual vint GetProduct() = 0;
	};
}

namespace demos
{
	class IWindowViewModel : public virtual vl::reflection::IDescriptable, public vl::reflection::Description<IWindowViewModel>
	{
	public:

		virtual Ptr<demos::IControlViewModel> GetControlViewModel() = 0;
	};
}

namespace demos
{
	class ISuperWindowViewModel : public virtual demos::IWindowViewModel, public vl::reflection::Description<ISuperWindowViewModel>
	{
	public:

		virtual void DoSomething(vint first, vint second) = 0;
	};
}

namespace demos
{
	struct MyStruct
	{
		vint x;
		vint y;
	};
}

namespace demos
{
	class MyClass : public vl::Object, public vl::reflection::Description<MyClass>
	{
	public:
		demos::MyStruct point;
		Ptr<demos::MyClass> next;
	};
}

namespace demos
{
	class MySuperClass : public demos::MyClass, public vl::reflection::Description<MySuperClass>
	{
	public:
		demos::MyStruct speed;
	};
}

namespace demos
{
	template<typename TImpl>
	class CalculatorControl_ : public vl::presentation::controls::GuiCustomControl, public vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiCustomControl>, public vl::reflection::Description<TImpl>
	{
	private:
		Ptr<demos::IControlViewModel> ViewModel_;
	protected:
		vl::presentation::controls::GuiSinglelineTextBox* textBox1;
		vl::presentation::controls::GuiSinglelineTextBox* textBox2;
		vl::presentation::controls::GuiSinglelineTextBox* textBox3;
		vl::presentation::controls::GuiSinglelineTextBox* textBox4;
		vl::presentation::controls::GuiSinglelineTextBox* textBox5;

		void InitializeComponents(Ptr<demos::IControlViewModel> ViewModel)
		{
			ViewModel_ = ViewModel;
			if (InitializeFromResource())
			{
				GUI_INSTANCE_REFERENCE(textBox1);
				GUI_INSTANCE_REFERENCE(textBox2);
				GUI_INSTANCE_REFERENCE(textBox3);
				GUI_INSTANCE_REFERENCE(textBox4);
				GUI_INSTANCE_REFERENCE(textBox5);
			}
			else
			{
				ViewModel_ = 0;
			}
		}
	public:
		CalculatorControl_()
			:vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiCustomControl>(L"demos::CalculatorControl")
			,vl::presentation::controls::GuiCustomControl(vl::presentation::theme::GetCurrentTheme()->CreateCustomControlStyle())
			,textBox1(0)
			,textBox2(0)
			,textBox3(0)
			,textBox4(0)
			,textBox5(0)
		{
		}

		Ptr<demos::IControlViewModel> GetViewModel()
		{
			return ViewModel_;
		}
	};

	class CalculatorControl;
}

namespace demos
{
	template<typename TImpl>
	class MainWindow_ : public vl::presentation::controls::GuiWindow, public vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>, public vl::reflection::Description<TImpl>
	{
	private:
		Ptr<demos::IWindowViewModel> ViewModel_;
	protected:
		demos::CalculatorControl* calculator;
		vl::presentation::controls::GuiMultilineTextBox* textBoxError;

		void InitializeComponents(Ptr<demos::IWindowViewModel> ViewModel)
		{
			ViewModel_ = ViewModel;
			if (InitializeFromResource())
			{
				GUI_INSTANCE_REFERENCE(calculator);
				GUI_INSTANCE_REFERENCE(textBoxError);
			}
			else
			{
				ViewModel_ = 0;
			}
		}
	public:
		MainWindow_()
			:vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>(L"demos::MainWindow")
			,vl::presentation::controls::GuiWindow(vl::presentation::theme::GetCurrentTheme()->CreateWindowStyle())
			,calculator(0)
			,textBoxError(0)
		{
		}

		Ptr<demos::IWindowViewModel> GetViewModel()
		{
			return ViewModel_;
		}
	};

	class MainWindow;
}

namespace demos
{
	template<typename TImpl>
	class MyComposition_ : public vl::presentation::compositions::GuiBoundsComposition, public vl::presentation::GuiInstancePartialClass<vl::presentation::compositions::GuiBoundsComposition>, public vl::reflection::Description<TImpl>
	{
	private:
	protected:

		void InitializeComponents()
		{
			if (InitializeFromResource())
			{
			}
			else
			{
			}
		}
	public:
		MyComposition_()
			:vl::presentation::GuiInstancePartialClass<vl::presentation::compositions::GuiBoundsComposition>(L"demos::MyComposition")
		{
		}
	};

	class MyComposition;
}

namespace vl
{
	namespace reflection
	{
		namespace description
		{
			DECL_TYPE_INFO(demos::IControlViewModel)
			DECL_TYPE_INFO(demos::ISuperWindowViewModel)
			DECL_TYPE_INFO(demos::IWindowViewModel)
			DECL_TYPE_INFO(demos::MyClass)
			DECL_TYPE_INFO(demos::MyStruct)
			DECL_TYPE_INFO(demos::MySuperClass)
			DECL_TYPE_INFO(demos::CalculatorControl)
			DECL_TYPE_INFO(demos::MainWindow)
			DECL_TYPE_INFO(demos::MyComposition)
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
		void button_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		// #endregion CLASS_MEMBER_GUIEVENT_HANDLER
	public:
		CalculatorControl(Ptr<demos::IControlViewModel> ViewModel);
	};
}


CalculatorControl.cpp :
namespace demos
{
	// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)

	void CalculatorControl::button_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

	CalculatorControl::CalculatorControl(Ptr<demos::IControlViewModel> ViewModel)
	{
		InitializeComponents(ViewModel);
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
		MainWindow(Ptr<demos::IWindowViewModel> ViewModel);
	};
}


MainWindow.cpp :
namespace demos
{
	// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)

	// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

	MainWindow::MainWindow(Ptr<demos::IWindowViewModel> ViewModel)
	{
		InitializeComponents(ViewModel);
	}
}


MyComposition.h :
namespace demos
{
	class MyComposition : public MyComposition_<MyComposition>
	{
		friend class MyComposition_<MyComposition>;
		friend struct vl::reflection::description::CustomTypeDescriptorSelector<MyComposition>;
	protected:

		// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)
		// #endregion CLASS_MEMBER_GUIEVENT_HANDLER
	public:
		MyComposition();
	};
}


MyComposition.cpp :
namespace demos
{
	// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)

	// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

	MyComposition::MyComposition()
	{
		InitializeComponents();
	}
}


*/

#endif
