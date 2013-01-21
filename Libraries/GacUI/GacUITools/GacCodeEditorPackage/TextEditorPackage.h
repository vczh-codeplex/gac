/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI Tools::Text Editor Package

Interfaces:
***********************************************************************/
#ifndef GACUI_TOOLS_TEXTEDITORPACKAGE
#define GACUI_TOOLS_TEXTEDITORPACKAGE

#include "..\Source\DocumentFramework.h"

namespace vl
{
	namespace gactools
	{
		class TextEditorPackage : public Object, public IDocumentPackage
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