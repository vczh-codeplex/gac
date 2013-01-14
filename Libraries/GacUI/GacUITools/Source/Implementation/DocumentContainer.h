/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Document Framework Base Implementations

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTCONTAINER
#define GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTCONTAINER

#include "..\DocumentFramework.h"

namespace vl
{
	namespace gactools
	{
		class DocumentContainer : public Object, public IDocumentContainer
		{
		public:
			DocumentContainer();
			~DocumentContainer();
		};
	}
}

#endif