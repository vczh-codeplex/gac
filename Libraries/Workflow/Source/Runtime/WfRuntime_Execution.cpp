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
							break;
						case WfInsCode::LoadLambda:
							break;
						case WfInsCode::LoadException:
							break;
						case WfInsCode::LoadLocalVar:
							break;
						case WfInsCode::LoadGlobalVar:
							break;
						case WfInsCode::StoreLocalVar:
							break;
						case WfInsCode::StoreGlobalVar:
							break;
						case WfInsCode::Pop:
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
							break;
						case WfInsCode::CreateMap:
							break;
						case WfInsCode::ConvertToType:
							break;
						case WfInsCode::AssertAsType:
							break;
						case WfInsCode::TestType:
							break;
						case WfInsCode::Jump:
							break;
						case WfInsCode::JumpIf:
							break;
						case WfInsCode::Invoke:
							break;
						case WfInsCode::InvokeMethod:
							break;
						case WfInsCode::AttachEvent:
							break;
						case WfInsCode::DetachEvent:
							break;
						case WfInsCode::InstallTry:
							break;
						case WfInsCode::UninstallTry:
							break;
						case WfInsCode::RaiseException:
							break;
						case WfInsCode::CompareLiteral:
							break;
						case WfInsCode::CompareReference:
							break;
						case WfInsCode::OpNot:
							break;
						case WfInsCode::OpPositive:
							break;
						case WfInsCode::OpNegative:
							break;
						case WfInsCode::OpConcat:
							break;
						case WfInsCode::OpExp:
							break;
						case WfInsCode::OpAdd:
							break;
						case WfInsCode::OpSub:
							break;
						case WfInsCode::OpMul:
							break;
						case WfInsCode::OpDiv:
							break;
						case WfInsCode::OpShl:
							break;
						case WfInsCode::OpShr:
							break;
						case WfInsCode::OpXor:
							break;
						case WfInsCode::OpAnd:
							break;
						case WfInsCode::OpOr:
							break;
						case WfInsCode::OpLT:
							break;
						case WfInsCode::OpGT:
							break;
						case WfInsCode::OpLE:
							break;
						case WfInsCode::OpGE:
							break;
						case WfInsCode::OpEQ:
							break;
						case WfInsCode::OpNE:
							break;
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