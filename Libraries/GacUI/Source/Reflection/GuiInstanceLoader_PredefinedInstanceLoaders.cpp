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
GuiVrtualTypeInstanceLoader
***********************************************************************/

		class GuiTemplateControlInstanceLoader : public Object, public IGuiInstanceLoader
		{
		protected:
			WString										typeName;
			Func<Value()>								defaultConstructor;
			Func<Value(Ptr<GuiTemplate::IFactory>)>		templateConstructor;
		public:
			GuiTemplateControlInstanceLoader(const WString& _typeName, const Func<Value()>& _defaultConstructor, const Func<Value(Ptr<GuiTemplate::IFactory>)>& _templateConstructor)
				:typeName(_typeName)
				, defaultConstructor(_defaultConstructor)
				, templateConstructor(_templateConstructor)
			{
				if (typeName == L"")
				{

				}
			}

			WString GetTypeName()override
			{
				return typeName;
			}

			bool IsCreatable(const TypeInfo& typeInfo)override
			{
				return typeName == typeInfo.typeName;
			}

			description::Value CreateInstance(Ptr<GuiInstanceEnvironment> env, const TypeInfo& typeInfo, collections::Group<WString, description::Value>& constructorArguments)override
			{
				if(typeName==typeInfo.typeName)
				{
					vint indexControlTemplate = constructorArguments.Keys().IndexOf(L"ControlTemplate");
					if (indexControlTemplate == -1)
					{
						return defaultConstructor();
					}
					else
					{
						auto factory = CreateTemplateFactory(constructorArguments.GetByIndex(indexControlTemplate)[0].GetText());
						return templateConstructor(factory);
					}
				}
				return Value();
			}

			void GetConstructorParameters(const TypeInfo& typeInfo, collections::List<WString>& propertyNames)override
			{
				propertyNames.Add(L"ControlTemplate");
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"ControlTemplate")
				{
					auto info = GuiInstancePropertyInfo::Assign(description::GetTypeDescriptor<WString>());
					info->constructorParameter = true;
					return info;
				}
				return 0;
			}
		};

