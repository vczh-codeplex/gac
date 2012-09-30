/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Framework::Reflection

XML Representation for Code Generation:
***********************************************************************/

#ifndef VCZH_PRESENTATION_REFLECTION_GUITYPEDESCRIPTOR
#define VCZH_PRESENTATION_REFLECTION_GUITYPEDESCRIPTOR

#include "..\Basic.h"
#include "..\Pointer.h"
#include "..\String.h"
#include "..\Function.h"
#include "..\Collections\Interfaces.h"

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
					DescriptableObjectRef,
					DescriptableObjectPtr,
					Text,
				};
			protected:
				ValueType						valueType;
				DescriptableObject*				descriptableObjectRef;
				Ptr<DescriptableObject>			descriptableObjectPtr;
				WString							text;
			public:
				Value();
				Value(DescriptableObject* value);
				Value(Ptr<DescriptableObject> value);
				Value(const WString& value);
				Value(const Value& value);

				Value&							operator=(const Value& value);

				ValueType						GetValueType()const;
				DescriptableObject*				GetDescriptableObjectRef()const;
				Ptr<DescriptableObject>			GetDescriptableObjectPtr()const;
				const WString&					GetText()const;
			};

			class IValueSerializer : public Interface
			{
			public:
				virtual WString					GetName()=0;
				virtual bool					Validate(const Value& value)=0;
			};

			template<typename T>
			class ITypedValueSerializer : public IValueSerializer
			{
				virtual bool					Serialize(const T& input, Value& value)=0;
				virtual bool					Deserialize(T& output, const Value& value)=0;
			};

/***********************************************************************
ITypeDescriptor (basic)
***********************************************************************/

			class IMemberInfo : public Interface
			{
			public:
				virtual ITypeDescriptor*		GetOwnerTypeDescriptor()=0;
				virtual const WString&			GetName()=0;
			};

			class IValueInfo : public Interface
			{
			public:
				virtual Value::ValueType		GetExpectedValueType()=0;
				virtual IValueSerializer*		GetExpectedValueSerializer()=0;
				virtual ITypeDescriptor*		GetExpectedTypeDescriptor()=0;
				virtual bool					CanBeNull()=0;
			};

/***********************************************************************
ITypeDescriptor (property)
***********************************************************************/

			class IPropertyInfo : public IMemberInfo, public IValueInfo
			{
			public:
				virtual Value					GetValue(const Value& thisObject)=0;
				virtual void					SetValue(const Value& thisObject, Value newValue)=0;
			};

/***********************************************************************
ITypeDescriptor (method)
***********************************************************************/

			class IMethodInfo;
			class IMethodGroupInfo;

			class IParameterInfo : public IMemberInfo, public IValueInfo
			{
			public:
				virtual IMethodInfo*			GetOwnerMethod()=0;
				virtual bool					CanOutput()=0;
			};

			class IMethodInfo : public IMemberInfo
			{
			public:
				virtual IMethodGroupInfo*		GetOwnerMethodGroup()=0;
				virtual int						GetParameterCount()=0;
				virtual IParameterInfo*			GetParameter(int index)=0;
				virtual IValueInfo*				GetReturn()=0;
				virtual Value					Invoke(const Value& thisObject, const collections::IReadonlyList<Value>& arguments)=0;
			};

			class IMethodGroupInfo : public IMemberInfo
			{
			public:
				virtual int						GetMethodCount()=0;
				virtual IMethodInfo*			GetMethod(int index)=0;
			};

/***********************************************************************
ITypeDescriptor (event)
***********************************************************************/

			class IEventInfo;

			class IEventHandler : public Interface
			{
			public:
				virtual IEventInfo*				GetOwnerEvent()=0;
				virtual Value					GetOwnerObject()=0;
				virtual bool					IsAttached()=0;
				virtual bool					Detach()=0;
			};

			class IEventInfo : public IMemberInfo
			{
			public:
				virtual Ptr<IEventHandler>		Attach(const Value& thisObject, const Func<void(const Value&, const Value&)>& handler)=0;
				virtual void					Invoke(const Value& thisObject, const Value& arguments)=0;
			};

/***********************************************************************
ITypeDescriptor
***********************************************************************/

			class ITypeDescriptor : public Interface
			{
			public:
				virtual int						GetBaseTypeDescriptorCount()=0;
				virtual ITypeDescriptor*		GetBaseTypeDescriptor(int index)=0;

				virtual int						GetPropertyCount()=0;
				virtual IPropertyInfo*			GetProperty(int index)=0;
				virtual bool					IsPropertyExists(const WString& name, bool inheritance)=0;
				virtual IPropertyInfo*			GetPropertyByName(const WString& name, bool inheritance)=0;

				virtual int						GetEventCount()=0;
				virtual IEventInfo*				GetEvent(int index)=0;
				virtual bool					IsEventExists(const WString& name, bool inheritance)=0;
				virtual IEventInfo*				GetEventByName(const WString& name, bool inheritance)=0;

				virtual int						GetMethodGroupCount()=0;
				virtual IMethodGroupInfo*		GetMethodGroup(int index)=0;
				virtual bool					IsMethodGroupExists(const WString& name, bool inheritance)=0;
				virtual IMethodGroupInfo*		GetMethodGroupByName(const WString& name, bool inheritance)=0;
				virtual IMethodGroupInfo*		GetConstructorGroup()=0;
			};
		}
	}
}

#endif