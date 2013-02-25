/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Framework::Reflection

Classes:
***********************************************************************/

#ifndef VCZH_REFLECTION_GUITYPEDESCRIPTORBUILDER
#define VCZH_REFLECTION_GUITYPEDESCRIPTORBUILDER

#include "GuiTypeDescriptor.h"
#include "..\Collections\List.h"
#include "..\Collections\Dictionary.h"

namespace vl
{
	namespace reflection
	{
		namespace description
		{

/***********************************************************************
ParameterInfoImpl
***********************************************************************/

			class ParameterInfoImpl : public Object, public IParameterInfo
			{
			protected:
				IMethodInfo*							ownerMethod;
				WString									name;
				ITypeDescriptor*						type;
				Decorator								decorator;
				bool									canOutput;
			public:
				ParameterInfoImpl(IMethodInfo* _ownerMethod, const WString& _name, ITypeDescriptor* _type, Decorator _decorator, bool _canOutput);
				~ParameterInfoImpl();

				ITypeDescriptor*						GetOwnerTypeDescriptor()override;
				const WString&							GetName()override;
				ITypeDescriptor*						GetValueTypeDescriptor()override;
				IMethodInfo*							GetOwnerMethod()override;
				Decorator								GetDecorator()override;
				bool									CanOutput()override;
				WString									GetTypeFriendlyName()override;
			};

/***********************************************************************
MethodInfoImpl
***********************************************************************/

			class MethodInfoImpl : public Object, public IMethodInfo
			{
				friend class PropertyInfoImpl;
			protected:
				IMethodGroupInfo*						ownerMethodGroup;
				IPropertyInfo*							ownerProperty;
				collections::List<Ptr<IParameterInfo>>	parameters;
				Ptr<IParameterInfo>						returnInfo;

				virtual Value							InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)=0;
			public:
				MethodInfoImpl(IMethodGroupInfo* _ownerMethodGroup, ITypeDescriptor* _returnType, IParameterInfo::Decorator _returnDecorator);
				~MethodInfoImpl();

				ITypeDescriptor*						GetOwnerTypeDescriptor()override;
				IPropertyInfo*							GetOwnerProperty()override;
				const WString&							GetName()override;
				IMethodGroupInfo*						GetOwnerMethodGroup()override;
				vint									GetParameterCount()override;
				IParameterInfo*							GetParameter(vint index)override;
				IParameterInfo*							GetReturn()override;
				Value									Invoke(const Value& thisObject, collections::Array<Value>& arguments);
				bool									AddParameter(Ptr<IParameterInfo> parameter);
			};

/***********************************************************************
MethodGroupInfoImpl
***********************************************************************/

			class MethodGroupInfoImpl : public Object, public IMethodGroupInfo
			{
			protected:
				ITypeDescriptor*						ownerTypeDescriptor;
				WString									name;
				collections::List<Ptr<IMethodInfo>>		methods;
			public:
				MethodGroupInfoImpl(ITypeDescriptor* _ownerTypeDescriptor, const WString& _name);
				~MethodGroupInfoImpl();

				ITypeDescriptor*						GetOwnerTypeDescriptor()override;
				const WString&							GetName()override;
				vint									GetMethodCount()override;
				IMethodInfo*							GetMethod(vint index)override;
				bool									AddMethod(Ptr<IMethodInfo> _method);
			};

/***********************************************************************
EventInfoImpl
***********************************************************************/

			class EventInfoImpl : public Object, public IEventInfo
			{
				friend class PropertyInfoImpl;
			protected:
				class EventHandlerImpl : public Object, public IEventHandler
				{
				protected:
					EventInfoImpl*						ownerEvent;
					DescriptableObject*					ownerObject;
					Func<void(const Value&, Value&)>	handler;
					bool								attached;
				public:
					EventHandlerImpl(EventInfoImpl* _ownerEvent, DescriptableObject* ownerObject, const Func<void(const Value&, Value&)>& _handler);
					~EventHandlerImpl();

					IEventInfo*							GetOwnerEvent()override;
					Value								GetOwnerObject()override;
					bool								IsAttached()override;
					bool								Detach()override;
					void								Invoke(const Value& thisObject, Value& arguments)override;
				};