/***********************************************************************
GuiControlInstanceLoader
***********************************************************************/

		class GuiControlInstanceLoader : public Object, public IGuiInstanceLoader
		{
		public:
			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiControl>()->GetTypeName();
			}

			void GetPropertyNames(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				propertyNames.Add(L"");
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"")
				{
					auto info = GuiInstancePropertyInfo::Collection();
					info->acceptableTypes.Add(description::GetTypeDescriptor<GuiControl>());
					info->acceptableTypes.Add(description::GetTypeDescriptor<GuiGraphicsComposition>());
					if (propertyInfo.typeInfo.typeDescriptor->CanConvertTo(description::GetTypeDescriptor<GuiInstanceRootObject>()))
					{
						info->acceptableTypes.Add(description::GetTypeDescriptor<GuiComponent>());
					}
					return info;
				}
				return IGuiInstanceLoader::GetPropertyType(propertyInfo);
			}

			bool SetPropertyValue(PropertyValue& propertyValue)override
			{
				if (auto container = dynamic_cast<GuiInstanceRootObject*>(propertyValue.instanceValue.GetRawPtr()))
				{
					if (propertyValue.propertyName == L"")
					{
						if (auto component = dynamic_cast<GuiComponent*>(propertyValue.propertyValue.GetRawPtr()))
						{
							container->AddComponent(component);
							return true;
						}
						else if (auto controlHost = dynamic_cast<GuiControlHost*>(propertyValue.propertyValue.GetRawPtr()))
						{
							container->AddComponent(new GuiObjectComponent<GuiControlHost>(controlHost));
							return true;
						}
					}
				}
				if (auto container = dynamic_cast<GuiControl*>(propertyValue.instanceValue.GetRawPtr()))
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
							container->GetContainerComposition()->AddChild(composition);
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

		class GuiTabInstanceLoader : public Object, public IGuiInstanceLoader
		{
		public:
			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiTab>()->GetTypeName();
			}

			void GetPropertyNames(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				propertyNames.Add(L"");
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"")
				{
					return GuiInstancePropertyInfo::CollectionWithParent(description::GetTypeDescriptor<GuiTabPage>());
				}
				return IGuiInstanceLoader::GetPropertyType(propertyInfo);
			}

			bool SetPropertyValue(PropertyValue& propertyValue)override
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

		class GuiTabPageInstanceLoader : public Object, public IGuiInstanceLoader
		{
		public:
			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiTabPage>()->GetTypeName();
			}

			void GetPropertyNames(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				propertyNames.Add(L"");
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"")
				{
					auto info = GuiInstancePropertyInfo::Collection();
					info->acceptableTypes.Add(description::GetTypeDescriptor<GuiControl>());
					info->acceptableTypes.Add(description::GetTypeDescriptor<GuiGraphicsComposition>());
					return info;
				}
				return IGuiInstanceLoader::GetPropertyType(propertyInfo);
			}

			bool SetPropertyValue(PropertyValue& propertyValue)override
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

		class GuiToolstripMenuInstanceLoader : public Object, public IGuiInstanceLoader
		{
		public:
			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiToolstripMenu>()->GetTypeName();
			}

			bool IsCreatable(const TypeInfo& typeInfo)override
			{
				return GetTypeName() == typeInfo.typeName;
			}

			description::Value CreateInstance(Ptr<GuiInstanceEnvironment> env, const TypeInfo& typeInfo, collections::Group<WString, description::Value>& constructorArguments)override
			{
				if(GetTypeName() == typeInfo.typeName)
				{
					vint indexControlTemplate = constructorArguments.Keys().IndexOf(L"ControlTemplate");
					if (indexControlTemplate == -1)
					{
						return Value::From(g::NewMenu(0));
					}
					else
					{
						auto factory = CreateTemplateFactory(constructorArguments.GetByIndex(indexControlTemplate)[0].GetText());
						return Value::From(new GuiToolstripMenu(new GuiMenuTemplate_StyleProvider(factory), 0));
					}
				}
				return Value();
			}

			void GetPropertyNames(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				propertyNames.Add(L"");
			}

			void GetConstructorParameters(const TypeInfo& typeInfo, collections::List<WString>& propertyNames)override
			{
				propertyNames.Add(L"ControlTemplate");
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"")
				{
					return GuiInstancePropertyInfo::CollectionWithParent(description::GetTypeDescriptor<GuiControl>());
				}
				else if (propertyInfo.propertyName == L"ControlTemplate")
				{
					auto info = GuiInstancePropertyInfo::Assign(description::GetTypeDescriptor<WString>());
					info->constructorParameter = true;
					return info;
				}
				return IGuiInstanceLoader::GetPropertyType(propertyInfo);
			}

			bool SetPropertyValue(PropertyValue& propertyValue)override
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

		class GuiToolstripMenuBarInstanceLoader : public Object, public IGuiInstanceLoader
		{
		public:
			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiToolstripMenuBar>()->GetTypeName();
			}

			bool IsCreatable(const TypeInfo& typeInfo)override
			{
				return GetTypeName() == typeInfo.typeName;
			}

			description::Value CreateInstance(Ptr<GuiInstanceEnvironment> env, const TypeInfo& typeInfo, collections::Group<WString, description::Value>& constructorArguments)override
			{
				if(GetTypeName() == typeInfo.typeName)
				{
					vint indexControlTemplate = constructorArguments.Keys().IndexOf(L"ControlTemplate");
					if (indexControlTemplate == -1)
					{
						return Value::From(new GuiToolstripMenuBar(GetCurrentTheme()->CreateMenuBarStyle()));
					}
					else
					{
						auto factory = CreateTemplateFactory(constructorArguments.GetByIndex(indexControlTemplate)[0].GetText());
						return Value::From(new GuiToolstripMenuBar(new GuiControlTemplate_StyleProvider(factory)));
					}
				}
				return Value();
			}

			void GetPropertyNames(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				propertyNames.Add(L"");
			}

			void GetConstructorParameters(const TypeInfo& typeInfo, collections::List<WString>& propertyNames)override
			{
				propertyNames.Add(L"ControlTemplate");
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"")
				{
					return GuiInstancePropertyInfo::CollectionWithParent(description::GetTypeDescriptor<GuiControl>());
				}
				else if (propertyInfo.propertyName == L"ControlTemplate")
				{
					auto info = GuiInstancePropertyInfo::Assign(description::GetTypeDescriptor<WString>());
					info->constructorParameter = true;
					return info;
				}
				return IGuiInstanceLoader::GetPropertyType(propertyInfo);
			}

			bool SetPropertyValue(PropertyValue& propertyValue)override
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

		class GuiToolstripToolBarInstanceLoader : public Object, public IGuiInstanceLoader
		{
		public:
			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiToolstripToolBar>()->GetTypeName();
			}

			bool IsCreatable(const TypeInfo& typeInfo)override
			{
				return GetTypeName() == typeInfo.typeName;
			}

			description::Value CreateInstance(Ptr<GuiInstanceEnvironment> env, const TypeInfo& typeInfo, collections::Group<WString, description::Value>& constructorArguments)override
			{
				if(GetTypeName() == typeInfo.typeName)
				{
					vint indexControlTemplate = constructorArguments.Keys().IndexOf(L"ControlTemplate");
					if (indexControlTemplate == -1)
					{
						return Value::From(new GuiToolstripToolBar(GetCurrentTheme()->CreateToolBarStyle()));
					}
					else
					{
						auto factory = CreateTemplateFactory(constructorArguments.GetByIndex(indexControlTemplate)[0].GetText());
						return Value::From(new GuiToolstripToolBar(new GuiControlTemplate_StyleProvider(factory)));
					}
				}
				return Value();
			}

			void GetPropertyNames(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				propertyNames.Add(L"");
			}

			void GetConstructorParameters(const TypeInfo& typeInfo, collections::List<WString>& propertyNames)override
			{
				propertyNames.Add(L"ControlTemplate");
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"")
				{
					return GuiInstancePropertyInfo::CollectionWithParent(description::GetTypeDescriptor<GuiControl>());
				}
				else if (propertyInfo.propertyName == L"ControlTemplate")
				{
					auto info = GuiInstancePropertyInfo::Assign(description::GetTypeDescriptor<WString>());
					info->constructorParameter = true;
					return info;
				}
				return IGuiInstanceLoader::GetPropertyType(propertyInfo);
			}

			bool SetPropertyValue(PropertyValue& propertyValue)override
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

		class GuiToolstripButtonInstanceLoader : public Object, public IGuiInstanceLoader
		{
		public:
			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiToolstripButton>()->GetTypeName();
			}

			bool IsCreatable(const TypeInfo& typeInfo)override
			{
				return typeInfo.typeName == GetTypeName();
			}

			description::Value CreateInstance(Ptr<GuiInstanceEnvironment> env, const TypeInfo& typeInfo, collections::Group<WString, description::Value>& constructorArguments)override
			{
				if (typeInfo.typeName == GetTypeName())
				{
					vint indexControlTemplate = constructorArguments.Keys().IndexOf(L"ControlTemplate");
					if (indexControlTemplate == -1)
					{
						return Value::From(g::NewToolBarButton());
					}
					else
					{
						auto factory = CreateTemplateFactory(constructorArguments.GetByIndex(indexControlTemplate)[0].GetText());
						return Value::From(new GuiToolstripButton(new GuiToolstripButtonTemplate_StyleProvider(factory)));
					}
				}
				return Value();
			}

			void GetPropertyNames(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				propertyNames.Add(L"SubMenu");
			}

			void GetConstructorParameters(const TypeInfo& typeInfo, collections::List<WString>& propertyNames)override
			{
				propertyNames.Add(L"ControlTemplate");
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"ControlTemplate")
				{
					auto info = GuiInstancePropertyInfo::Assign(description::GetTypeDescriptor<WString>());
					info->constructorParameter = true;
					return info;
				}
				else if (propertyInfo.propertyName == L"SubMenu")
				{
					return GuiInstancePropertyInfo::Set(description::GetTypeDescriptor<GuiToolstripMenu>());
				}
				return IGuiInstanceLoader::GetPropertyType(propertyInfo);
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
GuiSelectableListControlInstanceLoader
***********************************************************************/

		class GuiSelectableListControlInstanceLoader : public Object, public IGuiInstanceLoader
		{
		public:
			GuiSelectableListControlInstanceLoader()
			{
			}

			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiSelectableListControl>()->GetTypeName();
			}

			void GetPropertyNames(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				propertyNames.Add(L"ItemTemplate");
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"ItemTemplate")
				{
					auto info = GuiInstancePropertyInfo::Assign(description::GetTypeDescriptor<WString>());
					return info;
				}
				return IGuiInstanceLoader::GetPropertyType(propertyInfo);
			}

			bool SetPropertyValue(PropertyValue& propertyValue)override
			{
				if (GuiSelectableListControl* container = dynamic_cast<GuiSelectableListControl*>(propertyValue.instanceValue.GetRawPtr()))
				{
					if (propertyValue.propertyName == L"ItemTemplate")
					{
						auto factory = CreateTemplateFactory(propertyValue.propertyValue.GetText());
						auto styleProvider = new GuiListItemTemplate_ItemStyleProvider(factory);
						container->SetStyleProvider(styleProvider);
						return true;
					}
				}
				return false;
			}
		};

