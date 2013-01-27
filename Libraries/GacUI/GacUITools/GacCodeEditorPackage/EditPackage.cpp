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

		const wchar_t* EditPackage::PackageId = L"EditPackage";
	}
}