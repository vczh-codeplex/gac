#include "GuiInstanceLoader.h"
#include "TypeDescriptors\GuiReflectionControls.h"
#include "..\Resources\GuiParserManager.h"
#include "..\..\..\Workflow\Source\Expression\WfExpressionParser.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;
		using namespace reflection::description;
		using namespace workflow;

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

			void GetRequiredContexts(collections::List<WString>& contextNames)override
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
GuiWorkflowGlobalContext
***********************************************************************/

		struct WorkflowDataBinding
		{
			Value						instance;
			IPropertyInfo*				propertyInfo;
			Ptr<WfBindExpression>		bindExpression;
		};

		class GuiWorkflowGlobalContext : public Object, public IGuiInstanceBindingContext
		{
		public:
			WString GetContextName()override
			{
				return L"WORKFLOW-GLOBAL-CONTEXT";
			}

			void Initialize(Ptr<GuiInstanceEnvironment> env)override
			{
			}
		};

/***********************************************************************
GuiScriptInstanceBinder
***********************************************************************/

		class GuiScriptInstanceBinder : public GuiTextInstanceBinderBase
		{
		public:
			virtual WString TranslateExpression(const WString& input) = 0;

			void GetRequiredContexts(collections::List<WString>& contextNames)override
			{
				contextNames.Add(L"WORKFLOW-GLOBAL-CONTEXT");
			}

			bool SetPropertyValue(Ptr<GuiInstanceEnvironment> env, IGuiInstanceLoader* loader, IGuiInstanceLoader::PropertyValue& propertyValue)override
			{
				if (propertyValue.propertyValue.GetValueType() == Value::Text)
				{
					WString expressionCode = TranslateExpression(propertyValue.propertyValue.GetText());
					auto parser = GetParserManager()->GetParser<WfExpression>(L"WORKFLOW-EXPRESSION");
					auto expression = parser->TypedParse(expressionCode).Cast<WfBindExpression>();
					if (!expression)
					{
						env->scope->errors.Add(L"Failed to parse the workflow expression \"" + expressionCode + L"\".");
						return false;
					}



					return true;
				}
				return false;
			}
		};

/***********************************************************************
GuiBindInstanceBinder
***********************************************************************/

		class GuiBindInstanceBinder : public GuiScriptInstanceBinder
		{
		public:
			WString GetBindingName()override
			{
				return L"bind";
			}

			WString TranslateExpression(const WString& input)override
			{
				return L"bind(" + input + L")";
			}
		};

/***********************************************************************
GuiFormatInstanceBinder
***********************************************************************/

		class GuiFormatInstanceBinder : public GuiScriptInstanceBinder
		{
		public:
			WString GetBindingName()override
			{
				return L"format";
			}

			WString TranslateExpression(const WString& input)override
			{
				return L"bind($\"" + input + L"\")";
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
				WfLoadTypes();
			}

			void AfterLoad()override
			{
				{
					IGuiParserManager* manager = GetParserManager();
					manager->SetParsingTable(L"WORKFLOW", &WfLoadTable);
					manager->SetTableParser(L"WORKFLOW", L"WORKFLOW-EXPRESSION", &WfParseExpression);
					manager->SetTableParser(L"WORKFLOW", L"WORKFLOW-MODULE", &WfParseModule);
				}
				{
					IGuiInstanceLoaderManager* manager=GetInstanceLoaderManager();

					manager->AddInstanceBindingContextFactory(new GuiInstanceBindingContextFactory<GuiWorkflowGlobalContext>(L"WORKFLOW-GLOBAL-CONTEXT"));

					manager->AddInstanceBinder(new GuiResourceInstanceBinder);
					manager->AddInstanceBinder(new GuiReferenceInstanceBinder);
					manager->AddInstanceBinder(new GuiBindInstanceBinder);
					manager->AddInstanceBinder(new GuiFormatInstanceBinder);
				}
			}

			void Unload()override
			{
			}
		};
		GUI_REGISTER_PLUGIN(GuiPredefinedInstanceBindersPlugin)
	}
}