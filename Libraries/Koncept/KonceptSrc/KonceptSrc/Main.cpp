#include "..\..\Source\KonceptVlppReferences.h"
#include "..\..\..\..\Common\Source\Console.h"

using namespace vl;
using namespace vl::console;

int wmain(int argc, wchar_t* argv[])
{
	Console::SetTitle(L"Koncept UnitTest Project");
	Console::WriteLine(L"Hello, world!");
	Console::Read();
}