#include "TestFunctions.h"

TEST_CASE(TestCodegen)
{
	LoadTypes();
	Ptr<ParsingTable> table = GetWorkflowTable();
	List<WString> codegenNames;
	LoadSampleIndex(L"Codegen", codegenNames);

	WfLexicalScopeManager manager(table);
	FOREACH(WString, codegenName, codegenNames)
	{
		auto pair = INVLOC.FindFirst(codegenName, L"=", Locale::None);
		WString itemName = codegenName.Sub(0, pair.key);
		WString itemResult = codegenName.Sub(pair.key + pair.value, codegenName.Length() - pair.key - pair.value);

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

		WfRuntimeThreadContext context(assembly);
		TEST_ASSERT(context.status == WfRuntimeExecutionStatus::Finished);

		vint functionIndex = assembly->functionByName[L"main"];
		context.PushStackFrame(functionIndex);
		TEST_ASSERT(context.status == WfRuntimeExecutionStatus::Ready);

		while (context.status != WfRuntimeExecutionStatus::Finished)
		{
			auto action = context.Execute();
			TEST_ASSERT(action != WfRuntimeExecutionAction::Nop);
		}
		TEST_ASSERT(context.Execute() == WfRuntimeExecutionAction::Nop);
		
		Value result;
		TEST_ASSERT(context.PopValue(result) == WfRuntimeThreadContextError::Success);
		TEST_ASSERT(result.GetText() == itemResult);
		TEST_ASSERT(context.PopValue(result) == WfRuntimeThreadContextError::EmptyStack);
	}

	UnloadTypes();
}