/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Document Framework Base Implementations

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTEDITORFACTORY
#define GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTEDITORFACTORY

#include "..\DocumentFramework.h"

namespace vl
{
	namespace gactools
	{
		class DocumentEditorFactory : public Object, public IDocumentEditorFactory
		{
		public:
			DocumentEditorFactory();
			~DocumentEditorFactory();
		};
	}
}

#endif