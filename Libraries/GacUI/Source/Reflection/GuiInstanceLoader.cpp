#include "GuiInstanceLoader.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;
		using namespace parsing;
		using namespace parsing::xml;
		using namespace parsing::tabling;
		using namespace controls;

/***********************************************************************
GuiInstanceContext
***********************************************************************/

		Ptr<GuiInstanceContext> GuiInstanceContext::LoadFromXml(Ptr<parsing::xml::XmlDocument> xml)
		{
			throw 0;
		}

/***********************************************************************
GuiInstanceLoaderManager
***********************************************************************/

		IGuiInstanceLoaderManager* instanceLoaderManager=0;

		IGuiInstanceLoaderManager* GetInstanceLoaderManager()
		{
			return instanceLoaderManager;
		}

		class GuiInstanceLoaderManager : public Object, public IGuiInstanceLoaderManager, public IGuiPlugin
		{
		public:
			void Load()override
			{
				instanceLoaderManager=this;
			}
				
			void AfterLoad()override
			{
			}

			void Unload()override
			{
				instanceLoaderManager=0;
			}

			bool AddInstanceBinder(Ptr<IGuiInstanceBinder> binder)override
			{
				throw 0;
			}

			IGuiInstanceBinder* GetInstanceBinder(const WString& name)override
			{
				throw 0;
			}

			bool SetLoaderForType(description::ITypeDescriptor* typeDescriptor, Ptr<IGuiInstanceLoader> loader)override
			{
				throw 0;
			}

			IGuiInstanceLoader* GetLoaderFromType(description::ITypeDescriptor* typeDescriptor)override
			{
				throw 0;
			}

			IGuiInstanceLoader* GetLoaderFromContext(Ptr<GuiInstanceContext> context)override
			{
				throw 0;
			}

			DescriptableObject* LoadObject(Ptr<GuiInstanceContext> context)override
			{
				throw 0;
			}

			bool LoadObject(DescriptableObject* createdInstance, Ptr<GuiInstanceContext> context)override
			{
				throw 0;
			}
		};
		GUI_REGISTER_PLUGIN(GuiInstanceLoaderManager)
	}
}