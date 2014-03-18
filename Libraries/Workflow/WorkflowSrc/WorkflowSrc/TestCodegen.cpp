#include "TestFunctions.h"

TEST_CASE(TestCodegen)
{
	LoadTypes();
	Ptr<ParsingTable> table = GetWorkflowTable();
	List<WString> itemNames;
	LoadSampleIndex(L"Codegen", itemNames);

	WfLexicalScopeManager manager(table);
	FOREACH(WString, itemName, itemNames)
	{
		UnitTest::PrintInfo(itemName);
		WString sample = LoadSample(L"Codegen", itemName);
		List<Ptr<ParsingError>> errors;
		Ptr<ParsingTreeNode> node = WfParseModuleAsParsingTreeNode(sample, table, errors);
		TEST_ASSERT(node);

		manager.Clear(true, true);
		List<RegexToken> tokens;
		Ptr<WfModule> module = WfConvertParsingTreeNode(node, tokens).Cast<WfModule>();
		manager.modules.Add(module);
		manager.Rebuild(true);
		LogSampleParseResult(L"Codegen", itemName, sample, node, &manager);
		TEST_ASSERT(manager.errors.Count() == 0);
		
		Ptr<WfAssembly> assembly = GenerateAssembly(&manager);
		TEST_ASSERT(assembly);
		LogSampleCodegenResult(L"Codegen", itemName, assembly);
	}

	UnloadTypes();
}