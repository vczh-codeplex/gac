#include "TextEditorPackage.h"

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