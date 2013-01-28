/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Xml Document Editor

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_XMLDOCUMENTEDITOR
#define GACUI_TOOLS_XMLDOCUMENTEDITOR

#include "TextDocumentEditor.h"

namespace vl
{
	namespace gactools
	{
		class XmlTextDocumentEditor : public TextDocumentEditor
		{
		protected:
			GuiControl*					CreateEditorControlInternal()override;
		public:
			XmlTextDocumentEditor(IDocumentEditorFactory* _editorFactory, IDocumentView* _editingView);
			~XmlTextDocumentEditor();
		};

		class XmlTextDocumentEditorFactory : public DocumentEditorFactory
		{
		public:
			static const wchar_t*		EditorTypeId;

		protected:

			Ptr<IDocumentEditor>		CreateEditorInternal(IDocumentView* view)override;
		public:
			XmlTextDocumentEditorFactory();
			~XmlTextDocumentEditorFactory();
		};
	}
}

#endif