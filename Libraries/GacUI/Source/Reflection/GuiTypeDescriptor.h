/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Reflection

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_REFLECTION_GUITYPEDESCRIPTOR
#define VCZH_PRESENTATION_REFLECTION_GUITYPEDESCRIPTOR

#include "..\NativeWindow\GuiTypes.h"

namespace vl
{
	namespace presentation
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

			static void SetTypeDescroptor(description::ITypeDescriptor* typeDescroptor)
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
ITypeDescriptor
***********************************************************************/

			class IMethodInfo;
			class IMethodGroupInfo;

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

			class IPropertyInfo : public IMemberInfo, public IValueInfo
			{
			public:
				virtual Value					GetValue(Value thisObject)=0;
				virtual void					SetValue(Value thisObject, Value newValue)=0;
			};

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
				virtual Value					Invoke(Value thisObject, const collections::IReadonlyList<Value>& arguments)=0;
			};

			class IMethodGroupInfo : public IMemberInfo
			{
			public:
				virtual const WString&			GetName()=0;
				virtual int						GetMethodCount()=0;
				virtual IMethodInfo*			GetMethod(int index)=0;
			};

			class ITypeDescriptor : public Interface
			{
			public:
				virtual int						GetBaseTypeDescriptorCount()=0;
				virtual ITypeDescriptor*		GetBaseTypeDescriptor(int index)=0;

				virtual int						GetPropertyCount()=0;
				virtual IPropertyInfo*			GetProperty(int index)=0;
				virtual bool					IsPropertyExists(const WString& name, bool inheritance)=0;
				virtual IPropertyInfo*			GetPropertyByName(const WString& name, bool inheritance)=0;

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