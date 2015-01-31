﻿/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
GacUI::RenameFileWindow

This file is generated by: Vczh GacUI Resource Code Generator
***********************************************************************/

#ifndef VCZH_GACUI_RESOURCE_CODE_GENERATOR_GacStudioUI_RenameFileWindow
#define VCZH_GACUI_RESOURCE_CODE_GENERATOR_GacStudioUI_RenameFileWindow

#include "GacStudioUIPartialClasses.h"

namespace ui
{
	class RenameFileWindow : public ui::RenameFileWindow_<ui::RenameFileWindow>
	{
		friend class ui::RenameFileWindow_<ui::RenameFileWindow>;
		friend struct vl::reflection::description::CustomTypeDescriptorSelector<ui::RenameFileWindow>;
	protected:

		// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)
		void buttonCancel_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void buttonCreate_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		// #endregion CLASS_MEMBER_GUIEVENT_HANDLER
	public:
		RenameFileWindow(Ptr<vm::ISolutionItemModel> SolutionItem, Ptr<vm::IRenameItemAction> Action);
	};
}

#endif
