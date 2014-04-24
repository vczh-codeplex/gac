/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
GacUI::CalculatorControl

本文件使用Vczh GacUI Resource Code Generator工具自动生成
***********************************************************************/

#include "HelloWorld.h"

namespace demos
{
	// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)

	void CalculatorControl::button_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments)
	{
		textBox4->SetText(itow(GetViewModel()->GetProduct()));
	}

	// #endregion CLASS_MEMBER_GUIEVENT_HANDLER

	CalculatorControl::CalculatorControl(Ptr<demos::IControlViewModel> ViewModel)
	{
		InitializeComponents(ViewModel);
	}
}