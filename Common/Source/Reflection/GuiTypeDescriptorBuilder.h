/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Framework::Reflection

Classes:
***********************************************************************/

#ifndef VCZH_REFLECTION_GUITYPEDESCRIPTORBUILDER
#define VCZH_REFLECTION_GUITYPEDESCRIPTORBUILDER

#include "GuiTypeDescriptorPredefined.h"

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
				void									CheckArguments(collections::Array<Value>& arguments)override;
				Value									Invoke(const Value& thisObject, collections::Array<Value>& arguments)override;
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
				typedef collections::List<Ptr<IEventHandler>>		EventHandlerList;
				static const wchar_t*								EventHandlerListInternalPropertyName;

				class EventHandlerImpl : public Object, public IEventHandler
				{
				protected:
					EventInfoImpl*						ownerEvent;
					DescriptableObject*					ownerObject;
					Ptr<IValueFunctionProxy>			handler;
					Ptr<DescriptableObject>				tag;
					bool								attached;
				public:
					EventHandlerImpl(EventInfoImpl* _ownerEvent, DescriptableObject* _ownerObject, Ptr<IValueFunctionProxy> _handler);
					~EventHandlerImpl();

					IEventInfo*							GetOwnerEvent()override;
					Value								GetOwnerObject()override;
					bool								IsAttached()override;
					bool								Detach()override;
					void								Invoke(const Value& thisObject, Value& arguments)override;

					Ptr<DescriptableObject>				GetTag();
					void								SetTag(Ptr<DescriptableObject> _tag);
				};

			protected:
				ITypeDescriptor*						ownerTypeDescriptor;
				collections::List<IPropertyInfo*>		observingProperties;
				WString									name;

				virtual void							AttachInternal(DescriptableObject* thisObject, IEventHandler* eventHandler)=0;
				virtual void							DetachInternal(DescriptableObject* thisObject, IEventHandler* eventHandler)=0;

				void									AddEventHandler(DescriptableObject* thisObject, Ptr<IEventHandler> eventHandler);
				void									RemoveEventHandler(DescriptableObject* thisObject, IEventHandler* eventHandler);
			public:
				EventInfoImpl(ITypeDescriptor* _ownerTypeDescriptor, const WString& _name);
				~EventInfoImpl();

				ITypeDescriptor*						GetOwnerTypeDescriptor()override;
				const WString&							GetName()override;
				vint									GetObservingPropertyCount()override;
				IPropertyInfo*							GetObservingProperty(vint index)override;
				Ptr<IEventHandler>						Attach(const Value& thisObject, Ptr<IValueFunctionProxy> handler)override;
			};

