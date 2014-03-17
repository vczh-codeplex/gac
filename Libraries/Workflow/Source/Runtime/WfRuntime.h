/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Workflow::Runtime

Interfaces:
**********************************************************************/

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
				// Instruction		//	param				: <Stack-Pattern> -> <Stack-Pattern> in the order of <bottom ---- top>
				LoadValue,			//						: () -> Value
				LoadFunction,		//	function			: () -> Value
				LoadLambda,			//	function, count		: Value-1, ..., Value-count -> Value
				LoadException,		//						: () -> Value
				LoadLocalVar,		//						: int -> Value
				LoadGlobalVar,		//						: int -> Value
				StoreLocalVar,		//	index				: Value -> ()
				StoreGlobalVar,		//	index				: Value -> ()
				StoreResult,		//						: Value -> ()
				CreateArray,		//	count				: Value-1, ..., Value-count -> <array>
				CreateMap,			//	count				: Value-1, ..., Value-count*2 -> <map>
				ConvertToType,		//	flag, type			: Value -> Value
				AssertAsType,		//	flag, type			: Value -> Value
				TestType,			//	flag, type			: Value -> <bool>
				Jump,				//	label				: () -> ()
				JumpIf,				//	label				: () -> ()
				Invoke,				//	function, count		: Value-1, ..., Value-n -> Value
				InvokeExternal,		//	IMethodInfo*, count	: Value-this, Value-1, ..., Value-n -> Value
				InstallTry,			//	label				: () -> ()
				UninstallTry,		//	label				: () -> ()
				RaiseException,		//						: Value -> ()
				CompareLiteral,		//	I48/U48/F48/S		: Value, Value -> <bool>
				CompareReference,	//						: Value, Value -> <int>
				OpNot,				//	B/I1248/U1248		: Value -> Value
				OpPositive,			//	I1248/U1248			: Value -> Value
				OpNegative,			//	I1248				: Value -> Value
				OpConcat,			//						: <string>, <string> -> <string>
				OpExp,				//	I48/U48/F48			: Value, Value -> Value
				OpAdd,				//	I48/U48/F48			: Value, Value -> Value
				OpSub,				//	I48/U48/F48			: Value, Value -> Value
				OpMul,				//	I48/U48/F48			: Value, Value -> Value
				OpDiv,				//	I48/U48/F48			: Value, Value -> Value
				OpShl,				//	I48/U48				: Value, Value -> Value
				OpShr,				//	I48/U48				: Value, Value -> Value
				OpXor,				//						: <bool>, <bool> -> <bool>
				OpAnd,				//						: <bool>, <bool> -> <bool>
				OpOr,				//						: <bool>, <bool> -> <bool>
				OpLT,				//						: <int> -> <bool>
				OpGT,				//						: <int> -> <bool>
				OpLE,				//						: <int> -> <bool>
				OpGE,				//						: <int> -> <bool>
				OpEQ,				//						: <int> -> <bool>
				OpNE,				//						: <int> -> <bool>
			};
		}
	}
}