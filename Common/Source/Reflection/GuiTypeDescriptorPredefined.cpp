#include <limits.h>
#include <float.h>
#include "GuiTypeDescriptorPredefined.h"

namespace vl
{
	using namespace collections;

	namespace reflection
	{
		namespace description
		{

/***********************************************************************
SerializableTypeDescriptorBase
***********************************************************************/

			SerializableTypeDescriptorBase::SerializableTypeDescriptorBase(const WString& _typeName, Ptr<IValueSerializer> _serializer)
				:typeName(_typeName)
				,serializer(_serializer)
			{
			}

			SerializableTypeDescriptorBase::~SerializableTypeDescriptorBase()
			{
			}

			const WString& SerializableTypeDescriptorBase::GetTypeName()
			{
				return typeName;
			}

			IValueSerializer* SerializableTypeDescriptorBase::GetValueSerializer()
			{
				return serializer.Obj();
			}

			vint SerializableTypeDescriptorBase::GetBaseTypeDescriptorCount()
			{
				return 0;
			}

			ITypeDescriptor* SerializableTypeDescriptorBase::GetBaseTypeDescriptor(vint index)
			{
				return 0;
			}

			bool SerializableTypeDescriptorBase::CanConvertTo(ITypeDescriptor* targetType)
			{
				return this==targetType;
			}

			vint SerializableTypeDescriptorBase::GetPropertyCount()
			{
				return 0;
			}

			IPropertyInfo* SerializableTypeDescriptorBase::GetProperty(vint index)
			{
				return 0;
			}

			bool SerializableTypeDescriptorBase::IsPropertyExists(const WString& name, bool inheritable)
			{
				return false;
			}

			IPropertyInfo* SerializableTypeDescriptorBase::GetPropertyByName(const WString& name, bool inheritable)
			{
				return 0;
			}

			vint SerializableTypeDescriptorBase::GetEventCount()
			{
				return 0;
			}

			IEventInfo* SerializableTypeDescriptorBase::GetEvent(vint index)
			{
				return 0;
			}

			bool SerializableTypeDescriptorBase::IsEventExists(const WString& name, bool inheritable)
			{
				return false;
			}

			IEventInfo* SerializableTypeDescriptorBase::GetEventByName(const WString& name, bool inheritable)
			{
				return 0;
			}

			vint SerializableTypeDescriptorBase::GetMethodGroupCount()
			{
				return 0;
			}

			IMethodGroupInfo* SerializableTypeDescriptorBase::GetMethodGroup(vint index)
			{
				return 0;
			}

			bool SerializableTypeDescriptorBase::IsMethodGroupExists(const WString& name, bool inheritable)
			{
				return false;
			}

			IMethodGroupInfo* SerializableTypeDescriptorBase::GetMethodGroupByName(const WString& name, bool inheritable)
			{
				return 0;
			}

			IMethodGroupInfo* SerializableTypeDescriptorBase::GetConstructorGroup()
			{
				return 0;
			}

/***********************************************************************
TypeName
***********************************************************************/

			const wchar_t* TypeInfo<unsigned __int8>::TypeName	= L"unsigned __int8";
			const wchar_t* TypeInfo<unsigned __int16>::TypeName	= L"unsigned __int16";
			const wchar_t* TypeInfo<unsigned __int32>::TypeName	= L"unsigned __int32";
			const wchar_t* TypeInfo<unsigned __int64>::TypeName	= L"unsigned __int64";
			const wchar_t* TypeInfo<signed __int8>::TypeName	= L"signed __int8";
			const wchar_t* TypeInfo<signed __int16>::TypeName	= L"signed __int16";
			const wchar_t* TypeInfo<signed __int32>::TypeName	= L"signed __int32";
			const wchar_t* TypeInfo<signed __int64>::TypeName	= L"signed __int64";
			const wchar_t* TypeInfo<float>::TypeName			= L"float";
			const wchar_t* TypeInfo<double>::TypeName			= L"double";
			const wchar_t* TypeInfo<bool>::TypeName				= L"bool";
			const wchar_t* TypeInfo<WString>::TypeName			= L"WString";

/***********************************************************************
TypedValueSerializerProvider
***********************************************************************/

			bool TypedValueSerializerProvider<unsigned __int8>::Serialize(const unsigned __int8& input, WString& output)
			{
				output=u64tow(input);
				return true;
			}

			bool TypedValueSerializerProvider<unsigned __int8>::Deserialize(const WString& input, unsigned __int8& output)
			{
				bool success=false;
				unsigned __int64 result=wtou64_test(input, success);
				if(!success) return false;
				if(result>_UI8_MAX) return false;
				output=(unsigned __int8)result;
				return true;
			}

			bool TypedValueSerializerProvider<unsigned __int16>::Serialize(const unsigned __int16& input, WString& output)
			{
				output=u64tow(input);
				return true;
			}

			bool TypedValueSerializerProvider<unsigned __int16>::Deserialize(const WString& input, unsigned __int16& output)
			{
				bool success=false;
				unsigned __int64 result=wtou64_test(input, success);
				if(!success) return false;
				if(result>_UI16_MAX) return false;
				output=(unsigned __int16)result;
				return true;
			}

			bool TypedValueSerializerProvider<unsigned __int32>::Serialize(const unsigned __int32& input, WString& output)
			{
				output=u64tow(input);
				return true;
			}

			bool TypedValueSerializerProvider<unsigned __int32>::Deserialize(const WString& input, unsigned __int32& output)
			{
				bool success=false;
				unsigned __int64 result=wtou64_test(input, success);
				if(!success) return false;
				if(result>_UI32_MAX) return false;
				output=(unsigned __int32)result;
				return true;
			}