/***********************************************************************
TypeDescriptorImpl
***********************************************************************/

			class PropertyInfoImpl : public Object, public IPropertyInfo
			{
			protected:
				ITypeDescriptor*						ownerTypeDescriptor;
				WString									name;
				MethodInfoImpl*							getter;
				MethodInfoImpl*							setter;
				EventInfoImpl*							valueChangedEvent;
			public:
				PropertyInfoImpl(ITypeDescriptor* _ownerTypeDescriptor, const WString& _name, MethodInfoImpl* _getter, MethodInfoImpl* _setter, EventInfoImpl* _valueChangedEvent);
				~PropertyInfoImpl();

				ITypeDescriptor*						GetOwnerTypeDescriptor()override;
				const WString&							GetName()override;
				bool									IsReadable()override;
				bool									IsWritable()override;
				IParameterInfo*							GetReturn()override;
				IMethodInfo*							GetGetter()override;
				IMethodInfo*							GetSetter()override;
				IEventInfo*								GetValueChangedEvent()override;
				Value									GetValue(const Value& thisObject)override;
				void									SetValue(Value& thisObject, const Value& newValue)override;
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
				virtual void							SetValueInternal(Value& thisObject, const Value& newValue)=0;
			public:
				FieldInfoImpl(ITypeDescriptor* _ownerTypeDescriptor, const WString& _name, Ptr<IParameterInfo> _returnInfo);
				~FieldInfoImpl();

				ITypeDescriptor*						GetOwnerTypeDescriptor()override;
				const WString&							GetName()override;
				bool									IsReadable()override;
				bool									IsWritable()override;
				IParameterInfo*							GetReturn()override;
				IMethodInfo*							GetGetter()override;
				IMethodInfo*							GetSetter()override;
				IEventInfo*								GetValueChangedEvent()override;
				Value									GetValue(const Value& thisObject)override;
				void									SetValue(Value& thisObject, const Value& newValue)override;
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
				static const IParameterInfo::Decorator					Decorator=IParameterInfo::Text;
				static const bool										Output=false;
				static const bool										OutputFromRef=true;
				typedef T												Type;
				typedef T												TempValueType;
			};

			template<typename T>
			struct ParameterTypeInfo<const T>
			{
				static const IParameterInfo::Decorator					Decorator=ParameterTypeInfo<T>::Decorator;
				static const bool										Output=false;
				static const bool										OutputFromRef=false;
				typedef typename ParameterTypeInfo<T>::Type				Type;
				typedef T												TempValueType;
			};

			template<typename T>
			struct ParameterTypeInfo<volatile T>
			{
				static const IParameterInfo::Decorator					Decorator=ParameterTypeInfo<T>::Decorator;
				static const bool										Output=false;
				static const bool										OutputFromRef=true;
				typedef typename ParameterTypeInfo<T>::Type				Type;
				typedef T												TempValueType;
			};

			template<typename T>
			struct ParameterTypeInfo<T*>
			{
				static const IParameterInfo::Decorator					Decorator=IParameterInfo::RawPtr;
				static const bool										Output=false;
				static const bool										OutputFromRef=true;
				typedef typename ParameterTypeInfo<T>::Type				Type;
				typedef T*												TempValueType;
			};

			template<typename T>
			struct ParameterTypeInfo<Ptr<T>>
			{
				static const IParameterInfo::Decorator					Decorator=IParameterInfo::SharedPtr;
				static const bool										Output=false;
				static const bool										OutputFromRef=true;
				typedef typename ParameterTypeInfo<T>::Type				Type;
				typedef Ptr<T>											TempValueType;
			};

			template<typename T>
			struct ParameterTypeInfo<T&>
			{
				static const IParameterInfo::Decorator					Decorator=ParameterTypeInfo<T>::Decorator;
				static const bool										Output=ParameterTypeInfo<T>::OutputFromRef;
				static const bool										OutputFromRef=true;
				typedef typename ParameterTypeInfo<T>::Type				Type;
				typedef typename ParameterTypeInfo<T>::TempValueType	TempValueType;
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

			template<>
			struct ValueAccessor<Value, IParameterInfo::Text>
			{
				static Value BoxValue(const Value& object, ITypeDescriptor* typeDescriptor)
				{
					return object;
				}

				static Value UnboxValue(const Value& value, ITypeDescriptor* typeDescriptor, const WString& valueName)
				{
					return value;
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
						return BoxValue<TField>(object->*FieldRef, GetReturn()->GetValueTypeDescriptor());
					}
					return Value();
				}

				void SetValueInternal(Value& thisObject, const Value& newValue)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject);
					if(object)
					{
						object->*FieldRef=UnboxValue<TField>(newValue, GetReturn()->GetValueTypeDescriptor(), L"newValue");
					}
				}
			public:
				CustomFieldInfoImpl(ITypeDescriptor* _ownerTypeDescriptor, const WString& _name)
					:FieldInfoImpl(_ownerTypeDescriptor, _name, CreateParameterInfo<TField>(0, L""))
				{
				}
			};

