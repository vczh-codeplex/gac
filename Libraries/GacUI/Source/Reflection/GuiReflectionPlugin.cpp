#include "..\Reflection\GuiReflectionBasic.h"
#include "..\Reflection\GuiReflectionElements.h"
#include "..\Reflection\GuiReflectionCompositions.h"
#include "..\Reflection\GuiReflectionControls.h"
#include "..\Reflection\GuiReflectionEvents.h"

namespace vl
{
	namespace reflection
	{
		namespace description
		{
			class GuiReflectionPlugin : public Object, public IGuiPlugin
			{
			public:
				void Load()override
				{
					LoadPredefinedTypes();
					LoadParsingTypes();
					LoadGuiBasicTypes();
					LoadGuiElementTypes();
					LoadGuiCompositionTypes();
					LoadGuiControlsTypes();
					LoadGuiEventTypes();
				}

				void Unload()override
				{
				}
			};
			GUI_REGISTER_PLUGIN(GuiReflectionPlugin)
		}
	}
}