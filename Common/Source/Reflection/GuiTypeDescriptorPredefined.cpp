#include <limits.h>
#include <float.h>
#include "GuiTypeDescriptorMacros.h"
#include "../Regex/Regex.h"

#ifdef VCZH_GCC
#define _I8_MIN     -128
#define _I8_MAX     127
#define _UI8_MAX    255

#define _I16_MIN    -32768
#define _I16_MAX    32767
#define _UI16_MAX   65535

#define _I32_MIN    -2147483648
#define _I32_MAX    2147483647
#define _UI32_MAX   4194967295
#endif

namespace vl
{
	using namespace collections;
	using namespace regex;

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
			
			const wchar_t* TypeInfo<void>::TypeName						= L"system::Void";
			const wchar_t* TypeInfo<VoidValue>::TypeName				= L"system::Void";
			const wchar_t* TypeInfo<IDescriptable>::TypeName			= L"system::Interface";
			const wchar_t* TypeInfo<DescriptableObject>::TypeName		= L"system::ReferenceType";
			const wchar_t* TypeInfo<Value>::TypeName					= L"system::Object";
			const wchar_t* TypeInfo<vuint8_t>::TypeName					= L"system::UInt8";
			const wchar_t* TypeInfo<vuint16_t>::TypeName				= L"system::UInt16";
			const wchar_t* TypeInfo<vuint32_t>::TypeName				= L"system::UInt32";
			const wchar_t* TypeInfo<vuint64_t>::TypeName				= L"system::UInt64";
			const wchar_t* TypeInfo<vint8_t>::TypeName					= L"system::Int8";
			const wchar_t* TypeInfo<vint16_t>::TypeName					= L"system::Int16";
			const wchar_t* TypeInfo<vint32_t>::TypeName					= L"system::Int32";
			const wchar_t* TypeInfo<vint64_t>::TypeName					= L"system::Int64";
			const wchar_t* TypeInfo<float>::TypeName					= L"system::Single";
			const wchar_t* TypeInfo<double>::TypeName					= L"system::Double";
			const wchar_t* TypeInfo<bool>::TypeName						= L"system::Boolean";
			const wchar_t* TypeInfo<wchar_t>::TypeName					= L"system::Char";
			const wchar_t* TypeInfo<WString>::TypeName					= L"system::String";
			const wchar_t* TypeInfo<DateTime>::TypeName					= L"system::DateTime";
			const wchar_t* TypeInfo<Locale>::TypeName					= L"system::Locale";

			const wchar_t* TypeInfo<IValueEnumerator>::TypeName			= L"system::Enumerator";
			const wchar_t* TypeInfo<IValueEnumerable>::TypeName			= L"system::Enumerable";
			const wchar_t* TypeInfo<IValueReadonlyList>::TypeName		= L"system::ReadonlyList";
			const wchar_t* TypeInfo<IValueList>::TypeName				= L"system::List";
			const wchar_t* TypeInfo<IValueReadonlyDictionary>::TypeName	= L"system::ReadonlyDictionary";
			const wchar_t* TypeInfo<IValueDictionary>::TypeName			= L"system::Dictionary";
			const wchar_t* TypeInfo<IValueInterfaceProxy>::TypeName		= L"system::InterfaceProxy";
			const wchar_t* TypeInfo<IValueFunctionProxy>::TypeName		= L"system::Function";
			const wchar_t* TypeInfo<IValueListener>::TypeName			= L"system::Listener";
			const wchar_t* TypeInfo<IValueSubscription>::TypeName		= L"system::Subscription";

