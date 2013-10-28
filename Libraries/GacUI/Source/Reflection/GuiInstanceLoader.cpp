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

			bool SetLoaderForType(description::ITypeDescriptor* baseType, Ptr<IGuiInstanceLoader> loader)override
			{
				throw 0;
			}

			bool SetLoaderForVirtualType(const WString& typeName, description::ITypeDescriptor* baseType, Ptr<IGuiInstanceLoader> loader)override
			{
				throw 0;
			}

			IGuiInstanceLoader* GetLoaderFromType(const WString& typeName)override
			{
				throw 0;
			}

			IGuiInstanceLoader* GetLoaderForInstance(Ptr<GuiInstanceContext> context, Ptr<GuiInstanceRepr> instance, Ptr<GuiResourcePathResolver> resolver)override
			{
				throw 0;
			}

			DescriptableObject* LoadObject(Ptr<GuiInstanceContext> context, Ptr<GuiResourcePathResolver> resolver)override
			{
				throw 0;
			}

			bool LoadObject(DescriptableObject* createdInstance, Ptr<GuiInstanceContext> context, Ptr<GuiResourcePathResolver> resolver)override
			{
				throw 0;
			}
		};
		GUI_REGISTER_PLUGIN(GuiInstanceLoaderManager)
	}
}