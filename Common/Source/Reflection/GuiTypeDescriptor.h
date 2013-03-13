/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Framework::Reflection

XML Representation for Code Generation:
***********************************************************************/

#ifndef VCZH_REFLECTION_GUITYPEDESCRIPTOR
#define VCZH_REFLECTION_GUITYPEDESCRIPTOR

#include "..\Basic.h"
#include "..\Pointer.h"
#include "..\String.h"
#include "..\Exception.h"
#include "..\Function.h"
#include "..\Collections\List.h"
#include "..\Collections\Dictionary.h"
#include "..\Collections\Operation.h"
#include "..\Stream\Accessor.h"

namespace vl
{
	namespace reflection
	{

/***********************************************************************
Attribute
***********************************************************************/

		namespace description
		{
			class ITypeDescriptor;
			class ITypeInfo;
			class IEventHandler;
			class IEventInfo;
			class IPropertyInfo;
			class IParameterInfo;
			class IMethodInfo;
			class IMethodGroupInfo;
		}

		class DescriptableObject
		{
			template<typename T>
			friend class Description;
			friend class DescriptableValue;

			typedef collections::Dictionary<WString, Ptr<Object>>		InternalPropertyMap;
		protected:
			size_t									objectSize;
			description::ITypeDescriptor**			typeDescriptor;
			Ptr<InternalPropertyMap>				internalProperties;
		public:
			DescriptableObject();
			virtual ~DescriptableObject();

			description::ITypeDescriptor*			GetTypeDescriptor();
			Ptr<Object>								GetInternalProperty(const WString& name);
			void									SetInternalProperty(const WString& name, Ptr<Object> value);
		};
		
		template<typename T>
		class Description : public virtual DescriptableObject
		{
		protected:
			static description::ITypeDescriptor*		associatedTypeDescriptor;
		public:
			Description()
			{
				if(objectSize<sizeof(T))
				{
					objectSize=sizeof(T);
					if(!typeDescriptor || !*typeDescriptor || associatedTypeDescriptor)
					{
						typeDescriptor=&associatedTypeDescriptor;
					}
				}
			}

			static description::ITypeDescriptor* GetAssociatedTypeDescriptor()
			{
				return associatedTypeDescriptor;
			}

			static void SetAssociatedTypeDescroptor(description::ITypeDescriptor* typeDescroptor)
			{
				associatedTypeDescriptor=typeDescroptor;
			}
		};

		template<typename T>
		description::ITypeDescriptor* Description<T>::associatedTypeDescriptor=0;

		class IDescriptable : public virtual Interface, public Description<IDescriptable>
		{
		public:
			~IDescriptable(){}
		};

/***********************************************************************
Value
***********************************************************************/

		namespace description
		{
			class Value : public Object
			{
			public:
				enum ValueType
				{
					Null,
					RawPtr,
					SharedPtr,
					Text,
				};
			protected:
				ValueType						valueType;
				DescriptableObject*				rawPtr;
				Ptr<DescriptableObject>			sharedPtr;
				WString							text;
				ITypeDescriptor*				typeDescriptor;

				Value(DescriptableObject* value);
				Value(Ptr<DescriptableObject> value);
				Value(const WString& value, ITypeDescriptor* associatedTypeDescriptor);
			public:
				Value();
				Value(const Value& value);
				Value&							operator=(const Value& value);
				bool							operator==(const Value& value)const;
				bool							operator!=(const Value& value)const;

				ValueType						GetValueType()const;
				DescriptableObject*				GetRawPtr()const;
				Ptr<DescriptableObject>			GetSharedPtr()const;
				const WString&					GetText()const;
				ITypeDescriptor*				GetTypeDescriptor()const;
				WString							GetTypeFriendlyName()const;
				bool							IsNull()const;
				bool							CanConvertTo(ITypeDescriptor* targetType, ValueType targetValueType)const;
				bool							CanConvertTo(ITypeInfo* targetType)const;

				static Value					From(DescriptableObject* value);
				static Value					From(Ptr<DescriptableObject> value);
				static Value					From(const WString& value, ITypeDescriptor* type);

