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

	auto assembly = MakePtr<WfAssembly>();
	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Read(assembly.Obj(), 0)) == 0);
	TEST_ASSERT(callback->BreakRead(assembly.Obj(), 0) == true);
	TEST_ASSERT(debugger->RemoveBreakPoint(0) == true);
	TEST_ASSERT(callback->BreakRead(assembly.Obj(), 0) == false);
}

TEST_CASE(TestBreakpointTesting_Write)
{
	auto debugger = MakePtr<WfDebugger>();
	auto callback = GetDebuggerCallback(debugger.Obj());

	auto assembly = MakePtr<WfAssembly>();
	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Write(assembly.Obj(), 0)) == 0);
	TEST_ASSERT(callback->BreakWrite(assembly.Obj(), 0) == true);
	TEST_ASSERT(debugger->RemoveBreakPoint(0) == true);
	TEST_ASSERT(callback->BreakWrite(assembly.Obj(), 0) == false);
}

TEST_CASE(TestBreakpointTesting_Get)
{
	auto debugger = MakePtr<WfDebugger>();
	auto callback = GetDebuggerCallback(debugger.Obj());

	auto td = GetTypeDescriptor(L"test::ObservableValue");
	auto prop1 = td->GetPropertyByName(L"Name", true);
	auto prop2 = td->GetPropertyByName(L"DisplayName", true);

	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Get(nullptr, prop1)) == 0);
	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Get(nullptr, prop2)) == 1);
	TEST_ASSERT(callback->BreakGet(nullptr, prop1) == true);
	TEST_ASSERT(callback->BreakGet(nullptr, prop2) == true);
	TEST_ASSERT(callback->BreakInvoke(nullptr, prop1->GetGetter()) == true);
	TEST_ASSERT(callback->BreakInvoke(nullptr, prop2->GetGetter()) == true);
	TEST_ASSERT(debugger->RemoveBreakPoint(0) == true);
	TEST_ASSERT(debugger->RemoveBreakPoint(1) == true);
	TEST_ASSERT(callback->BreakGet(nullptr, prop1) == false);
	TEST_ASSERT(callback->BreakGet(nullptr, prop2) == false);
	TEST_ASSERT(callback->BreakInvoke(nullptr, prop1->GetGetter()) == false);
	TEST_ASSERT(callback->BreakInvoke(nullptr, prop2->GetGetter()) == false);
}

TEST_CASE(TestBreakpointTesting_Set)
{
	auto debugger = MakePtr<WfDebugger>();
	auto callback = GetDebuggerCallback(debugger.Obj());

	auto td = GetTypeDescriptor(L"test::ObservableValue");
	auto prop1 = td->GetPropertyByName(L"Name", true);
	auto prop2 = td->GetPropertyByName(L"DisplayName", true);

	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Set(nullptr, prop1)) == 0);
	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Set(nullptr, prop2)) == 1);
	TEST_ASSERT(callback->BreakSet(nullptr, prop1) == true);
	TEST_ASSERT(callback->BreakSet(nullptr, prop2) == true);
	TEST_ASSERT(callback->BreakInvoke(nullptr, prop1->GetSetter()) == true);
	TEST_ASSERT(callback->BreakInvoke(nullptr, prop2->GetSetter()) == false);
	TEST_ASSERT(debugger->RemoveBreakPoint(0) == true);
	TEST_ASSERT(debugger->RemoveBreakPoint(1) == true);
	TEST_ASSERT(callback->BreakSet(nullptr, prop1) == false);
	TEST_ASSERT(callback->BreakSet(nullptr, prop2) == false);
	TEST_ASSERT(callback->BreakInvoke(nullptr, prop1->GetSetter()) == false);
	TEST_ASSERT(callback->BreakInvoke(nullptr, prop2->GetSetter()) == false);
}

TEST_CASE(TestBreakpointTesting_Attach)
{
	auto debugger = MakePtr<WfDebugger>();
	auto callback = GetDebuggerCallback(debugger.Obj());

	auto td = GetTypeDescriptor(L"test::ObservableValue");
	auto ev = td->GetEventByName(L"ValueChanged", true);

	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Attach(nullptr, ev)) == 0);
	TEST_ASSERT(callback->BreakAttach(nullptr, ev) == true);
	TEST_ASSERT(debugger->RemoveBreakPoint(0) == true);
	TEST_ASSERT(callback->BreakAttach(nullptr, ev) == false);
}

