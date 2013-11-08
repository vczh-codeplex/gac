#include "GuiInstanceLoader.h"
#include "GuiInstanceHelperTypes.h"
#include "TypeDescriptors\GuiReflectionControls.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;
		using namespace reflection::description;
		using namespace controls;
		using namespace compositions;
		using namespace theme;
		using namespace helper_types;

#ifndef VCZH_DEBUG_NO_REFLECTION

/***********************************************************************
GuiRewriteInstanceLoader
***********************************************************************/

		class GuiRewriteInstanceLoader : public Object, public IGuiInstanceLoader
		{
		public:
			GuiRewriteInstanceLoader()
			{
			}

			bool IsDeserializable(const TypeInfo& typeInfo)override
			{
				return false;
			}

			description::Value Deserialize(const TypeInfo& typeInfo, const WString& text)override
			{
				return Value();
			}

			description::Value CreateInstance(const TypeInfo& typeInfo)override
			{
				return Value();
			}

			IGuiInstanceLoader::PropertyType GetPropertyType(const PropertyInfo& propertyInfo, collections::List<description::ITypeDescriptor*>& acceptableTypes)override
			{
				return IGuiInstanceLoader::HandleByParentLoader;
			}

			bool GetPropertyValue(PropertyValue& propertyValue)override
			{
				return false;
			}

			bool SetPropertyValue(PropertyValue& propertyValue, vint currentIndex)override
			{
				return false;
			}
		};

/***********************************************************************
GuiVrtualTypeInstanceLoader
***********************************************************************/

		class GuiVrtualTypeInstanceLoader : public GuiRewriteInstanceLoader
		{
		protected:
			WString							typeName;
			Func<Value()>					constructor;
		public:
			GuiVrtualTypeInstanceLoader(const WString& _typeName, const Func<Value()>& _constructor)
				:typeName(_typeName)
				,constructor(_constructor)
			{
			}

			WString GetTypeName()override
			{
				return typeName;
			}

			description::Value CreateInstance(const TypeInfo& typeInfo)override
			{
				if(typeName==typeInfo.typeName)
				{
					return constructor();
				}
				return Value();
			}
		};

/***********************************************************************
GuiControlInstanceLoader
***********************************************************************/

		class GuiControlInstanceLoader : public GuiRewriteInstanceLoader
		{
		public:
			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiControl>()->GetTypeName();
			}

			IGuiInstanceLoader::PropertyType GetPropertyType(const PropertyInfo& propertyInfo, collections::List<description::ITypeDescriptor*>& acceptableTypes)override
			{
				if (propertyInfo.propertyName == L"")
				{
					acceptableTypes.Add(description::GetTypeDescriptor<GuiControl>());
					acceptableTypes.Add(description::GetTypeDescriptor<GuiGraphicsComposition>());
					return IGuiInstanceLoader::SupportedProperty;
				}
				return IGuiInstanceLoader::HandleByParentLoader;
			}

			bool SetPropertyValue(PropertyValue& propertyValue, vint currentIndex)override
			{
				if (GuiControl* container = dynamic_cast<GuiControl*>(propertyValue.instanceValue.GetRawPtr()))
				{
					if (propertyValue.propertyName == L"")
					{
						if (auto control = dynamic_cast<GuiControl*>(propertyValue.propertyValue.GetRawPtr()))
						{
							container->AddChild(control);
							return true;
						}
						else if (auto composition = dynamic_cast<GuiGraphicsComposition*>(propertyValue.propertyValue.GetRawPtr()))
						{
							container->GetBoundsComposition()->AddChild(composition);
							return true;
						}
					}
				}
				return false;
			}
		};

/***********************************************************************
GuiControlHostInstanceLoader
***********************************************************************/

		class GuiControlHostInstanceLoader : public GuiRewriteInstanceLoader
		{
		public:
			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiControlHost>()->GetTypeName();
			}

			IGuiInstanceLoader::PropertyType GetPropertyType(const PropertyInfo& propertyInfo, collections::List<description::ITypeDescriptor*>& acceptableTypes)override
			{
				if (propertyInfo.propertyName == L"")
				{
					acceptableTypes.Add(description::GetTypeDescriptor<GuiComponent>());
					return (IGuiInstanceLoader::PropertyType)(IGuiInstanceLoader::SupportedProperty | IGuiInstanceLoader::HandleByParentLoader);
				}
				return IGuiInstanceLoader::HandleByParentLoader;
			}

			bool SetPropertyValue(PropertyValue& propertyValue, vint currentIndex)override
			{
				if (GuiControlHost* container = dynamic_cast<GuiControlHost*>(propertyValue.instanceValue.GetRawPtr()))
				{
					if (propertyValue.propertyName == L"")
					{
						if (auto component = dynamic_cast<GuiComponent*>(propertyValue.propertyValue.GetRawPtr()))
						{
							container->AddComponent(component);
							return true;
						}
					}
				}
				return false;
			}
		};