/***********************************************************************
GuiVirtualTreeViewInstanceLoader
***********************************************************************/

		class GuiVirtualTreeViewInstanceLoader : public Object, public IGuiInstanceLoader
		{
		public:
			GuiVirtualTreeViewInstanceLoader()
			{
			}

			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiVirtualTreeView>()->GetTypeName();
			}

			void GetPropertyNames(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				propertyNames.Add(L"ItemTemplate");
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"ItemTemplate")
				{
					auto info = GuiInstancePropertyInfo::Assign(description::GetTypeDescriptor<WString>());
					return info;
				}
				return IGuiInstanceLoader::GetPropertyType(propertyInfo);
			}

			bool SetPropertyValue(PropertyValue& propertyValue)override
			{
				if (propertyValue.propertyName == L"ItemTemplate")
				{
					return true;
				}
				return false;
			}
		};

/***********************************************************************
GuiVirtualDataGridInstanceLoader
***********************************************************************/

		class GuiVirtualDataGridInstanceLoader : public Object, public IGuiInstanceLoader
		{
		public:
			GuiVirtualDataGridInstanceLoader()
			{
			}

			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiVirtualDataGrid>()->GetTypeName();
			}

			void GetPropertyNames(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				propertyNames.Add(L"ItemTemplate");
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"ItemTemplate")
				{
					auto info = GuiInstancePropertyInfo::Assign(description::GetTypeDescriptor<WString>());
					return info;
				}
				return IGuiInstanceLoader::GetPropertyType(propertyInfo);
			}

			bool SetPropertyValue(PropertyValue& propertyValue)override
			{
				if (propertyValue.propertyName == L"ItemTemplate")
				{
					return true;
				}
				return false;
			}
		};

