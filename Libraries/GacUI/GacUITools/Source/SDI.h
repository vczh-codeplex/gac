/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::SDI

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_SDI
#define GACUI_TOOLS_SDI

#include "DocumentFramework.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
SDI Application
***********************************************************************/

		struct SDIConfiguration
		{
			WString				applicationName;
		};

		extern int				StartSDI(const SDIConfiguration& config);
	}
}

#endif