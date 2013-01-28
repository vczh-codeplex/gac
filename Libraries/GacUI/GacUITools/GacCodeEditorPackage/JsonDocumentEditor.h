/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Json Document Editor

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_JSONDOCUMENTEDITOR
#define GACUI_TOOLS_JSONDOCUMENTEDITOR

#include "TextDocumentEditor.h"
#include "JsonDocument.h"

namespace vl
{
	namespace gactools
	{
		class JsonTextDocumentEditor : public TextDocumentEditor
		{
		protected:
			GuiControl*					CreateEditorControlInternal()override;
		public:
			JsonTextDocumentEditor(IDocumentEditorFactory* _editorFactory, IDocumentView* _editingView);
			~JsonTextDocumentEditor();
		};

		class JsonTextDocumentEditorFactory : public DocumentEditorFactory
		{
		public:
			static const wchar_t*		EditorTypeId;

		protected:

			Ptr<IDocumentEditor>		CreateEditorInternal(IDocumentView* view)override;
		public:
			JsonTextDocumentEditorFactory();
			~JsonTextDocumentEditorFactory();
		};
	}
}

#endif