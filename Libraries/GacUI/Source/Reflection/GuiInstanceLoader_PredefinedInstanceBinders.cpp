#include "GuiInstanceLoader.h"
#include "TypeDescriptors\GuiReflectionControls.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;
		using namespace reflection::description;

/***********************************************************************
GuiTextInstanceBinderBase
***********************************************************************/

		class GuiTextInstanceBinderBase : public Object, public IGuiInstanceBinder
		{
		protected:
			ITypeDescriptor*				stringTypeDescriptor;
		public:
			GuiTextInstanceBinderBase()
				:stringTypeDescriptor(description::GetTypeDescriptor<WString>())
			{
			}

			void GetExpectedValueTypes(collections::List<description::ITypeDescriptor*>& expectedTypes)override
			{
				expectedTypes.Add(stringTypeDescriptor);
			}
		};

/***********************************************************************
GuiResourceInstanceBinder
***********************************************************************/

		class GuiResourceInstanceBinder : public GuiTextInstanceBinderBase
		{
		public:
			WString GetBindingName()override
			{
				return L"uri";
			}

			bool SetPropertyValue(Ptr<GuiInstanceEnvironment> env, IGuiInstanceLoader* loader, IGuiInstanceLoader::PropertyValue& propertyValue)override
			{
				if (propertyValue.propertyValue.GetValueType() == Value::Text)
				{
					WString protocol, path;
					if (IsResourceUrl(propertyValue.propertyValue.GetText(), protocol, path))
					{
						if(Ptr<DescriptableObject> resource=env->resolver->ResolveResource(protocol, path))
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
								return loader->SetPropertyValue(newValue);
							}
						}
					}
				}
				return false;
			}
		};

/***********************************************************************
GuiReferenceInstanceBinder
***********************************************************************/

		class GuiReferenceInstanceBinder : public GuiTextInstanceBinderBase
		{
		public:
			WString GetBindingName()override
			{
				return L"ref";
			}

			bool SetPropertyValue(Ptr<GuiInstanceEnvironment> env, IGuiInstanceLoader* loader, IGuiInstanceLoader::PropertyValue& propertyValue)override
			{
				if (propertyValue.propertyValue.GetValueType() == Value::Text)
				{
					WString name = propertyValue.propertyValue.GetText();
					vint index = env->scope->referenceValues.Keys().IndexOf(name);
					if (index != -1)
					{
						IGuiInstanceLoader::PropertyValue newValue = propertyValue;
						newValue.propertyValue = env->scope->referenceValues.Values()[index];
						if (!newValue.propertyValue.IsNull())
						{
							return loader->SetPropertyValue(newValue);
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
				manager->AddInstanceBinder(new GuiReferenceInstanceBinder);
			}

			void Unload()override
			{
			}
		};
		GUI_REGISTER_PLUGIN(GuiPredefinedInstanceBindersPlugin)
	}
}