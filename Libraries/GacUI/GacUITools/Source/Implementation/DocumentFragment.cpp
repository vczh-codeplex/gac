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
			return true;
		}

		DocumentFragment* DocumentFragment::GetOwnerFragmentInternal()
		{
			return ownedFragment;
		}

		DocumentFragment::DocumentFragment(IDocumentContainer* _ownedContainer, DocumentFragment* _ownedFragment, const WString& _friendlyName)
			:ownedContainer(_ownedContainer)
			,ownedFragment(_ownedFragment)
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

		void DocumentFragment::NotifyUpdateFragment()
		{
			FOREACH(ICallback*, callback, callbacks)
			{
				callback->OnFragmentUpdated(this);
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
			return ownedContainer;
		}

		IDocumentFragment* DocumentFragment::GetOwnedFragment()
		{
			return ownedFragment;
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

/***********************************************************************
FileDocumentFragment
***********************************************************************/

		void FileDocumentFragment::NotifyUpdateFragment()
		{
			modified=true;
			DocumentFragment::NotifyUpdateFragment();
		}

		FileDocumentFragment::FileDocumentFragment(IDocumentContainer* _ownedContainer, DocumentFragment* _ownedFragment, const WString& _friendlyName, const WString& _filePath)
			:DocumentFragment(_ownedContainer, _ownedFragment, _friendlyName)
			,currentFilePath(_filePath)
			,modified(false)
		{
		}

		FileDocumentFragment::~FileDocumentFragment()
		{
		}

		bool FileDocumentFragment::IsStoredInSeparatedFile()
		{
			return true;
		}

		bool FileDocumentFragment::CanSaveSeparately()
		{
			return true;
		}

		bool FileDocumentFragment::CanSaveToAnotherFile()
		{
			return true;
		}

		WString FileDocumentFragment::GetFilePath()
		{
			return currentFilePath;
		}

		bool FileDocumentFragment::ReloadDocument()
		{
			if(currentFilePath==L"") return false;
			return LoadDocumentInternal(currentFilePath);
		}

		bool FileDocumentFragment::SaveDocument()
		{
			if(currentFilePath==L"") return false;
			if(!SaveDocumentInternal(currentFilePath)) return false;
			modified=false;
			return true;
		}

		bool FileDocumentFragment::SaveDocumentAs(const WString& filePath)
		{
			if(!SaveDocumentInternal(filePath)) return false;
			currentFilePath=filePath;
			modified=false;
			return true;
		}

		bool FileDocumentFragment::IsModified()
		{
			return modified;
		}

/***********************************************************************
VirtualDocumentFragment
***********************************************************************/

		void VirtualDocumentFragment::NotifyUpdateFragment()
		{
			DocumentFragment::NotifyUpdateFragment();
			return GetOwnerFragmentInternal()->NotifyUpdateFragment();
		}

		VirtualDocumentFragment::VirtualDocumentFragment(IDocumentContainer* _ownedContainer, DocumentFragment* _ownedFragment, const WString& _friendlyName)
			:DocumentFragment(_ownedContainer, _ownedFragment, _friendlyName)
		{
		}

		VirtualDocumentFragment::~VirtualDocumentFragment()
		{
		}

		bool VirtualDocumentFragment::IsStoredInSeparatedFile()
		{
			return false;
		}

		bool VirtualDocumentFragment::CanSaveSeparately()
		{
			return false;
		}

		bool VirtualDocumentFragment::CanSaveToAnotherFile()
		{
			return false;
		}

		WString VirtualDocumentFragment::GetFilePath()
		{
			return L"";
		}

		bool VirtualDocumentFragment::ReloadDocument()
		{
			return false;
		}

		bool VirtualDocumentFragment::SaveDocument()
		{
			return false;
		}

		bool VirtualDocumentFragment::SaveDocumentAs(const WString& filePath)
		{
			return false;
		}

		bool VirtualDocumentFragment::IsModified()
		{
			return GetOwnerFragmentInternal()->IsModified();
		}
	}
}