/***********************************************************************
GuiTabInstanceLoader
***********************************************************************/

		class GuiTabInstanceLoader : public GuiRewriteInstanceLoader
		{
		public:
			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiTab>()->GetTypeName();
			}

			IGuiInstanceLoader::PropertyType GetPropertyType(const PropertyInfo& propertyInfo, collections::List<description::ITypeDescriptor*>& acceptableTypes)override
			{
				if (propertyInfo.propertyName == L"")
				{
					acceptableTypes.Add(description::GetTypeDescriptor<GuiTabPage>());
					return (IGuiInstanceLoader::PropertyType)(IGuiInstanceLoader::SupportedProperty | IGuiInstanceLoader::HandleByParentLoader);
				}
				return IGuiInstanceLoader::HandleByParentLoader;
			}

			bool SetPropertyValue(PropertyValue& propertyValue, vint currentIndex)override
			{
				if (GuiTab* container = dynamic_cast<GuiTab*>(propertyValue.instanceValue.GetRawPtr()))
				{
					if (propertyValue.propertyName == L"")
					{
						if (auto tabPage = dynamic_cast<GuiTabPage*>(propertyValue.propertyValue.GetRawPtr()))
						{
							container->CreatePage(tabPage);
							return true;
						}
					}
				}
				return false;
			}
		};

/***********************************************************************
GuiTabPageInstanceLoader
***********************************************************************/

		class GuiTabPageInstanceLoader : public GuiRewriteInstanceLoader
		{
		public:
			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiTabPage>()->GetTypeName();
			}

			IGuiInstanceLoader::PropertyType GetPropertyType(const PropertyInfo& propertyInfo, collections::List<description::ITypeDescriptor*>& acceptableTypes)override
			{
				if (propertyInfo.propertyName == L"")
				{
					acceptableTypes.Add(description::GetTypeDescriptor<GuiControl>());
					acceptableTypes.Add(description::GetTypeDescriptor<GuiGraphicsComposition>());
					return IGuiInstanceLoader::SupportedProperty;
				}
				return IGuiInstanceLoader::HandleByParentLoader;
			}

			bool SetPropertyValue(PropertyValue& propertyValue, vint currentIndex)override
			{
				if (GuiTabPage* container = dynamic_cast<GuiTabPage*>(propertyValue.instanceValue.GetRawPtr()))
				{
					if (propertyValue.propertyName == L"")
					{
						if (auto control = dynamic_cast<GuiControl*>(propertyValue.propertyValue.GetRawPtr()))
						{
							container->GetContainerComposition()->AddChild(control->GetBoundsComposition());
							return true;
						}
						else if (auto composition = dynamic_cast<GuiGraphicsComposition*>(propertyValue.propertyValue.GetRawPtr()))
						{
							container->GetContainerComposition()->AddChild(composition);
							return true;
						}
					}
				}
				return false;
			}
		};

/***********************************************************************
GuiToolstripMenuInstanceLoader
***********************************************************************/

		class GuiToolstripMenuInstanceLoader : public GuiRewriteInstanceLoader
		{
		public:
			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiToolstripMenu>()->GetTypeName();
			}

			IGuiInstanceLoader::PropertyType GetPropertyType(const PropertyInfo& propertyInfo, collections::List<description::ITypeDescriptor*>& acceptableTypes)override
			{
				if (propertyInfo.propertyName == L"")
				{
					acceptableTypes.Add(description::GetTypeDescriptor<GuiControl>());
					return (IGuiInstanceLoader::PropertyType)(IGuiInstanceLoader::SupportedProperty | IGuiInstanceLoader::HandleByParentLoader);
				}
				return IGuiInstanceLoader::HandleByParentLoader;
			}

			bool SetPropertyValue(PropertyValue& propertyValue, vint currentIndex)override
			{
				if (GuiToolstripMenu* container = dynamic_cast<GuiToolstripMenu*>(propertyValue.instanceValue.GetRawPtr()))
				{
					if (propertyValue.propertyName == L"")
					{
						if (auto control = dynamic_cast<GuiControl*>(propertyValue.propertyValue.GetRawPtr()))
						{
							container->GetToolstripItems().Add(control);
							return true;
						}
					}
				}
				return false;
			}
		};