			protected:
				ITypeDescriptor*						ownerTypeDescriptor;
				IPropertyInfo*							observingProperty;
				WString									name;

				virtual void							AttachInternal(DescriptableObject* thisObject, Ptr<IEventHandler> eventHandler)=0;
				virtual void							DetachInternal(DescriptableObject* thisObject, IEventHandler* eventHandler)=0;
			public:
				EventInfoImpl(ITypeDescriptor* _ownerTypeDescriptor, const WString& _name);
				~EventInfoImpl();

				ITypeDescriptor*						GetOwnerTypeDescriptor()override;
				const WString&							GetName()override;
				IPropertyInfo*							GetObservingProperty()override;
				Ptr<IEventHandler>						Attach(const Value& thisObject, const Func<void(const Value&, Value&)>& handler)override;
			};

/***********************************************************************
TypeDescriptorImpl
***********************************************************************/

			class PropertyInfoImpl : public Object, public IPropertyInfo
			{
			protected:
				ITypeDescriptor*						ownerTypeDescriptor;
				WString									name;
				ITypeDescriptor*						type;
				MethodInfoImpl*							getter;
				MethodInfoImpl*							setter;
				EventInfoImpl*							valueChangedEvent;
			public:
				PropertyInfoImpl(ITypeDescriptor* _ownerTypeDescriptor, const WString& _name, ITypeDescriptor* _type, MethodInfoImpl* _getter, MethodInfoImpl* _setter, EventInfoImpl* _valueChangedEvent);
				~PropertyInfoImpl();

				ITypeDescriptor*						GetOwnerTypeDescriptor()override;
				const WString&							GetName()override;
				ITypeDescriptor*						GetValueTypeDescriptor()override;
				bool									IsReadable()override;
				bool									IsWritable()override;
				IMethodInfo*							GetGetter()=0;
				IMethodInfo*							GetSetter()=0;
				IEventInfo*								GetValueChangedEvent()override;
				Value									GetValue(const Value& thisObject)override;
				void									SetValue(const Value& thisObject, const Value& newValue)override;
			};

/***********************************************************************
TypeDescriptorImpl
***********************************************************************/

			class TypeDescriptorImpl : public Object, public ITypeDescriptor
			{
			private:
				bool														loaded;
				WString														typeName;
			protected:
				ITypeDescriptor*											ownerTypeDescriptor;
				IValueSerializer*											valueSerializer;
				collections::List<ITypeDescriptor*>							baseTypeDescriptors;
				collections::Dictionary<WString, Ptr<IPropertyInfo>>		properties;
				collections::Dictionary<WString, Ptr<IEventInfo>>			events;
				collections::Dictionary<WString, Ptr<IMethodGroupInfo>>		methodGroups;
				Ptr<IMethodGroupInfo>										contructorGroup;

				virtual void				LoadInternal()=0;
				void						Load();
			public:
				TypeDescriptorImpl(const WString& _typeName);
				~TypeDescriptorImpl();

				const WString&				GetTypeName()override;
				IValueSerializer*			GetValueSerializer()override;
				vint						GetBaseTypeDescriptorCount()override;
				ITypeDescriptor*			GetBaseTypeDescriptor(vint index)override;
				bool						CanConvertTo(ITypeDescriptor* targetType)override;

				vint						GetPropertyCount()override;
				IPropertyInfo*				GetProperty(vint index)override;
				bool						IsPropertyExists(const WString& name, bool inheritable)override;
				IPropertyInfo*				GetPropertyByName(const WString& name, bool inheritable)override;

				vint						GetEventCount()override;
				IEventInfo*					GetEvent(vint index)override;
				bool						IsEventExists(const WString& name, bool inheritable)override;
				IEventInfo*					GetEventByName(const WString& name, bool inheritable)override;

				vint						GetMethodGroupCount()override;
				IMethodGroupInfo*			GetMethodGroup(vint index)override;
				bool						IsMethodGroupExists(const WString& name, bool inheritable)override;
				IMethodGroupInfo*			GetMethodGroupByName(const WString& name, bool inheritable)override;
				IMethodGroupInfo*			GetConstructorGroup()override;
			};

/***********************************************************************
TypeDescriptorImpl
***********************************************************************/
		}
	}
}

#endif