				static IMethodInfo*				SelectMethod(IMethodGroupInfo* methodGroup, collections::Array<Value>& arguments);
				static Value					Create(const WString& typeName);
				static Value					Create(const WString& typeName, collections::Array<Value>& arguments);
				static Value					InvokeStatic(const WString& typeName, const WString& name);
				static Value					InvokeStatic(const WString& typeName, const WString& name, collections::Array<Value>& arguments);
				Value							GetProperty(const WString& name)const;
				void							SetProperty(const WString& name, const Value& newValue);
				Value							Invoke(const WString& name)const;
				Value							Invoke(const WString& name, collections::Array<Value>& arguments)const;
				Ptr<IEventHandler>				AttachEvent(const WString& name, const Value& function)const;
				bool							DeleteRawPtr();

				template<typename T>
				void SetProperty(const WString& name, const T& newValue)
				{
					return SetProperty(name, BoxValue<T>(newValue));
				}

				class xs
				{
				protected:
					collections::Array<Value>	arguments;
				public:
					xs()
					{
					}

					template<typename T>
					xs& operator,(const T& value)
					{
						arguments.Resize(arguments.Count()+1);
						arguments[arguments.Count()-1]=BoxValue<T>(value);
						return *this;
					}

					xs& operator,(const Value& value)
					{
						arguments.Resize(arguments.Count()+1);
						arguments[arguments.Count()-1]=value;
						return *this;
					}

					operator collections::Array<Value>&()
					{
						return arguments;
					}
				};
			};

			class IValueSerializer : public Interface
			{
			public:
				virtual ITypeDescriptor*		GetOwnerTypeDescriptor()=0;
				virtual bool					Validate(const WString& text)=0;
				virtual bool					Parse(const WString& input, Value& output)=0;

				virtual bool					HasCandidate()=0;
				virtual vint					GetCandidateCount()=0;
				virtual WString					GetCandidate(vint index)=0;
				virtual bool					CanMergeCandidate()=0;
			};

			template<typename T>
			class ITypedValueSerializer : public IValueSerializer
			{
			public:
				virtual bool					Serialize(const T& input, Value& output)=0;
				virtual bool					Deserialize(const Value& input, T& output)=0;
			};

/***********************************************************************
Collections
***********************************************************************/

			class IValueReadonlyList : public virtual IDescriptable, public Description<IValueReadonlyList>
			{
			public:
				virtual vint					Count()=0;
				virtual Value					Get(vint index)=0;
				virtual bool					Contains(const Value& value)=0;
				virtual vint					IndexOf(const Value& value)=0;

				template<typename T>
				collections::LazyList<T> GetLazyList()
				{
					return collections::Range(0, Count())
						.Select([this](int i){return UnboxValue<T>(Get(i));});
				}
			};

			class IValueList : public IValueReadonlyList, public Description<IValueList>
			{
			public:
				virtual void					Set(vint index, const Value& value)=0;
				virtual vint					Add(const Value& value)=0;
				virtual vint					Insert(vint index, const Value& value)=0;
				virtual bool					Remove(const Value& value)=0;
				virtual bool					RemoveAt(vint index)=0;
				virtual void					Clear()=0;

				static Ptr<IValueList>			Create();
				static Ptr<IValueList>			Create(Ptr<IValueReadonlyList> values);
				static Ptr<IValueList>			Create(collections::LazyList<Value> values);
			};

			namespace trait_helper
			{
				template<typename T>
				struct RemovePtr
				{
					typedef T					Type;
				};
				
				template<typename T>
				struct RemovePtr<T*>
				{
					typedef T					Type;
				};
				
				template<typename T>
				struct RemovePtr<Ptr<T>>
				{
					typedef T					Type;
				};
			}

			template<typename T>
			class ValueReadonlyListWrapper : public Object, public IValueReadonlyList
			{
			protected:
				typedef typename trait_helper::RemovePtr<T>::Type		ContainerType;
				typedef typename ContainerType::ElementType				ElementType;
				typedef typename KeyType<ElementType>::Type				ElementKeyType;

				T								wrapperPointer;
			public:
				ValueReadonlyListWrapper(const T& _wrapperPointer)
					:wrapperPointer(_wrapperPointer)
				{
				}

				vint Count()override
				{
					return wrapperPointer->Count();
				}

				Value Get(vint index)override
				{
					return BoxValue<ElementType>(wrapperPointer->Get(index));
				}

