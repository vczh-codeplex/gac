/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
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
#pragma warning(push)
#pragma warning(disable:4250)

/***********************************************************************
Control Template
***********************************************************************/

			class GuiControlTemplate_StyleProvider
				: public Object
				, public virtual controls::GuiControl::IStyleController
				, public Description<GuiControlTemplate_StyleProvider>
			{
			protected:
				GuiControlTemplate*									controlTemplate;

			public:
				GuiControlTemplate_StyleProvider(Ptr<GuiTemplate::IFactory> factory);
				~GuiControlTemplate_StyleProvider();

				compositions::GuiBoundsComposition*					GetBoundsComposition()override;
				compositions::GuiGraphicsComposition*				GetContainerComposition()override;
				void												SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
				void												SetText(const WString& value)override;
				void												SetFont(const FontProperties& value)override;
				void												SetVisuallyEnabled(bool value)override;
			};

/***********************************************************************
Item Template (GuiListItemTemplate)
***********************************************************************/

			class GuiListItemTemplate_ItemStyleProvider
				: public Object
				, public virtual controls::GuiSelectableListControl::IItemStyleProvider
				, public Description<GuiListItemTemplate_ItemStyleProvider>
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
				, public Description<GuiListItemTemplate_ItemStyleController>
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
				, protected virtual controls::tree::INodeProviderCallback
				, public Description<GuiTreeItemTemplate_ItemStyleProvider>
			{
			protected:
				Ptr<GuiTemplate::IFactory>							factory;
				controls::GuiVirtualTreeListControl*				treeListControl;
				controls::tree::INodeItemBindingView*				bindingView;
				controls::GuiListControl::IItemStyleProvider*		itemStyleProvider;
				
				void												UpdateExpandingButton(controls::tree::INodeProvider* node);
				void												OnAttached(controls::tree::INodeRootProvider* provider)override;
				void												OnBeforeItemModified(controls::tree::INodeProvider* parentNode, vint start, vint count, vint newCount)override;
				void												OnAfterItemModified(controls::tree::INodeProvider* parentNode, vint start, vint count, vint newCount)override;
				void												OnItemExpanded(controls::tree::INodeProvider* node)override;
				void												OnItemCollapsed(controls::tree::INodeProvider* node)override;

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
			
			class GuiTreeItemTemplate_ItemStyleController
				: public GuiListItemTemplate_ItemStyleController
				, public virtual controls::tree::INodeItemStyleController
				, public Description<GuiTreeItemTemplate_ItemStyleController>
			{
			protected:
				GuiTreeItemTemplate_ItemStyleProvider*				nodeStyleProvider;

			public:
				GuiTreeItemTemplate_ItemStyleController(GuiTreeItemTemplate_ItemStyleProvider* _nodeStyleProvider);
				~GuiTreeItemTemplate_ItemStyleController();
				
				controls::GuiListControl::IItemStyleProvider*		GetStyleProvider()override;
				controls::tree::INodeItemStyleProvider*				GetNodeStyleProvider()override;
			};

/***********************************************************************
Helper Functions
***********************************************************************/

#pragma warning(pop)
		}
	}
}

#endif