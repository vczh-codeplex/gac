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
		const wchar_t* reading = codegenName.Buffer();
		vint index = wcschr(reading, L'=') - reading;
		WString itemName = codegenName.Sub(0, index);
		WString itemResult = codegenName.Sub(index + 1, codegenName.Length() - index - 1);

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

TEST_CASE(TestWorkflow)
{
	LoadTypes();
	List<Ptr<ParsingError>> errors;
	List<WString> moduleCodes;
	moduleCodes.Add(LR"workflow(
module test;

func main():string
{
	return "Hello, world!";
}
)workflow");

	auto table = GetWorkflowTable();
	auto assembly = Compile(table, moduleCodes, errors);
	TEST_ASSERT(errors.Count() == 0);

	WfRuntimeThreadContext context(assembly);
	context.PushStackFrame(assembly->functionByName[L"main"]);
	context.ExecuteToEnd();

	Value result;
	TEST_ASSERT(context.PopValue(result) == WfRuntimeThreadContextError::Success);
	TEST_ASSERT(result.GetText() == L"Hello, world!");
	UnloadTypes();
}