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
TypeInfoImp;
***********************************************************************/

			class TypeInfoImpl : public Object, public ITypeInfo
			{
			protected:
				Decorator								decorator;
				ITypeDescriptor*						typeDescriptor;
				collections::List<Ptr<ITypeInfo>>		genericArguments;
				Ptr<ITypeInfo>							elementType;
			public:
				TypeInfoImpl(Decorator _decorator);
				~TypeInfoImpl();

				Decorator								GetDecorator()override;
				ITypeInfo*								GetElementType()override;
				ITypeDescriptor*						GetTypeDescriptor()override;
				vint									GetGenericArgumentCount()override;
				ITypeInfo*								GetGenericArgument(vint index)override;
				WString									GetTypeFriendlyName()override;

				void									SetTypeDescriptor(ITypeDescriptor* value);
				void									AddGenericArgument(Ptr<ITypeInfo> value);
				void									SetElementType(Ptr<ITypeInfo> value);
			};

/***********************************************************************
ParameterInfoImpl
***********************************************************************/

			class ParameterInfoImpl : public Object, public IParameterInfo
			{
			protected:
				IMethodInfo*							ownerMethod;
				WString									name;
				Ptr<ITypeInfo>							type;
			public:
				ParameterInfoImpl(IMethodInfo* _ownerMethod, const WString& _name, Ptr<ITypeInfo> _type);
				~ParameterInfoImpl();

				ITypeDescriptor*						GetOwnerTypeDescriptor()override;
				const WString&							GetName()override;
				ITypeInfo*								GetType()override;
				IMethodInfo*							GetOwnerMethod()override;
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
				Ptr<ITypeInfo>							returnInfo;
				bool									isStatic;

				virtual Value							InvokeInternal(const Value& thisObject, collections::Array<Value>& arguments)=0;
			public:
				MethodInfoImpl(IMethodGroupInfo* _ownerMethodGroup, Ptr<ITypeInfo> _return, bool _isStatic);
				~MethodInfoImpl();

				ITypeDescriptor*						GetOwnerTypeDescriptor()override;
				IPropertyInfo*							GetOwnerProperty()override;
				const WString&							GetName()override;
				IMethodGroupInfo*						GetOwnerMethodGroup()override;
				vint									GetParameterCount()override;
				IParameterInfo*							GetParameter(vint index)override;
				ITypeInfo*								GetReturn()override;
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
				ITypeInfo*								GetReturn()override;
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
				Ptr<ITypeInfo>							returnInfo;
				WString									name;

				virtual Value							GetValueInternal(const Value& thisObject)=0;
				virtual void							SetValueInternal(Value& thisObject, const Value& newValue)=0;
			public:
				FieldInfoImpl(ITypeDescriptor* _ownerTypeDescriptor, const WString& _name, Ptr<ITypeInfo> _returnInfo);
				~FieldInfoImpl();

				ITypeDescriptor*						GetOwnerTypeDescriptor()override;
				const WString&							GetName()override;
				bool									IsReadable()override;
				bool									IsWritable()override;
				ITypeInfo*								GetReturn()override;
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
TypeFlagSelector
***********************************************************************/

			struct TypeFlags
			{
				struct ReadonlyListType{};
				struct ListType{};
				struct NonGenericType{};
			};

			template<typename T>
			struct ValueRetriver
			{
				T* pointer;
			};

			template<typename T>
			struct ValueRetriver<T&>
			{
				T* pointer;
			};

			template<typename TDerived>
			struct IsInheritsFromEnumerable
			{
				template<typename T>
				static void* Inherit(collections::IEnumerable<T>* source){}
				static char Inherit(void* source){}
				static char Inherit(const void* source){}

				static const bool										Result=sizeof(Inherit(((ValueRetriver<TDerived>*)0)->pointer))==sizeof(void*);
			};

			template<typename TDerived>
			struct IsInheritsFromConstEnumerable
			{
				template<typename T>
				static void* Inherit(const collections::IEnumerable<T>* source){}
				static char Inherit(void* source){}
				static char Inherit(const void* source){}

				static const bool										Result=sizeof(Inherit(((ValueRetriver<TDerived>*)0)->pointer))==sizeof(void*);
			};

			template<typename T, bool IsEnumerable, bool IsConstEnumerable>
			struct TypeFlagSelectorCase
			{
				typedef int												TypeFlag;
			};

			template<typename T>
			struct TypeFlagSelectorCase<T, true, true>
			{
				typedef TypeFlags::ListType								TypeFlag;
			};

			template<typename T>
			struct TypeFlagSelectorCase<T, false, true>
			{
				typedef TypeFlags::ReadonlyListType						TypeFlag;
			};

			template<typename T>
			struct TypeFlagSelectorCase<T, false, false>
			{
				typedef TypeFlags::NonGenericType						TypeFlag;
			};

			template<typename T>
			struct TypeFlagSelector
			{
				typedef typename TypeFlagSelectorCase<
					T,
					IsInheritsFromEnumerable<T>::Result,
					IsInheritsFromConstEnumerable<T>::Result
					>::TypeFlag											TypeFlag;
			};

/***********************************************************************
TypeInfoRetriver
***********************************************************************/

			template<typename T, typename TTypeFlag>
			struct DetailTypeInfoRetriver
			{
				static const ITypeInfo::Decorator						Decorator=ITypeInfo::TypeDescriptor;
				typedef void											Type;
				typedef void											TempValueType;
				typedef void											ResultReferenceType;
				typedef void											ResultNonReferenceType;
			};

			template<typename T>
			struct TypeInfoRetriver
			{
				typedef typename TypeFlagSelector<T>::TypeFlag									TypeFlag;

				static const ITypeInfo::Decorator												Decorator=DetailTypeInfoRetriver<T, TypeFlag>::Decorator;

				typedef typename DetailTypeInfoRetriver<T, TypeFlag>::Type						Type;
				typedef typename DetailTypeInfoRetriver<T, TypeFlag>::TempValueType				TempValueType;
				typedef typename DetailTypeInfoRetriver<T, TypeFlag>::ResultReferenceType		ResultReferenceType;
				typedef typename DetailTypeInfoRetriver<T, TypeFlag>::ResultNonReferenceType	ResultNonReferenceType;

				static Ptr<ITypeInfo> CreateTypeInfo()
				{
					return DetailTypeInfoRetriver<T, TypeFlag>::CreateTypeInfo();
				}
			};

			template<typename T>
			struct DetailTypeInfoRetriver<T, TypeFlags::NonGenericType>
			{
				static const ITypeInfo::Decorator						Decorator=ITypeInfo::TypeDescriptor;
				typedef T												Type;
				typedef T												TempValueType;
				typedef T&												ResultReferenceType;
				typedef T												ResultNonReferenceType;

				static Ptr<ITypeInfo> CreateTypeInfo()
				{
					Ptr<TypeInfoImpl> type=new TypeInfoImpl(ITypeInfo::TypeDescriptor);
					type->SetTypeDescriptor(GetTypeDescriptor<Type>());
					return type;
				}
			};

			template<typename T>
			struct DetailTypeInfoRetriver<const T, TypeFlags::NonGenericType>
			{
				typedef DetailTypeInfoRetriver<T, TypeFlags::NonGenericType>	UpLevelRetriver;

				static const ITypeInfo::Decorator								Decorator=UpLevelRetriver::Decorator;
				typedef typename UpLevelRetriver::Type							Type;
				typedef T														TempValueType;
				typedef const T&												ResultReferenceType;
				typedef const T													ResultNonReferenceType;

				static Ptr<ITypeInfo> CreateTypeInfo()
				{
					return TypeInfoRetriver<T>::CreateTypeInfo();
				}
			};

			template<typename T>
			struct DetailTypeInfoRetriver<volatile T, TypeFlags::NonGenericType>
			{
				typedef DetailTypeInfoRetriver<T, TypeFlags::NonGenericType>	UpLevelRetriver;

				static const ITypeInfo::Decorator								Decorator=UpLevelRetriver::Decorator;
				typedef typename UpLevelRetriver::Type							Type;
				typedef T														TempValueType;
				typedef T&														ResultReferenceType;
				typedef T														ResultNonReferenceType;

				static Ptr<ITypeInfo> CreateTypeInfo()
				{
					return TypeInfoRetriver<T>::CreateTypeInfo();
				}
			};

			template<typename T>
			struct DetailTypeInfoRetriver<T*, TypeFlags::NonGenericType>
			{
				typedef DetailTypeInfoRetriver<T, TypeFlags::NonGenericType>	UpLevelRetriver;

				static const ITypeInfo::Decorator								Decorator=ITypeInfo::RawPtr;
				typedef typename UpLevelRetriver::Type							Type;
				typedef T*														TempValueType;
				typedef T*&														ResultReferenceType;
				typedef T*														ResultNonReferenceType;

				static Ptr<ITypeInfo> CreateTypeInfo()
				{
					Ptr<ITypeInfo> elementType=TypeInfoRetriver<T>::CreateTypeInfo();
					Ptr<TypeInfoImpl> type=new TypeInfoImpl(ITypeInfo::RawPtr);
					type->SetElementType(elementType);
					return type;
				}
			};

			template<typename T>
			struct DetailTypeInfoRetriver<Ptr<T>, TypeFlags::NonGenericType>
			{
				typedef DetailTypeInfoRetriver<T, TypeFlags::NonGenericType>	UpLevelRetriver;

				static const ITypeInfo::Decorator								Decorator=ITypeInfo::SharedPtr;
				typedef typename UpLevelRetriver::Type							Type;
				typedef Ptr<T>													TempValueType;
				typedef Ptr<T>&													ResultReferenceType;
				typedef Ptr<T>													ResultNonReferenceType;

				static Ptr<ITypeInfo> CreateTypeInfo()
				{
					Ptr<ITypeInfo> elementType=TypeInfoRetriver<T>::CreateTypeInfo();
					Ptr<TypeInfoImpl> type=new TypeInfoImpl(ITypeInfo::SharedPtr);
					type->SetElementType(elementType);
					return type;
				}
			};

			template<typename T>
			struct DetailTypeInfoRetriver<T&, TypeFlags::NonGenericType>
			{
				typedef DetailTypeInfoRetriver<T, TypeFlags::NonGenericType>	UpLevelRetriver;

				static const ITypeInfo::Decorator								Decorator=UpLevelRetriver::Decorator;
				typedef typename UpLevelRetriver::Type							Type;
				typedef typename UpLevelRetriver::TempValueType					TempValueType;
				typedef T&														ResultReferenceType;
				typedef T														ResultNonReferenceType;

				static Ptr<ITypeInfo> CreateTypeInfo()
				{
					return TypeInfoRetriver<T>::CreateTypeInfo();
				}
			};

			template<typename T>
			struct DetailTypeInfoRetriver<T, TypeFlags::ReadonlyListType>
			{
				typedef DetailTypeInfoRetriver<T, TypeFlags::NonGenericType>	UpLevelRetriver;

				static const ITypeInfo::Decorator								Decorator=UpLevelRetriver::Decorator;
				typedef IValueReadonlyList										Type;
				typedef typename UpLevelRetriver::TempValueType					TempValueType;
				typedef typename UpLevelRetriver::ResultReferenceType			ResultReferenceType;
				typedef typename UpLevelRetriver::ResultNonReferenceType		ResultNonReferenceType;

				static Ptr<ITypeInfo> CreateTypeInfo()
				{
					typedef typename DetailTypeInfoRetriver<T, TypeFlags::NonGenericType>::Type		ContainerType;
					typedef typename ContainerType::ElementType										ElementType;

					Ptr<TypeInfoImpl> arrayType=new TypeInfoImpl(ITypeInfo::TypeDescriptor);
					arrayType->SetTypeDescriptor(Description<IValueReadonlyList>::GetAssociatedTypeDescriptor());

					Ptr<TypeInfoImpl> genericType=new TypeInfoImpl(ITypeInfo::Generic);
					genericType->SetElementType(arrayType);
					genericType->AddGenericArgument(TypeInfoRetriver<ElementType>::CreateTypeInfo());

					Ptr<TypeInfoImpl> type=new TypeInfoImpl(ITypeInfo::SharedPtr);
					type->SetElementType(genericType);
					return type;
				}
			};

			template<typename T>
			struct DetailTypeInfoRetriver<T, TypeFlags::ListType>
			{
				typedef DetailTypeInfoRetriver<T, TypeFlags::NonGenericType>	UpLevelRetriver;

				static const ITypeInfo::Decorator								Decorator=UpLevelRetriver::Decorator;
				typedef IValueList												Type;
				typedef typename UpLevelRetriver::TempValueType					TempValueType;
				typedef typename UpLevelRetriver::ResultReferenceType			ResultReferenceType;
				typedef typename UpLevelRetriver::ResultNonReferenceType		ResultNonReferenceType;

				static Ptr<ITypeInfo> CreateTypeInfo()
				{
					typedef typename DetailTypeInfoRetriver<T, TypeFlags::NonGenericType>::Type		ContainerType;
					typedef typename ContainerType::ElementType										ElementType;

					Ptr<TypeInfoImpl> arrayType=new TypeInfoImpl(ITypeInfo::TypeDescriptor);
					arrayType->SetTypeDescriptor(Description<IValueList>::GetAssociatedTypeDescriptor());

					Ptr<TypeInfoImpl> genericType=new TypeInfoImpl(ITypeInfo::Generic);
					genericType->SetElementType(arrayType);
					genericType->AddGenericArgument(TypeInfoRetriver<ElementType>::CreateTypeInfo());

					Ptr<TypeInfoImpl> type=new TypeInfoImpl(ITypeInfo::SharedPtr);
					type->SetElementType(genericType);
					return type;
				}
			};

/***********************************************************************
TypeInfoRetriver Helper Functions (BoxValue, UnboxValue)
***********************************************************************/

			template<typename T, ITypeInfo::Decorator Decorator>
			struct ValueAccessor
			{
			};

			template<typename T>
			struct ValueAccessor<T*, ITypeInfo::RawPtr>
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
			struct ValueAccessor<Ptr<T>, ITypeInfo::SharedPtr>
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
			struct ValueAccessor<T, ITypeInfo::TypeDescriptor>
			{
				static Value BoxValue(const T& object, ITypeDescriptor* typeDescriptor)
				{
					if(!typeDescriptor)
					{
						typeDescriptor=GetTypeDescriptor<typename TypeInfoRetriver<T>::Type>();
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
							typeDescriptor=GetTypeDescriptor<typename TypeInfoRetriver<T>::Type>();
						}
						throw ArgumentTypeMismtatchException(valueName, typeDescriptor, Value::Text, value);
					}
					return result;
				}
			};

			template<>
			struct ValueAccessor<Value, ITypeInfo::TypeDescriptor>
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
				return ValueAccessor<T, TypeInfoRetriver<T>::Decorator>::BoxValue(object, typeDescriptor);
			}

			template<typename T>
			T UnboxValue(const Value& value, ITypeDescriptor* typeDescriptor=0, const WString& valueName=L"value")
			{
				return ValueAccessor<T, TypeInfoRetriver<T>::Decorator>::UnboxValue(value, typeDescriptor, valueName);
			}

