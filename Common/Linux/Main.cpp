#include "../Source/String.h"
#include "../Source/UnitTest/UnitTest.h"

using namespace vl;

WString GetPath()
{
	return L"../UnitTest/TestFiles/";
}

int main()
{
	unittest::UnitTest::RunAndDisposeTests();
	return 0;
}
