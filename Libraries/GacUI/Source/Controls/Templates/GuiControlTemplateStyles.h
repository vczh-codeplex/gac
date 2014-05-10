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

			class GuiListItemTemplate_ItemStyleProvider
				: public Object
				, public virtual controls::GuiSelectableListControl::IItemStyleProvider
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
				void												SetStyleIndex(controls::GuiListControl::IItemStyleController* style, vint value)override;
				void												SetStyleSelected(controls::GuiListControl::IItemStyleController* style, bool value)override;
			};

			class GuiListItemTemplate_ItemStyleController
				: public Object
				, public virtual controls::GuiListControl::IItemStyleController
			{
			protected:
				GuiListItemTemplate_ItemStyleProvider*				itemStyleProvider;
				GuiListItemTemplate*								itemTemplate;
				bool												installed;

			public:
				GuiListItemTemplate_ItemStyleController(GuiListItemTemplate_ItemStyleProvider* _itemStyleProvider);
				~GuiListItemTemplate_ItemStyleController();

				GuiListItemTemplate*								GetTemplate();
				void												SetTemplate(GuiListItemTemplate* _itemTemplate);

				controls::GuiListControl::IItemStyleProvider*		GetStyleProvider()override;
				vint												GetItemStyleId()override;
				compositions::GuiBoundsComposition*					GetBoundsComposition()override;
				bool												IsCacheable()override;
				bool												IsInstalled()override;
				void												OnInstalled()override;
				void												OnUninstalled()override;
			};

/***********************************************************************
Item Template (GuiTreeItemTemplate)
***********************************************************************/

			class GuiTreeItemTemplate_ItemStyleProvider
				: public Object
				, public virtual controls::tree::INodeItemStyleProvider
			{
			protected:
				Ptr<GuiTemplate::IFactory>							factory;
				controls::GuiVirtualTreeListControl*				treeListControl;
				controls::tree::INodeItemBindingView*				bindingView;
				controls::GuiListControl::IItemStyleProvider*		itemStyleProvider;

			public:
				GuiTreeItemTemplate_ItemStyleProvider(Ptr<GuiTemplate::IFactory> _factory);
				~GuiTreeItemTemplate_ItemStyleProvider();
				
				void												BindItemStyleProvider(controls::GuiListControl::IItemStyleProvider* styleProvider)override;
				controls::GuiListControl::IItemStyleProvider*		GetBindedItemStyleProvider()override;
				void												AttachListControl(controls::GuiListControl* value)override;
				void												DetachListControl()override;
				vint												GetItemStyleId(controls::tree::INodeProvider* node)override;
				controls::tree::INodeItemStyleController*			CreateItemStyle(vint styleId)override;
				void												DestroyItemStyle(controls::tree::INodeItemStyleController* style)override;
				void												Install(controls::tree::INodeItemStyleController* style, controls::tree::INodeProvider* node, vint itemIndex)override;
				void												SetStyleIndex(controls::tree::INodeItemStyleController* style, vint value)override;
				void												SetStyleSelected(controls::tree::INodeItemStyleController* style, bool value)override;
			};
			
#pragma warning(push)
#pragma warning(disable:4250)
			class GuiTreeItemTemplate_ItemStyleController
				: public GuiListItemTemplate_ItemStyleController
				, public virtual controls::tree::INodeItemStyleController
			{
			protected:
				GuiTreeItemTemplate_ItemStyleProvider*				nodeStyleProvider;

			public:
				GuiTreeItemTemplate_ItemStyleController(GuiTreeItemTemplate_ItemStyleProvider* _nodeStyleProvider);
				~GuiTreeItemTemplate_ItemStyleController();
				
				controls::GuiListControl::IItemStyleProvider*		GetStyleProvider()override;
				controls::tree::INodeItemStyleProvider*				GetNodeStyleProvider()override;
			};
#pragma warning(pop)
		}
	}
}

#endif