TEST_CASE(TestBreakpointTesting_Detach)
{
	auto debugger = MakePtr<WfDebugger>();
	auto callback = GetDebuggerCallback(debugger.Obj());

	auto td = GetTypeDescriptor(L"test::ObservableValue");
	auto ev = td->GetEventByName(L"ValueChanged", true);

	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Detach(nullptr, ev)) == 0);
	TEST_ASSERT(callback->BreakDetach(nullptr, ev) == true);
	TEST_ASSERT(debugger->RemoveBreakPoint(0) == true);
	TEST_ASSERT(callback->BreakDetach(nullptr, ev) == false);
}

TEST_CASE(TestBreakpointTesting_Invoke)
{
	auto debugger = MakePtr<WfDebugger>();
	auto callback = GetDebuggerCallback(debugger.Obj());

	auto td = GetTypeDescriptor(L"test::ObservableValue");
	auto m = td->GetMethodGroupByName(L"Create", true)->GetMethod(0);

	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Invoke(nullptr, m)) == 0);
	TEST_ASSERT(callback->BreakInvoke(nullptr, m) == true);
	TEST_ASSERT(debugger->RemoveBreakPoint(0) == true);
	TEST_ASSERT(callback->BreakInvoke(nullptr, m) == false);
}

TEST_CASE(TestBreakpointTesting_Create)
{
	auto debugger = MakePtr<WfDebugger>();
	auto callback = GetDebuggerCallback(debugger.Obj());
	
	auto td = GetTypeDescriptor(L"test::ObservableValue");
	auto mg = td->GetConstructorGroup();
	auto count = mg->GetMethodCount();

	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Create(td)) == 0);
	TEST_ASSERT(callback->BreakCreate(td) == true);
	for (vint i = 0; i < count; i++)
	{
		TEST_ASSERT(callback->BreakInvoke(nullptr, mg->GetMethod(i)) == true);
	}

	TEST_ASSERT(debugger->RemoveBreakPoint(0) == true);
	TEST_ASSERT(callback->BreakCreate(td) == false);
	for (vint i = 0; i < count; i++)
	{
		TEST_ASSERT(callback->BreakInvoke(nullptr, mg->GetMethod(i)) == false);
	}
}

namespace debugger_helper
{
	Ptr<WfRuntimeGlobalContext> CreateThreadContextFromSample(const WString& name)
	{
		List<WString> moduleCodes;
		moduleCodes.Add(LoadSample(L"Debugger", name));
		List<Ptr<ParsingError>> errors;
		auto assembly = Compile(GetWorkflowTable(), moduleCodes, errors);
		TEST_ASSERT(assembly && errors.Count() == 0);
		return new WfRuntimeGlobalContext(assembly);
	}
}
using namespace debugger_helper;

TEST_CASE(TestDebugger_NoBreakPoint)
{
	SetDebugferForCurrentThread(new WfDebugger);
	auto context = CreateThreadContextFromSample(L"HelloWorld");
	LoadFunction<void()>(context, L"<initialize>")();
	auto result = LoadFunction<WString()>(context, L"Main")();
	TEST_ASSERT(result == L"Hello, world!");
	SetDebugferForCurrentThread(nullptr);
}

namespace debugger_helper
{
	class MultithreadDebugger : public WfDebugger
	{
	protected:
		Ptr<Thread>				debuggerOperatorThread;
		EventObject				blockOperatorEvent;
		EventObject				blockDebuggerEvent;
		bool					requiredToPause = false;
		
		void OnBlockExecution()override
		{
			blockOperatorEvent.Signal();
			blockDebuggerEvent.Wait();
		}

		void OnStartExecution()override
		{
			blockDebuggerEvent.Wait();
			if (requiredToPause)
			{
				Pause();
			}
		}

		void OnStopExecution()override
		{
			blockOperatorEvent.Signal();
		}
	public:
		MultithreadDebugger(Func<void(MultithreadDebugger*)> debuggerOperator)
		{
			blockOperatorEvent.CreateAutoUnsignal(false);
			blockDebuggerEvent.CreateAutoUnsignal(false);
			debuggerOperatorThread = Thread::CreateAndStart(
				[=]()
				{
					debuggerOperator(this);
				}, false);
		}

