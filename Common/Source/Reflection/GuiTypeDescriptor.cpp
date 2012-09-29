#include "GuiTypeDescriptor.h"

namespace vl
{
	namespace reflection
	{

/***********************************************************************
DescriptableObject
***********************************************************************/

		DescriptableObject::DescriptableObject()
			:objectSize(0)
			,typeDescriptor(0)
		{
		}

		DescriptableObject::~DescriptableObject()
		{
		}

		description::ITypeDescriptor* DescriptableObject::GetTypeDescriptor()
		{
			return typeDescriptor?*typeDescriptor:0;
		}

/***********************************************************************
DescriptableObject
***********************************************************************/

		namespace description
		{
			Value::Value()
				:valueType(Null)
				,descriptableObjectRef(0)
			{
			}

			Value::Value(DescriptableObject* value)
				:valueType(DescriptableObjectRef)
				,descriptableObjectRef(value)
			{
			}

			Value::Value(Ptr<DescriptableObject> value)
				:valueType(DescriptableObjectPtr)
				,descriptableObjectRef(value.Obj())
				,descriptableObjectPtr(value)
			{
			}

			Value::Value(const WString& value)
				:valueType(Null)
				,descriptableObjectRef(0)
				,text(value)
			{
			}

			Value::Value(const Value& value)
				:valueType(value.valueType)
				,descriptableObjectRef(value.descriptableObjectRef)
				,descriptableObjectPtr(value.descriptableObjectPtr)
				,text(value.text)
			{
			}

			Value& Value::operator=(const Value& value)
			{
				valueType=value.valueType;
				descriptableObjectRef=value.descriptableObjectRef;
				descriptableObjectPtr=value.descriptableObjectPtr;
				text=value.text;
				return *this;
			}

			Value::ValueType Value::GetValueType()const
			{
				return valueType;
			}

			DescriptableObject* Value::GetDescriptableObjectRef()const
			{
				return descriptableObjectRef;
			}

			Ptr<DescriptableObject> Value::GetDescriptableObjectPtr()const
			{
				return descriptableObjectPtr;
			}

			const WString& Value::GetText()const
			{
				return text;
			}
		}
	}
}