#include "FileSystemInformation.h"

/***********************************************************************
File System Operations
***********************************************************************/

WString GetWindowsDirectory()
{
	wchar_t folderPath[MAX_PATH]={0};
	HRESULT hr=SHGetFolderPath(NULL, CSIDL_WINDOWS, NULL, 0, folderPath);
	if(FAILED(hr)) return L"";
	return folderPath;
}

void SearchDirectoriesAndFiles(const WString& path, List<WString>& directories, List<WString>& files)
{
	// Use FindFirstFile, FindNextFile and FindClose to enumerate all directories and files
	WIN32_FIND_DATA findData;
	HANDLE findHandle=INVALID_HANDLE_VALUE;

	while(true)
	{
		if(findHandle==INVALID_HANDLE_VALUE)
		{
			WString searchPath=path+L"\\*";
			findHandle=FindFirstFile(searchPath.Buffer(), &findData);
			if(findHandle==INVALID_HANDLE_VALUE)
			{
				break;
			}
		}
		else
		{
			BOOL result=FindNextFile(findHandle, &findData);
			if(result==0)
			{
				FindClose(findHandle);
				break;
			}
		}

		if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(wcscmp(findData.cFileName, L".")!=0 && wcscmp(findData.cFileName, L"..")!=0)
			{
				directories.Add(findData.cFileName);
			}
		}
		else
		{
			files.Add(findData.cFileName);
		}
	}

	Func<vint(WString a, WString b)> comparer=[](WString a, WString b){return _wcsicmp(a.Buffer(), b.Buffer());};
	CopyFrom(directories.Wrap(), directories.Wrap()>>OrderBy(comparer));
	CopyFrom(files.Wrap(), files.Wrap()>>OrderBy(comparer));
}

Ptr<GuiImageData> GetFileIcon(const WString& fullPath, UINT uFlags)
{
	// Use SHGetFileInfo to get the correct icons for the specified directory or file.
	SHFILEINFO info;
	DWORD result=SHGetFileInfo(fullPath.Buffer(), 0, &info, sizeof(SHFILEINFO), uFlags);
	Ptr<GuiImageData> imageData;
	if(result)
	{
		Ptr<INativeImage> image=windows::CreateImageFromHICON(info.hIcon);
		if(image)
		{
			imageData=new GuiImageData(image, 0);
		}
		DestroyIcon(info.hIcon);
	}
	return imageData;
}

WString GetFileDisplayName(const WString& fullPath)
{
	SHFILEINFO info;
	DWORD result=SHGetFileInfo(fullPath.Buffer(), 0, &info, sizeof(SHFILEINFO), SHGFI_DISPLAYNAME);
	return result?info.szDisplayName:L"";
}

WString GetFileTypeName(const WString& fullPath)
{
	SHFILEINFO info;
	DWORD result=SHGetFileInfo(fullPath.Buffer(), 0, &info, sizeof(SHFILEINFO), SHGFI_TYPENAME);
	return result?info.szTypeName:L"";
}

WString GetFileLastWriteTime(const WString& fullPath)
{
	// Get file attributes.
	WIN32_FILE_ATTRIBUTE_DATA info;
	BOOL result=GetFileAttributesEx(fullPath.Buffer(), GetFileExInfoStandard, &info);

	// Get the localized string for the file last write date.
	FILETIME localFileTime;
	SYSTEMTIME localSystemTime;
	FileTimeToLocalFileTime(&info.ftLastWriteTime, &localFileTime);
	FileTimeToSystemTime(&localFileTime, &localSystemTime);

	// Get the correct locale
	wchar_t localeName[LOCALE_NAME_MAX_LENGTH]={0};
	GetSystemDefaultLocaleName(localeName, sizeof(localeName)/sizeof(*localeName));

	// Get the localized date string
	wchar_t dateString[100]={0};
	GetDateFormatEx(localeName, DATE_SHORTDATE, &localSystemTime, NULL, dateString, sizeof(dateString)/sizeof(*dateString), NULL);

	// Get the localized time string
	wchar_t timeString[100]={0};
	GetTimeFormatEx(localeName, TIME_FORCE24HOURFORMAT | TIME_NOSECONDS, &localSystemTime, NULL, timeString, sizeof(timeString)/sizeof(*timeString));

	return dateString+WString(L" ")+timeString;
}

WString GetFileSize(const WString& fullPath)
{
	// Get file attributes.
	WIN32_FILE_ATTRIBUTE_DATA info;
	BOOL result=GetFileAttributesEx(fullPath.Buffer(), GetFileExInfoStandard, &info);

	// Get the string for file size
	LARGE_INTEGER li;
	li.HighPart=info.nFileSizeHigh;
	li.LowPart=info.nFileSizeLow;

	WString unit;
	double size=0;
	if(li.QuadPart>=1024*1024*1024)
	{
		unit=L" GB";
		size=(double)li.QuadPart/(1024*1024*1024);
	}
	else if(li.QuadPart>=1024*1024)
	{
		unit=L" MB";
		size=(double)li.QuadPart/(1024*1024);
	}
	else if(li.QuadPart>=1024)
	{
		unit=L" KB";
		size=(double)li.QuadPart/1024;
	}
	else
	{
		unit=L" Bytes";
		size=(double)li.QuadPart;
	}

	WString sizeString=ftow(size);
	const wchar_t* reading=sizeString.Buffer();
	const wchar_t* point=wcschr(sizeString.Buffer(), L'.');
	if(point)
	{
		const wchar_t* max=reading+sizeString.Length();
		point+=4;
		if(point>max) point=max;
		sizeString=sizeString.Left(point-reading);
	}

	return sizeString+unit;
}