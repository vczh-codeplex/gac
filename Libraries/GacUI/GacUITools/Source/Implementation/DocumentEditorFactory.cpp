#include "DocumentEditorFactory.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
DocumentEditorFactory
***********************************************************************/

		bool DocumentEditorFactory::AddSupportedViewTypeId(const WString& viewTypeId)
		{
			if(supportedViewTypeIds.Contains(viewTypeId))
			{
				return false;
			}
			else
			{
				supportedViewTypeIds.Add(viewTypeId);
				return true;
			}
		}

		DocumentEditorFactory::DocumentEditorFactory(const WString& _id, const WString& _friendlyName)
			:id(_id)
			,friendlyName(_friendlyName)
		{
		}

		DocumentEditorFactory::~DocumentEditorFactory()
		{
		}

		bool DocumentEditorFactory::AttachCallback(ICallback* callback)
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

		bool DocumentEditorFactory::DetachCallback(ICallback* callback)
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

		WString DocumentEditorFactory::GetEditorTypeId()
		{
			return id;
		}

		WString DocumentEditorFactory::GetEditorTypeFriendlyName()
		{
			return friendlyName;
		}

		vint DocumentEditorFactory::GetSupportedViewTypeCount()
		{
			return supportedViewTypeIds.Count();
		}

		WString DocumentEditorFactory::GetSupportedViewTypeId(vint index)
		{
			return supportedViewTypeIds[index];
		}

		bool DocumentEditorFactory::IsSupportedViewTypeId(const WString& viewTypeId)
		{
			return supportedViewTypeIds.Contains(viewTypeId);
		}

		Ptr<IDocumentEditor> DocumentEditorFactory::CreateEditor(IDocumentView* view)
		{
			Ptr<IDocumentEditor> editor=CreateEditorInternal(view);
			if(editor)
			{
				view->BeginEdit(editor.Obj());
				FOREACH(ICallback*, callback, callbacks)
				{
					callback->OnEditorCreated(this, editor.Obj());
				}
			}
			return editor;
		}
	}
}