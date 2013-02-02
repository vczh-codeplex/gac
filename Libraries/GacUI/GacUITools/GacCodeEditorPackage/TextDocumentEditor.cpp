#include "TextDocumentEditor.h"
#include "TextDocument.h"

namespace vl
{
	namespace gactools
	{

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