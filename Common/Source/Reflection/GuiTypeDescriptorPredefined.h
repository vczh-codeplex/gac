/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Framework::Reflection

XML Representation for Code Generation:
***********************************************************************/

#ifndef VCZH_REFLECTION_GUITYPEDESCRIPTORPREDEFINED
#define VCZH_REFLECTION_GUITYPEDESCRIPTORPREDEFINED

#include "GuiTypeDescriptor.h"

namespace vl
{
	namespace reflection
	{
		namespace description
		{

/***********************************************************************
TypeInfo
***********************************************************************/

			template<typename T>
			struct TypeInfo
			{
			};

			template<typename T>
			ITypedValueSerializer<T>* GetValueSerializer()
			{
				return dynamic_cast<ITypedValueSerializer<T>*>(GetValueSerializer(TypeInfo<T>::TypeName));
			}

			template<typename T>
			ITypeDescriptor* GetTypeDescriptor()
			{
				return GetTypeDescriptor(TypeInfo<T>::TypeName);
			}

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
SerializableTypeDescriptor
***********************************************************************/

			template<typename T>
			struct TypedValueSerializerProvider
			{
			};

			template<typename T>
			class TypedValueSerializer : public Object, public ITypedValueSerializer<T>
			{
			protected:
				ITypeDescriptor*							ownedTypeDescriptor;
			public:
				TypedValueSerializer(ITypeDescriptor* _ownedTypeDescriptor)
					:ownedTypeDescriptor(_ownedTypeDescriptor)
				{
				}

				ITypeDescriptor* GetOwnerTypeDescriptor()
				{
					return ownedTypeDescriptor;
				}

				bool Validate(const WString& text)
				{
					T output;
					return TypedValueSerializerProvider<T>::Deserialize(text, output);
				}

				bool Parse(const WString& input, Value& output)
				{
					if(Validate(input))
					{
						output=Value::From(input, ownedTypeDescriptor);
						return true;
					}
					return false;
				}

				bool Serialize(const T& input, Value& output)
				{
					WString text;
					if(TypedValueSerializerProvider<T>::Serialize(input, text))
					{
						output=Value::From(text, ownedTypeDescriptor);
						return true;
					}
					return false;
				}

				bool Deserialize(const Value& input, T& output)
				{
					if(input.GetValueType()!=Value::Text)
					{
						return false;
					}
					return TypedValueSerializerProvider<T>::Deserialize(input.GetText(), output);
				}
			};

			template<typename T>
			class SerializableTypeDescriptor : public Object, public ITypeDescriptor
			{
			protected:
				Ptr<IValueSerializer>						serializer;
				WString										typeName;
			public:
				SerializableTypeDescriptor()
					:typeName(TypeInfo<T>::TypeName)
				{
					serializer=new TypedValueSerializer<T>(this);
				}

				const WString& GetTypeName()override
				{
					return typeName;
				}

				IValueSerializer* GetValueSerializer()override
				{
					return serializer.Obj();
				}

				vint GetBaseTypeDescriptorCount()override
				{
					return 0;
				}

				ITypeDescriptor* GetBaseTypeDescriptor(vint index)override
				{
					return 0;
				}

				vint GetPropertyCount()override
				{
					return 0;
				}

				IPropertyInfo* GetProperty(vint index)override
				{
					return 0;
				}

				bool IsPropertyExists(const WString& name, bool inheritable)override
				{
					return false;
				}

				IPropertyInfo* GetPropertyByName(const WString& name, bool inheritable)override
				{
					return 0;
				}

				vint GetEventCount()override
				{
					return 0;
				}

				IEventInfo* GetEvent(vint index)override
				{
					return 0;
				}

				bool IsEventExists(const WString& name, bool inheritable)override
				{
					return false;
				}

				IEventInfo* GetEventByName(const WString& name, bool inheritable)override
				{
					return 0;
				}

				vint GetMethodGroupCount()override
				{
					return 0;
				}

				IMethodGroupInfo* GetMethodGroup(vint index)override
				{
					return 0;
				}

