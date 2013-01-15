#include "DocumentFragment.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
DocumentFragment
***********************************************************************/

		const List<DocumentFragment::ICallback*>& DocumentFragment::GetCallbacks()
		{
			return callbacks;
		}

		bool DocumentFragment::AddSubFragment(IDocumentFragment* fragment)
		{
			if(subFragments.Contains(fragment)) return false;
			subFragments.Add(fragment);
			return true;
		}

		bool DocumentFragment::DeleteSubFragment(IDocumentFragment* fragment)
		{
			return subFragments.Remove(fragment);
		}

		bool DocumentFragment::AddSupportedView(Ptr<IDocumentView> view)
		{
			if(supportedViews.Keys().Contains(view->GetViewTypeId())) return false;
			supportedViews.Add(view->GetViewTypeId(), view);
			return true;
		}

		bool DocumentFragment::SetDefaultView(Ptr<IDocumentView> view)
		{
			if(!supportedViews.Keys().Contains(view->GetViewTypeId()) || supportedViews[view->GetViewTypeId()]!=view) return false;
			defaultViewTypeId=view->GetViewTypeId();
		}

		void DocumentFragment::NotifyUpdateFragment()
		{
			FOREACH(ICallback*, callback, callbacks)
			{
				callback->OnFragmentUpdated(this);
			}
		}

		DocumentFragment::DocumentFragment(IDocumentContainer* _ownerContainer, IDocumentFragment* _ownerFragment, const WString& _friendlyName)
			:ownerContainer(_ownerContainer)
			,ownerFragment(_ownerFragment)
			,friendlyName(_friendlyName)
		{
		}

		DocumentFragment::~DocumentFragment()
		{
			FOREACH(ICallback*, callback, callbacks)
			{
				callback->OnFragmentDestroyed(this);
			}
		}

		bool DocumentFragment::AttachCallback(ICallback* callback)
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

		bool DocumentFragment::DetachCallback(ICallback* callback)
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

		IDocumentContainer* DocumentFragment::GetOwnedContainer()
		{
			return ownerContainer;
		}

		IDocumentFragment* DocumentFragment::GetOwnedFragment()
		{
			return ownerFragment;
		}

		vint DocumentFragment::GetSubFragmentCount()
		{
			return subFragments.Count();
		}

		IDocumentFragment* DocumentFragment::GetSubFragment(vint index)
		{
			return subFragments[index].Obj();
		}

		WString DocumentFragment::GetFriendlyName()
		{
			return friendlyName;
		}

		vint DocumentFragment::GetSupportedViewTypeCount()
		{
			return supportedViews.Count();
		}

		WString DocumentFragment::GetSupportedViewTypeId(vint index)
		{
			return supportedViews.Keys().Get(index);
		}

		bool DocumentFragment::IsSupportedViewTypeId(const WString& viewTypeId)
		{
			return supportedViews.Keys().Contains(viewTypeId);
		}

		WString DocumentFragment::GetDefaultViewTypeId()
		{
			return defaultViewTypeId;
		}

		IDocumentView* DocumentFragment::GetView(const WString& viewTypeId)
		{
			vint index=supportedViews.Keys().IndexOf(viewTypeId);
			return index==-1?0:supportedViews.Values().Get(index).Obj();
		}
	}
}