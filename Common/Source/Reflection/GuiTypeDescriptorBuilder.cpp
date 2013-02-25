#include "GuiTypeDescriptorBuilder.h"

namespace vl
{
	using namespace collections;

	namespace reflection
	{
		namespace description
		{

/***********************************************************************
PropertyInfoImpl
***********************************************************************/

			PropertyInfoImpl::PropertyInfoImpl(ITypeDescriptor* _ownerTypeDescriptor, const WString& _name, ITypeDescriptor* _type, IMethodInfo* _getter, IMethodInfo* _setter, IEventInfo* _valueChangedEvent)
				:ownerTypeDescriptor(_ownerTypeDescriptor)
				,name(_name)
				,type(_type)
				,getter(_getter)
				,setter(_setter)
				,valueChangedEvent(_valueChangedEvent)
			{
			}

			PropertyInfoImpl::~PropertyInfoImpl()
			{
			}

			ITypeDescriptor* PropertyInfoImpl::GetOwnerTypeDescriptor()
			{
				return ownerTypeDescriptor;
			}

			const WString& PropertyInfoImpl::GetName()
			{
				return name;
			}

			ITypeDescriptor* PropertyInfoImpl::GetValueTypeDescriptor()
			{
				return type;
			}

			bool PropertyInfoImpl::IsReadable()
			{
				return getter!=0;
			}

			bool PropertyInfoImpl::IsWritable()
			{
				return setter!=0;
			}

			IEventInfo* PropertyInfoImpl::GetValueChangedEvent()
			{
				return valueChangedEvent;
			}

			Value PropertyInfoImpl::GetValue(const Value& thisObject)
			{
				if(getter)
				{
					Array<Value> arguments;
					return getter->Invoke(thisObject, arguments);
				}
				else
				{
					throw PropertyIsNotReadableException(this);
				}
			}

			void PropertyInfoImpl::SetValue(const Value& thisObject, const Value& newValue)
			{
				if(setter)
				{
					Array<Value> arguments(1);
					arguments[0]=newValue;
					setter->Invoke(thisObject, arguments);
				}
				else
				{
					throw PropertyIsNotWritableException(this);
				}
			}

/***********************************************************************
ParameterInfoImpl
***********************************************************************/

			ParameterInfoImpl::ParameterInfoImpl(IMethodInfo* _ownerMethod, const WString& _name, ITypeDescriptor* _type, bool _canOutput)
				:ownerMethod(_ownerMethod)
				,name(_name)
				,type(_type)
				,canOutput(_canOutput)
			{
			}

			ParameterInfoImpl::~ParameterInfoImpl()
			{
			}

			ITypeDescriptor* ParameterInfoImpl::GetOwnerTypeDescriptor()
			{
				return ownerMethod->GetOwnerTypeDescriptor();
			}

			const WString& ParameterInfoImpl::GetName()
			{
				return name;
			}

			ITypeDescriptor* ParameterInfoImpl::GetValueTypeDescriptor()
			{
				return type;
			}

			IMethodInfo* ParameterInfoImpl::GetOwnerMethod()
			{
				return ownerMethod;
			}

			bool ParameterInfoImpl::CanOutput()
			{
				return canOutput;
			}

/***********************************************************************
MethodReturnImpl
***********************************************************************/

			MethodReturnImpl::MethodReturnImpl(ITypeDescriptor* _type)
				:type(_type)
			{
			}

			MethodReturnImpl::~MethodReturnImpl()
			{
			}

			ITypeDescriptor* MethodReturnImpl::GetValueTypeDescriptor()
			{
				return type;
			}

/***********************************************************************
MethodInfoImpl
***********************************************************************/

			MethodInfoImpl::MethodInfoImpl(IMethodGroupInfo* _ownerMethodGroup, ITypeDescriptor* _returnType)
				:ownerMethodGroup(_ownerMethodGroup)
				,returnInfo(_returnType?new MethodReturnImpl(_returnType):0)
			{
			}

			MethodInfoImpl::~MethodInfoImpl()
			{
			}

			ITypeDescriptor* MethodInfoImpl::GetOwnerTypeDescriptor()
			{
				return ownerMethodGroup->GetOwnerTypeDescriptor();
			}

			const WString& MethodInfoImpl::GetName()
			{
				return ownerMethodGroup->GetName();
			}

			IMethodGroupInfo* MethodInfoImpl::GetOwnerMethodGroup()
			{
				return ownerMethodGroup;
			}

			vint MethodInfoImpl::GetParameterCount()
			{
				return parameters.Count();
			}

			IParameterInfo* MethodInfoImpl::GetParameter(vint index)
			{
				if(0<=index && index<parameters.Count())
				{
					return parameters[index].Obj();
				}
				else
				{
					return 0;
				}
			}

