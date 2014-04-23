/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GacUI Reflection: Instance Query

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_REFLECTION_INSTANCEQUERY_GUIINSTANCEQUERY
#define VCZH_PRESENTATION_REFLECTION_INSTANCEQUERY_GUIINSTANCEQUERY

#include "GuiInstanceQuery_Parser.h"
#include "..\GuiInstanceRepresentation.h"

namespace vl
{
	namespace presentation
	{
		extern void ExecuteQuery(Ptr<GuiIqQuery> query, Ptr<GuiInstanceContext> context, collections::List<Ptr<GuiAttSetterRepr>>& input, collections::List<Ptr<GuiAttSetterRepr>>& output);
		extern void ExecuteQuery(Ptr<GuiIqQuery> query, Ptr<GuiInstanceContext> context, collections::List<Ptr<GuiAttSetterRepr>>& output);
	}
}

#endif