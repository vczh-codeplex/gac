/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Workflow::Runtime

Interfaces:
**********************************************************************/

#ifndef VCZH_WORKFLOW_RUNTIME_WFRUNTIME
#define VCZH_WORKFLOW_RUNTIME_WFRUNTIME

#include "../WorkflowVlppReferences.h"

namespace vl
{
	namespace workflow
	{
		namespace runtime
		{

/***********************************************************************
Instruction
***********************************************************************/

			enum class WfInsCode
			{
				// Instruction		// param				: <Stack-Pattern> -> <Stack-Pattern> in the order of <bottom ---- top>
				Nop,				// 						: () -> ()										;
				LoadValue,			// value				: () -> Value									;
				LoadFunction,		// function				: () -> Value									;
				LoadLambda,			// function, count		: Value-1, ..., Value-count -> Value			;
				LoadException,		// 						: () -> Value									;
				LoadLocalVar,		// variable				: () -> Value									;
				LoadGlobalVar,		// variable				: () -> Value									;
				StoreLocalVar,		// variable				: Value -> ()									;
				StoreGlobalVar,		// variable				: Value -> ()									;
				Pop,				//						: Value -> ()									;
				Return,				// 						: Value -> Value								; (exit function)
				CreateArray,		// count				: Value-1, ..., Value-count -> <array>			;
				CreateMap,			// count				: Value-1, ..., Value-count -> <map>			;
				ConvertToType,		// flag, typeDescriptor	: Value -> Value								;
				AssertAsType,		// flag, typeDescriptor	: Value -> Value								;
				TestType,			// flag, typeDescriptor	: Value -> <bool>								;
				Jump,				// label				: () -> ()										;
				JumpIf,				// label				: () -> ()										;
				Invoke,				// function, count		: Value-1, ..., Value-n -> Value				;
				InvokeMethod,		// IMethodInfo*, count	: Value-this, Value-1, ..., Value-n -> Value	;
				AttachEvent,		// IEventInfo*			: Value-this, <function> -> <Listener>			;
				DetachEvent,		// 						: <Listener> -> bool							;
				InstallTry,			// label				: () -> ()										;
				UninstallTry,		// label				: () -> ()										;
				RaiseException,		// 						: Value -> ()									; (trap)
				CompareLiteral,		// I48/U48/F48/S		: Value, Value -> <bool>						;
				CompareReference,	// 						: Value, Value -> <int>							;
				OpNot,				// B/I1248/U1248		: Value -> Value								;
				OpPositive,			// I1248/U1248			: Value -> Value								;
				OpNegative,			// I1248				: Value -> Value								;
				OpConcat,			// 						: <string>, <string> -> <string>				;
				OpExp,				// I48/U48/F48			: Value, Value -> Value							;
				OpAdd,				// I48/U48/F48			: Value, Value -> Value							;
				OpSub,				// I48/U48/F48			: Value, Value -> Value							;
				OpMul,				// I48/U48/F48			: Value, Value -> Value							;
				OpDiv,				// I48/U48/F48			: Value, Value -> Value							;
				OpShl,				// I48/U48				: Value, Value -> Value							;
				OpShr,				// I48/U48				: Value, Value -> Value							;
				OpXor,				// 						: <bool>, <bool> -> <bool>						;
				OpAnd,				// 						: <bool>, <bool> -> <bool>						;
				OpOr,				// 						: <bool>, <bool> -> <bool>						;
				OpLT,				// 						: <int> -> <bool>								;
				OpGT,				// 						: <int> -> <bool>								;
				OpLE,				// 						: <int> -> <bool>								;
				OpGE,				// 						: <int> -> <bool>								;
				OpEQ,				// 						: <int> -> <bool>								;
				OpNE,				// 						: <int> -> <bool>								;
			};

#define INSTRUCTION_CASES(APPLY, APPLY_VALUE, APPLY_FUNCTION, APPLY_FUNCTION_COUNT, APPLY_VARIABLE, APPLY_COUNT, APPLY_FLAG_TYPEDESCRIPTOR, APPLY_METHOD_COUNT, APPLY_EVENT, APPLY_LABEL, APPLY_TYPE)\
			APPLY(Nop)\
			APPLY_VALUE(LoadValue)\
			APPLY_FUNCTION(LoadFunction)\
			APPLY_FUNCTION_COUNT(LoadLambda)\
			APPLY(LoadException)\
			APPLY_VARIABLE(LoadLocalVar)\
			APPLY_VARIABLE(LoadGlobalVar)\
			APPLY_VARIABLE(StoreLocalVar)\
			APPLY_VARIABLE(StoreGlobalVar)\
			APPLY(Pop)\
			APPLY(Return)\
			APPLY_COUNT(CreateArray)\
			APPLY_COUNT(CreateMap)\
			APPLY_FLAG_TYPEDESCRIPTOR(ConvertToType)\
			APPLY_FLAG_TYPEDESCRIPTOR(AssertAsType)\
			APPLY_FLAG_TYPEDESCRIPTOR(TestType)\
			APPLY_LABEL(Jump)\
			APPLY_LABEL(JumpIf)\
			APPLY_FUNCTION_COUNT(Invoke)\
			APPLY_METHOD_COUNT(InvokeMethod)\
			APPLY_EVENT(AttachEvent)\
			APPLY(DetachEvent)\
			APPLY_LABEL(InstallTry)\
			APPLY_LABEL(UninstallTry)\
			APPLY(RaiseException)\
			APPLY_TYPE(CompareLiteral)\
			APPLY(CompareReference)\
			APPLY_TYPE(OpNot)\
			APPLY_TYPE(OpPositive)\
			APPLY_TYPE(OpNegative)\
			APPLY(OpConcat)\
			APPLY_TYPE(OpExp)\
			APPLY_TYPE(OpAdd)\
			APPLY_TYPE(OpSub)\
			APPLY_TYPE(OpMul)\
			APPLY_TYPE(OpDiv)\
			APPLY_TYPE(OpShl)\
			APPLY_TYPE(OpShr)\
			APPLY(OpXor)\
			APPLY(OpAnd)\
			APPLY(OpOr)\
			APPLY(OpLT)\
			APPLY(OpGT)\
			APPLY(OpLE)\
			APPLY(OpGE)\
			APPLY(OpEQ)\
			APPLY(OpNE)\