/***********************************************************************
StructValueSeriaizer
***********************************************************************/

			template<typename T>
			class StructValueSeriaizer : public GeneralValueSeriaizer<T>
			{
			public:
				class FieldSerializerBase : public Object
				{
				public:
					virtual bool				SerializeField(const T& input, WString& output)=0;
					virtual bool				DeserializeField(WString& input, T& output)=0;
				};

				template<typename TField>
				class FieldSerializer : public FieldSerializerBase, public FieldInfoImpl
				{
				protected:
					TField T::*					field;

					Value GetValueInternal(const Value& thisObject)override
					{
						T structValue=UnboxValue<T>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
						TField fieldValue=structValue.*field;
						return BoxValue<TField>(fieldValue, GetReturn()->GetValueTypeDescriptor());
					}

					void SetValueInternal(Value& thisObject, const Value& newValue)override
					{
						T structValue=UnboxValue<T>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
						TField fieldValue=UnboxValue<TField>(newValue, GetReturn()->GetValueTypeDescriptor(), L"newValue");
						structValue.*field=fieldValue;
						thisObject=BoxValue<T>(structValue, GetOwnerTypeDescriptor());
					}
				public:
					FieldSerializer(ITypeDescriptor* _ownerTypeDescriptor, TField T::* _field, const WString& _name)
						:field(_field)
						,FieldInfoImpl(_ownerTypeDescriptor, _name, CreateParameterInfo<TField>(0, L""))
					{
					}

					bool SerializeField(const T& input, WString& output)override
					{
						ITypedValueSerializer<TField>* serializer=GetValueSerializer<TField>();
						if(!serializer) return false;
						Value result;
						if(!serializer->Serialize(input.*field, result)) return false;
						output=result.GetText();
						return true;
					}

					bool DeserializeField(WString& input, T& output)override
					{
						ITypedValueSerializer<TField>* serializer=GetValueSerializer<TField>();
						if(!serializer) return false;
						Value value=Value::From(input, serializer->GetOwnerTypeDescriptor());
						return serializer->Deserialize(value, output.*field);
					}
				};

			protected:
				collections::Dictionary<WString, Ptr<FieldSerializerBase>>		fieldSerializers;
				bool															loaded;

				virtual void													LoadInternal()=0;

				void Load()
				{
					if(!loaded)
					{
						loaded=true;
						LoadInternal();
					}
				}

				WString Escape(const WString& text)
				{
					const wchar_t* reading=text.Buffer();
					if(wcschr(reading, L' ')==0 && wcschr(reading, L'{')==0 && wcschr(reading, L'}')==0)
					{
						return text;
					}

					WString result;
					while(wchar_t c=*reading++)
					{
						switch(c)
						{
						case L'{':
							result+=L"{{";
						case L'}':
							result+=L"}}";
							break;
						default:
							result+=c;
						}
					}
					return L"{"+result+L"}";
				}

				bool Unescape(const wchar_t*& reading, WString& field)
				{
					if(*reading==L'{')
					{
						const wchar_t* start=reading+1;
						const wchar_t* end=start;
						bool stop=false;
						while(!stop)
						{
							switch(*end)
							{
							case L'\0':
								return false;
							case L'{':
								if(end[1]==L'{') return false;
								end+=2;
								field+=L'{';
								break;
							case L'}':
								if(end[1]==L'}')
								{
									end+=2;
									field+=L'}';
								}
								else
								{
									stop=true;
								}
								break;
							default:
								field+=*end;
								end++;
							}
						}
						reading=end+1;
					}
					else
					{
						const wchar_t* space=wcschr(reading, L' ');
						if(space)
						{
							field=WString(reading, space-reading);
							reading=space+1;
						}
						else
						{
							field=reading;
							reading+=field.Length();
						}
					}
					return true;
				}

				bool Serialize(const T& input, WString& output)override
				{
					Load();
					WString result, field;
					for(vint i=0;i<fieldSerializers.Count();i++)
					{
						if(result!=L"") result+=L" ";
						result+=fieldSerializers.Keys()[i]+L":";

						Ptr<FieldSerializerBase> fieldSerializer=fieldSerializers.Values().Get(i);
						if(!fieldSerializer->SerializeField(input, field)) return false;
						result+=Escape(field);
					}
					output=result;
					return true;
				}

				bool Deserialize(const WString& input, T& output)override
				{
					Load();
					const wchar_t* reading=input.Buffer();
					while(true)
					{
						while(*reading==L' ') reading++;
						if(*reading==0) break;
						const wchar_t* comma=wcschr(reading, L':');
						if(!comma) return false;

						vint index=fieldSerializers.Keys().IndexOf(WString(reading, comma-reading));
						if(index==-1) return false;
						reading=comma+1;

						WString field;
						if(!Unescape(reading, field)) return false;
						Ptr<FieldSerializerBase> fieldSerializer=fieldSerializers.Values().Get(index);
						if(!fieldSerializer->DeserializeField(field, output)) return false;
					}
					return true;
				}
			public:
				StructValueSeriaizer(ITypeDescriptor* _ownedTypeDescriptor)
					:GeneralValueSeriaizer(_ownedTypeDescriptor)
					,loaded(false)
				{
				}

				const collections::Dictionary<WString, Ptr<FieldSerializerBase>>& GetFieldSerializers()
				{
					Load();
					return fieldSerializers;
				}
			};

			template<typename TSerializer>
			class StructTypeDescriptor : public SerializableTypeDescriptor<TSerializer>
			{
			protected:
				Ptr<TSerializer>				typedSerializer;

			public:
				StructTypeDescriptor()
				{
					typedSerializer=serializer.Cast<TSerializer>();
				}

				vint GetPropertyCount()override
				{
					return typedSerializer->GetFieldSerializers().Count();
				}

				IPropertyInfo* GetProperty(vint index)override
				{
					return typedSerializer->GetFieldSerializers().Values().Get(index).Cast<IPropertyInfo>().Obj();
				}

				bool IsPropertyExists(const WString& name, bool inheritable)override
				{
					return typedSerializer->GetFieldSerializers().Keys().Contains(name);
				}

				IPropertyInfo* GetPropertyByName(const WString& name, bool inheritable)override
				{
					vint index=typedSerializer->GetFieldSerializers().Keys().IndexOf(name);
					if(index==-1) return 0;
					return GetProperty(index);
				}
			};
		}
	}
}

#endif