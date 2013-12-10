#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <windows.h>

#include "..\..\..\..\Common\Source\Console.h"
#include "..\..\Source\Expression\WfExpression.h"

using namespace vl;
using namespace vl::console;
using namespace vl::parsing::tabling;
using namespace vl::workflow;

Ptr<ParsingTable> workflowTable;

Ptr<ParsingTable> GetWorkflowTable()
{
	if (!workflowTable)
	{
		workflowTable = WfLoadTable();
	}
	return workflowTable;
}

WString GetPath()
{
	wchar_t buffer[65536];
	GetModuleFileName(NULL, buffer, sizeof(buffer)/sizeof(*buffer));
	vint pos=-1;
	vint index=0;
	while(buffer[index])
	{
		if(buffer[index]==L'\\')
		{
			pos=index;
		}
		index++;
	}
#ifdef _WIN64
	return WString(buffer, pos+1)+L"..\\..\\TestFiles\\";
#else
	return WString(buffer, pos+1)+L"..\\TestFiles\\";
#endif
}

int wmain(int argc, wchar_t* argv[])
{
	workflowTable = 0;
	_CrtDumpMemoryLeaks();
	return 0;
}