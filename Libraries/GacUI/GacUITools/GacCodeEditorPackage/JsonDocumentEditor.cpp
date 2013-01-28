#include "JsonDocumentEditor.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
JsonTextDocumentEditor
***********************************************************************/

		GuiControl* JsonTextDocumentEditor::CreateEditorControlInternal()
		{
			GuiControl* control=TextDocumentEditor::CreateEditorControlInternal();
			return control;
		}

		JsonTextDocumentEditor::JsonTextDocumentEditor(IDocumentEditorFactory* _editorFactory, IDocumentView* _editingView)
			:TextDocumentEditor(_editorFactory, _editingView)
		{
		}

		JsonTextDocumentEditor::~JsonTextDocumentEditor()
		{
		}

/***********************************************************************
JsonTextDocumentEditorFactory
***********************************************************************/

		const wchar_t* JsonTextDocumentEditorFactory::EditorTypeId = L"vl::gactools::JsonTextDocumentEditorFactory";

		Ptr<IDocumentEditor> JsonTextDocumentEditorFactory::CreateEditorInternal(IDocumentView* view)
		{
			return new TextDocumentEditor(this, view);
		}

		JsonTextDocumentEditorFactory::JsonTextDocumentEditorFactory()
			:DocumentEditorFactory(EditorTypeId, L"Json Text Editor")
		{
			AddSupportedViewTypeId(PlainTextView::ViewTypeId);
		}

		JsonTextDocumentEditorFactory::~JsonTextDocumentEditorFactory()
		{
		}
	}
}