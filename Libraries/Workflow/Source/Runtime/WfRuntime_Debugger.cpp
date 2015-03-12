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

			WfBreakPoint WfBreakPoint::Ins(WfAssembly* assembly, vint instruction)
			{
				WfBreakPoint breakPoint;
				breakPoint.type = Instruction;
				breakPoint.assembly = assembly;
				breakPoint.instruction = instruction;
				return breakPoint;
			}

			WfBreakPoint WfBreakPoint::Read(WfAssembly* assembly, vint variable)
			{
				WfBreakPoint breakPoint;
				breakPoint.type = ReadGlobalVar;
				breakPoint.assembly = assembly;
				breakPoint.variable = variable;
				return breakPoint;
			}

			WfBreakPoint WfBreakPoint::Write(WfAssembly* assembly, vint variable)
			{
				WfBreakPoint breakPoint;
				breakPoint.type = WriteGlobalVar;
				breakPoint.assembly = assembly;
				breakPoint.variable = variable;
				return breakPoint;
			}

			WfBreakPoint WfBreakPoint::Get(reflection::DescriptableObject* thisObject, reflection::description::IPropertyInfo* propertyInfo)
			{
				WfBreakPoint breakPoint;
				breakPoint.type = GetProperty;
				breakPoint.thisObject = thisObject;
				breakPoint.propertyInfo = propertyInfo;
				return breakPoint;
			}

			WfBreakPoint WfBreakPoint::Set(reflection::DescriptableObject* thisObject, reflection::description::IPropertyInfo* propertyInfo)
			{
				WfBreakPoint breakPoint;
				breakPoint.type = SetProperty;
				breakPoint.thisObject = thisObject;
				breakPoint.propertyInfo = propertyInfo;
				return breakPoint;
			}

			WfBreakPoint WfBreakPoint::Attach(reflection::DescriptableObject* thisObject, reflection::description::IEventInfo* eventInfo)
			{
				WfBreakPoint breakPoint;
				breakPoint.type = AttachEvent;
				breakPoint.thisObject = thisObject;
				breakPoint.eventInfo = eventInfo;
				return breakPoint;
			}

			WfBreakPoint WfBreakPoint::Detach(reflection::DescriptableObject* thisObject, reflection::description::IEventInfo* eventInfo)
			{
				WfBreakPoint breakPoint;
				breakPoint.type = DetachEvent;
				breakPoint.thisObject = thisObject;
				breakPoint.eventInfo = eventInfo;
				return breakPoint;
			}

			WfBreakPoint WfBreakPoint::Invoke(reflection::DescriptableObject* thisObject, reflection::description::IMethodInfo* methodInfo)
			{
				WfBreakPoint breakPoint;
				breakPoint.type = InvokeMethod;
				breakPoint.thisObject = thisObject;
				breakPoint.methodInfo = methodInfo;
				return breakPoint;
			}

			WfBreakPoint WfBreakPoint::Create(reflection::description::ITypeDescriptor* typeDescriptor)
			{
				WfBreakPoint breakPoint;
				breakPoint.type = CreateObject;
				breakPoint.typeDescriptor = typeDescriptor;
				return breakPoint;
			}

/***********************************************************************
IWfDebuggerCallback
***********************************************************************/

			void WfDebugger::BlockExecution()
			{
			}
				
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