			const wchar_t* TypeInfo<IValueSerializer>::TypeName			= L"system::reflection::ValueSerializer";
			const wchar_t* TypeInfo<ITypeInfo>::TypeName				= L"system::reflection::TypeInfo";
			const wchar_t* TypeInfo<ITypeInfo::Decorator>::TypeName		= L"system::reflection::TypeInfo::Decorator";
			const wchar_t* TypeInfo<IMemberInfo>::TypeName				= L"system::reflection::MemberInfo";
			const wchar_t* TypeInfo<IEventHandler>::TypeName			= L"system::reflection::EventHandler";
			const wchar_t* TypeInfo<IEventInfo>::TypeName				= L"system::reflection::EventInfo";
			const wchar_t* TypeInfo<IPropertyInfo>::TypeName			= L"system::reflection::PropertyInfo";
			const wchar_t* TypeInfo<IParameterInfo>::TypeName			= L"system::reflection::ParameterInfo";
			const wchar_t* TypeInfo<IMethodInfo>::TypeName				= L"system::reflection::MethodInfo";
			const wchar_t* TypeInfo<IMethodGroupInfo>::TypeName			= L"system::reflection::MethodGroupInfo";
			const wchar_t* TypeInfo<ITypeDescriptor>::TypeName			= L"system::reflection::TypeDescriptor";

/***********************************************************************
TypedValueSerializerProvider
***********************************************************************/

			vuint8_t TypedValueSerializerProvider<vuint8_t>::GetDefaultValue()
			{
				return 0;
			}

			bool TypedValueSerializerProvider<vuint8_t>::Serialize(const vuint8_t& input, WString& output)
			{
				output=u64tow(input);
				return true;
			}

			bool TypedValueSerializerProvider<vuint8_t>::Deserialize(const WString& input, vuint8_t& output)
			{
				bool success=false;
				vuint64_t result=wtou64_test(input, success);
				if(!success) return false;
				if(result>_UI8_MAX) return false;
				output=(vuint8_t)result;
				return true;
			}

			//---------------------------------------

			vuint16_t TypedValueSerializerProvider<vuint16_t>::GetDefaultValue()
			{
				return 0;
			}

			bool TypedValueSerializerProvider<vuint16_t>::Serialize(const vuint16_t& input, WString& output)
			{
				output=u64tow(input);
				return true;
			}

			bool TypedValueSerializerProvider<vuint16_t>::Deserialize(const WString& input, vuint16_t& output)
			{
				bool success=false;
				vuint64_t result=wtou64_test(input, success);
				if(!success) return false;
				if(result>_UI16_MAX) return false;
				output=(vuint16_t)result;
				return true;
			}

			//---------------------------------------

			vuint32_t TypedValueSerializerProvider<vuint32_t>::GetDefaultValue()
			{
				return 0;
			}

			bool TypedValueSerializerProvider<vuint32_t>::Serialize(const vuint32_t& input, WString& output)
			{
				output=u64tow(input);
				return true;
			}

			bool TypedValueSerializerProvider<vuint32_t>::Deserialize(const WString& input, vuint32_t& output)
			{
				bool success=false;
				vuint64_t result=wtou64_test(input, success);
				if(!success) return false;
				if(result>_UI32_MAX) return false;
				output=(vuint32_t)result;
				return true;
			}

			//---------------------------------------

			vuint64_t TypedValueSerializerProvider<vuint64_t>::GetDefaultValue()
			{
				return 0;
			}

			bool TypedValueSerializerProvider<vuint64_t>::Serialize(const vuint64_t& input, WString& output)
			{
				output=u64tow(input);
				return true;
			}

			bool TypedValueSerializerProvider<vuint64_t>::Deserialize(const WString& input, vuint64_t& output)
			{
				bool success=false;
				vuint64_t result=wtou64_test(input, success);
				if(!success) return false;
				output=result;
				return true;
			}

			//---------------------------------------

			vint8_t TypedValueSerializerProvider<vint8_t>::GetDefaultValue()
			{
				return 0;
			}

			bool TypedValueSerializerProvider<vint8_t>::Serialize(const vint8_t& input, WString& output)
			{
				output=i64tow(input);
				return true;
			}

			bool TypedValueSerializerProvider<vint8_t>::Deserialize(const WString& input, vint8_t& output)
			{
				bool success=false;
				vint64_t result=wtoi64_test(input, success);
				if(!success) return false;
				if(result<_I8_MIN || result>_I8_MAX) return false;
				output=(vint8_t)result;
				return true;
			}

			//---------------------------------------

			vint16_t TypedValueSerializerProvider<vint16_t>::GetDefaultValue()
			{
				return 0;
			}