/***********************************************************************
GuiToolstripMenuBarInstanceLoader
***********************************************************************/

		class GuiToolstripMenuBarInstanceLoader : public GuiRewriteInstanceLoader
		{
		public:
			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiToolstripMenuBar>()->GetTypeName();
			}

			IGuiInstanceLoader::PropertyType GetPropertyType(const PropertyInfo& propertyInfo, collections::List<description::ITypeDescriptor*>& acceptableTypes)override
			{
				if (propertyInfo.propertyName == L"")
				{
					acceptableTypes.Add(description::GetTypeDescriptor<GuiControl>());
					return (IGuiInstanceLoader::PropertyType)(IGuiInstanceLoader::SupportedProperty | IGuiInstanceLoader::HandleByParentLoader);
				}
				return IGuiInstanceLoader::HandleByParentLoader;
			}

			bool SetPropertyValue(PropertyValue& propertyValue, vint currentIndex)override
			{
				if (GuiToolstripMenuBar* container = dynamic_cast<GuiToolstripMenuBar*>(propertyValue.instanceValue.GetRawPtr()))
				{
					if (propertyValue.propertyName == L"")
					{
						if (auto control = dynamic_cast<GuiControl*>(propertyValue.propertyValue.GetRawPtr()))
						{
							container->GetToolstripItems().Add(control);
							return true;
						}
					}
				}
				return false;
			}
		};

/***********************************************************************
GuiToolstripToolBarInstanceLoader
***********************************************************************/

		class GuiToolstripToolBarInstanceLoader : public GuiRewriteInstanceLoader
		{
		public:
			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiToolstripToolBar>()->GetTypeName();
			}

			IGuiInstanceLoader::PropertyType GetPropertyType(const PropertyInfo& propertyInfo, collections::List<description::ITypeDescriptor*>& acceptableTypes)override
			{
				if (propertyInfo.propertyName == L"")
				{
					acceptableTypes.Add(description::GetTypeDescriptor<GuiControl>());
					return (IGuiInstanceLoader::PropertyType)(IGuiInstanceLoader::SupportedProperty | IGuiInstanceLoader::HandleByParentLoader);
				}
				return IGuiInstanceLoader::HandleByParentLoader;
			}

			bool SetPropertyValue(PropertyValue& propertyValue, vint currentIndex)override
			{
				if (GuiToolstripToolBar* container = dynamic_cast<GuiToolstripToolBar*>(propertyValue.instanceValue.GetRawPtr()))
				{
					if (propertyValue.propertyName == L"")
					{
						if (auto control = dynamic_cast<GuiControl*>(propertyValue.propertyValue.GetRawPtr()))
						{
							container->GetToolstripItems().Add(control);
							return true;
						}
					}
				}
				return false;
			}
		};

/***********************************************************************
GuiToolstripButtonInstanceLoader
***********************************************************************/

		class GuiToolstripButtonInstanceLoader : public GuiRewriteInstanceLoader
		{
		public:
			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiToolstripButton>()->GetTypeName();
			}

			description::Value CreateInstance(const TypeInfo& typeInfo)override
			{
				if (typeInfo.typeName == GetTypeName())
				{
					return Value::From(g::NewToolBarButton());
				}
				return Value();
			}

			IGuiInstanceLoader::PropertyType GetPropertyType(const PropertyInfo& propertyInfo, collections::List<description::ITypeDescriptor*>& acceptableTypes)override
			{
				if (propertyInfo.propertyName == L"SubMenu")
				{
					acceptableTypes.Add(description::GetTypeDescriptor<GuiToolstripMenu>());
					return IGuiInstanceLoader::SupportedProperty;
				}
				return IGuiInstanceLoader::HandleByParentLoader;
			}

			bool GetPropertyValue(PropertyValue& propertyValue)override
			{
				if (GuiToolstripButton* container = dynamic_cast<GuiToolstripButton*>(propertyValue.instanceValue.GetRawPtr()))
				{
					if (propertyValue.propertyName == L"SubMenu")
					{
						if (!container->GetToolstripSubMenu())
						{
							container->CreateToolstripSubMenu();
						}
						propertyValue.propertyValue = Value::From(container->GetToolstripSubMenu());
						return true;
					}
				}
				return false;
			}
		};

