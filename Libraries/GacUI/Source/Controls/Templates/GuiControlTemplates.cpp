#include "GuiControlTemplates.h"

namespace vl
{
	namespace presentation
	{
		namespace templates
		{
			using namespace description;
			using namespace collections;
			using namespace controls;
			using namespace compositions;

/***********************************************************************
GuiTemplate::IFactory
***********************************************************************/

			class GuiTemplateReflectableFactory : public Object, public virtual GuiTemplate::IFactory
			{
			protected:
				List<ITypeDescriptor*>			types;

			public:
				GuiTemplateReflectableFactory(const List<ITypeDescriptor*>& _types)
				{
					CopyFrom(types, _types);
				}

				GuiTemplate* CreateTemplate(const description::Value& viewModel)override
				{
					FOREACH(ITypeDescriptor*, type, types)
					{
						auto group = type->GetConstructorGroup();
						vint count = group->GetMethodCount();
						for (vint i = 0; i < count; i++)
						{
							auto ctor = group->GetMethod(i);
							if (ctor->GetReturn()->GetDecorator() == ITypeInfo::RawPtr && ctor->GetParameterCount() <= 1)
							{
								Array<Value> arguments(ctor->GetParameterCount());
								if (ctor->GetParameterCount() == 1)
								{
									if (!viewModel.CanConvertTo(ctor->GetParameter(0)->GetType()))
									{
										continue;
									}
									arguments[0] = viewModel;
								}
								return dynamic_cast<GuiTemplate*>(ctor->Invoke(Value(), arguments).GetRawPtr());
							}
						}
					}
					
					WString message = L"Unable to create a template from types {";
					FOREACH_INDEXER(ITypeDescriptor*, type, index, types)
					{
						if (index > 0) message += L", ";
						message += type->GetTypeName();
					}
					message += L"} using view model: ";
					if (viewModel.IsNull())
					{
						message += L"null.";
					}
					else
					{
						message += viewModel.GetTypeDescriptor()->GetTypeName() + L".";
					}

					throw TypeDescriptorException(message);
				}
			};

			Ptr<GuiTemplate::IFactory> GuiTemplate::IFactory::CreateTemplateFactory(const collections::List<description::ITypeDescriptor*>& types)
			{
				return new GuiTemplateReflectableFactory(types);
			}

/***********************************************************************
GuiTemplate
***********************************************************************/

			GuiTemplate_PROPERTIES(GUI_TEMPLATE_PROPERTY_IMPL)

			GuiTemplate::GuiTemplate()
				:VisuallyEnabled_(true)
			{
				GuiTemplate_PROPERTIES(GUI_TEMPLATE_PROPERTY_EVENT_INIT)
			}

			GuiTemplate::~GuiTemplate()
			{
			}

/***********************************************************************
GuiControlTemplate
***********************************************************************/

			GuiControlTemplate_PROPERTIES(GUI_TEMPLATE_PROPERTY_IMPL)

			GuiControlTemplate::GuiControlTemplate()
				:ContainerComposition_(this)
				, FocusableComposition_(0)
			{
				GuiControlTemplate_PROPERTIES(GUI_TEMPLATE_PROPERTY_EVENT_INIT)
			}

			GuiControlTemplate::~GuiControlTemplate()
			{
			}

/***********************************************************************
GuiWindowTemplate
***********************************************************************/

			GuiWindowTemplate::GuiWindowTemplate()
			{
			}

			GuiWindowTemplate::~GuiWindowTemplate()
			{
			}

/***********************************************************************
GuiButtonTemplate
***********************************************************************/

			GuiButtonTemplate_PROPERTIES(GUI_TEMPLATE_PROPERTY_IMPL)

			GuiButtonTemplate::GuiButtonTemplate()
				:State_(GuiButton::Normal)
			{
				GuiButtonTemplate_PROPERTIES(GUI_TEMPLATE_PROPERTY_EVENT_INIT)
			}

			GuiButtonTemplate::~GuiButtonTemplate()
			{
			}

/***********************************************************************
GuiSelectableButtonTemplate
***********************************************************************/

			GuiSelectableButtonTemplate_PROPERTIES(GUI_TEMPLATE_PROPERTY_IMPL)

			GuiSelectableButtonTemplate::GuiSelectableButtonTemplate()
				:Selected_(false)
			{
				GuiSelectableButtonTemplate_PROPERTIES(GUI_TEMPLATE_PROPERTY_EVENT_INIT)
			}

			GuiSelectableButtonTemplate::~GuiSelectableButtonTemplate()
			{
			}

/***********************************************************************
GuiToolstripButtonTemplate
***********************************************************************/

			GuiToolstripButtonTemplate_PROPERTIES(GUI_TEMPLATE_PROPERTY_IMPL)

			GuiToolstripButtonTemplate::GuiToolstripButtonTemplate()
				:SubMenuExisting_(false)
				, SubMenuOpening_(false)
				, SubMenuHost_(0)
			{
				GuiToolstripButtonTemplate_PROPERTIES(GUI_TEMPLATE_PROPERTY_EVENT_INIT)
			}

			GuiToolstripButtonTemplate::~GuiToolstripButtonTemplate()
			{
			}

/***********************************************************************
GuiListItemTemplate
***********************************************************************/

			GuiListItemTemplate_PROPERTIES(GUI_TEMPLATE_PROPERTY_IMPL)

			GuiListItemTemplate::GuiListItemTemplate()
				:Selected_(false)
				, Index_(0)
			{
				GuiListItemTemplate_PROPERTIES(GUI_TEMPLATE_PROPERTY_EVENT_INIT)
			}

			GuiListItemTemplate::~GuiListItemTemplate()
			{
			}

/***********************************************************************
GuiTreeItemTemplate
***********************************************************************/

			GuiTreeItemTemplate_PROPERTIES(GUI_TEMPLATE_PROPERTY_IMPL)

			GuiTreeItemTemplate::GuiTreeItemTemplate()
				:Expanding_(false)
			{
				GuiTreeItemTemplate_PROPERTIES(GUI_TEMPLATE_PROPERTY_EVENT_INIT)
			}

			GuiTreeItemTemplate::~GuiTreeItemTemplate()
			{
			}
		}
	}
}