/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Package Manager

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_PACKAGEMANAGER
#define GACUI_TOOLS_PACKAGEMANAGER

#include "DocumentFramework.h"
#include "ResourceManager.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
UI Builder
***********************************************************************/

		extern void			EnumeratePackages(Ptr<GuiResource> resource, List<Ptr<XmlElement>>& packages);
		extern void			EnumerateCommands(List<Ptr<XmlElement>>& packages, Dictionary<WString, Ptr<GuiToolstripCommand>>& commands);
	}
}

#endif