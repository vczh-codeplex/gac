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
		
		void OnBlockExecution()override
		{
			blockOperatorEvent.Signal();
			blockDebuggerEvent.Wait();
		}

		void OnStartExecution()override
		{
			blockDebuggerEvent.Wait();
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

		void BeginExecution()
		{
			TEST_ASSERT(state == Stopped);
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
			debugger->BeginExecution();
			debugger->BeginExecution();

			TEST_ASSERT(debugger->GetState() == WfDebugger::PauseByBreakPoint);
			TEST_ASSERT(debugger->GetLastActivatedBreakPoint() == 0);
			TEST_ASSERT(debugger->GetCurrentPosition().start.row == 5);
			TEST_ASSERT(UnboxValue<WString>(debugger->GetValueByName(L"s")) == L"zero");
			TEST_ASSERT(debugger->Run());
			TEST_ASSERT(debugger->GetState() == WfDebugger::Continue);
			debugger->Continue();
			
			TEST_ASSERT(debugger->GetState() == WfDebugger::PauseByBreakPoint);
			TEST_ASSERT(debugger->GetLastActivatedBreakPoint() == 1);
			TEST_ASSERT(debugger->GetCurrentPosition().start.row == 6);
			TEST_ASSERT(UnboxValue<WString>(debugger->GetValueByName(L"s")) == L"one");
			TEST_ASSERT(debugger->Run());
			TEST_ASSERT(debugger->GetState() == WfDebugger::Continue);
			debugger->Continue();
			
			TEST_ASSERT(debugger->GetState() == WfDebugger::PauseByBreakPoint);
			TEST_ASSERT(debugger->GetLastActivatedBreakPoint() == 2);
			TEST_ASSERT(debugger->GetCurrentPosition().start.row == 7);
			TEST_ASSERT(UnboxValue<WString>(debugger->GetValueByName(L"s")) == L"two");
			TEST_ASSERT(debugger->Run());
			TEST_ASSERT(debugger->GetState() == WfDebugger::Continue);
			debugger->Continue();

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
			debugger->BeginExecution();
			debugger->BeginExecution();

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
	TEST_ASSERT(debugger->AddCodeLineBreakPoint(assembly, 0, 6) == 1);
	TEST_ASSERT(debugger->AddCodeLineBreakPoint(assembly, 0, 7) == 2);

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