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
				public:\
					CustomEnumValueSerializer(ITypeDescriptor* _ownerTypeDescriptor)\
						:EnumValueSeriaizer(_ownerTypeDescriptor)\
					{\

#define BEGIN_ENUM_ITEM(TYPENAME) BEGIN_ENUM_ITEM_FLAG(TYPENAME, false)
#define BEGIN_ENUM_ITEM_MERGABLE(TYPENAME) BEGIN_ENUM_ITEM_FLAG(TYPENAME, true)

#define END_ENUM_ITEM(TYPENAME)\
					}\
				};\
				typedef SerializableTypeDescriptor<CustomEnumValueSerializer> CustomTypeDescriptorImpl;\
			};\

#define ENUM_ITEM(ITEMNAME) candidates.Add(L#ITEMNAME, ITEMNAME);

/***********************************************************************
Class
***********************************************************************/

#define BEGIN_TYPE_MEMBER(TYPENAME)\
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
					{\

#define END_TYPE_MEMBER(TYPENAME)\
					}\
				};\
			};\

#define TYPE_MEMBER_BASE(TYPENAME)\
			AddBaseType(GetTypeDescriptor<TYPENAME>());

/***********************************************************************
Field
***********************************************************************/

#define TYPE_MEMBER_FIELD(FIELDNAME)\
			AddProperty(\
				new CustomFieldInfoImpl<\
					ClassType,\
					decltype(((ClassType*)0)->FIELDNAME), &ClassType::FIELDNAME>\
					(this, L#FIELDNAME)\
				);

/***********************************************************************
Constructor
***********************************************************************/

#define NO_PARAMETER {L""}
#define TYPE_MEMBER_CONSTRUCTOR(FUNCTIONTYPE, PARAMETERNAMES)\
			{\
				const wchar_t* parameterNames[]=PARAMETERNAMES;\
				AddConstructor(new CustomConstructorInfoImpl<FUNCTIONTYPE>(parameterNames));\
			}

/***********************************************************************
Method
***********************************************************************/

#define TYPE_MEMBER_METHOD(FUNCTIONNAME, PARAMETERNAMES)\
			{\
				const wchar_t* parameterNames[]=PARAMETERNAMES;\
				AddMethod(\
					L#FUNCTIONNAME,\
					new CustomMethodInfoImplSelector<\
						ClassType,\
						vl::function_lambda::LambdaRetriveType<decltype(&ClassType::FUNCTIONNAME)>::FunctionType\
						>\
						::CustomMethodInfoImpl<&ClassType::FUNCTIONNAME>(parameterNames)\
					);\
			}

#define TYPE_MEMBER_METHOD_OVERLOAD(FUNCTIONNAME, PARAMETERNAMES, FUNCTIONTYPE)\
			{\
				const wchar_t* parameterNames[]=PARAMETERNAMES;\
				AddMethod(\
					L#FUNCTIONNAME,\
					new CustomMethodInfoImplSelector<\
						ClassType,\
						vl::function_lambda::LambdaRetriveType<FUNCTIONTYPE>::FunctionType\
						>\
						::CustomMethodInfoImpl<(FUNCTIONTYPE)&ClassType::FUNCTIONNAME>(parameterNames)\
					);\
			}

/***********************************************************************
Static Method
***********************************************************************/

#define TYPE_MEMBER_STATIC_METHOD(FUNCTIONNAME, PARAMETERNAMES)\
			{\
				const wchar_t* parameterNames[]=PARAMETERNAMES;\
				AddMethod(\
					L#FUNCTIONNAME,\
					new CustomMethodInfoImplSelector<\
						void,\
						vl::function_lambda::FunctionObjectRetriveType<decltype(&ClassType::FUNCTIONNAME)>::FunctionType\
						>\
						::CustomMethodInfoImpl<&ClassType::FUNCTIONNAME>(parameterNames)\
					);\
			}

#define TYPE_MEMBER_STATIC_METHOD_OVERLOAD(FUNCTIONNAME, PARAMETERNAMES, FUNCTIONTYPE)\
			{\
				const wchar_t* parameterNames[]=PARAMETERNAMES;\
				AddMethod(\
					L#FUNCTIONNAME,\
					new CustomMethodInfoImplSelector<\
						void,\
						vl::function_lambda::FunctionObjectRetriveType<FUNCTIONTYPE>::FunctionType\
						>\
						::CustomMethodInfoImpl<(FUNCTIONTYPE)&ClassType::FUNCTIONNAME>(parameterNames)\
					);\
			}

/***********************************************************************
Property
***********************************************************************/

#define TYPE_MEMBER_PROPERTY_READONLY(PROPERTYNAME, GETTER)\
			AddProperty(\
				new PropertyInfoImpl(\
					this,\
					L#PROPERTYNAME,\
					dynamic_cast<MethodInfoImpl*>(GetMethodGroupByName(L#GETTER, false)->GetMethod(0)),\
					0,\
					0\
					)\
				);

#define TYPE_MEMBER_PROPERTY(PROPERTYNAME, GETTER, SETTER)\
			AddProperty(\
				new PropertyInfoImpl(\
					this,\
					L#PROPERTYNAME,\
					dynamic_cast<MethodInfoImpl*>(GetMethodGroupByName(L#GETTER, false)->GetMethod(0)),\
					dynamic_cast<MethodInfoImpl*>(GetMethodGroupByName(L#SETTER, false)->GetMethod(0)),\
					0\
					)\
				);

#define TYPE_MEMBER_PROPERTY_EVENT(PROPERTYNAME, GETTER, SETTER, EVENT)\
			AddProperty(\
				new PropertyInfoImpl(\
					this,\
					L#PROPERTYNAME,\
					dynamic_cast<MethodInfoImpl*>(GetMethodGroupByName(L#GETTER, false)->GetMethod(0)),\
					dynamic_cast<MethodInfoImpl*>(GetMethodGroupByName(L#SETTER, false)->GetMethod(0)),\
					dynamic_cast<EventInfoImpl*>(GetEventByName(L#EVENT))\
					)\
				);
		}
	}
}

#endif