		~MultithreadDebugger()
		{
			debuggerOperatorThread->Wait();
		}

		void Continue()
		{
			TEST_ASSERT(state != Stopped);
			blockDebuggerEvent.Signal();
			blockOperatorEvent.Wait();
		}

		void BeginExecution(bool pause)
		{
			TEST_ASSERT(state == Stopped);
			requiredToPause = pause;
			blockDebuggerEvent.Signal();
			blockOperatorEvent.Wait();
		}
	};
}
using namespace debugger_helper;

TEST_CASE(TestDebugger_CodeLineBreakPoint)
{
	auto debugger = MakePtr<MultithreadDebugger>(
		[](MultithreadDebugger* debugger)
		{
			debugger->BeginExecution(false);
			debugger->BeginExecution(false);

			vint rows[] = { 5, 6, 7 };
			vint breakPoints[] = { 0, 1, 2 };
			WString values[] = { L"zero", L"one", L"two" };

			for (vint i = 0; i < sizeof(rows) / sizeof(*rows); i++)
			{
				TEST_ASSERT(debugger->GetState() == WfDebugger::PauseByBreakPoint);
				TEST_ASSERT(debugger->GetLastActivatedBreakPoint() == breakPoints[i]);
				TEST_ASSERT(debugger->GetCurrentPosition().start.row == rows[i]);
				TEST_ASSERT(UnboxValue<WString>(debugger->GetValueByName(L"s")) == values[i]);
				TEST_ASSERT(debugger->Run());
				TEST_ASSERT(debugger->GetState() == WfDebugger::Continue);
				debugger->Continue();
			}

			TEST_ASSERT(debugger->GetState() == WfDebugger::Stopped);
		});
	SetDebugferForCurrentThread(debugger);

	auto context = CreateThreadContextFromSample(L"Assignment");
	auto assembly = context->assembly.Obj();
	TEST_ASSERT(debugger->AddCodeLineBreakPoint(assembly, 0, 5) == 0);
	TEST_ASSERT(debugger->AddCodeLineBreakPoint(assembly, 0, 6) == 1);
	TEST_ASSERT(debugger->AddCodeLineBreakPoint(assembly, 0, 7) == 2);

	LoadFunction<void()>(context, L"<initialize>")();
	auto result = LoadFunction<WString()>(context, L"Main")();
	TEST_ASSERT(result == L"three");
	SetDebugferForCurrentThread(nullptr);
}

TEST_CASE(TestDebugger_Stop)
{
	auto debugger = MakePtr<MultithreadDebugger>(
		[](MultithreadDebugger* debugger)
		{
			debugger->BeginExecution(false);
			debugger->BeginExecution(false);

			TEST_ASSERT(debugger->GetState() == WfDebugger::PauseByBreakPoint);
			TEST_ASSERT(debugger->GetLastActivatedBreakPoint() == 0);
			TEST_ASSERT(debugger->Stop());
			TEST_ASSERT(debugger->GetState() == WfDebugger::RequiredToStop);
			debugger->Continue();
		});
	SetDebugferForCurrentThread(debugger);

	auto context = CreateThreadContextFromSample(L"Assignment");
	auto assembly = context->assembly.Obj();
	TEST_ASSERT(debugger->AddCodeLineBreakPoint(assembly, 0, 5) == 0);

	LoadFunction<void()>(context, L"<initialize>")();
	try
	{
		LoadFunction<WString()>(context, L"Main")();
		TEST_ASSERT(false);
	}
	catch (const TypeDescriptorException& ex)
	{
		TEST_ASSERT(ex.Message() == L"Internal error: Debugger stopped the program.");
	}
	SetDebugferForCurrentThread(nullptr);
}

