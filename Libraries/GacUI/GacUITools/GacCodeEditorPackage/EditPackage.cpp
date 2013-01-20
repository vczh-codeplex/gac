#include "EditPackage.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
EditPackage
***********************************************************************/

		WString EditPackage::GetPackageId()
		{
			return PackageId;
		}

		void EditPackage::BeforeInitialization()
		{
		}

		void EditPackage::AfterInitialization()
		{
		}

		void EditPackage::InstallToolstripCommand(DocumentToolstripCommand* command)
		{
		}

		const wchar_t* EditPackage::PackageId = L"EditPackage";
	}
}