/***********************************************************************
GuiCompositionInstanceLoader
***********************************************************************/

		class GuiCompositionInstanceLoader : public GuiRewriteInstanceLoader
		{
		public:
			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiGraphicsComposition>()->GetTypeName();
			}

			IGuiInstanceLoader::PropertyType GetPropertyType(const PropertyInfo& propertyInfo, collections::List<description::ITypeDescriptor*>& acceptableTypes)override
			{
				if (propertyInfo.propertyName == L"")
				{
					acceptableTypes.Add(description::GetTypeDescriptor<GuiControl>());
					acceptableTypes.Add(description::GetTypeDescriptor<GuiGraphicsComposition>());
					acceptableTypes.Add(description::GetTypeDescriptor<IGuiGraphicsElement>());
					return IGuiInstanceLoader::SupportedProperty;
				}
				return IGuiInstanceLoader::HandleByParentLoader;
			}

			bool SetPropertyValue(PropertyValue& propertyValue, vint currentIndex)override
			{
				if (GuiGraphicsComposition* container = dynamic_cast<GuiGraphicsComposition*>(propertyValue.instanceValue.GetRawPtr()))
				{
					if (propertyValue.propertyName == L"")
					{
						if (auto control = dynamic_cast<GuiControl*>(propertyValue.propertyValue.GetRawPtr()))
						{
							container->AddChild(control->GetBoundsComposition());
							return true;
						}
						else if(auto composition = dynamic_cast<GuiGraphicsComposition*>(propertyValue.propertyValue.GetRawPtr()))
						{
							container->AddChild(composition);
							return true;
						}
						else if (Ptr<IGuiGraphicsElement> element = propertyValue.propertyValue.GetSharedPtr().Cast<IGuiGraphicsElement>())
						{
							container->SetOwnedElement(element);
							return true;
						}
					}
				}
				return false;
			}
		};

/***********************************************************************
GuiTableCompositionInstanceLoader
***********************************************************************/

		class GuiTableCompositionInstanceLoader : public GuiRewriteInstanceLoader
		{
		public:
			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiTableComposition>()->GetTypeName();
			}

			IGuiInstanceLoader::PropertyType GetPropertyType(const PropertyInfo& propertyInfo, collections::List<description::ITypeDescriptor*>& acceptableTypes)override
			{
				if (propertyInfo.propertyName == L"Rows" || propertyInfo.propertyName==L"Columns")
				{
					acceptableTypes.Add(description::GetTypeDescriptor<GuiCellOption>());
					return IGuiInstanceLoader::SupportedProperty;
				}
				return IGuiInstanceLoader::HandleByParentLoader;
			}

			bool SetPropertyValue(PropertyValue& propertyValue, vint currentIndex)override
			{
				if (GuiTableComposition* container = dynamic_cast<GuiTableComposition*>(propertyValue.instanceValue.GetRawPtr()))
				{
					if (propertyValue.propertyName == L"Rows")
					{
						GuiCellOption option = UnboxValue<GuiCellOption>(propertyValue.propertyValue);
						container->SetRowsAndColumns(currentIndex + 1, container->GetColumns());
						container->SetRowOption(currentIndex, option);
						return true;
					}
					else if (propertyValue.propertyName == L"Columns")
					{
						GuiCellOption option = UnboxValue<GuiCellOption>(propertyValue.propertyValue);
						container->SetRowsAndColumns(container->GetRows(), currentIndex + 1);
						container->SetColumnOption(currentIndex, option);
						return true;
					}
				}
				return false;
			}
		};

/***********************************************************************
GuiCellCompositionInstanceLoader
***********************************************************************/

		class GuiCellCompositionInstanceLoader : public GuiRewriteInstanceLoader
		{
		public:

			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiCellComposition>()->GetTypeName();
			}

			IGuiInstanceLoader::PropertyType GetPropertyType(const PropertyInfo& propertyInfo, collections::List<description::ITypeDescriptor*>& acceptableTypes)override
			{
				if (propertyInfo.propertyName == L"Site")
				{
					acceptableTypes.Add(description::GetTypeDescriptor<SiteValue>());
					return IGuiInstanceLoader::SupportedProperty;
				}
				return IGuiInstanceLoader::HandleByParentLoader;
			}

			bool SetPropertyValue(PropertyValue& propertyValue, vint currentIndex)override
			{
				if (GuiCellComposition* container = dynamic_cast<GuiCellComposition*>(propertyValue.instanceValue.GetRawPtr()))
				{
					if (propertyValue.propertyName == L"Site")
					{
						SiteValue site = UnboxValue<SiteValue>(propertyValue.propertyValue);
						container->SetSite(site.row, site.column, site.rowSpan, site.columnSpan);
						return true;
					}
				}
				return false;
			}
		};

