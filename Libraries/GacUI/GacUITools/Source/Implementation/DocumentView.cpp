#include "DocumentView.h"
#include "DocumentFragment.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
DocumentView
***********************************************************************/

		const List<DocumentView::ICallback*>& DocumentView::GetCallbacks()
		{
			return callbacks;
		}

		bool DocumentView::AddSupportedOperation(Ptr<IDocumentOperation> operation)
		{
			if(supportedOperations.Keys().Contains(operation->GetOperationTypeId())) return false;
			supportedOperations.Add(operation->GetOperationTypeId(), operation);
			return true;
		}

		DocumentView::DocumentView(DocumentFragment* _ownedFragment, const WString& _id, const WString& _friendlyName)
			:ownedFragment(_ownedFragment)
			,id(_id)
			,friendlyName(_friendlyName)
			,currentEditor(0)
		{
		}

		DocumentView::~DocumentView()
		{
			FOREACH(ICallback*, callback, callbacks)
			{
				callback->OnViewDestroyed(this);
			}
		}

		void DocumentView::NotifyUpdateView()
		{
			FOREACH(ICallback*, callback, callbacks)
			{
				callback->OnViewUpdated(this);
			}
			ownedFragment->NotifyUpdateFragmentAndViews(this);
		}

		bool DocumentView::AttachCallback(ICallback* callback)
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

		bool DocumentView::DetachCallback(ICallback* callback)
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

		IDocumentFragment* DocumentView::GetOwnedFragment()
		{
			return ownedFragment;
		}

		WString DocumentView::GetViewTypeId()
		{
			return id;
		}

		WString DocumentView::GetViewTypeFriendlyName()
		{
			return friendlyName;
		}

		bool DocumentView::IsReadOnlyView()
		{
			return false;
		}

		vint DocumentView::GetSupportedOperationTypeCount()
		{
			return supportedOperations.Count();
		}

		WString DocumentView::GetSupportedOperationType(vint index)
		{
			return supportedOperations.Keys().Get(index);
		}

		bool DocumentView::IsSupportedOperationTypeId(const WString& operationTypeId)
		{
			return supportedOperations.Keys().Contains(operationTypeId);
		}

		IDocumentOperation* DocumentView::GetOperation(const WString& operationTypeId)
		{
			vint index=supportedOperations.Keys().IndexOf(operationTypeId);
			return index==-1?0:supportedOperations.Values().Get(index).Obj();
		}

		bool DocumentView::BeginEdit(IDocumentEditor* editor)
		{
			if(currentEditor || !editor) return false;
			for(vint i=0;i<ownedFragment->GetSupportedViewTypeCount();i++)
			{
				WString viewTypeId=ownedFragment->GetSupportedViewTypeId(i);
				IDocumentView* view=ownedFragment->GetView(viewTypeId);
				if(view && view!=this)
				{
					if(!view->IsReadOnlyView() || !this->IsReadOnlyView())
					{
						return false;
					}
				}
			}
			currentEditor=editor;
			FOREACH(ICallback*, callback, callbacks)
			{
				callback->OnBeginEdit(this);
			}
			return true;
		}

		bool DocumentView::FinishEdit(IDocumentEditor* editor)
		{
			if(!currentEditor || currentEditor!=editor) return false;
			FOREACH(ICallback*, callback, callbacks)
			{
				callback->OnFinishEdit(this);
			}
			currentEditor=0;
			return true;
		}

		IDocumentEditor* DocumentView::GetEditor()
		{
			return currentEditor;
		}
	}
}