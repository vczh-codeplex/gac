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
			F(elements::ElementShape)\
			F(elements::GuiSolidBorderElement)\
			F(elements::GuiRoundBorderElement)\
			F(elements::Gui3DBorderElement)\
			F(elements::Gui3DSplitterElement)\
			F(elements::Gui3DSplitterElement::Direction)\
			F(elements::GuiSolidBackgroundElement)\
			F(elements::GuiGradientBackgroundElement)\
			F(elements::GuiGradientBackgroundElement::Direction)\
			F(elements::GuiSolidLabelElement)\
			F(elements::GuiImageFrameElement)\
			F(elements::GuiPolygonElement)\
			F(elements::text::TextLines)\
			F(elements::text::ColorItem)\
			F(elements::text::ColorEntry)\
			F(elements::GuiColorizedTextElement)\
			F(elements::GuiDocumentElement)\

			GUIREFLECTIONELEMENT_TYPELIST(DECL_TYPE_INFO)

/***********************************************************************
Type Loader
***********************************************************************/

			extern bool						LoadGuiElementTypes();
		}
	}
}

#endif