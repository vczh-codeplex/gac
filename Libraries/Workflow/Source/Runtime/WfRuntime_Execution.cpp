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
				return WfRuntimeExecutionAction::Nop;
			}
		}
	}
}