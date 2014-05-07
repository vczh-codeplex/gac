/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Template System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_TEMPLATES_GUICONTROLTEMPLATESTYLES
#define VCZH_PRESENTATION_CONTROLS_TEMPLATES_GUICONTROLTEMPLATESTYLES

#include "GuiControlTemplates.h"

namespace vl
{
	namespace presentation
	{
		namespace templates
		{

/***********************************************************************
Control Template
***********************************************************************/

/***********************************************************************
Item Template (GuiListItemTemplate)
***********************************************************************/

			class GuiListItemTemplate_ItemStyleProvider : public Object, public virtual controls::GuiListControl::IItemStyleProvider
			{
			protected:
				Ptr<GuiTemplate::IFactory>							factory;
				controls::GuiListControl*							listControl;
				controls::GuiListControl::IItemBindingView*			bindingView;

			public:
				GuiListItemTemplate_ItemStyleProvider(Ptr<GuiTemplate::IFactory> _factory);
				~GuiListItemTemplate_ItemStyleProvider();

				void												AttachListControl(controls::GuiListControl* value)override;
				void												DetachListControl()override;
				vint												GetItemStyleId(vint itemIndex)override;
				controls::GuiListControl::IItemStyleController*		CreateItemStyle(vint styleId)override;
				void												DestroyItemStyle(controls::GuiListControl::IItemStyleController* style)override;
				void												Install(controls::GuiListControl::IItemStyleController* style, vint itemIndex)override;
			};

			class GuiListItemTemplate_ItemStyleController : public Object, public virtual controls::GuiListControl::IItemStyleController
			{
			protected:
				GuiListItemTemplate_ItemStyleProvider*				itemStyleProvider;
				GuiListItemTemplate*								itemTemplate;
				bool												installed;

			public:
				GuiListItemTemplate_ItemStyleController(GuiListItemTemplate_ItemStyleProvider* _itemStyleProvider);
				~GuiListItemTemplate_ItemStyleController();

				void												SetTemplate(GuiListItemTemplate* _itemTemplate);

				controls::GuiListControl::IItemStyleProvider*		GetStyleProvider()override;
				vint												GetItemStyleId()override;
				compositions::GuiBoundsComposition*					GetBoundsComposition()override;
				bool												IsCacheable()override;
				bool												IsInstalled()override;
				void												OnInstalled()override;
				void												OnUninstalled()override;
			};
		}
	}
}

#endif