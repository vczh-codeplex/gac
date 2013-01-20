/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Package Manager

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_EDITPACKAGE
#define GACUI_TOOLS_EDITPACKAGE

#include "..\Source\DocumentFramework.h"

namespace vl
{
	namespace gactools
	{
		class EditPackage : public Object, public IDocumentPackage
		{
		public:
			static const wchar_t*			PackageId;

			WString							GetPackageId()override;
			void							BeforeInitialization()override;
			void							AfterInitialization()override;
			void							InstallToolstripCommand(DocumentToolstripCommand* command)override;
		};
	}
}

#endif