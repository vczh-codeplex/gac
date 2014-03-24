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
WfInstruction
***********************************************************************/

			WfInstruction::WfInstruction()
				:flagParameter(Value::Null)
				, typeDescriptorParameter(0)
			{

			}

			#define CTOR(NAME)\
				WfInstruction WfInstruction::NAME()\
				{\
					WfInstruction ins;\
					ins.code = WfInsCode::NAME;\
					return ins;\
				}\

			#define CTOR_VALUE(NAME)\
				WfInstruction WfInstruction::NAME(const reflection::description::Value& value)\
				{\
					WfInstruction ins;\
					ins.code = WfInsCode::NAME;\
					ins.valueParameter = value;\
					return ins;\
				}\

			#define CTOR_FUNCTION(NAME)\
				WfInstruction WfInstruction::NAME(vint function)\
				{\
					WfInstruction ins;\
					ins.code = WfInsCode::NAME;\
					ins.indexParameter = function;\
					return ins;\
				}\

			#define CTOR_FUNCTION_COUNT(NAME)\
				WfInstruction WfInstruction::NAME(vint function, vint count)\
				{\
					WfInstruction ins;\
					ins.code = WfInsCode::NAME;\
					ins.indexParameter = function;\
					ins.countParameter = count;\
					return ins;\
				}\

			#define CTOR_VARIABLE(NAME)\
				WfInstruction WfInstruction::NAME(vint variable)\
				{\
					WfInstruction ins;\
					ins.code = WfInsCode::NAME;\
					ins.indexParameter = variable;\
					return ins;\
				}\

			#define CTOR_COUNT(NAME)\
				WfInstruction WfInstruction::NAME(vint count)\
				{\
					WfInstruction ins;\
					ins.code = WfInsCode::NAME;\
					ins.countParameter = count;\
					return ins;\
				}\

			#define CTOR_FLAG_TYPEDESCRIPTOR(NAME)\
				WfInstruction WfInstruction::NAME(reflection::description::Value::ValueType flag, reflection::description::ITypeDescriptor* typeDescriptor)\
				{\
					WfInstruction ins;\
					ins.code = WfInsCode::NAME;\
					ins.flagParameter = flag;\
					ins.typeDescriptorParameter = typeDescriptor;\
					return ins;\
				}\

			#define CTOR_PROPERTY(NAME)\
				WfInstruction WfInstruction::NAME(reflection::description::IPropertyInfo* propertyInfo)\
				{\
					WfInstruction ins;\
					ins.code = WfInsCode::NAME;\
					ins.propertyParameter = propertyInfo;\
					return ins;\
				}\

			#define CTOR_METHOD_COUNT(NAME)\
				WfInstruction WfInstruction::NAME(reflection::description::IMethodInfo* methodInfo, vint count)\
				{\
					WfInstruction ins;\
					ins.code = WfInsCode::NAME;\
					ins.methodParameter = methodInfo;\
					ins.countParameter = count;\
					return ins;\
				}\

			#define CTOR_EVENT(NAME)\
				WfInstruction WfInstruction::NAME(reflection::description::IEventInfo* eventInfo)\
				{\
					WfInstruction ins;\
					ins.code = WfInsCode::NAME;\
					ins.eventParameter = eventInfo;\
					return ins;\
				}\

			#define CTOR_LABEL(NAME)\
				WfInstruction WfInstruction::NAME(vint label)\
				{\
					WfInstruction ins;\
					ins.code = WfInsCode::NAME;\
					ins.indexParameter = label;\
					return ins;\
				}\

			#define CTOR_TYPE(NAME)\
				WfInstruction WfInstruction::NAME(WfInsType type)\
				{\
					WfInstruction ins;\
					ins.code = WfInsCode::NAME;\
					ins.typeParameter = type;\
					return ins;\
				}\

			INSTRUCTION_CASES(
				CTOR,
				CTOR_VALUE,
				CTOR_FUNCTION,
				CTOR_FUNCTION_COUNT,
				CTOR_VARIABLE,
				CTOR_COUNT,
				CTOR_FLAG_TYPEDESCRIPTOR,
				CTOR_PROPERTY,
				CTOR_METHOD_COUNT,
				CTOR_EVENT,
				CTOR_LABEL,
				CTOR_TYPE)

			#undef CTOR
			#undef CTOR_VALUE
			#undef CTOR_FUNCTION
			#undef CTOR_FUNCTION_COUNT
			#undef CTOR_VARIABLE
			#undef CTOR_COUNT
			#undef CTOR_FLAG_TYPEDESCRIPTOR
			#undef CTOR_PROPERTY
			#undef CTOR_METHOD_COUNT
			#undef CTOR_EVENT
			#undef CTOR_LABEL
			#undef CTOR_TYPE

