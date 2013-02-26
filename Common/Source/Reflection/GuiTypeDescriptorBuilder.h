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
				bool									isStatic;

				virtual Value							InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)=0;
			public:
				MethodInfoImpl(IMethodGroupInfo* _ownerMethodGroup, ITypeDescriptor* _returnType, IParameterInfo::Decorator _returnDecorator, bool _isStatic);
				~MethodInfoImpl();

				ITypeDescriptor*						GetOwnerTypeDescriptor()override;
				IPropertyInfo*							GetOwnerProperty()override;
				const WString&							GetName()override;
				IMethodGroupInfo*						GetOwnerMethodGroup()override;
				vint									GetParameterCount()override;
				IParameterInfo*							GetParameter(vint index)override;
				IParameterInfo*							GetReturn()override;
				bool									IsStatic()override;
				Value									Invoke(const Value& thisObject, collections::Array<Value>& arguments);
				bool									AddParameter(Ptr<IParameterInfo> parameter);
				bool									SetOwnerMethodgroup(IMethodGroupInfo* _ownerMethodGroup);
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
				IParameterInfo*							GetReturn()override;
				IMethodInfo*							GetGetter()override;
				IMethodInfo*							GetSetter()override;
				IEventInfo*								GetValueChangedEvent()override;
				Value									GetValue(const Value& thisObject)override;
				void									SetValue(const Value& thisObject, const Value& newValue)override;
			};

