#include "GuiTypeDescriptorBuilder.h"

namespace vl
{
	using namespace collections;

	namespace reflection
	{
		namespace description
		{

/***********************************************************************
description::PropertyInfoImpl
***********************************************************************/

			class PropertyInfoImpl : public Object, public IPropertyInfo
			{
			protected:
				ITypeDescriptor*						ownerTypeDescriptor;
				WString									name;
				ITypeDescriptor*						type;
				bool									nullable;
				Func<Value(const Value&)>				getter;
				Func<void(const Value&, const Value&)>	setter;
				WString									valueChangedEventName;
			public:
				PropertyInfoImpl(ITypeDescriptor* _ownerTypeDescriptor, const WString& _name, ITypeDescriptor* _type, bool _nullable, const Func<Value(const Value&)>& _getter, const Func<void(const Value&, const Value&)>& _setter, const WString& _valueChangedEventName)
					:ownerTypeDescriptor(_ownerTypeDescriptor)
					,name(_name)
					,type(_type)
					,nullable(_nullable)
					,getter(_getter)
					,setter(_setter)
					,valueChangedEventName(_valueChangedEventName)
				{
				}

				ITypeDescriptor* GetOwnerTypeDescriptor()override
				{
					return ownerTypeDescriptor;
				}

				const WString& GetName()override
				{
					return name;
				}

				ITypeDescriptor* GetValueTypeDescriptor()override
				{
					return type;
				}

				bool CanBeNull()override
				{
					return nullable;
				}

				bool IsReadable()override
				{
					return getter;
				}

				bool IsWritable()override
				{
					return setter;
				}

				IEventInfo* GetValueChangedEvent()override
				{
					return ownerTypeDescriptor->GetEventByName(valueChangedEventName, true);
				}

				Value GetValue(const Value& thisObject)override
				{
					if(getter)
					{
						return getter(thisObject);
					}
					else
					{
						throw PropertyIsNotReadableException(this);
					}
				}

				void SetValue(const Value& thisObject, Value newValue)override
				{
					if(setter)
					{
						setter(thisObject, newValue);
					}
					else
					{
						throw PropertyIsNotWritableException(this);
					}
				}
			};

/***********************************************************************
description::ParameterInfoImpl
***********************************************************************/

			class ParameterInfoImpl : public Object, public IParameterInfo
			{
			protected:
				IMethodInfo*							ownerMethod;
				WString									name;
				ITypeDescriptor*						type;
				bool									nullable;
				bool									canOutput;
			public:
				ParameterInfoImpl(IMethodInfo* _ownerMethod, const WString& _name, ITypeDescriptor* _type, bool _nullable, bool _canOutput)
					:ownerMethod(_ownerMethod)
					,name(_name)
					,type(_type)
					,nullable(_nullable)
					,canOutput(_canOutput)
				{
				}

				ITypeDescriptor* GetOwnerTypeDescriptor()override
				{
					return ownerMethod->GetOwnerTypeDescriptor();
				}

				const WString& GetName()override
				{
					return name;
				}

				ITypeDescriptor* GetValueTypeDescriptor()override
				{
					return type;
				}

				bool CanBeNull()override
				{
					return nullable;
				}

				IMethodInfo* GetOwnerMethod()override
				{
					return ownerMethod;
				}

				bool CanOutput()override
				{
					return canOutput;
				}
			};

/***********************************************************************
description::MethodReturnImpl
***********************************************************************/

			class MethodReturnImpl : public Object, public IValueInfo
			{
			protected:
				ITypeDescriptor*						type;
				bool									nullable;
			public:
				MethodReturnImpl(ITypeDescriptor* _type, bool _nullable)
					:type(_type)
					,nullable(_nullable)
				{
				}

				ITypeDescriptor* GetValueTypeDescriptor()override
				{
					return type;
				}

				bool CanBeNull()override
				{
					return nullable;
				}
			};

/***********************************************************************
description::MethodInfoImpl
***********************************************************************/

			class MethodInfoImpl : public Object, public IMethodInfo
			{
			protected:
				IMethodGroupInfo*											ownerMethodGroup;
				List<Ptr<ParameterInfoImpl>>									parameters;
				Ptr<MethodReturnImpl>										returnInfo;
				Func<Value(const Value&, collections::IArray<Value>&)>		invoker;
			public:
				MethodInfoImpl(IMethodGroupInfo* _ownerMethodGroup)
					:ownerMethodGroup(_ownerMethodGroup)
				{
				}

				ITypeDescriptor* GetOwnerTypeDescriptor()override
				{
					return ownerMethodGroup->GetOwnerTypeDescriptor();
				}

				const WString& GetName()override
				{
					return ownerMethodGroup->GetName();
				}

