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
			class CustomTypeDescriptorImpl{};

#define BEGIN_TYPE_INFO_NAMESPACE namespace vl{namespace reflection{namespace description{
#define END_TYPE_INFO_NAMESPACE }}}
#define DECL_TYPE_INFO(TYPENAME) template<>struct TypeInfo<TYPENAME>{static const wchar_t* TypeName;};
#define IMPL_TYPE_INFO(TYPENAME) const wchar_t* TypeInfo<TYPENAME>::TypeName = L#TYPENAME;
#define ADD_TYPE_INFO(TYPENAME) manager->SetTypeDescriptor(TypeInfo<TYPENAME>::TypeName, new CustomTypeDescriptorImpl<TYPENAME>);

#define BEGIN_TYPE_MEMBER(TYPENAME)\
			template<>\
			class CustomTypeDescriptorImpl<TYPENAME> : public TypeDescriptorImpl\
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

#define TYPE_MEMBER_BASE(TYPENAME) AddBaseType(GetTypeDescriptor<TYPENAME>());
#define TYPE_MEMBER_FIELD(FIELDNAME) AddProperty(new CustomFieldInfoImpl<ClassType, decltype(((ClassType*)0)->FIELDNAME), &ClassType::FIELDNAME>(this, L#FIELDNAME));

		}
	}
}

#endif