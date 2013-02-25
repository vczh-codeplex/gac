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
		protected:
			size_t									objectSize;
			description::ITypeDescriptor**			typeDescriptor;
		public:
			DescriptableObject();
			virtual ~DescriptableObject();

			description::ITypeDescriptor*			GetTypeDescriptor();
		};

		class IDescriptable : public virtual Interface, public virtual DescriptableObject
		{
		public:
			~IDescriptable(){}
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
					typeDescriptor=&associatedTypeDescriptor;
				}
			}

			static description::ITypeDescriptor* GetAssociatedTypeDescriptor()
			{
				return associatedTypeDescriptor;
			}

			static void SetAssociatedTypeDescroptor(description::ITypeDescriptor* typeDescroptor)
			{
				if(!associatedTypeDescriptor)
				{
					associatedTypeDescriptor=typeDescroptor;
				}
			}
		};

		template<typename T>
		description::ITypeDescriptor* Description<T>::associatedTypeDescriptor=0;

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

				ValueType						GetValueType()const;
				DescriptableObject*				GetRawPtr()const;
				Ptr<DescriptableObject>			GetSharedPtr()const;
				const WString&					GetText()const;
				ITypeDescriptor*				GetTypeDescriptor()const;
				WString							GetTypeFriendlyName()const;
				bool							IsNull()const;
				bool							CanConvertTo(ITypeDescriptor* targetType, ValueType targetValueType)const;
				bool							CanConvertTo(IParameterInfo* targetType)const;

				static Value					From(DescriptableObject* value);
				static Value					From(Ptr<DescriptableObject> value);
				static Value					From(const WString& value, ITypeDescriptor* type);
			};

			class IValueSerializer : public Interface
			{
			public:
				virtual ITypeDescriptor*		GetOwnerTypeDescriptor()=0;
				virtual bool					Validate(const WString& text)=0;
				virtual bool					Parse(const WString& input, Value& output)=0;
			};

			template<typename T>
			class ITypedValueSerializer : public IValueSerializer
			{
			public:
				virtual bool					Serialize(const T& input, Value& output)=0;
				virtual bool					Deserialize(const Value& input, T& output)=0;
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

			class IEventHandler : public Interface
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
				virtual IPropertyInfo*			GetObservingProperty()=0;
				virtual Ptr<IEventHandler>		Attach(const Value& thisObject, const Func<void(const Value&, Value&)>& handler)=0;
			};

/***********************************************************************
ITypeDescriptor (property)
***********************************************************************/

			class IPropertyInfo : public IMemberInfo
			{
			public:
				virtual ITypeDescriptor*		GetValueTypeDescriptor()=0;
				virtual bool					IsReadable()=0;
				virtual bool					IsWritable()=0;
				virtual IMethodInfo*			GetGetter()=0;
				virtual IMethodInfo*			GetSetter()=0;
				virtual IEventInfo*				GetValueChangedEvent()=0;
				virtual Value					GetValue(const Value& thisObject)=0;
				virtual void					SetValue(const Value& thisObject, const Value& newValue)=0;
			};

/***********************************************************************
ITypeDescriptor (method)
***********************************************************************/

			class IParameterInfo : public IMemberInfo
			{
			public:
				enum Decorator
				{
					RawPtr,
					SharedPtr,
					Text,
				};

				virtual ITypeDescriptor*		GetValueTypeDescriptor()=0;
				virtual IMethodInfo*			GetOwnerMethod()=0;
				virtual Decorator				GetDecorator()=0;
				virtual bool					CanOutput()=0;
				virtual WString					GetTypeFriendlyName()=0;
			};

			class IMethodInfo : public IMemberInfo
			{
			public:
				virtual IMethodGroupInfo*		GetOwnerMethodGroup()=0;
				virtual IPropertyInfo*			GetOwnerProperty()=0;
				virtual vint					GetParameterCount()=0;
				virtual IParameterInfo*			GetParameter(vint index)=0;
				virtual IParameterInfo*			GetReturn()=0;
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

			class ITypeDescriptor : public Interface
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

			class ITypeLoader : public Interface
			{
			public:
				virtual void					Load(ITypeManager* manager)=0;
				virtual void					Unload(ITypeManager* manager)=0;
			};

			class ITypeManager : public Interface
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
			};

			extern ITypeManager*				GetGlobalTypeManager();
			extern bool							DestroyGlobalTypeManager();
			extern IValueSerializer*			GetValueSerializer(const WString& name);
			extern ITypeDescriptor*				GetTypeDescriptor(const WString& name);

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

			class PropertyIsNotReadableException : public TypeDescriptorException
			{
			public:
				PropertyIsNotReadableException(IPropertyInfo* propertyInfo)
					:TypeDescriptorException(L"Cannot read value from a property \""+propertyInfo->GetName()+L"\" that is not readable in type \""+propertyInfo->GetOwnerTypeDescriptor()->GetTypeName()+L"\"/")
				{
				}
			};

			class PropertyIsNotWritableException : public TypeDescriptorException
			{
			public:
				PropertyIsNotWritableException(IPropertyInfo* propertyInfo)
					:TypeDescriptorException(L"Cannot write value to a property \""+propertyInfo->GetName()+L"\" that is not writable in type \""+propertyInfo->GetOwnerTypeDescriptor()->GetTypeName()+L"\"/")
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
				ArgumentTypeMismtatchException(const WString& name, IParameterInfo* expected, const Value& actual)
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