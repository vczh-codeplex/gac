/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::SDI

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_SDI
#define GACUI_TOOLS_SDI

#include "DocumentFramework.h"
#include "ResourceManager.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
SDI Application
***********************************************************************/

		class ISDIApplication : public Interface
		{
		public:
			virtual Ptr<GuiResource>	GetApplicationResource()=0;
		};

		extern int						StartSDI(ISDIApplication* application);
	}
}

#endif