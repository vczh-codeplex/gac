#include "DocumentEditor.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
DocumentEditor::DocumentViewCallback
***********************************************************************/

		DocumentEditor::DocumentViewCallback::DocumentViewCallback(DocumentEditor* _ownedEditor)
			:ownedEditor(_ownedEditor)
			,view(0)
		{
		}

		DocumentEditor::DocumentViewCallback::~DocumentViewCallback()
		{
			if(view)
			{
				view->DetachCallback(this);
			}
		}

		void DocumentEditor::DocumentViewCallback::OnAttach(IDocumentView* sender)
		{
			view=sender;
		}

		void DocumentEditor::DocumentViewCallback::OnDetach(IDocumentView* sender)
		{
			view=0;
		}

		void DocumentEditor::DocumentViewCallback::OnViewUpdated(IDocumentView* sender)
		{
		}

		void DocumentEditor::DocumentViewCallback::OnViewDestroyed(IDocumentView* sender)
		{
			ownedEditor->OnLostActiveView();
			view=0;
		}

		void DocumentEditor::DocumentViewCallback::OnBeginEdit(IDocumentView* sender)
		{
		}

		void DocumentEditor::DocumentViewCallback::OnFinishEdit(IDocumentView* sender)
		{
		}

/***********************************************************************
DocumentEditor
***********************************************************************/

		void DocumentEditor::OnLostActiveView()
		{
			if(editingView)
			{
				AfterEditView(editingView);
				FOREACH(ICallback*, callback, callbacks)
				{
					callback->OnLostActiveView(this);
				}
				editingView=0;
			}
		}

		const List<DocumentEditor::ICallback*>& DocumentEditor::GetCallbacks()
		{
			return callbacks;
		}

		bool DocumentEditor::AddSupportedOperation(Ptr<IDocumentOperation> operation)
		{
			if(supportedOperations.Keys().Contains(operation->GetOperationTypeId())) return false;
			supportedOperations.Add(operation->GetOperationTypeId(), operation);
			return true;
		}

		DocumentEditor::DocumentEditor(IDocumentEditorFactory* _editorFactory, IDocumentView* _editingView)
			:editorFactory(_editorFactory)
			,editingView(_editingView)
			,editorControl(0)
		{
			documentViewCallback=new DocumentViewCallback(this);
			editingView->AttachCallback(documentViewCallback.Obj());
		}

		DocumentEditor::~DocumentEditor()
		{
			FinishEdit();
			FOREACH(ICallback*, callback, callbacks)
			{
				callback->OnEditorDestroyed(this);
			}
			if(editorControl)
			{
				delete editorControl;
			}
		}

		void DocumentEditor::NotifyUpdateEditor()
		{
			FOREACH(ICallback*, callback, callbacks)
			{
				callback->OnEditorUpdated(this);
			}
		}

		bool DocumentEditor::AttachCallback(ICallback* callback)
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

		bool DocumentEditor::DetachCallback(ICallback* callback)
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

		IDocumentEditorFactory* DocumentEditor::GetOwnedFactory()
		{
			return editorFactory;
		}

		GuiControl* DocumentEditor::GetEditorControl()
		{
			if(!editorControl)
			{
				editorControl=CreateEditorControlInternal();
				BeforeEditView(editingView);
			}
			return editorControl;
		}

		IDocumentView* DocumentEditor::GetEditingView()
		{
			return editingView;
		}

		bool DocumentEditor::FinishEdit()
		{
			if(editingView)
			{
				AfterEditView(editingView);
				editingView->DetachCallback(documentViewCallback.Obj());
				documentViewCallback=0;
				editingView->FinishEdit(this);
				FOREACH(ICallback*, callback, callbacks)
				{
					callback->OnFinishEdit(this);
				}
				editingView=0;
				return true;
			}
			else
			{
				return false;
			}
		}

		bool DocumentEditor::IsAvailable()
		{
			return editorControl!=0;
		}

		vint DocumentEditor::GetSupportedOperationTypeCount()
		{
			return supportedOperations.Count();
		}

		WString DocumentEditor::GetSupportedOperationType(vint index)
		{
			return supportedOperations.Keys().Get(index);
		}

		bool DocumentEditor::IsSupportedOperationTypeId(const WString& operationTypeId)
		{
			return supportedOperations.Keys().Contains(operationTypeId);
		}

		IDocumentOperation* DocumentEditor::GetOperation(const WString& operationTypeId)
		{
			vint index=supportedOperations.Keys().IndexOf(operationTypeId);
			return index==-1?0:supportedOperations.Values().Get(index).Obj();
		}
	}
}