			bool TypedValueSerializerProvider<vint16_t>::Serialize(const vint16_t& input, WString& output)
			{
				output=i64tow(input);
				return true;
			}

			bool TypedValueSerializerProvider<vint16_t>::Deserialize(const WString& input, vint16_t& output)
			{
				bool success=false;
				vint64_t result=wtoi64_test(input, success);
				if(!success) return false;
				if(result<_I16_MIN || result>_I16_MAX) return false;
				output=(vint16_t)result;
				return true;
			}

			//---------------------------------------

			vint32_t TypedValueSerializerProvider<vint32_t>::GetDefaultValue()
			{
				return 0;
			}

			bool TypedValueSerializerProvider<vint32_t>::Serialize(const vint32_t& input, WString& output)
			{
				output=i64tow(input);
				return true;
			}

			bool TypedValueSerializerProvider<vint32_t>::Deserialize(const WString& input, vint32_t& output)
			{
				bool success=false;
				vint64_t result=wtoi64_test(input, success);
				if(!success) return false;
				if(result<_I32_MIN || result>_I32_MAX) return false;
				output=(vint32_t)result;
				return true;
			}

			//---------------------------------------

			vint64_t TypedValueSerializerProvider<vint64_t>::GetDefaultValue()
			{
				return 0;
			}

			bool TypedValueSerializerProvider<vint64_t>::Serialize(const vint64_t& input, WString& output)
			{
				output=i64tow(input);
				return true;
			}

			bool TypedValueSerializerProvider<vint64_t>::Deserialize(const WString& input, vint64_t& output)
			{
				bool success=false;
				vint64_t result=wtoi64_test(input, success);
				if(!success) return false;
				output=result;
				return true;
			}

			//---------------------------------------