			enum class WfInsType
			{
				Bool,
				I1,
				I2,
				I4,
				I8,
				U1,
				U2,
				U4,
				U8,
				F4,
				F8,
				String,
				Unknown,
			};

			struct WfInstruction
			{
				WfInsCode											code;
				reflection::description::Value						valueParameter;
				WfInsType											typeParameter = WfInsType::Unknown;
				reflection::description::Value::ValueType			flagParameter = reflection::description::Value::Null;
				reflection::description::ITypeDescriptor*			typeDescriptorParameter = 0;
				vint												indexParameter = 0;
				vint												countParameter = 0;
				reflection::description::IMethodInfo*				methodParameter = 0;
				reflection::description::IEventInfo*				eventParameter = 0;

				#define CTOR(NAME)						static WfInstruction NAME();
				#define CTOR_VALUE(NAME)				static WfInstruction NAME(const reflection::description::Value& value);
				#define CTOR_FUNCTION(NAME)				static WfInstruction NAME(vint function);
				#define CTOR_FUNCTION_COUNT(NAME)		static WfInstruction NAME(vint function, vint count);
				#define CTOR_VARIABLE(NAME)				static WfInstruction NAME(vint variable);
				#define CTOR_COUNT(NAME)				static WfInstruction NAME(vint count);
				#define CTOR_FLAG_TYPEDESCRIPTOR(NAME)	static WfInstruction NAME(reflection::description::Value::ValueType flag, reflection::description::ITypeDescriptor* typeDescriptor);
				#define CTOR_METHOD_COUNT(NAME)			static WfInstruction NAME(reflection::description::IMethodInfo* methodInfo, vint count);
				#define CTOR_EVENT(NAME)				static WfInstruction NAME(reflection::description::IEventInfo* eventInfo);
				#define CTOR_LABEL(NAME)				static WfInstruction NAME(vint label);
				#define CTOR_TYPE(NAME)					static WfInstruction NAME(WfInsType type);

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
			};

			struct WfAssemblyFunction
			{
				WString												name;
				collections::List<WString>							argumentNames;
				collections::List<WString>							capturedVariableNames;
				collections::List<WString>							localVariableNames;
				vint												firstInstruction = -1;
				vint												lastInstruction = -1;
			};

			struct WfAssembly
			{
				collections::List<WString>							variableNames;
				collections::List<Ptr<WfAssemblyFunction>>			functions;
				collections::List<WfInstruction>					instructions;
			};

/***********************************************************************
Runtime
***********************************************************************/

			struct WfRuntimeVariableContext
			{
				typedef collections::Array<reflection::description::Value>		VariableArray;

				VariableArray					variables;
			};

			struct WfRuntimeGlobalContext
			{
				Ptr<WfAssembly>					assembly;
				Ptr<WfRuntimeVariableContext>	globalVariables;

				WfRuntimeGlobalContext(Ptr<WfAssembly> _assembly);
			};

			struct WfRuntimeStackFrame
			{
				Ptr<WfRuntimeVariableContext>	capturedVariables;
				vint							functionIndex = -1;
				vint							nextInstructionIndex = -1;
				vint							stackBase = 0;
				vint							fixedVariableCount = 0;
				vint							freeStackBase = 0;
			};

			struct WfRuntimeTrapFrame
			{
				vint							stackFrameIndex = -1;
				vint							instructionIndex = -1;
			};

			enum class WfRuntimeExecutionStatus
			{
				Ready,
				Executing,
				RaisedException,
				Finished,
			};

			enum class WfRuntimeExecutionAction
			{
				ExecuteInstruction,
				UnwrapStack,
				EnterStackFrame,
				ExitStackFrame,
				Nop,
			};

			struct WfRuntimeThreadContext
			{
				typedef collections::List<reflection::description::Value>		VariableList;
				typedef collections::List<WfRuntimeStackFrame>					StackFrameList;
				typedef collections::List<WfRuntimeTrapFrame>					TrapFrameList;

				Ptr<WfRuntimeGlobalContext>		globalContext;
				reflection::description::Value	exceptionValue;
				VariableList					stack;
				StackFrameList					stackFrames;
				TrapFrameList					trapFrames;
				WfRuntimeExecutionStatus		status = WfRuntimeExecutionStatus::Finished;

				WfRuntimeThreadContext(Ptr<WfRuntimeGlobalContext> _context);
				WfRuntimeThreadContext(Ptr<WfAssembly> _assembly);

				WfRuntimeStackFrame&			GetCurrentStackFrame();
				void							PushStackFrame(vint functionIndex, vint fixedVariableCount, Ptr<WfRuntimeVariableContext> capturedVariables = 0);
				bool							PopStackFrame();
				void							PushTrapFrame(vint instructionIndex);
				bool							PopTrapFrame();
				void							PushValue(const reflection::description::Value& value);
				bool							PopValue(reflection::description::Value& value);

				WfRuntimeExecutionAction		Execute();
			};
		}
	}
}

#endif