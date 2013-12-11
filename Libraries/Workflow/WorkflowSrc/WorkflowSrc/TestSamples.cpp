#include "..\..\..\..\Common\Source\UnitTest\UnitTest.h"
#include "..\..\Source\Expression\WfExpression.h"

using namespace vl;
using namespace vl::collections;
using namespace vl::stream;
using namespace vl::unittest;
using namespace vl::parsing;
using namespace vl::parsing::tabling;
using namespace vl::workflow;

extern Ptr<ParsingTable>		GetWorkflowTable();
extern WString					GetPath();

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

void LogSampleParseResult(const WString& sampleName, const WString& itemName, const WString& sample, Ptr<ParsingTreeNode> node)
{
	FileStream fileStream(GetPath() + L"Parsing." + sampleName + L"." + itemName + L".txt", FileStream::WriteOnly);
	BomEncoder encoder(BomEncoder::Utf16);
	EncoderStream encoderStream(fileStream, encoder);
	StreamWriter writer(encoderStream);
	writer.WriteLine(sample);
	writer.WriteLine(L"========================================================");
	Log(node.Obj(), L"", writer);
}

TEST_CASE(TestSampleExpressions)
{
	Ptr<ParsingTable> table = GetWorkflowTable();
	List<WString> itemNames;
	LoadSampleIndex(L"Expression", itemNames);
	FOREACH(WString, itemName, itemNames)
	{
		UnitTest::PrintInfo(itemName);
		WString sample = LoadSample(L"Expression", itemName);
		List<Ptr<ParsingError>> errors;
		Ptr<ParsingTreeNode> node = WfParseExpressionAsParsingTreeNode(sample, table, errors);
		TEST_ASSERT(node);
		LogSampleParseResult(L"Expression", itemName, sample, node);
	}
}

TEST_CASE(TestSampleStatements)
{
	Ptr<ParsingTable> table = GetWorkflowTable();
	List<WString> itemNames;
	LoadSampleIndex(L"Statement", itemNames);
	FOREACH(WString, itemName, itemNames)
	{
		UnitTest::PrintInfo(itemName);
		WString sample = LoadSample(L"Statement", itemName);
		List<Ptr<ParsingError>> errors;
		Ptr<ParsingTreeNode> node = WfParseStatementAsParsingTreeNode(sample, table, errors);
		TEST_ASSERT(node);
		LogSampleParseResult(L"Statement", itemName, sample, node);
	}
}

TEST_CASE(TestSampleDeclarations)
{
	Ptr<ParsingTable> table = GetWorkflowTable();
	List<WString> itemNames;
	LoadSampleIndex(L"Declaration", itemNames);
	FOREACH(WString, itemName, itemNames)
	{
		UnitTest::PrintInfo(itemName);
		WString sample = LoadSample(L"Declaration", itemName);
		List<Ptr<ParsingError>> errors;
		Ptr<ParsingTreeNode> node = WfParseDeclarationAsParsingTreeNode(sample, table, errors);
		TEST_ASSERT(node);
		LogSampleParseResult(L"Declaration", itemName, sample, node);
	}
}

TEST_CASE(TestSampleModules)
{
	Ptr<ParsingTable> table = GetWorkflowTable();
	List<WString> itemNames;
	LoadSampleIndex(L"Module", itemNames);
	FOREACH(WString, itemName, itemNames)
	{
		UnitTest::PrintInfo(itemName);
		WString sample = LoadSample(L"Module", itemName);
		List<Ptr<ParsingError>> errors;
		Ptr<ParsingTreeNode> node = WfParseModuleAsParsingTreeNode(sample, table, errors);
		TEST_ASSERT(node);
		LogSampleParseResult(L"Module", itemName, sample, node);
	}
}