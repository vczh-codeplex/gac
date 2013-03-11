/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Framework::Reflection

Classes:
***********************************************************************/

#ifndef VCZH_REFLECTION_GUITYPEDESCRIPTORMACROS
#define VCZH_REFLECTION_GUITYPEDESCRIPTORMACROS

#include "GuiTypeDescriptorBuilder.h"
#include "GuiTypeDescriptorBuilder_Gen.h"
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
#define IMPL_TYPE_INFO(TYPENAME) const wchar_t* TypeInfo<TYPENAME>::TypeName = L#TYPENAME;
#define ADD_TYPE_INFO(TYPENAME)\
			{\
				Ptr<ITypeDescriptor> type=new CustomTypeDescriptorSelector<TYPENAME>::CustomTypeDescriptorImpl();\
				Description<TYPENAME>::SetAssociatedTypeDescroptor(type.Obj());\
				manager->SetTypeDescriptor(TypeInfo<TYPENAME>::TypeName, type);\
			}

#define INVOKE_INTERFACE_PROXY(METHODNAME, ...)\
	proxy->Invoke(L#METHODNAME, IValueList::Create(collections::From((collections::Array<Value>&)(Value::xs(), __VA_ARGS__))))

#define INVOKE_INTERFACE_PROXY_NOPARAM(METHODNAME)\
	proxy->Invoke(L#METHODNAME, IValueList::Create())

#define INVOKEGET_INTERFACE_PROXY(METHODNAME, ...)\
	UnboxValue<decltype(METHODNAME(__VA_ARGS__))>(proxy->Invoke(L#METHODNAME, IValueList::Create(collections::From((collections::Array<Value>&)(Value::xs(), __VA_ARGS__)))))

#define INVOKEGET_INTERFACE_PROXY_NOPARAMS(METHODNAME)\
	UnboxValue<decltype(METHODNAME())>(proxy->Invoke(L#METHODNAME, IValueList::Create()))

/***********************************************************************
Enum
***********************************************************************/

#define BEGIN_ENUM_ITEM_FLAG(TYPENAME, FLAG)\
			template<>\
			struct CustomTypeDescriptorSelector<TYPENAME>\
			{\
			public:\
				class CustomEnumValueSerializer : public EnumValueSeriaizer<TYPENAME, FLAG>\
				{\
					typedef TYPENAME EnumType;\
				public:\
					CustomEnumValueSerializer(ITypeDescriptor* _ownerTypeDescriptor)\
						:EnumValueSeriaizer(_ownerTypeDescriptor)\
					{

#define BEGIN_ENUM_ITEM(TYPENAME) BEGIN_ENUM_ITEM_FLAG(TYPENAME, false)
#define BEGIN_ENUM_ITEM_MERGABLE(TYPENAME) BEGIN_ENUM_ITEM_FLAG(TYPENAME, true)

#define END_ENUM_ITEM(TYPENAME)\
					}\
				};\
				typedef SerializableTypeDescriptor<CustomEnumValueSerializer> CustomTypeDescriptorImpl;\
			};

#define ENUM_ITEM_NAMESPACE(TYPENAME) typedef TYPENAME EnumItemNamespace;
#define ENUM_ITEM(ITEMNAME) candidates.Add(L#ITEMNAME, ITEMNAME);
#define ENUM_NAMESPACE_ITEM(ITEMNAME) candidates.Add(L#ITEMNAME, EnumItemNamespace::ITEMNAME);
#define ENUM_CLASS_ITEM(ITEMNAME) candidates.Add(L#ITEMNAME, EnumType::ITEMNAME);

/***********************************************************************
Struct
***********************************************************************/

#define BEGIN_STRUCT_MEMBER(TYPENAME)\
			template<>\
			struct CustomTypeDescriptorSelector<TYPENAME>\
			{\
			public:\
				class CustomStructValueSerializer : public StructValueSeriaizer<TYPENAME>\
				{\
					typedef TYPENAME StructType;\
				public:\
					CustomStructValueSerializer(ITypeDescriptor* _ownerTypeDescriptor)\
						:StructValueSeriaizer(_ownerTypeDescriptor)\
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
	fieldSerializers.Add(L#FIELDNAME, new FieldSerializer<decltype(((StructType*)0)->FIELDNAME)>(GetOwnerTypeDescriptor(), &StructType::FIELDNAME, L#FIELDNAME));

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
					}\
				protected:\
					void LoadInternal()override\
					{

#define END_CLASS_MEMBER(TYPENAME)\
					}\
				};\
			};

#define CLASS_MEMBER_BASE(TYPENAME)\
			AddBaseType(GetTypeDescriptor<TYPENAME>());

/***********************************************************************
Field
***********************************************************************/

#define CLASS_MEMBER_FIELD(FIELDNAME)\
			AddProperty(\
				new CustomFieldInfoImpl<\
					ClassType,\
					decltype(((ClassType*)0)->FIELDNAME), (decltype(((ClassType*)0)->FIELDNAME) ClassType::*)&ClassType::FIELDNAME>\
					(this, L#FIELDNAME)\
				);

/***********************************************************************
Constructor
***********************************************************************/

#define NO_PARAMETER {L""}

#define CLASS_MEMBER_CONSTRUCTOR(FUNCTIONTYPE, PARAMETERNAMES)\
			{\
				const wchar_t* parameterNames[]=PARAMETERNAMES;\
				AddConstructor(new CustomConstructorInfoImpl<FUNCTIONTYPE>(parameterNames));\
			}

#define CLASS_MEMBER_EXTERNALCTOR(FUNCTIONTYPE, PARAMETERNAMES, SOURCE)\
			{\
				const wchar_t* parameterNames[]=PARAMETERNAMES;\
				AddConstructor(\
					new CustomMethodInfoImplSelector<\
						void,\
						FUNCTIONTYPE\
						>\
						::CustomMethodInfoImpl(parameterNames, SOURCE)\
					);\
			}

/***********************************************************************
Method
***********************************************************************/

#define CLASS_MEMBER_EXTERNALMETHOD(FUNCTIONNAME, PARAMETERNAMES, FUNCTIONTYPE, SOURCE)\
			{\
				const wchar_t* parameterNames[]=PARAMETERNAMES;\
				AddMethod(\
					L#FUNCTIONNAME,\
					new CustomMethodInfoImplSelector<\
						ClassType,\
						vl::function_lambda::LambdaRetriveType<FUNCTIONTYPE>::FunctionType\
						>\
						::ExternalMethodInfoImpl(parameterNames, SOURCE)\
					);\
			}

#define CLASS_MEMBER_METHOD_OVERLOAD(FUNCTIONNAME, PARAMETERNAMES, FUNCTIONTYPE)\
			{\
				const wchar_t* parameterNames[]=PARAMETERNAMES;\
				AddMethod(\
					L#FUNCTIONNAME,\
					new CustomMethodInfoImplSelector<\
						ClassType,\
						vl::function_lambda::LambdaRetriveType<FUNCTIONTYPE>::FunctionType\
						>\
						::CustomMethodInfoImpl(parameterNames, (FUNCTIONTYPE)&ClassType::FUNCTIONNAME)\
					);\
			}

#define CLASS_MEMBER_METHOD(FUNCTIONNAME, PARAMETERNAMES)\
			CLASS_MEMBER_METHOD_OVERLOAD(FUNCTIONNAME, PARAMETERNAMES, decltype(&ClassType::FUNCTIONNAME))

/***********************************************************************
Static Method
***********************************************************************/

#define CLASS_MEMBER_STATIC_EXTERNALMETHOD(FUNCTIONNAME, PARAMETERNAMES, FUNCTIONTYPE, SOURCE)\
			{\
				const wchar_t* parameterNames[]=PARAMETERNAMES;\
				AddMethod(\
					L#FUNCTIONNAME,\
					new CustomMethodInfoImplSelector<\
						void,\
						vl::function_lambda::FunctionObjectRetriveType<FUNCTIONTYPE>::FunctionType\
						>\
						::CustomMethodInfoImpl(parameterNames, SOURCE)\
					);\
			}

#define CLASS_MEMBER_STATIC_METHOD_OVERLOAD(FUNCTIONNAME, PARAMETERNAMES, FUNCTIONTYPE)\
			CLASS_MEMBER_STATIC_EXTERNALMETHOD(FUNCTIONNAME, PARAMETERNAMES, FUNCTIONTYPE, (FUNCTIONTYPE)&ClassType::FUNCTIONNAME)

#define CLASS_MEMBER_STATIC_METHOD(FUNCTIONNAME, PARAMETERNAMES)\
			CLASS_MEMBER_STATIC_METHOD_OVERLOAD(FUNCTIONNAME, PARAMETERNAMES, decltype(&ClassType::FUNCTIONNAME))

/***********************************************************************
Property
***********************************************************************/

#define CLASS_MEMBER_PROPERTY_READONLY(PROPERTYNAME, GETTER)\
			AddProperty(\
				new PropertyInfoImpl(\
					this,\
					L#PROPERTYNAME,\
					dynamic_cast<MethodInfoImpl*>(GetMethodGroupByName(L#GETTER, false)->GetMethod(0)),\
					0,\
					0\
					)\
				);

#define CLASS_MEMBER_PROPERTY(PROPERTYNAME, GETTER, SETTER)\
			AddProperty(\
				new PropertyInfoImpl(\
					this,\
					L#PROPERTYNAME,\
					dynamic_cast<MethodInfoImpl*>(GetMethodGroupByName(L#GETTER, false)->GetMethod(0)),\
					dynamic_cast<MethodInfoImpl*>(GetMethodGroupByName(L#SETTER, false)->GetMethod(0)),\
					0\
					)\
				);

#define CLASS_MEMBER_PROPERTY_EVENT(PROPERTYNAME, GETTER, SETTER, EVENT)\
			AddProperty(\
				new PropertyInfoImpl(\
					this,\
					L#PROPERTYNAME,\
					dynamic_cast<MethodInfoImpl*>(GetMethodGroupByName(L#GETTER, false)->GetMethod(0)),\
					dynamic_cast<MethodInfoImpl*>(GetMethodGroupByName(L#SETTER, false)->GetMethod(0)),\
					dynamic_cast<EventInfoImpl*>(GetEventByName(L#EVENT))\
					)\
				);

#define CLASS_MEMBER_PROPERTY_READONLY_FAST(PROPERTYNAME)\
			CLASS_MEMBER_METHOD(Get##PROPERTYNAME, NO_PARAMETER)\
			CLASS_MEMBER_PROPERTY_READONLY(PROPERTYNAME, Get##PROPERTYNAME)\

#define CLASS_MEMBER_PROPERTY_FAST(PROPERTYNAME)\
			CLASS_MEMBER_METHOD(Get##PROPERTYNAME, NO_PARAMETER)\
			CLASS_MEMBER_METHOD(Set##PROPERTYNAME, {L"value"})\
			CLASS_MEMBER_PROPERTY(PROPERTYNAME, Get##PROPERTYNAME, Set##PROPERTYNAME)\

		}
	}
}

#endif