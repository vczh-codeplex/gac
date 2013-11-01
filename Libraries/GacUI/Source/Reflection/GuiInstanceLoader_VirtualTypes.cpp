#include "GuiInstanceLoader.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;
		using namespace reflection::description;
		using namespace controls;
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
				else
				{
					IGuiInstanceLoader* parentLoader=GetInstanceLoaderManager()->GetParentLoader(this);
					if(parentLoader)
					{
						return parentLoader->CreateInstance(context, ctor, resolver, _typeName, typeDescriptor);
					}
				}
				return Value();
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
#define ADD_VIRTUAL_TYPE(VIRTUALTYPENAME, TYPENAME, CONSTRUCTOR)\
	manager->CreateVirtualType(\
		L"presentation::controls::Gui" L#VIRTUALTYPENAME,\
		L"presentation::controls::" L#TYPENAME,\
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