			IValueInfo* MethodInfoImpl::GetReturn()
			{
				return returnInfo.Obj();
			}

			bool MethodInfoImpl::AddParameter(Ptr<IParameterInfo> parameter)
			{
				for(vint i=0;i<parameters.Count();i++)
				{
					if(parameters[i]->GetName()==parameter->GetName())
					{
						return false;
					}
				}
				parameters.Add(parameter);
				return true;
			}

/***********************************************************************
MethodGroupInfoImpl
***********************************************************************/

			MethodGroupInfoImpl::MethodGroupInfoImpl(ITypeDescriptor* _ownerTypeDescriptor, const WString& _name)
				:ownerTypeDescriptor(_ownerTypeDescriptor)
				,name(_name)
			{
			}

			MethodGroupInfoImpl::~MethodGroupInfoImpl()
			{
			}

			ITypeDescriptor* MethodGroupInfoImpl::GetOwnerTypeDescriptor()
			{
				return ownerTypeDescriptor;
			}

			const WString& MethodGroupInfoImpl::GetName()
			{
				return name;
			}

			vint MethodGroupInfoImpl::GetMethodCount()
			{
				return methods.Count();
			}

			IMethodInfo* MethodGroupInfoImpl::GetMethod(vint index)
			{
				if(0<=index && index<methods.Count())
				{
					return methods[index].Obj();
				}
				else
				{
					return 0;
				}
			}

			bool MethodGroupInfoImpl::AddMethod(Ptr<IMethodInfo> _method)
			{
				methods.Add(_method);
				return true;
			}

/***********************************************************************
EventInfoImpl::EventHandlerImpl
***********************************************************************/

			EventInfoImpl::EventHandlerImpl::EventHandlerImpl(EventInfoImpl* _ownerEvent, DescriptableObject* ownerObject, const Func<void(const Value&, Value&)>& _handler)
				:ownerEvent(_ownerEvent)
				,handler(_handler)
			{
			}

			EventInfoImpl::EventHandlerImpl::~EventHandlerImpl()
			{
			}

			IEventInfo* EventInfoImpl::EventHandlerImpl::GetOwnerEvent()
			{
				return ownerEvent;
			}

			Value EventInfoImpl::EventHandlerImpl::GetOwnerObject()
			{
				return Value::From(ownerObject);
			}

			bool EventInfoImpl::EventHandlerImpl::IsAttached()
			{
				return attached;
			}

			bool EventInfoImpl::EventHandlerImpl::Detach()
			{
				if(attached)
				{
					attached=false;
					ownerEvent->DetachInternal(ownerObject, this);
					return true;
				}
				else
				{
					return false;
				}
			}

			void EventInfoImpl::EventHandlerImpl::Invoke(const Value& thisObject, Value& arguments)
			{
				handler(thisObject, arguments);
			}

/***********************************************************************
EventInfoImpl
***********************************************************************/

			EventInfoImpl::EventInfoImpl(ITypeDescriptor* _ownerTypeDescriptor, const WString& _name)
				:ownerTypeDescriptor(_ownerTypeDescriptor)
				,name(_name)
			{
			}

			EventInfoImpl::~EventInfoImpl()
			{
			}

			ITypeDescriptor* EventInfoImpl::GetOwnerTypeDescriptor()
			{
				return ownerTypeDescriptor;
			}

			const WString& EventInfoImpl::GetName()
			{
				return name;
			}

			Ptr<IEventHandler> EventInfoImpl::Attach(const Value& thisObject, const Func<void(const Value&, Value&)>& handler)
			{
				DescriptableObject* rawThisObject=thisObject.GetRawPtr();
				if(rawThisObject)
				{
					Ptr<EventHandlerImpl> eventHandler=new EventHandlerImpl(this, rawThisObject, handler);
					AttachInternal(rawThisObject, eventHandler);
					return eventHandler;
				}
				else
				{
					return 0;
				}
			}

/***********************************************************************
TypeDescriptorImpl
***********************************************************************/

			void TypeDescriptorImpl::Load()
			{
				if(!loaded)
				{
					loaded=true;
					LoadInternal();
				}
			}

			TypeDescriptorImpl::TypeDescriptorImpl(const WString& _typeName)
				:typeName(_typeName)
				,loaded(false)
			{
			}

			TypeDescriptorImpl::~TypeDescriptorImpl()
			{
			}

			const WString& TypeDescriptorImpl::GetTypeName()
			{
				Load();
				return typeName;
			}

			IValueSerializer* TypeDescriptorImpl::GetValueSerializer()
			{
				Load();
				return valueSerializer;
			}