				IMethodGroupInfo* GetOwnerMethodGroup()override
				{
					return ownerMethodGroup;
				}

				vint GetParameterCount()override
				{
					return parameters.Count();
				}

				IParameterInfo* GetParameter(vint index)override
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

				IValueInfo* GetReturn()override
				{
					return returnInfo.Obj();
				}

				Value Invoke(const Value& thisObject, collections::IArray<Value>& arguments)override
				{
					return invoker(thisObject, arguments);
				}

				void BuilderSetParameter(const WString& _name, ITypeDescriptor* _type, bool _nullable, bool _canOutput)
				{
					for(vint i=0;i<parameters.Count();i++)
					{
						if(parameters[i]->GetName()==_name)
						{
							throw ParameterAlreadyExistsException(this, _name);
						}
					}
					parameters.Add(new ParameterInfoImpl(this, _name, _type, _nullable, _canOutput));
				}

				void BuilderSetReturn(ITypeDescriptor* _type, bool _nullable)
				{
					returnInfo=new MethodReturnImpl(_type, _nullable);
				}

				void BuilderSetInvoker(const Func<Value(const Value&, collections::IArray<Value>&)>& _invoker)
				{
					invoker=_invoker;
				}
			};

/***********************************************************************
description::MethodGroupInfoImpl
***********************************************************************/

			class MethodGroupInfoImpl : public Object, public IMethodGroupInfo
			{
			protected:
				ITypeDescriptor*						ownerTypeDescriptor;
				WString									name;
				List<Ptr<MethodInfoImpl>>				methods;
			public:
				MethodGroupInfoImpl(ITypeDescriptor* _ownerTypeDescriptor, const WString& _name)
					:ownerTypeDescriptor(_ownerTypeDescriptor)
					,name(_name)
				{
				}

				ITypeDescriptor* GetOwnerTypeDescriptor()override
				{
					return ownerTypeDescriptor;
				}

				const WString& GetName()override
				{
					return name;
				}

				vint GetMethodCount()
				{
					return methods.Count();
				}

				IMethodInfo* GetMethod(vint index)
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

				void BuilderSetMethod(Ptr<MethodInfoImpl> _method)
				{
					methods.Add(_method);
				}
			};

/***********************************************************************
description::GeneralTypeDescriptor::PropertyGroup
***********************************************************************/

			GeneralTypeDescriptor::PropertyGroup::PropertyGroup()
				:valueSerializer(0)
				,loaded(false)
				,ownerTypeDescriptor(0)
			{
			}

			GeneralTypeDescriptor::PropertyGroup::~PropertyGroup()
			{
			}

			void GeneralTypeDescriptor::PropertyGroup::Prepare()
			{
				if(!loaded)
				{
					loaded=true;
					loaderProcedure(this);
				}
			}

			//----------------------------------------------------------

			GeneralTypeDescriptor::PropertyGroup::MethodBuilder::MethodBuilder(PropertyGroup& _propertyGroup, const WString& _name)
				:propertyGroup(_propertyGroup)
			{
				vint index=propertyGroup.methodGroups.Keys().IndexOf(_name);
				Ptr<MethodGroupInfoImpl> methodGroup;
				if(index==-1)
				{
					methodGroup=new MethodGroupInfoImpl(propertyGroup.ownerTypeDescriptor, _name);
					buildingMethodGroup=methodGroup;
					propertyGroup.methodGroups.Add(_name, buildingMethodGroup);
				}
				else
				{
					buildingMethodGroup=propertyGroup.methodGroups.Values()[index];
					methodGroup=buildingMethodGroup.Cast<MethodGroupInfoImpl>();
				}

				Ptr<MethodInfoImpl> method=new MethodInfoImpl(buildingMethodGroup.Obj());
				methodGroup->BuilderSetMethod(method);
				buildingMethod=method;
			}

			GeneralTypeDescriptor::PropertyGroup::MethodBuilder& GeneralTypeDescriptor::PropertyGroup::MethodBuilder::Parameter(const WString& _name, ITypeDescriptor* _type, bool _nullable, bool _canOutput)
			{
				dynamic_cast<MethodInfoImpl*>(buildingMethod.Obj())->BuilderSetParameter(_name, _type, _nullable, _canOutput);
				return *this;
			}

			GeneralTypeDescriptor::PropertyGroup::MethodBuilder& GeneralTypeDescriptor::PropertyGroup::MethodBuilder::Return(ITypeDescriptor* _type, bool _nullable)
			{
				dynamic_cast<MethodInfoImpl*>(buildingMethod.Obj())->BuilderSetReturn(_type, _nullable);
				return *this;
			}

