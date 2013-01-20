#include "..\Source\DocumentFramework.h"

namespace vl
{
	namespace gactools
	{
		class EditPackage : public Object, public IDocumentPackage
		{
		public:
			static const wchar_t*			PackageId;

			WString GetPackageId()
			{
				return PackageId;
			}

			void BeforeInitialization()
			{
			}

			void AfterInitialization()
			{
			}

			void InstallToolstripCommand(DocumentToolstripCommand* command)
			{
			}
		};
		const wchar_t* EditPackage::PackageId = L"EditPackage";
		INSTALL_PACKAGE(EditPackage);
	}
}