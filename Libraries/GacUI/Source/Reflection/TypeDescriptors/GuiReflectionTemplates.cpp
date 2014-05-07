#include "GuiReflectionTemplates.h"

namespace vl
{
	namespace reflection
	{
		namespace description
		{
			using namespace collections;
			using namespace parsing;
			using namespace parsing::tabling;
			using namespace parsing::xml;
			using namespace stream;

#ifndef VCZH_DEBUG_NO_REFLECTION

			GUIREFLECTIONTEMPLATES_TYPELIST(IMPL_TYPE_INFO)

/***********************************************************************
Type Declaration
***********************************************************************/

#define _ ,

#define INTERFACE_EXTERNALCTOR(CONTROL, INTERFACE)\
	CLASS_MEMBER_EXTERNALCTOR(decltype(interface_proxy::CONTROL##_##INTERFACE::Create(0))(Ptr<IValueInterfaceProxy>), {L"proxy"}, &interface_proxy::CONTROL##_##INTERFACE::Create)

			BEGIN_CLASS_MEMBER(GuiTemplate)
				CLASS_MEMBER_CONSTRUCTOR(GuiTemplate*(), NO_PARAMETER)
			END_CLASS_MEMBER(GuiTemplate)
			
			BEGIN_CLASS_MEMBER(GuiTemplate::IFactory)
				INTERFACE_EXTERNALCTOR(GuiTemplate, IFactory)

				CLASS_MEMBER_METHOD(CreateTemplate, NO_PARAMETER)
			END_CLASS_MEMBER(GuiTemplate::IFactory)

#undef INTERFACE_EXTERNALCTOR
#undef _

/***********************************************************************
Type Loader
***********************************************************************/

			class GuiTemplateTypeLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					GUIREFLECTIONTEMPLATES_TYPELIST(ADD_TYPE_INFO)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};

#endif

			bool LoadGuiTemplateTypes()
			{
#ifndef VCZH_DEBUG_NO_REFLECTION
				ITypeManager* manager=GetGlobalTypeManager();
				if(manager)
				{
					Ptr<ITypeLoader> loader=new GuiTemplateTypeLoader;
					return manager->AddTypeLoader(loader);
				}
#endif
				return false;
			}
		}
	}
}