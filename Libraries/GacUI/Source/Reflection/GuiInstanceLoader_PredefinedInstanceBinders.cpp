#include "GuiInstanceLoader.h"
#include "TypeDescriptors\GuiReflectionControls.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;
		using namespace reflection::description;

/***********************************************************************
GuiResourceInstanceBinder
***********************************************************************/

		class GuiResourceInstanceBinder : public Object, public IGuiInstanceBinder
		{
		protected:
			ITypeDescriptor*				stringTypeDescriptor;
		public:
			GuiResourceInstanceBinder()
				:stringTypeDescriptor(description::GetTypeDescriptor<WString>())
			{
			}

			WString GetBindingName()override
			{
				return L"uri";
			}

			description::ITypeDescriptor* GetExpectedValueType()
			{
				return stringTypeDescriptor;
			}

			bool SetPropertyValue(
				IGuiInstanceLoader* loader,
				Ptr<GuiResourcePathResolver> resolver,
				IGuiInstanceLoader::PropertyValue& propertyValue,
				bool collectionProperty
				)override
			{
				if (propertyValue.propertyValue.GetValueType() == Value::Text)
				{
					WString protocol, path;
					if (IsResourceUrl(propertyValue.propertyValue.GetText(), protocol, path))
					{
						if(Ptr<Object> resource=resolver->ResolveResource(protocol, path))
						{
							Value value;
							if(Ptr<GuiTextData> text=resource.Cast<GuiTextData>())
							{
								value=Value::From(text->GetText(), stringTypeDescriptor);
							}
							else if(Ptr<DescriptableObject> obj=resource.Cast<DescriptableObject>())
							{
								value=Value::From(obj);
							}

							if(!value.IsNull())
							{
								IGuiInstanceLoader::PropertyValue newValue = propertyValue;
								newValue.propertyValue = value;
								if(collectionProperty)
								{
									return loader->SetPropertyCollection(newValue);
								}
								else
								{
									return loader->SetPropertyValue(newValue);
								}
							}
						}
					}
				}
				return false;
			}
		};

/***********************************************************************
GuiPredefinedInstanceBindersPlugin
***********************************************************************/

		class GuiPredefinedInstanceBindersPlugin : public Object, public IGuiPlugin
		{
		public:
			void Load()override
			{
			}

			void AfterLoad()override
			{
				IGuiInstanceLoaderManager* manager=GetInstanceLoaderManager();

				manager->AddInstanceBinder(new GuiResourceInstanceBinder);
			}

			void Unload()override
			{
			}
		};
		GUI_REGISTER_PLUGIN(GuiPredefinedInstanceBindersPlugin)
	}
}