#include "TextEditorPackage.h"
#include "TextDocument.h"
#include "TextDocumentEditor.h"
#include "XmlDocument.h"
#include "XmlDocumentEditor.h"
#include "JsonDocument.h"
#include "JsonDocumentEditor.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
TextEditorPackage
***********************************************************************/

		WString TextEditorPackage::GetPackageId()
		{
			return PackageId;
		}

		void TextEditorPackage::OnBeforeInit()
		{
			{
				GetDocumentManager()->RegisterFileType(new TextFileType);
				Ptr<TextDocumentEditorFactory> editorFactory=new TextDocumentEditorFactory;
				GetDocumentManager()->RegisterEditorFactory(editorFactory);
				GetDocumentManager()->BindDefaultEditor(PlainTextView::ViewTypeId, editorFactory.Obj());
			}
			{
				GetDocumentManager()->RegisterFileType(new XmlTextFileType);
				Ptr<XmlTextDocumentEditorFactory> editorFactory=new XmlTextDocumentEditorFactory;
				GetDocumentManager()->RegisterEditorFactory(editorFactory);
				GetDocumentManager()->BindDefaultEditor(XmlTextView::ViewTypeId, editorFactory.Obj());
			}
			{
				GetDocumentManager()->RegisterFileType(new JsonTextFileType);
				Ptr<JsonTextDocumentEditorFactory> editorFactory=new JsonTextDocumentEditorFactory;
				GetDocumentManager()->RegisterEditorFactory(editorFactory);
				GetDocumentManager()->BindDefaultEditor(JsonTextView::ViewTypeId, editorFactory.Obj());
			}
		}

		const wchar_t* TextEditorPackage::PackageId = L"TextEditorPackage";
	}
}