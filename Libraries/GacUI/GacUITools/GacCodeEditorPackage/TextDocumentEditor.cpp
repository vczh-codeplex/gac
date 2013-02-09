#include "TextDocumentEditor.h"
#include "TextDocument.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
TextDocumentEditor::EditorSelectionOperation
***********************************************************************/

		TextDocumentEditor::EditorSelectionOperation::EditorSelectionOperation(TextDocumentEditor* _editor)
			:editor(_editor)
			,operationCallback(0)
		{
		}

		IDocumentEditor* TextDocumentEditor::EditorSelectionOperation::GetOwnedEditor()
		{
			return editor;
		}

		bool TextDocumentEditor::EditorSelectionOperation::Initialize(IEditorSelectionCallback* callback)
		{
			if(!operationCallback)
			{
				operationCallback=callback;
				return true;
			}
			else
			{
				return false;
			}
		}

		bool TextDocumentEditor::EditorSelectionOperation::IsInitialized()
		{
			return operationCallback!=0;
		}

		bool TextDocumentEditor::EditorSelectionOperation::CanUndo()
		{
			return !editor->textBox->GetReadonly() && editor->textBox->CanUndo();
		}

		bool TextDocumentEditor::EditorSelectionOperation::CanRedo()
		{
			return !editor->textBox->GetReadonly() && editor->textBox->CanRedo();
		}

		bool TextDocumentEditor::EditorSelectionOperation::CanCut()
		{
			return !editor->textBox->GetReadonly() && editor->textBox->CanCut();
		}

		bool TextDocumentEditor::EditorSelectionOperation::CanCopy()
		{
			return editor->textBox->CanCopy();
		}

		bool TextDocumentEditor::EditorSelectionOperation::CanPaste()
		{
			return !editor->textBox->GetReadonly() && editor->textBox->CanPaste();
		}

		bool TextDocumentEditor::EditorSelectionOperation::CanDelete()
		{
			return !editor->textBox->GetReadonly() && editor->textBox->GetCaretBegin()!=editor->textBox->GetCaretEnd();
		}

		bool TextDocumentEditor::EditorSelectionOperation::CanSelectAll()
		{
			return true;
		}

		bool TextDocumentEditor::EditorSelectionOperation::PerformUndo()
		{
			if(!CanUndo()) return false;
			return editor->textBox->Undo();
		}

		bool TextDocumentEditor::EditorSelectionOperation::PerformRedo()
		{
			if(!CanRedo()) return false;
			return editor->textBox->Redo();
		}

		bool TextDocumentEditor::EditorSelectionOperation::PerformCut()
		{
			if(!CanCut()) return false;
			return editor->textBox->Cut();
		}

		bool TextDocumentEditor::EditorSelectionOperation::PerformCopy()
		{
			if(!CanCopy()) return false;
			return editor->textBox->Copy();
		}

		bool TextDocumentEditor::EditorSelectionOperation::PerformPaste()
		{
			if(!CanPaste()) return false;
			return editor->textBox->Paste();
		}

		bool TextDocumentEditor::EditorSelectionOperation::PerformDelete()
		{
			if(!CanDelete()) return false;
			editor->textBox->SetSelectionText(L"");
			return true;
		}

		bool TextDocumentEditor::EditorSelectionOperation::PerformSelectAll()
		{
			if(!CanSelectAll()) return false;
			editor->textBox->SelectAll();
			return true;
		}

/***********************************************************************
TextDocumentEditor
***********************************************************************/

		WString TextDocumentEditor::GetContent()
		{
			return textBox->GetText();
		}

		void TextDocumentEditor::SetContent(const WString& value)
		{
			textBox->SetText(value);
		}

		void TextDocumentEditor::textBox_TextChanged(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			if(!loading)
			{
				PlainTextView* plainTextView=dynamic_cast<PlainTextView*>(GetEditingView());
				if(plainTextView)
				{
					plainTextView->NotifyUpdateView(true);
					plainTextView->GetOwnedFragment()->SetModified(textBox->GetModified());
				}
			}
		}

		void TextDocumentEditor::BeforeEditView(IDocumentView* view)
		{
			PlainTextView* plainTextView=dynamic_cast<PlainTextView*>(view);
			if(plainTextView)
			{
				loading=true;
				plainTextView->ActivateProxy(this);
				textBox->ClearUndoRedo();
				textBox->NotifyModificationSaved();
				loading=false;
			}
		}

		void TextDocumentEditor::AfterEditView(IDocumentView* view)
		{
			PlainTextView* plainTextView=dynamic_cast<PlainTextView*>(view);
			if(plainTextView)
			{
				plainTextView->DeactivateProxy(this);
			}
		}

		void TextDocumentEditor::OnViewSaved(IDocumentView* view)
		{
			textBox->NotifyModificationSaved();
		}

		GuiControl* TextDocumentEditor::CreateEditorControlInternal()
		{
			textBox=g::NewMultilineTextBox();
			textBox->TextChanged.AttachMethod(this, &TextDocumentEditor::textBox_TextChanged);
			return textBox;
		}

		TextDocumentEditor::TextDocumentEditor(IDocumentEditorFactory* _editorFactory, IDocumentView* _editingView)
			:DocumentEditor(_editorFactory, _editingView)
			,loading(false)
			,textBox(0)
		{
			AddSupportedOperation(new EditorSelectionOperation(this));
		}

		TextDocumentEditor::~TextDocumentEditor()
		{
		}

/***********************************************************************
TextDocumentEditorFactory
***********************************************************************/

		const wchar_t* TextDocumentEditorFactory::EditorTypeId = L"vl::gactools::TextDocumentEditorFactory";

		Ptr<IDocumentEditor> TextDocumentEditorFactory::CreateEditorInternal(IDocumentView* view)
		{
			return new TextDocumentEditor(this, view);
		}

		TextDocumentEditorFactory::TextDocumentEditorFactory()
			:DocumentEditorFactory(EditorTypeId, L"Plain Text Editor")
		{
			AddSupportedViewTypeId(PlainTextView::ViewTypeId);
		}

		TextDocumentEditorFactory::~TextDocumentEditorFactory()
		{
		}
	}
}