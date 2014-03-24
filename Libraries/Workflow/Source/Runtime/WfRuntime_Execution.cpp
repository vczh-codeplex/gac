#include "WfRuntime.h"

namespace vl
{
	namespace workflow
	{
		namespace runtime
		{
			using namespace collections;
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

#define UNARY_OPERATOR(NAME, OPERATOR)\
			template<typename T>\
			WfRuntimeExecutionAction OPERATOR_##NAME(WfRuntimeThreadContext& context)\
			{\
				Value operand;\
				CONTEXT_ACTION(PopValue(operand), L"failed to pop a value from the stack.");\
				T value = OPERATOR UnboxValue<T>(operand);\
				context.PushValue(BoxValue(value));\
				return WfRuntimeExecutionAction::ExecuteInstruction;\
			}\

#define BINARY_OPERATOR(NAME, OPERATOR)\
			template<typename T>\
			WfRuntimeExecutionAction OPERATOR_##NAME(WfRuntimeThreadContext& context)\
			{\
				Value first, second;\
				CONTEXT_ACTION(PopValue(second), L"failed to pop a value from the stack.");\
				CONTEXT_ACTION(PopValue(first), L"failed to pop a value from the stack.");\
				T value = UnboxValue<T>(first) OPERATOR UnboxValue<T>(second);\
				context.PushValue(BoxValue(value));\
				return WfRuntimeExecutionAction::ExecuteInstruction;\
			}\

			//-------------------------------------------------------------------------------

			UNARY_OPERATOR(OpNot, ~)
			UNARY_OPERATOR(OpNot_Bool, !)
			UNARY_OPERATOR(OpPositive, +)
			UNARY_OPERATOR(OpNegative, -)

			BINARY_OPERATOR(OpAdd, +)
			BINARY_OPERATOR(OpSub, -)
			BINARY_OPERATOR(OpMul, *)
			BINARY_OPERATOR(OpDiv, /)
			BINARY_OPERATOR(OpShl, <<)
			BINARY_OPERATOR(OpShr, >>)
			BINARY_OPERATOR(OpAnd, &)
			BINARY_OPERATOR(OpAnd_Bool, &&)
			BINARY_OPERATOR(OpOr, |)
			BINARY_OPERATOR(OpOr_Bool, ||)
			BINARY_OPERATOR(OpXor, ^)
			
			template<typename T>
			WfRuntimeExecutionAction OPERATOR_OpCompare(WfRuntimeThreadContext& context)
			{
				Value first, second;
				CONTEXT_ACTION(PopValue(second), L"failed to pop a value from the stack.");
				CONTEXT_ACTION(PopValue(first), L"failed to pop a value from the stack.");
				T firstValue = UnboxValue<T>(first);
				T secondValue = UnboxValue<T>(second);
				if (firstValue < secondValue)
				{
					context.PushValue(BoxValue((vint)-1));
				}
				else if (firstValue > secondValue)
				{
					context.PushValue(BoxValue((vint)1));
				}
				else
				{
					context.PushValue(BoxValue((vint)0));
				}
				return WfRuntimeExecutionAction::ExecuteInstruction;
			}

