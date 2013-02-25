#include "..\Source\SDI.h"
#include "..\GacCodeEditorPackage\EditPackage.h"
#include "..\GacCodeEditorPackage\TextEditorPackage.h"
#include <Windows.h>

using namespace vl::gactools;

namespace vl
{
	namespace gactools
	{
		INSTALL_PACKAGE(EditPackage);
		INSTALL_PACKAGE(TextEditorPackage);
	}
}

/***********************************************************************
WinMain
***********************************************************************/

class SDIApplication : public ISDIApplication
{
protected:
	Ptr<GuiResource>			resource;

public:
	SDIApplication()
	{
	}
	
	~SDIApplication()
	{
	}

	Ptr<GuiResource> GetApplicationResource()
	{
		if(!resource)
		{
			resource=GuiResource::LoadFromXml(GetApplication()->GetExecutableFolder()+L"..\\Source\\PackageResources\\Applications\\GacNotepad.xml");
		}
		return resource;
	}
};

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	SDIApplication application;
	return StartSDI(&application);
}