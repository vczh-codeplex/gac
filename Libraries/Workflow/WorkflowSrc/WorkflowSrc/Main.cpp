#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <windows.h>

#include "TestFunctions.h"
#include "..\..\..\..\Common\Source\Console.h"

Ptr<ParsingTable> workflowTable;

Ptr<ParsingTable> GetWorkflowTable()
{
	if (!workflowTable)
	{
		workflowTable = WfLoadTable();
	}
	return workflowTable;
}

WString GetPath()
{
	wchar_t buffer[65536];
	GetModuleFileName(NULL, buffer, sizeof(buffer)/sizeof(*buffer));
	vint pos=-1;
	vint index=0;
	while(buffer[index])
	{
		if(buffer[index]==L'\\')
		{
			pos=index;
		}
		index++;
	}
#ifdef _WIN64
	return WString(buffer, pos+1)+L"..\\..\\TestFiles\\";
#else
	return WString(buffer, pos+1)+L"..\\TestFiles\\";
#endif
}

void LoadSampleIndex(const WString& sampleName, List<WString>& itemNames)
{
	FileStream fileStream(GetPath() + L"Samples\\Index" + sampleName + L".txt", FileStream::ReadOnly);
	BomDecoder decoder;
	DecoderStream decoderStream(fileStream, decoder);
	StreamReader reader(decoderStream);

	while (!reader.IsEnd())
	{
		itemNames.Add(reader.ReadLine());
	}
}

WString LoadSample(const WString& sampleName, const WString& itemName)
{
	FileStream fileStream(GetPath() + L"Samples\\" + sampleName + L"\\" + itemName + L".txt", FileStream::ReadOnly);
	BomDecoder decoder;
	DecoderStream decoderStream(fileStream, decoder);
	StreamReader reader(decoderStream);
	return reader.ReadToEnd();
}

void LogSampleParseResult(const WString& sampleName, const WString& itemName, const WString& sample, Ptr<ParsingTreeNode> node, WfLexicalScopeManager* manager)
{
	FileStream fileStream(GetPath() + L"Parsing." + sampleName + L"." + itemName + L".txt", FileStream::WriteOnly);
	BomEncoder encoder(BomEncoder::Utf16);
	EncoderStream encoderStream(fileStream, encoder);
	StreamWriter writer(encoderStream);
	writer.WriteLine(sample);

	if (manager && manager->errors.Count() > 0)
	{
		writer.WriteLine(L"========================================================");
		FOREACH(Ptr<ParsingError>, error, manager->errors)
		{
			writer.WriteLine(L"Line: " + itow(error->codeRange.start.row + 1) + L", Column: " + itow(error->codeRange.start.column + 1) + L", Message: " + error->errorMessage);
		}
	}

	writer.WriteLine(L"========================================================");
	Log(node.Obj(), L"", writer);
}

void LoadTypes()
{
	LoadPredefinedTypes();
	LoadParsingTypes();
	XmlLoadTypes();
	JsonLoadTypes();
	WfLoadTypes();
	TEST_ASSERT(GetGlobalTypeManager()->Load());
}

void UnloadTypes()
{
	TEST_ASSERT(GetGlobalTypeManager()->Unload());
	TEST_ASSERT(ResetGlobalTypeManager());
}

int wmain(int argc, wchar_t* argv[])
{
	workflowTable = 0;
	_CrtDumpMemoryLeaks();
	return 0;
}