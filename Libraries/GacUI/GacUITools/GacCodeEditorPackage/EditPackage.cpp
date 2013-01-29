#include "EditPackage.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
EditPackage
***********************************************************************/

		const wchar_t* EditPackage::PackageId = L"EditPackage";

		WString EditPackage::GetPackageId()
		{
			return PackageId;
		}
	}
}