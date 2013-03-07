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
			F(presentation::elements::ElementShape)\
			F(presentation::elements::GuiSolidBorderElement)\
			F(presentation::elements::GuiRoundBorderElement)\
			F(presentation::elements::Gui3DBorderElement)\
			F(presentation::elements::Gui3DSplitterElement)\
			F(presentation::elements::Gui3DSplitterElement::Direction)\
			F(presentation::elements::GuiSolidBackgroundElement)\
			F(presentation::elements::GuiGradientBackgroundElement)\
			F(presentation::elements::GuiGradientBackgroundElement::Direction)\
			F(presentation::elements::GuiSolidLabelElement)\
			F(presentation::elements::GuiImageFrameElement)\
			F(presentation::elements::GuiPolygonElement)\
			F(presentation::elements::text::TextLines)\
			F(presentation::elements::text::ColorItem)\
			F(presentation::elements::text::ColorEntry)\
			F(presentation::elements::GuiColorizedTextElement)\
			F(presentation::elements::GuiDocumentElement)\

			GUIREFLECTIONELEMENT_TYPELIST(DECL_TYPE_INFO)

/***********************************************************************
Type Loader
***********************************************************************/

			extern bool						LoadGuiElementTypes();
		}
	}
}

#endif