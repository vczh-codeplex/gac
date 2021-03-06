﻿/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
GacUI::AddExistingFilesWindow

This file is generated by: Vczh GacUI Resource Code Generator
***********************************************************************/

#ifndef VCZH_GACUI_RESOURCE_CODE_GENERATOR_GacStudioUI_AddExistingFilesWindow
#define VCZH_GACUI_RESOURCE_CODE_GENERATOR_GacStudioUI_AddExistingFilesWindow

#include "GacStudioUIPartialClasses.h"

namespace ui
{
	class AddExistingFilesWindow : public ui::AddExistingFilesWindow_<ui::AddExistingFilesWindow>
	{
		friend class ui::AddExistingFilesWindow_<ui::AddExistingFilesWindow>;
		friend struct vl::reflection::description::CustomTypeDescriptorSelector<ui::AddExistingFilesWindow>;
	protected:

		// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)
		void buttonAdd_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void buttonCancel_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void buttonOK_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		void buttonRemove_Clicked(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiEventArgs& arguments);
		// #endregion CLASS_MEMBER_GUIEVENT_HANDLER
	public:
		AddExistingFilesWindow(Ptr<vm::IStudioModel> ViewModel, Ptr<vm::IStudioAddExistingFilesModel> OperationModel, Ptr<vm::IAddFileItemAction> Action);
	};
}

#endif