			bool TypedValueSerializerProvider<unsigned __int64>::Serialize(const unsigned __int64& input, WString& output)
			{
				output=u64tow(input);
				return true;
			}

			bool TypedValueSerializerProvider<unsigned __int64>::Deserialize(const WString& input, unsigned __int64& output)
			{
				bool success=false;
				unsigned __int64 result=wtou64_test(input, success);
				if(!success) return false;
				output=result;
				return true;
			}

			bool TypedValueSerializerProvider<signed __int8>::Serialize(const signed __int8& input, WString& output)
			{
				output=i64tow(input);
				return true;
			}

			bool TypedValueSerializerProvider<signed __int8>::Deserialize(const WString& input, signed __int8& output)
			{
				bool success=false;
				signed __int64 result=wtoi64_test(input, success);
				if(!success) return false;
				if(result<_I8_MIN || result>_I8_MAX) return false;
				output=(signed __int8)result;
				return true;
			}

			bool TypedValueSerializerProvider<signed __int16>::Serialize(const signed __int16& input, WString& output)
			{
				output=i64tow(input);
				return true;
			}

			bool TypedValueSerializerProvider<signed __int16>::Deserialize(const WString& input, signed __int16& output)
			{
				bool success=false;
				signed __int64 result=wtoi64_test(input, success);
				if(!success) return false;
				if(result<_I16_MIN || result>_I16_MAX) return false;
				output=(signed __int16)result;
				return true;
			}

			bool TypedValueSerializerProvider<signed __int32>::Serialize(const signed __int32& input, WString& output)
			{
				output=i64tow(input);
				return true;
			}

			bool TypedValueSerializerProvider<signed __int32>::Deserialize(const WString& input, signed __int32& output)
			{
				bool success=false;
				signed __int64 result=wtoi64_test(input, success);
				if(!success) return false;
				if(result<_I32_MIN || result>_I32_MAX) return false;
				output=(signed __int32)result;
				return true;
			}

			bool TypedValueSerializerProvider<signed __int64>::Serialize(const signed __int64& input, WString& output)
			{
				output=i64tow(input);
				return true;
			}

			bool TypedValueSerializerProvider<signed __int64>::Deserialize(const WString& input, signed __int64& output)
			{
				bool success=false;
				signed __int64 result=wtoi64_test(input, success);
				if(!success) return false;
				output=result;
				return true;
			}

			bool TypedValueSerializerProvider<float>::Serialize(const float& input, WString& output)
			{
				output=ftow(input);
				return true;
			}

			bool TypedValueSerializerProvider<float>::Deserialize(const WString& input, float& output)
			{
				bool success=false;
				double result=wtof_test(input, success);
				if(!success) return false;
				if(result<-FLT_MAX  || result>FLT_MAX) return false;
				output=(float)result;
				return true;
			}

			bool TypedValueSerializerProvider<double>::Serialize(const double& input, WString& output)
			{
				output=ftow(input);
				return true;
			}

			bool TypedValueSerializerProvider<double>::Deserialize(const WString& input, double& output)
			{
				bool success=false;
				double result=wtof_test(input, success);
				if(!success) return false;
				output=result;
				return true;
			}

			bool TypedValueSerializerProvider<WString>::Serialize(const WString& input, WString& output)
			{
				output=input;
				return true;
			}

			bool TypedValueSerializerProvider<WString>::Deserialize(const WString& input, WString& output)
			{
				output=input;
				return true;
			}

/***********************************************************************
BoolValueSerializer
***********************************************************************/

			class BoolValueSeriaizer : public EnumValueSeriaizer<bool, false>
			{
			public:
				BoolValueSeriaizer(ITypeDescriptor* _ownerTypeDescriptor)
					:EnumValueSeriaizer(_ownerTypeDescriptor)
				{
					candidates.Add(L"true", true);
					candidates.Add(L"false", false);
				}
			};

/***********************************************************************
LoadPredefinedTypes
***********************************************************************/

			class PredefinedTypeLoader : public Object, public ITypeLoader
			{
			public:
				template<typename TSerializer>
				void AddSerializableType(ITypeManager* manager)
				{
					manager->SetTypeDescriptor(TypeInfo<TSerializer::ValueType>::TypeName, new SerializableTypeDescriptor<TSerializer>);
				}

				void Load(ITypeManager* manager)override
				{
					AddSerializableType<TypedValueSerializer<unsigned __int8>>(manager);
					AddSerializableType<TypedValueSerializer<unsigned __int16>>(manager);
					AddSerializableType<TypedValueSerializer<unsigned __int32>>(manager);
					AddSerializableType<TypedValueSerializer<unsigned __int64>>(manager);
					AddSerializableType<TypedValueSerializer<signed __int8>>(manager);
					AddSerializableType<TypedValueSerializer<signed __int16>>(manager);
					AddSerializableType<TypedValueSerializer<signed __int32>>(manager);
					AddSerializableType<TypedValueSerializer<signed __int64>>(manager);
					AddSerializableType<TypedValueSerializer<float>>(manager);
					AddSerializableType<TypedValueSerializer<double>>(manager);
					AddSerializableType<BoolValueSeriaizer>(manager);
					AddSerializableType<TypedValueSerializer<WString>>(manager);
				}

				void Unload(ITypeManager* manager)override
				{
				}
			};

			bool LoadPredefinedTypes()
			{
				ITypeManager* manager=GetGlobalTypeManager();
				if(manager)
				{
					Ptr<ITypeLoader> loader=new PredefinedTypeLoader;
					return manager->AddTypeLoader(loader);
				}
				return false;
			}
		}
	}
}