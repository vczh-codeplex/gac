/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
GacUI::CalculatorControl

本文件使用Vczh GacUI Resource Code Generator工具自动生成
***********************************************************************/

#ifndef VCZH_GACUI_RESOURCE_CODE_GENERATOR_HelloWorld_CalculatorControl
#define VCZH_GACUI_RESOURCE_CODE_GENERATOR_HelloWorld_CalculatorControl

#include "HelloWorldPartialClasses.h"

namespace demos
{
	class CalculatorControl : public CalculatorControl_<CalculatorControl>
	{
		friend class CalculatorControl_<CalculatorControl>;
		friend struct vl::reflection::description::CustomTypeDescriptorSelector<CalculatorControl>;
	protected:

		// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)
		void button_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		// #endregion CLASS_MEMBER_GUIEVENT_HANDLER
	public:
		CalculatorControl(Ptr<demos::IControlViewModel> ViewModel);
	};
}

#endif











