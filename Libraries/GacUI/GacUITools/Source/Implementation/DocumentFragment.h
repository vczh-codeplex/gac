/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Document Framework Base Implementations

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTFRAGMENT
#define GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTFRAGMENT

#include "..\DocumentFramework.h"

namespace vl
{
	namespace gactools
	{
		class DocumentFragment : public Object, public IDocumentFragment
		{
		public:
			DocumentFragment();
			~DocumentFragment();
		};
	}
}

#endif