			float TypedValueSerializerProvider<float>::GetDefaultValue()
			{
				return 0;
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

			//---------------------------------------

			double TypedValueSerializerProvider<double>::GetDefaultValue()
			{
				return 0;
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

			//---------------------------------------

			wchar_t TypedValueSerializerProvider<wchar_t>::GetDefaultValue()
			{
				return 0;
			}

			bool TypedValueSerializerProvider<wchar_t>::Serialize(const wchar_t& input, WString& output)
			{
				output=input?WString(input):L"";
				return true;
			}

			bool TypedValueSerializerProvider<wchar_t>::Deserialize(const WString& input, wchar_t& output)
			{
				if(input.Length()>1) return false;
				output=input.Length()==0?0:input[0];
				return true;
			}

			//---------------------------------------

			WString TypedValueSerializerProvider<WString>::GetDefaultValue()
			{
				return L"";
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

			//---------------------------------------

			Locale TypedValueSerializerProvider<Locale>::GetDefaultValue()
			{
				return Locale();
			}

			bool TypedValueSerializerProvider<Locale>::Serialize(const Locale& input, WString& output)
			{
				output=input.GetName();
				return true;
			}

			bool TypedValueSerializerProvider<Locale>::Deserialize(const WString& input, Locale& output)
			{
				output=Locale(input);
				return true;
			}

/***********************************************************************
ObjectTypeDescriptor
***********************************************************************/

			class ObjectTypeDescriptor : public SerializableTypeDescriptorBase
			{
			public:
				ObjectTypeDescriptor()
					:SerializableTypeDescriptorBase(TypeInfo<Value>::TypeName, 0)
				{
				}
			};

/***********************************************************************
BoolValueSerializer
***********************************************************************/

			class BoolValueSerializer : public EnumValueSerializer<bool, false>
			{
			public:
				BoolValueSerializer(ITypeDescriptor* _ownerTypeDescriptor)
					:EnumValueSerializer(_ownerTypeDescriptor, false)
				{
					candidates.Add(L"true", true);
					candidates.Add(L"false", false);
				}
			};

/***********************************************************************
DateTimeValueSerializer
***********************************************************************/

			class DateTimeValueSerializer : public GeneralValueSerializer<DateTime>
			{
			protected:
				Regex				regexDateTime;

				DateTime GetDefaultValue()override
				{
					return DateTime();
				}

				WString Format(vint number, vint length)
				{
					WString result = itow(number);
					while (result.Length() < length)
					{
						result = L"0" + result;
					}
					return result;
				}

				bool Serialize(const DateTime& input, WString& output)override
				{
					output = 
						Format(input.year, 4) + L"-" + Format(input.month, 2) + L"-" + Format(input.day, 2) + L" " + 
						Format(input.hour, 2) + L":" + Format(input.minute, 2) + L":" + Format(input.second, 2) + L"." + 
						Format(input.milliseconds, 3);
					return true;
				}

				bool Deserialize(const WString& input, DateTime& output)override
				{
					Ptr<RegexMatch> match=regexDateTime.Match(input);
					if(!match) return false;
					if(!match->Success()) return false;
					if(match->Result().Start()!=0) return false;
					if(match->Result().Length()!=input.Length()) return false;

					vint year=wtoi(match->Groups()[L"Y"].Get(0).Value());
					vint month=wtoi(match->Groups()[L"M"].Get(0).Value());
					vint day=wtoi(match->Groups()[L"D"].Get(0).Value());
					vint hour=wtoi(match->Groups()[L"h"].Get(0).Value());
					vint minute=wtoi(match->Groups()[L"m"].Get(0).Value());
					vint second=wtoi(match->Groups()[L"s"].Get(0).Value());
					vint milliseconds=wtoi(match->Groups()[L"ms"].Get(0).Value());

					output=DateTime::FromDateTime(year, month, day, hour, minute, second, milliseconds);
					return true;
				}
			public:
				DateTimeValueSerializer(ITypeDescriptor* _ownerTypeDescriptor)
					:GeneralValueSerializer<DateTime>(_ownerTypeDescriptor)
					,regexDateTime(L"(<Y>/d/d/d/d)-(<M>/d/d)-(<D>/d/d) (<h>/d/d):(<m>/d/d):(<s>/d/d).(<ms>/d/d/d)")
				{
				}
			};

/***********************************************************************
Helper Functions
***********************************************************************/

			vint ITypeDescriptor_GetTypeDescriptorCount()
			{
				return GetGlobalTypeManager()->GetTypeDescriptorCount();
			}

			ITypeDescriptor* ITypeDescriptor_GetTypeDescriptor(vint index)
			{
				return GetGlobalTypeManager()->GetTypeDescriptor(index);
			}

			ITypeDescriptor* ITypeDescriptor_GetTypeDescriptor(const WString& name)
			{
				return GetGlobalTypeManager()->GetTypeDescriptor(name);
			}

			ITypeDescriptor* ITypeDescriptor_GetTypeDescriptor(const Value& value)
			{
				return value.GetTypeDescriptor();
			}

			Value IValueSerializer_Parse(IValueSerializer* serializer, const WString& input)
			{
				Value value;
				if(serializer->Parse(input, value))
				{
					return value;
				}
				else
				{
					return Value();
				}
			}

/***********************************************************************
Interface Implementation Proxy (Implement)
***********************************************************************/

			namespace interface_proxy
			{
				class description_IValueListener : public ValueInterfaceRoot, public IValueListener
				{
				public:
					description_IValueListener(Ptr<IValueInterfaceProxy> proxy)
						:ValueInterfaceRoot(proxy)
					{
					}

					static Ptr<IValueListener> Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new description_IValueListener(proxy);
					}

					IValueSubscription* GetSubscription()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetSubscription);
					}

					bool GetStopped()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetStopped);
					}

					bool StopListening()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(StopListening);
					}
				};

				class description_IValueSubscription: public ValueInterfaceRoot, public IValueSubscription
				{
				public:
					description_IValueSubscription(Ptr<IValueInterfaceProxy> proxy)
						:ValueInterfaceRoot(proxy)
					{
					}

					static Ptr<IValueSubscription> Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new description_IValueSubscription(proxy);
					}

					Ptr<IValueListener> Subscribe(const Func<void(Value)>& callback)override
					{
						return INVOKEGET_INTERFACE_PROXY(Subscribe, callback);
					}

					bool Close()override
					{
						return INVOKEGET_INTERFACE_PROXY_NOPARAMS(Close);
					}
				};
			}

