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

namespace vl
{
	namespace gactools
	{
		class TextDocumentEditor : public DocumentEditor
		{
		protected:

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