/***********************************************************************
TypeInfoRetriver Helper Functions (UnboxParameter)
***********************************************************************/

			template<typename T, typename TTypeFlag>
			struct ParameterAccessor
			{
			};

			template<typename T>
			struct ParameterAccessor<T, TypeFlags::NonGenericType>
			{
				static Value BoxParameter(T& object, ITypeDescriptor* typeDescriptor)
				{
					return BoxValue<T>(object, typeDescriptor);
				}

				static void UnboxParameter(const Value& value, T& result, ITypeDescriptor* typeDescriptor, const WString& valueName)
				{
					result=UnboxValue<T>(value, typeDescriptor, valueName);
				}
			};

			template<typename T>
			struct ParameterAccessor<T, TypeFlags::ReadonlyListType>
			{
				static Value BoxParameter(T& object, ITypeDescriptor* typeDescriptor)
				{
					Ptr<IValueReadonlyList> result=new ValueReadonlyListWrapper<T*>(&object);
					return BoxValue<Ptr<IValueReadonlyList>>(result, Description<IValueReadonlyList>::GetAssociatedTypeDescriptor());
				}

				static void UnboxParameter(const Value& value, T& result, ITypeDescriptor* typeDescriptor, const WString& valueName)
				{
					typedef typename T::ElementType ElementType;
					Ptr<IValueReadonlyList> listProxy=UnboxValue<Ptr<IValueReadonlyList>>(value, typeDescriptor, valueName);
					LazyList<ElementType> lazyList=listProxy->GetLazyList<ElementType>();
					collections::CopyFrom(result, lazyList);
				}
			};

			template<typename T>
			struct ParameterAccessor<T, TypeFlags::ListType>
			{
				static Value BoxParameter(T& object, ITypeDescriptor* typeDescriptor)
				{
					Ptr<IValueList> result=new ValueListWrapper<T*>(&object);
					return BoxValue<Ptr<IValueList>>(result, Description<IValueList>::GetAssociatedTypeDescriptor());
				}

				static void UnboxParameter(const Value& value, T& result, ITypeDescriptor* typeDescriptor, const WString& valueName)
				{
					typedef typename T::ElementType ElementType;
					Ptr<IValueList> listProxy=UnboxValue<Ptr<IValueList>>(value, typeDescriptor, valueName);
					LazyList<ElementType> lazyList=listProxy->GetLazyList<ElementType>();
					collections::CopyFrom(result, lazyList);
				}
			};

			template<typename T>
			Value BoxParameter(typename TypeInfoRetriver<T>::ResultReferenceType object, ITypeDescriptor* typeDescriptor=0)
			{
				return ParameterAccessor<typename TypeInfoRetriver<T>::ResultNonReferenceType, typename TypeInfoRetriver<T>::TypeFlag>::BoxParameter(object, typeDescriptor);
			}

			template<typename T>
			void UnboxParameter(const Value& value, T& result, ITypeDescriptor* typeDescriptor=0, const WString& valueName=L"value")
			{
				ParameterAccessor<T, typename TypeInfoRetriver<T>::TypeFlag>::UnboxParameter(value, result, typeDescriptor, valueName);
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
						return BoxParameter<TField>(object->*FieldRef, GetReturn()->GetTypeDescriptor());
					}
					return Value();
				}

				void SetValueInternal(Value& thisObject, const Value& newValue)override
				{
					TClass* object=UnboxValue<TClass*>(thisObject);
					if(object)
					{
						UnboxParameter<TField>(newValue, object->*FieldRef, GetReturn()->GetTypeDescriptor(), L"newValue");
					}
				}
			public:
				CustomFieldInfoImpl(ITypeDescriptor* _ownerTypeDescriptor, const WString& _name)
					:FieldInfoImpl(_ownerTypeDescriptor, _name, TypeInfoRetriver<TField>::CreateTypeInfo())
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
						return BoxValue<TField>(fieldValue, GetReturn()->GetTypeDescriptor());
					}

					void SetValueInternal(Value& thisObject, const Value& newValue)override
					{
						T structValue=UnboxValue<T>(thisObject, GetOwnerTypeDescriptor(), L"thisObject");
						TField fieldValue=UnboxValue<TField>(newValue, GetReturn()->GetTypeDescriptor(), L"newValue");
						structValue.*field=fieldValue;
						thisObject=BoxValue<T>(structValue, GetOwnerTypeDescriptor());
					}
				public:
					FieldSerializer(ITypeDescriptor* _ownerTypeDescriptor, TField T::* _field, const WString& _name)
						:field(_field)
						,FieldInfoImpl(_ownerTypeDescriptor, _name, TypeInfoRetriver<TField>::CreateTypeInfo())
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