/***********************************************************************
GuiListViewInstanceLoader
***********************************************************************/

		class GuiListViewInstanceLoader : public Object, public IGuiInstanceLoader
		{
		protected:
			bool				bindable;

		public:
			GuiListViewInstanceLoader(bool _bindable)
				:bindable(_bindable)
			{
			}

			WString GetTypeName()override
			{
				if (bindable)
				{
					return description::GetTypeDescriptor<GuiBindableListView>()->GetTypeName();
				}
				else
				{
					return description::GetTypeDescriptor<GuiListView>()->GetTypeName();
				}
			}

			bool IsCreatable(const TypeInfo& typeInfo)override
			{
				return typeInfo.typeName == GetTypeName();
			}

			description::Value CreateInstance(Ptr<GuiInstanceEnvironment> env, const TypeInfo& typeInfo, collections::Group<WString, description::Value>& constructorArguments)override
			{
				if (typeInfo.typeName == GetTypeName())
				{
					Ptr<IValueEnumerable> itemSource;
					ListViewViewType viewType = ListViewViewType::Detail;
					GuiListViewBase::IStyleProvider* styleProvider = 0;
					Size iconSize;
					{
						vint itemSourceIndex = constructorArguments.Keys().IndexOf(L"ItemSource");
						if (itemSourceIndex != -1)
						{
							itemSource = UnboxValue<Ptr<IValueEnumerable>>(constructorArguments.GetByIndex(itemSourceIndex)[0]);
						}
						else if (bindable)
						{
							return Value();
						}

						vint indexView = constructorArguments.Keys().IndexOf(L"View");
						if (indexView != -1)
						{
							viewType = UnboxValue<ListViewViewType>(constructorArguments.GetByIndex(indexView)[0]);
						}

						vint indexIconSize = constructorArguments.Keys().IndexOf(L"IconSize");
						if (indexIconSize != -1)
						{
							iconSize = UnboxValue<Size>(constructorArguments.GetByIndex(indexIconSize)[0]);
						}

						vint indexControlTemplate = constructorArguments.Keys().IndexOf(L"ControlTemplate");
						if (indexControlTemplate == -1)
						{
							styleProvider = GetCurrentTheme()->CreateListViewStyle();
						}
						else
						{
							auto factory = CreateTemplateFactory(constructorArguments.GetByIndex(indexControlTemplate)[0].GetText());
							styleProvider = new GuiListViewTemplate_StyleProvider(factory);
						}
					}

					GuiVirtualListView* listView = 0;
					if (bindable)
					{
						listView = new GuiBindableListView(styleProvider, itemSource);
					}
					else
					{
						listView = new GuiListView(styleProvider);
					}
					switch (viewType)
					{
#define VIEW_TYPE_CASE(NAME)\
					case ListViewViewType::NAME:\
						if (iconSize == Size())\
						{\
							listView->ChangeItemStyle(new list::ListView##NAME##ContentProvider);\
						}\
						else\
						{\
							listView->ChangeItemStyle(new list::ListView##NAME##ContentProvider(iconSize, false));\
						}\
						break;\

						VIEW_TYPE_CASE(BigIcon)
						VIEW_TYPE_CASE(SmallIcon)
						VIEW_TYPE_CASE(List)
						VIEW_TYPE_CASE(Tile)
						VIEW_TYPE_CASE(Information)
						VIEW_TYPE_CASE(Detail)

#undef VIEW_TYPE_CASE
					}

					return Value::From(listView);
				}
				return Value();
			}

			void GetPropertyNames(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
			}

			void GetConstructorParameters(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				if (typeInfo.typeName == GetTypeName())
				{
					propertyNames.Add(L"ControlTemplate");
					propertyNames.Add(L"View");
					propertyNames.Add(L"IconSize");
					if (bindable)
					{
						propertyNames.Add(L"ItemSource");
					}
				}
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"ControlTemplate")
				{
					auto info = GuiInstancePropertyInfo::Assign(description::GetTypeDescriptor<WString>());
					info->constructorParameter = true;
					return info;
				}
				else if (propertyInfo.propertyName == L"View")
				{
					auto info = GuiInstancePropertyInfo::Assign(description::GetTypeDescriptor<ListViewViewType>());
					info->constructorParameter = true;
					return info;
				}
				else if (propertyInfo.propertyName == L"IconSize")
				{
					auto info = GuiInstancePropertyInfo::Assign(description::GetTypeDescriptor<Size>());
					info->constructorParameter = true;
					return info;
				}
				else if (propertyInfo.propertyName == L"ItemSource")
				{
					if (bindable)
					{
						auto info = GuiInstancePropertyInfo::Assign(description::GetTypeDescriptor<IValueEnumerable>());
						info->constructorParameter = true;
						info->required = true;
						return info;
					}
				}
				return IGuiInstanceLoader::GetPropertyType(propertyInfo);
			}

			bool SetPropertyValue(PropertyValue& propertyValue)override
			{
				return false;
			}
		};

/***********************************************************************
GuiTreeViewInstanceLoader
***********************************************************************/

		class GuiTreeViewInstanceLoader : public Object, public IGuiInstanceLoader
		{
		protected:
			bool				bindable;

		public:
			GuiTreeViewInstanceLoader(bool _bindable)
				:bindable(_bindable)
			{
			}

			WString GetTypeName()override
			{
				if (bindable)
				{
					return description::GetTypeDescriptor<GuiBindableTreeView>()->GetTypeName();
				}
				else
				{
					return description::GetTypeDescriptor<GuiTreeView>()->GetTypeName();
				}
			}

			bool IsCreatable(const TypeInfo& typeInfo)override
			{
				return typeInfo.typeName == GetTypeName();
			}

			description::Value CreateInstance(Ptr<GuiInstanceEnvironment> env, const TypeInfo& typeInfo, collections::Group<WString, description::Value>& constructorArguments)override
			{
				if (typeInfo.typeName == GetTypeName())
				{
					vint indexItemSource = constructorArguments.Keys().IndexOf(L"ItemSource");
					GuiVirtualTreeView::IStyleProvider* styleProvider = 0;
					{
						vint indexControlTemplate = constructorArguments.Keys().IndexOf(L"ControlTemplate");
						if (indexControlTemplate == -1)
						{
							styleProvider = GetCurrentTheme()->CreateTreeViewStyle();
						}
						else
						{
							auto factory = CreateTemplateFactory(constructorArguments.GetByIndex(indexControlTemplate)[0].GetText());
							styleProvider = new GuiTreeViewTemplate_StyleProvider(factory);
						}
					}

					GuiVirtualTreeView* treeView = 0;
					if (bindable)
					{
						if (indexItemSource == -1)
						{
							return Value();
						}

						auto itemSource = constructorArguments.GetByIndex(indexItemSource)[0];
						treeView = new GuiBindableTreeView(styleProvider, itemSource);
					}
					else
					{
						treeView = new GuiTreeView(styleProvider);
					}

					vint indexIconSize = constructorArguments.Keys().IndexOf(L"IconSize");
					if (indexIconSize != -1)
					{
						auto iconSize = UnboxValue<Size>(constructorArguments.GetByIndex(indexIconSize)[0]);
						treeView->SetNodeStyleProvider(new tree::TreeViewNodeItemStyleProvider(iconSize, false));
					}

					return Value::From(treeView);
				}
				return Value();
			}

			void GetPropertyNames(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				if (!bindable)
				{
					propertyNames.Add(L"Nodes");
				}
			}

			void GetConstructorParameters(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				if (typeInfo.typeName == GetTypeName())
				{
					propertyNames.Add(L"ControlTemplate");
					propertyNames.Add(L"IconSize");
					if (bindable)
					{
						propertyNames.Add(L"ItemSource");
					}
				}
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"Nodes")
				{
					if (!bindable)
					{
						return GuiInstancePropertyInfo::Collection(description::GetTypeDescriptor<tree::MemoryNodeProvider>());
					}
				}
				else if (propertyInfo.propertyName == L"ControlTemplate")
				{
					auto info = GuiInstancePropertyInfo::Assign(description::GetTypeDescriptor<WString>());
					info->constructorParameter = true;
					return info;
				}
				else if (propertyInfo.propertyName == L"ItemSource")
				{
					if (bindable)
					{
						auto info = GuiInstancePropertyInfo::Assign(description::GetTypeDescriptor<Value>());
						info->constructorParameter = true;
						info->required = true;
						return info;
					}
				}
				else if (propertyInfo.propertyName == L"IconSize")
				{
					auto info = GuiInstancePropertyInfo::Assign(description::GetTypeDescriptor<Size>());
					info->constructorParameter = true;
					return info;
				}
				return IGuiInstanceLoader::GetPropertyType(propertyInfo);
			}

			bool SetPropertyValue(PropertyValue& propertyValue)override
			{
				if (GuiTreeView* container = dynamic_cast<GuiTreeView*>(propertyValue.instanceValue.GetRawPtr()))
				{
					if (propertyValue.propertyName == L"Nodes")
					{
						auto item = UnboxValue<Ptr<tree::MemoryNodeProvider>>(propertyValue.propertyValue);
						container->Nodes()->Children().Add(item);
						return true;
					}
				}
				return false;
			}
		};

