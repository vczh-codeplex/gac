#include "GuiTypeDescriptorBuilder.h"

namespace vl
{
	using namespace collections;

	namespace reflection
	{
		namespace description
		{

/***********************************************************************
ParameterInfoImpl
***********************************************************************/

			ParameterInfoImpl::ParameterInfoImpl(IMethodInfo* _ownerMethod, const WString& _name, ITypeDescriptor* _type, Decorator _decorator, bool _canOutput)
				:ownerMethod(_ownerMethod)
				,name(_name)
				,type(_type)
				,decorator(_decorator)
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

			IParameterInfo::Decorator ParameterInfoImpl::GetDecorator()
			{
				return decorator;
			}

			bool ParameterInfoImpl::CanOutput()
			{
				return canOutput;
			}

			WString ParameterInfoImpl::GetTypeFriendlyName()
			{
				switch(decorator)
				{
				case RawPtr:
					return type->GetTypeName()+L"*"+(canOutput?L"&":L"");
				case SharedPtr:
					return L"Ptr<"+type->GetTypeName()+L">"+(canOutput?L"&":L"");
				case Text:
					return type->GetTypeName()+(canOutput?L"&":L"");
				default:
					return L"";
				}
			}

/***********************************************************************
MethodInfoImpl
***********************************************************************/

			MethodInfoImpl::MethodInfoImpl(IMethodGroupInfo* _ownerMethodGroup, ITypeDescriptor* _returnType, IParameterInfo::Decorator _returnDecorator, bool _isStatic)
				:ownerMethodGroup(_ownerMethodGroup)
				,ownerProperty(0)
				,isStatic(_isStatic)
			{
				if(_returnType)
				{
					returnInfo=new ParameterInfoImpl(this, L"", _returnType, _returnDecorator, false);
				}
			}

			MethodInfoImpl::~MethodInfoImpl()
			{
			}

			ITypeDescriptor* MethodInfoImpl::GetOwnerTypeDescriptor()
			{
				return ownerMethodGroup->GetOwnerTypeDescriptor();
			}

