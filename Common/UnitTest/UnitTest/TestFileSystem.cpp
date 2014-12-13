#include "../../Source/UnitTest/UnitTest.h"
#include "../../Source/FileSystem.h"
#include "../../Source/Locale.h"

using namespace vl;
using namespace vl::filesystem;
using namespace vl::collections;
using namespace vl::stream;

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
	{
		FilePath p;
		TEST_ASSERT(p.IsFile() == false);
		TEST_ASSERT(p.IsFolder() == false);
		TEST_ASSERT(p.IsRoot() == true);
		TEST_ASSERT(p.GetFullPath() == L"");
	}
	{
		FilePath p = L"C:\\";
		TEST_ASSERT(p.IsFile() == false);
		TEST_ASSERT(p.IsFolder() == true);
		TEST_ASSERT(p.IsRoot() == false);
		TEST_ASSERT(p.GetFullPath() == L"C:");
	}
	{
		FilePath p = L"C:\\Windows\\";
		TEST_ASSERT(p.IsFile() == false);
		TEST_ASSERT(p.IsFolder() == true);
		TEST_ASSERT(p.IsRoot() == false);
		TEST_ASSERT(p.GetFullPath() == L"C:\\Windows");
	}
	{
		FilePath p = L"C:\\Windows\\Explorer.exe";
		TEST_ASSERT(p.IsFile() == true);
		TEST_ASSERT(p.IsFolder() == false);
		TEST_ASSERT(p.IsRoot() == false);
		TEST_ASSERT(p.GetFullPath() == L"C:\\Windows\\Explorer.exe");
	}
	{
		FilePath p = L"C:\\Windows\\vczh.txt";
		TEST_ASSERT(p.IsFile() == false);
		TEST_ASSERT(p.IsFolder() == false);
		TEST_ASSERT(p.IsRoot() == false);
		TEST_ASSERT(p.GetFullPath() == L"C:\\Windows\\vczh.txt");
	}
	{
		FilePath p = L"C:\\Windows";
		auto q = p / L"Explorer.exe";
		TEST_ASSERT(q.GetFullPath() == L"C:\\Windows\\Explorer.exe");
	}
	{
		FilePath p = L"C:\\Program Files";
		auto q = p / L"..\\Windows\\.\\Explorer.exe";
		TEST_ASSERT(q.GetFullPath() == L"C:\\Windows\\Explorer.exe");
	}
	{
		FilePath p = L"C:\\Program Files";
		FilePath q = L"C:\\Windows\\Explorer.exe";
		auto r = p.GetRelativePathFor(q);
		TEST_ASSERT(r == L"..\\Windows\\Explorer.exe");
	}
	{
		FilePath p = L"C:\\Program Files\\vczh.txt";
		FilePath q = L"C:\\Windows\\Explorer.exe";
		auto r = p.GetRelativePathFor(q);
		TEST_ASSERT(r == L"..\\Windows\\Explorer.exe");
	}
	{
		FilePath p = L"C:\\Program Files\\vczh.txt";
		FilePath q = L"D:\\Windows\\Explorer.exe";
		auto r = p.GetRelativePathFor(q);
		TEST_ASSERT(r == L"D:\\Windows\\Explorer.exe");
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
	FilePath folder = GetPath() + L"FileSystem";
	File file = folder / L"vczh.txt";

	WString text;
	List<WString> lines;
	TEST_ASSERT(file.Exists() == false);
	TEST_ASSERT(file.ReadAllText() == L"");
	TEST_ASSERT(file.ReadAllText(text) == false && text == L"");
	TEST_ASSERT(file.ReadAllLines(lines) == false && lines.Count() == 0);
	
	text = L"";
	lines.Clear();
	file.WriteAllText(L"Vczh is a genius!");
	TEST_ASSERT(file.Exists() == true);
	TEST_ASSERT(file.ReadAllText() == L"Vczh is a genius!");
	TEST_ASSERT(file.ReadAllText(text) == true && text == L"Vczh is a genius!");
	TEST_ASSERT(file.ReadAllLines(lines) == true && lines.Count() == 1 && lines[0] == L"Vczh is a genius!");
	
	text = L"";
	lines.Clear();
	file.WriteAllText(L"Vczh is a genius!", true, BomEncoder::Mbcs);
	TEST_ASSERT(file.Exists() == true);
	TEST_ASSERT(file.ReadAllText() == L"Vczh is a genius!");
	TEST_ASSERT(file.ReadAllText(text) == true && text == L"Vczh is a genius!");
	TEST_ASSERT(file.ReadAllLines(lines) == true && lines.Count() == 1 && lines[0] == L"Vczh is a genius!");
	
	text = L"";
	lines.Clear();
	file.WriteAllText(L"Vczh is a genius!", true, BomEncoder::Utf8);
	TEST_ASSERT(file.Exists() == true);
	TEST_ASSERT(file.ReadAllText() == L"Vczh is a genius!");
	TEST_ASSERT(file.ReadAllText(text) == true && text == L"Vczh is a genius!");
	TEST_ASSERT(file.ReadAllLines(lines) == true && lines.Count() == 1 && lines[0] == L"Vczh is a genius!");
	
	text = L"";
	lines.Clear();
	file.WriteAllText(L"Vczh is a genius!", true, BomEncoder::Utf16);
	TEST_ASSERT(file.Exists() == true);
	TEST_ASSERT(file.ReadAllText() == L"Vczh is a genius!");
	TEST_ASSERT(file.ReadAllText(text) == true && text == L"Vczh is a genius!");
	TEST_ASSERT(file.ReadAllLines(lines) == true && lines.Count() == 1 && lines[0] == L"Vczh is a genius!");
	
	text = L"";
	lines.Clear();
	file.WriteAllText(L"Vczh is a genius!", true, BomEncoder::Utf16BE);
	TEST_ASSERT(file.Exists() == true);
	TEST_ASSERT(file.ReadAllText() == L"Vczh is a genius!");
	TEST_ASSERT(file.ReadAllText(text) == true && text == L"Vczh is a genius!");
	TEST_ASSERT(file.ReadAllLines(lines) == true && lines.Count() == 1 && lines[0] == L"Vczh is a genius!");
	
	text = L"";
	lines.Clear();
	TEST_ASSERT(file.Delete());
	TEST_ASSERT(file.Exists() == false);
	TEST_ASSERT(file.ReadAllText() == L"");
	TEST_ASSERT(file.ReadAllText(text) == false && text == L"");
	TEST_ASSERT(file.ReadAllLines(lines) == false && lines.Count() == 0);
}