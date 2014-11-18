/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Framework::Reflection

Classes:
***********************************************************************/

#ifndef VCZH_REFLECTION_GUITYPEDESCRIPTORMACROS
#define VCZH_REFLECTION_GUITYPEDESCRIPTORMACROS

#include "GuiTypeDescriptorBuilder.h"
#include "GuiTypeDescriptorBuilder_Struct.h"
#include "GuiTypeDescriptorBuilder_Function.h"
#include "GuiTypeDescriptorBuilder_Container.h"
#include "GuiTypeDescriptorPredefined.h"

/***********************************************************************
Macros
***********************************************************************/

namespace vl
{
	namespace reflection
	{
		namespace description
		{
			template<typename T>
			struct CustomTypeDescriptorSelector{};

/***********************************************************************
Type
***********************************************************************/

#define BEGIN_TYPE_INFO_NAMESPACE namespace vl{namespace reflection{namespace description{
#define END_TYPE_INFO_NAMESPACE }}}
#define DECL_TYPE_INFO(TYPENAME) template<>struct TypeInfo<TYPENAME>{static const wchar_t* TypeName;};
#define IMPL_TYPE_INFO(TYPENAME) const wchar_t* TypeInfo<TYPENAME>::TypeName = L ## #TYPENAME;
#define IMPL_TYPE_INFO_RENAME(TYPENAME, EXPECTEDNAME) const wchar_t* TypeInfo<TYPENAME>::TypeName = L ## #EXPECTEDNAME;
#define ADD_TYPE_INFO(TYPENAME)\
			{\
				Ptr<ITypeDescriptor> type=new CustomTypeDescriptorSelector<TYPENAME>::CustomTypeDescriptorImpl();\
				manager->SetTypeDescriptor(TypeInfo<TYPENAME>::TypeName, type);\
			}

#define INVOKE_INTERFACE_PROXY(METHODNAME, ...)\
	proxy->Invoke(L ## #METHODNAME, IValueList::Create(collections::From((collections::Array<Value>&)(Value_xs(), __VA_ARGS__))))

#define INVOKE_INTERFACE_PROXY_NOPARAMS(METHODNAME)\
	proxy->Invoke(L ## #METHODNAME, IValueList::Create())

#define INVOKEGET_INTERFACE_PROXY(METHODNAME, ...)\
	UnboxValue<decltype(METHODNAME(__VA_ARGS__))>(proxy->Invoke(L ## #METHODNAME, IValueList::Create(collections::From((collections::Array<Value>&)(Value_xs(), __VA_ARGS__)))))

#define INVOKEGET_INTERFACE_PROXY_NOPARAMS(METHODNAME)\
	UnboxValue<decltype(METHODNAME())>(proxy->Invoke(L ## #METHODNAME, IValueList::Create()))

/***********************************************************************
Enum
***********************************************************************/

#define BEGIN_ENUM_ITEM_FLAG(TYPENAME, DEFAULTVALUE, FLAG)\
			template<>\
			struct CustomTypeDescriptorSelector<TYPENAME>\
			{\
			public:\
				class CustomEnumValueSerializer : public EnumValueSerializer<TYPENAME, FLAG>\
				{\
					typedef TYPENAME EnumType;\
				public:\
					CustomEnumValueSerializer(ITypeDescriptor* _ownerTypeDescriptor)\
						:EnumValueSerializer(_ownerTypeDescriptor, DEFAULTVALUE)\
					{

#define BEGIN_ENUM_ITEM_DEFAULT_VALUE(TYPENAME, DEFAULTVALUE) BEGIN_ENUM_ITEM_FLAG(TYPENAME, TYPENAME::DEFAULTVALUE, false)
#define BEGIN_ENUM_ITEM(TYPENAME) BEGIN_ENUM_ITEM_FLAG(TYPENAME, (TYPENAME)0, false)
#define BEGIN_ENUM_ITEM_MERGABLE(TYPENAME) BEGIN_ENUM_ITEM_FLAG(TYPENAME, (TYPENAME)0, true)

#define END_ENUM_ITEM(TYPENAME)\
					}\
				};\
				typedef SerializableTypeDescriptor<CustomEnumValueSerializer> CustomTypeDescriptorImpl;\
			};

#define ENUM_ITEM_NAMESPACE(TYPENAME) typedef TYPENAME EnumItemNamespace;
#define ENUM_ITEM(ITEMNAME) candidates.Add(L ## #ITEMNAME, ITEMNAME);
#define ENUM_NAMESPACE_ITEM(ITEMNAME) candidates.Add(L ## #ITEMNAME, EnumItemNamespace::ITEMNAME);
#define ENUM_CLASS_ITEM(ITEMNAME) candidates.Add(L ## #ITEMNAME, EnumType::ITEMNAME);

/***********************************************************************
Struct
***********************************************************************/

#define BEGIN_STRUCT_MEMBER(TYPENAME)\
			template<>\
			struct CustomTypeDescriptorSelector<TYPENAME>\
			{\
			public:\
				class CustomStructValueSerializer : public StructValueSerializer<TYPENAME>\
				{\
					typedef TYPENAME StructType;\
				public:\
					CustomStructValueSerializer(ITypeDescriptor* _ownerTypeDescriptor)\
						:StructValueSerializer(_ownerTypeDescriptor)\
					{\
					}\
				protected:\
					void LoadInternal()override\
					{

#define END_STRUCT_MEMBER(TYPENAME)\
					}\
				};\
				typedef StructTypeDescriptor<CustomStructValueSerializer> CustomTypeDescriptorImpl;\
			};

#define STRUCT_MEMBER(FIELDNAME)\
	fieldSerializers.Add(L ## #FIELDNAME, new FieldSerializer<decltype(((StructType*)0)->FIELDNAME)>(GetOwnerTypeDescriptor(), &StructType::FIELDNAME, L ## #FIELDNAME));

/***********************************************************************
Class
***********************************************************************/

#define BEGIN_CLASS_MEMBER(TYPENAME)\
			template<>\
			struct CustomTypeDescriptorSelector<TYPENAME>\
			{\
			public:\
				class CustomTypeDescriptorImpl : public TypeDescriptorImpl\
				{\
					typedef TYPENAME ClassType;\
				public:\
					CustomTypeDescriptorImpl()\
						:TypeDescriptorImpl(TypeInfo<TYPENAME>::TypeName)\
					{\
						Description<TYPENAME>::SetAssociatedTypeDescroptor(this);\
					}\
					~CustomTypeDescriptorImpl()\
					{\
						Description<TYPENAME>::SetAssociatedTypeDescroptor(0);\
					}\
				protected:\
					void LoadInternal()override\
					{

#define CLASS_MEMBER_BASE(TYPENAME)\
			AddBaseType(description::GetTypeDescriptor<TYPENAME>());

#define END_CLASS_MEMBER(TYPENAME)\
						if (GetBaseTypeDescriptorCount() == 0) CLASS_MEMBER_BASE(DescriptableObject)\
					}\
				};\
			};

/***********************************************************************
Field
***********************************************************************/

#define CLASS_MEMBER_FIELD(FIELDNAME)\
			AddProperty(\
				new CustomFieldInfoImpl<\
					ClassType,\
					decltype(((ClassType*)0)->FIELDNAME)\
					>(this, L ## #FIELDNAME, &ClassType::FIELDNAME)\
				);

/***********************************************************************
Constructor
***********************************************************************/

#define NO_PARAMETER {L""}
#define PROTECT_PARAMETERS(...) __VA_ARGS__

#define CLASS_MEMBER_CONSTRUCTOR(FUNCTIONTYPE, PARAMETERNAMES)\
			{\
				const wchar_t* parameterNames[]=PARAMETERNAMES;\
				AddConstructor(new CustomConstructorInfoImpl<FUNCTIONTYPE>(parameterNames));\
			}

#define CLASS_MEMBER_EXTERNALCTOR(FUNCTIONTYPE, PARAMETERNAMES, SOURCE)\
			{\
				const wchar_t* parameterNames[]=PARAMETERNAMES;\
				AddConstructor(\
					new CustomStaticMethodInfoImpl<FUNCTIONTYPE>(parameterNames, SOURCE)\
					);\
			}

/***********************************************************************
Method
***********************************************************************/

#define CLASS_MEMBER_EXTERNALMETHOD(FUNCTIONNAME, PARAMETERNAMES, FUNCTIONTYPE, SOURCE)\
			{\
				const wchar_t* parameterNames[]=PARAMETERNAMES;\
				AddMethod(\
					L ## #FUNCTIONNAME,\
					new CustomExternalMethodInfoImpl<\
						ClassType,\
						vl::function_lambda::LambdaRetriveType<FUNCTIONTYPE>::FunctionType\
						>(parameterNames, SOURCE)\
					);\
			}

#define CLASS_MEMBER_METHOD_OVERLOAD_RENAME(EXPECTEDNAME, FUNCTIONNAME, PARAMETERNAMES, FUNCTIONTYPE)\
			{\
				const wchar_t* parameterNames[]=PARAMETERNAMES;\
				AddMethod(\
					L ## #EXPECTEDNAME,\
					new CustomMethodInfoImpl<\
						ClassType,\
						vl::function_lambda::LambdaRetriveType<FUNCTIONTYPE>::FunctionType\
						>(parameterNames, (FUNCTIONTYPE)&ClassType::FUNCTIONNAME)\
					);\
			}

#define CLASS_MEMBER_METHOD_OVERLOAD(FUNCTIONNAME, PARAMETERNAMES, FUNCTIONTYPE)\
			CLASS_MEMBER_METHOD_OVERLOAD_RENAME(FUNCTIONNAME, FUNCTIONNAME, PROTECT_PARAMETERS(PARAMETERNAMES), FUNCTIONTYPE)

#define CLASS_MEMBER_METHOD_RENAME(EXPECTEDNAME, FUNCTIONNAME, PARAMETERNAMES)\
			CLASS_MEMBER_METHOD_OVERLOAD_RENAME(EXPECTEDNAME, FUNCTIONNAME, PROTECT_PARAMETERS(PARAMETERNAMES), decltype(&ClassType::FUNCTIONNAME))

#define CLASS_MEMBER_METHOD(FUNCTIONNAME, PARAMETERNAMES)\
			CLASS_MEMBER_METHOD_OVERLOAD(FUNCTIONNAME, PROTECT_PARAMETERS(PARAMETERNAMES), decltype(&ClassType::FUNCTIONNAME))

/***********************************************************************
Static Method
***********************************************************************/

#define CLASS_MEMBER_STATIC_EXTERNALMETHOD(FUNCTIONNAME, PARAMETERNAMES, FUNCTIONTYPE, SOURCE)\
			{\
				const wchar_t* parameterNames[]=PARAMETERNAMES;\
				AddMethod(\
					L ## #FUNCTIONNAME,\
					new CustomStaticMethodInfoImpl<\
						vl::function_lambda::FunctionObjectRetriveType<FUNCTIONTYPE>::FunctionType\
						>(parameterNames, SOURCE)\
					);\
			}

#define CLASS_MEMBER_STATIC_METHOD_OVERLOAD(FUNCTIONNAME, PARAMETERNAMES, FUNCTIONTYPE)\
			CLASS_MEMBER_STATIC_EXTERNALMETHOD(FUNCTIONNAME, PROTECT_PARAMETERS(PARAMETERNAMES), FUNCTIONTYPE, (FUNCTIONTYPE)&ClassType::FUNCTIONNAME)

#define CLASS_MEMBER_STATIC_METHOD(FUNCTIONNAME, PARAMETERNAMES)\
			CLASS_MEMBER_STATIC_METHOD_OVERLOAD(FUNCTIONNAME, PROTECT_PARAMETERS(PARAMETERNAMES), decltype(&ClassType::FUNCTIONNAME))

/***********************************************************************
Event
***********************************************************************/

#define CLASS_MEMBER_EVENT(EVENTNAME)\
			AddEvent(\
				new CustomEventInfoImpl<\
					ClassType,\
					CustomEventFunctionTypeRetriver<decltype(&ClassType::EVENTNAME)>::Type\
					>(this, L ## #EVENTNAME, &ClassType::EVENTNAME)\
				);

/***********************************************************************
Property
***********************************************************************/

#define CLASS_MEMBER_PROPERTY_READONLY(PROPERTYNAME, GETTER)\
			AddProperty(\
				new PropertyInfoImpl(\
					this,\
					L ## #PROPERTYNAME,\
					dynamic_cast<MethodInfoImpl*>(GetMethodGroupByName(L ## #GETTER, true)->GetMethod(0)),\
					0,\
					0\
					)\
				);

#define CLASS_MEMBER_PROPERTY(PROPERTYNAME, GETTER, SETTER)\
			AddProperty(\
				new PropertyInfoImpl(\
					this,\
					L ## #PROPERTYNAME,\
					dynamic_cast<MethodInfoImpl*>(GetMethodGroupByName(L ## #GETTER, true)->GetMethod(0)),\
					dynamic_cast<MethodInfoImpl*>(GetMethodGroupByName(L ## #SETTER, true)->GetMethod(0)),\
					0\
					)\
				);

#define CLASS_MEMBER_PROPERTY_EVENT(PROPERTYNAME, GETTER, SETTER, EVENT)\
			AddProperty(\
				new PropertyInfoImpl(\
					this,\
					L ## #PROPERTYNAME,\
					dynamic_cast<MethodInfoImpl*>(GetMethodGroupByName(L ## #GETTER, true)->GetMethod(0)),\
					dynamic_cast<MethodInfoImpl*>(GetMethodGroupByName(L ## #SETTER, true)->GetMethod(0)),\
					dynamic_cast<EventInfoImpl*>(GetEventByName(L ## #EVENT, true))\
					)\
				);

#define CLASS_MEMBER_PROPERTY_EVENT_READONLY(PROPERTYNAME, GETTER, EVENT)\
			AddProperty(\
				new PropertyInfoImpl(\
					this,\
					L ## #PROPERTYNAME,\
					dynamic_cast<MethodInfoImpl*>(GetMethodGroupByName(L ## #GETTER, true)->GetMethod(0)),\
					0,\
					dynamic_cast<EventInfoImpl*>(GetEventByName(L ## #EVENT, true))\
					)\
				);

#define CLASS_MEMBER_PROPERTY_READONLY_FAST(PROPERTYNAME)\
			CLASS_MEMBER_METHOD(Get##PROPERTYNAME, NO_PARAMETER)\
			CLASS_MEMBER_PROPERTY_READONLY(PROPERTYNAME, Get##PROPERTYNAME)\

#define CLASS_MEMBER_PROPERTY_FAST(PROPERTYNAME)\
			CLASS_MEMBER_METHOD(Get##PROPERTYNAME, NO_PARAMETER)\
			CLASS_MEMBER_METHOD(Set##PROPERTYNAME, {L"value"})\
			CLASS_MEMBER_PROPERTY(PROPERTYNAME, Get##PROPERTYNAME, Set##PROPERTYNAME)\

#define CLASS_MEMBER_PROPERTY_EVENT_FAST(PROPERTYNAME, EVENTNAME)\
			CLASS_MEMBER_METHOD(Get##PROPERTYNAME, NO_PARAMETER)\
			CLASS_MEMBER_METHOD(Set##PROPERTYNAME, {L"value"})\
			CLASS_MEMBER_PROPERTY_EVENT(PROPERTYNAME, Get##PROPERTYNAME, Set##PROPERTYNAME, EVENTNAME)\

#define CLASS_MEMBER_PROPERTY_EVENT_READONLY_FAST(PROPERTYNAME, EVENTNAME)\
			CLASS_MEMBER_METHOD(Get##PROPERTYNAME, NO_PARAMETER)\
			CLASS_MEMBER_PROPERTY_EVENT_READONLY(PROPERTYNAME, Get##PROPERTYNAME, EVENTNAME)\

		}
	}
}

#endif