#include "TextEditorPackage.h"
#include "TextDocument.h"
#include "TextDocumentEditor.h"

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

		void TextEditorPackage::BeforeInitialization()
		{
			{
				Ptr<TextDocumentEditorFactory> textDocumentEditorFactory=new TextDocumentEditorFactory;
				GetDocumentManager()->RegisterFileType(new TextFileType);
				GetDocumentManager()->RegisterEditorFactory(textDocumentEditorFactory);
				GetDocumentManager()->BindDefaultEditor(PlainTextView::ViewTypeId, textDocumentEditorFactory.Obj());
			}
		}

		void TextEditorPackage::AfterInitialization()
		{
		}

		void TextEditorPackage::InstallToolstripCommand(DocumentToolstripCommand* command)
		{
		}

		const wchar_t* TextEditorPackage::PackageId = L"TextEditorPackage";
	}
}