#include "FileSystem.h"

#if defined VCZH_MSVC

namespace vl
{
	namespace filesystem
	{

/***********************************************************************
FilePath
***********************************************************************/

		FilePath::FilePath()
		{
			throw 0;
		}

		FilePath::FilePath(const WString& _filePath)
		{
			throw 0;
		}

		FilePath::FilePath(const wchar_t* _filePath)
		{
			throw 0;
		}

		FilePath::FilePath(const FilePath& _filePath)
		{
			throw 0;
		}

		FilePath::~FilePath()
		{
			throw 0;
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
			throw 0;
		}

		bool FilePath::IsFolder()const
		{
			throw 0;
		}

		bool FilePath::IsRoot()const
		{
			throw 0;
		}

		WString FilePath::GetName()const
		{
			throw 0;
		}

		FilePath FilePath::GetFolder()const
		{
			throw 0;
		}

		WString FilePath::GetFullPath()const
		{
			throw 0;
		}

		WString FilePath::GetRelativePathFor(const FilePath& _filePath)
		{
			throw 0;
		}

/***********************************************************************
File
***********************************************************************/
		
		File::File(const FilePath& _filePath)
		{
			throw 0;
		}

		File::~File()
		{
			throw 0;
		}

		const FilePath& File::GetFilePath()const
		{
			throw 0;
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
			throw 0;
		}

		bool File::Create()const
		{
			throw 0;
		}

		bool File::Delete()const
		{
			throw 0;
		}

/***********************************************************************
Folder
***********************************************************************/
		
		Folder::Folder(const FilePath& _filePath)
		{
			throw 0;
		}

		Folder::~Folder()
		{
			throw 0;
		}

		const FilePath& Folder::GetFilePath()const
		{
			throw 0;
		}

		bool Folder::GetFolders(collections::List<Folder>& folders)const
		{
			throw 0;
		}

		bool Folder::GetFiles(collections::List<File>& files)const
		{
			throw 0;
		}

		bool Folder::Exists()const
		{
			throw 0;
		}

		bool Folder::Create(bool recursively)const
		{
			throw 0;
		}

		bool Folder::Delete(bool recursively)const
		{
			throw 0;
		}
	}
}

#endif