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

		class GuiVrtualTypeInstanceLoader : public Object, public IGuiInstanceLoader
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

			bool IsCreatable(const TypeInfo& typeInfo)override
			{
				return typeName==typeInfo.typeName;
			}

			description::Value CreateInstance(const TypeInfo& typeInfo, collections::Group<WString, description::Value>& constructorArguments)override
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
					return info;
				}
				return IGuiInstanceLoader::GetPropertyType(propertyInfo);
			}

			bool SetPropertyValue(PropertyValue& propertyValue)override
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

		class GuiControlHostInstanceLoader : public Object, public IGuiInstanceLoader
		{
		public:
			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiControlHost>()->GetTypeName();
			}

			void GetPropertyNames(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				propertyNames.Add(L"");
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"")
				{
					return GuiInstancePropertyInfo::CollectionWithParent(description::GetTypeDescriptor<GuiComponent>());
				}
				return IGuiInstanceLoader::GetPropertyType(propertyInfo);
			}

			bool SetPropertyValue(PropertyValue& propertyValue)override
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

			void GetPropertyNames(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				propertyNames.Add(L"");
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"")
				{
					return GuiInstancePropertyInfo::CollectionWithParent(description::GetTypeDescriptor<GuiControl>());
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

			void GetPropertyNames(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				propertyNames.Add(L"");
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"")
				{
					return GuiInstancePropertyInfo::CollectionWithParent(description::GetTypeDescriptor<GuiControl>());
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

			void GetPropertyNames(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				propertyNames.Add(L"");
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"")
				{
					return GuiInstancePropertyInfo::CollectionWithParent(description::GetTypeDescriptor<GuiControl>());
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

			description::Value CreateInstance(const TypeInfo& typeInfo, collections::Group<WString, description::Value>& constructorArguments)override
			{
				if (typeInfo.typeName == GetTypeName())
				{
					return Value::From(g::NewToolBarButton());
				}
				return Value();
			}

			void GetPropertyNames(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				propertyNames.Add(L"SubMenu");
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"SubMenu")
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
GuiListViewInstanceLoader
***********************************************************************/

		class GuiListViewInstanceLoader : public Object, public IGuiInstanceLoader
		{
		public:
			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiListView>()->GetTypeName();
			}

			bool IsCreatable(const TypeInfo& typeInfo)override
			{
				return typeInfo.typeName == GetTypeName();
			}

			description::Value CreateInstance(const TypeInfo& typeInfo, collections::Group<WString, description::Value>& constructorArguments)override
			{
				if (typeInfo.typeName == GetTypeName())
				{
					ListViewViewType viewType = ListViewViewType::Detail;
					Size iconSize;
					{
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
					}

					auto listView = new GuiListView(GetCurrentTheme()->CreateListViewStyle());
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
				if (typeInfo.typeName == GetTypeName())
				{
					propertyNames.Add(L"View");
					propertyNames.Add(L"IconSize");
				}
				propertyNames.Add(L"Items");
				propertyNames.Add(L"Columns");
				propertyNames.Add(L"DataColumns");
			}

			void GetConstructorParameters(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				if (typeInfo.typeName == GetTypeName())
				{
					propertyNames.Add(L"View");
					propertyNames.Add(L"IconSize");
				}
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"View")
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
				else if (propertyInfo.propertyName == L"Items")
				{
					return GuiInstancePropertyInfo::Collection(description::GetTypeDescriptor<list::ListViewItem>());
				}
				else if (propertyInfo.propertyName == L"Columns")
				{
					return GuiInstancePropertyInfo::Collection(description::GetTypeDescriptor<list::ListViewColumn>());
				}
				else if (propertyInfo.propertyName == L"DataColumns")
				{
					return GuiInstancePropertyInfo::Collection(description::GetTypeDescriptor<vint>());
				}
				return IGuiInstanceLoader::GetPropertyType(propertyInfo);
			}

			bool SetPropertyValue(PropertyValue& propertyValue)override
			{
				if (GuiListView* container = dynamic_cast<GuiListView*>(propertyValue.instanceValue.GetRawPtr()))
				{
					if (propertyValue.propertyName == L"Items")
					{
						auto item = UnboxValue<Ptr<list::ListViewItem>>(propertyValue.propertyValue);
						container->GetItems().Add(item);
						return true;
					}
					else if (propertyValue.propertyName == L"Columns")
					{
						auto item = UnboxValue<Ptr<list::ListViewColumn>>(propertyValue.propertyValue);
						container->GetItems().GetColumns().Add(item);
						return true;
					}
					else if (propertyValue.propertyName == L"DataColumns")
					{
						auto item = UnboxValue<vint>(propertyValue.propertyValue);
						container->GetItems().GetDataColumns().Add(item);
						return true;
					}
				}
				return false;
			}
		};

/***********************************************************************
GuiTreeViewInstanceLoader
***********************************************************************/

		class GuiTreeViewInstanceLoader : public Object, public IGuiInstanceLoader
		{
		public:
			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<GuiTreeView>()->GetTypeName();
			}

			bool IsCreatable(const TypeInfo& typeInfo)override
			{
				return typeInfo.typeName == GetTypeName();
			}

			description::Value CreateInstance(const TypeInfo& typeInfo, collections::Group<WString, description::Value>& constructorArguments)override
			{
				if (typeInfo.typeName == GetTypeName())
				{
					GuiTreeView* treeView = new GuiTreeView(GetCurrentTheme()->CreateTreeViewStyle());

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
				if (typeInfo.typeName == GetTypeName())
				{
					propertyNames.Add(L"IconSize");
				}
				propertyNames.Add(L"Nodes");
			}

			void GetConstructorParameters(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				if (typeInfo.typeName == GetTypeName())
				{
					propertyNames.Add(L"IconSize");
				}
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"Nodes")
				{
					return GuiInstancePropertyInfo::Collection(description::GetTypeDescriptor<tree::MemoryNodeProvider>());
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

			description::Value CreateInstance(const TypeInfo& typeInfo, collections::Group<WString, description::Value>& constructorArguments)override
			{
				if (typeInfo.typeName == GetTypeName())
				{
					vint indexListControl = constructorArguments.Keys().IndexOf(L"ListControl");
					if (indexListControl != -1)
					{
						auto listControl = UnboxValue<GuiSelectableListControl*>(constructorArguments.GetByIndex(indexListControl)[0]);
						auto comboBox = new GuiComboBoxListControl(GetCurrentTheme()->CreateComboBoxStyle(), listControl);
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
						CopyFrom(options, UnboxValue<Ptr<IValueList>>(propertyValue.propertyValue)->GetLazyList<GuiCellOption>());
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
						CopyFrom(options, UnboxValue<Ptr<IValueList>>(propertyValue.propertyValue)->GetLazyList<GuiCellOption>());
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
GuiTextItemInstanceLoader
***********************************************************************/

		class GuiTextItemInstanceLoader : public Object, public IGuiInstanceLoader
		{
		public:

			WString GetTypeName()override
			{
				return description::GetTypeDescriptor<list::TextItem>()->GetTypeName();
			}

			bool IsCreatable(const TypeInfo& typeInfo)override
			{
				return typeInfo.typeName == GetTypeName();
			}

			description::Value CreateInstance(const TypeInfo& typeInfo, collections::Group<WString, description::Value>& constructorArguments)override
			{
				if (typeInfo.typeName == GetTypeName())
				{
					vint indexText = constructorArguments.Keys().IndexOf(L"Text");
					vint indexChecked = constructorArguments.Keys().IndexOf(L"Checked");

					if (indexText != -1)
					{
						WString text = UnboxValue<WString>(constructorArguments.GetByIndex(indexText)[0]);
						bool checked = false;
						if (indexChecked != -1)
						{
							checked=UnboxValue<bool>(constructorArguments.GetByIndex(indexChecked)[0]);
						}
						return Value::From(Ptr<list::TextItem>(new list::TextItem(text, checked)));
					}
				}
				return Value();
			}

			void GetPropertyNames(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				if (typeInfo.typeName == GetTypeName())
				{
					propertyNames.Add(L"Text");
					propertyNames.Add(L"Checked");
				}
			}

			void GetConstructorParameters(const TypeInfo& typeInfo, List<WString>& propertyNames)override
			{
				if (typeInfo.typeName == GetTypeName())
				{
					propertyNames.Add(L"Text");
					propertyNames.Add(L"Checked");
				}
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (propertyInfo.propertyName == L"Text")
				{
					auto info = GuiInstancePropertyInfo::Assign(description::GetTypeDescriptor<WString>());
					info->constructorParameter = true;
					info->required = true;
					return info;
				}
				else if (propertyInfo.propertyName == L"Checked")
				{
					auto info = GuiInstancePropertyInfo::Assign(description::GetTypeDescriptor<bool>());
					info->constructorParameter = true;
					return info;
				}
				return IGuiInstanceLoader::GetPropertyType(propertyInfo);
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

			description::Value CreateInstance(const TypeInfo& typeInfo, collections::Group<WString, description::Value>& constructorArguments)override
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
						}
					}
					else if (propertyValue.propertyName == L"Image")
					{
						if (auto item = container->GetData().Cast<tree::TreeViewItem>())
						{
							item->image = UnboxValue<Ptr<GuiImageData>>(propertyValue.propertyValue);
							container->NotifyDataModified();
						}
					}
					else if (propertyValue.propertyName == L"Tag")
					{
						if (auto item = container->GetData().Cast<tree::TreeViewItem>())
						{
							item->tag = propertyValue.propertyValue;
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

#define ADD_VIRTUAL_TYPE(VIRTUALTYPENAME, TYPENAME, CONSTRUCTOR)\
	manager->CreateVirtualType(\
		description::GetTypeDescriptor<TYPENAME>()->GetTypeName(),\
		new GuiVrtualTypeInstanceLoader(\
			L"presentation::controls::Gui" L#VIRTUALTYPENAME,\
			[](){return Value::From(CONSTRUCTOR());})\
		)

				manager->SetLoader(new GuiControlInstanceLoader);
				manager->SetLoader(new GuiControlHostInstanceLoader);
				manager->SetLoader(new GuiTabInstanceLoader);
				manager->SetLoader(new GuiTabPageInstanceLoader);
				manager->SetLoader(new GuiToolstripMenuInstanceLoader);
				manager->SetLoader(new GuiToolstripMenuBarInstanceLoader);
				manager->SetLoader(new GuiToolstripToolBarInstanceLoader);
				manager->SetLoader(new GuiToolstripButtonInstanceLoader);
				manager->SetLoader(new GuiListViewInstanceLoader);
				manager->SetLoader(new GuiTreeViewInstanceLoader);

				manager->SetLoader(new GuiCompositionInstanceLoader);
				manager->SetLoader(new GuiTableCompositionInstanceLoader);
				manager->SetLoader(new GuiCellCompositionInstanceLoader);

				manager->SetLoader(new GuiTextItemInstanceLoader);
				
				ADD_VIRTUAL_TYPE_LOADER(GuiComboBoxListControl,						GuiComboBoxInstanceLoader);
				ADD_VIRTUAL_TYPE_LOADER(tree::MemoryNodeProvider,					GuiTreeNodeInstanceLoader);

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