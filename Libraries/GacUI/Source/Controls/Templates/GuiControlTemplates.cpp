#include "GuiControlTemplates.h"

namespace vl
{
	namespace presentation
	{
		namespace templates
		{

/***********************************************************************
GuiTemplate
***********************************************************************/

			GuiTemplate::GuiTemplate()
			{
			}

			GuiTemplate::~GuiTemplate()
			{
			}

/***********************************************************************
GuiListItemTemplate
***********************************************************************/

			GuiListItemTemplate_PROPERTIES(GUI_TEMPLATE_PROPERTY_IMPL)

			GuiListItemTemplate::GuiListItemTemplate()
				:Selected_(false)
				, Index_(0)
			{
				GuiListItemTemplate_PROPERTIES(GUI_TEMPLATE_PROPERTY_EVENT_INIT)
			}

			GuiListItemTemplate::~GuiListItemTemplate()
			{
			}
		}
	}
}