/***********************************************************************
WfRuntimeGlobalContext
***********************************************************************/

			WfRuntimeGlobalContext::WfRuntimeGlobalContext(Ptr<WfAssembly> _assembly)
				:assembly(_assembly)
			{
				globalVariables = new WfRuntimeVariableContext;
				globalVariables->variables.Resize(assembly->variableNames.Count());
			}

/***********************************************************************
WfRuntimeThreadContext
***********************************************************************/

			WfRuntimeThreadContext::WfRuntimeThreadContext(Ptr<WfRuntimeGlobalContext> _context)
				:globalContext(_context)
			{
				stack.SetLessMemoryMode(false);
				stackFrames.SetLessMemoryMode(false);
			}

			WfRuntimeThreadContext::WfRuntimeThreadContext(Ptr<WfAssembly> _assembly)
				:globalContext(new WfRuntimeGlobalContext(_assembly))
			{
				stack.SetLessMemoryMode(false);
				stackFrames.SetLessMemoryMode(false);
			}

			WfRuntimeStackFrame& WfRuntimeThreadContext::GetCurrentStackFrame()
			{
				return stackFrames[stackFrames.Count() - 1];
			}

			WfRuntimeThreadContextError WfRuntimeThreadContext::PushStackFrame(vint functionIndex, vint argumentCount, Ptr<WfRuntimeVariableContext> capturedVariables)
			{
				if (stackFrames.Count() == 0)
				{
					if (stack.Count() != 0)
					{
						return WfRuntimeThreadContextError::StackCorrupted;
					}
				}
				else
				{
					auto& frame = GetCurrentStackFrame();
					if (stack.Count() - frame.freeStackBase < argumentCount)
					{
						return WfRuntimeThreadContextError::StackCorrupted;
					}
				}
				if (functionIndex < 0 || functionIndex >= globalContext->assembly->functions.Count())
				{
					return WfRuntimeThreadContextError::WrongFunctionIndex;
				}
				auto meta = globalContext->assembly->functions[functionIndex];
				if (meta->argumentNames.Count() != argumentCount)
				{
					return WfRuntimeThreadContextError::WrongArgumentCount;
				}
				if (meta->capturedVariableNames.Count() == 0)
				{
					if (capturedVariables)
					{
						return WfRuntimeThreadContextError::WrongCapturedVariableCount;
					}
				}
				else
				{
					if (!capturedVariables || capturedVariables->variables.Count() != meta->capturedVariableNames.Count())
					{
						return WfRuntimeThreadContextError::WrongCapturedVariableCount;
					}
				}

				WfRuntimeStackFrame frame;
				frame.capturedVariables = capturedVariables;
				frame.functionIndex = functionIndex;
				frame.nextInstructionIndex = globalContext->assembly->functions[functionIndex]->firstInstruction;
				frame.stackBase = stack.Count() - argumentCount;

				frame.fixedVariableCount = meta->argumentNames.Count() + meta->localVariableNames.Count();
				frame.freeStackBase = frame.stackBase + frame.fixedVariableCount;
				stackFrames.Add(frame);

				for (vint i = 0; i < meta->localVariableNames.Count(); i++)
				{
					stack.Add(Value());
				}
				if (status == WfRuntimeExecutionStatus::Finished)
				{
					status = WfRuntimeExecutionStatus::Ready;
				}
				return WfRuntimeThreadContextError::Success;
			}

			WfRuntimeThreadContextError WfRuntimeThreadContext::PopStackFrame()
			{
				if (stackFrames.Count() == 0) return WfRuntimeThreadContextError::EmptyStackFrame;
				WfRuntimeStackFrame frame = GetCurrentStackFrame();
				stackFrames.RemoveAt(stackFrames.Count() - 1);

				if (stack.Count() > frame.stackBase)
				{
					stack.RemoveRange(frame.stackBase, stack.Count() - frame.stackBase);
				}
				return WfRuntimeThreadContextError::Success;
			}

			WfRuntimeThreadContextError WfRuntimeThreadContext::PushTrapFrame(vint instructionIndex)
			{
				WfRuntimeTrapFrame frame;
				frame.stackFrameIndex = stackFrames.Count() - 1;
				frame.instructionIndex = instructionIndex;
				trapFrames.Add(frame);
				return WfRuntimeThreadContextError::Success;
			}

			WfRuntimeThreadContextError WfRuntimeThreadContext::PopTrapFrame()
			{
				if (trapFrames.Count() == 0) return WfRuntimeThreadContextError::EmptyTrapFrame;
				WfRuntimeTrapFrame& frame = trapFrames[trapFrames.Count() - 1];
				if (frame.stackFrameIndex != stackFrames.Count() - 1) return WfRuntimeThreadContextError::TrapFrameCorrupted;
				trapFrames.RemoveAt(trapFrames.Count() - 1);
				return WfRuntimeThreadContextError::Success;
			}

			WfRuntimeThreadContextError WfRuntimeThreadContext::PushValue(const reflection::description::Value& value)
			{
				stack.Add(value);
				return WfRuntimeThreadContextError::Success;
			}

			WfRuntimeThreadContextError WfRuntimeThreadContext::PopValue(reflection::description::Value& value)
			{
				if (stackFrames.Count() == 0)
				{
					if (stack.Count() == 0) return WfRuntimeThreadContextError::EmptyStack;
				}
				else
				{
					WfRuntimeStackFrame& frame = GetCurrentStackFrame();
					if (stack.Count() <= frame.freeStackBase) return WfRuntimeThreadContextError::StackCorrupted;
				}
				value = stack[stack.Count() - 1];
				stack.RemoveAt(stack.Count() - 1);
				return WfRuntimeThreadContextError::Success;
			}

			WfRuntimeThreadContextError WfRuntimeThreadContext::RaiseException(const reflection::description::Value& exception)
			{
				exceptionValue = exception;
				return WfRuntimeThreadContextError::Success;
			}

			WfRuntimeThreadContextError WfRuntimeThreadContext::LoadStackValue(vint stackItemIndex, reflection::description::Value& value)
			{
				if (stackFrames.Count() == 0) return WfRuntimeThreadContextError::EmptyStackFrame;
				auto frame = GetCurrentStackFrame();
				if (stackItemIndex < frame.freeStackBase || stackItemIndex >= stack.Count())
				{
					return WfRuntimeThreadContextError::WrongVariableIndex;
				}

				value = stack[stackItemIndex];
				return WfRuntimeThreadContextError::Success;
			}

			WfRuntimeThreadContextError WfRuntimeThreadContext::LoadGlobalVariable(vint variableIndex, reflection::description::Value& value)
			{
				if (variableIndex < 0 || variableIndex >= globalContext->globalVariables->variables.Count())
				{
					return WfRuntimeThreadContextError::WrongVariableIndex;
				}

				value = globalContext->globalVariables->variables[variableIndex];
				return WfRuntimeThreadContextError::Success;
			}

			WfRuntimeThreadContextError WfRuntimeThreadContext::StoreGlobalVariable(vint variableIndex, const reflection::description::Value& value)
			{
				if (variableIndex < 0 || variableIndex >= globalContext->globalVariables->variables.Count())
				{
					return WfRuntimeThreadContextError::WrongVariableIndex;
				}

				globalContext->globalVariables->variables[variableIndex] = value;
				return WfRuntimeThreadContextError::Success;
			}

			WfRuntimeThreadContextError WfRuntimeThreadContext::LoadCapturedVariable(vint variableIndex, reflection::description::Value& value)
			{
				if (stackFrames.Count() == 0) return WfRuntimeThreadContextError::EmptyStackFrame;
				auto frame = GetCurrentStackFrame();
				if (variableIndex < 0 || variableIndex >= frame.capturedVariables->variables.Count())
				{
					return WfRuntimeThreadContextError::WrongVariableIndex;
				}

				value = frame.capturedVariables->variables[variableIndex];
				return WfRuntimeThreadContextError::Success;
			}

			WfRuntimeThreadContextError WfRuntimeThreadContext::LoadLocalVariable(vint variableIndex, reflection::description::Value& value)
			{
				if (stackFrames.Count() == 0) return WfRuntimeThreadContextError::EmptyStackFrame;
				auto frame = GetCurrentStackFrame();
				if (variableIndex < 0 || variableIndex >= frame.fixedVariableCount)
				{
					return WfRuntimeThreadContextError::WrongVariableIndex;
				}

				value = stack[frame.stackBase + variableIndex];
				return WfRuntimeThreadContextError::Success;
			}

			WfRuntimeThreadContextError WfRuntimeThreadContext::StoreLocalVariable(vint variableIndex, const reflection::description::Value& value)
			{
				if (stackFrames.Count() == 0) return WfRuntimeThreadContextError::EmptyStackFrame;
				auto frame = GetCurrentStackFrame();
				if (variableIndex < 0 || variableIndex >= frame.fixedVariableCount)
				{
					return WfRuntimeThreadContextError::WrongVariableIndex;
				}

				stack[frame.stackBase + variableIndex] = value;
				return WfRuntimeThreadContextError::Success;
			}

			void WfRuntimeThreadContext::ExecuteToEnd()
			{
				while (Execute() != WfRuntimeExecutionAction::Nop);
			}
		}
	}
}