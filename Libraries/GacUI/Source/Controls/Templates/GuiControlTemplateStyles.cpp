#include "GuiControlTemplateStyles.h"

namespace vl
{
	namespace presentation
	{
		namespace templates
		{
			using namespace compositions;
			using namespace controls;
			using namespace reflection::description;

/***********************************************************************
GuiListItemTemplate_ItemStyleProvider
***********************************************************************/

			GuiListItemTemplate_ItemStyleProvider::GuiListItemTemplate_ItemStyleProvider(Ptr<GuiTemplate::IFactory> _factory)
				:factory(_factory)
				, listControl(0)
				, bindingView(0)
			{
			}

			GuiListItemTemplate_ItemStyleProvider::~GuiListItemTemplate_ItemStyleProvider()
			{
			}

			void GuiListItemTemplate_ItemStyleProvider::AttachListControl(controls::GuiListControl* value)
			{
				listControl = value;
				bindingView = dynamic_cast<GuiListControl::IItemBindingView*>(listControl->GetItemProvider()->RequestView(GuiListControl::IItemBindingView::Identifier));
			}

			void GuiListItemTemplate_ItemStyleProvider::DetachListControl()
			{
				if (listControl && bindingView)
				{
					listControl->GetItemProvider()->ReleaseView(bindingView);
				}
				listControl = 0;
				bindingView = 0;
			}

			vint GuiListItemTemplate_ItemStyleProvider::GetItemStyleId(vint itemIndex)
			{
				return 0;
			}

			controls::GuiListControl::IItemStyleController* GuiListItemTemplate_ItemStyleProvider::CreateItemStyle(vint styleId)
			{
				return new GuiListItemTemplate_ItemStyleController(this);
			}

			void GuiListItemTemplate_ItemStyleProvider::DestroyItemStyle(controls::GuiListControl::IItemStyleController* style)
			{
				delete style;
			}

			void GuiListItemTemplate_ItemStyleProvider::Install(controls::GuiListControl::IItemStyleController* style, vint itemIndex)
			{
				if (auto controller = dynamic_cast<GuiListItemTemplate_ItemStyleController*>(style))
				{
					Value viewModel;
					if (bindingView)
					{
						viewModel = bindingView->GetBindingValue(itemIndex);
					}
					
					GuiTemplate* itemTemplate = factory->CreateTemplate(viewModel);
					if (auto listItemTemplate = dynamic_cast<GuiListItemTemplate*>(itemTemplate))
					{
						listItemTemplate->SetIndex(itemIndex);
						controller->SetTemplate(listItemTemplate);
					}
					else
					{
						delete itemTemplate;
					}
				}
			}

			void GuiListItemTemplate_ItemStyleProvider::SetStyleIndex(controls::GuiListControl::IItemStyleController* style, vint value)
			{
				if (auto controller = dynamic_cast<GuiListItemTemplate_ItemStyleController*>(style))
				{
					if (auto itemTemplate = controller->GetTemplate())
					{
						itemTemplate->SetIndex(value);
					}
				}
			}

			void GuiListItemTemplate_ItemStyleProvider::SetStyleSelected(controls::GuiListControl::IItemStyleController* style, bool value)
			{
				if (auto controller = dynamic_cast<GuiListItemTemplate_ItemStyleController*>(style))
				{
					if (auto itemTemplate = controller->GetTemplate())
					{
						itemTemplate->SetSelected(value);
					}
				}
			}

/***********************************************************************
GuiListItemTemplate_ItemStyleController
***********************************************************************/

			GuiListItemTemplate_ItemStyleController::GuiListItemTemplate_ItemStyleController(GuiListItemTemplate_ItemStyleProvider* _itemStyleProvider)
				:itemStyleProvider(_itemStyleProvider)
				, itemTemplate(0)
				, installed(false)
			{

			}

			GuiListItemTemplate_ItemStyleController::~GuiListItemTemplate_ItemStyleController()
			{
				SafeDeleteComposition(itemTemplate);
			}

			GuiListItemTemplate* GuiListItemTemplate_ItemStyleController::GetTemplate()
			{
				return itemTemplate;
			}

			void GuiListItemTemplate_ItemStyleController::SetTemplate(GuiListItemTemplate* _itemTemplate)
			{
				SafeDeleteComposition(itemTemplate);
				itemTemplate = _itemTemplate;
			}

			controls::GuiListControl::IItemStyleProvider* GuiListItemTemplate_ItemStyleController::GetStyleProvider()
			{
				return itemStyleProvider;
			}

			vint GuiListItemTemplate_ItemStyleController::GetItemStyleId()
			{
				return 0;
			}

			compositions::GuiBoundsComposition* GuiListItemTemplate_ItemStyleController::GetBoundsComposition()
			{
				return itemTemplate;
			}

			bool GuiListItemTemplate_ItemStyleController::IsCacheable()
			{
				return false;
			}

			bool GuiListItemTemplate_ItemStyleController::IsInstalled()
			{
				return installed;
			}

