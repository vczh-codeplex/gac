﻿/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
GacUI::SignInWindow

本文件使用Vczh GacUI Resource Code Generator工具自动生成
***********************************************************************/

#ifndef VCZH_GACUI_RESOURCE_CODE_GENERATOR_CodeplexClient_SignInWindow
#define VCZH_GACUI_RESOURCE_CODE_GENERATOR_CodeplexClient_SignInWindow

#include "CodeplexClientPartialClasses.h"

namespace codeplex_client
{
	class SignInWindow : public SignInWindow_<SignInWindow>
	{
		friend class SignInWindow_<SignInWindow>;
		friend struct vl::reflection::description::CustomTypeDescriptorSelector<SignInWindow>;
	protected:

	public:
		SignInWindow();
	};
}

namespace vl
{
	namespace reflection
	{
		namespace description
		{
			// -- REFLECTION CODE --
			// DO NOT MODIFY IF YOU DO NOT UNDERATAND.
			// THESE LINES OF CODE IS NECESSARY TO CORRECTLY LOAD THE CONTROL FROM THE RESOURCE.
			DECL_TYPE_INFO(codeplex_client::SignInWindow)

			BEGIN_CLASS_MEMBER(codeplex_client::SignInWindow)
				CLASS_MEMBER_BASE(vl::presentation::controls::GuiWindow)
				CLASS_MEMBER_CONSTRUCTOR(codeplex_client::SignInWindow*(), NO_PARAMETER)

			END_CLASS_MEMBER(codeplex_client::SignInWindow)
		}
	}
}

#endif
