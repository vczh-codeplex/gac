#include "..\Source\SDI.h"

using namespace vl::sdi;

/***********************************************************************
WinMain
***********************************************************************/

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	SDIConfiguration config;
	config.applicationName=L"Notepad";
	return StartSDI(config);
}