				bool Contains(const Value& value)override
				{
					ElementKeyType item=UnboxValue<ElementKeyType>(value);
					return wrapperPointer->Contains(item);
				}

				vint IndexOf(const Value& value)override
				{
					ElementKeyType item=UnboxValue<ElementKeyType>(value);
					return wrapperPointer->IndexOf(item);
				}
			};

			template<typename T>
			class ValueListWrapper : public Object, public IValueList
			{
			protected:
				typedef typename trait_helper::RemovePtr<T>::Type		ContainerType;
				typedef typename ContainerType::ElementType				ElementType;
				typedef typename KeyType<ElementType>::Type				ElementKeyType;

				T								wrapperPointer;
			public:
				ValueListWrapper(const T& _wrapperPointer)
					:wrapperPointer(_wrapperPointer)
				{
				}

				vint Count()override
				{
					return wrapperPointer->Count();
				}

				Value Get(vint index)override
				{
					return BoxValue<ElementType>(wrapperPointer->Get(index));
				}

				bool Contains(const Value& value)override
				{
					ElementKeyType item=UnboxValue<ElementKeyType>(value);
					return wrapperPointer->Contains(item);
				}

				vint IndexOf(const Value& value)override
				{
					ElementKeyType item=UnboxValue<ElementKeyType>(value);
					return wrapperPointer->IndexOf(item);
				}

				void Set(vint index, const Value& value)override
				{
					ElementType item=UnboxValue<ElementType>(value);
					wrapperPointer->Set(index, item);
				}

				vint Add(const Value& value)override
				{
					ElementType item=UnboxValue<ElementType>(value);
					return wrapperPointer->Add(item);
				}

				vint Insert(vint index, const Value& value)override
				{
					ElementType item=UnboxValue<ElementType>(value);
					return wrapperPointer->Insert(index, item);
				}

				bool Remove(const Value& value)override
				{
					ElementKeyType item=UnboxValue<ElementKeyType>(value);
					return wrapperPointer->Remove(item);
				}

				bool RemoveAt(vint index)override
				{
					return wrapperPointer->RemoveAt(index);
				}

				void Clear()override
				{
					wrapperPointer->Clear();
				}
			};

/***********************************************************************
Interface Implementation Proxy
***********************************************************************/

			class IValueInterfaceProxy : public virtual IDescriptable, public Description<IValueReadonlyList>
			{
			public:
				virtual Value					Invoke(const WString& methodName, Ptr<IValueList> arguments)=0;
			};

			class IValueFunctionProxy : public virtual IDescriptable, public Description<IValueFunctionProxy>
			{
			public:
				virtual Value					Invoke(Ptr<IValueList> arguments)=0;
			};

			class ValueInterfaceRoot : public virtual IDescriptable
			{
			protected:
				Ptr<IValueInterfaceProxy>		proxy;
			public:
				ValueInterfaceRoot(Ptr<IValueInterfaceProxy> _proxy)
					:proxy(_proxy)
				{
				}

				Ptr<IValueInterfaceProxy> GetProxy()
				{
					return proxy;
				}
			};

/***********************************************************************
ITypeDescriptor (type)
***********************************************************************/

			class ITypeInfo : public virtual Interface
			{
			public:
				enum Decorator
				{
					RawPtr,
					SharedPtr,
					TypeDescriptor,
					Generic,
				};

				virtual Decorator				GetDecorator()=0;
				virtual ITypeInfo*				GetElementType()=0;
				virtual ITypeDescriptor*		GetTypeDescriptor()=0;
				virtual vint					GetGenericArgumentCount()=0;
				virtual ITypeInfo*				GetGenericArgument(vint index)=0;
				virtual WString					GetTypeFriendlyName()=0;
			};

/***********************************************************************
ITypeDescriptor (basic)
***********************************************************************/

			class IMemberInfo : public virtual Interface
			{
			public:
				virtual ITypeDescriptor*		GetOwnerTypeDescriptor()=0;
				virtual const WString&			GetName()=0;
			};

/***********************************************************************
ITypeDescriptor (event)
***********************************************************************/

