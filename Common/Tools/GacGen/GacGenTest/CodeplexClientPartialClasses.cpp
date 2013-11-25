﻿/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
GacUI::Partial Classes

本文件使用Vczh GacUI Resource Code Generator工具自动生成
************************************************************************
DO NOT MODIFY
***********************************************************************/

#include "CodeplexClient.h"

namespace vl
{
	namespace reflection
	{
		namespace description
		{
			class CodeplexClientResourceLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					ADD_TYPE_INFO(codeplex_client::SignInWindow)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};

			class CodeplexClientResourcePlugin : public Object, public vl::presentation::controls::IGuiPlugin
			{
			public:
				void Load()override
				{
					GetGlobalTypeManager()->AddTypeLoader(new CodeplexClientResourceLoader);
				}

				void AfterLoad()override
				{
				}

				void Unload()override
				{
				}
			};
			GUI_REGISTER_PLUGIN(CodeplexClientResourcePlugin)
		}
	}
}
