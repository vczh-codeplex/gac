/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Document Framework Base Implementations

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTEDITOR
#define GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTEDITOR

#include "..\DocumentFramework.h"

namespace vl
{
	namespace gactools
	{
		class DocumentEditor : public Object, public IDocumentEditor
		{
		public:
			DocumentEditor();
			~DocumentEditor();
		};
	}
}

#endif