/***********************************************************************
Collections
***********************************************************************/

#define _ ,	
			
			template<>
			struct CustomTypeDescriptorSelector<DescriptableObject>
			{
			public:
				class CustomTypeDescriptorImpl : public TypeDescriptorImpl
				{
				public:
					CustomTypeDescriptorImpl()
						:TypeDescriptorImpl(TypeInfo<DescriptableObject>::TypeName)
					{
						Description<DescriptableObject>::SetAssociatedTypeDescroptor(this);
					}
					~CustomTypeDescriptorImpl()
					{
						Description<DescriptableObject>::SetAssociatedTypeDescroptor(0);
					}
				protected:
					void LoadInternal()override
					{
					}
				};
			};

			BEGIN_STRUCT_MEMBER(VoidValue)
			END_STRUCT_MEMBER(VoidValue)

			BEGIN_CLASS_MEMBER(IDescriptable)
			END_CLASS_MEMBER(IDescriptable)

			BEGIN_CLASS_MEMBER(IValueEnumerator)
				CLASS_MEMBER_BASE(IDescriptable)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Current)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Index)
				CLASS_MEMBER_METHOD(Next, NO_PARAMETER)
			END_CLASS_MEMBER(IValueEnumerator)

			BEGIN_CLASS_MEMBER(IValueEnumerable)
				CLASS_MEMBER_BASE(IDescriptable)
				CLASS_MEMBER_METHOD(CreateEnumerator, NO_PARAMETER)
			END_CLASS_MEMBER(IValueEnumerable)

			BEGIN_CLASS_MEMBER(IValueReadonlyList)
				CLASS_MEMBER_BASE(IValueEnumerable)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Count)
				CLASS_MEMBER_METHOD(Get, {L"index"})
				CLASS_MEMBER_METHOD(Contains, {L"value"})
				CLASS_MEMBER_METHOD(IndexOf, {L"value"})
			END_CLASS_MEMBER(IValueReadonlyList)

			BEGIN_CLASS_MEMBER(IValueList)
				CLASS_MEMBER_BASE(IValueReadonlyList)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<IValueList>(), NO_PARAMETER, (Ptr<IValueList>(*)())&IValueList::Create)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<IValueList>(Ptr<IValueReadonlyList>), {L"values"}, (Ptr<IValueList>(*)(Ptr<IValueReadonlyList>))&IValueList::Create)

				CLASS_MEMBER_METHOD(Set, {L"index" _ L"value"})
				CLASS_MEMBER_METHOD(Add, {L"value"})
				CLASS_MEMBER_METHOD(Insert, {L"index" _ L"value"})
				CLASS_MEMBER_METHOD(Remove, {L"value"})
				CLASS_MEMBER_METHOD(RemoveAt, {L"index"})
				CLASS_MEMBER_METHOD(Clear, NO_PARAMETER)
			END_CLASS_MEMBER(IValueList)

			BEGIN_CLASS_MEMBER(IValueReadonlyDictionary)
				CLASS_MEMBER_BASE(IDescriptable)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Keys)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Values)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Count)
				CLASS_MEMBER_METHOD(Get, {L"key"})
			END_CLASS_MEMBER(IValueReadonlyDictionary)

			BEGIN_CLASS_MEMBER(IValueDictionary)
				CLASS_MEMBER_BASE(IValueReadonlyDictionary)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<IValueDictionary>(), NO_PARAMETER, (Ptr<IValueDictionary>(*)())&IValueDictionary::Create)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<IValueDictionary>(Ptr<IValueReadonlyDictionary>), {L"values"}, (Ptr<IValueDictionary>(*)(Ptr<IValueReadonlyDictionary>))&IValueDictionary::Create)
				CLASS_MEMBER_METHOD(Set, {L"key" _ L"value"})
				CLASS_MEMBER_METHOD(Remove, {L"key"})
				CLASS_MEMBER_METHOD(Clear, NO_PARAMETER)
			END_CLASS_MEMBER(IValueDictionary)

			BEGIN_CLASS_MEMBER(IValueInterfaceProxy)
				CLASS_MEMBER_BASE(IDescriptable)
				CLASS_MEMBER_METHOD(Invoke, {L"name" _ L"arguments"})
			END_CLASS_MEMBER(IValueInterfaceProxy)

			BEGIN_CLASS_MEMBER(IValueFunctionProxy)
				CLASS_MEMBER_BASE(IDescriptable)
				CLASS_MEMBER_METHOD(Invoke, {L"arguments"})
			END_CLASS_MEMBER(IValueFunctionProxy)

			BEGIN_CLASS_MEMBER(IValueListener)
				CLASS_MEMBER_BASE(IDescriptable)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<IValueListener>(Ptr<IValueInterfaceProxy>), {L"proxy"}, &interface_proxy::description_IValueListener::Create)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Subscription)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Stopped)
				CLASS_MEMBER_METHOD(StopListening, NO_PARAMETER)
			END_CLASS_MEMBER(IValueListener)

			BEGIN_CLASS_MEMBER(IValueSubscription)
				CLASS_MEMBER_BASE(IDescriptable)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<IValueSubscription>(Ptr<IValueInterfaceProxy>), {L"proxy"}, &interface_proxy::description_IValueSubscription::Create)
				CLASS_MEMBER_METHOD(Subscribe, { L"callback" })
				CLASS_MEMBER_METHOD(Close, NO_PARAMETER)
			END_CLASS_MEMBER(IValueSubscription)

			BEGIN_CLASS_MEMBER(IValueSerializer)
				CLASS_MEMBER_BASE(IDescriptable)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(OwnerTypeDescriptor)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(CandidateCount)

				CLASS_MEMBER_METHOD(Validate, {L"text"})
				CLASS_MEMBER_EXTERNALMETHOD(Parse, {L"input"}, Value(IValueSerializer::*)(const WString&), &IValueSerializer_Parse)
				CLASS_MEMBER_METHOD(HasCandidate, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetCandidate, {L"index"})
				CLASS_MEMBER_METHOD(CanMergeCandidate, NO_PARAMETER)
			END_CLASS_MEMBER(IValueSerializer)

			BEGIN_CLASS_MEMBER(ITypeInfo)
				CLASS_MEMBER_BASE(IDescriptable)
				
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Decorator)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ElementType)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(TypeDescriptor)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(GenericArgumentCount)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(TypeFriendlyName)
				
				CLASS_MEMBER_METHOD(GetGenericArgument, {L"index"})
			END_CLASS_MEMBER(ITypeInfo)

			BEGIN_ENUM_ITEM(ITypeInfo::Decorator)
				ENUM_ITEM_NAMESPACE(ITypeInfo)

				ENUM_NAMESPACE_ITEM(RawPtr)
				ENUM_NAMESPACE_ITEM(SharedPtr)
				ENUM_NAMESPACE_ITEM(Nullable)
				ENUM_NAMESPACE_ITEM(TypeDescriptor)
				ENUM_NAMESPACE_ITEM(Generic)
			END_ENUM_ITEM(ITypeInfo::Decorator)

			BEGIN_CLASS_MEMBER(IMemberInfo)
				CLASS_MEMBER_BASE(IDescriptable)
				
				CLASS_MEMBER_PROPERTY_READONLY_FAST(OwnerTypeDescriptor)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Name)
			END_CLASS_MEMBER(IMemberInfo)

			BEGIN_CLASS_MEMBER(IEventHandler)
				CLASS_MEMBER_BASE(IDescriptable)
				
				CLASS_MEMBER_PROPERTY_READONLY_FAST(OwnerEvent)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(OwnerObject)
				
				CLASS_MEMBER_METHOD(IsAttached, NO_PARAMETER)
				CLASS_MEMBER_METHOD(Detach, NO_PARAMETER)
				CLASS_MEMBER_METHOD(Invoke, {L"thisObject" _ L"arguments"})
			END_CLASS_MEMBER(IEventHandler)

			BEGIN_CLASS_MEMBER(IEventInfo)
				CLASS_MEMBER_BASE(IMemberInfo)
				
				CLASS_MEMBER_PROPERTY_READONLY_FAST(HandlerType)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ObservingPropertyCount)
				
				CLASS_MEMBER_METHOD(GetObservingProperty, {L"index"})
				CLASS_MEMBER_METHOD(Attach, {L"thisObject" _ L"handler"})
				CLASS_MEMBER_METHOD(Invoke, {L"thisObject" _ L"arguments"})
			END_CLASS_MEMBER(IEventInfo)

			BEGIN_CLASS_MEMBER(IPropertyInfo)
				CLASS_MEMBER_BASE(IMemberInfo)
				
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Return)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Getter)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Setter)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ValueChangedEvent)
				
				CLASS_MEMBER_METHOD(IsReadable, NO_PARAMETER)
				CLASS_MEMBER_METHOD(IsWritable, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetValue, {L"thisObject"})
				CLASS_MEMBER_METHOD(SetValue, {L"thisObject" _ L"newValue"})
			END_CLASS_MEMBER(IPropertyInfo)

			BEGIN_CLASS_MEMBER(IParameterInfo)
				CLASS_MEMBER_BASE(IMemberInfo)
				
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Type)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(OwnerMethod)
			END_CLASS_MEMBER(IParameterInfo)

			BEGIN_CLASS_MEMBER(IMethodInfo)
				CLASS_MEMBER_BASE(IMemberInfo)
			
				CLASS_MEMBER_PROPERTY_READONLY_FAST(OwnerMethodGroup)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(OwnerProperty)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ParameterCount)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Return)
				
				CLASS_MEMBER_METHOD(GetParameter, {L"index"})
				CLASS_MEMBER_METHOD(IsStatic, NO_PARAMETER)
				CLASS_MEMBER_METHOD(CheckArguments, {L"arguments"})
				CLASS_MEMBER_METHOD(Invoke, {L"thisObject" _ L"arguments"})
				CLASS_MEMBER_BASE(IMemberInfo)
			END_CLASS_MEMBER(IMethodInfo)

			BEGIN_CLASS_MEMBER(IMethodGroupInfo)
				CLASS_MEMBER_BASE(IMemberInfo)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(MethodCount)
				
				CLASS_MEMBER_METHOD(GetMethod, {L"index"})
			END_CLASS_MEMBER(IMethodGroupInfo)

			BEGIN_CLASS_MEMBER(ITypeDescriptor)
				CLASS_MEMBER_BASE(IDescriptable)
				
				CLASS_MEMBER_PROPERTY_READONLY_FAST(TypeName)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ValueSerializer)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(BaseTypeDescriptorCount)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(PropertyCount)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(EventCount)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(MethodGroupCount)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ConstructorGroup)
				
				CLASS_MEMBER_METHOD(GetBaseTypeDescriptor, {L"index"})
				CLASS_MEMBER_METHOD(CanConvertTo, {L"targetType"})
				CLASS_MEMBER_METHOD(GetProperty, {L"index"})
				CLASS_MEMBER_METHOD(IsPropertyExists, {L"name" _ L"inheritable"})
				CLASS_MEMBER_METHOD(GetPropertyByName, {L"name" _ L"inheritable"})
				CLASS_MEMBER_METHOD(GetProperty, {L"index"})
				CLASS_MEMBER_METHOD(IsPropertyExists, {L"name" _ L"inheritable"})
				CLASS_MEMBER_METHOD(GetPropertyByName, {L"name" _ L"inheritable"})
				CLASS_MEMBER_METHOD(GetEvent, {L"index"})
				CLASS_MEMBER_METHOD(IsEventExists, {L"name" _ L"inheritable"})
				CLASS_MEMBER_METHOD(GetEventByName, {L"name" _ L"inheritable"})
				CLASS_MEMBER_METHOD(GetMethodGroup, {L"index"})
				CLASS_MEMBER_METHOD(IsMethodGroupExists, {L"name" _ L"inheritable"})
				CLASS_MEMBER_METHOD(GetMethodGroupByName, {L"name" _ L"inheritable"})

				CLASS_MEMBER_STATIC_EXTERNALMETHOD(GetTypeDescriptorCount, NO_PARAMETER, vint(*)(), &ITypeDescriptor_GetTypeDescriptorCount)
				CLASS_MEMBER_STATIC_EXTERNALMETHOD(GetTypeDescriptor, {L"index"}, ITypeDescriptor*(*)(vint), &ITypeDescriptor_GetTypeDescriptor)
				CLASS_MEMBER_STATIC_EXTERNALMETHOD(GetTypeDescriptor, {L"name"}, ITypeDescriptor*(*)(const WString&), &ITypeDescriptor_GetTypeDescriptor)
				CLASS_MEMBER_STATIC_EXTERNALMETHOD(GetTypeDescriptor, {L"value"}, ITypeDescriptor*(*)(const Value&), &ITypeDescriptor_GetTypeDescriptor)
			END_CLASS_MEMBER(ITypeDescriptor)