			class IEventHandler : public virtual Interface
			{
			public:
				virtual IEventInfo*				GetOwnerEvent()=0;
				virtual Value					GetOwnerObject()=0;
				virtual bool					IsAttached()=0;
				virtual bool					Detach()=0;
				virtual void					Invoke(const Value& thisObject, Value& arguments)=0;
			};

			class IEventInfo : public IMemberInfo
			{
			public:
				virtual ITypeInfo*				GetHandlerType()=0;
				virtual vint					GetObservingPropertyCount()=0;
				virtual IPropertyInfo*			GetObservingProperty(vint index)=0;
				virtual Ptr<IEventHandler>		Attach(const Value& thisObject, Ptr<IValueFunctionProxy> handler)=0;
			};

/***********************************************************************
ITypeDescriptor (property)
***********************************************************************/

			class IPropertyInfo : public IMemberInfo
			{
			public:
				virtual bool					IsReadable()=0;
				virtual bool					IsWritable()=0;
				virtual ITypeInfo*				GetReturn()=0;
				virtual IMethodInfo*			GetGetter()=0;
				virtual IMethodInfo*			GetSetter()=0;
				virtual IEventInfo*				GetValueChangedEvent()=0;
				virtual Value					GetValue(const Value& thisObject)=0;
				virtual void					SetValue(Value& thisObject, const Value& newValue)=0;
			};

/***********************************************************************
ITypeDescriptor (method)
***********************************************************************/

			class IParameterInfo : public IMemberInfo
			{
			public:
				virtual ITypeInfo*				GetType()=0;
				virtual IMethodInfo*			GetOwnerMethod()=0;
			};

			class IMethodInfo : public IMemberInfo
			{
			public:
				virtual IMethodGroupInfo*		GetOwnerMethodGroup()=0;
				virtual IPropertyInfo*			GetOwnerProperty()=0;
				virtual vint					GetParameterCount()=0;
				virtual IParameterInfo*			GetParameter(vint index)=0;
				virtual ITypeInfo*				GetReturn()=0;
				virtual bool					IsStatic()=0;
				virtual void					CheckArguments(collections::Array<Value>& arguments)=0;
				virtual Value					Invoke(const Value& thisObject, collections::Array<Value>& arguments)=0;
			};

			class IMethodGroupInfo : public IMemberInfo
			{
			public:
				virtual vint					GetMethodCount()=0;
				virtual IMethodInfo*			GetMethod(vint index)=0;
			};

/***********************************************************************
ITypeDescriptor
***********************************************************************/

			class ITypeDescriptor : public virtual Interface
			{
			public:
				virtual const WString&			GetTypeName()=0;
				virtual IValueSerializer*		GetValueSerializer()=0;
				virtual vint					GetBaseTypeDescriptorCount()=0;
				virtual ITypeDescriptor*		GetBaseTypeDescriptor(vint index)=0;
				virtual bool					CanConvertTo(ITypeDescriptor* targetType)=0;

				virtual vint					GetPropertyCount()=0;
				virtual IPropertyInfo*			GetProperty(vint index)=0;
				virtual bool					IsPropertyExists(const WString& name, bool inheritable)=0;
				virtual IPropertyInfo*			GetPropertyByName(const WString& name, bool inheritable)=0;

				virtual vint					GetEventCount()=0;
				virtual IEventInfo*				GetEvent(vint index)=0;
				virtual bool					IsEventExists(const WString& name, bool inheritable)=0;
				virtual IEventInfo*				GetEventByName(const WString& name, bool inheritable)=0;

				virtual vint					GetMethodGroupCount()=0;
				virtual IMethodGroupInfo*		GetMethodGroup(vint index)=0;
				virtual bool					IsMethodGroupExists(const WString& name, bool inheritable)=0;
				virtual IMethodGroupInfo*		GetMethodGroupByName(const WString& name, bool inheritable)=0;
				virtual IMethodGroupInfo*		GetConstructorGroup()=0;
			};

/***********************************************************************
ITypeManager
***********************************************************************/

			class ITypeManager;

			class ITypeLoader : public virtual Interface
			{
			public:
				virtual void					Load(ITypeManager* manager)=0;
				virtual void					Unload(ITypeManager* manager)=0;
			};

