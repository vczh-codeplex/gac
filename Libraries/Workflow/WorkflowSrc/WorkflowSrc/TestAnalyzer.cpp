#include "TestFunctions.h"

void LoadMultipleSamples(WfLexicalScopeManager* manager, const WString& sampleName)
{
	Ptr<ParsingTable> table = GetWorkflowTable();
	List<WString> itemNames;
	LoadSampleIndex(sampleName, itemNames);
	FOREACH(WString, itemName, itemNames)
	{
		WString sample = LoadSample(sampleName, itemName);

		List<Ptr<ParsingError>> errors;
		Ptr<ParsingTreeNode> node = WfParseModuleAsParsingTreeNode(sample, table, errors);
		TEST_ASSERT(node);
		LogSampleParseResult(sampleName, itemName, sample, node);

		List<RegexToken> tokens;
		Ptr<WfModule> module = WfConvertParsingTreeNode(node, tokens).Cast<WfModule>();
		manager->modules.Add(module);
	}
}

TEST_CASE(TestBuildGlobalName)
{
	LoadTypes();
	WfLexicalScopeManager manager;
	LoadMultipleSamples(&manager, L"AnalyzerScope");
	manager.BuildGlobalName(false);
	manager.BuildScopes();

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
		TEST_ASSERT(manager.namespaceNames[name->declarations[0].Cast<WfNamespaceDeclaration>().Obj()] == name);
		TEST_ASSERT(manager.namespaceNames[name->declarations[1].Cast<WfNamespaceDeclaration>().Obj()] == name);
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