			IPropertyInfo* MethodInfoImpl::GetOwnerProperty()
			{
				return ownerProperty;
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

			IParameterInfo* MethodInfoImpl::GetReturn()
			{
				return returnInfo.Obj();
			}

			bool MethodInfoImpl::IsStatic()
			{
				return isStatic;
			}

			void MethodInfoImpl::CheckArguments(collections::Array<Value>& arguments)
			{
				if(arguments.Count()!=parameters.Count())
				{
					throw ArgumentCountMismtatchException();
				}
				for(vint i=0;i<parameters.Count();i++)
				{
					if(!arguments[i].CanConvertTo(parameters[i].Obj()))
					{
						throw ArgumentTypeMismtatchException(parameters[i]->GetName(), parameters[i].Obj(), arguments[i]);
					}
				}
			}

			Value MethodInfoImpl::Invoke(const Value& thisObject, collections::Array<Value>& arguments)
			{
				if(thisObject.IsNull())
				{
					if(!isStatic)
					{
						throw ArgumentNullException(L"thisObject");
					}
				}
				else if(!thisObject.CanConvertTo(ownerMethodGroup->GetOwnerTypeDescriptor(), Value::RawPtr))
				{
					throw ArgumentTypeMismtatchException(L"thisObject", ownerMethodGroup->GetOwnerTypeDescriptor(), Value::RawPtr, thisObject);
				}
				CheckArguments(arguments);
				return InvokeInternal(thisObject, arguments);
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

			bool MethodInfoImpl::SetOwnerMethodgroup(IMethodGroupInfo* _ownerMethodGroup)
			{
				if(ownerMethodGroup) return false;
				ownerMethodGroup=_ownerMethodGroup;
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
				if(thisObject.IsNull())
				{
					throw ArgumentNullException(L"thisObject");
				}
				else if(!thisObject.CanConvertTo(ownerEvent->GetOwnerTypeDescriptor(), Value::RawPtr))
				{
					throw ArgumentTypeMismtatchException(L"thisObject", ownerEvent->GetOwnerTypeDescriptor(), Value::RawPtr, thisObject);
				}
				handler(thisObject, arguments);
			}

/***********************************************************************
EventInfoImpl
***********************************************************************/

			EventInfoImpl::EventInfoImpl(ITypeDescriptor* _ownerTypeDescriptor, const WString& _name)
				:ownerTypeDescriptor(_ownerTypeDescriptor)
				,observingProperty(0)
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

			IPropertyInfo* EventInfoImpl::GetObservingProperty()
			{
				return observingProperty;
			}

			const WString& EventInfoImpl::GetName()
			{
				return name;
			}

			Ptr<IEventHandler> EventInfoImpl::Attach(const Value& thisObject, const Func<void(const Value&, Value&)>& handler)
			{
				if(thisObject.IsNull())
				{
					throw ArgumentNullException(L"thisObject");
				}
				else if(!thisObject.CanConvertTo(ownerTypeDescriptor, Value::RawPtr))
				{
					throw ArgumentTypeMismtatchException(L"thisObject", ownerTypeDescriptor, Value::RawPtr, thisObject);
				}
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
PropertyInfoImpl
***********************************************************************/

			PropertyInfoImpl::PropertyInfoImpl(ITypeDescriptor* _ownerTypeDescriptor, const WString& _name, MethodInfoImpl* _getter, MethodInfoImpl* _setter, EventInfoImpl* _valueChangedEvent)
				:ownerTypeDescriptor(_ownerTypeDescriptor)
				,name(_name)
				,getter(_getter)
				,setter(_setter)
				,valueChangedEvent(_valueChangedEvent)
			{
				if(getter) getter->ownerProperty=this;
				if(setter) setter->ownerProperty=this;
				if(valueChangedEvent) valueChangedEvent->observingProperty=this;
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

			bool PropertyInfoImpl::IsReadable()
			{
				return getter!=0;
			}

			bool PropertyInfoImpl::IsWritable()
			{
				return setter!=0;
			}

			IParameterInfo* PropertyInfoImpl::GetReturn()
			{
				return getter?getter->GetReturn():0;
			}

			IMethodInfo* PropertyInfoImpl::GetGetter()
			{
				return getter;
			}

			IMethodInfo* PropertyInfoImpl::GetSetter()
			{
				return setter;
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
FieldInfoImpl
***********************************************************************/

			FieldInfoImpl::FieldInfoImpl(ITypeDescriptor* _ownerTypeDescriptor, const WString& _name, Ptr<IParameterInfo> _returnInfo)
				:ownerTypeDescriptor(_ownerTypeDescriptor)
				,name(_name)
				,returnInfo(_returnInfo)
			{
			}

			FieldInfoImpl::~FieldInfoImpl()
			{
			}

			ITypeDescriptor* FieldInfoImpl::GetOwnerTypeDescriptor()
			{
				return ownerTypeDescriptor;
			}

			const WString& FieldInfoImpl::GetName()
			{
				return name;
			}

			bool FieldInfoImpl::IsReadable()
			{
				return true;
			}

			bool FieldInfoImpl::IsWritable()
			{
				return true;
			}

			IParameterInfo* FieldInfoImpl::GetReturn()
			{
				return returnInfo.Obj();
			}

			IMethodInfo* FieldInfoImpl::GetGetter()
			{
				return 0;
			}

			IMethodInfo* FieldInfoImpl::GetSetter()
			{
				return 0;
			}

			IEventInfo* FieldInfoImpl::GetValueChangedEvent()
			{
				return 0;
			}

			Value FieldInfoImpl::GetValue(const Value& thisObject)
			{
				if(thisObject.IsNull())
				{
					throw ArgumentNullException(L"thisObject");
				}
				else if(!thisObject.CanConvertTo(ownerTypeDescriptor, Value::RawPtr))
				{
					throw ArgumentTypeMismtatchException(L"thisObject", ownerTypeDescriptor, Value::RawPtr, thisObject);
				}
				DescriptableObject* rawThisObject=thisObject.GetRawPtr();
				if(rawThisObject)
				{
					return GetValueInternal(thisObject);
				}
				else
				{
					return Value();
				}
			}

			void FieldInfoImpl::SetValue(const Value& thisObject, const Value& newValue)
			{
				if(thisObject.IsNull())
				{
					throw ArgumentNullException(L"thisObject");
				}
				else if(!thisObject.CanConvertTo(ownerTypeDescriptor, Value::RawPtr))
				{
					throw ArgumentTypeMismtatchException(L"thisObject", ownerTypeDescriptor, Value::RawPtr, thisObject);
				}
				if(!newValue.CanConvertTo(returnInfo.Obj()))
				{
					throw ArgumentTypeMismtatchException(L"newValue", returnInfo.Obj(), newValue);
				}
				DescriptableObject* rawThisObject=thisObject.GetRawPtr();
				if(rawThisObject)
				{
					SetValueInternal(thisObject, newValue);
				}
			}

/***********************************************************************
TypeDescriptorImpl
***********************************************************************/

			MethodGroupInfoImpl* TypeDescriptorImpl::PrepareMethodGroup(const WString& name)
			{
				vint index=methodGroups.Keys().IndexOf(name);
				if(index==-1)
				{
					Ptr<MethodGroupInfoImpl> methodGroup=new MethodGroupInfoImpl(this, name);
					methodGroups.Add(name, methodGroup);
					return methodGroup.Obj();
				}
				else
				{
					return methodGroups.Values().Get(index).Obj();
				}
			}

			MethodGroupInfoImpl* TypeDescriptorImpl::PrepareConstructorGroup()
			{
				if(!constructorGroup)
				{
					constructorGroup=new MethodGroupInfoImpl(this, L"");
				}
				return constructorGroup.Obj();
			}

			IPropertyInfo* TypeDescriptorImpl::AddProperty(Ptr<IPropertyInfo> value)
			{
				properties.Add(value->GetName(), value);
				return value.Obj();
			}

			IEventInfo* TypeDescriptorImpl::AddEvent(Ptr<IEventInfo> value)
			{
				events.Add(value->GetName(), value);
				return value.Obj();
			}

			IMethodInfo* TypeDescriptorImpl::AddMethod(const WString& name, Ptr<MethodInfoImpl> value)
			{
				MethodGroupInfoImpl* methodGroup=PrepareMethodGroup(name);
				value->SetOwnerMethodgroup(methodGroup);
				methodGroup->AddMethod(value);
				return value.Obj();
			}

			IMethodInfo* TypeDescriptorImpl::AddConstructor(Ptr<MethodInfoImpl> value)
			{
				MethodGroupInfoImpl* methodGroup=PrepareConstructorGroup();
				value->SetOwnerMethodgroup(methodGroup);
				methodGroup->AddMethod(value);
				return value.Obj();
			}

			void TypeDescriptorImpl::AddBaseType(ITypeDescriptor* value)
			{
				baseTypeDescriptors.Add(value);
			}

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
				return typeName;
			}

			IValueSerializer* TypeDescriptorImpl::GetValueSerializer()
			{
				Load();
				return valueSerializer.Obj();
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

			bool TypeDescriptorImpl::CanConvertTo(ITypeDescriptor* targetType)
			{
				if(this==targetType) return true;
				for(vint i=0;i<baseTypeDescriptors.Count();i++)
				{
					if(baseTypeDescriptors[i]->CanConvertTo(targetType)) return true;
				}
				return false;
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
				return constructorGroup.Obj();
			}
		}
	}
}