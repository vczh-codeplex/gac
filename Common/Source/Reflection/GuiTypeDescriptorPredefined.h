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
TypedValueSerializer
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

/***********************************************************************
EnumValueSeriaizer
***********************************************************************/

			template<typename T>
			class EnumValueSeriaizer : public Object, public ITypedValueSerializer<T>
			{
			protected:
				ITypeDescriptor*							ownedTypeDescriptor;
				bool										canMergeCandidate;
				collections::Dictionary<WString, T>			candidates;

				bool Serialize(const T& input, WString& output)
				{
					if(canMergeCandidate)
					{
						return false;
					}
					else
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
				}

				bool Deserialize(const WString& input, T& output)
				{
					if(canMergeCandidate)
					{
						return false;
					}
					else
					{
						vint index=candidates.Keys().IndexOf(input);
						if(index==-1) return false;
						output=candidates.Values().Get(index);
						return true;
					}
				}
			public:
				typedef T EnumItemType;

				EnumValueSeriaizer(ITypeDescriptor* _ownedTypeDescriptor, bool _canMergeCandidate)
					:ownedTypeDescriptor(_ownedTypeDescriptor)
					,canMergeCandidate(_canMergeCandidate)
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
					return canMergeCandidate;
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

			template<typename T>
			class SerializableTypeDescriptor : public SerializableTypeDescriptorBase
			{
			public:
				SerializableTypeDescriptor()
					:SerializableTypeDescriptorBase(TypeInfo<T>::TypeName, 0)
				{
					serializer=new TypedValueSerializer<T>(this);
				}
			};

			template<typename TSerializer>
			class EnumTypeDescriptor : public SerializableTypeDescriptorBase
			{
			public:
				EnumTypeDescriptor()
					:SerializableTypeDescriptorBase(TypeInfo<typename TSerializer::EnumItemType>::TypeName, 0)
				{
					serializer=new TSerializer(this);
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