			vint TypeDescriptorImpl::GetBaseTypeDescriptorCount()
			{
				Load();
				return baseTypeDescriptors.Count();
			}

			ITypeDescriptor* TypeDescriptorImpl::GetBaseTypeDescriptor(vint index)
			{
				Load();
				if(0<=index && index<baseTypeDescriptors.Count())
				{
					return baseTypeDescriptors[index];
				}
				else
				{
					return 0;
				}
			}

			vint TypeDescriptorImpl::GetPropertyCount()
			{
				Load();
				return properties.Count();
			}

			IPropertyInfo* TypeDescriptorImpl::GetProperty(vint index)
			{
				Load();
				if(0<=index && index<properties.Count())
				{
					return properties.Values().Get(index).Obj();
				}
				else
				{
					return 0;
				}
			}

			bool TypeDescriptorImpl::IsPropertyExists(const WString& name, bool inheritable)
			{
				Load();
				if(properties.Keys().Contains(name))
				{
					return true;
				}
				if(inheritable)
				{
					for(vint i=0;i<baseTypeDescriptors.Count();i++)
					{
						if(baseTypeDescriptors[i]->IsPropertyExists(name, true))
						{
							return true;
						}
					}
				}
				return false;
			}

			IPropertyInfo* TypeDescriptorImpl::GetPropertyByName(const WString& name, bool inheritable)
			{
				Load();
				vint index=properties.Keys().IndexOf(name);
				if(index!=-1)
				{
					return properties.Values().Get(index).Obj();
				}
				if(inheritable)
				{
					for(vint i=0;i<baseTypeDescriptors.Count();i++)
					{
						IPropertyInfo* result=baseTypeDescriptors[i]->GetPropertyByName(name, true);
						if(result)
						{
							return result;
						}
					}
				}
				return 0;
			}

			vint TypeDescriptorImpl::GetEventCount()
			{
				Load();
				return events.Count();
			}

			IEventInfo* TypeDescriptorImpl::GetEvent(vint index)
			{
				Load();
				if(0<=index && index<events.Count())
				{
					return events.Values().Get(index).Obj();
				}
				else
				{
					return 0;
				}
			}

			bool TypeDescriptorImpl::IsEventExists(const WString& name, bool inheritable)
			{
				Load();
				if(events.Keys().Contains(name))
				{
					return true;
				}
				if(inheritable)
				{
					for(vint i=0;i<baseTypeDescriptors.Count();i++)
					{
						if(baseTypeDescriptors[i]->IsEventExists(name, true))
						{
							return true;
						}
					}
				}
				return false;
			}

			IEventInfo* TypeDescriptorImpl::GetEventByName(const WString& name, bool inheritable)
			{
				Load();
				vint index=events.Keys().IndexOf(name);
				if(index!=-1)
				{
					return events.Values().Get(index).Obj();
				}
				if(inheritable)
				{
					for(vint i=0;i<baseTypeDescriptors.Count();i++)
					{
						IEventInfo* result=baseTypeDescriptors[i]->GetEventByName(name, true);
						if(result)
						{
							return result;
						}
					}
				}
				return 0;
			}

			vint TypeDescriptorImpl::GetMethodGroupCount()
			{
				Load();
				return methodGroups.Count();
			}

			IMethodGroupInfo* TypeDescriptorImpl::GetMethodGroup(vint index)
			{
				Load();
				if(0<=index && index<methodGroups.Count())
				{
					return methodGroups.Values().Get(index).Obj();
				}
				else
				{
					return 0;
				}
			}

			bool TypeDescriptorImpl::IsMethodGroupExists(const WString& name, bool inheritable)
			{
				Load();
				if(methodGroups.Keys().Contains(name))
				{
					return true;
				}
				if(inheritable)
				{
					for(vint i=0;i<baseTypeDescriptors.Count();i++)
					{
						if(baseTypeDescriptors[i]->IsMethodGroupExists(name, true))
						{
							return true;
						}
					}
				}
				return false;
			}

			IMethodGroupInfo* TypeDescriptorImpl::GetMethodGroupByName(const WString& name, bool inheritable)
			{
				Load();
				vint index=methodGroups.Keys().IndexOf(name);
				if(index!=-1)
				{
					return methodGroups.Values().Get(index).Obj();
				}
				if(inheritable)
				{
					for(vint i=0;i<baseTypeDescriptors.Count();i++)
					{
						IMethodGroupInfo* result=baseTypeDescriptors[i]->GetMethodGroupByName(name, true);
						if(result)
						{
							return result;
						}
					}
				}
				return 0;
			}

			IMethodGroupInfo* TypeDescriptorImpl::GetConstructorGroup()
			{
				Load();
				return contructorGroup.Obj();
			}
		}
	}
}