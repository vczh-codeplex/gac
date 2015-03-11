#include "TestFunctions.h"

TEST_CASE(TestBreakpointTesting_Ins)
{
	auto debugger = MakePtr<WfDebugger>();
	auto callback = GetDebuggerCallback(debugger.Obj());

	auto assembly = MakePtr<WfAssembly>();

	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Ins(assembly.Obj(), 0)) == 0);
	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Ins(assembly.Obj(), 0)) == -1);
	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Ins(assembly.Obj(), 1)) == 1);
	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Ins(assembly.Obj(), 1)) == -1);

	TEST_ASSERT(callback->BreakIns(assembly.Obj(), 0) == true);
	TEST_ASSERT(callback->BreakIns(assembly.Obj(), 1) == true);
	TEST_ASSERT(callback->BreakIns(assembly.Obj(), 2) == false);

	TEST_ASSERT(debugger->RemoveBreakPoint(0) == true);
	TEST_ASSERT(debugger->RemoveBreakPoint(0) == false);
	TEST_ASSERT(debugger->RemoveBreakPoint(1) == true);
	TEST_ASSERT(debugger->RemoveBreakPoint(1) == false);
	
	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Ins(assembly.Obj(), 3)) == 1);
	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Ins(assembly.Obj(), 3)) == -1);
	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Ins(assembly.Obj(), 4)) == 0);
	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Ins(assembly.Obj(), 4)) == -1);

	TEST_ASSERT(callback->BreakIns(assembly.Obj(), 1) == false);
	TEST_ASSERT(callback->BreakIns(assembly.Obj(), 3) == true);
	TEST_ASSERT(callback->BreakIns(assembly.Obj(), 5) == false);

	TEST_ASSERT(debugger->RemoveBreakPoint(0) == true);
	TEST_ASSERT(debugger->RemoveBreakPoint(0) == false);
	TEST_ASSERT(debugger->RemoveBreakPoint(1) == true);
	TEST_ASSERT(debugger->RemoveBreakPoint(1) == false);
}

TEST_CASE(TestBreakpointTesting_Read)
{
	auto debugger = MakePtr<WfDebugger>();
	auto callback = GetDebuggerCallback(debugger.Obj());
}

TEST_CASE(TestBreakpointTesting_Write)
{
	auto debugger = MakePtr<WfDebugger>();
	auto callback = GetDebuggerCallback(debugger.Obj());
}

TEST_CASE(TestBreakpointTesting_Get)
{
	auto debugger = MakePtr<WfDebugger>();
	auto callback = GetDebuggerCallback(debugger.Obj());
}

TEST_CASE(TestBreakpointTesting_Set)
{
	auto debugger = MakePtr<WfDebugger>();
	auto callback = GetDebuggerCallback(debugger.Obj());
}

TEST_CASE(TestBreakpointTesting_Attach)
{
	auto debugger = MakePtr<WfDebugger>();
	auto callback = GetDebuggerCallback(debugger.Obj());
}

TEST_CASE(TestBreakpointTesting_Detach)
{
	auto debugger = MakePtr<WfDebugger>();
	auto callback = GetDebuggerCallback(debugger.Obj());
}

TEST_CASE(TestBreakpointTesting_Invoke)
{
	auto debugger = MakePtr<WfDebugger>();
	auto callback = GetDebuggerCallback(debugger.Obj());
}

TEST_CASE(TestBreakpointTesting_Create)
{
	auto debugger = MakePtr<WfDebugger>();
	auto callback = GetDebuggerCallback(debugger.Obj());
}