#include "GuiInstanceLoader.h"

namespace vl
{
	namespace presentation
	{
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

			bool SetLoaderForType(description::ITypeDescriptor* typeDescriptor, Ptr<IGuiInstanceLoader> loader)override
			{
				throw 0;
			}

			IGuiInstanceLoader* GetLoaderFromType(description::ITypeDescriptor* typeDescriptor)override
			{
				throw 0;
			}
		};
		GUI_REGISTER_PLUGIN(GuiInstanceLoaderManager)
	}
}