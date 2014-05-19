/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
GacUI::MyComposition

本文件使用Vczh GacUI Resource Code Generator工具自动生成
***********************************************************************/

#ifndef VCZH_GACUI_RESOURCE_CODE_GENERATOR_HelloWorld_MyComposition
#define VCZH_GACUI_RESOURCE_CODE_GENERATOR_HelloWorld_MyComposition

#include "HelloWorldPartialClasses.h"

namespace demos
{
	class MyComposition : public MyComposition_<MyComposition>
	{
		friend class MyComposition_<MyComposition>;
		friend struct vl::reflection::description::CustomTypeDescriptorSelector<MyComposition>;
	protected:

		// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)
		// #endregion CLASS_MEMBER_GUIEVENT_HANDLER
	public:
		MyComposition();
	};
}

#endif
