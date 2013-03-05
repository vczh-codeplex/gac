#include "GuiReflectionCompositions.h"

namespace vl
{
	namespace reflection
	{
		namespace description
		{
			using namespace collections;
			using namespace parsing;
			using namespace parsing::tabling;
			using namespace parsing::xml;
			using namespace stream;

			GUIREFLECTIONCOMPOSITION_TYPELIST(IMPL_TYPE_INFO)

/***********************************************************************
External Functions
***********************************************************************/

/***********************************************************************
Type Declaration
***********************************************************************/

#define _ ,

			BEGIN_CLASS_MEMBER(GuiStackComposition)
			END_CLASS_MEMBER(GuiStackComposition)

			BEGIN_ENUM_ITEM(GuiStackComposition::Direction)
				ENUM_ITEM_NAMESPACE(GuiStackComposition)
				ENUM_NAMESPACE_ITEM(Horizontal)
				ENUM_NAMESPACE_ITEM(Vertical)
			END_ENUM_ITEM(GuiStackComposition::Direction)

			BEGIN_CLASS_MEMBER(GuiStackItemComposition)
			END_CLASS_MEMBER(GuiStackItemComposition)

			BEGIN_CLASS_MEMBER(GuiCellOption)
			END_CLASS_MEMBER(GuiCellOption)

			BEGIN_ENUM_ITEM(GuiCellOption::ComposeType)
				ENUM_ITEM_NAMESPACE(GuiCellOption)
				ENUM_NAMESPACE_ITEM(Absolute)
				ENUM_NAMESPACE_ITEM(Percentage)
				ENUM_NAMESPACE_ITEM(MinSize)
			END_ENUM_ITEM(GuiCellOption::ComposeType)

			BEGIN_CLASS_MEMBER(GuiTableComposition)
			END_CLASS_MEMBER(GuiTableComposition)

			BEGIN_CLASS_MEMBER(GuiCellComposition)
			END_CLASS_MEMBER(GuiCellComposition)

			BEGIN_CLASS_MEMBER(GuiSideAlignedComposition)
			END_CLASS_MEMBER(GuiSideAlignedComposition)

			BEGIN_ENUM_ITEM(GuiSideAlignedComposition::Direction)
				ENUM_ITEM_NAMESPACE(GuiSideAlignedComposition)
				ENUM_NAMESPACE_ITEM(Left)
				ENUM_NAMESPACE_ITEM(Top)
				ENUM_NAMESPACE_ITEM(Right)
				ENUM_NAMESPACE_ITEM(Bottom)
			END_ENUM_ITEM(GuiSideAlignedComposition::Direction)

			BEGIN_CLASS_MEMBER(GuiPartialViewComposition)
			END_CLASS_MEMBER(GuiPartialViewComposition)

#undef _

/***********************************************************************
Type Loader
***********************************************************************/

			class GuiCompositionTypeLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					GUIREFLECTIONCOMPOSITION_TYPELIST(ADD_TYPE_INFO)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};

			bool LoadGuiCompositionTypes()
			{
				ITypeManager* manager=GetGlobalTypeManager();
				if(manager)
				{
					Ptr<ITypeLoader> loader=new GuiCompositionTypeLoader;
					return manager->AddTypeLoader(loader);
				}
				return false;
			}
		}
	}
}