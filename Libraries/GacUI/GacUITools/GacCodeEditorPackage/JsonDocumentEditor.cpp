#include "JsonDocumentEditor.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
JsonEditorColorizer
***********************************************************************/
		
		class JsonEditorColorizer : public GuiTextBoxRegexColorizer
		{
		public:
			JsonEditorColorizer()
			{
				text::ColorEntry entry=GetCurrentTheme()->GetDefaultTextBoxColorEntry();
				SetDefaultColor(entry);

				entry.normal.text=Color(207, 67, 67);
				AddToken(L"\"[^\"]*\"", entry);

				entry.normal.text=Color(0, 0, 0);
				AddToken(L"[/[/]{},:]", entry);

				entry.normal.text=Color(0, 0, 255);
				AddToken(L"[a-zA-Z0-9_+/-:]+", entry);

				Setup();
			}
		};

/***********************************************************************
JsonTextDocumentEditor
***********************************************************************/

		GuiControl* JsonTextDocumentEditor::CreateEditorControlInternal()
		{
			GuiControl* control=TextDocumentEditor::CreateEditorControlInternal();
			textBox->SetColorizer(new JsonEditorColorizer);
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
			return new JsonTextDocumentEditor(this, view);
		}

		JsonTextDocumentEditorFactory::JsonTextDocumentEditorFactory()
			:DocumentEditorFactory(EditorTypeId, L"Json Text Editor")
		{
			AddSupportedViewTypeId(JsonTextView::ViewTypeId);
		}

		JsonTextDocumentEditorFactory::~JsonTextDocumentEditorFactory()
		{
		}
	}
}