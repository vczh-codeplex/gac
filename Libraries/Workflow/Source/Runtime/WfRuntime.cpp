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

			void WfRuntimeThreadContext::PushStackFrame(vint functionIndex, vint fixedVariableCount, Ptr<WfRuntimeVariableContext> capturedVariables)
			{
				WfRuntimeStackFrame frame;
				frame.capturedVariables = capturedVariables;
				frame.functionIndex = functionIndex;
				frame.nextInstructionIndex = globalContext->assembly->functions[functionIndex]->lastInstruction;
				frame.stackBase = stack.Count();
				frame.fixedVariableCount = fixedVariableCount;
				frame.freeStackBase = frame.stackBase + frame.fixedVariableCount;
				stackFrames.Add(frame);

				for (vint i = 0; i < fixedVariableCount; i++)
				{
					stack.Add(Value());
				}

				if (status == WfRuntimeExecutionStatus::Finished)
				{
					status = WfRuntimeExecutionStatus::Ready;
				}
			}

			bool WfRuntimeThreadContext::PopStackFrame()
			{
				if (stackFrames.Count() == 0) return false;
				WfRuntimeStackFrame frame = GetCurrentStackFrame();
				stackFrames.RemoveAt(stackFrames.Count() - 1);

				if (stack.Count() > frame.stackBase)
				{
					stack.RemoveRange(frame.stackBase, stack.Count() - frame.stackBase);
				}
				return true;
			}

			void WfRuntimeThreadContext::PushTrapFrame(vint instructionIndex)
			{
				WfRuntimeTrapFrame frame;
				frame.stackFrameIndex = stackFrames.Count() - 1;
				frame.instructionIndex = instructionIndex;
				trapFrames.Add(frame);
			}

			bool WfRuntimeThreadContext::PopTrapFrame()
			{
				if (trapFrames.Count() == 0) return false;
				WfRuntimeTrapFrame& frame = trapFrames[trapFrames.Count() - 1];
				if (frame.stackFrameIndex != stackFrames.Count() - 1) return false;
				trapFrames.RemoveAt(trapFrames.Count() - 1);
				return true;
			}

			void WfRuntimeThreadContext::PushValue(const reflection::description::Value& value)
			{
				stack.Add(value);
			}

			bool WfRuntimeThreadContext::PopValue(reflection::description::Value& value)
			{
				if (stackFrames.Count() == 0) return false;
				WfRuntimeStackFrame& frame = GetCurrentStackFrame();
				if (stackFrames.Count() <= frame.freeStackBase) return false;
				value = stack[stack.Count() - 1];
				stack.RemoveAt(stack.Count() - 1);
				return true;
			}
		}
	}
}