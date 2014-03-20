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
WfRuntimeThreadContext (Operations)
***********************************************************************/

#define INTERNAL_ERROR(MESSAGE)\
				do{\
					context.RaiseException(BoxValue<WString>(L"Internal error: " MESSAGE));\
					return WfRuntimeExecutionAction::Nop; \
				} while (0)\

#define CONTEXT_ACTION(ACTION, MESSAGE)\
				do{\
					if ((context.ACTION) != WfRuntimeThreadContextError::Success)\
					{\
						INTERNAL_ERROR(MESSAGE);\
					}\
				} while (0)\

			//-------------------------------------------------------------------------------

			template<typename T>
			struct OpNot
			{
				static WfRuntimeExecutionAction Do(WfRuntimeThreadContext& context)
				{
					Value result;
					CONTEXT_ACTION(PopValue(result), L"failed to pop a value from the stack.");
					T value = ~UnboxValue<T>(result);
					context.PushValue(BoxValue(value));
					return WfRuntimeExecutionAction::ExecuteInstruction;
				}
			};

			template<>
			struct OpNot<bool>
			{
				static WfRuntimeExecutionAction Do(WfRuntimeThreadContext& context)
				{
					Value result;
					CONTEXT_ACTION(PopValue(result), L"failed to pop a value from the stack.");
					bool value = !UnboxValue<bool>(result);
					context.PushValue(BoxValue(value));
					return WfRuntimeExecutionAction::ExecuteInstruction;
				}
			};

			//-------------------------------------------------------------------------------

#undef INTERNAL_ERROR
#undef CONTEXT_ACTION

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
				do{\
					if ((ACTION) != WfRuntimeThreadContextError::Success)\
					{\
						INTERNAL_ERROR(MESSAGE);\
					}\
				} while (0)\

#define TYPE_OF_Bool							bool
#define TYPE_OF_I1								vint8_t
#define TYPE_OF_I2								vint16_t
#define TYPE_OF_I4								vint32_t
#define TYPE_OF_I8								vint64_t
#define TYPE_OF_U1								vuint8_t
#define TYPE_OF_U2								vuint16_t
#define TYPE_OF_U4								vuint32_t
#define TYPE_OF_U8								vuint64_t
#define TYPE_OF_String							WString
#define EXECUTE(OPERATION, TYPE)				case WfInsType::TYPE: return OPERATION<TYPE_OF_##TYPE>::Do(*this);
#define BEGIN_TYPE								switch(ins.typeParameter) {
#define END_TYPE								default: INTERNAL_ERROR(L"unexpected type argument."); }

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
						case WfInsCode::LoadCapturedVar:
							{
								Value result;
								CONTEXT_ACTION(LoadCapturedVariable(ins.indexParameter, result), L"illegal captured variable index.");
								PushValue(result);
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
						case WfInsCode::LoadGlobalVar:
							{
								Value result;
								CONTEXT_ACTION(LoadGlobalVariable(ins.indexParameter, result), L"illegal global variable index.");
								PushValue(result);
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
						case WfInsCode::StoreLocalVar:
							{
								Value result;
								CONTEXT_ACTION(PopValue(result), L"failed to pop a value from the stack.");
								CONTEXT_ACTION(StoreLocalVariable(ins.indexParameter, result), L"illegal local variable index.");
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
						case WfInsCode::StoreGlobalVar:
							{
								Value result;
								CONTEXT_ACTION(PopValue(result), L"failed to pop a value from the stack.");
								CONTEXT_ACTION(StoreGlobalVariable(ins.indexParameter, result), L"illegal global variable index.");
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
						case WfInsCode::Pop:
							{
								Value result;
								CONTEXT_ACTION(PopValue(result), L"failed to pop a value from the stack.");
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
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
						case WfInsCode::CreateArray:
							throw 0;
						case WfInsCode::CreateMap:
							throw 0;
						case WfInsCode::ConvertToType:
							{
								Value result, converted;
								CONTEXT_ACTION(PopValue(result), L"failed to pop a value from the stack.");

								switch (ins.flagParameter)
								{
								case Value::Null:
									INTERNAL_ERROR(L"failed to do type conversion.");
								case Value::RawPtr:
									if (result.GetValueType() == Value::Text)
									{
										INTERNAL_ERROR(L"failed to do type conversion.");
									}
									else if (result.GetRawPtr())
									{
										if (result.GetTypeDescriptor()->CanConvertTo(ins.typeDescriptorParameter))
										{
											converted = Value::From(result.GetRawPtr());
										}
										else
										{
											INTERNAL_ERROR(L"failed to do type conversion.");
										}
									}
									break;
								case Value::SharedPtr:
									if (result.GetValueType() == Value::Text)
									{
										INTERNAL_ERROR(L"failed to do type conversion.");
									}
									else if (result.GetRawPtr())
									{
										if (result.GetTypeDescriptor()->CanConvertTo(ins.typeDescriptorParameter))
										{
											converted = Value::From(Ptr<DescriptableObject>(result.GetRawPtr()));
										}
										else
										{
											INTERNAL_ERROR(L"failed to do type conversion.");
										}
									}
									break;
								case Value::Text:
									if (result.GetValueType() != Value::Text)
									{
										INTERNAL_ERROR(L"failed to do type conversion.");
									}
									else if (ins.typeDescriptorParameter == GetTypeDescriptor<void>())
									{
										if (result.GetText() != L"")
										{
											INTERNAL_ERROR(L"failed to do type conversion.");
										}
									}
									else
									{
										auto serializer = ins.typeDescriptorParameter->GetValueSerializer();
										if (!serializer)
										{
											INTERNAL_ERROR(L"failed to do type conversion.");
										}
										if (!serializer->Parse(result.GetText(), converted))
										{
											INTERNAL_ERROR(L"failed to do type conversion.");
										}
									}
									break;
								}

								PushValue(converted);
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
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
							{
								Value result;
								CONTEXT_ACTION(PopValue(result), L"failed to pop a value from the stack.");
								RaiseException(result);
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
						case WfInsCode::CompareLiteral:
							throw 0;
						case WfInsCode::CompareReference:
							throw 0;
						case WfInsCode::OpNot:
							BEGIN_TYPE
								EXECUTE(OpNot, Bool)
								EXECUTE(OpNot, I1)
								EXECUTE(OpNot, I2)
								EXECUTE(OpNot, I4)
								EXECUTE(OpNot, I8)
								EXECUTE(OpNot, U1)
								EXECUTE(OpNot, U2)
								EXECUTE(OpNot, U4)
								EXECUTE(OpNot, U8)
							END_TYPE
						case WfInsCode::OpPositive:
							throw 0;
						case WfInsCode::OpNegative:
							throw 0;
						case WfInsCode::OpConcat:
							{
								Value first, second;
								CONTEXT_ACTION(PopValue(second), L"failed to pop a value from the stack.");
								CONTEXT_ACTION(PopValue(first), L"failed to pop a value from the stack.");
								PushValue(BoxValue(first.GetText() + second.GetText()));
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
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
#undef TYPE_OF_Bool
#undef TYPE_OF_I1
#undef TYPE_OF_I2
#undef TYPE_OF_I4
#undef TYPE_OF_I8
#undef TYPE_OF_U1
#undef TYPE_OF_U2
#undef TYPE_OF_U4
#undef TYPE_OF_U8
#undef TYPE_OF_String
#undef EXECUTE
#undef BEGIN_TYPE
#undef END_TYPE
			}
		}
	}
}