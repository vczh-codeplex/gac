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
			using namespace collections;

/***********************************************************************
GuiControlTemplate_StyleProvider
***********************************************************************/

			GuiControlTemplate_StyleProvider::GuiControlTemplate_StyleProvider(Ptr<GuiTemplate::IFactory> factory)
				:controlTemplate(0)
			{
				GuiTemplate* itemTemplate = factory->CreateTemplate(Value());
				if (!(controlTemplate = dynamic_cast<GuiControlTemplate*>(itemTemplate)))
				{
					delete itemTemplate;
					CHECK_FAIL(L"GuiControlTemplate_StyleProvider::GuiControlTemplate_StyleProvider()#An instance of GuiTemplate is expected.");
				}
			}

			GuiControlTemplate_StyleProvider::~GuiControlTemplate_StyleProvider()
			{
			}

			compositions::GuiBoundsComposition* GuiControlTemplate_StyleProvider::GetBoundsComposition()
			{
				return controlTemplate;
			}

			compositions::GuiGraphicsComposition* GuiControlTemplate_StyleProvider::GetContainerComposition()
			{
				return controlTemplate->GetContainerComposition();
			}

			void GuiControlTemplate_StyleProvider::AssociateStyleController(IStyleController* controller)
			{
			}

			void GuiControlTemplate_StyleProvider::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
				controlTemplate->SetFocusableComposition(value);
			}

			void GuiControlTemplate_StyleProvider::SetText(const WString& value)
			{
				controlTemplate->SetText(value);
			}

			void GuiControlTemplate_StyleProvider::SetFont(const FontProperties& value)
			{
				controlTemplate->SetFont(value);
			}

			void GuiControlTemplate_StyleProvider::SetVisuallyEnabled(bool value)
			{
				controlTemplate->SetVisuallyEnabled(value);
			}

/***********************************************************************
GuiLabelTemplate_StyleProvider
***********************************************************************/

			GuiLabelTemplate_StyleProvider::GuiLabelTemplate_StyleProvider(Ptr<GuiTemplate::IFactory> factory)
				:GuiControlTemplate_StyleProvider(factory)
			{
				if (!(controlTemplate = dynamic_cast<GuiLabelTemplate*>(GetBoundsComposition())))
				{
					CHECK_FAIL(L"GuiLabelTemplate_StyleProvider::GuiLabelTemplate_StyleProvider()#An instance of GuiLabelTemplate is expected.");
				}
			}

			GuiLabelTemplate_StyleProvider::~GuiLabelTemplate_StyleProvider()
			{
			}

			Color GuiLabelTemplate_StyleProvider::GetDefaultTextColor()
			{
				return controlTemplate->GetDefaultTextColor();
			}

			void GuiLabelTemplate_StyleProvider::SetTextColor(Color value)
			{
				controlTemplate->SetTextColor(value);
			}

/***********************************************************************
GuiWindowTemplate_StyleProvider
***********************************************************************/

			GuiWindowTemplate_StyleProvider::GuiWindowTemplate_StyleProvider(Ptr<GuiTemplate::IFactory> factory)
				:GuiControlTemplate_StyleProvider(factory)
			{
			}

			GuiWindowTemplate_StyleProvider::~GuiWindowTemplate_StyleProvider()
			{
			}

/***********************************************************************
GuiButtonTemplate_StyleProvider
***********************************************************************/

			GuiButtonTemplate_StyleProvider::GuiButtonTemplate_StyleProvider(Ptr<GuiTemplate::IFactory> factory)
				:GuiControlTemplate_StyleProvider(factory)
			{
				if (!(controlTemplate = dynamic_cast<GuiButtonTemplate*>(GetBoundsComposition())))
				{
					CHECK_FAIL(L"GuiButtonTemplate_StyleProvider::GuiButtonTemplate_StyleProvider()#An instance of GuiButtonTemplate is expected.");
				}
			}

			GuiButtonTemplate_StyleProvider::~GuiButtonTemplate_StyleProvider()
			{
			}

			void GuiButtonTemplate_StyleProvider::Transfer(controls::GuiButton::ControlState value)
			{
				controlTemplate->SetState(value);
			}

/***********************************************************************
GuiSelectableButtonTemplate_StyleProvider
***********************************************************************/

			GuiSelectableButtonTemplate_StyleProvider::GuiSelectableButtonTemplate_StyleProvider(Ptr<GuiTemplate::IFactory> factory)
				:GuiButtonTemplate_StyleProvider(factory)
			{
				if (!(controlTemplate = dynamic_cast<GuiSelectableButtonTemplate*>(GetBoundsComposition())))
				{
					CHECK_FAIL(L"GuiButtonTemplate_StyleProvider::GuiButtonTemplate_StyleProvider()#An instance of GuiSelectableButtonTemplate is expected.");
				}
			}

			GuiSelectableButtonTemplate_StyleProvider::~GuiSelectableButtonTemplate_StyleProvider()
			{
			}

			void GuiSelectableButtonTemplate_StyleProvider::SetSelected(bool value)
			{
				controlTemplate->SetSelected(value);
			}

