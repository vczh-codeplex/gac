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
			IDocumentFileType*			ownedFileType;

		public:
			DocumentContainer(IDocumentFileType* _ownedFileType);
			~DocumentContainer();

			bool						AttachCallback(ICallback* callback)override;
			bool						DetachCallback(ICallback* callback)override;
			IDocumentFileType*			GetOwnedFileType()override;

			IDocumentFragment*			GetRootFragment()override;
			void						SetRootFragment(Ptr<IDocumentFragment> _rootFragment);
		};
	}
}

#endif