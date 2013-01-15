#include "DocumentContainer.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
DocumentContainer
***********************************************************************/

		DocumentContainer::DocumentContainer()
		{
		}

		DocumentContainer::~DocumentContainer()
		{
			FOREACH(ICallback*, callback, callbacks)
			{
				callback->OnDocumentDestroyed(this);
			}
		}

		bool DocumentContainer::AttachCallback(ICallback* callback)
		{
			if(callbacks.Contains(callback))
			{
				return false;
			}
			else
			{
				callbacks.Add(callback);
				callback->OnAttach(this);
				return true;
			}
		}

		bool DocumentContainer::DetachCallback(ICallback* callback)
		{
			if(callbacks.Contains(callback))
			{
				callback->OnDetach(this);
				callbacks.Remove(callback);
				return true;
			}
			else
			{
				return false;
			}
		}

		IDocumentFragment* DocumentContainer::GetRootFragment()
		{
			return rootFragment.Obj();
		}

		void DocumentContainer::SetRootFragment(Ptr<IDocumentFragment> _rootFragment)
		{
			rootFragment=_rootFragment;
		}
	}
}