TEST_CASE(TestDebugger_StepOver1)
{
	auto debugger = MakePtr<MultithreadDebugger>(
		[](MultithreadDebugger* debugger)
		{
			debugger->BeginExecution(false);
			debugger->BeginExecution(true);

			vint rows[] = { 4, 5, 6, 7, 8 };
			WString values[] = { L"", L"zero", L"one", L"two", L"three" };

			for (vint i = 0; i < sizeof(rows) / sizeof(*rows); i++)
			{
				TEST_ASSERT(debugger->GetState() == WfDebugger::PauseByOperation);
				TEST_ASSERT(debugger->GetLastActivatedBreakPoint() == WfDebugger::PauseBreakPoint);
				TEST_ASSERT(debugger->GetCurrentPosition().start.row == rows[i]);
				auto s = debugger->GetValueByName(L"s");
				TEST_ASSERT((s.IsNull() && values[i] == L"") || UnboxValue<WString>(s) == values[i]);
				TEST_ASSERT(debugger->StepOver());
				TEST_ASSERT(debugger->GetState() == WfDebugger::Continue);
				debugger->Continue();
			}

			TEST_ASSERT(debugger->GetState() == WfDebugger::Stopped);
		});
	SetDebugferForCurrentThread(debugger);

	auto context = CreateThreadContextFromSample(L"Assignment");
	auto assembly = context->assembly.Obj();

	LoadFunction<void()>(context, L"<initialize>")();
	auto result = LoadFunction<WString()>(context, L"Main")();
	TEST_ASSERT(result == L"three");
	SetDebugferForCurrentThread(nullptr);
}

TEST_CASE(TestDebugger_StepOver2)
{
	auto debugger = MakePtr<MultithreadDebugger>(
		[](MultithreadDebugger* debugger)
		{
			debugger->BeginExecution(false);
			debugger->BeginExecution(true);

			vint rows[] = { 19, 20, 21, 22, 23 };
			WString values[] = { L"", L"zero", L"one", L"two", L"three" };

			for (vint i = 0; i < sizeof(rows) / sizeof(*rows); i++)
			{
				TEST_ASSERT(debugger->GetState() == WfDebugger::PauseByOperation);
				TEST_ASSERT(debugger->GetLastActivatedBreakPoint() == WfDebugger::PauseBreakPoint);
				TEST_ASSERT(debugger->GetCurrentPosition().start.row == rows[i]);
				auto s = debugger->GetValueByName(L"s");
				TEST_ASSERT((s.IsNull() && values[i] == L"") || UnboxValue<WString>(s) == values[i]);
				TEST_ASSERT(debugger->StepOver());
				TEST_ASSERT(debugger->GetState() == WfDebugger::Continue);
				debugger->Continue();
			}

			TEST_ASSERT(debugger->GetState() == WfDebugger::Stopped);
		});
	SetDebugferForCurrentThread(debugger);

	auto context = CreateThreadContextFromSample(L"Function");
	auto assembly = context->assembly.Obj();

	LoadFunction<void()>(context, L"<initialize>")();
	auto result = LoadFunction<WString()>(context, L"Main")();
	TEST_ASSERT(result == L"three");
	SetDebugferForCurrentThread(nullptr);
}

TEST_CASE(TestDebugger_StepInto1)
{
	auto debugger = MakePtr<MultithreadDebugger>(
		[](MultithreadDebugger* debugger)
		{
			debugger->BeginExecution(false);
			debugger->BeginExecution(true);

			vint rows[] = { 4, 5, 6, 7, 8 };
			WString values[] = { L"", L"zero", L"one", L"two", L"three" };

			for (vint i = 0; i < sizeof(rows) / sizeof(*rows); i++)
			{
				TEST_ASSERT(debugger->GetState() == WfDebugger::PauseByOperation);
				TEST_ASSERT(debugger->GetLastActivatedBreakPoint() == WfDebugger::PauseBreakPoint);
				TEST_ASSERT(debugger->GetCurrentPosition().start.row == rows[i]);
				auto s = debugger->GetValueByName(L"s");
				TEST_ASSERT((s.IsNull() && values[i] == L"") || UnboxValue<WString>(s) == values[i]);
				TEST_ASSERT(debugger->StepInto());
				TEST_ASSERT(debugger->GetState() == WfDebugger::Continue);
				debugger->Continue();
			}

			TEST_ASSERT(debugger->GetState() == WfDebugger::Stopped);
		});
	SetDebugferForCurrentThread(debugger);

	auto context = CreateThreadContextFromSample(L"Assignment");
	auto assembly = context->assembly.Obj();

	LoadFunction<void()>(context, L"<initialize>")();
	auto result = LoadFunction<WString()>(context, L"Main")();
	TEST_ASSERT(result == L"three");
	SetDebugferForCurrentThread(nullptr);
}

