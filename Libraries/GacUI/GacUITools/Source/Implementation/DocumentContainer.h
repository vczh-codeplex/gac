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
		private:
			List<ICallback*>			callbacks;
			Ptr<IDocumentFragment>		rootFragment;

		public:
			DocumentContainer(Ptr<IDocumentFragment> _rootFragment);
			~DocumentContainer();

			bool						AttachCallback(ICallback* callback)override;
			bool						DetachCallback(ICallback* callback)override;

			IDocumentFragment*			GetRootFragment()override;
		};
	}
}

#endif