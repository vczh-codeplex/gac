#include "GuiInstanceLoader.h"
#include "GuiInstanceLoader_WorkflowCompiler.h"
#include "..\Controls\GuiApplication.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;
		using namespace reflection::description;
		using namespace workflow;
		using namespace workflow::analyzer;
		using namespace workflow::runtime;
		using namespace controls;

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

			bool ApplicableToConstructorArgument()override
			{
				return false;
			}

			bool RequireInstanceName()override
			{
				return false;
			}

			void GetRequiredContexts(collections::List<WString>& contextNames)override
			{
			}

			void GetExpectedValueTypes(collections::List<description::ITypeDescriptor*>& expectedTypes)override
			{
				expectedTypes.Add(stringTypeDescriptor);
			}

			description::Value GetValue(Ptr<GuiInstanceEnvironment> env, const description::Value& propertyValue)override
			{
				return Value();
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

			bool SetPropertyValue(Ptr<GuiInstanceEnvironment> env, IGuiInstanceLoader* loader, Nullable<WString> instanceName, IGuiInstanceLoader::PropertyValue& propertyValue)override
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

			bool SetPropertyValue(Ptr<GuiInstanceEnvironment> env, IGuiInstanceLoader* loader, Nullable<WString> instanceName, IGuiInstanceLoader::PropertyValue& propertyValue)override
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
GuiWorkflowCache
***********************************************************************/

		class GuiWorkflowCache : public Object, public IGuiInstanceCache
		{
		public:
			static const wchar_t*			CacheTypeName;

			Ptr<WfAssembly>					assembly;

			GuiWorkflowCache()
			{
			}

			GuiWorkflowCache(Ptr<WfAssembly> _assembly)
				:assembly(_assembly)
			{
			}

			WString GetCacheTypeName()override
			{
				return CacheTypeName;
			}
		};

		const wchar_t* GuiWorkflowCache::CacheTypeName = L"WORKFLOW-ASSEMBLY-CACHE";

		class GuiWorkflowCacheResolver : public Object, public IGuiInstanceCacheResolver
		{
		public:
			WString GetCacheTypeName()override
			{
				return GuiWorkflowCache::CacheTypeName;
			}

			bool Serialize(Ptr<IGuiInstanceCache> cache, stream::IStream& stream)override
			{
				if (auto obj = cache.Cast<GuiWorkflowCache>())
				{
					obj->assembly->Serialize(stream);
					return true;
				}
				else
				{
					return false;
				}
			}

			Ptr<IGuiInstanceCache> Deserialize(stream::IStream& stream)override
			{
				auto assembly = new WfAssembly(stream);
				return new GuiWorkflowCache(assembly);
			}
		};

/***********************************************************************
GuiWorkflowGlobalContext
***********************************************************************/

		class GuiWorkflowGlobalContext : public Object, public IGuiInstanceBindingContext
		{
		public:
			static const wchar_t*			ContextName;

			List<WorkflowDataBinding>		dataBindings;
			Ptr<WfRuntimeGlobalContext>		globalContext;

			GuiWorkflowGlobalContext()
			{
			}

			WString GetContextName()override
			{
				return ContextName;
			}

			void Initialize(Ptr<GuiInstanceEnvironment> env)override
			{
				Ptr<WfAssembly> assembly;
				vint cacheIndex = env->context->precompiledCaches.Keys().IndexOf(GetContextName());
				if (cacheIndex != -1)
				{
					assembly = env->context->precompiledCaches.Values()[cacheIndex].Cast<GuiWorkflowCache>()->assembly;
				}
				else
				{
					types::VariableTypeMap types;
					ITypeDescriptor* thisType = env->scope->rootInstance.GetTypeDescriptor();
					Workflow_GetVariableTypes(env, types);
					assembly = Workflow_CompileDataBinding(types, thisType, env->scope->errors, dataBindings);
					env->context->precompiledCaches.Add(GetContextName(), new GuiWorkflowCache(assembly));
				}

				if (assembly)
				{
					globalContext = new WfRuntimeGlobalContext(assembly);
				
					LoadFunction<void()>(globalContext, L"<initialize>")();
					Workflow_SetVariablesForReferenceValues(globalContext, env);
					{
						vint index = assembly->variableNames.IndexOf(L"<this>");
						globalContext->globalVariables->variables[index] = env->scope->rootInstance;
					}
					LoadFunction<void()>(globalContext, L"<initialize-data-binding>")();
				}
			}
		};

		const wchar_t* GuiWorkflowGlobalContext::ContextName = L"WORKFLOW-GLOBAL-CONTEXT";

/***********************************************************************
GuiScriptInstanceBinder
***********************************************************************/

		class GuiScriptInstanceBinder : public GuiTextInstanceBinderBase
		{
		public:
			virtual WString TranslateExpression(const WString& input) = 0;

			bool RequireInstanceName()override
			{
				return true;
			}

			void GetRequiredContexts(collections::List<WString>& contextNames)override
			{
				contextNames.Add(GuiWorkflowGlobalContext::ContextName);
			}

			bool SetPropertyValue(Ptr<GuiInstanceEnvironment> env, IGuiInstanceLoader* loader, Nullable<WString> instanceName, IGuiInstanceLoader::PropertyValue& propertyValue)override
			{
				auto context = env->scope->bindingContexts[GuiWorkflowGlobalContext::ContextName].Cast<GuiWorkflowGlobalContext>();
				WorkflowDataBinding dataBinding;
				dataBinding.variableName = instanceName.Value();

				if (env->context->precompiledCaches.Keys().Contains(GuiWorkflowGlobalContext::ContextName))
				{
					goto SUCCESS;
				}
				if (propertyValue.propertyValue.GetValueType() == Value::Text)
				{
					WString expressionCode = TranslateExpression(propertyValue.propertyValue.GetText());
					Ptr<WfExpression> expression;
					types::VariableTypeMap types;
					Workflow_GetVariableTypes(env, types);
					if (Workflow_ValidateExpression(types, env->scope->errors, propertyValue, expressionCode, expression))
					{
						auto expr = expression;
						if (auto bind = expr.Cast<WfBindExpression>())
						{
							bind->expandedExpression = 0;
							expr = bind->expression;
						}
						if (auto format = expr.Cast<WfFormatExpression>())
						{
							format->expandedExpression = 0;
						}
						
						auto td = propertyValue.typeInfo.typeDescriptor;
						auto propertyInfo = td->GetPropertyByName(propertyValue.propertyName, true);
						dataBinding.propertyInfo = propertyInfo;
						dataBinding.bindExpression = expression;
						goto SUCCESS;
					}
					else
					{
						goto FAILED;
					}
				}

			FAILED:
				context->dataBindings.Add(dataBinding);
				return false;
			SUCCESS:
				context->dataBindings.Add(dataBinding);
				return true;
			}
		};

/***********************************************************************
GuiEvalInstanceBinder
***********************************************************************/

		class GuiEvalInstanceBinder : public GuiScriptInstanceBinder
		{
		public:
			WString GetBindingName()override
			{
				return L"eval";
			}

			bool ApplicableToConstructorArgument()override
			{
				return true;
			}

			description::Value GetValue(Ptr<GuiInstanceEnvironment> env, const description::Value& propertyValue)override
			{
				if (propertyValue.GetValueType() == Value::Text)
				{
					Ptr<WfAssembly> assembly;
					WString expressionCode = TranslateExpression(propertyValue.GetText());
					WString cacheKey = L"<att.eval>" + expressionCode;
					vint cacheIndex = env->context->precompiledCaches.Keys().IndexOf(cacheKey);
					if (cacheIndex != -1)
					{
						assembly = env->context->precompiledCaches.Values()[cacheIndex].Cast<GuiWorkflowCache>()->assembly;
					}
					else
					{
						types::VariableTypeMap types;
						Workflow_GetVariableTypes(env, types);
						assembly = Workflow_CompileExpression(types, env->scope->errors, expressionCode);
						env->context->precompiledCaches.Add(cacheKey, new GuiWorkflowCache(assembly));
					}

					if (assembly)
					{
						auto globalContext = MakePtr<WfRuntimeGlobalContext>(assembly);
				
						LoadFunction<void()>(globalContext, L"<initialize>")();
						Workflow_SetVariablesForReferenceValues(globalContext, env);
						vint variableIndex = assembly->variableNames.IndexOf(L"<initialize-data-binding>");
						auto variable = globalContext->globalVariables->variables[variableIndex];
						auto proxy = UnboxValue<Ptr<IValueFunctionProxy>>(variable);
						auto translated = proxy->Invoke(IValueList::Create());

						// the global context contains a closure variable <initialize-data-binding> which captured the context
						// clear all variables to break the circle references
						globalContext->globalVariables = 0;
						return translated;
					}
				}
				return Value();
			}

			WString TranslateExpression(const WString& input)override
			{
				return input;
			}
		};

/***********************************************************************
GuiEvalInstanceEventBinder
***********************************************************************/

		class GuiEvalInstanceEventBinder : public Object, public IGuiInstanceEventBinder
		{
		public:
			WString GetBindingName()override
			{
				return L"eval";
			}

			bool RequireInstanceName()override
			{
				return true;
			}

			void GetRequiredContexts(collections::List<WString>& contextNames)override
			{
			}

			bool AttachEvent(Ptr<GuiInstanceEnvironment> env, IGuiInstanceLoader* loader, Nullable<WString> instanceName, IGuiInstanceLoader::PropertyValue& propertyValue)
			{
				auto handler = propertyValue.propertyValue;
				if (handler.GetValueType() == Value::Text)
				{
					Ptr<WfAssembly> assembly;
					WString statementCode = handler.GetText();
					WString cacheKey = L"<ev.eval><" + instanceName.Value() + L"><" + propertyValue.propertyName + L">" + statementCode;
					vint cacheIndex = env->context->precompiledCaches.Keys().IndexOf(cacheKey);
					if (cacheIndex != -1)
					{
						assembly = env->context->precompiledCaches.Values()[cacheIndex].Cast<GuiWorkflowCache>()->assembly;
					}
					else
					{
						types::VariableTypeMap types;
						Workflow_GetVariableTypes(env, types);
						assembly = Workflow_CompileEventHandler(types, env->scope->errors, propertyValue, statementCode);
						env->context->precompiledCaches.Add(cacheKey, new GuiWorkflowCache(assembly));
					}

					if (assembly)
					{
						auto globalContext = MakePtr<WfRuntimeGlobalContext>(assembly);
				
						LoadFunction<void()>(globalContext, L"<initialize>")();
						Workflow_SetVariablesForReferenceValues(globalContext, env);
						auto eventHandler = LoadFunction(globalContext, L"<event-handler>");
						handler = BoxValue(eventHandler);

						propertyValue.propertyValue = handler;
						return loader->SetEventValue(propertyValue);
					}
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
			GuiPredefinedInstanceBindersPlugin()
			{
			}

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
					manager->SetTableParser(L"WORKFLOW", L"WORKFLOW-STATEMENT", &WfParseStatement);
					manager->SetTableParser(L"WORKFLOW", L"WORKFLOW-MODULE", &WfParseModule);
					manager->SetParsingTable(L"INSTANCE-QUERY", &GuiIqLoadTable);
					manager->SetTableParser(L"INSTANCE-QUERY", L"INSTANCE-QUERY", &GuiIqParse);
				}
				{
					IGuiInstanceLoaderManager* manager=GetInstanceLoaderManager();

					manager->AddInstanceBindingContextFactory(new GuiInstanceBindingContextFactory<GuiWorkflowGlobalContext>(GuiWorkflowGlobalContext::ContextName));
					manager->AddInstanceCacheResolver(new GuiWorkflowCacheResolver);

					manager->AddInstanceBinder(new GuiResourceInstanceBinder);
					manager->AddInstanceBinder(new GuiReferenceInstanceBinder);
					manager->AddInstanceBinder(new GuiEvalInstanceBinder);
					manager->AddInstanceEventBinder(new GuiEvalInstanceEventBinder);
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