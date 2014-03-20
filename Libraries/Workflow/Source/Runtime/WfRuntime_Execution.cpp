#include "WfRuntime.h"

namespace vl
{
	namespace workflow
	{
		namespace runtime
		{
			using namespace reflection;
			using namespace reflection::description;

/***********************************************************************
WfRuntimeThreadContext
***********************************************************************/

			WfRuntimeExecutionAction WfRuntimeThreadContext::Execute()
			{
#define INTERNAL_ERROR(MESSAGE)\
				do{\
					RaiseException(BoxValue<WString>(L"Internal error: " MESSAGE));\
					return WfRuntimeExecutionAction::Nop; \
				} while (0)\

#define CONTEXT_ACTION(ACTION, MESSAGE)\
				if ((ACTION) != WfRuntimeThreadContextError::Success)\
				{\
					INTERNAL_ERROR(MESSAGE);\
				}\

				switch (status)
				{
				case WfRuntimeExecutionStatus::Ready:
				case WfRuntimeExecutionStatus::Executing:
					{
						if (stackFrames.Count() == 0)
						{
							INTERNAL_ERROR(L"empty stack frame.");
						}
						auto& stackFrame = GetCurrentStackFrame();
						if (stackFrame.nextInstructionIndex < 0 || stackFrame.nextInstructionIndex >= globalContext->assembly->instructions.Count())
						{
							INTERNAL_ERROR(L"illegal instruction index.");
						}
						auto& ins = globalContext->assembly->instructions[stackFrame.nextInstructionIndex++];

						switch (ins.code)
						{
						case WfInsCode::Nop:
							break;
						case WfInsCode::LoadValue:
							PushValue(ins.valueParameter);
							return WfRuntimeExecutionAction::ExecuteInstruction;
						case WfInsCode::LoadFunction:
							throw 0;
						case WfInsCode::LoadLambda:
							throw 0;
						case WfInsCode::LoadException:
							throw 0;
						case WfInsCode::LoadLocalVar:
							{
								Value result;
								CONTEXT_ACTION(LoadLocalVariable(ins.indexParameter, result), L"illegal local variable index.");
								PushValue(result);
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
							break;
						case WfInsCode::LoadCapturedVar:
							{
								Value result;
								CONTEXT_ACTION(LoadCapturedVariable(ins.indexParameter, result), L"illegal captured variable index.");
								PushValue(result);
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
							break;
						case WfInsCode::LoadGlobalVar:
							{
								Value result;
								CONTEXT_ACTION(LoadGlobalVariable(ins.indexParameter, result), L"illegal global variable index.");
								PushValue(result);
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
							break;
						case WfInsCode::StoreLocalVar:
							{
								Value result;
								CONTEXT_ACTION(PopValue(result), L"failed to pop a value from the stack.");
								CONTEXT_ACTION(StoreLocalVariable(ins.indexParameter, result), L"illegal local variable index.");
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
							break;
						case WfInsCode::StoreGlobalVar:
							{
								Value result;
								CONTEXT_ACTION(PopValue(result), L"failed to pop a value from the stack.");
								CONTEXT_ACTION(StoreGlobalVariable(ins.indexParameter, result), L"illegal global variable index.");
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
							break;
						case WfInsCode::Pop:
							{
								Value result;
								CONTEXT_ACTION(PopValue(result), L"failed to pop a value from the stack.");
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
							break;
						case WfInsCode::Return:
							{
								Value result;
								CONTEXT_ACTION(PopValue(result), L"failed to pop the function result.");
								CONTEXT_ACTION(PopStackFrame(), L"failed to pop the the stack frame.");
								PushValue(result);
								if (stackFrames.Count() == 0)
								{
									status = WfRuntimeExecutionStatus::Finished;
								}
								return WfRuntimeExecutionAction::ExitStackFrame;
							}
							break;
						case WfInsCode::CreateArray:
							throw 0;
						case WfInsCode::CreateMap:
							throw 0;
						case WfInsCode::ConvertToType:
							throw 0;
						case WfInsCode::AssertAsType:
							throw 0;
						case WfInsCode::TestType:
							throw 0;
						case WfInsCode::Jump:
							throw 0;
						case WfInsCode::JumpIf:
							throw 0;
						case WfInsCode::Invoke:
							throw 0;
						case WfInsCode::InvokeMethod:
							throw 0;
						case WfInsCode::AttachEvent:
							throw 0;
						case WfInsCode::DetachEvent:
							throw 0;
						case WfInsCode::InstallTry:
							throw 0;
						case WfInsCode::UninstallTry:
							throw 0;
						case WfInsCode::RaiseException:
							throw 0;
						case WfInsCode::CompareLiteral:
							throw 0;
						case WfInsCode::CompareReference:
							throw 0;
						case WfInsCode::OpNot:
							throw 0;
						case WfInsCode::OpPositive:
							throw 0;
						case WfInsCode::OpNegative:
							throw 0;
						case WfInsCode::OpConcat:
							throw 0;
						case WfInsCode::OpExp:
							throw 0;
						case WfInsCode::OpAdd:
							throw 0;
						case WfInsCode::OpSub:
							throw 0;
						case WfInsCode::OpMul:
							throw 0;
						case WfInsCode::OpDiv:
							throw 0;
						case WfInsCode::OpShl:
							throw 0;
						case WfInsCode::OpShr:
							throw 0;
						case WfInsCode::OpXor:
							throw 0;
						case WfInsCode::OpAnd:
							throw 0;
						case WfInsCode::OpOr:
							throw 0;
						case WfInsCode::OpLT:
							throw 0;
						case WfInsCode::OpGT:
							throw 0;
						case WfInsCode::OpLE:
							throw 0;
						case WfInsCode::OpGE:
							throw 0;
						case WfInsCode::OpEQ:
							throw 0;
						case WfInsCode::OpNE:
							throw 0;
						}
					}
					break;
				case WfRuntimeExecutionStatus::RaisedException:
					break;
				}
				return WfRuntimeExecutionAction::Nop;

#undef INTERNAL_ERROR
#undef CONTEXT_ACTION
			}
		}
	}
}