/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Document Framework Base Implementations

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTFILETYPE
#define GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTFILETYPE

#include "..\DocumentFramework.h"

namespace vl
{
	namespace gactools
	{
		class DocumentFileType : public Object, public IDocumentFileType
		{
		public:
			DocumentFileType();
			~DocumentFileType();
		};
	}
}

#endif