/***********************************************************************
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

		// #region CLASS_MEMBER_GUIEVENT_HANDLER
		void buttonLogin_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void window_gotFocus(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void window_lostFocus(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		// #endregion CLASS_MEMBER_GUIEVENT_HANDLER
	public:
		SignInWindow();
	};
}

#endif
