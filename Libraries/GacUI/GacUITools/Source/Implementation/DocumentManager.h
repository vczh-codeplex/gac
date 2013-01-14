/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Document Framework Base Implementations

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTMANAGER
#define GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTMANAGER

#include "..\DocumentFramework.h"

namespace vl
{
	namespace gactools
	{
		class DocumentManager : public Object, public IDocumentManager
		{
		public:
			DocumentManager();
			~DocumentManager();
		};
	}
}

#endif