				bool IsMethodGroupExists(const WString& name, bool inheritable)override
				{
					return false;
				}

				IMethodGroupInfo* GetMethodGroupByName(const WString& name, bool inheritable)override
				{
					return 0;
				}

				IMethodGroupInfo* GetConstructorGroup()override
				{
					return 0;
				}
			};

/***********************************************************************
Predefined Types
***********************************************************************/

			template<>struct TypeInfo<unsigned __int8>{static const wchar_t* TypeName;};
			template<>struct TypeInfo<unsigned __int16>{static const wchar_t* TypeName;};
			template<>struct TypeInfo<unsigned __int32>{static const wchar_t* TypeName;};
			template<>struct TypeInfo<unsigned __int64>{static const wchar_t* TypeName;};
			template<>struct TypeInfo<signed __int8>{static const wchar_t* TypeName;};
			template<>struct TypeInfo<signed __int16>{static const wchar_t* TypeName;};
			template<>struct TypeInfo<signed __int32>{static const wchar_t* TypeName;};
			template<>struct TypeInfo<signed __int64>{static const wchar_t* TypeName;};
			template<>struct TypeInfo<float>{static const wchar_t* TypeName;};
			template<>struct TypeInfo<double>{static const wchar_t* TypeName;};
			template<>struct TypeInfo<bool>{static const wchar_t* TypeName;};
			template<>struct TypeInfo<WString>{static const wchar_t* TypeName;};

			template<>
			struct TypedValueSerializerProvider<unsigned __int8>
			{
				static bool Serialize(const unsigned __int8& input, WString& output);
				static bool Deserialize(const WString& input, unsigned __int8& output);
			};

			template<>
			struct TypedValueSerializerProvider<unsigned __int16>
			{
				static bool Serialize(const unsigned __int16& input, WString& output);
				static bool Deserialize(const WString& input, unsigned __int16& output);
			};

			template<>
			struct TypedValueSerializerProvider<unsigned __int32>
			{
				static bool Serialize(const unsigned __int32& input, WString& output);
				static bool Deserialize(const WString& input, unsigned __int32& output);
			};

			template<>
			struct TypedValueSerializerProvider<unsigned __int64>
			{
				static bool Serialize(const unsigned __int64& input, WString& output);
				static bool Deserialize(const WString& input, unsigned __int64& output);
			};

			template<>
			struct TypedValueSerializerProvider<signed __int8>
			{
				static bool Serialize(const signed __int8& input, WString& output);
				static bool Deserialize(const WString& input, signed __int8& output);
			};

			template<>
			struct TypedValueSerializerProvider<signed __int16>
			{
				static bool Serialize(const signed __int16& input, WString& output);
				static bool Deserialize(const WString& input, signed __int16& output);
			};

			template<>
			struct TypedValueSerializerProvider<signed __int32>
			{
				static bool Serialize(const signed __int32& input, WString& output);
				static bool Deserialize(const WString& input, signed __int32& output);
			};

			template<>
			struct TypedValueSerializerProvider<signed __int64>
			{
				static bool Serialize(const signed __int64& input, WString& output);
				static bool Deserialize(const WString& input, signed __int64& output);
			};

			template<>
			struct TypedValueSerializerProvider<float>
			{
				static bool Serialize(const float& input, WString& output);
				static bool Deserialize(const WString& input, float& output);
			};

			template<>
			struct TypedValueSerializerProvider<double>
			{
				static bool Serialize(const double& input, WString& output);
				static bool Deserialize(const WString& input, double& output);
			};

			template<>
			struct TypedValueSerializerProvider<bool>
			{
				static bool Serialize(const bool& input, WString& output);
				static bool Deserialize(const WString& input, bool& output);
			};

			template<>
			struct TypedValueSerializerProvider<WString>
			{
				static bool Serialize(const WString& input, WString& output);
				static bool Deserialize(const WString& input, WString& output);
			};

/***********************************************************************
LoadPredefinedTypes
***********************************************************************/

			extern bool										LoadPredefinedTypes();
		}
	}
}

#endif