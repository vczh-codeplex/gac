#include "..\Source\SDI.h"
#include <Windows.h>

using namespace vl::gactools;

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
			resource=new GuiResource;
			resource->LoadResourceXml(GetApplication()->GetExecutableFolder()+L"..\\Source\\PackageResources\\Applications\\GacNotepad.xml");
		}
		return resource;
	}
};

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	SDIApplication application;
	return StartSDI(&application);
}