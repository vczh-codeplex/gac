#include "GuiInstanceLoader.h"
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

			WString GetTypeName()
			{
				return typeName;
			}

			description::Value CreateInstance(
				Ptr<GuiInstanceContext> context,
				Ptr<GuiConstructorRepr> ctor,
				Ptr<GuiResourcePathResolver> resolver,
				const WString& _typeName,
				description::ITypeDescriptor* typeDescriptor
				)
			{
				if(typeName==_typeName)
				{
					return constructor();
				}
				return Value();
			}

			IGuiInstanceLoader::PropertyType GetPropertyType(
				const WString& typeName,
				description::ITypeDescriptor* typeDescriptor,
				const WString& propertyName,
				description::ITypeDescriptor*& elementType,
				bool &nullable
				)override
			{
				return IGuiInstanceLoader::HandleByParentLoader;
			}

			description::Value GetPropertyValue(
				description::Value createdInstance,
				const WString& typeName,
				description::ITypeDescriptor* typeDescriptor,
				const WString& propertyName
				)override
			{
				return Value();
			}

			bool SetPropertyValue(
				description::Value createdInstance,
				const WString& typeName,
				description::ITypeDescriptor* typeDescriptor,
				const WString& propertyName,
				description::Value propertyValue
				)override
			{
				return false;
			}

			bool SetPropertyCollection(
				description::Value createdInstance,
				const WString& typeName,
				description::ITypeDescriptor* typeDescriptor,
				const WString& propertyName,
				description::Value propertyValue
				)override
			{
				return false;
			}
		};

/***********************************************************************
GuiRewriteInstanceLoader
***********************************************************************/

		class GuiRewriteInstanceLoader : public Object, public IGuiInstanceLoader
		{
		public:
			GuiRewriteInstanceLoader()
			{
			}

			description::Value CreateInstance(
				Ptr<GuiInstanceContext> context,
				Ptr<GuiConstructorRepr> ctor,
				Ptr<GuiResourcePathResolver> resolver,
				const WString& _typeName,
				description::ITypeDescriptor* typeDescriptor
				)
			{
				return Value();
			}

			IGuiInstanceLoader::PropertyType GetPropertyType(
				const WString& typeName,
				description::ITypeDescriptor* typeDescriptor,
				const WString& propertyName,
				description::ITypeDescriptor*& elementType,
				bool &nullable
				)override
			{
				return IGuiInstanceLoader::HandleByParentLoader;
			}

			description::Value GetPropertyValue(
				description::Value createdInstance,
				const WString& typeName,
				description::ITypeDescriptor* typeDescriptor,
				const WString& propertyName
				)override
			{
				return Value();
			}

			bool SetPropertyValue(
				description::Value createdInstance,
				const WString& typeName,
				description::ITypeDescriptor* typeDescriptor,
				const WString& propertyName,
				description::Value propertyValue
				)override
			{
				return false;
			}

			bool SetPropertyCollection(
				description::Value createdInstance,
				const WString& typeName,
				description::ITypeDescriptor* typeDescriptor,
				const WString& propertyName,
				description::Value propertyValue
				)override
			{
				return false;
			}
		};

/***********************************************************************
GuiControlInstanceLoader
***********************************************************************/

		class GuiControlInstanceLoader : public GuiRewriteInstanceLoader
		{
		public:
			GuiControlInstanceLoader()
			{
			}

			WString GetTypeName()
			{
				return description::GetTypeDescriptor<GuiControl>()->GetTypeName();
			}

			IGuiInstanceLoader::PropertyType GetPropertyType(
				const WString& typeName,
				description::ITypeDescriptor* typeDescriptor,
				const WString& propertyName,
				description::ITypeDescriptor*& elementType,
				bool &nullable
				)override
			{
				if(propertyName==L"")
				{
					elementType=description::GetTypeDescriptor<Value>();
					nullable=false;
				}
				return IGuiInstanceLoader::HandleByParentLoader;
			}

			description::Value GetPropertyValue(
				description::Value createdInstance,
				const WString& typeName,
				description::ITypeDescriptor* typeDescriptor,
				const WString& propertyName
				)override
			{
				return Value();
			}

			bool SetPropertyCollection(
				description::Value createdInstance,
				const WString& typeName,
				description::ITypeDescriptor* typeDescriptor,
				const WString& propertyName,
				description::Value propertyValue
				)override
			{
				if(GuiControl* container=dynamic_cast<GuiControl*>(createdInstance.GetRawPtr()))
				{
					if(auto control=dynamic_cast<GuiControl*>(propertyValue.GetRawPtr()))
					{
						container->AddChild(control);
					}
					else if(auto composition=dynamic_cast<GuiGraphicsComposition*>(propertyValue.GetRawPtr()))
					{
						container->GetBoundsComposition()->AddChild(composition);
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
			GuiCompositionInstanceLoader()
			{
			}

			WString GetTypeName()
			{
				return description::GetTypeDescriptor<GuiGraphicsComposition>()->GetTypeName();
			}

			IGuiInstanceLoader::PropertyType GetPropertyType(
				const WString& typeName,
				description::ITypeDescriptor* typeDescriptor,
				const WString& propertyName,
				description::ITypeDescriptor*& elementType,
				bool &nullable
				)override
			{
				return IGuiInstanceLoader::HandleByParentLoader;
			}

			description::Value GetPropertyValue(
				description::Value createdInstance,
				const WString& typeName,
				description::ITypeDescriptor* typeDescriptor,
				const WString& propertyName
				)override
			{
				return Value();
			}

			bool SetPropertyCollection(
				description::Value createdInstance,
				const WString& typeName,
				description::ITypeDescriptor* typeDescriptor,
				const WString& propertyName,
				description::Value propertyValue
				)override
			{
				return false;
			}
		};

/***********************************************************************
GuiInstanceLoaderVirtualTypesPlugin
***********************************************************************/

		class GuiInstanceLoaderVirtualTypesPlugin : public Object, public IGuiPlugin
		{
		public:
			void Load()override
			{
			}

			void AfterLoad()override
			{
				IGuiInstanceLoaderManager* manager=GetInstanceLoaderManager();

				manager->SetLoader(new GuiControlInstanceLoader);
				manager->SetLoader(new GuiCompositionInstanceLoader);

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
				ADD_VIRTUAL_TYPE(ToolstripButton,			GuiToolstripButton,		g::NewToolbarButton);
				ADD_VIRTUAL_TYPE(ToolstripDropdownButton,	GuiToolstripButton,		g::NewToolbarDropdownButton);
				ADD_VIRTUAL_TYPE(ToolstripSplitButton,		GuiToolstripButton,		g::NewToolbarSplitButton);
				ADD_VIRTUAL_TYPE(ToolstripSplitter,			GuiControl,				g::NewToolbarSplitter);
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
			}

			void Unload()override
			{
			}
		};
		GUI_REGISTER_PLUGIN(GuiInstanceLoaderVirtualTypesPlugin)
	}
}