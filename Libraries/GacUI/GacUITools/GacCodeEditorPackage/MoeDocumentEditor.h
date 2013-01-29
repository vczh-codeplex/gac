/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Moe Document Editor

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_MOEDOCUMENTEDITOR
#define GACUI_TOOLS_MOEDOCUMENTEDITOR

#include "TextDocumentEditor.h"
#include "MoeDocument.h"

namespace vl
{
	namespace gactools
	{
		class MoeTextDocumentEditor : public TextDocumentEditor
		{
		protected:
			GuiControl*					CreateEditorControlInternal()override;
		public:
			MoeTextDocumentEditor(IDocumentEditorFactory* _editorFactory, IDocumentView* _editingView);
			~MoeTextDocumentEditor();
		};

		class MoeTextDocumentEditorFactory : public DocumentEditorFactory
		{
		public:
			static const wchar_t*		EditorTypeId;

		protected:

			Ptr<IDocumentEditor>		CreateEditorInternal(IDocumentView* view)override;
		public:
			MoeTextDocumentEditorFactory();
			~MoeTextDocumentEditorFactory();
		};
	}
}

#endif