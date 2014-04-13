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

	class IViewModel : public virtual IDescriptable, public Description<IViewModel>
	{
	public:
		virtual vint		GetFirst() = 0;
		virtual void		SetFirst(vint value) = 0;
		virtual vint		GetSecond() = 0;
		virtual void		SetSecond(vint value) = 0;
		virtual vint		GetSum() = 0;

		Event<void()>		FirstChanged;
		Event<void()>		SecondChanged;
		Event<void()>		SumChanged;
	};

	template<typename TImpl>
	class MainWindow_ : public GuiWindow, public GuiInstancePartialClass<GuiWindow>, public Description<TImpl>
	{
	private:
		Ptr<IViewModel>			_ViewModel;
	protected:
		GuiSinglelineTextBox*	textBox1;
		GuiSinglelineTextBox*	textBox2;
		GuiSinglelineTextBox*	textBox3;
		GuiMultilineTextBox*	textBoxError;

		void InitializeComponents(Ptr<IViewModel> ViewModel)
		{
			_ViewModel = ViewModel;
			if (InitializeFromResource())
			{
				GUI_INSTANCE_REFERENCE(textBox1);
				GUI_INSTANCE_REFERENCE(textBox2);
				GUI_INSTANCE_REFERENCE(textBox3);
				GUI_INSTANCE_REFERENCE(textBoxError);
			}
			else
			{
				_ViewModel = 0;
			}
		}
	public:
		MainWindow_()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
			,GuiInstancePartialClass<GuiWindow>(L"demos::MainWindow")
			,textBox1(0)
			,textBox2(0)
			,textBox3(0)
			,textBoxError(0)
		{
		}

		Ptr<IViewModel> GetViewModel()
		{
			return _ViewModel;
		}
	};

// -------- WILL BE GENERATED BY "Vczh GacUI Resource Code Generator" -------- //
}

namespace demos
{
	class ViewModel : public Object, public virtual IViewModel
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
				FirstChanged();
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
				SecondChanged();
			}
		}

		vint GetSum()override
		{
			return first + second;
		}

	};

	class MainWindow : public MainWindow_<MainWindow>
	{
		friend class MainWindow_<MainWindow>;
		friend struct CustomTypeDescriptorSelector<MainWindow>;
	protected:

	public:
		MainWindow(Ptr<IViewModel> ViewModel)
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
			
			DECL_TYPE_INFO(demos::IViewModel)
			DECL_TYPE_INFO(demos::MainWindow)
			IMPL_TYPE_INFO(demos::IViewModel)
			IMPL_TYPE_INFO(demos::MainWindow)

			BEGIN_CLASS_MEMBER(demos::IViewModel)
				CLASS_MEMBER_BASE(IDescriptable)

				CLASS_MEMBER_EVENT(FirstChanged)
				CLASS_MEMBER_EVENT(SecondChanged)
				CLASS_MEMBER_EVENT(SumChanged)

				CLASS_MEMBER_PROPERTY_EVENT_FAST(First, FirstChanged)
				CLASS_MEMBER_PROPERTY_EVENT_FAST(Second, SecondChanged)
				CLASS_MEMBER_PROPERTY_EVENT_READONLY_FAST(Sum, SumChanged)
			END_CLASS_MEMBER(demos::IViewModel)

			BEGIN_CLASS_MEMBER(demos::MainWindow)
				CLASS_MEMBER_BASE(GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(demos::MainWindow*(Ptr<demos::IViewModel>), { L"ViewModel" })

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ViewModel)
			END_CLASS_MEMBER(demos::MainWindow)

			class DemoResourceLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					ADD_TYPE_INFO(demos::IViewModel)
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

	GetInstanceLoaderManager()->SetResource(L"Demo", GuiResource::LoadFromXml(L"..\\GacUISrcCodepackedTest\\Resources\\XmlWindowResourceDataBinding.xml"));
	demos::MainWindow window(new demos::ViewModel);
	window.ForceCalculateSizeImmediately();
	window.MoveToScreenCenter();
	GetApplication()->Run(&window);
}