/***********************************************************************
GuiToolstripButtonTemplate_StyleProvider
***********************************************************************/

			void GuiToolstripButtonTemplate_StyleProvider::controlTemplate_SubMenuTemplateChanged(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				subMenuTemplateFactory = 0;
			}

			GuiToolstripButtonTemplate_StyleProvider::GuiToolstripButtonTemplate_StyleProvider(Ptr<GuiTemplate::IFactory> factory)
				:GuiSelectableButtonTemplate_StyleProvider(factory)
			{
				if (!(controlTemplate = dynamic_cast<GuiToolstripButtonTemplate*>(GetBoundsComposition())))
				{
					CHECK_FAIL(L"GuiButtonTemplate_StyleProvider::GuiButtonTemplate_StyleProvider()#An instance of GuiSelectableButtonTemplate is expected.");
				}
				controlTemplate->SubMenuTemplateChanged.AttachMethod(this, &GuiToolstripButtonTemplate_StyleProvider::controlTemplate_SubMenuTemplateChanged);
			}

			GuiToolstripButtonTemplate_StyleProvider::~GuiToolstripButtonTemplate_StyleProvider()
			{
			}
				
			controls::GuiMenu::IStyleController* GuiToolstripButtonTemplate_StyleProvider::CreateSubMenuStyleController()
			{
				if (!subMenuTemplateFactory)
				{
					subMenuTemplateFactory = CreateTemplateFactory(controlTemplate->GetSubMenuTemplate());
				}
				return new GuiWindowTemplate_StyleProvider(subMenuTemplateFactory);
			}

			void GuiToolstripButtonTemplate_StyleProvider::SetSubMenuExisting(bool value)
			{
				controlTemplate->SetSubMenuExisting(value);
			}

			void GuiToolstripButtonTemplate_StyleProvider::SetSubMenuOpening(bool value)
			{
				controlTemplate->SetSubMenuOpening(value);
			}

			controls::GuiButton* GuiToolstripButtonTemplate_StyleProvider::GetSubMenuHost()
			{
				return controlTemplate->GetSubMenuHost();
			}

			void GuiToolstripButtonTemplate_StyleProvider::SetImage(Ptr<GuiImageData> value)
			{
				controlTemplate->SetImage(value);
			}

			void GuiToolstripButtonTemplate_StyleProvider::SetShortcutText(const WString& value)
			{
				controlTemplate->SetShortcutText(value);
			}

			compositions::GuiSubComponentMeasurer::IMeasuringSource* GuiToolstripButtonTemplate_StyleProvider::GetMeasuringSource()
			{
				return 0;
			}

/***********************************************************************
GuiScrollTemplate_StyleProvider
***********************************************************************/

			GuiScrollTemplate_StyleProvider::GuiScrollTemplate_StyleProvider(Ptr<GuiTemplate::IFactory> factory)
				:GuiControlTemplate_StyleProvider(factory)
			{
				if (!(controlTemplate = dynamic_cast<GuiScrollTemplate*>(GetBoundsComposition())))
				{
					CHECK_FAIL(L"GuiButtonTemplate_StyleProvider::GuiButtonTemplate_StyleProvider()#An instance of GuiScrollTemplate is expected.");
				}
			}

			GuiScrollTemplate_StyleProvider::~GuiScrollTemplate_StyleProvider()
			{
			}

			void GuiScrollTemplate_StyleProvider::SetCommandExecutor(controls::GuiScroll::ICommandExecutor* value)
			{
				controlTemplate->SetCommands(value);
			}

			void GuiScrollTemplate_StyleProvider::SetTotalSize(vint value)
			{
				controlTemplate->SetTotalSize(value);
			}

			void GuiScrollTemplate_StyleProvider::SetPageSize(vint value)
			{
				controlTemplate->SetPageSize(value);
			}

			void GuiScrollTemplate_StyleProvider::SetPosition(vint value)
			{
				controlTemplate->SetPosition(value);
			}

