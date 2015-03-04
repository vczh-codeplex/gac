﻿/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
GacUI::VScrollTemplate

This file is generated by: Vczh GacUI Resource Code Generator
***********************************************************************/

#ifndef VCZH_GACUI_RESOURCE_CODE_GENERATOR_HelloWorld_VScrollTemplate
#define VCZH_GACUI_RESOURCE_CODE_GENERATOR_HelloWorld_VScrollTemplate

#include "HelloWorldPartialClasses.h"

namespace darkskin
{
	class VScrollTemplate : public VScrollTemplate_<VScrollTemplate>
	{
		friend class VScrollTemplate_<VScrollTemplate>;
		friend struct vl::reflection::description::CustomTypeDescriptorSelector<VScrollTemplate>;
	protected:

		Point								draggingStartLocation;
		bool								draggingHandle = false;

		// #region CLASS_MEMBER_GUIEVENT_HANDLER (DO NOT PUT OTHER CONTENT IN THIS #region.)
		void OnHandleMouseDown(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiMouseEventArgs& arguments);
		void OnHandleMouseMove(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiMouseEventArgs& arguments);
		void OnHandleMouseUp(GuiGraphicsComposition* sender, vl::presentation::compositions::GuiMouseEventArgs& arguments);
		// #endregion CLASS_MEMBER_GUIEVENT_HANDLER
	public:
		VScrollTemplate();
	};
}

#endif