#define TEST(AVAILABLE, KEY, MAP) if (AVAILABLE && available == MAP.Keys().Contains(KEY)) return false;
#define SET(AVAILABLE, KEY, MAP) if (AVAILABLE) if (available) MAP.Add(KEY, index); else MAP.Remove(KEY);

			bool WfDebugger::SetBreakPoint(const WfBreakPoint& breakPoint, bool available, vint index)
			{
				switch (breakPoint.type)
				{
				case WfBreakPoint::Instruction:
					{
						AssemblyKey key(breakPoint.assembly, breakPoint.instruction);
						TEST(true, key, insBreakPoints);
						SET(true, key, insBreakPoints);
					}
					break;
				case WfBreakPoint::ReadGlobalVar:
					{
						AssemblyKey key(breakPoint.assembly, breakPoint.variable);
						TEST(true, key, getGlobalVarBreakPoints);
						SET(true, key, getGlobalVarBreakPoints);
					}
					break;
				case WfBreakPoint::WriteGlobalVar:
					{
						AssemblyKey key(breakPoint.assembly, breakPoint.instruction);
						TEST(true, key, setGlobalVarBreakPoints);
						SET(true, key, setGlobalVarBreakPoints);
					}
					break;
				case WfBreakPoint::GetProperty:
					{
						PropertyKey key1(breakPoint.thisObject, breakPoint.propertyInfo);
						MethodKey key2(breakPoint.thisObject, breakPoint.propertyInfo->GetGetter());
						TEST(true, key1, getPropertyBreakPoints);
						TEST(key2.f1, key2, invokeMethodBreakPoints);
						SET(true, key1, getPropertyBreakPoints);
						SET(key2.f1, key2, invokeMethodBreakPoints);
					}
					break;
				case WfBreakPoint::SetProperty:
					{
						PropertyKey key1(breakPoint.thisObject, breakPoint.propertyInfo);
						MethodKey key2(breakPoint.thisObject, breakPoint.propertyInfo->GetSetter());
						TEST(true, key1, setPropertyBreakPoints);
						TEST(key2.f1, key2, invokeMethodBreakPoints);
						SET(true, key1, setPropertyBreakPoints);
						SET(key2.f1, key2, invokeMethodBreakPoints);
					}
					break;
				case WfBreakPoint::AttachEvent:
					{
						EventKey key(breakPoint.thisObject, breakPoint.eventInfo);
						TEST(true, key, attachEventBreakPoints);
						SET(true, key, attachEventBreakPoints);
					}
					break;
				case WfBreakPoint::DetachEvent:
					{
						EventKey key(breakPoint.thisObject, breakPoint.eventInfo);
						TEST(true, key, detachEventBreakPoints);
						SET(true, key, detachEventBreakPoints);
					}
					break;
				case WfBreakPoint::InvokeMethod:
					{
						// get property, set property and new object are all compiled to invoke method
						// so here it is not noecessary to generate other keys
						MethodKey key(breakPoint.thisObject, breakPoint.methodInfo);
						TEST(true, key, invokeMethodBreakPoints);
						SET(true, key, invokeMethodBreakPoints);
					}
					break;
				case WfBreakPoint::CreateObject:
					{
						auto group = breakPoint.typeDescriptor->GetConstructorGroup();
						vint count = group ? group->GetMethodCount() : 0;

						TEST(true, breakPoint.typeDescriptor, createObjectBreakPoints);
						for (vint i = 0; i < count; i++)
						{
							MethodKey key(nullptr, group->GetMethod(i));
							TEST(true, key, invokeMethodBreakPoints);
						}
						SET(true, breakPoint.typeDescriptor, createObjectBreakPoints);
						for (vint i = 0; i < count; i++)
						{
							MethodKey key(nullptr, group->GetMethod(i));
							SET(true, key, invokeMethodBreakPoints);
						}
					}
					break;
				default:
					return false;
				}
				return true;
			}

#undef TEST
#undef SET

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
				return GetDebuggerCallback(GetDebuggerForCurrentThread().Obj());
			}

			IWfDebuggerCallback* GetDebuggerCallback(WfDebugger* debugger)
			{
				return debugger;
			}

			Ptr<WfDebugger> GetDebuggerForCurrentThread()
			{
				return threadDebugger.HasData() ? threadDebugger.Get() : nullptr;
			}

			void SetDebugferForCurrentThread(Ptr<WfDebugger> debugger)
			{
				threadDebugger.Set(debugger);
			}
		}
	}
}