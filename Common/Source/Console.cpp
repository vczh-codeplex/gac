#include "Console.h"
#if defined VCZH_WINDOWS
#include <Windows.h>
#elif defined VCZH_LINUX
#include <wchar.h>
#endif

namespace vl
{
	namespace console
	{
		
/***********************************************************************
Console
***********************************************************************/

		void Console::Write(const wchar_t* string, vint length)
		{
#if defined VCZH_WINDOWS
			HANDLE outHandle=GetStdHandle(STD_OUTPUT_HANDLE);
			DWORD fileMode=0;
			DWORD written=0;
			if((GetFileType(outHandle) & FILE_TYPE_CHAR) && GetConsoleMode(outHandle, &fileMode))
			{
				WriteConsole(outHandle, string, (int)length, &written,0);
			}
			else
			{
				int codePage = GetConsoleOutputCP();
				int charCount = WideCharToMultiByte(codePage, 0, string, -1, 0, 0, 0, 0);
				char* codePageBuffer = new char[charCount];
				WideCharToMultiByte(codePage, 0, string, -1, codePageBuffer, charCount, 0, 0);
				WriteFile(outHandle, codePageBuffer, charCount-1, &written, 0);
				delete codePageBuffer;
			}
#elif defined VCZH_LINUX
			wprintf(L"%.*s", (int)length, string);
#endif
		}

		void Console::Write(const wchar_t* string)
		{
			Write(string, wcslen(string));
		}

		void Console::Write(const WString& string)
		{
			Write(string.Buffer(), string.Length());
		}

		void Console::WriteLine(const WString& string)
		{
			Write(string);
			Write(L"\r\n");
		}

		WString Console::Read()
		{
#if defined VCZH_WINDOWS
			WString result;
			DWORD count;
			for(;;)
			{
				wchar_t buffer;
				ReadConsole(GetStdHandle(STD_INPUT_HANDLE),&buffer,1,&count,0);
				if(buffer==L'\r')
				{
					ReadConsole(GetStdHandle(STD_INPUT_HANDLE),&buffer,1,&count,0);
					break;
				}
				else if(buffer==L'\n')
				{
					break;
				}
				else
				{
					result=result+buffer;
				}
			}
			return result;
#elif defined VCZH_LINUX
			wchar_t buffer[1024];
			memset(buffer, 0, sizeof(buffer));
			wscanf(L"%1023s\n", buffer);
			return buffer;
#endif
		}

		void Console::SetColor(bool red, bool green, bool blue, bool light)
		{
#if defined VCZH_WINDOWS
			WORD attribute=0;
			if(red)attribute		|=FOREGROUND_RED;
			if(green)attribute		|=FOREGROUND_GREEN;
			if(blue)attribute		|=FOREGROUND_BLUE;
			if(light)attribute		|=FOREGROUND_INTENSITY;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),attribute);
			SetConsoleTextAttribute(GetStdHandle(STD_INPUT_HANDLE),attribute);
#elif defined VCZH_LINUX
			vint color = (blue?1:0)*4 + (green?1:0)*2 + (red?1:0);
			if(light)
				wprintf(L"\e[3%dm;", (int)color);
			else
				wprintf(L"\e[9%dm;", (int)color);
#endif
		}

		void Console::SetTitle(const WString& string)
		{
#if defined VCZH_WINDOWS
			SetConsoleTitle(string.Buffer());
#endif
		}
	}
}