/***********************************************************************
GuiComboBoxInstanceLoader
***********************************************************************/

		class GuiComboBoxInstanceLoader : public Object, public IGuiInstanceLoader
		{
		protected:
			WString							typeName;
		public:
			GuiComboBoxInstanceLoader()
				:typeName(L"presentation::controls::GuiComboBox")
			{
			}

			WString GetTypeName()override
			{
				return typeName;
			}

			bool IsCreatable(const TypeInfo& typeInfo)override
			{
				return typeInfo.typeName == GetTypeName();
			}

			description::Value CreateInstance(Ptr<GuiInstanceEnvironment> env, const TypeInfo& typeInfo, collections::Group<WString, description::Value>& constructorArguments)override
			{
				if (typeInfo.typeName == GetTypeName())
				{
					vint indexListControl = constructorArguments.Keys().IndexOf(L"ListControl");
					vint indexControlTemplate = constructorArguments.Keys().IndexOf(L"ControlTemplate");
					if (indexListControl != -1)
					{
						Ptr<GuiTemplate::IFactory> factory;
						if (indexControlTemplate != -1)
						{
							factory = CreateTemplateFactory(constructorArguments.GetByIndex(indexControlTemplate)[0].GetText());
						}

						GuiComboBoxBase::IStyleController* styleController = 0;
						if (factory)
						{
							styleController = new GuiComboBoxTemplate_StyleProvider(factory);
						}
						else
						{
							styleController = GetCurrentTheme()->CreateComboBoxStyle();
						}

						auto listControl = UnboxValue<GuiSelectableListControl*>(constructorArguments.GetByIndex(indexListControl)[0]);
						auto comboBox = new GuiComboBoxListControl(styleController, listControl);
						return Value::From(comboBox);
					}
				}
				return Value();
			}

			void GetPropertyNames(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				if (typeInfo.typeName == GetTypeName())
				{
					propertyNames.Add(L"ListControl");
				}
			}

			void GetConstructorParameters(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				if (typeInfo.typeName == GetTypeName())
				{
					propertyNames.Add(L"ControlTemplate");
					propertyNames.Add(L"ListControl");
				}
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"ListControl")
				{
					auto info = GuiInstancePropertyInfo::Assign(description::GetTypeDescriptor<GuiSelectableListControl>());
					info->constructorParameter = true;
					info->required = true;
					return info;
				}
				else if (propertyInfo.propertyName == L"ControlTemplate")
				{
					auto info = GuiInstancePropertyInfo::Assign(description::GetTypeDescriptor<WString>());
					info->constructorParameter = true;
					return info;
				}
				return IGuiInstanceLoader::GetPropertyType(propertyInfo);
			}
		};

/***********************************************************************
GuiBindableTextListInstanceLoader
***********************************************************************/

		class GuiBindableTextListInstanceLoader : public Object, public IGuiInstanceLoader
		{
			typedef Func<list::TextItemStyleProvider::ITextItemStyleProvider*()>		ItemStyleProviderFactory;
		protected:
			WString							typeName;
			ItemStyleProviderFactory		itemStyleProviderFactory;

		public:
			GuiBindableTextListInstanceLoader(const WString& type, const ItemStyleProviderFactory& factory)
				:typeName(L"presentation::controls::GuiBindable" + type + L"TextList")
				, itemStyleProviderFactory(factory)
			{
			}

			WString GetTypeName()override
			{
				return typeName;
			}

			bool IsCreatable(const TypeInfo& typeInfo)override
			{
				return typeInfo.typeName == GetTypeName();
			}

			description::Value CreateInstance(Ptr<GuiInstanceEnvironment> env, const TypeInfo& typeInfo, collections::Group<WString, description::Value>& constructorArguments)override
			{
				if (typeInfo.typeName == GetTypeName())
				{
					vint indexItemSource = constructorArguments.Keys().IndexOf(L"ItemSource");
					if (indexItemSource != -1)
					{
						GuiTextListTemplate_StyleProvider* styleProvider = 0;
						{
							vint indexControlTemplate = constructorArguments.Keys().IndexOf(L"ControlTemplate");
							if (indexControlTemplate != -1)
							{
								auto factory = CreateTemplateFactory(constructorArguments.GetByIndex(indexControlTemplate)[0].GetText());
								styleProvider = new GuiTextListTemplate_StyleProvider(factory);
							}
						}

						auto itemSource = UnboxValue<Ptr<IValueEnumerable>>(constructorArguments.GetByIndex(indexItemSource)[0]);
						GuiBindableTextList* control = 0;
						if (styleProvider)
						{
							control = new GuiBindableTextList(styleProvider, styleProvider->CreateArgument(), itemSource);
						}
						else
						{
							control = new GuiBindableTextList(GetCurrentTheme()->CreateTextListStyle(), itemStyleProviderFactory(), itemSource);
						}
						return Value::From(control);
					}
				}
				return Value();
			}

			void GetConstructorParameters(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				if (typeInfo.typeName == GetTypeName())
				{
					propertyNames.Add(L"ControlTemplate");
					propertyNames.Add(L"ItemSource");
				}
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"ControlTemplate")
				{
					auto info = GuiInstancePropertyInfo::Assign(description::GetTypeDescriptor<WString>());
					info->constructorParameter = true;
					return info;
				}
				if (propertyInfo.propertyName == L"ItemSource")
				{
					auto info = GuiInstancePropertyInfo::Assign(description::GetTypeDescriptor<IValueEnumerable>());
					info->constructorParameter = true;
					info->required = true;
					return info;
				}
				return IGuiInstanceLoader::GetPropertyType(propertyInfo);
			}
		};

