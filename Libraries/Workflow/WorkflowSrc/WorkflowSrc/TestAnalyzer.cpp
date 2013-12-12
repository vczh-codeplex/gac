#include "TestFunctions.h"

TEST_CASE(TestBuildGlobalName)
{
	LoadTypes();
	WfLexicalScopeManager manager;
	manager.BuildGlobalName();
	UnloadTypes();
}