			class ITypeManager : public virtual Interface
			{
			public:
				virtual vint					GetTypeDescriptorCount()=0;
				virtual ITypeDescriptor*		GetTypeDescriptor(vint index)=0;
				virtual ITypeDescriptor*		GetTypeDescriptor(const WString& name)=0;
				virtual bool					SetTypeDescriptor(const WString& name, Ptr<ITypeDescriptor> typeDescriptor)=0;

				virtual bool					AddTypeLoader(Ptr<ITypeLoader> typeLoader)=0;
				virtual bool					RemoveTypeLoader(Ptr<ITypeLoader> typeLoader)=0;
				virtual bool					Load()=0;
				virtual bool					Unload()=0;
				virtual bool					Reload()=0;
				virtual bool					IsLoaded()=0;
				virtual ITypeDescriptor*		GetRootType()=0;
			};

			extern ITypeManager*				GetGlobalTypeManager();
			extern bool							DestroyGlobalTypeManager();
			extern bool							ResetGlobalTypeManager();
			extern IValueSerializer*			GetValueSerializer(const WString& name);
			extern ITypeDescriptor*				GetTypeDescriptor(const WString& name);
			extern void							LogTypeManager(stream::TextWriter& writer);

/***********************************************************************
Exceptions
***********************************************************************/

			class TypeDescriptorException : public Exception
			{
			public:
				TypeDescriptorException(const WString& message)
					:Exception(message)
				{
				}
			};

			class TypeNotExistsException : public TypeDescriptorException
			{
			public:
				TypeNotExistsException(const WString& name)
					:TypeDescriptorException(L"Cannot find the type \""+name+L"\".")
				{
				}
			};

			class ConstructorNotExistsException : public TypeDescriptorException
			{
			public:
				ConstructorNotExistsException()
					:TypeDescriptorException(L"Cannot find any constructor.")
				{
				}
			};

			class MemberNotExistsException : public TypeDescriptorException
			{
			public:
				MemberNotExistsException(const WString& name)
					:TypeDescriptorException(L"Cannot find the member \""+name+L"\".")
				{
				}
			};

			class PropertyIsNotReadableException : public TypeDescriptorException
			{
			public:
				PropertyIsNotReadableException(IPropertyInfo* propertyInfo)
					:TypeDescriptorException(L"Cannot read value from a property \""+propertyInfo->GetName()+L"\" that is not readable in type \""+propertyInfo->GetOwnerTypeDescriptor()->GetTypeName()+L"\".")
				{
				}
			};

			class PropertyIsNotWritableException : public TypeDescriptorException
			{
			public:
				PropertyIsNotWritableException(IPropertyInfo* propertyInfo)
					:TypeDescriptorException(L"Cannot write value to a property \""+propertyInfo->GetName()+L"\" that is not writable in type \""+propertyInfo->GetOwnerTypeDescriptor()->GetTypeName()+L"\".")
				{
				}
			};

			class ArgumentNullException : public TypeDescriptorException
			{
			public:
				ArgumentNullException(const WString& name)
					:TypeDescriptorException(L"Argument \""+name+L"\" cannot be null.")
				{
				}
			};

			class ArgumentTypeMismtatchException : public TypeDescriptorException
			{
			public:
				ArgumentTypeMismtatchException(const WString& name, ITypeInfo* expected, const Value& actual)
					:TypeDescriptorException(L"Argument \""+name+L"\" cannot convert from \""+actual.GetTypeFriendlyName()+L"\" to \""+expected->GetTypeFriendlyName()+L"\".")
				{
				}

				ArgumentTypeMismtatchException(const WString& name, ITypeDescriptor* type, Value::ValueType valueType, const Value& actual)
					:TypeDescriptorException(L"Argument \""+name+L"\" cannot convert from \""+actual.GetTypeFriendlyName()+L"\" to \""+
						(valueType==Value::SharedPtr?L"Ptr<":L"")+type->GetTypeName()+(valueType==Value::SharedPtr?L">":valueType==Value::RawPtr?L"*":L"")
						+L"\".")
				{
				}
			};

			class ArgumentCountMismtatchException : public TypeDescriptorException
			{
			public:
				ArgumentCountMismtatchException()
					:TypeDescriptorException(L"Argument count does not match the definition.")
				{
				}
			};
		}
	}
}

#endif