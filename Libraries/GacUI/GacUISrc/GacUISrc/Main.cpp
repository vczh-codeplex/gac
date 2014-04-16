#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "..\..\Source\GacUI.h"
#include "..\..\..\Workflow\Source\Analyzer\WfAnalyzer.h"
#include "..\..\Source\Reflection\GuiInstanceLoader.h"
#include "..\..\Source\Reflection\TypeDescriptors\GuiReflectionControls.h"
#include "..\..\Source\Reflection\TypeDescriptors\GuiReflectionEvents.h"
#include <Windows.h>

using namespace vl::collections;
using namespace vl::stream;
using namespace vl::reflection::description;
using namespace vl::parsing;
using namespace vl::workflow;
using namespace vl::workflow::analyzer;
using namespace vl::workflow::runtime;

#define GUI_GRAPHICS_RENDERER_DIRECT2D

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
#ifdef GUI_GRAPHICS_RENDERER_GDI
	int result=SetupWindowsGDIRenderer();
#endif
#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D
	int result = SetupWindowsDirect2DRenderer();
#endif

#if _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return result;
}

extern void UnitTestInGuiMain();

/*
Data Binding: IValueObservableList/ItemTemplate
	GuiSelectableListControl
	GuiVirtualTextList
	GuiVirtualListView
	GuiVirtualTreeView
	GuiVirtualDataGrid
	GuiTab
	GuiComboBoxListControl

Features:
	View Model				: View model interface declaration
	Data Binding			: Data structure declaration
	Control Template		: Interface methods to events, or prepare predefined classes
	Workflow as Code Behind
	Data Service and Query
*/

namespace demos
{
// -------- WILL BE GENERATED BY "Vczh GacUI Resource Code Generator" -------- //

	class IControlViewModel : public virtual IDescriptable, public Description<IControlViewModel>
	{
	public:
		virtual vint		GetFirst() = 0;
		virtual void		SetFirst(vint value) = 0;
		virtual vint		GetSecond() = 0;
		virtual void		SetSecond(vint value) = 0;
		virtual vint		GetSum() = 0;
		virtual vint		GetProduct() = 0;

		Event<void()>		SumChanged;
	};

	class IWindowViewModel : public virtual IDescriptable, public Description<IWindowViewModel>
	{
	public:
		virtual Ptr<IControlViewModel>		GetControlViewModel() = 0;
	};

	class CalculatorControl;
	class MainWindow;

	template<typename TImpl>
	class CalculatorControl_ : public GuiCustomControl, public GuiInstancePartialClass<GuiCustomControl>, public Description<TImpl>
	{
	private:
		Ptr<IControlViewModel>	ViewModel_;
	protected:
		GuiSinglelineTextBox*	textBox1;
		GuiSinglelineTextBox*	textBox2;
		GuiSinglelineTextBox*	textBox3;
		GuiSinglelineTextBox*	textBox4;
		GuiSinglelineTextBox*	textBox5;

		void InitializeComponents(Ptr<IControlViewModel> ViewModel)
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
			:GuiCustomControl(GetCurrentTheme()->CreateCustomControlStyle())
			,GuiInstancePartialClass<GuiCustomControl>(L"demos::CalculatorControl")
			,textBox1(0)
			,textBox2(0)
			,textBox3(0)
			,textBox4(0)
			,textBox5(0)
		{
		}

		Ptr<IControlViewModel> GetViewModel()
		{
			return ViewModel_;
		}
	};

	template<typename TImpl>
	class MainWindow_ : public GuiWindow, public GuiInstancePartialClass<GuiWindow>, public Description<TImpl>
	{
	private:
		Ptr<IWindowViewModel>	ViewModel_;
	protected:
		CalculatorControl*		calculator;
		GuiMultilineTextBox*	textBoxError;

		void InitializeComponents(Ptr<IWindowViewModel> ViewModel)
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
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
			,GuiInstancePartialClass<GuiWindow>(L"demos::MainWindow")
			,calculator(0)
			,textBoxError(0)
		{
		}

		Ptr<IWindowViewModel> GetViewModel()
		{
			return ViewModel_;
		}
	};

// -------- WILL BE GENERATED BY "Vczh GacUI Resource Code Generator" -------- //
}

namespace demos
{
	class ControlViewModel : public Object, public virtual IControlViewModel
	{
	private:
		vint			first = 0;
		vint			second = 0;
	public:
		vint GetFirst()override
		{
			return first;
		}

		void SetFirst(vint value)override
		{
			if (first != value)
			{
				first = value;
				SumChanged();
			}
		}

		vint GetSecond()override
		{
			return second;
		}

		void SetSecond(vint value)override
		{
			if (second != value)
			{
				second = value;
				SumChanged();
			}
		}

		vint GetSum()override
		{
			return first + second;
		}

		vint GetProduct()override
		{
			return first * second;
		}
	};