#undef _

/***********************************************************************
LoadPredefinedTypes
***********************************************************************/

			class PredefinedTypeLoader : public Object, public ITypeLoader
			{
			public:
				template<typename TSerializer>
				void AddSerializableType(ITypeManager* manager)
				{
					manager->SetTypeDescriptor(TypeInfo<typename TSerializer::ValueType>::TypeName, new SerializableTypeDescriptor<TSerializer>);
				}

				void Load(ITypeManager* manager)override
				{
					manager->SetTypeDescriptor(TypeInfo<Value>::TypeName, new ObjectTypeDescriptor);
					AddSerializableType<TypedDefaultValueSerializer<vuint8_t>>(manager);
					AddSerializableType<TypedDefaultValueSerializer<vuint16_t>>(manager);
					AddSerializableType<TypedDefaultValueSerializer<vuint32_t>>(manager);
					AddSerializableType<TypedDefaultValueSerializer<vuint64_t>>(manager);
					AddSerializableType<TypedDefaultValueSerializer<vint8_t>>(manager);
					AddSerializableType<TypedDefaultValueSerializer<vint16_t>>(manager);
					AddSerializableType<TypedDefaultValueSerializer<vint32_t>>(manager);
					AddSerializableType<TypedDefaultValueSerializer<vint64_t>>(manager);
					AddSerializableType<TypedDefaultValueSerializer<float>>(manager);
					AddSerializableType<TypedDefaultValueSerializer<double>>(manager);
					AddSerializableType<TypedDefaultValueSerializer<wchar_t>>(manager);
					AddSerializableType<TypedDefaultValueSerializer<WString>>(manager);
					AddSerializableType<TypedDefaultValueSerializer<Locale>>(manager);
					AddSerializableType<BoolValueSerializer>(manager);
					AddSerializableType<DateTimeValueSerializer>(manager);
					ADD_TYPE_INFO(VoidValue)
					ADD_TYPE_INFO(IDescriptable)
					ADD_TYPE_INFO(DescriptableObject)

					ADD_TYPE_INFO(IValueEnumerator)
					ADD_TYPE_INFO(IValueEnumerable)
					ADD_TYPE_INFO(IValueReadonlyList)
					ADD_TYPE_INFO(IValueList)
					ADD_TYPE_INFO(IValueReadonlyDictionary)
					ADD_TYPE_INFO(IValueDictionary)
					ADD_TYPE_INFO(IValueInterfaceProxy)
					ADD_TYPE_INFO(IValueFunctionProxy)
					
					ADD_TYPE_INFO(IValueListener)
					ADD_TYPE_INFO(IValueSubscription)
					ADD_TYPE_INFO(IValueSerializer)
					ADD_TYPE_INFO(ITypeInfo)
					ADD_TYPE_INFO(ITypeInfo::Decorator)
					ADD_TYPE_INFO(IMemberInfo)
					ADD_TYPE_INFO(IEventHandler)
					ADD_TYPE_INFO(IEventInfo)
					ADD_TYPE_INFO(IPropertyInfo)
					ADD_TYPE_INFO(IParameterInfo)
					ADD_TYPE_INFO(IMethodInfo)
					ADD_TYPE_INFO(IMethodGroupInfo)
					ADD_TYPE_INFO(ITypeDescriptor)
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
