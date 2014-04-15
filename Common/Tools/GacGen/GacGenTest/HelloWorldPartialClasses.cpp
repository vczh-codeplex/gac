/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
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
			IMPL_TYPE_INFO(demos::IWindowViewModel)
			IMPL_TYPE_INFO(demos::MyClass)
			IMPL_TYPE_INFO(demos::MyStruct)
			IMPL_TYPE_INFO(demos::CalculatorControl)
			IMPL_TYPE_INFO(demos::MainWindow)

			BEGIN_CLASS_MEMBER(demos::IControlViewModel)
				CLASS_MEMBER_PROPERTY_FAST(First)
				CLASS_MEMBER_PROPERTY_FAST(Second)
				CLASS_MEMBER_EVENT(SumChanged)
				CLASS_MEMBER_PROPERTY_EVENT_READONLY_FAST(Sum, SumChanged)
			END_CLASS_MEMBER(demos::IControlViewModel)

			BEGIN_CLASS_MEMBER(demos::IWindowViewModel)
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

			BEGIN_CLASS_MEMBER(demos::CalculatorControl)
				CLASS_MEMBER_BASE(vl::presentation::controls::GuiCustomControl)
				CLASS_MEMBER_CONSTRUCTOR(demos::CalculatorControl*(Ptr<demos::IControlViewModel>), { L"ViewModel" })

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ViewModel)
			END_CLASS_MEMBER(demos::CalculatorControl)

			BEGIN_CLASS_MEMBER(demos::MainWindow)
				CLASS_MEMBER_BASE(vl::presentation::controls::GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(demos::MainWindow*(Ptr<demos::IWindowViewModel>), { L"ViewModel" })

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ViewModel)
			END_CLASS_MEMBER(demos::MainWindow)

			#undef _

			class HelloWorldResourceLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					ADD_TYPE_INFO(demos::IControlViewModel)
					ADD_TYPE_INFO(demos::IWindowViewModel)
					ADD_TYPE_INFO(demos::MyClass)
					ADD_TYPE_INFO(demos::MyStruct)
					ADD_TYPE_INFO(demos::CalculatorControl)
					ADD_TYPE_INFO(demos::MainWindow)
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

