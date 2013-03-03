/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Reflection: Elements

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_REFLECTION_GUIREFLECTIONELEMENTS
#define VCZH_PRESENTATION_REFLECTION_GUIREFLECTIONELEMENTS

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

#define GUIREFLECTIONELEMENT_TYPELIST(F)\
			F(ElementShape)\
			F(GuiSolidBorderElement)\
			F(GuiRoundBorderElement)\
			F(Gui3DBorderElement)\
			F(Gui3DSplitterElement)\
			F(Gui3DSplitterElement::Direction)\
			F(GuiSolidBackgroundElement)\
			F(GuiGradientBackgroundElement)\
			F(GuiGradientBackgroundElement::Direction)\
			F(GuiSolidLabelElement)\
			F(GuiImageFrameElement)\
			F(GuiPolygonElement)\
			F(text::TextLines)\
			F(text::ColorItem)\
			F(text::ColorEntry)\
			F(GuiColorizedTextElement)\
			F(GuiDocumentElement)\

			GUIREFLECTIONELEMENT_TYPELIST(DECL_TYPE_INFO)

/***********************************************************************
Type Loader
***********************************************************************/

			extern bool						LoadGuiElementTypes();
		}
	}
}

#endif