			GeneralTypeDescriptor::PropertyGroup::MethodBuilder& GeneralTypeDescriptor::PropertyGroup::MethodBuilder::Invoker(const Func<Value(const Value&, collections::IArray<Value>&)>& _invoker)
			{
				dynamic_cast<MethodInfoImpl*>(buildingMethod.Obj())->BuilderSetInvoker(_invoker);
				return *this;
			}

			GeneralTypeDescriptor::PropertyGroup& GeneralTypeDescriptor::PropertyGroup::MethodBuilder::Done()
			{
				return propertyGroup;
			}

			//----------------------------------------------------------

			GeneralTypeDescriptor::PropertyGroup::EventBuilder::EventBuilder(PropertyGroup& _propertyGroup, const WString& _name)
				:propertyGroup(_propertyGroup)
			{
			}

			GeneralTypeDescriptor::PropertyGroup::EventBuilder& GeneralTypeDescriptor::PropertyGroup::EventBuilder::TriggerInstaller(const Func<void(const Value&, IEventInfo*)>& _triggerInstaller)
			{
				return *this;
			}

			GeneralTypeDescriptor::PropertyGroup::EventBuilder& GeneralTypeDescriptor::PropertyGroup::EventBuilder::Invoker(const Func<Value(const Value&, Value&)>& _invoker)
			{
				return *this;
			}

			GeneralTypeDescriptor::PropertyGroup& GeneralTypeDescriptor::PropertyGroup::EventBuilder::Done()
			{
				return propertyGroup;
			}

			//----------------------------------------------------------

			GeneralTypeDescriptor::PropertyGroup& GeneralTypeDescriptor::PropertyGroup::TypeName(const WString& _typeName)
			{
				typeName=_typeName;
				return *this;
			}

			GeneralTypeDescriptor::PropertyGroup& GeneralTypeDescriptor::PropertyGroup::Property(const WString& _name, ITypeDescriptor* _type, bool _nullable, const Func<Value(const Value&)>& _getter, const Func<void(const Value&, const Value&)>& _setter, const WString& _valueChangedEventName)
			{
				if(!properties.Keys().Contains(_name))
				{
					properties.Add(_name, new PropertyInfoImpl(ownerTypeDescriptor, _name, _type, _nullable, _getter, _setter, _valueChangedEventName));
					return *this;
				}
				else
				{
					throw PropertyAlreadyExistsException(ownerTypeDescriptor, _name);
				}
			}

			GeneralTypeDescriptor::PropertyGroup::MethodBuilder GeneralTypeDescriptor::PropertyGroup::Method(const WString& _name)
			{
				return MethodBuilder(*this, _name);
			}

			GeneralTypeDescriptor::PropertyGroup::EventBuilder GeneralTypeDescriptor::PropertyGroup::Event(const WString& _name)
			{
				return EventBuilder(*this, _name);
			}

/***********************************************************************
description::GeneralTypeDescriptor
***********************************************************************/

			GeneralTypeDescriptor::GeneralTypeDescriptor(const Func<void(PropertyGroup*)>& loaderProcedure)
			{
				propertyGroup.loaderProcedure=loaderProcedure;
				propertyGroup.ownerTypeDescriptor=this;
			}

			GeneralTypeDescriptor::~GeneralTypeDescriptor()
			{
			}

			GeneralTypeDescriptor::PropertyGroup& GeneralTypeDescriptor::Operations()
			{
				return propertyGroup;
			}

			const WString& GeneralTypeDescriptor::GetTypeName()
			{
				propertyGroup.Prepare();
				return propertyGroup.typeName;
			}

			IValueSerializer* GeneralTypeDescriptor::GetValueSerializer()
			{
				propertyGroup.Prepare();
				return propertyGroup.valueSerializer;
			}

			vint GeneralTypeDescriptor::GetBaseTypeDescriptorCount()
			{
				propertyGroup.Prepare();
				return propertyGroup.baseTypeDescriptors.Count();
			}

			ITypeDescriptor* GeneralTypeDescriptor::GetBaseTypeDescriptor(vint index)
			{
				propertyGroup.Prepare();
				if(0<=index && index<propertyGroup.baseTypeDescriptors.Count())
				{
					return propertyGroup.baseTypeDescriptors[index];
				}
				else
				{
					return 0;
				}
			}

			vint GeneralTypeDescriptor::GetPropertyCount()
			{
				propertyGroup.Prepare();
				return propertyGroup.properties.Count();
			}

			IPropertyInfo* GeneralTypeDescriptor::GetProperty(vint index)
			{
				propertyGroup.Prepare();
				if(0<=index && index<propertyGroup.properties.Count())
				{
					return propertyGroup.properties.Values()[index].Obj();
				}
				else
				{
					return 0;
				}
			}