			//-------------------------------------------------------------------------------

#undef INTERNAL_ERROR
#undef CONTEXT_ACTION
#undef UNARY_OPERATOR
#undef BINARY_OPERATOR

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
#define TYPE_OF_F4								float
#define TYPE_OF_F8								double
#define TYPE_OF_String							WString
#define EXECUTE(OPERATION, TYPE)				case WfInsType::TYPE: return OPERATOR_##OPERATION<TYPE_OF_##TYPE>(*this);
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
								Value operand;
								CONTEXT_ACTION(LoadLocalVariable(ins.indexParameter, operand), L"illegal local variable index.");
								PushValue(operand);
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
						case WfInsCode::LoadCapturedVar:
							{
								Value operand;
								CONTEXT_ACTION(LoadCapturedVariable(ins.indexParameter, operand), L"illegal captured variable index.");
								PushValue(operand);
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
						case WfInsCode::LoadGlobalVar:
							{
								Value operand;
								CONTEXT_ACTION(LoadGlobalVariable(ins.indexParameter, operand), L"illegal global variable index.");
								PushValue(operand);
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
						case WfInsCode::StoreLocalVar:
							{
								Value operand;
								CONTEXT_ACTION(PopValue(operand), L"failed to pop a value from the stack.");
								CONTEXT_ACTION(StoreLocalVariable(ins.indexParameter, operand), L"illegal local variable index.");
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
						case WfInsCode::StoreGlobalVar:
							{
								Value operand;
								CONTEXT_ACTION(PopValue(operand), L"failed to pop a value from the stack.");
								CONTEXT_ACTION(StoreGlobalVariable(ins.indexParameter, operand), L"illegal global variable index.");
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
						case WfInsCode::Pop:
							{
								Value operand;
								CONTEXT_ACTION(PopValue(operand), L"failed to pop a value from the stack.");
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
						case WfInsCode::Return:
							{
								Value operand;
								CONTEXT_ACTION(PopValue(operand), L"failed to pop the function result.");
								CONTEXT_ACTION(PopStackFrame(), L"failed to pop the the stack frame.");
								PushValue(operand);
								if (stackFrames.Count() == 0)
								{
									status = WfRuntimeExecutionStatus::Finished;
								}
								return WfRuntimeExecutionAction::ExitStackFrame;
							}
						case WfInsCode::CreateArray:
							{
								auto list = IValueList::Create();
								Value operand;
								for (vint i = 0; i < ins.countParameter; i++)
								{
									CONTEXT_ACTION(PopValue(operand), L"failed to pop the function result.");
									list->Add(operand);
								}
								PushValue(Value::From(list));
								return WfRuntimeExecutionAction::ExitStackFrame;
							}
						case WfInsCode::CreateMap:
							{
								auto map = IValueDictionary::Create();
								Value key, value;
								for (vint i = 0; i < ins.countParameter; i+=2)
								{
									CONTEXT_ACTION(PopValue(value), L"failed to pop the function result.");
									CONTEXT_ACTION(PopValue(key), L"failed to pop the function result.");
									map->Set(key, value);
								}
								PushValue(Value::From(map));
								return WfRuntimeExecutionAction::ExitStackFrame;
							}
						case WfInsCode::CreateRange:
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
						case WfInsCode::TryConvertToType:
							throw 0;
						case WfInsCode::TestType:
							throw 0;
						case WfInsCode::GetType:
							{
								Value operand;
								CONTEXT_ACTION(PopValue(operand), L"failed to pop a value from the stack.");
								PushValue(Value::From(operand.GetTypeDescriptor()));
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
						case WfInsCode::Jump:
							throw 0;
						case WfInsCode::JumpIf:
							throw 0;
						case WfInsCode::Invoke:
							throw 0;
						case WfInsCode::GetProperty:
							throw 0;
						case WfInsCode::InvokeMethod:
							{
								Value thisValue;
								CONTEXT_ACTION(PopValue(thisValue), L"failed to pop a value from the stack.");

								Array<Value> arguments(ins.countParameter);
								for (vint i = 0; i < ins.countParameter; i++)
								{
									Value argument;
									CONTEXT_ACTION(PopValue(argument), L"failed to pop a value from the stack.");
									arguments[ins.countParameter - i - 1] = argument;
								}

								Value result = ins.methodParameter->Invoke(thisValue, arguments);
								PushValue(result);
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
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
								Value operand;
								CONTEXT_ACTION(PopValue(operand), L"failed to pop a value from the stack.");
								RaiseException(operand);
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
						case WfInsCode::TestElementInSet:
							throw 0;
						case WfInsCode::CompareLiteral:
							BEGIN_TYPE
								EXECUTE(OpCompare, Bool)
								EXECUTE(OpCompare, I1)
								EXECUTE(OpCompare, I2)
								EXECUTE(OpCompare, I4)
								EXECUTE(OpCompare, I8)
								EXECUTE(OpCompare, U1)
								EXECUTE(OpCompare, U2)
								EXECUTE(OpCompare, U4)
								EXECUTE(OpCompare, U8)
								EXECUTE(OpCompare, F4)
								EXECUTE(OpCompare, F8)
								EXECUTE(OpCompare, String)
							END_TYPE
						case WfInsCode::CompareReference:
							{
								Value first, second;
								CONTEXT_ACTION(PopValue(second), L"failed to pop a value from the stack.");
								CONTEXT_ACTION(PopValue(first), L"failed to pop a value from the stack.");
								bool result = first.GetRawPtr() == second.GetRawPtr();
								PushValue(BoxValue(result));
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
						case WfInsCode::OpNot:
							BEGIN_TYPE
								EXECUTE(OpNot_Bool, Bool)
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
							BEGIN_TYPE
								EXECUTE(OpPositive, I1)
								EXECUTE(OpPositive, I2)
								EXECUTE(OpPositive, I4)
								EXECUTE(OpPositive, I8)
								EXECUTE(OpPositive, U1)
								EXECUTE(OpPositive, U2)
								EXECUTE(OpPositive, U4)
								EXECUTE(OpPositive, U8)
							END_TYPE
						case WfInsCode::OpNegative:
							BEGIN_TYPE
								EXECUTE(OpNegative, I1)
								EXECUTE(OpNegative, I2)
								EXECUTE(OpNegative, I4)
								EXECUTE(OpNegative, I8)
							END_TYPE
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
							BEGIN_TYPE
								EXECUTE(OpAdd, I1)
								EXECUTE(OpAdd, I2)
								EXECUTE(OpAdd, I4)
								EXECUTE(OpAdd, I8)
								EXECUTE(OpAdd, U1)
								EXECUTE(OpAdd, U2)
								EXECUTE(OpAdd, U4)
								EXECUTE(OpAdd, U8)
								EXECUTE(OpAdd, F4)
								EXECUTE(OpAdd, F8)
							END_TYPE
						case WfInsCode::OpSub:
							BEGIN_TYPE
								EXECUTE(OpSub, I1)
								EXECUTE(OpSub, I2)
								EXECUTE(OpSub, I4)
								EXECUTE(OpSub, I8)
								EXECUTE(OpSub, U1)
								EXECUTE(OpSub, U2)
								EXECUTE(OpSub, U4)
								EXECUTE(OpSub, U8)
								EXECUTE(OpSub, F4)
								EXECUTE(OpSub, F8)
							END_TYPE
						case WfInsCode::OpMul:
							BEGIN_TYPE
								EXECUTE(OpMul, I1)
								EXECUTE(OpMul, I2)
								EXECUTE(OpMul, I4)
								EXECUTE(OpMul, I8)
								EXECUTE(OpMul, U1)
								EXECUTE(OpMul, U2)
								EXECUTE(OpMul, U4)
								EXECUTE(OpMul, U8)
								EXECUTE(OpMul, F4)
								EXECUTE(OpMul, F8)
							END_TYPE
						case WfInsCode::OpDiv:
							BEGIN_TYPE
								EXECUTE(OpDiv, I1)
								EXECUTE(OpDiv, I2)
								EXECUTE(OpDiv, I4)
								EXECUTE(OpDiv, I8)
								EXECUTE(OpDiv, U1)
								EXECUTE(OpDiv, U2)
								EXECUTE(OpDiv, U4)
								EXECUTE(OpDiv, U8)
								EXECUTE(OpDiv, F4)
								EXECUTE(OpDiv, F8)
							END_TYPE
						case WfInsCode::OpShl:
							BEGIN_TYPE
								EXECUTE(OpShl, I1)
								EXECUTE(OpShl, I2)
								EXECUTE(OpShl, I4)
								EXECUTE(OpShl, I8)
								EXECUTE(OpShl, U1)
								EXECUTE(OpShl, U2)
								EXECUTE(OpShl, U4)
								EXECUTE(OpShl, U8)
							END_TYPE
						case WfInsCode::OpShr:
							BEGIN_TYPE
								EXECUTE(OpShr, I1)
								EXECUTE(OpShr, I2)
								EXECUTE(OpShr, I4)
								EXECUTE(OpShr, I8)
								EXECUTE(OpShr, U1)
								EXECUTE(OpShr, U2)
								EXECUTE(OpShr, U4)
								EXECUTE(OpShr, U8)
							END_TYPE
						case WfInsCode::OpXor:
							BEGIN_TYPE
								EXECUTE(OpXor, Bool)
								EXECUTE(OpXor, I1)
								EXECUTE(OpXor, I2)
								EXECUTE(OpXor, I4)
								EXECUTE(OpXor, I8)
								EXECUTE(OpXor, U1)
								EXECUTE(OpXor, U2)
								EXECUTE(OpXor, U4)
								EXECUTE(OpXor, U8)
							END_TYPE
						case WfInsCode::OpAnd:
							BEGIN_TYPE
								EXECUTE(OpAnd_Bool, Bool)
								EXECUTE(OpAnd, I1)
								EXECUTE(OpAnd, I2)
								EXECUTE(OpAnd, I4)
								EXECUTE(OpAnd, I8)
								EXECUTE(OpAnd, U1)
								EXECUTE(OpAnd, U2)
								EXECUTE(OpAnd, U4)
								EXECUTE(OpAnd, U8)
							END_TYPE
						case WfInsCode::OpOr:
							BEGIN_TYPE
								EXECUTE(OpOr_Bool, Bool)
								EXECUTE(OpOr, I1)
								EXECUTE(OpOr, I2)
								EXECUTE(OpOr, I4)
								EXECUTE(OpOr, I8)
								EXECUTE(OpOr, U1)
								EXECUTE(OpOr, U2)
								EXECUTE(OpOr, U4)
								EXECUTE(OpOr, U8)
							END_TYPE
						case WfInsCode::OpLT:
							{
								Value operand;
								CONTEXT_ACTION(PopValue(operand), L"failed to pop a value from the stack.");
								vint value = UnboxValue<vint>(operand);
								PushValue(BoxValue(value < 0));
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
							break;
						case WfInsCode::OpGT:
							{
								Value operand;
								CONTEXT_ACTION(PopValue(operand), L"failed to pop a value from the stack.");
								vint value = UnboxValue<vint>(operand);
								PushValue(BoxValue(value > 0));
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
							break;
						case WfInsCode::OpLE:
							{
								Value operand;
								CONTEXT_ACTION(PopValue(operand), L"failed to pop a value from the stack.");
								vint value = UnboxValue<vint>(operand);
								PushValue(BoxValue(value <= 0));
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
							break;
						case WfInsCode::OpGE:
							{
								Value operand;
								CONTEXT_ACTION(PopValue(operand), L"failed to pop a value from the stack.");
								vint value = UnboxValue<vint>(operand);
								PushValue(BoxValue(value >= 0));
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
							break;
						case WfInsCode::OpEQ:
							{
								Value operand;
								CONTEXT_ACTION(PopValue(operand), L"failed to pop a value from the stack.");
								vint value = UnboxValue<vint>(operand);
								PushValue(BoxValue(value == 0));
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
							break;
						case WfInsCode::OpNE:
							{
								Value operand;
								CONTEXT_ACTION(PopValue(operand), L"failed to pop a value from the stack.");
								vint value = UnboxValue<vint>(operand);
								PushValue(BoxValue(value != 0));
								return WfRuntimeExecutionAction::ExecuteInstruction;
							}
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
#undef TYPE_OF_Bool
#undef TYPE_OF_I1
#undef TYPE_OF_I2
#undef TYPE_OF_I4
#undef TYPE_OF_I8
#undef TYPE_OF_U1
#undef TYPE_OF_U2
#undef TYPE_OF_U4
#undef TYPE_OF_U8
#undef TYPE_OF_F4
#undef TYPE_OF_F8
#undef TYPE_OF_String
#undef EXECUTE
#undef BEGIN_TYPE
#undef END_TYPE
			}
		}
	}
}