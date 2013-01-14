/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Document Framework Base Implementations

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTVIEW
#define GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTVIEW

#include "..\DocumentFramework.h"

namespace vl
{
	namespace gactools
	{
		class DocumentView : public Object, public IDocumentView
		{
		public:
			DocumentView();
			~DocumentView();
		};
	}
}

#endif