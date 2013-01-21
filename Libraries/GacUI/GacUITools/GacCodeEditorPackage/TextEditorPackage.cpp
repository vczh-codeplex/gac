#include "TextEditorPackage.h"
#include "TextDocument.h"

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
			GetDocumentManager()->RegisterFileType(new TextFileType);
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