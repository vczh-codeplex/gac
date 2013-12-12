#include "TestFunctions.h"

TEST_CASE(TestBuildGlobalName)
{
	Ptr<ParsingTable> table = GetWorkflowTable();
	LoadTypes();
	WfLexicalScopeManager manager;
	{
		List<WString> itemNames;
		LoadSampleIndex(L"AnalyzerScope", itemNames);
		FOREACH(WString, itemName, itemNames)
		{
			WString sample = LoadSample(L"AnalyzerScope", itemName);

			List<Ptr<ParsingError>> errors;
			Ptr<ParsingTreeNode> node = WfParseModuleAsParsingTreeNode(sample, table, errors);
			TEST_ASSERT(node);
			LogSampleParseResult(L"AnalyzerScope", itemName, sample, node);

			Ptr<WfModule> module = WfParseModule(sample, table);
			manager.modules.Add(module);
		}
	}
	manager.BuildGlobalName();
	UnloadTypes();
}