			void GuiListItemTemplate_ItemStyleController::OnInstalled()
			{
				installed = true;
			}

			void GuiListItemTemplate_ItemStyleController::OnUninstalled()
			{
				installed = false;
			}

/***********************************************************************
GuiTreeItemTemplate_ItemStyleProvider
***********************************************************************/

			GuiTreeItemTemplate_ItemStyleProvider::GuiTreeItemTemplate_ItemStyleProvider(Ptr<GuiTemplate::IFactory> _factory)
				:factory(_factory)
				, treeListControl(0)
				, bindingView(0)
				, itemStyleProvider(0)
			{

			}

			GuiTreeItemTemplate_ItemStyleProvider::~GuiTreeItemTemplate_ItemStyleProvider()
			{
			}
				
			void GuiTreeItemTemplate_ItemStyleProvider::BindItemStyleProvider(controls::GuiListControl::IItemStyleProvider* styleProvider)
			{
				itemStyleProvider = styleProvider;
			}

			controls::GuiListControl::IItemStyleProvider* GuiTreeItemTemplate_ItemStyleProvider::GetBindedItemStyleProvider()
			{
				return itemStyleProvider;
			}

			void GuiTreeItemTemplate_ItemStyleProvider::AttachListControl(GuiListControl* value)
			{
				treeListControl = dynamic_cast<GuiVirtualTreeListControl*>(value);
				if (treeListControl)
				{
					bindingView = dynamic_cast<tree::INodeItemBindingView*>(treeListControl->GetNodeRootProvider()->RequestView(tree::INodeItemBindingView::Identifier));
				}
			}

			void GuiTreeItemTemplate_ItemStyleProvider::DetachListControl()
			{
				if (treeListControl && bindingView)
				{
					treeListControl->GetNodeRootProvider()->ReleaseView(bindingView);
				}
				treeListControl = 0;
				bindingView = 0;
			}

			vint GuiTreeItemTemplate_ItemStyleProvider::GetItemStyleId(controls::tree::INodeProvider* node)
			{
				return 0;
			}

			controls::tree::INodeItemStyleController* GuiTreeItemTemplate_ItemStyleProvider::CreateItemStyle(vint styleId)
			{
				return new GuiTreeItemTemplate_ItemStyleController(this);
			}

			void GuiTreeItemTemplate_ItemStyleProvider::DestroyItemStyle(controls::tree::INodeItemStyleController* style)
			{
				delete style;
			}

			void GuiTreeItemTemplate_ItemStyleProvider::Install(controls::tree::INodeItemStyleController* style, controls::tree::INodeProvider* node, vint itemIndex)
			{
				if (auto controller = dynamic_cast<GuiTreeItemTemplate_ItemStyleController*>(style))
				{
					Value viewModel;
					if (bindingView)
					{
						viewModel = bindingView->GetBindingValue(node);
					}
					
					GuiTemplate* itemTemplate = factory->CreateTemplate(viewModel);
					if (auto treeItemTemplate = dynamic_cast<GuiTreeItemTemplate*>(itemTemplate))
					{
						treeItemTemplate->SetIndex(itemIndex);
						controller->SetTemplate(treeItemTemplate);
					}
					else
					{
						delete itemTemplate;
					}
				}
			}

			void GuiTreeItemTemplate_ItemStyleProvider::SetStyleIndex(controls::tree::INodeItemStyleController* style, vint value)
			{
				if (auto controller = dynamic_cast<GuiTreeItemTemplate_ItemStyleController*>(style))
				{
					if (auto itemTemplate = controller->GetTemplate())
					{
						itemTemplate->SetIndex(value);
					}
				}
			}

			void GuiTreeItemTemplate_ItemStyleProvider::SetStyleSelected(controls::tree::INodeItemStyleController* style, bool value)
			{
				if (auto controller = dynamic_cast<GuiTreeItemTemplate_ItemStyleController*>(style))
				{
					if (auto itemTemplate = controller->GetTemplate())
					{
						itemTemplate->SetSelected(value);
					}
				}
			}

/***********************************************************************
GuiTreeItemTemplate_ItemStyleController
***********************************************************************/

			GuiTreeItemTemplate_ItemStyleController::GuiTreeItemTemplate_ItemStyleController(GuiTreeItemTemplate_ItemStyleProvider* _nodeStyleProvider)
				:GuiListItemTemplate_ItemStyleController(0)
				, nodeStyleProvider(_nodeStyleProvider)
			{

			}

			GuiTreeItemTemplate_ItemStyleController::~GuiTreeItemTemplate_ItemStyleController()
			{

			}

			controls::GuiListControl::IItemStyleProvider* GuiTreeItemTemplate_ItemStyleController::GetStyleProvider()
			{
				return nodeStyleProvider->GetBindedItemStyleProvider();
			}

			controls::tree::INodeItemStyleProvider* GuiTreeItemTemplate_ItemStyleController::GetNodeStyleProvider()
			{
				return nodeStyleProvider;
			}
		}
	}
}