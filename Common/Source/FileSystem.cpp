#define _CRT_SECURE_NO_WARNINGS
#include "FileSystem.h"
#include "Locale.h"
#if defined VCZH_MSVC
#include <Windows.h>
#elif defined VCZH_GCC
#endif

namespace vl
{
	namespace filesystem
	{
		using namespace collections;

		// ReadDirectoryChangesW

/***********************************************************************
FilePath
***********************************************************************/

		void FilePath::Initialize()
		{
			Array<wchar_t> buffer(fullPath.Length() + 1);
			wcscpy(&buffer[0], fullPath.Buffer());
			for (vint i = 0; i < buffer.Count(); i++)
			{
				if (buffer[i] == L'\\' || buffer[i] == L'/')
				{
					buffer[i] = Delimiter;
				}
			}
			fullPath = &buffer[0];

#if defined VCZH_MSVC
			if (fullPath.Length() < 2 || fullPath[1] != L':')
			{
				wchar_t buffer[MAX_PATH + 1];
				GetCurrentDirectory(sizeof(buffer) / sizeof(*buffer), buffer);
				fullPath = WString(buffer) + L"\\" + fullPath;
			}
			{
				wchar_t buffer[MAX_PATH + 1];
				GetFullPathName(fullPath.Buffer(), sizeof(buffer) / sizeof(*buffer), buffer, NULL);
				fullPath = buffer;
			}

			if (fullPath[fullPath.Length() - 1] == Delimiter)
			{
				fullPath = fullPath.Left(fullPath.Length() - 1);
			}
#elif defined VCZH_GCC
			throw 0;
#endif
		}

		FilePath::FilePath()
		{
			Initialize();
		}

		FilePath::FilePath(const WString& _filePath)
			:fullPath(_filePath)
		{
			Initialize();
		}

		FilePath::FilePath(const wchar_t* _filePath)
			:fullPath(_filePath)
		{
			Initialize();
		}

		FilePath::FilePath(const FilePath& _filePath)
			:fullPath(_filePath.fullPath)
		{
			Initialize();
		}

		FilePath::~FilePath()
		{
		}

		vint FilePath::Compare(const FilePath& a, const FilePath& b)
		{
			throw 0;
		}

		FilePath FilePath::operator/(const WString& relativePath)const
		{
			throw 0;
		}

		bool FilePath::IsFile()const
		{
#if defined VCZH_MSVC
			WIN32_FILE_ATTRIBUTE_DATA info;
			BOOL result = GetFileAttributesEx(fullPath.Buffer(), GetFileExInfoStandard, &info);
			if (!result) return false;
			return (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
#elif defined VCZH_GCC
			throw 0;
#endif
		}

		bool FilePath::IsFolder()const
		{
#if defined VCZH_MSVC
			WIN32_FILE_ATTRIBUTE_DATA info;
			BOOL result = GetFileAttributesEx(fullPath.Buffer(), GetFileExInfoStandard, &info);
			if (!result) return false;
			return (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
#elif defined VCZH_GCC
			throw 0;
#endif
		}

		bool FilePath::IsRoot()const
		{
#if defined VCZH_MSVC
			return fullPath == L"";
#elif defined VCZH_GCC
			throw 0;
#endif
		}

		WString FilePath::GetName()const
		{
			WString delimiter = Delimiter;
			auto index = INVLOC.FindLast(fullPath, delimiter, Locale::None);
			if (index.key == -1) return L"";
			return fullPath.Right(fullPath.Length() - index.key - 1);
		}

		FilePath FilePath::GetFolder()const
		{
			WString delimiter = Delimiter;
			auto index = INVLOC.FindLast(fullPath, delimiter, Locale::None);
			if (index.key == -1) return L"";
			return fullPath.Left(index.key);
		}

		WString FilePath::GetFullPath()const
		{
			return fullPath;
		}

		WString FilePath::GetRelativePathFor(const FilePath& _filePath)
		{
#if defined VCZH_MSVC
			throw 0;
#elif defined VCZH_GCC
			throw 0;
#endif
		}

/***********************************************************************
File
***********************************************************************/
		
		File::File(const FilePath& _filePath)
			:filePath(_filePath)
		{
		}

		File::~File()
		{
		}

		const FilePath& File::GetFilePath()const
		{
			return filePath;
		}

		WString File::ReadAllText()const
		{
			throw 0;
		}

		bool File::ReadAllText(WString& text)const
		{
			throw 0;
		}

		bool File::ReadAllLines(collections::List<WString>& lines)const
		{
			throw 0;
		}

		bool File::WriteAllText(const WString& text, bool bom, stream::BomEncoder::Encoding encoding)
		{
			throw 0;
		}

		bool File::WriteAllLines(collections::List<WString>& lines, bool bom, stream::BomEncoder::Encoding encoding)
		{
			throw 0;
		}

		bool File::Exists()const
		{
			return filePath.IsFile();
		}

		bool File::Create()const
		{
#if defined VCZH_MSVC
			throw 0;
#elif defined VCZH_GCC
			throw 0;
#endif
		}

		bool File::Delete()const
		{
#if defined VCZH_MSVC
			throw 0;
#elif defined VCZH_GCC
			throw 0;
#endif
		}

/***********************************************************************
Folder
***********************************************************************/
		
		Folder::Folder(const FilePath& _filePath)
			:filePath(_filePath)
		{
		}

		Folder::~Folder()
		{
		}

		const FilePath& Folder::GetFilePath()const
		{
			return filePath;
		}

		bool Folder::GetFolders(collections::List<Folder>& folders)const
		{
#if defined VCZH_MSVC
			throw 0;
#elif defined VCZH_GCC
			throw 0;
#endif
		}

		bool Folder::GetFiles(collections::List<File>& files)const
		{
#if defined VCZH_MSVC
			throw 0;
#elif defined VCZH_GCC
			throw 0;
#endif
		}

		bool Folder::Exists()const
		{
			return filePath.IsFolder();
		}

		bool Folder::Create(bool recursively)const
		{
#if defined VCZH_MSVC
			if (recursively)
			{
				throw 0;
			}
			else
			{
				return CreateDirectory(filePath.GetFullPath().Buffer(), NULL) != 0;
			}
#elif defined VCZH_GCC
			throw 0;
#endif
		}

		bool Folder::Delete(bool recursively)const
		{
#if defined VCZH_MSVC
			if (recursively)
			{
				throw 0;
			}
			else
			{
				return RemoveDirectory(filePath.GetFullPath().Buffer()) != 0;
			}
#elif defined VCZH_GCC
			throw 0;
#endif
		}
	}
}