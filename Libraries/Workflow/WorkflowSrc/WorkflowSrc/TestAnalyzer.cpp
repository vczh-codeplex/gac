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

	{
		auto parent = manager.globalName;
		auto name = parent->children[L"system"];
		TEST_ASSERT(name->typeDescriptor == 0);
		TEST_ASSERT(name->declarations.Count() == 0);
		TEST_ASSERT(name->parent == parent.Obj());
	}
	{
		auto parent = manager.globalName->children[L"system"];
		auto name = parent->children[L"Object"];
		TEST_ASSERT(name->typeDescriptor == GetTypeDescriptor<Value>());
		TEST_ASSERT(name->declarations.Count() == 0);
		TEST_ASSERT(name->parent ==parent.Obj());
	}
	{
		auto parent = manager.globalName->children[L"system"]->children[L"reflection"];
		auto name = parent->children[L"TypeDescriptor"];
		TEST_ASSERT(name->typeDescriptor == GetTypeDescriptor<ITypeDescriptor>());
		TEST_ASSERT(name->declarations.Count() == 0);
		TEST_ASSERT(name->parent ==parent.Obj());
	}
	{
		auto parent = manager.globalName;
		auto name = parent->children[L"test"];
		TEST_ASSERT(name->typeDescriptor == 0);
		TEST_ASSERT(name->declarations.Count() == 2);
		TEST_ASSERT(name->parent == parent.Obj());
	}
	{
		auto parent = manager.globalName->children[L"test"];
		auto name = parent->children[L"itoa"];
		TEST_ASSERT(name->typeDescriptor == 0);
		TEST_ASSERT(name->declarations.Count() == 1);
		TEST_ASSERT(name->parent == parent.Obj());

		auto function = name->declarations[0].Cast<WfFunctionDeclaration>();
		TEST_ASSERT(function->name.value == L"itoa");
	}
	{
		auto parent = manager.globalName;
		auto name = parent->children[L"main"];
		TEST_ASSERT(name->typeDescriptor == 0);
		TEST_ASSERT(name->declarations.Count() == 1);
		TEST_ASSERT(name->parent == parent.Obj());

		auto function = name->declarations[0].Cast<WfFunctionDeclaration>();
		TEST_ASSERT(function->name.value == L"main");
	}

	UnloadTypes();
}