/***********************************************************************
GuiCompositionInstanceLoader
***********************************************************************/

		class GuiCompositionInstanceLoader : public Object, public IGuiInstanceLoader
		{
		public:
			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiGraphicsComposition>()->GetTypeName();
			}

			void GetPropertyNames(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				propertyNames.Add(L"");
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"")
				{
					auto info = GuiInstancePropertyInfo::Collection();
					info->acceptableTypes.Add(description::GetTypeDescriptor<GuiControl>());
					info->acceptableTypes.Add(description::GetTypeDescriptor<GuiGraphicsComposition>());
					info->acceptableTypes.Add(description::GetTypeDescriptor<IGuiGraphicsElement>());
					return info;
				}
				return IGuiInstanceLoader::GetPropertyType(propertyInfo);
			}

			bool SetPropertyValue(PropertyValue& propertyValue)override
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

		class GuiTableCompositionInstanceLoader : public Object, public IGuiInstanceLoader
		{
		public:
			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiTableComposition>()->GetTypeName();
			}

			void GetPropertyNames(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				propertyNames.Add(L"Rows");
				propertyNames.Add(L"Columns");
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"Rows" || propertyInfo.propertyName==L"Columns")
				{
					return GuiInstancePropertyInfo::Array(description::GetTypeDescriptor<GuiCellOption>());
				}
				return IGuiInstanceLoader::GetPropertyType(propertyInfo);
			}

			bool SetPropertyValue(PropertyValue& propertyValue)override
			{
				if (GuiTableComposition* container = dynamic_cast<GuiTableComposition*>(propertyValue.instanceValue.GetRawPtr()))
				{
					if (propertyValue.propertyName == L"Rows")
					{
						List<GuiCellOption> options;
						CopyFrom(options, GetLazyList<GuiCellOption>(UnboxValue<Ptr<IValueList>>(propertyValue.propertyValue)));
						container->SetRowsAndColumns(options.Count(), container->GetColumns());
						FOREACH_INDEXER(GuiCellOption, option, index, options)
						{
							container->SetRowOption(index, option);
						}
						return true;
					}
					else if (propertyValue.propertyName == L"Columns")
					{
						List<GuiCellOption> options;
						CopyFrom(options, GetLazyList<GuiCellOption>(UnboxValue<Ptr<IValueList>>(propertyValue.propertyValue)));
						container->SetRowsAndColumns(container->GetRows(), options.Count());
						FOREACH_INDEXER(GuiCellOption, option, index, options)
						{
							container->SetColumnOption(index, option);
						}
						return true;
					}
				}
				return false;
			}
		};

/***********************************************************************
GuiCellCompositionInstanceLoader
***********************************************************************/

		class GuiCellCompositionInstanceLoader : public Object, public IGuiInstanceLoader
		{
		public:

			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiCellComposition>()->GetTypeName();
			}

			void GetPropertyNames(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				propertyNames.Add(L"Site");
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"Site")
				{
					return GuiInstancePropertyInfo::Assign(description::GetTypeDescriptor<SiteValue>());
				}
				return IGuiInstanceLoader::GetPropertyType(propertyInfo);
			}

			bool SetPropertyValue(PropertyValue& propertyValue)override
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