	class WindowViewModel : public Object, public virtual IWindowViewModel
	{
	private:
		Ptr<IControlViewModel>		controlViewModel;
	public:
		WindowViewModel()
		{
			controlViewModel = new ControlViewModel;
		}

		Ptr<IControlViewModel> GetControlViewModel()override
		{
			return controlViewModel;
		}
	};

	class CalculatorControl : public CalculatorControl_<CalculatorControl>
	{
		friend class CalculatorControl_<CalculatorControl>;
		friend struct CustomTypeDescriptorSelector<CalculatorControl>;
	protected:

		void button_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			textBox4->SetText(itow(GetViewModel()->GetProduct()));
		}

	public:
		CalculatorControl(Ptr<IControlViewModel> ViewModel)
		{
			InitializeComponents(ViewModel);
		}
	};

	class MainWindow : public MainWindow_<MainWindow>
	{
		friend class MainWindow_<MainWindow>;
		friend struct CustomTypeDescriptorSelector<MainWindow>;
	protected:

	public:
		MainWindow(Ptr<IWindowViewModel> ViewModel)
		{
			InitializeComponents(ViewModel);
			if (textBoxError && GetScope())
			{
				WString errorText;
				FOREACH(WString, error, GetScope()->errors)
				{
					errorText += error + L"\r\n";
				}
				textBoxError->SetText(errorText);
				textBoxError->Select(TextPos(), TextPos());
			}
		}
	};
}

namespace vl
{
	namespace reflection
	{
		namespace description
		{
// -------- WILL BE GENERATED BY "Vczh GacUI Resource Code Generator" -------- //
			
			DECL_TYPE_INFO(demos::IControlViewModel)
			DECL_TYPE_INFO(demos::IWindowViewModel)
			DECL_TYPE_INFO(demos::CalculatorControl)
			DECL_TYPE_INFO(demos::MainWindow)
			IMPL_TYPE_INFO(demos::IControlViewModel)
			IMPL_TYPE_INFO(demos::IWindowViewModel)
			IMPL_TYPE_INFO(demos::CalculatorControl)
			IMPL_TYPE_INFO(demos::MainWindow)

			BEGIN_CLASS_MEMBER(demos::IControlViewModel)
				CLASS_MEMBER_BASE(IDescriptable)
				CLASS_MEMBER_EVENT(SumChanged)

				CLASS_MEMBER_PROPERTY_FAST(First)
				CLASS_MEMBER_PROPERTY_FAST(Second)
				CLASS_MEMBER_PROPERTY_EVENT_READONLY_FAST(Sum, SumChanged)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Product)
			END_CLASS_MEMBER(demos::IControlViewModel)

			BEGIN_CLASS_MEMBER(demos::IWindowViewModel)
				CLASS_MEMBER_BASE(IDescriptable)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ControlViewModel)
			END_CLASS_MEMBER(demos::IWindowViewModel)

			BEGIN_CLASS_MEMBER(demos::CalculatorControl)
				CLASS_MEMBER_BASE(GuiCustomControl)
				CLASS_MEMBER_CONSTRUCTOR(demos::CalculatorControl*(Ptr<demos::IControlViewModel>), { L"ViewModel" })

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ViewModel)
				CLASS_MEMBER_GUIEVENT_HANDLER(button_Clicked, GuiEventArgs)
			END_CLASS_MEMBER(demos::CalculatorControl)

			BEGIN_CLASS_MEMBER(demos::MainWindow)
				CLASS_MEMBER_BASE(GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(demos::MainWindow*(Ptr<demos::IWindowViewModel>), { L"ViewModel" })

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ViewModel)
			END_CLASS_MEMBER(demos::MainWindow)

			class DemoResourceLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					ADD_TYPE_INFO(demos::IControlViewModel)
					ADD_TYPE_INFO(demos::IWindowViewModel)
					ADD_TYPE_INFO(demos::CalculatorControl)
					ADD_TYPE_INFO(demos::MainWindow)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};

			class DemoResourcePlugin : public Object, public IGuiPlugin
			{
			public:
				void Load()override
				{
					GetGlobalTypeManager()->AddTypeLoader(new DemoResourceLoader);
				}

				void AfterLoad()override
				{
				}

				void Unload()override
				{
				}
			};
			GUI_REGISTER_PLUGIN(DemoResourcePlugin)

// -------- WILL BE GENERATED BY "Vczh GacUI Resource Code Generator" -------- //
		}
	}
}

void GuiMain()
{
	//UnitTestInGuiMain();

	List<WString> errors;
	auto resource = GuiResource::LoadFromXml(L"..\\GacUISrcCodepackedTest\\Resources\\XmlWindowResourceDataBinding.xml", errors);
	GetInstanceLoaderManager()->SetResource(L"Demo", resource);
	demos::MainWindow window(new demos::WindowViewModel);
	window.ForceCalculateSizeImmediately();
	window.MoveToScreenCenter();
	GetApplication()->Run(&window);
}