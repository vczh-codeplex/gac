/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Reflection: Compositions

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_REFLECTION_GUIREFLECTIONCOMPOSITIONS
#define VCZH_PRESENTATION_REFLECTION_GUIREFLECTIONCOMPOSITIONS

#include "GuiReflectionBasic.h"

namespace vl
{
	namespace reflection
	{
		namespace description
		{

/***********************************************************************
Type List
***********************************************************************/

#define GUIREFLECTIONCOMPOSITION_TYPELIST(F)\
			F(compositions::GuiStackComposition)\
			F(compositions::GuiStackComposition::Direction)\
			F(compositions::GuiStackItemComposition)\
			F(compositions::GuiCellOption)\
			F(compositions::GuiCellOption::ComposeType)\
			F(compositions::GuiTableComposition)\
			F(compositions::GuiCellComposition)\
			F(compositions::GuiSideAlignedComposition)\
			F(compositions::GuiSideAlignedComposition::Direction)\
			F(compositions::GuiPartialViewComposition)\

			GUIREFLECTIONCOMPOSITION_TYPELIST(DECL_TYPE_INFO)

/***********************************************************************
Type Loader
***********************************************************************/

			extern bool						LoadGuiCompositionTypes();
		}
	}
}

#endif