/***********************************************************************
FieldInfoImpl
***********************************************************************/

			class FieldInfoImpl : public Object, public IPropertyInfo
			{
			protected:
				ITypeDescriptor*						ownerTypeDescriptor;
				Ptr<IParameterInfo>						returnInfo;
				WString									name;

				virtual Value							GetValueInternal(const Value& thisObject)=0;
				virtual void							SetValueInternal(const Value& thisObject, const Value& newValue)=0;
			public:
				FieldInfoImpl(ITypeDescriptor* _ownerTypeDescriptor, const WString& _name, Ptr<IParameterInfo> _returnInfo);
				~FieldInfoImpl();

				ITypeDescriptor*						GetOwnerTypeDescriptor()override;
				const WString&							GetName()override;
				ITypeDescriptor*						GetValueTypeDescriptor()override;
				bool									IsReadable()override;
				bool									IsWritable()override;
				IParameterInfo*							GetReturn()override;
				IMethodInfo*							GetGetter()override;
				IMethodInfo*							GetSetter()override;
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
				Ptr<IValueSerializer>										valueSerializer;
				collections::List<ITypeDescriptor*>							baseTypeDescriptors;
				collections::Dictionary<WString, Ptr<IPropertyInfo>>		properties;
				collections::Dictionary<WString, Ptr<IEventInfo>>			events;
				collections::Dictionary<WString, Ptr<MethodGroupInfoImpl>>	methodGroups;
				Ptr<MethodGroupInfoImpl>									constructorGroup;

			protected:
				MethodGroupInfoImpl*		PrepareMethodGroup(const WString& name);
				MethodGroupInfoImpl*		PrepareConstructorGroup();
				IPropertyInfo*				AddProperty(Ptr<IPropertyInfo> value);
				IEventInfo*					AddEvent(Ptr<IEventInfo> value);
				IMethodInfo*				AddMethod(const WString& name, Ptr<MethodInfoImpl> value);
				IMethodInfo*				AddConstructor(Ptr<MethodInfoImpl> value);
				void						AddBaseType(ITypeDescriptor* value);

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
ParameterTypeInfo
***********************************************************************/

			template<typename T>
			struct ParameterTypeInfo
			{
				static const IParameterInfo::Decorator		Decorator=IParameterInfo::Text;
				static const bool							Output=false;
				static const bool							OutputFromRef=true;
				typedef T									Type;
			};

			template<typename T>
			struct ParameterTypeInfo<const T>
			{
				static const IParameterInfo::Decorator		Decorator=ParameterTypeInfo<T>::Decorator;
				static const bool							Output=false;
				static const bool							OutputFromRef=false;
				typedef typename ParameterTypeInfo<T>::Type	Type;
			};

			template<typename T>
			struct ParameterTypeInfo<volatile T>
			{
				static const IParameterInfo::Decorator		Decorator=ParameterTypeInfo<T>::Decorator;
				static const bool							Output=false;
				static const bool							OutputFromRef=true;
				typedef typename ParameterTypeInfo<T>::Type	Type;
			};

			template<typename T>
			struct ParameterTypeInfo<T*>
			{
				static const IParameterInfo::Decorator		Decorator=IParameterInfo::RawPtr;
				static const bool							Output=false;
				static const bool							OutputFromRef=true;
				typedef typename ParameterTypeInfo<T>::Type	Type;
			};

			template<typename T>
			struct ParameterTypeInfo<Ptr<T>>
			{
				static const IParameterInfo::Decorator		Decorator=IParameterInfo::SharedPtr;
				static const bool							Output=false;
				static const bool							OutputFromRef=true;
				typedef typename ParameterTypeInfo<T>::Type	Type;
			};

			template<typename T>
			struct ParameterTypeInfo<T&>
			{
				static const IParameterInfo::Decorator		Decorator=ParameterTypeInfo<T>::Decorator;
				static const bool							Output=ParameterTypeInfo<T>::OutputFromRef;
				static const bool							OutputFromRef=true;
				typedef typename ParameterTypeInfo<T>::Type	Type;
			};

/***********************************************************************
ParameterTypeInfo Helper Functions
***********************************************************************/

			template<typename T>
			IParameterInfo* CreateParameterInfo(IMethodInfo* ownerMethod, const WString& name)
			{
				return new ParameterInfoImpl(
					0,
					L"",
					GetTypeDescriptor<typename ParameterTypeInfo<T>::Type>(),
					ParameterTypeInfo<T>::Decorator,
					ParameterTypeInfo<T>::Output
					);
			}

			template<typename T, IParameterInfo::Decorator Decorator>
			struct ValueAccessor
			{
			};

			template<typename T>
			struct ValueAccessor<T*, IParameterInfo::RawPtr>
			{
				static Value BoxValue(T* object, ITypeDescriptor* typeDescriptor)
				{
					return Value::From(object);
				}

				static T* UnboxValue(const Value& value, ITypeDescriptor* typeDescriptor, const WString& valueName)
				{
					if(value.IsNull()) return 0;
					T* result=dynamic_cast<T*>(value.GetRawPtr());
					if(!result)
					{
						if(!typeDescriptor)
						{
							typeDescriptor=GetTypeDescriptor<T>();
						}
						throw ArgumentTypeMismtatchException(valueName, typeDescriptor, Value::RawPtr, value);
					}
					return result;
				}
			};

			template<typename T>
			struct ValueAccessor<Ptr<T>, IParameterInfo::SharedPtr>
			{
				static Value BoxValue(Ptr<T> object, ITypeDescriptor* typeDescriptor)
				{
					return Value::From(object);
				}

				static Ptr<T> UnboxValue(const Value& value, ITypeDescriptor* typeDescriptor, const WString& valueName)
				{
					if(value.IsNull()) return 0;
					Ptr<T> result=value.GetSharedPtr().Cast<T>();
					if(!result)
					{
						if(!typeDescriptor)
						{
							typeDescriptor=GetTypeDescriptor<T>();
						}
						throw ArgumentTypeMismtatchException(valueName, typeDescriptor, Value::SharedPtr, value);
					}
					return result;
				}
			};

			template<typename T>
			struct ValueAccessor<T, IParameterInfo::Text>
			{
				static Value BoxValue(const T& object, ITypeDescriptor* typeDescriptor)
				{
					if(!typeDescriptor)
					{
						typeDescriptor=GetTypeDescriptor<T>();
					}
					ITypedValueSerializer<T>* serializer=dynamic_cast<ITypedValueSerializer<T>*>(typeDescriptor->GetValueSerializer());
					Value result;
					serializer->Serialize(object, result);
					return result;
				}

				static T UnboxValue(const Value& value, ITypeDescriptor* typeDescriptor, const WString& valueName)
				{
					ITypedValueSerializer<T>* serializer=dynamic_cast<ITypedValueSerializer<T>*>(value.GetTypeDescriptor()->GetValueSerializer());
					T result;
					if(!serializer->Deserialize(value, result))
					{
						if(!typeDescriptor)
						{
							typeDescriptor=GetTypeDescriptor<T>();
						}
						throw ArgumentTypeMismtatchException(valueName, typeDescriptor, Value::Text, value);
					}
					return result;
				}
			};

			template<typename T>
			Value BoxValue(const T& object, ITypeDescriptor* typeDescriptor=0)
			{
				return ValueAccessor<T, ParameterTypeInfo<T>::Decorator>::BoxValue(object, typeDescriptor);
			}

			template<typename T>
			T UnboxValue(const Value& value, ITypeDescriptor* typeDescriptor=0, const WString& valueName=L"value")
			{
				return ValueAccessor<T, ParameterTypeInfo<T>::Decorator>::UnboxValue(value, typeDescriptor, valueName);
			}

/***********************************************************************
CustomFieldInfoImpl
***********************************************************************/

			template<typename TClass, typename TField, TField TClass::* FieldRef>
			class CustomFieldInfoImpl : public FieldInfoImpl
			{
			protected:
				Value GetValueInternal(const Value& thisObject)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject);
					if(object)
					{
						return BoxValue<TField>(object->*FieldRef, GetValueTypeDescriptor());
					}
					return Value();
				}

				void SetValueInternal(const Value& thisObject, const Value& newValue)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject);
					if(object)
					{
						object->*FieldRef=UnboxValue<TField>(newValue, GetValueTypeDescriptor(), L"newValue");
					}
				}
			public:
				CustomFieldInfoImpl(ITypeDescriptor* _ownerTypeDescriptor, const WString& _name)
					:FieldInfoImpl(_ownerTypeDescriptor, _name, CreateParameterInfo<TField>(0, L""))
				{
				}
			};
		}
	}
}

#endif