/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Document Framework Base Implementations

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTEDITOR
#define GACUI_TOOLS_DOCUMENTFRAMEWORK_DOCUMENTEDITOR

#include "..\DocumentFramework.h"

namespace vl
{
	namespace gactools
	{
		class DocumentEditor : public Object, public IDocumentEditor
		{
		private:
			class DocumentViewCallback : public Object, public IDocumentView::ICallback
			{
			private:
				DocumentEditor*					ownedEditor;
				IDocumentView*					view;
			public:
				DocumentViewCallback(DocumentEditor* _ownedEditor);
				~DocumentViewCallback();

				void							OnAttach(IDocumentView* sender)override;
				void							OnDetach(IDocumentView* sender)override;
				void							OnViewUpdated(IDocumentView* sender)override;
				void							OnViewDestroyed(IDocumentView* sender)override;
				void							OnBeginEdit(IDocumentView* sender)override;
				void							OnFinishEdit(IDocumentView* sender)override;
				void							OnFragmentSaved(IDocumentView* sender)override;
			};

			List<ICallback*>									callbacks;
			Ptr<DocumentViewCallback>							documentViewCallback;
			IDocumentEditorFactory*								editorFactory;
			IDocumentView*										editingView;
			GuiControl*											editorControl;
			Dictionary<WString, Ptr<IDocumentEditorOperation>>	supportedOperations;

			void								OnLostActiveView();
		protected:

			const List<ICallback*>&				GetCallbacks();
			bool								AddSupportedOperation(Ptr<IDocumentEditorOperation> operation);
			virtual void						BeforeEditView(IDocumentView* view)=0;
			virtual void						AfterEditView(IDocumentView* view)=0;
			virtual void						OnViewSaved(IDocumentView* view)=0;
			virtual GuiControl*					CreateEditorControlInternal()=0;
		public:
			DocumentEditor(IDocumentEditorFactory* _editorFactory, IDocumentView* _editingView);
			~DocumentEditor();

			virtual void						NotifyUpdateEditor();
			bool								AttachCallback(ICallback* callback)override;
			bool								DetachCallback(ICallback* callback)override;

			IDocumentEditorFactory*				GetOwnedFactory()override;
			GuiControl*							GetEditorControl()override;
			IDocumentView*						GetEditingView()override;
			bool								FinishEdit()override;
			bool								IsAvailable()override;

			vint								GetSupportedOperationTypeCount()override;
			WString								GetSupportedOperationType(vint index)override;
			bool								IsSupportedOperationTypeId(const WString& operationTypeId)override;
			IDocumentEditorOperation*			GetOperation(const WString& operationTypeId)override;
		};
	}
}

#endif