#include "TextDocumentEditor.h"
#include "TextDocument.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
TextDocumentEditor
***********************************************************************/

		GuiControl* TextDocumentEditor::CreateEditorControlInternal()
		{
			return g::NewMultilineTextBox();
		}

		TextDocumentEditor::TextDocumentEditor(IDocumentEditorFactory* _editorFactory, IDocumentView* _editingView)
			:DocumentEditor(_editorFactory, _editingView)
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