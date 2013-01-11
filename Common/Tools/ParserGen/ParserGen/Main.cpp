#include "..\..\..\Source\Console.h"
#include "..\..\..\Source\Pointer.h"
#include "..\..\..\Source\Stream\FileStream.h"
#include "..\..\..\Source\Stream\Accessor.h"
#include "..\..\..\Source\Stream\CharFormat.h"
#include "..\..\..\Source\Collections\List.h"
#include "..\..\..\Source\Collections\Dictionary.h"
#include "..\..\..\Source\Collections\Operation.h"
#include "..\..\..\Source\Parsing\ParsingAutomaton.h"
#include "..\..\..\Source\Parsing\Parsing.h"

#include <windows.h>

using namespace vl;
using namespace vl::console;
using namespace vl::stream;
using namespace vl::collections;
using namespace vl::parsing;
using namespace vl::parsing::analyzing;
using namespace vl::parsing::definitions;
using namespace vl::parsing::tabling;

/***********************************************************************
Main function

Include:xxxx
Include:xxxx
...
EscapeFunction:abc
EscapeFunction:abc
...
Namespace:abc.def.ghi
Grammar:
***********************************************************************/

WString GetDirectory(const WString& fileName)
{
	int index=0;
	for(int i=0;i<fileName.Length();i++)
	{
		if(fileName[i]==L'\\')
		{
			index=i;
		}
	}
	return fileName.Left(index+1);
}

int wmain(int argc, wchar_t* argv[])
{
	WString baseDirectory;
	{
		wchar_t currentDirectory[MAX_PATH]={0};
		GetCurrentDirectory(MAX_PATH, currentDirectory);
		baseDirectory=currentDirectory;
		if(baseDirectory[baseDirectory.Length()-1]!=L'\\')
		{
			baseDirectory+=L'\\';
		}
	}

	Console::SetTitle(L"Vczh Parser Generator for C++");
	Console::SetColor(false, true, false, true);
	Console::WriteLine(L"parsing>Files : "+itow(argc-1));
	for(int i=1;i<argc;i++)
	{
		Console::WriteLine(L"------------------------------------------------------------");
		WString inputPath=argv[i];
		if(inputPath.Length()<2 || inputPath[1]!=L':')
		{
			inputPath=baseDirectory+inputPath;
		}
		Console::WriteLine(L"parsing>Making : "+inputPath);
		if(inputPath.Length()<11 || inputPath.Right(11)!=L".parser.txt")
		{
			Console::SetColor(true, false, false, true);
			Console::WriteLine(L"error> The extenion name of the input file path must be \".parser.txt\".");
			Console::SetColor(false, true, false, true);
		}
		else
		{
			WString outputMetaPath=inputPath.Left(inputPath.Length()-11);
			WString outputHeaderPath=outputMetaPath+L".h";
			WString outputCppPath=outputMetaPath+L".cpp";
			WString logPath=outputMetaPath+L".log";
			Console::WriteLine(L"parsing>Output header path : "+outputHeaderPath);
			Console::WriteLine(L"parsing>Output cpp path : "+outputCppPath);
			Console::WriteLine(L"parsing>Log path : "+logPath);

			FileStream logFileStream(logPath, FileStream::WriteOnly);
			BomEncoder logEncoder(BomEncoder::Mbcs);
			EncoderStream logStream(logFileStream, logEncoder);
			StreamWriter logWriter(logStream);
		}
	}
	Console::WriteLine(L"Finished!");
	return 0;
}