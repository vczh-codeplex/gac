/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
GacUI::Partial Classes

本文件使用Vczh GacUI Resource Code Generator工具自动生成
************************************************************************
DO NOT MODIFY
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
		vl::presentation::controls::GuiButton* buttonLogin;
		vl::presentation::controls::GuiDocumentLabel* documentLabelBottom;
		vl::presentation::controls::GuiDocumentLabel* documentLabelTop;
		vl::presentation::controls::GuiSinglelineTextBox* textBoxPassword;
		vl::presentation::controls::GuiSinglelineTextBox* textBoxUserName;

		void InitializeComponents()
		{
			if (InitializeFromResource())
			{
				GUI_INSTANCE_REFERENCE(buttonLogin);
				GUI_INSTANCE_REFERENCE(documentLabelBottom);
				GUI_INSTANCE_REFERENCE(documentLabelTop);
				GUI_INSTANCE_REFERENCE(textBoxPassword);
				GUI_INSTANCE_REFERENCE(textBoxUserName);
			}
		}
	public:
		SignInWindow_()
			:vl::presentation::controls::GuiWindow(vl::presentation::theme::GetCurrentTheme()->CreateWindowStyle())
			,vl::presentation::GuiInstancePartialClass<vl::presentation::controls::GuiWindow>(L"codeplex_client::SignInWindow")
			,buttonLogin(0)
			,documentLabelBottom(0)
			,documentLabelTop(0)
			,textBoxPassword(0)
			,textBoxUserName(0)
		{
		}
	};

	class SignInWindow;
}

/*
SignInWindow.h :
namespace codeplex_client
{
	class SignInWindow : public SignInWindow_<SignInWindow>
	{
		friend class SignInWindow_<SignInWindow>;
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


SignInWindow.cpp :
namespace codeplex_client
{
	SignInWindow::SignInWindow()
	{
		InitializeComponents();
	}
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
			IMPL_TYPE_INFO(codeplex_client::SignInWindow)
		}
	}
}


*/

#endif
