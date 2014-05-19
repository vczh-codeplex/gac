/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
GacUI::Partial Classes

本文件使用Vczh GacUI Resource Code Generator工具自动生成
************************************************************************
DO NOT MODIFY
***********************************************************************/

#include "HelloWorld.h"

namespace vl
{
	namespace reflection
	{
		namespace description
		{
			#define _ ,
			IMPL_TYPE_INFO(demos::IControlViewModel)
			IMPL_TYPE_INFO(demos::ISuperWindowViewModel)
			IMPL_TYPE_INFO(demos::IWindowViewModel)
			IMPL_TYPE_INFO(demos::MyClass)
			IMPL_TYPE_INFO(demos::MyStruct)
			IMPL_TYPE_INFO(demos::MySuperClass)
			IMPL_TYPE_INFO(demos::CalculatorControl)
			IMPL_TYPE_INFO(demos::MainWindow)
			IMPL_TYPE_INFO(demos::MyComposition)

			BEGIN_CLASS_MEMBER(demos::IControlViewModel)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_PROPERTY_FAST(First)
				CLASS_MEMBER_PROPERTY_FAST(Second)
				CLASS_MEMBER_EVENT(SumChanged)
				CLASS_MEMBER_PROPERTY_EVENT_READONLY_FAST(Sum, SumChanged)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Product)
			END_CLASS_MEMBER(demos::IControlViewModel)

			BEGIN_CLASS_MEMBER(demos::ISuperWindowViewModel)
				CLASS_MEMBER_BASE(demos::IWindowViewModel)
				CLASS_MEMBER_METHOD(DoSomething, { L"first" _ L"second" });
			END_CLASS_MEMBER(demos::ISuperWindowViewModel)

			BEGIN_CLASS_MEMBER(demos::IWindowViewModel)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ControlViewModel)
			END_CLASS_MEMBER(demos::IWindowViewModel)

			BEGIN_CLASS_MEMBER(demos::MyClass)
				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<demos::MyClass>(), NO_PARAMETER)
				CLASS_MEMBER_FIELD(point)
				CLASS_MEMBER_FIELD(next)
			END_CLASS_MEMBER(demos::MyClass)

			BEGIN_STRUCT_MEMBER(demos::MyStruct)
				STRUCT_MEMBER(x)
				STRUCT_MEMBER(y)
			END_STRUCT_MEMBER(demos::MyStruct)

			BEGIN_CLASS_MEMBER(demos::MySuperClass)
				CLASS_MEMBER_BASE(demos::MyClass)
				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<demos::MySuperClass>(), NO_PARAMETER)
				CLASS_MEMBER_FIELD(speed)
			END_CLASS_MEMBER(demos::MySuperClass)

			BEGIN_CLASS_MEMBER(demos::CalculatorControl)
				CLASS_MEMBER_BASE(vl::presentation::controls::GuiCustomControl)
				CLASS_MEMBER_CONSTRUCTOR(demos::CalculatorControl*(Ptr<demos::IControlViewModel>), { L"ViewModel" })

				CLASS_MEMBER_GUIEVENT_HANDLER(button_Clicked, vl::presentation::compositions::GuiEventArgs)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ViewModel)
			END_CLASS_MEMBER(demos::CalculatorControl)

			BEGIN_CLASS_MEMBER(demos::MainWindow)
				CLASS_MEMBER_BASE(vl::presentation::controls::GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(demos::MainWindow*(Ptr<demos::IWindowViewModel>), { L"ViewModel" })

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ViewModel)
			END_CLASS_MEMBER(demos::MainWindow)

			BEGIN_CLASS_MEMBER(demos::MyComposition)
				CLASS_MEMBER_BASE(vl::presentation::compositions::GuiBoundsComposition)
				CLASS_MEMBER_CONSTRUCTOR(demos::MyComposition*(), NO_PARAMETER)
			END_CLASS_MEMBER(demos::MyComposition)

			#undef _

			class HelloWorldResourceLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					ADD_TYPE_INFO(demos::IControlViewModel)
					ADD_TYPE_INFO(demos::ISuperWindowViewModel)
					ADD_TYPE_INFO(demos::IWindowViewModel)
					ADD_TYPE_INFO(demos::MyClass)
					ADD_TYPE_INFO(demos::MyStruct)
					ADD_TYPE_INFO(demos::MySuperClass)
					ADD_TYPE_INFO(demos::CalculatorControl)
					ADD_TYPE_INFO(demos::MainWindow)
					ADD_TYPE_INFO(demos::MyComposition)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};

			class HelloWorldResourcePlugin : public Object, public vl::presentation::controls::IGuiPlugin
			{
			public:
				void Load()override
				{
					GetGlobalTypeManager()->AddTypeLoader(new HelloWorldResourceLoader);
				}

				void AfterLoad()override
				{
				}

				void Unload()override
				{
				}
			};
			GUI_REGISTER_PLUGIN(HelloWorldResourcePlugin)
		}
	}
}

