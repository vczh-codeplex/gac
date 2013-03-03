/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
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
GeneralValueSeriaizer
***********************************************************************/

			template<typename T>
			class GeneralValueSeriaizer : public Object, public ITypedValueSerializer<T>
			{
			protected:
				ITypeDescriptor*							ownedTypeDescriptor;

				virtual bool								Serialize(const T& input, WString& output)=0;
				virtual bool								Deserialize(const WString& input, T& output)=0;
			public:
				typedef T ValueType;

				GeneralValueSeriaizer(ITypeDescriptor* _ownedTypeDescriptor)
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
					return Deserialize(text, output);
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

				bool HasCandidate()override
				{
					return false;
				}

				vint GetCandidateCount()override
				{
					return 0;
				}

				WString GetCandidate(vint index)override
				{
					return L"";
				}

				bool CanMergeCandidate()override
				{
					return false;
				}

				bool Serialize(const T& input, Value& output)
				{
					WString text;
					if(Serialize(input, text))
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
					return Deserialize(input.GetText(), output);
				}
			};

/***********************************************************************
TypedValueSerializer
***********************************************************************/

			template<typename T>
			struct TypedValueSerializerProvider
			{
			};

			template<typename T>
			class TypedValueSerializer : public GeneralValueSeriaizer<T>
			{
			protected:
				bool Serialize(const T& input, WString& output)override
				{
					return TypedValueSerializerProvider<T>::Serialize(input, output);
				}

				bool Deserialize(const WString& input, T& output)override
				{
					return TypedValueSerializerProvider<T>::Deserialize(input, output);
				}
			public:
				TypedValueSerializer(ITypeDescriptor* _ownedTypeDescriptor)
					:GeneralValueSeriaizer(_ownedTypeDescriptor)
				{
				}
			};

/***********************************************************************
EnumValueSeriaizer
***********************************************************************/

			template<typename T, bool CanMerge>
			struct EnumValueSerializerProvider
			{
			};

			template<typename T>
			struct EnumValueSerializerProvider<T, true>
			{
				static bool Serialize(collections::Dictionary<WString, T>& candidates, const T& input, WString& output)
				{
					WString result;
					for(vint i=0;i<candidates.Count();i++)
					{
						if(candidates.Values().Get(i)&input)
						{
							if(result!=L"") result+=L"|";
							result+=candidates.Keys()[i];
						}
					}
					output=result;
					return true;
				}

				static bool Deserialize(collections::Dictionary<WString, T>& candidates, const WString& input, T& output)
				{
					T result=(T)0;
					const wchar_t* reading=input.Buffer();
					while(*reading)
					{
						const wchar_t* sep=wcschr(reading, L'|');
						if(!sep) sep=reading+wcslen(reading);
						WString item(reading, sep-reading);
						reading=*sep?sep+1:sep;

						vint index=candidates.Keys().IndexOf(item);
						if(index==-1) return false;
						result=(T)(result|candidates.Values().Get(index));
					}
					output=result;
					return true;
				}
			};

			template<typename T>
			struct EnumValueSerializerProvider<T, false>
			{
				static bool Serialize(collections::Dictionary<WString, T>& candidates, const T& input, WString& output)
				{
					for(vint i=0;i<candidates.Count();i++)
					{
						if(candidates.Values().Get(i)==input)
						{
							output=candidates.Keys()[i];
							return true;
						}
					}
					return false;
				}

				static bool Deserialize(collections::Dictionary<WString, T>& candidates, const WString& input, T& output)
				{
					vint index=candidates.Keys().IndexOf(input);
					if(index==-1) return false;
					output=candidates.Values().Get(index);
					return true;
				}
			};

			template<typename T, bool CanMerge>
			class EnumValueSeriaizer : public GeneralValueSeriaizer<T>
			{
			protected:
				collections::Dictionary<WString, T>			candidates;

				bool Serialize(const T& input, WString& output)override
				{
					return EnumValueSerializerProvider<T, CanMerge>::Serialize(candidates, input, output);
				}

				bool Deserialize(const WString& input, T& output)override
				{
					return EnumValueSerializerProvider<T, CanMerge>::Deserialize(candidates, input, output);
				}
			public:
				EnumValueSeriaizer(ITypeDescriptor* _ownedTypeDescriptor)
					:GeneralValueSeriaizer(_ownedTypeDescriptor)
				{
				}

				bool HasCandidate()override
				{
					return true;
				}

				vint GetCandidateCount()override
				{
					return candidates.Count();
				}

				WString GetCandidate(vint index)override
				{
					return candidates.Keys()[index];
				}

				bool CanMergeCandidate()override
				{
					return CanMerge;
				}
			};

/***********************************************************************
SerializableTypeDescriptor
***********************************************************************/

			class SerializableTypeDescriptorBase : public Object, public ITypeDescriptor
			{
			protected:
				Ptr<IValueSerializer>						serializer;
				WString										typeName;
			public:
				SerializableTypeDescriptorBase(const WString& _typeName, Ptr<IValueSerializer> _serializer);
				~SerializableTypeDescriptorBase();

				const WString&								GetTypeName()override;
				IValueSerializer*							GetValueSerializer()override;
				vint										GetBaseTypeDescriptorCount()override;
				ITypeDescriptor*							GetBaseTypeDescriptor(vint index)override;
				bool										CanConvertTo(ITypeDescriptor* targetType)override;
				vint										GetPropertyCount()override;
				IPropertyInfo*								GetProperty(vint index)override;
				bool										IsPropertyExists(const WString& name, bool inheritable)override;
				IPropertyInfo*								GetPropertyByName(const WString& name, bool inheritable)override;
				vint										GetEventCount()override;
				IEventInfo*									GetEvent(vint index)override;
				bool										IsEventExists(const WString& name, bool inheritable)override;
				IEventInfo*									GetEventByName(const WString& name, bool inheritable)override;
				vint										GetMethodGroupCount()override;
				IMethodGroupInfo*							GetMethodGroup(vint index)override;
				bool										IsMethodGroupExists(const WString& name, bool inheritable)override;
				IMethodGroupInfo*							GetMethodGroupByName(const WString& name, bool inheritable)override;
				IMethodGroupInfo*							GetConstructorGroup()override;
			};

			template<typename TSerializer>
			class SerializableTypeDescriptor : public SerializableTypeDescriptorBase
			{
			public:
				SerializableTypeDescriptor()
					:SerializableTypeDescriptorBase(TypeInfo<typename TSerializer::ValueType>::TypeName, 0)
				{
					serializer=new TSerializer(this);
				}
			};

/***********************************************************************
Predefined Types
***********************************************************************/

			template<>struct TypeInfo<Value>{static const wchar_t* TypeName;};
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
			template<>struct TypeInfo<wchar_t>{static const wchar_t* TypeName;};
			template<>struct TypeInfo<WString>{static const wchar_t* TypeName;};
			template<>struct TypeInfo<IValueReadonlyList>{static const wchar_t* TypeName;};
			template<>struct TypeInfo<IValueList>{static const wchar_t* TypeName;};

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
			struct TypedValueSerializerProvider<wchar_t>
			{
				static bool Serialize(const wchar_t& input, WString& output);
				static bool Deserialize(const WString& input, wchar_t& output);
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