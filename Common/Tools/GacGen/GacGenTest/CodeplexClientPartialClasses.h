/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
GacUI::Partial Classes

本文件使用Vczh GacUI Resource Code Generator工具自动生成
***********************************************************************/

#ifndef VCZH_GACUI_RESOURCE_CODE_GENERATOR_CodeplexClient_PARTIAL_CLASSES
#define VCZH_GACUI_RESOURCE_CODE_GENERATOR_CodeplexClient_PARTIAL_CLASSES

#include "..\..\..\..\Libraries\GacUI\Public\Source\GacUIReflection.h"

namespace codeplex_client
{
	template<typename TImpl>
	class SignInWindow_ : public vl::presentation::controls::GuiWindow, public vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>, public vl::reflection::Description<TImpl>
	{
	protected:

	void InitializeComponents()
		{
		}
	public:
		SignInWindow_()
			:vl::presentation::controls::GuiWindow(vl::presentation::theme::GetCurrentTheme()->CreateWindowStyle())
			,vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>(L"codeplex_client::SignInWindow")
		{
		}
	};
}

/*
SignInWindow.h :
	class SignInWindow : public SignInWindow_<SignInWindow>
	{
		friend class SignInWindow_<SignInWindow>;
	public:
		SignInWindow();
	};

SignInWindow.cpp :
	SignInWindow::SignInWindow()
	{
		InitializeComponents();
	}

*/

#endif
