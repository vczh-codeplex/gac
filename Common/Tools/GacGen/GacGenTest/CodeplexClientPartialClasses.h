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

namespace vl
{
	namespace reflection
	{
		namespace description
		{
			DECL_TYPE_INFO(codeplex_client::SignInWindow)
		}
	}
}

/*
SignInWindow.h :
namespace codeplex_client
{
	class SignInWindow : public SignInWindow_<SignInWindow>
	{
		friend class SignInWindow_<SignInWindow>;
		friend struct vl::reflection::description::CustomTypeDescriptorSelector<SignInWindow>;
	protected:

		// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)
		void buttonLogin_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void textBoxPassword_TextChanged(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void textBoxUserName_TextChanged(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void window_gotFocus(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void window_lostFocus(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		// #endregion CLASS_MEMBER_GUIEVENT_HANDLER
	public:
		SignInWindow();
	};
}


SignInWindow.cpp :
namespace codeplex_client
{
	// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)

	void SignInWindow::buttonLogin_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	void SignInWindow::textBoxPassword_TextChanged(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	void SignInWindow::textBoxUserName_TextChanged(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	void SignInWindow::window_gotFocus(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	void SignInWindow::window_lostFocus(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
	}

	// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

	SignInWindow::SignInWindow()
	{
		InitializeComponents();
	}
}


*/

#endif