#endif

/***********************************************************************
GuiPredefinedInstanceLoadersPlugin
***********************************************************************/

		class GuiPredefinedInstanceLoadersPlugin : public Object, public IGuiPlugin
		{
		public:
			void Load()override
			{
			}

			void AfterLoad()override
			{
#ifndef VCZH_DEBUG_NO_REFLECTION
				IGuiInstanceLoaderManager* manager=GetInstanceLoaderManager();

				manager->SetLoader(new GuiControlInstanceLoader);
				manager->SetLoader(new GuiControlHostInstanceLoader);
				manager->SetLoader(new GuiTabInstanceLoader);
				manager->SetLoader(new GuiTabPageInstanceLoader);
				manager->SetLoader(new GuiToolstripMenuInstanceLoader);
				manager->SetLoader(new GuiToolstripMenuBarInstanceLoader);
				manager->SetLoader(new GuiToolstripToolBarInstanceLoader);
				manager->SetLoader(new GuiToolstripButtonInstanceLoader);
				manager->SetLoader(new GuiCompositionInstanceLoader);
				manager->SetLoader(new GuiTableCompositionInstanceLoader);
				manager->SetLoader(new GuiCellCompositionInstanceLoader);

#define ADD_VIRTUAL_TYPE(VIRTUALTYPENAME, TYPENAME, CONSTRUCTOR)\
	manager->CreateVirtualType(\
		L"presentation::controls::Gui" L#VIRTUALTYPENAME,\
		description::GetTypeDescriptor<TYPENAME>()->GetTypeName(),\
		new GuiVrtualTypeInstanceLoader(\
			L"presentation::controls::Gui" L#VIRTUALTYPENAME,\
			[](){return Value::From(CONSTRUCTOR());})\
		)

				ADD_VIRTUAL_TYPE(GroupBox,					GuiControl,				g::NewGroupBox);
				ADD_VIRTUAL_TYPE(MenuSplitter,				GuiControl,				g::NewMenuSplitter);
				ADD_VIRTUAL_TYPE(MenuBarButton,				GuiToolstripButton,		g::NewMenuBarButton);
				ADD_VIRTUAL_TYPE(MenuItemButton,			GuiToolstripButton,		g::NewMenuItemButton);
				ADD_VIRTUAL_TYPE(ToolstripDropdownButton,	GuiToolstripButton,		g::NewToolBarDropdownButton);
				ADD_VIRTUAL_TYPE(ToolstripSplitButton,		GuiToolstripButton,		g::NewToolBarSplitButton);
				ADD_VIRTUAL_TYPE(ToolstripSplitter,			GuiControl,				g::NewToolBarSplitter);
				ADD_VIRTUAL_TYPE(CheckBox,					GuiSelectableButton,	g::NewCheckBox);
				ADD_VIRTUAL_TYPE(RadioButton,				GuiSelectableButton,	g::NewRadioButton);
				ADD_VIRTUAL_TYPE(HScroll,					GuiScroll,				g::NewHScroll);
				ADD_VIRTUAL_TYPE(VScroll,					GuiScroll,				g::NewVScroll);
				ADD_VIRTUAL_TYPE(HTracker,					GuiScroll,				g::NewHTracker);
				ADD_VIRTUAL_TYPE(VTracker,					GuiScroll,				g::NewVTracker);
				ADD_VIRTUAL_TYPE(ProgressBar,				GuiScroll,				g::NewProgressBar);
				ADD_VIRTUAL_TYPE(CheckTextList,				GuiTextList,			g::NewCheckTextList);
				ADD_VIRTUAL_TYPE(RadioTextList,				GuiTextList,			g::NewRadioTextList);

#undef ADD_VIRTUAL_TYPE
#endif
			}

			void Unload()override
			{
			}
		};
		GUI_REGISTER_PLUGIN(GuiPredefinedInstanceLoadersPlugin)
	}
}