			bool GeneralTypeDescriptor::IsPropertyExists(const WString& name, bool inheritable)
			{
				propertyGroup.Prepare();
				if(propertyGroup.properties.Keys().Contains(name))
				{
					return true;
				}
				if(inheritable)
				{
					for(vint i=0;i<propertyGroup.baseTypeDescriptors.Count();i++)
					{
						if(propertyGroup.baseTypeDescriptors[i]->IsPropertyExists(name, true))
						{
							return true;
						}
					}
				}
				return false;
			}

			IPropertyInfo* GeneralTypeDescriptor::GetPropertyByName(const WString& name, bool inheritable)
			{
				propertyGroup.Prepare();
				vint index=propertyGroup.properties.Keys().IndexOf(name);
				if(index!=-1)
				{
					return propertyGroup.properties.Values()[index].Obj();
				}
				if(inheritable)
				{
					for(vint i=0;i<propertyGroup.baseTypeDescriptors.Count();i++)
					{
						IPropertyInfo* result=propertyGroup.baseTypeDescriptors[i]->GetPropertyByName(name, true);
						if(result)
						{
							return result;
						}
					}
				}
				return 0;
			}

			vint GeneralTypeDescriptor::GetEventCount()
			{
				propertyGroup.Prepare();
				return propertyGroup.events.Count();
			}

			IEventInfo* GeneralTypeDescriptor::GetEvent(vint index)
			{
				propertyGroup.Prepare();
				if(0<=index && index<propertyGroup.events.Count())
				{
					return propertyGroup.events.Values()[index].Obj();
				}
				else
				{
					return 0;
				}
			}

			bool GeneralTypeDescriptor::IsEventExists(const WString& name, bool inheritable)
			{
				propertyGroup.Prepare();
				if(propertyGroup.events.Keys().Contains(name))
				{
					return true;
				}
				if(inheritable)
				{
					for(vint i=0;i<propertyGroup.baseTypeDescriptors.Count();i++)
					{
						if(propertyGroup.baseTypeDescriptors[i]->IsEventExists(name, true))
						{
							return true;
						}
					}
				}
				return false;
			}

			IEventInfo* GeneralTypeDescriptor::GetEventByName(const WString& name, bool inheritable)
			{
				propertyGroup.Prepare();
				vint index=propertyGroup.events.Keys().IndexOf(name);
				if(index!=-1)
				{
					return propertyGroup.events.Values()[index].Obj();
				}
				if(inheritable)
				{
					for(vint i=0;i<propertyGroup.baseTypeDescriptors.Count();i++)
					{
						IEventInfo* result=propertyGroup.baseTypeDescriptors[i]->GetEventByName(name, true);
						if(result)
						{
							return result;
						}
					}
				}
				return 0;
			}

			vint GeneralTypeDescriptor::GetMethodGroupCount()
			{
				propertyGroup.Prepare();
				return propertyGroup.methodGroups.Count();
			}

			IMethodGroupInfo* GeneralTypeDescriptor::GetMethodGroup(vint index)
			{
				propertyGroup.Prepare();
				if(0<=index && index<propertyGroup.methodGroups.Count())
				{
					return propertyGroup.methodGroups.Values()[index].Obj();
				}
				else
				{
					return 0;
				}
			}

			bool GeneralTypeDescriptor::IsMethodGroupExists(const WString& name, bool inheritable)
			{
				propertyGroup.Prepare();
				if(propertyGroup.methodGroups.Keys().Contains(name))
				{
					return true;
				}
				if(inheritable)
				{
					for(vint i=0;i<propertyGroup.baseTypeDescriptors.Count();i++)
					{
						if(propertyGroup.baseTypeDescriptors[i]->IsMethodGroupExists(name, true))
						{
							return true;
						}
					}
				}
				return false;
			}

			IMethodGroupInfo* GeneralTypeDescriptor::GetMethodGroupByName(const WString& name, bool inheritable)
			{
				propertyGroup.Prepare();
				vint index=propertyGroup.methodGroups.Keys().IndexOf(name);
				if(index!=-1)
				{
					return propertyGroup.methodGroups.Values()[index].Obj();
				}
				if(inheritable)
				{
					for(vint i=0;i<propertyGroup.baseTypeDescriptors.Count();i++)
					{
						IMethodGroupInfo* result=propertyGroup.baseTypeDescriptors[i]->GetMethodGroupByName(name, true);
						if(result)
						{
							return result;
						}
					}
				}
				return 0;
			}

			IMethodGroupInfo* GeneralTypeDescriptor::GetConstructorGroup()
			{
				propertyGroup.Prepare();
				return propertyGroup.contructorGroup.Obj();
			}
		}
	}
}