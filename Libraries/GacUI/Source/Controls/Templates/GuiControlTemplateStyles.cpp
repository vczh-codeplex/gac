#include "GuiControlTemplateStyles.h"

namespace vl
{
	namespace presentation
	{
		namespace templates
		{
			using namespace compositions;
			using namespace elements;
			using namespace controls;
			using namespace reflection::description;
			using namespace collections;

#define INITIALIZE_FACTORY_FROM_TEMPLATE(VARIABLE, PROPERTY)\
	controlTemplate->PROPERTY##Changed.AttachLambda([this](GuiGraphicsComposition*, GuiEventArgs&)\
	{\
		this->VARIABLE = 0;\
	});\

#define GET_FACTORY_FROM_TEMPLATE(TEMPLATE, VARIABLE, PROPERTY)\
	if (!this->VARIABLE)\
	{\
		this->VARIABLE = CreateTemplateFactory(controlTemplate->Get##PROPERTY());\
	}\
	return new TEMPLATE##_StyleProvider(this->VARIABLE);\

#define GET_FACTORY_FROM_TEMPLATE_OPT(TEMPLATE, VARIABLE, PROPERTY)\
	if (controlTemplate->Get##PROPERTY() == L"")\
	{\
		return 0;\
	}\
	GET_FACTORY_FROM_TEMPLATE(TEMPLATE, VARIABLE, PROPERTY)\

/***********************************************************************
GuiControlTemplate_StyleProvider
***********************************************************************/

			GuiControlTemplate_StyleProvider::GuiControlTemplate_StyleProvider(Ptr<GuiTemplate::IFactory> factory)
				:associatedStyleController(0)
				, controlTemplate(0)
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

			void GuiControlTemplate_StyleProvider::AssociateStyleController(controls::GuiControl::IStyleController* controller)
			{
				associatedStyleController = controller;
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
GuiSinglelineTextBoxTemplate_StyleProvider
***********************************************************************/

			GuiSinglelineTextBoxTemplate_StyleProvider::GuiSinglelineTextBoxTemplate_StyleProvider(Ptr<GuiTemplate::IFactory> factory)
				:GuiControlTemplate_StyleProvider(factory)
			{
				if (!(controlTemplate = dynamic_cast<GuiSinglelineTextBoxTemplate*>(GetBoundsComposition())))
				{
					CHECK_FAIL(L"GuiSinglelineTextBoxTemplate_StyleProvider::GuiSinglelineTextBoxTemplate_StyleProvider()#An instance of GuiSinglelineTextBoxTemplate is expected.");
				}
			}

			GuiSinglelineTextBoxTemplate_StyleProvider::~GuiSinglelineTextBoxTemplate_StyleProvider()
			{
			}
			
			void GuiSinglelineTextBoxTemplate_StyleProvider::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
				GuiControlTemplate_StyleProvider::SetFocusableComposition(value);
				if (auto style = dynamic_cast<GuiSinglelineTextBox::StyleController*>(associatedStyleController))
				{
					auto element = style->GetTextElement();
					Array<text::ColorEntry> colors(1);
					colors[0] = controlTemplate->GetTextColor();
					element->SetColors(colors);
					element->SetCaretColor(controlTemplate->GetCaretColor());
				}
			}

			compositions::GuiGraphicsComposition* GuiSinglelineTextBoxTemplate_StyleProvider::InstallBackground(compositions::GuiBoundsComposition* boundsComposition)
			{
				controlTemplate->SetAlignmentToParent(Margin(0, 0, 0, 0));
				boundsComposition->AddChild(controlTemplate);
				return controlTemplate->GetContainerComposition();
			}

/***********************************************************************
GuiMenuTemplate_StyleProvider
***********************************************************************/

			GuiMenuTemplate_StyleProvider::GuiMenuTemplate_StyleProvider(Ptr<GuiTemplate::IFactory> factory)
				:GuiControlTemplate_StyleProvider(factory)
			{
			}

			GuiMenuTemplate_StyleProvider::~GuiMenuTemplate_StyleProvider()
			{
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

			GuiToolstripButtonTemplate_StyleProvider::GuiToolstripButtonTemplate_StyleProvider(Ptr<GuiTemplate::IFactory> factory)
				:GuiSelectableButtonTemplate_StyleProvider(factory)
			{
				if (!(controlTemplate = dynamic_cast<GuiToolstripButtonTemplate*>(GetBoundsComposition())))
				{
					CHECK_FAIL(L"GuiButtonTemplate_StyleProvider::GuiButtonTemplate_StyleProvider()#An instance of GuiToolstripButtonTemplate is expected.");
				}
				INITIALIZE_FACTORY_FROM_TEMPLATE(subMenuTemplateFactory, SubMenuTemplate);
			}

			GuiToolstripButtonTemplate_StyleProvider::~GuiToolstripButtonTemplate_StyleProvider()
			{
			}
				
			controls::GuiMenu::IStyleController* GuiToolstripButtonTemplate_StyleProvider::CreateSubMenuStyleController()
			{
				GET_FACTORY_FROM_TEMPLATE(GuiMenuTemplate, subMenuTemplateFactory, SubMenuTemplate);
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

/***********************************************************************
GuiListViewColumnHeaderTemplate_StyleProvider
***********************************************************************/

			GuiListViewColumnHeaderTemplate_StyleProvider::GuiListViewColumnHeaderTemplate_StyleProvider(Ptr<GuiTemplate::IFactory> factory)
				:GuiToolstripButtonTemplate_StyleProvider(factory)
			{
				if (!(controlTemplate = dynamic_cast<GuiListViewColumnHeaderTemplate*>(GetBoundsComposition())))
				{
					CHECK_FAIL(L"GuiListViewColumnHeaderTemplate_StyleProvider::GuiListViewColumnHeaderTemplate_StyleProvider()#An instance of GuiListViewColumnHeaderTemplate is expected.");
				}
			}

			GuiListViewColumnHeaderTemplate_StyleProvider::~GuiListViewColumnHeaderTemplate_StyleProvider()
			{
			}

			void GuiListViewColumnHeaderTemplate_StyleProvider::SetColumnSortingState(controls::GuiListViewColumnHeader::ColumnSortingState value)
			{
				controlTemplate->SetSortingState(value);
			}

/***********************************************************************
GuiComboBoxTemplate_StyleProvider
***********************************************************************/

			GuiComboBoxTemplate_StyleProvider::GuiComboBoxTemplate_StyleProvider(Ptr<GuiTemplate::IFactory> factory)
				:GuiToolstripButtonTemplate_StyleProvider(factory)
			{
				if (!(controlTemplate = dynamic_cast<GuiComboBoxTemplate*>(GetBoundsComposition())))
				{
					CHECK_FAIL(L"GuiComboBoxTemplate_StyleProvider::GuiComboBoxTemplate_StyleProvider()#An instance of GuiComboBoxTemplate is expected.");
				}
			}

			GuiComboBoxTemplate_StyleProvider::~GuiComboBoxTemplate_StyleProvider()
			{
			}

			void GuiComboBoxTemplate_StyleProvider::SetCommandExecutor(controls::GuiComboBoxBase::ICommandExecutor* value)
			{
				controlTemplate->SetCommands(value);
			}

			void GuiComboBoxTemplate_StyleProvider::OnItemSelected()
			{
			}

/***********************************************************************
GuiDatePickerTemplate_StyleProvider
***********************************************************************/

			GuiDatePickerTemplate_StyleProvider::GuiDatePickerTemplate_StyleProvider(Ptr<GuiTemplate::IFactory> factory)
				:GuiControlTemplate_StyleProvider(factory)
			{
				if (!(controlTemplate = dynamic_cast<GuiDatePickerTemplate*>(GetBoundsComposition())))
				{
					CHECK_FAIL(L"GuiDatePickerTemplate_StyleProvider::GuiDatePickerTemplate_StyleProvider()#An instance of GuiDatePickerTemplate is expected.");
				}
				INITIALIZE_FACTORY_FROM_TEMPLATE(dateButtonTemplateFactory, DateButtonTemplate);
				INITIALIZE_FACTORY_FROM_TEMPLATE(dateTextListTemplateFactory, DateTextListTemplate);
				INITIALIZE_FACTORY_FROM_TEMPLATE(dateComboBoxTemplateFactory, DateComboBoxTemplate);
			}

			GuiDatePickerTemplate_StyleProvider::~GuiDatePickerTemplate_StyleProvider()
			{
				delete controlTemplate;
			}

			controls::GuiSelectableButton::IStyleController* GuiDatePickerTemplate_StyleProvider::CreateDateButtonStyle()
			{
				GET_FACTORY_FROM_TEMPLATE(GuiSelectableButtonTemplate, dateButtonTemplateFactory, DateButtonTemplate);
			}

			GuiTextListTemplate_StyleProvider* GuiDatePickerTemplate_StyleProvider::CreateTextListStyle()
			{
				GET_FACTORY_FROM_TEMPLATE(GuiTextListTemplate, dateTextListTemplateFactory, DateTextListTemplate);
			}

			controls::GuiTextList* GuiDatePickerTemplate_StyleProvider::CreateTextList()
			{
				auto style = CreateTextListStyle();
				return new GuiTextList(style, style->CreateArgument());
			}

			controls::GuiComboBoxListControl::IStyleController* GuiDatePickerTemplate_StyleProvider::CreateComboBoxStyle()
			{
				GET_FACTORY_FROM_TEMPLATE(GuiComboBoxTemplate, dateComboBoxTemplateFactory, DateComboBoxTemplate);
			}

			Color GuiDatePickerTemplate_StyleProvider::GetBackgroundColor()
			{
				return controlTemplate->GetBackgroundColor();
			}

			Color GuiDatePickerTemplate_StyleProvider::GetPrimaryTextColor()
			{
				return controlTemplate->GetPrimaryTextColor();
			}

			Color GuiDatePickerTemplate_StyleProvider::GetSecondaryTextColor()
			{
				return controlTemplate->GetSecondaryTextColor();
			}

/***********************************************************************
GuiDateComboBoxTemplate_StyleProvider
***********************************************************************/

			GuiDateComboBoxTemplate_StyleProvider::GuiDateComboBoxTemplate_StyleProvider(Ptr<GuiTemplate::IFactory> factory)
				:GuiComboBoxTemplate_StyleProvider(factory)
			{
				if (!(controlTemplate = dynamic_cast<GuiDateComboBoxTemplate*>(GetBoundsComposition())))
				{
					CHECK_FAIL(L"GuiDateComboBoxTemplate_StyleProvider::GuiDateComboBoxTemplate_StyleProvider()#An instance of GuiDateComboBoxTemplate is expected.");
				}
				INITIALIZE_FACTORY_FROM_TEMPLATE(datePickerTemplateFactory, DatePickerTemplate);
			}

			GuiDateComboBoxTemplate_StyleProvider::~GuiDateComboBoxTemplate_StyleProvider()
			{
			}

			controls::GuiDatePicker* GuiDateComboBoxTemplate_StyleProvider::CreateArgument()
			{
				return new GuiDatePicker(CreateDatePickerStyle());
			}

			controls::GuiDatePicker::IStyleProvider* GuiDateComboBoxTemplate_StyleProvider::CreateDatePickerStyle()
			{
				GET_FACTORY_FROM_TEMPLATE(GuiDatePickerTemplate, datePickerTemplateFactory, DatePickerTemplate);
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

			GuiScrollViewTemplate_StyleProvider::GuiScrollViewTemplate_StyleProvider(Ptr<GuiTemplate::IFactory> factory)
				:GuiControlTemplate_StyleProvider(factory)
			{
				if (!(controlTemplate = dynamic_cast<GuiScrollViewTemplate*>(GetBoundsComposition())))
				{
					CHECK_FAIL(L"GuiScrollViewTemplate_StyleProvider::GuiScrollViewTemplate_StyleProvider()#An instance of GuiScrollViewTemplate is expected.");
				}
				INITIALIZE_FACTORY_FROM_TEMPLATE(hScrollTemplateFactory, HScrollTemplate);
				INITIALIZE_FACTORY_FROM_TEMPLATE(vScrollTemplateFactory, VScrollTemplate);
			}

			GuiScrollViewTemplate_StyleProvider::~GuiScrollViewTemplate_StyleProvider()
			{
			}
				
			controls::GuiScroll::IStyleController* GuiScrollViewTemplate_StyleProvider::CreateHorizontalScrollStyle()
			{
				GET_FACTORY_FROM_TEMPLATE(GuiScrollTemplate, hScrollTemplateFactory, HScrollTemplate);
			}

			controls::GuiScroll::IStyleController* GuiScrollViewTemplate_StyleProvider::CreateVerticalScrollStyle()
			{
				GET_FACTORY_FROM_TEMPLATE(GuiScrollTemplate, vScrollTemplateFactory, VScrollTemplate);
			}

			vint GuiScrollViewTemplate_StyleProvider::GetDefaultScrollSize()
			{
				return controlTemplate->GetDefaultScrollSize();
			}

			compositions::GuiGraphicsComposition* GuiScrollViewTemplate_StyleProvider::InstallBackground(compositions::GuiBoundsComposition* boundsComposition)
			{
				controlTemplate->SetAlignmentToParent(Margin(0, 0, 0, 0));
				boundsComposition->AddChild(controlTemplate);
				return controlTemplate->GetContainerComposition();
			}

/***********************************************************************
GuiSinglelineTextBoxTemplate_StyleProvider
***********************************************************************/

			GuiMultilineTextBoxTemplate_StyleProvider::GuiMultilineTextBoxTemplate_StyleProvider(Ptr<GuiTemplate::IFactory> factory)
				:GuiScrollViewTemplate_StyleProvider(factory)
			{
				if (!(controlTemplate = dynamic_cast<GuiMultilineTextBoxTemplate*>(GetBoundsComposition())))
				{
					CHECK_FAIL(L"GuiMultilineTextBoxTemplate_StyleProvider::GuiMultilineTextBoxTemplate_StyleProvider()#An instance of GuiMultilineTextBoxTemplate is expected.");
				}
			}

			GuiMultilineTextBoxTemplate_StyleProvider::~GuiMultilineTextBoxTemplate_StyleProvider()
			{
			}
			
			void GuiMultilineTextBoxTemplate_StyleProvider::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
				GuiScrollViewTemplate_StyleProvider::SetFocusableComposition(value);
				if (auto style = dynamic_cast<GuiMultilineTextBox::StyleController*>(associatedStyleController))
				{
					auto element = style->GetTextElement();
					Array<text::ColorEntry> colors(1);
					colors[0] = controlTemplate->GetTextColor();
					element->SetColors(colors);
					element->SetCaretColor(controlTemplate->GetCaretColor());
				}
			}

/***********************************************************************
GuiTextListTemplate_StyleProvider::ItemStyleProvider
***********************************************************************/

			GuiTextListTemplate_StyleProvider::ItemStyleProvider::ItemStyleProvider(GuiTextListTemplate_StyleProvider* _styleProvider)
				:styleProvider(_styleProvider)
			{
			}

			GuiTextListTemplate_StyleProvider::ItemStyleProvider::~ItemStyleProvider()
			{
			}

			controls::GuiSelectableButton::IStyleController* GuiTextListTemplate_StyleProvider::ItemStyleProvider::CreateBackgroundStyleController()
			{
				return styleProvider->CreateBackgroundStyle();
			}

			controls::GuiSelectableButton::IStyleController* GuiTextListTemplate_StyleProvider::ItemStyleProvider::CreateBulletStyleController()
			{
				return styleProvider->CreateBulletStyle();
			}

			Color GuiTextListTemplate_StyleProvider::ItemStyleProvider::GetTextColor()
			{
				return styleProvider->controlTemplate->GetTextColor();
			}

/***********************************************************************
GuiTextListTemplate_StyleProvider
***********************************************************************/

			GuiTextListTemplate_StyleProvider::GuiTextListTemplate_StyleProvider(Ptr<GuiTemplate::IFactory> factory)
				:GuiScrollViewTemplate_StyleProvider(factory)
			{
				if (!(controlTemplate = dynamic_cast<GuiTextListTemplate*>(GetBoundsComposition())))
				{
					CHECK_FAIL(L"GuiTextListTemplate_StyleProvider::GuiTextListTemplate_StyleProvider()#An instance of GuiTextListTemplate is expected.");
				}
				INITIALIZE_FACTORY_FROM_TEMPLATE(backgroundTemplateFactory, BackgroundTemplate);
				INITIALIZE_FACTORY_FROM_TEMPLATE(bulletTemplateFactory, BulletTemplate);
			}

			GuiTextListTemplate_StyleProvider::~GuiTextListTemplate_StyleProvider()
			{
			}

			controls::list::TextItemStyleProvider::ITextItemStyleProvider* GuiTextListTemplate_StyleProvider::CreateArgument()
			{
				return new ItemStyleProvider(this);
			}

			controls::GuiSelectableButton::IStyleController* GuiTextListTemplate_StyleProvider::CreateBackgroundStyle()
			{
				GET_FACTORY_FROM_TEMPLATE(GuiSelectableButtonTemplate, backgroundTemplateFactory, BackgroundTemplate);
			}

			controls::GuiSelectableButton::IStyleController* GuiTextListTemplate_StyleProvider::CreateBulletStyle()
			{
				GET_FACTORY_FROM_TEMPLATE_OPT(GuiSelectableButtonTemplate, bulletTemplateFactory, BulletTemplate);
			}

/***********************************************************************
GuiListViewTemplate_StyleProvider
***********************************************************************/

			GuiListViewTemplate_StyleProvider::GuiListViewTemplate_StyleProvider(Ptr<GuiTemplate::IFactory> factory)
				:GuiScrollViewTemplate_StyleProvider(factory)
			{
				if (!(controlTemplate = dynamic_cast<GuiListViewTemplate*>(GetBoundsComposition())))
				{
					CHECK_FAIL(L"GuiListViewTemplate_StyleProvider::GuiListViewTemplate_StyleProvider()#An instance of GuiListViewTemplate is expected.");
				}
				INITIALIZE_FACTORY_FROM_TEMPLATE(backgroundTemplateFactory, BackgroundTemplate);
				INITIALIZE_FACTORY_FROM_TEMPLATE(columnHeaderTemplateFactory, ColumnHeaderTemplate);
			}

			GuiListViewTemplate_StyleProvider::~GuiListViewTemplate_StyleProvider()
			{
			}
				
			controls::GuiSelectableButton::IStyleController* GuiListViewTemplate_StyleProvider::CreateItemBackground()
			{
				GET_FACTORY_FROM_TEMPLATE(GuiSelectableButtonTemplate, backgroundTemplateFactory, BackgroundTemplate);
			}

			controls::GuiListViewColumnHeader::IStyleController* GuiListViewTemplate_StyleProvider::CreateColumnStyle()
			{
				GET_FACTORY_FROM_TEMPLATE(GuiListViewColumnHeaderTemplate, columnHeaderTemplateFactory, ColumnHeaderTemplate);
			}

			Color GuiListViewTemplate_StyleProvider::GetPrimaryTextColor()
			{
				return controlTemplate->GetPrimaryTextColor();
			}

			Color GuiListViewTemplate_StyleProvider::GetSecondaryTextColor()
			{
				return controlTemplate->GetSecondaryTextColor();
			}

			Color GuiListViewTemplate_StyleProvider::GetItemSeparatorColor()
			{
				return controlTemplate->GetItemSeparatorColor();
			}

/***********************************************************************
GuiTreeViewTemplate_StyleProvider
***********************************************************************/

			GuiTreeViewTemplate_StyleProvider::GuiTreeViewTemplate_StyleProvider(Ptr<GuiTemplate::IFactory> factory)
				:GuiScrollViewTemplate_StyleProvider(factory)
			{
				if (!(controlTemplate = dynamic_cast<GuiTreeViewTemplate*>(GetBoundsComposition())))
				{
					CHECK_FAIL(L"GuiTreeViewTemplate_StyleProvider::GuiTreeViewTemplate_StyleProvider()#An instance of GuiTreeViewTemplate is expected.");
				}
				INITIALIZE_FACTORY_FROM_TEMPLATE(backgroundTemplateFactory, BackgroundTemplate);
				INITIALIZE_FACTORY_FROM_TEMPLATE(expandingDecoratorTemplateFactory, ExpandingDecoratorTemplate);
			}

			GuiTreeViewTemplate_StyleProvider::~GuiTreeViewTemplate_StyleProvider()
			{
			}
				
			controls::GuiSelectableButton::IStyleController* GuiTreeViewTemplate_StyleProvider::CreateItemBackground()
			{
				GET_FACTORY_FROM_TEMPLATE(GuiSelectableButtonTemplate, backgroundTemplateFactory, BackgroundTemplate);
			}

			controls::GuiSelectableButton::IStyleController* GuiTreeViewTemplate_StyleProvider::CreateItemExpandingDecorator()
			{
				GET_FACTORY_FROM_TEMPLATE(GuiSelectableButtonTemplate, expandingDecoratorTemplateFactory, ExpandingDecoratorTemplate);
			}

			Color GuiTreeViewTemplate_StyleProvider::GetTextColor()
			{
				return controlTemplate->GetTextColor();
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