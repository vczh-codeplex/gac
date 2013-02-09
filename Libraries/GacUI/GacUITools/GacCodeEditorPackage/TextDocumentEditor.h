/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Text Document Editor

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_TEXTDOCUMENTEDITOR
#define GACUI_TOOLS_TEXTDOCUMENTEDITOR

#include "..\Source\Implementation\DocumentEditor.h"
#include "..\Source\Implementation\DocumentEditorFactory.h"
#include "TextDocument.h"
#include "EditPackage.h"

namespace vl
{
	namespace gactools
	{
		class TextDocumentEditor : public DocumentEditor, private IPlainTextViewContentProxy
		{
		private:
			class EditorSelectionOperation : public Object, public IEditorSelectionOperation
			{
			protected:
				TextDocumentEditor*				editor;
				IEditorSelectionCallback*		operationCallback;

			public:
				EditorSelectionOperation(TextDocumentEditor* _editor);

				IDocumentEditor*		GetOwnedEditor()override;
				bool					Initialize(IEditorSelectionCallback* callback)override;
				bool					IsInitialized()override;

				bool					CanUndo()override;
				bool					CanRedo()override;
				bool					CanCut()override;
				bool					CanCopy()override;
				bool					CanPaste()override;
				bool					CanDelete()override;
				bool					CanSelectAll()override;

				bool					PerformUndo()override;
				bool					PerformRedo()override;
				bool					PerformCut()override;
				bool					PerformCopy()override;
				bool					PerformPaste()override;
				bool					PerformDelete()override;
				bool					PerformSelectAll()override;
			};
			
			WString						GetContent()override;
			void						SetContent(const WString& value)override;
		protected:
			bool						loading;
			GuiMultilineTextBox*		textBox;

			void						textBox_TextChanged(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void						BeforeEditView(IDocumentView* view)override;
			void						AfterEditView(IDocumentView* view)override;
			void						OnViewSaved(IDocumentView* view)override;
			GuiControl*					CreateEditorControlInternal()override;
		public:
			TextDocumentEditor(IDocumentEditorFactory* _editorFactory, IDocumentView* _editingView);
			~TextDocumentEditor();
		};

		class TextDocumentEditorFactory : public DocumentEditorFactory
		{
		public:
			static const wchar_t*		EditorTypeId;

		protected:

			Ptr<IDocumentEditor>		CreateEditorInternal(IDocumentView* view)override;
		public:
			TextDocumentEditorFactory();
			~TextDocumentEditorFactory();
		};
	}
}

#endif