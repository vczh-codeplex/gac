#include "GuiControlTemplateStyles.h"

namespace vl
{
	namespace presentation
	{
		namespace templates
		{
			using namespace compositions;
			using namespace controls;

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
		}
	}
}