﻿/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
GacUI::Partial Classes

本文件使用Vczh GacUI Resource Code Generator工具自动生成
************************************************************************
DO NOT MODIFY
***********************************************************************/

#include "Demo.h"

namespace vl
{
	namespace reflection
	{
		namespace description
		{
			#define _ ,
			IMPL_TYPE_INFO(demos::ISeason)
			IMPL_TYPE_INFO(demos::IViewModel)
			IMPL_TYPE_INFO(demos::MainWindow)

			BEGIN_CLASS_MEMBER(demos::ISeason)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Season)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Description)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Children)
			END_CLASS_MEMBER(demos::ISeason)

			BEGIN_CLASS_MEMBER(demos::IViewModel)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Seasons)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ComplexSeasons)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(TreeSeasons)
				CLASS_MEMBER_METHOD(AddSeason, NO_PARAMETER);
				CLASS_MEMBER_METHOD(AddComplexSeason, NO_PARAMETER);
				CLASS_MEMBER_METHOD(AddTreeSeason, NO_PARAMETER);
			END_CLASS_MEMBER(demos::IViewModel)

			BEGIN_CLASS_MEMBER(demos::MainWindow)
				CLASS_MEMBER_BASE(vl::presentation::controls::GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(demos::MainWindow*(Ptr<demos::IViewModel>), { L"ViewModel" })

				CLASS_MEMBER_PROPERTY_READONLY_FAST(ViewModel)
			END_CLASS_MEMBER(demos::MainWindow)

			#undef _

			class DemoResourceLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					ADD_TYPE_INFO(demos::ISeason)
					ADD_TYPE_INFO(demos::IViewModel)
					ADD_TYPE_INFO(demos::MainWindow)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};

			class DemoResourcePlugin : public Object, public vl::presentation::controls::IGuiPlugin
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
		}
	}
}