TEST_CASE(TestDebugger_StepInto2)
{
	auto debugger = MakePtr<MultithreadDebugger>(
		[](MultithreadDebugger* debugger)
		{
			debugger->BeginExecution(false);
			debugger->BeginExecution(true);

			vint rows[] = { 19, 20, 4, 20, 21, 9, 21, 22, 14, 22, 23 };
			WString values[] = { L"", L"zero", L"", L"zero", L"one", L"", L"one", L"two", L"", L"two", L"three" };

			for (vint i = 0; i < sizeof(rows) / sizeof(*rows); i++)
			{
				TEST_ASSERT(debugger->GetState() == WfDebugger::PauseByOperation);
				TEST_ASSERT(debugger->GetLastActivatedBreakPoint() == WfDebugger::PauseBreakPoint);
				TEST_ASSERT(debugger->GetCurrentPosition().start.row == rows[i]);
				auto s = debugger->GetValueByName(L"s");
				TEST_ASSERT((s.IsNull() && values[i] == L"") || UnboxValue<WString>(s) == values[i]);
				TEST_ASSERT(debugger->StepInto());
				TEST_ASSERT(debugger->GetState() == WfDebugger::Continue);
				debugger->Continue();
			}

			TEST_ASSERT(debugger->GetState() == WfDebugger::Stopped);
		});
	SetDebugferForCurrentThread(debugger);

	auto context = CreateThreadContextFromSample(L"Function");
	auto assembly = context->assembly.Obj();

	LoadFunction<void()>(context, L"<initialize>")();
	auto result = LoadFunction<WString()>(context, L"Main")();
	TEST_ASSERT(result == L"three");
	SetDebugferForCurrentThread(nullptr);
}

TEST_CASE(TestDebugger_OperationBreakPoint)
{
	auto debugger = MakePtr<MultithreadDebugger>(
		[](MultithreadDebugger* debugger)
		{
			debugger->BeginExecution(false);
			debugger->BeginExecution(false);

			vint rows[] = { 8, 9, 10, 11, 12, 13, 14, 15 };
			vint breakPoints[] = { 0, 1, 2, 3, 4, 5, 6, 7 };

			for (vint i = 0; i < sizeof(rows) / sizeof(*rows); i++)
			{
				TEST_ASSERT(debugger->GetState() == WfDebugger::PauseByBreakPoint);
				TEST_ASSERT(debugger->GetLastActivatedBreakPoint() == breakPoints[i]);
				TEST_ASSERT(debugger->GetCurrentPosition().start.row == rows[i]);
				TEST_ASSERT(debugger->StepInto());
				TEST_ASSERT(debugger->GetState() == WfDebugger::Continue);
				debugger->Continue();
			}

			TEST_ASSERT(debugger->GetState() == WfDebugger::Stopped);
		});
	SetDebugferForCurrentThread(debugger);

	auto context = CreateThreadContextFromSample(L"Operation");
	auto assembly = context->assembly.Obj();
	auto variable = assembly->variableNames.IndexOf(L"s");
	auto type = GetTypeDescriptor(L"test::ObservableValue");
	auto prop = type->GetPropertyByName(L"Value", true);
	auto ev = type->GetEventByName(L"ValueChanged", true);
	auto method = type->GetMethodGroupByName(L"GetDisplayName", true)->GetMethod(0);

	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Read(assembly, variable)) == 0);
	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Write(assembly, variable)) == 1);
	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Create(type)) == 2);
	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Get(nullptr, prop)) == 3);
	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Set(nullptr, prop)) == 4);
	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Attach(nullptr, ev)) == 5);
	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Detach(nullptr, ev)) == 6);
	TEST_ASSERT(debugger->AddBreakPoint(WfBreakPoint::Invoke(nullptr, method)) == 7);

	LoadFunction<void()>(context, L"<initialize>")();
	LoadFunction<void()>(context, L"Main")();
	SetDebugferForCurrentThread(nullptr);
}