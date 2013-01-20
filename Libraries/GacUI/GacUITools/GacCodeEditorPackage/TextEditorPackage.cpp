#include "..\Source\DocumentFramework.h"

namespace vl
{
	namespace gactools
	{
		class TextEditorPackage : public Object, public IDocumentPackage
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
		const wchar_t* TextEditorPackage::PackageId = L"TextEditorPackage";
		INSTALL_PACKAGE(TextEditorPackage);
	}
}