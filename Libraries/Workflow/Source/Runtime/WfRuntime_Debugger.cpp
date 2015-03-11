#include "WfRuntime.h"
#include <math.h>

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
IWfDebuggerCallback
***********************************************************************/
				
			template<typename TKey>
			bool WfDebugger::HandleBreakPoint(const TKey& key, collections::Dictionary<TKey, vint>& breakPointMap)
			{
				if (evaluatingBreakPoint)
				{
					return false;
				}

				evaluatingBreakPoint = true;
				bool activated = false;
				vint index = breakPointMap.Keys().IndexOf(key);
				if (index != -1)
				{
					index = breakPointMap.Values()[index];
					const auto& breakPoint = breakPoints[index];
					if (breakPoint.available && breakPoint.enabled)
					{
						if (breakPoint.action)
						{
							activated = breakPoint.action->EvaluateCondition(this);
							breakPoint.action->PostAction(this);
						}
						else
						{
							activated = true;
						}
					}
				}
				evaluatingBreakPoint = false;
				return activated;
			}

			void WfDebugger::EnterThreadContext(WfRuntimeThreadContext* context)
			{
				threadContexts.Add(context);
			}

			void WfDebugger::LeaveThreadContext(WfRuntimeThreadContext* context)
			{
				auto oldContext = threadContexts[threadContexts.Count() - 1];
				threadContexts.RemoveAt(threadContexts.Count() - 1);
				CHECK_ERROR(context == oldContext, L"vl::workflow::runtime::WfDebugger::LeaveThreadContext(WfRuntimeThreadContext*)#EnterThreadContext and LeaveThreadContext should be called in pairs.");
			}

			bool WfDebugger::BreakIns(WfAssembly* assembly, vint instruction)
			{
				AssemblyKey key(assembly, instruction);
				return HandleBreakPoint(key, insBreakPoints);
			}

			bool WfDebugger::BreakRead(WfAssembly* assembly, vint variable)
			{
				AssemblyKey key(assembly, variable);
				return HandleBreakPoint(key, getGlobalVarBreakPoints);
			}

			bool WfDebugger::BreakWrite(WfAssembly* assembly, vint variable)
			{
				AssemblyKey key(assembly, variable);
				return HandleBreakPoint(key, setGlobalVarBreakPoints);
			}

			bool WfDebugger::BreakGet(reflection::DescriptableObject* thisObject, reflection::description::IPropertyInfo* propertyInfo)
			{
				PropertyKey key(thisObject, propertyInfo);
				return HandleBreakPoint(key, getPropertyBreakPoints);
			}

			bool WfDebugger::BreakSet(reflection::DescriptableObject* thisObject, reflection::description::IPropertyInfo* propertyInfo)
			{
				PropertyKey key(thisObject, propertyInfo);
				return HandleBreakPoint(key, setPropertyBreakPoints);
			}

			bool WfDebugger::BreakAttach(reflection::DescriptableObject* thisObject, reflection::description::IEventInfo* eventInfo)
			{
				EventKey key(thisObject, eventInfo);
				return HandleBreakPoint(key, attachEventBreakPoints);
			}

			bool WfDebugger::BreakDetach(reflection::DescriptableObject* thisObject, reflection::description::IEventInfo* eventInfo)
			{
				EventKey key(thisObject, eventInfo);
				return HandleBreakPoint(key, detachEventBreakPoints);
			}

			bool WfDebugger::BreakInvoke(reflection::DescriptableObject* thisObject, reflection::description::IMethodInfo* methodInfo)
			{
				MethodKey key(thisObject, methodInfo);
				return HandleBreakPoint(key, invokeMethodBreakPoints);
			}

			bool WfDebugger::BreakCreate(reflection::description::ITypeDescriptor* typeDescriptor)
			{
				return HandleBreakPoint(typeDescriptor, createObjectBreakPoints);
			}

			bool WfDebugger::WaitForContinue()
			{
				throw 0;
			}

/***********************************************************************
WfDebugger
***********************************************************************/

			bool WfDebugger::SetBreakPoint(const WfBreakPoint& breakPoint, bool available, vint index)
			{
				throw 0;
			}

			WfDebugger::WfDebugger()
			{
			}

			WfDebugger::~WfDebugger()
			{
			}

			vint WfDebugger::AddBreakPoint(const WfBreakPoint& breakPoint)
			{
				vint index = breakPoints.Count();
				if (freeBreakPointIndices.Count() > 0)
				{
					index = freeBreakPointIndices[freeBreakPointIndices.Count() - 1];
				}

				if (!SetBreakPoint(breakPoint, true, index))
				{
					return -1;
				}
				
				if (index == breakPoints.Count())
				{
					breakPoints.Add(breakPoint);
				}
				else
				{
					freeBreakPointIndices.RemoveAt(freeBreakPointIndices.Count() - 1);
					breakPoints[index] = breakPoint;
				}

				breakPoints[index].id = index;
				breakPoints[index].available = true;
				breakPoints[index].enabled = true;
				return index;
			}

			vint WfDebugger::GetBreakPointCount()
			{
				return breakPoints.Count();
			}

			const WfBreakPoint& WfDebugger::GetBreakPoint(vint index)
			{
				return breakPoints[index];
			}

			bool WfDebugger::RemoveBreakPoint(vint index)
			{
				if (index < 0 || index >= breakPoints.Count())
				{
					return false;
				}

				auto& breakPoint = breakPoints[index];
				if (!breakPoint.available || !SetBreakPoint(breakPoint, false, -1))
				{
					return false;
				}

				breakPoint.available = false;
				freeBreakPointIndices.Add(index);
				return true;
			}

			bool WfDebugger::EnableBreakPoint(vint index, bool enabled)
			{
				if (0 <= index && index <= breakPoints.Count())
				{
					auto& breakPoint = breakPoints[index];
					if (breakPoint.available)
					{
						breakPoint.enabled = enabled;
						return true;
					}
				}
				return false;
			}

			bool WfDebugger::Run()
			{
				throw 0;
			}

			bool WfDebugger::Stop()
			{
				throw 0;
			}

			bool WfDebugger::StepOver()
			{
				throw 0;
			}

			bool WfDebugger::StepInto()
			{
				throw 0;
			}

			bool WfDebugger::IsRunning()
			{
				return threadContexts.Count() > 0;
			}

			WfRuntimeThreadContext* WfDebugger::GetCurrentThreadContext()
			{
				if (threadContexts.Count() == 0)
				{
					return nullptr;
				}
				return threadContexts[threadContexts.Count() - 1];
			}

/***********************************************************************
Helper Functions
***********************************************************************/

			ThreadVariable<Ptr<WfDebugger>> threadDebugger;

			IWfDebuggerCallback* GetDebuggerCallback()
			{
				return GetDebuggerCallback(threadDebugger.Get().Obj());
			}

			IWfDebuggerCallback* GetDebuggerCallback(WfDebugger* debugger)
			{
				return debugger;
			}

			Ptr<WfDebugger> GetDebuggerForCurrentThread()
			{
				return threadDebugger.Get();
			}

			void SetDebugferForCurrentThread(Ptr<WfDebugger> debugger)
			{
				threadDebugger.Set(debugger);
			}
		}
	}
}