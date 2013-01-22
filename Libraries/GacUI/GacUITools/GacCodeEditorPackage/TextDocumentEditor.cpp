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
			PlainTextView* plainTextView=dynamic_cast<PlainTextView*>(GetEditingView());
			if(plainTextView)
			{
				plainTextView->NotifyUpdateView();
			}
		}

		void TextDocumentEditor::BeforeEditView(IDocumentView* view)
		{
			PlainTextView* plainTextView=dynamic_cast<PlainTextView*>(view);
			if(plainTextView)
			{
				plainTextView->ActivateProxy(this);
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

		GuiControl* TextDocumentEditor::CreateEditorControlInternal()
		{
			textBox=g::NewMultilineTextBox();
			textBox->TextChanged.AttachMethod(this, &TextDocumentEditor::textBox_TextChanged);
			return textBox;
		}

		TextDocumentEditor::TextDocumentEditor(IDocumentEditorFactory* _editorFactory, IDocumentView* _editingView)
			:DocumentEditor(_editorFactory, _editingView)
			,textBox(0)
		{
		}

		TextDocumentEditor::~TextDocumentEditor()
		{
		}

/***********************************************************************
TextDocumentEditorFactory
***********************************************************************/

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

		const wchar_t* TextDocumentEditorFactory::EditorTypeId = L"vl::gactools::TextDocumentEditorFactory";
	}
}