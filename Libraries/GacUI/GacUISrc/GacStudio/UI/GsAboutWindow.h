﻿/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
GacUI::AboutWindow

This file is generated by: Vczh GacUI Resource Code Generator
***********************************************************************/

#ifndef VCZH_GACUI_RESOURCE_CODE_GENERATOR_GacStudioUI_AboutWindow
#define VCZH_GACUI_RESOURCE_CODE_GENERATOR_GacStudioUI_AboutWindow

#include "GacStudioUIPartialClasses.h"

namespace ui
{
	class AboutWindow : public AboutWindow_<AboutWindow>
	{
		friend class AboutWindow_<AboutWindow>;
		friend struct vl::reflection::description::CustomTypeDescriptorSelector<AboutWindow>;
	protected:

		// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)
		void documentViewer_ActiveHyperlinkExecuted(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		// #endregion CLASS_MEMBER_GUIEVENT_HANDLER
	public:
		AboutWindow();
	};
}

#endif
