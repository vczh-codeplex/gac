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

			BEGIN_CLASS_MEMBER(gacstudio::user_interface::MainWindow)
				CLASS_MEMBER_BASE(vl::presentation::controls::GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(gacstudio::user_interface::MainWindow*(), NO_PARAMETER)

			END_CLASS_MEMBER(gacstudio::user_interface::MainWindow)

			class GacStudioUIResourceLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					ADD_TYPE_INFO(gacstudio::user_interface::MainWindow)
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

