/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Element System and Infrastructure Interfaces

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSDOCUMENTINTERFACES
#define VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSDOCUMENTINTERFACES

#include "..\NativeWindow\GuiTypes.h"

namespace vl
{
	namespace presentation
	{
		using namespace reflection;

		namespace elements
		{

/***********************************************************************
Layout Engine
***********************************************************************/

			/// <summary>Renderer awared rich text document layout engine provider interface.</summary>
			class IGuiGraphicsLayoutProvider : public Interface
			{
			public:
			};
		}
	}
}

#endif