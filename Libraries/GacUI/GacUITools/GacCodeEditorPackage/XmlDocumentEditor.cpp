#include "XmlDocumentEditor.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
XmlTextDocumentEditor
***********************************************************************/

		GuiControl* XmlTextDocumentEditor::CreateEditorControlInternal()
		{
			GuiControl* control=TextDocumentEditor::CreateEditorControlInternal();
			return control;
		}

		XmlTextDocumentEditor::XmlTextDocumentEditor(IDocumentEditorFactory* _editorFactory, IDocumentView* _editingView)
			:TextDocumentEditor(_editorFactory, _editingView)
		{
		}

		XmlTextDocumentEditor::~XmlTextDocumentEditor()
		{
		}

/***********************************************************************
XmlTextDocumentEditorFactory
***********************************************************************/

		const wchar_t* XmlTextDocumentEditorFactory::EditorTypeId = L"vl::gactools::XmlTextDocumentEditorFactory";

		Ptr<IDocumentEditor> XmlTextDocumentEditorFactory::CreateEditorInternal(IDocumentView* view)
		{
			return new TextDocumentEditor(this, view);
		}

		XmlTextDocumentEditorFactory::XmlTextDocumentEditorFactory()
			:DocumentEditorFactory(EditorTypeId, L"Xml Text Editor")
		{
			AddSupportedViewTypeId(PlainTextView::ViewTypeId);
		}

		XmlTextDocumentEditorFactory::~XmlTextDocumentEditorFactory()
		{
		}
	}
}