/***********************************************************************
GuiScrollViewTemplate_StyleProvider
***********************************************************************/

			void GuiScrollViewTemplate_StyleProvider::controlTemplate_HScrollTemplateChanged(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				hScrollTemplateFactory = 0;
			}

			void GuiScrollViewTemplate_StyleProvider::controlTemplate_VScrollTemplateChanged(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				vScrollTemplateFactory = 0;
			}

			GuiScrollViewTemplate_StyleProvider::GuiScrollViewTemplate_StyleProvider(Ptr<GuiTemplate::IFactory> factory)
				:GuiControlTemplate_StyleProvider(factory)
			{
				if (!(controlTemplate = dynamic_cast<GuiScrollViewTemplate*>(GetBoundsComposition())))
				{
					CHECK_FAIL(L"GuiScrollViewTemplate_StyleProvider::GuiScrollViewTemplate_StyleProvider()#An instance of GuiScrollViewTemplate is expected.");
				}
			}

			GuiScrollViewTemplate_StyleProvider::~GuiScrollViewTemplate_StyleProvider()
			{
			}
				
			controls::GuiScroll::IStyleController* GuiScrollViewTemplate_StyleProvider::CreateHorizontalScrollStyle()
			{
				if (!hScrollTemplateFactory)
				{
					hScrollTemplateFactory = CreateTemplateFactory(controlTemplate->GetHScrollTemplate());
				}
				return new GuiScrollTemplate_StyleProvider(hScrollTemplateFactory);
			}

			controls::GuiScroll::IStyleController* GuiScrollViewTemplate_StyleProvider::CreateVerticalScrollStyle()
			{
				if (!vScrollTemplateFactory)
				{
					vScrollTemplateFactory = CreateTemplateFactory(controlTemplate->GetVScrollTemplate());
				}
				return new GuiScrollTemplate_StyleProvider(vScrollTemplateFactory);
			}

			vint GuiScrollViewTemplate_StyleProvider::GetDefaultScrollSize()
			{
				return controlTemplate->GetDefaultScrollSize();
			}

			compositions::GuiGraphicsComposition* GuiScrollViewTemplate_StyleProvider::InstallBackground(compositions::GuiBoundsComposition* boundsComposition)
			{
				boundsComposition->AddChild(controlTemplate);
				return controlTemplate->GetContainerComposition();
			}

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

			void GuiTreeItemTemplate_ItemStyleProvider::UpdateExpandingButton(controls::tree::INodeProvider* node)
			{
				vint index=treeListControl->GetNodeItemView()->CalculateNodeVisibilityIndex(node);
				if(index!=-1)
				{
					if(auto style = treeListControl->GetArranger()->GetVisibleStyle(index))
					{
						if (auto controller = dynamic_cast<GuiTreeItemTemplate_ItemStyleController*>(style))
						{
							if (auto itemTemplate = dynamic_cast<GuiTreeItemTemplate*>(controller->GetTemplate()))
							{
								itemTemplate->SetExpanding(node->GetExpanding());
							}
						}
					}
				}
			}

			void GuiTreeItemTemplate_ItemStyleProvider::OnAttached(controls::tree::INodeRootProvider* provider)
			{
			}

			void GuiTreeItemTemplate_ItemStyleProvider::OnBeforeItemModified(controls::tree::INodeProvider* parentNode, vint start, vint count, vint newCount)
			{
			}

			void GuiTreeItemTemplate_ItemStyleProvider::OnAfterItemModified(controls::tree::INodeProvider* parentNode, vint start, vint count, vint newCount)
			{
				UpdateExpandingButton(parentNode);
			}

			void GuiTreeItemTemplate_ItemStyleProvider::OnItemExpanded(controls::tree::INodeProvider* node)
			{
				UpdateExpandingButton(node);
			}

			void GuiTreeItemTemplate_ItemStyleProvider::OnItemCollapsed(controls::tree::INodeProvider* node)
			{
				UpdateExpandingButton(node);
			}

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
					treeListControl->GetNodeRootProvider()->AttachCallback(this);
					bindingView = dynamic_cast<tree::INodeItemBindingView*>(treeListControl->GetNodeRootProvider()->RequestView(tree::INodeItemBindingView::Identifier));
				}
			}

			void GuiTreeItemTemplate_ItemStyleProvider::DetachListControl()
			{
				if (treeListControl)
				{
					treeListControl->GetNodeRootProvider()->DetachCallback(this);
					if (bindingView)
					{
						treeListControl->GetNodeRootProvider()->ReleaseView(bindingView);
					}
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

/***********************************************************************
Helper Functions
***********************************************************************/

			void SplitBySemicolon(const WString& input, collections::List<WString>& fragments)
			{
				const wchar_t* attValue = input.Buffer();
				while(*attValue)
				{
					// split the value by ';'
					const wchar_t* attSemicolon = wcschr(attValue, L';');
					WString pattern;
					if(attSemicolon)
					{
						pattern = WString(attValue, attSemicolon - attValue);
						attValue = attSemicolon + 1;
					}
					else
					{
						vint len = wcslen(attValue);
						pattern = WString(attValue, len);
						attValue += len;
					}

					fragments.Add(pattern);
				}
			}

			Ptr<GuiTemplate::IFactory> CreateTemplateFactory(const WString& typeValues)
			{
				List<ITypeDescriptor*> types;
				List<WString> typeNames;
				SplitBySemicolon(typeValues, typeNames);
				CopyFrom(
					types,
					From(typeNames)
						.Select(&description::GetTypeDescriptor)
						.Where([](ITypeDescriptor* type){return type != 0; })
					);

				return GuiTemplate::IFactory::CreateTemplateFactory(types);
			}
		}
	}
}