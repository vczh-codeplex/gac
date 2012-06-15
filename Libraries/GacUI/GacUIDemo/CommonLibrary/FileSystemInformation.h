#ifndef GACUI_DEMO_FILE_SYSTEM_INFORMATION
#define GACUI_DEMO_FILE_SYSTEM_INFORMATION

#include "..\..\Public\Source\GacUI.h"
#include <ShlObj.h>

using namespace vl;
using namespace vl::collections;

extern WString GetWindowsDirectory();
extern void SearchDirectoriesAndFiles(const WString& path, List<WString>& directories, List<WString>& files);
extern Ptr<GuiImageData> GetFileIcon(const WString& fullPath, UINT uFlags);
extern WString GetFileDisplayName(const WString& fullPath);
extern WString GetFileTypeName(const WString& fullPath);
extern WString GetFileLastWriteTime(const WString& fullPath);
extern WString GetFileSize(const WString& fullPath);

#endif