/***********************************************************************
GuiTreeNodeInstanceLoader
***********************************************************************/

		class GuiTreeNodeInstanceLoader : public Object, public IGuiInstanceLoader
		{
		protected:
			WString							typeName;
		public:
			GuiTreeNodeInstanceLoader()
				:typeName(L"presentation::controls::tree::TreeNode")
			{
			}

			WString GetTypeName()override
			{
				return typeName;
			}

			bool IsCreatable(const TypeInfo& typeInfo)override
			{
				return typeInfo.typeName == GetTypeName();
			}

			description::Value CreateInstance(Ptr<GuiInstanceEnvironment> env, const TypeInfo& typeInfo, collections::Group<WString, description::Value>& constructorArguments)override
			{
				if (typeInfo.typeName == GetTypeName())
				{
					Ptr<tree::TreeViewItem> item = new tree::TreeViewItem;
					Ptr<tree::MemoryNodeProvider> node = new tree::MemoryNodeProvider(item);
					return Value::From(node);
				}
				return Value();
			}

			void GetPropertyNames(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				propertyNames.Add(L"Text");
				propertyNames.Add(L"Image");
				propertyNames.Add(L"Tag");
				propertyNames.Add(L"");
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"Text")
				{
					return GuiInstancePropertyInfo::Assign(description::GetTypeDescriptor<WString>());
				}
				else if (propertyInfo.propertyName == L"Image")
				{
					return GuiInstancePropertyInfo::Assign(description::GetTypeDescriptor<GuiImageData>());
				}
				else if (propertyInfo.propertyName == L"Tag")
				{
					return GuiInstancePropertyInfo::Assign(description::GetTypeDescriptor<Value>());
				}
				else if (propertyInfo.propertyName == L"")
				{
					return GuiInstancePropertyInfo::Collection(description::GetTypeDescriptor<tree::MemoryNodeProvider>());
				}
				return IGuiInstanceLoader::GetPropertyType(propertyInfo);
			}

			bool SetPropertyValue(PropertyValue& propertyValue)override
			{
				if (tree::MemoryNodeProvider* container = dynamic_cast<tree::MemoryNodeProvider*>(propertyValue.instanceValue.GetRawPtr()))
				{
					if (propertyValue.propertyName == L"Text")
					{
						if (auto item = container->GetData().Cast<tree::TreeViewItem>())
						{
							item->text = UnboxValue<WString>(propertyValue.propertyValue);
							container->NotifyDataModified();
							return true;
						}
					}
					else if (propertyValue.propertyName == L"Image")
					{
						if (auto item = container->GetData().Cast<tree::TreeViewItem>())
						{
							item->image = UnboxValue<Ptr<GuiImageData>>(propertyValue.propertyValue);
							container->NotifyDataModified();
							return true;
						}
					}
					else if (propertyValue.propertyName == L"Tag")
					{
						if (auto item = container->GetData().Cast<tree::TreeViewItem>())
						{
							item->tag = propertyValue.propertyValue;
							return true;
						}
					}
					else if (propertyValue.propertyName == L"")
					{
						auto item = UnboxValue<Ptr<tree::MemoryNodeProvider>>(propertyValue.propertyValue);
						container->Children().Add(item);
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

		void InitializeTrackerProgressBar(GuiScroll* control)
		{
			control->SetPageSize(0);
		}

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

#define ADD_VIRTUAL_TYPE_LOADER(TYPENAME, LOADER)\
	manager->CreateVirtualType(\
		description::GetTypeDescriptor<TYPENAME>()->GetTypeName(),\
		new LOADER\
		)

#define ADD_TEMPLATE_CONTROL(TYPENAME, CONSTRUCTOR, TEMPLATE)\
	manager->SetLoader(\
		new GuiTemplateControlInstanceLoader(\
			L"presentation::controls::" L#TYPENAME,\
			[](){return Value::From(CONSTRUCTOR());},\
			[](Ptr<GuiTemplate::IFactory> factory){return Value::From(new TYPENAME(new TEMPLATE##_StyleProvider(factory))); }\
			)\
		)

#define ADD_TEMPLATE_CONTROL_2(TYPENAME, CONSTRUCTOR, TEMPLATE)\
	manager->SetLoader(\
		new GuiTemplateControlInstanceLoader(\
			L"presentation::controls::" L#TYPENAME,\
			[](){return Value::From(CONSTRUCTOR());},\
			[](Ptr<GuiTemplate::IFactory> factory)\
			{\
				auto style = new TEMPLATE##_StyleProvider(factory);\
				auto argument = style->CreateArgument();\
				return Value::From(new TYPENAME(style, argument));\
			}\
			)\
		)

#define ADD_VIRTUAL_CONTROL(VIRTUALTYPENAME, TYPENAME, CONSTRUCTOR, TEMPLATE)\
	manager->CreateVirtualType(\
		description::GetTypeDescriptor<TYPENAME>()->GetTypeName(),\
		new GuiTemplateControlInstanceLoader(\
			L"presentation::controls::Gui" L#VIRTUALTYPENAME,\
			[](){return Value::From(CONSTRUCTOR());},\
			[](Ptr<GuiTemplate::IFactory> factory){return Value::From(new TYPENAME(new TEMPLATE##_StyleProvider(factory))); }\
			)\
		)

#define ADD_VIRTUAL_CONTROL_2(VIRTUALTYPENAME, TYPENAME, CONSTRUCTOR, TEMPLATE)\
	manager->CreateVirtualType(\
		description::GetTypeDescriptor<TYPENAME>()->GetTypeName(),\
		new GuiTemplateControlInstanceLoader(\
			L"presentation::controls::Gui" L#VIRTUALTYPENAME,\
			[](){return Value::From(CONSTRUCTOR());},\
			[](Ptr<GuiTemplate::IFactory> factory)\
			{\
				auto style = new TEMPLATE##_StyleProvider(factory);\
				auto argument = style->CreateArgument();\
				return Value::From(new TYPENAME(style, argument));\
			}\
			)\
		)

#define ADD_VIRTUAL_CONTROL_F(VIRTUALTYPENAME, TYPENAME, CONSTRUCTOR, TEMPLATE, FUNCTION)\
	manager->CreateVirtualType(\
		description::GetTypeDescriptor<TYPENAME>()->GetTypeName(),\
		new GuiTemplateControlInstanceLoader(\
			L"presentation::controls::Gui" L#VIRTUALTYPENAME,\
			[](){return Value::From(CONSTRUCTOR());},\
			[](Ptr<GuiTemplate::IFactory> factory)\
			{\
				auto control = new TYPENAME(new TEMPLATE##_StyleProvider(factory));\
				FUNCTION(control);\
				return Value::From(control);\
			}\
			)\
		)

				manager->SetLoader(new GuiControlInstanceLoader);
				manager->SetLoader(new GuiTabInstanceLoader);
				manager->SetLoader(new GuiTabPageInstanceLoader);
				manager->SetLoader(new GuiToolstripMenuInstanceLoader);				// ControlTemplate
				manager->SetLoader(new GuiToolstripMenuBarInstanceLoader);			// ControlTemplate
				manager->SetLoader(new GuiToolstripToolBarInstanceLoader);			// ControlTemplate
				manager->SetLoader(new GuiToolstripButtonInstanceLoader);			// ControlTemplate
				manager->SetLoader(new GuiSelectableListControlInstanceLoader);		// ItemTemplate
				manager->SetLoader(new GuiVirtualTreeViewInstanceLoader);			// ItemTemplate
				manager->SetLoader(new GuiVirtualDataGridInstanceLoader);			// ItemTemplate
				manager->SetLoader(new GuiListViewInstanceLoader(false));			// ControlTemplate
				manager->SetLoader(new GuiTreeViewInstanceLoader(false));			// ControlTemplate
				manager->SetLoader(new GuiBindableTextListInstanceLoader(L"", [](){return GetCurrentTheme()->CreateTextListItemStyle(); }));	// ControlTemplate, ItemSource
				manager->SetLoader(new GuiListViewInstanceLoader(true));			// ControlTemplate, ItemSource
				manager->SetLoader(new GuiTreeViewInstanceLoader(true));			// ControlTemplate, ItemSource

				manager->SetLoader(new GuiCompositionInstanceLoader);
				manager->SetLoader(new GuiTableCompositionInstanceLoader);
				manager->SetLoader(new GuiCellCompositionInstanceLoader);
				
				ADD_VIRTUAL_TYPE_LOADER(GuiComboBoxListControl,						GuiComboBoxInstanceLoader);				// ControlTemplate
				ADD_VIRTUAL_TYPE_LOADER(tree::MemoryNodeProvider,					GuiTreeNodeInstanceLoader);

				ADD_TEMPLATE_CONTROL	(							GuiCustomControl,		g::NewCustomControl,			GuiControlTemplate);			// ControlTemplate
				ADD_TEMPLATE_CONTROL	(							GuiLabel,				g::NewLabel,					GuiLabelTemplate);				// ControlTemplate
				ADD_TEMPLATE_CONTROL	(							GuiButton,				g::NewButton,					GuiButtonTemplate);				// ControlTemplate
				ADD_TEMPLATE_CONTROL	(							GuiScrollContainer,		g::NewScrollContainer,			GuiScrollViewTemplate);			// ControlTemplate
				ADD_TEMPLATE_CONTROL	(							GuiWindow,				g::NewWindow,					GuiWindowTemplate);				// ControlTemplate
				ADD_TEMPLATE_CONTROL_2	(							GuiTextList,			g::NewTextList,					GuiTextListTemplate);			// ControlTemplate
				ADD_TEMPLATE_CONTROL	(							GuiDocumentViewer,		g::NewDocumentViewer,			GuiScrollViewTemplate);			// ControlTemplate
				ADD_TEMPLATE_CONTROL	(							GuiDocumentLabel,		g::NewDocumentLabel,			GuiControlTemplate);			// ControlTemplate
				ADD_TEMPLATE_CONTROL	(							GuiMultilineTextBox,	g::NewMultilineTextBox,			GuiMultilineTextBoxTemplate);	// ControlTemplate
				ADD_TEMPLATE_CONTROL	(							GuiSinglelineTextBox,	g::NewTextBox,					GuiSinglelineTextBoxTemplate);	// ControlTemplate
				ADD_TEMPLATE_CONTROL	(							GuiDatePicker,			g::NewDatePicker,				GuiDatePickerTemplate);			// ControlTemplate
				ADD_TEMPLATE_CONTROL_2	(							GuiDateComboBox,		g::NewDateComboBox,				GuiDateComboBoxTemplate);		// ControlTemplate
				ADD_TEMPLATE_CONTROL	(							GuiStringGrid,			g::NewStringGrid,				GuiListViewTemplate);			// ControlTemplate
																																							// ControlTemplate
				ADD_VIRTUAL_CONTROL		(GroupBox,					GuiControl,				g::NewGroupBox,					GuiControlTemplate);			// ControlTemplate
				ADD_VIRTUAL_CONTROL		(MenuSplitter,				GuiControl,				g::NewMenuSplitter,				GuiControlTemplate);			// ControlTemplate
				ADD_VIRTUAL_CONTROL		(MenuBarButton,				GuiToolstripButton,		g::NewMenuBarButton,			GuiToolstripButtonTemplate);	// ControlTemplate
				ADD_VIRTUAL_CONTROL		(MenuItemButton,			GuiToolstripButton,		g::NewMenuItemButton,			GuiToolstripButtonTemplate);	// ControlTemplate
				ADD_VIRTUAL_CONTROL		(ToolstripDropdownButton,	GuiToolstripButton,		g::NewToolBarDropdownButton,	GuiToolstripButtonTemplate);	// ControlTemplate
				ADD_VIRTUAL_CONTROL		(ToolstripSplitButton,		GuiToolstripButton,		g::NewToolBarSplitButton,		GuiToolstripButtonTemplate);	// ControlTemplate
				ADD_VIRTUAL_CONTROL		(ToolstripSplitter,			GuiControl,				g::NewToolBarSplitter,			GuiControlTemplate);			// ControlTemplate
				ADD_VIRTUAL_CONTROL		(CheckBox,					GuiSelectableButton,	g::NewCheckBox,					GuiSelectableButtonTemplate);	// ControlTemplate
				ADD_VIRTUAL_CONTROL		(RadioButton,				GuiSelectableButton,	g::NewRadioButton,				GuiSelectableButtonTemplate);	// ControlTemplate
				ADD_VIRTUAL_CONTROL		(HScroll,					GuiScroll,				g::NewHScroll,					GuiScrollTemplate);				// ControlTemplate
				ADD_VIRTUAL_CONTROL		(VScroll,					GuiScroll,				g::NewVScroll,					GuiScrollTemplate);				// ControlTemplate
				ADD_VIRTUAL_CONTROL_F	(HTracker,					GuiScroll,				g::NewHTracker,					GuiScrollTemplate,				InitializeTrackerProgressBar);	// ControlTemplate
				ADD_VIRTUAL_CONTROL_F	(VTracker,					GuiScroll,				g::NewVTracker,					GuiScrollTemplate,				InitializeTrackerProgressBar);	// ControlTemplate
				ADD_VIRTUAL_CONTROL_F	(ProgressBar,				GuiScroll,				g::NewProgressBar,				GuiScrollTemplate,				InitializeTrackerProgressBar);	// ControlTemplate
				ADD_VIRTUAL_CONTROL_2	(CheckTextList,				GuiTextList,			g::NewCheckTextList,			GuiTextListTemplate);			// ControlTemplate
				ADD_VIRTUAL_CONTROL_2	(RadioTextList,				GuiTextList,			g::NewRadioTextList,			GuiTextListTemplate);			// ControlTemplate

				auto bindableTextListName = description::GetTypeDescriptor<GuiBindableTextList>()->GetTypeName();			// ControlTemplate, ItemSource
				manager->CreateVirtualType(bindableTextListName, new GuiBindableTextListInstanceLoader(L"Check", [](){return GetCurrentTheme()->CreateCheckTextListItemStyle(); }));
				manager->CreateVirtualType(bindableTextListName, new GuiBindableTextListInstanceLoader(L"Radio", [](){return GetCurrentTheme()->CreateRadioTextListItemStyle(); }));

#undef ADD_VIRTUAL_TYPE
#undef ADD_VIRTUAL_TYPE_LOADER
#endif
			}

			void Unload()override
			{
			}
		};
		GUI_REGISTER_PLUGIN(GuiPredefinedInstanceLoadersPlugin)
	}
}