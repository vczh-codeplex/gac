#include "../../Source/UnitTest/UnitTest.h"
#include "../../Source/FileSystem.h"
#include "../../Source/Locale.h"

using namespace vl;
using namespace vl::filesystem;

extern WString GetPath();

void ClearTestFolders()
{
	FilePath path = GetPath();
	TEST_ASSERT(path.IsFolder());

	Folder folder(GetPath() + L"FileSystem");
	auto folderPath = folder.GetFilePath().GetFullPath();
#if defined VCZH_MSVC
	TEST_ASSERT(INVLOC.FindLast(folderPath, L"\\FileSystem\\", Locale::None).key == folderPath.Length() - 12);
#elif define VCZH_GCC
	TEST_ASSERT(INVLOC.FindLast(folderPath, L"/FileSystem/", Locale::None).key == folderPath.Length() - 12);
#endif

	if (!folder.Exists())
	{
		TEST_ASSERT(folder.Create(false) == true);
	}
	else
	{
		TEST_ASSERT(folder.Create(false) == false);
	}
}

TEST_CASE(CreateDeleteFolders)
{
	ClearTestFolders();
}

TEST_CASE(CreateDeleteFiles)
{
	ClearTestFolders();
}

TEST_CASE(EnumerateFoldersAndFiles)
{
	ClearTestFolders();
}

TEST_CASE(FastAccessFiles)
{
	ClearTestFolders();
}