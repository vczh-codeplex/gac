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
	TEST_ASSERT(folderPath[1] == L':');
	TEST_ASSERT(INVLOC.FindLast(folderPath, L"\\FileSystem", Locale::None).key == folderPath.Length() - 11);
#elif define VCZH_GCC
	TEST_ASSERT(folderPath[0] == L'/');
	TEST_ASSERT(INVLOC.FindLast(folderPath, L"/FileSystem", Locale::None).key == folderPath.Length() - 11);
#endif

	if (folder.Exists())
	{
		TEST_ASSERT(folder.Delete(true) == true);
	}
	TEST_ASSERT(folder.Create(false) == true);
	TEST_ASSERT(folder.Exists() == true);
	TEST_ASSERT(folder.Create(false) == false);
}

TEST_CASE(TestFilePath)
{
	ClearTestFolders();
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