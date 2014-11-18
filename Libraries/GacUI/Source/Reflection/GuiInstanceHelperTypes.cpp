#include "GuiInstanceHelperTypes.h"
#include "../Controls/GuiApplication.h"

namespace vl
{
	namespace presentation
	{
		namespace helper_types
		{
		}
	}

#ifndef VCZH_DEBUG_NO_REFLECTION

	namespace reflection
	{
		namespace description
		{
			using namespace presentation::helper_types;

/***********************************************************************
Type Declaration
***********************************************************************/

			GUIREFLECTIONHELPERTYPES_TYPELIST(IMPL_TYPE_INFO)

#define _ ,

			BEGIN_STRUCT_MEMBER(SiteValue)
				STRUCT_MEMBER(row)
				STRUCT_MEMBER(column)
				STRUCT_MEMBER(rowSpan)
				STRUCT_MEMBER(columnSpan)
			END_STRUCT_MEMBER(SiteValue)

			BEGIN_ENUM_ITEM(ListViewViewType)
				ENUM_ITEM_NAMESPACE(ListViewViewType)
				ENUM_NAMESPACE_ITEM(BigIcon)
				ENUM_NAMESPACE_ITEM(SmallIcon)
				ENUM_NAMESPACE_ITEM(List)
				ENUM_NAMESPACE_ITEM(Tile)
				ENUM_NAMESPACE_ITEM(Information)
				ENUM_NAMESPACE_ITEM(Detail)
			END_ENUM_ITEM(ListViewViewType)

#undef _
		}
	}

	namespace presentation
	{
		using namespace reflection::description;
		using namespace controls;

/***********************************************************************
Type Loader
***********************************************************************/

			class GuiHelperTypesLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					GUIREFLECTIONHELPERTYPES_TYPELIST(ADD_TYPE_INFO)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};

/***********************************************************************
GuiHelperTypesLoaderPlugin
***********************************************************************/

		class GuiHelperTypesLoaderPlugin : public Object, public IGuiPlugin
		{
		public:
			void Load()override
			{
				ITypeManager* manager=GetGlobalTypeManager();
				if(manager)
				{
					Ptr<ITypeLoader> loader=new GuiHelperTypesLoader;
					manager->AddTypeLoader(loader);
				}
			}

			void AfterLoad()override
			{
			}

			void Unload()override
			{
			}
		};
		GUI_REGISTER_PLUGIN(GuiHelperTypesLoaderPlugin)
	}
#endif
}