#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <windows.h>
#include "../../Source/String.h"
#include "../../Source/GlobalStorage.h"

using namespace vl;

WString GetPath()
{
	wchar_t buffer[65536];
	GetModuleFileName(NULL, buffer, sizeof(buffer)/sizeof(*buffer));
	vint pos=-1;
	vint index=0;
	while(buffer[index])
	{
		if(buffer[index]==L'\\' || buffer[index]==L'/')
		{
			pos=index;
		}
		index++;
	}
#ifdef _WIN64
	return WString(buffer, pos+1)+L"../../TestFiles/";
#else
	return WString(buffer, pos+1)+L"../TestFiles/";
#endif
}

int wmain(vint argc , wchar_t* args[])
{
	FinalizeGlobalStorage();
	_CrtDumpMemoryLeaks();
	return 0;
}