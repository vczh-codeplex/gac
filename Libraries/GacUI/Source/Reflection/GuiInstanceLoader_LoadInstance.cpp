#include "GuiInstanceLoader.h"
#include "..\Resources\GuiParserManager.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;
		using namespace reflection::description;

/***********************************************************************
Helper Functions Declarations
***********************************************************************/

		struct FillInstanceBindingSetter
		{
			IGuiInstanceBinder*					binder;
			IGuiInstanceLoader*					loader;
			IGuiInstanceLoader::PropertyValue	propertyValue;
			vint								currentIndex;

			FillInstanceBindingSetter()
				:binder(0)
				,loader(0)
				,currentIndex(-1)
			{
			}
		};

		bool LoadInstancePropertyValue(
			Ptr<GuiInstanceEnvironment> env,
			const WString& binding,
			IGuiInstanceLoader::PropertyValue propertyValue,
			List<Ptr<GuiValueRepr>>& input,
			IGuiInstanceLoader* propertyLoader,
			bool constructorArgument,
			List<Pair<Value, IGuiInstanceLoader*>>& output,
			List<FillInstanceBindingSetter>& bindingSetters
			);

		void FillInstance(
			description::Value createdInstance,
			Ptr<GuiInstanceEnvironment> env,
			GuiAttSetterRepr* attSetter,
			IGuiInstanceLoader* loader,
			bool skipDefaultProperty,
			const WString& typeName,
			List<FillInstanceBindingSetter>& bindingSetters
			);

		description::Value CreateInstance(
			Ptr<GuiInstanceEnvironment> env,
			GuiConstructorRepr* ctor,
			description::ITypeDescriptor* expectedType,
			WString& typeName,
			List<FillInstanceBindingSetter>& bindingSetters
			);

		void ExecuteBindingSetters(
			Ptr<GuiInstanceEnvironment> env,
			List<FillInstanceBindingSetter>& bindingSetters
			);

		Ptr<GuiInstanceContextScope> LoadInstanceFromContext(
			Ptr<GuiInstanceContext> context,
			Ptr<GuiResourcePathResolver> resolver,
			description::ITypeDescriptor* expectedType
			);

		Ptr<GuiInstanceContextScope> InitializeInstanceFromConstructor(
			Ptr<GuiInstanceEnvironment> env,
			GuiConstructorRepr* ctor,
			IGuiInstanceLoader* instanceLoader,
			const WString& typeName,
			description::Value instance,
			bool deserialized,
			List<FillInstanceBindingSetter>& bindingSetters
			);

		namespace visitors
		{

/***********************************************************************
LoadValueVisitor
***********************************************************************/

			class LoadValueVisitor : public Object, public GuiValueRepr::IVisitor
			{
			public:
				Ptr<GuiInstanceEnvironment>				env;
				List<ITypeDescriptor*>&					acceptableTypes;
				List<FillInstanceBindingSetter>&		bindingSetters;
				bool									result;
				Value									loadedValue;

				LoadValueVisitor(Ptr<GuiInstanceEnvironment> _env, List<ITypeDescriptor*>& _acceptableTypes, List<FillInstanceBindingSetter>& _bindingSetters)
					:env(_env)
					,acceptableTypes(_acceptableTypes)
					,bindingSetters(_bindingSetters)
					,result(false)
				{
				}

				void Visit(GuiTextRepr* repr)override
				{
					FOREACH(ITypeDescriptor*, typeDescriptor, acceptableTypes)
					{
						if(IValueSerializer* serializer=typeDescriptor->GetValueSerializer())
						{
							if (serializer->Parse(repr->text, loadedValue))
							{
								result = true;
								return;
							}
						}
					}
				}

				void Visit(GuiAttSetterRepr* repr)override
				{
				}

				void Visit(GuiConstructorRepr* repr)override
				{
					FOREACH(ITypeDescriptor*, typeDescriptor, acceptableTypes)
					{
						WString _typeName;
						loadedValue=CreateInstance(env, repr, typeDescriptor, _typeName, bindingSetters);
						if (!loadedValue.IsNull())
						{
							result = true;
							return;
						}
					}
				}

				static bool LoadValue(Ptr<GuiValueRepr> valueRepr, Ptr<GuiInstanceEnvironment> env, List<ITypeDescriptor*>& acceptableTypes, List<FillInstanceBindingSetter>& bindingSetters, Value& loadedValue)
				{
					LoadValueVisitor visitor(env, acceptableTypes, bindingSetters);
					valueRepr->Accept(&visitor);
					if (visitor.result)
					{
						loadedValue = visitor.loadedValue;
					}
					return visitor.result;
				}
			};

		}
		using namespace visitors;

/***********************************************************************
FindInstanceLoadingSource
***********************************************************************/

		InstanceLoadingSource FindInstanceLoadingSource(
			Ptr<GuiInstanceEnvironment> env,
			GuiConstructorRepr* ctor
			)
		{
			vint index=env->context->namespaces.Keys().IndexOf(ctor->typeNamespace);
			if(index!=-1)
			{
				Ptr<GuiInstanceContext::NamespaceInfo> namespaceInfo=env->context->namespaces.Values()[index];
				FOREACH(Ptr<GuiInstanceNamespace>, ns, namespaceInfo->namespaces)
				{
					WString fullName = ns->prefix + ctor->typeName + ns->postfix;
					IGuiInstanceLoader* loader = GetInstanceLoaderManager()->GetLoader(fullName);
					if(loader)
					{
						return InstanceLoadingSource(loader, fullName);
					}
				}
			}
			return InstanceLoadingSource();
		}

/***********************************************************************
LoadInstancePropertyValue
***********************************************************************/

		bool LoadInstancePropertyValue(
			Ptr<GuiInstanceEnvironment> env,
			const WString& binding,
			IGuiInstanceLoader::PropertyValue propertyValue,
			List<Ptr<GuiValueRepr>>& input,
			IGuiInstanceLoader* propertyLoader,
			bool constructorArgument,
			List<Pair<Value, IGuiInstanceLoader*>>& output,
			List<FillInstanceBindingSetter>& bindingSetters
			)
		{
			vint loadedValueCount = 0;
			// try to look for a loader to handle this property
			while(propertyLoader && loadedValueCount<input.Count())
			{
				if (auto propertyInfo = propertyLoader->GetPropertyType(propertyValue))
				{
					if (propertyInfo->constructorParameter != constructorArgument)
					{
						return false;
					}
					if (propertyInfo->support == GuiInstancePropertyInfo::NotSupport)
					{
						break;
					}

					switch (propertyInfo->support)
					{
					case GuiInstancePropertyInfo::SupportSet:
						if (input.Count() != 1) return false;
						if (constructorArgument) return false;
						if (binding != L"set") return false;
						{
							// set binding: get the property value and apply another property list on it
							if(Ptr<GuiAttSetterRepr> propertyAttSetter=input[0].Cast<GuiAttSetterRepr>())
							{
								if(propertyLoader->GetPropertyValue(propertyValue) && propertyValue.propertyValue.GetRawPtr())
								{
									input[0] = 0;
									loadedValueCount++;

									ITypeDescriptor* propertyTypeDescriptor=propertyValue.propertyValue.GetRawPtr()->GetTypeDescriptor();
									IGuiInstanceLoader* propertyInstanceLoader=GetInstanceLoaderManager()->GetLoader(propertyTypeDescriptor->GetTypeName());
									if(propertyInstanceLoader)
									{
										FillInstance(propertyValue.propertyValue, env, propertyAttSetter.Obj(), propertyInstanceLoader, false, propertyTypeDescriptor->GetTypeName(), bindingSetters);
									}
								}
							}
						}
						break;
					case GuiInstancePropertyInfo::SupportCollection:
						if (binding != L"") return false;
						{
							FOREACH_INDEXER(Ptr<GuiValueRepr>, valueRepr, index, input)
							{
								if (valueRepr)
								{
									// default binding: set the value directly
									if (LoadValueVisitor::LoadValue(valueRepr, env, propertyInfo->acceptableTypes, bindingSetters, propertyValue.propertyValue))
									{
										input[index] = 0;
										loadedValueCount++;
										output.Add(Pair<Value, IGuiInstanceLoader*>(propertyValue.propertyValue, propertyLoader));
									}
								}
							}
						}
						break;
					case GuiInstancePropertyInfo::SupportAssign:
						if (input.Count() != 1) return false;
						if (constructorArgument && binding != L"") return false;
						if (binding == L"set") return false;
						{
							vint currentIndex = 0;
							FOREACH_INDEXER(Ptr<GuiValueRepr>, valueRepr, index, input)
							{
								if (valueRepr)
								{
									bool canRemoveLoadedValue = false;
									if (binding == L"")
									{
										// default binding: set the value directly
										if (LoadValueVisitor::LoadValue(valueRepr, env, propertyInfo->acceptableTypes, bindingSetters, propertyValue.propertyValue))
										{
											canRemoveLoadedValue = true;
											output.Add(Pair<Value, IGuiInstanceLoader*>(propertyValue.propertyValue, propertyLoader));
										}
									}
									else if (IGuiInstanceBinder* binder=GetInstanceLoaderManager()->GetInstanceBinder(binding))
									{
										// other binding: provide the property value to the specified binder
										List<ITypeDescriptor*> binderExpectedTypes;
										binder->GetExpectedValueTypes(binderExpectedTypes);
										if (LoadValueVisitor::LoadValue(valueRepr, env, binderExpectedTypes, bindingSetters, propertyValue.propertyValue))
										{
											canRemoveLoadedValue = true;
											currentIndex++;
											FillInstanceBindingSetter bindingSetter;
											bindingSetter.binder = binder;
											bindingSetter.loader = propertyLoader;
											bindingSetter.propertyValue = propertyValue;
											bindingSetter.currentIndex = currentIndex;
											bindingSetters.Add(bindingSetter);
											currentIndex++;
										}
									}

									if (canRemoveLoadedValue)
									{
										input[index] = 0;
										loadedValueCount++;
									}
								}
							}
						}
						break;
					case GuiInstancePropertyInfo::SupportArray:
						if (binding != L"") return false;
						{
							auto list = IValueList::Create();
							FOREACH_INDEXER(Ptr<GuiValueRepr>, valueRepr, index, input)
							{
								// default binding: add the value to the list
								if (LoadValueVisitor::LoadValue(valueRepr, env, propertyInfo->acceptableTypes, bindingSetters, propertyValue.propertyValue))
								{
									input[index] = 0;
									loadedValueCount++;
									list->Add(propertyValue.propertyValue);
								}
							}

							// set the whole list to the property
							output.Add(Pair<Value, IGuiInstanceLoader*>(Value::From(list), propertyLoader));
						}
						break;
					}

					if (!propertyInfo->tryParent)
					{
						break;
					}
				}
				if (constructorArgument)
				{
					break;
				}
				else
				{
					propertyLoader=GetInstanceLoaderManager()->GetParentLoader(propertyLoader);
				}
			}
			return true;
		}

/***********************************************************************
FillInstance
***********************************************************************/

		void FillInstance(
			description::Value createdInstance,
			Ptr<GuiInstanceEnvironment> env,
			GuiAttSetterRepr* attSetter,
			IGuiInstanceLoader* loader,
			bool skipDefaultProperty,
			const WString& typeName,
			List<FillInstanceBindingSetter>& bindingSetters
			)
		{
			// reverse loop to set the default property (name == L"") after all other properties
			for(vint i=attSetter->setters.Count()-1;i>=0;i--)
			{
				WString propertyName=attSetter->setters.Keys()[i];
				if (propertyName == L"" && skipDefaultProperty)
				{
					continue;
				}

				auto propertyValue=attSetter->setters.Values()[i];
				IGuiInstanceLoader* propertyLoader=loader;
				IGuiInstanceLoader::PropertyValue cachedPropertyValue(
					IGuiInstanceLoader::TypeInfo(typeName, createdInstance.GetTypeDescriptor()),
					propertyName,
					createdInstance
					);
				List<Ptr<GuiValueRepr>> input;
				List<Pair<Value, IGuiInstanceLoader*>> output;

				// extract all loaded property values
				CopyFrom(input, propertyValue->values);
				LoadInstancePropertyValue(env, propertyValue->binding, cachedPropertyValue, input, propertyLoader, false, output, bindingSetters);

				// if there is no binding, set all values into the specified property
				if (propertyValue->binding == L"")
				{
					vint currentIndex = 0;
					for (vint i = 0; i < output.Count(); i++)
					{
						auto value = output[i].key;
						auto valueLoader = output[i].value;
						cachedPropertyValue.propertyValue = value;
						if (valueLoader->SetPropertyValue(cachedPropertyValue, currentIndex))
						{
							currentIndex++;
						}
						else
						{
							value.DeleteRawPtr();
						}
					}
				}
			}
		}

/***********************************************************************
CreateInstance
***********************************************************************/

		description::Value CreateInstance(
			Ptr<GuiInstanceEnvironment> env,
			GuiConstructorRepr* ctor,
			description::ITypeDescriptor* expectedType,
			WString& typeName,
			List<FillInstanceBindingSetter>& bindingSetters
			)
		{
			// search for a correct loader
			InstanceLoadingSource source=FindInstanceLoadingSource(env, ctor);
			Value instance;
			IGuiInstanceLoader* instanceLoader = 0;
			bool deserialized = false;

			if(source.loader)
			{
				// found the correct loader, prepare a TypeInfo
				IGuiInstanceLoader* loader=source.loader;
				instanceLoader = source.loader;
				typeName=source.typeName;
				ITypeDescriptor* typeDescriptor=GetInstanceLoaderManager()->GetTypeDescriptorForType(source.typeName);
				
				// see if the constructor contains only a single text value
				Ptr<GuiTextRepr> singleTextValue;
				{
					vint index = ctor->setters.Keys().IndexOf(L"");
					if (index != -1)
					{
						auto setterValue = ctor->setters.Values()[index];
						if (setterValue->values.Count() == 1)
						{
							singleTextValue = setterValue->values[0].Cast<GuiTextRepr>();
						}
					}
					else
					{
						singleTextValue = new GuiTextRepr;
						singleTextValue->text = L"";
					}
				}

				// if the target type is not the expected type, fail
				if (!expectedType || typeDescriptor->CanConvertTo(expectedType))
				{
					// traverse the loader and all ancestors to load the type
					IGuiInstanceLoader::TypeInfo typeInfo(typeName, typeDescriptor);
					bool foundLoader = false;
					while(!foundLoader && loader && instance.IsNull())
					{
						if (singleTextValue && loader->IsDeserializable(typeInfo))
						{
							foundLoader = true;
							// if the loader support deserialization and this is a single text value constructor
							// then choose deserialization
							instance = loader->Deserialize(typeInfo, singleTextValue->text);
							if (!instance.IsNull())
							{
								deserialized = true;
							}
						}
						else if (loader->IsCreatable(typeInfo))
						{
							foundLoader = true;
							// find all constructor parameters
							List<WString> constructorParameters;
							List<WString> requiredParameters;
							loader->GetConstructorParameters(typeInfo, constructorParameters);
							
							// see if all parameters exists
							Group<WString, Value> constructorArguments;
							FOREACH(WString, propertyName, constructorParameters)
							{
								IGuiInstanceLoader::PropertyInfo propertyInfo(typeInfo, propertyName);
								auto info = loader->GetPropertyType(propertyInfo);
								vint index = ctor->setters.Keys().IndexOf(propertyName);

								if (info->constructorParameter)
								{
									if (info->required)
									{
										if (index == -1)
										{
											// if a required parameter doesn't exist, fail
											goto SKIP_CREATE_INSTANCE;
										}
										requiredParameters.Add(propertyName);
									}

									if (index != -1)
									{
										auto setterValue = ctor->setters.Values()[index];
										if (setterValue->binding != L"")
										{
											// if the constructor argument uses binding, fail
											goto SKIP_CREATE_INSTANCE;
										}

										// load the parameter
										List<Ptr<GuiValueRepr>> input;
										List<Pair<Value, IGuiInstanceLoader*>> output;
										IGuiInstanceLoader::PropertyValue propertyValue(typeInfo, propertyName, Value());

										CopyFrom(input, setterValue->values);
										LoadInstancePropertyValue(env, setterValue->binding, propertyValue, input, loader, true, output, bindingSetters);

										for (vint i = 0; i < output.Count(); i++)
										{
											constructorArguments.Add(propertyName, output[i].key);
										}
									}
								}
							}
							
							// check if all required parameters exist
							FOREACH(WString, propertyName, requiredParameters)
							{
								if (!constructorArguments.Contains(propertyName))
								{
									goto SKIP_CREATE_INSTANCE;
								}
							}

							// create the instance
							instance=loader->CreateInstance(typeInfo, constructorArguments);
						SKIP_CREATE_INSTANCE:
							// delete all arguments if the constructing fails
							if (instance.IsNull())
							{
								for (vint i = 0; i < constructorArguments.Count(); i++)
								{
									FOREACH(Value, value, constructorArguments.GetByIndex(i))
									{
										value.DeleteRawPtr();
									}
								}
							}
						}
						loader=GetInstanceLoaderManager()->GetParentLoader(loader);
					}
				}
			}
			else if(source.context)
			{
				// found another instance in the resource
				if (Ptr<GuiInstanceContextScope> scope = LoadInstanceFromContext(source.context, env->resolver, expectedType))
				{
					typeName = scope->typeName;
					instance = scope->rootInstance;
					instanceLoader=GetInstanceLoaderManager()->GetLoader(typeName);
				}
			}

			if(instance.GetRawPtr() && instanceLoader)
			{
				InitializeInstanceFromConstructor(env, ctor, instanceLoader, typeName, instance, deserialized, bindingSetters);
			}
			return instance;
		}

/***********************************************************************
ExecuteBindingSetters
***********************************************************************/

		void ExecuteBindingSetters(
			Ptr<GuiInstanceEnvironment> env,
			List<FillInstanceBindingSetter>& bindingSetters
			)
		{
			// set all -bind attributes
			FOREACH(FillInstanceBindingSetter, bindingSetter, bindingSetters)
			{
				if (!bindingSetter.binder->SetPropertyValue(env, bindingSetter.loader, bindingSetter.propertyValue, bindingSetter.currentIndex))
				{
					bindingSetter.propertyValue.propertyValue.DeleteRawPtr();
				}
			}
		}

/***********************************************************************
LoadInstance
***********************************************************************/

		Ptr<GuiInstanceContextScope> LoadInstanceFromContext(
			Ptr<GuiInstanceContext> context,
			Ptr<GuiResourcePathResolver> resolver,
			description::ITypeDescriptor* expectedType
			)
		{
			Ptr<GuiInstanceEnvironment> env = new GuiInstanceEnvironment(context, resolver);
			List<FillInstanceBindingSetter> bindingSetters;
			Value instance = CreateInstance(env, context->instance.Obj(), expectedType, env->scope->typeName, bindingSetters);
			
			if (!instance.IsNull())
			{
				ExecuteBindingSetters(env, bindingSetters);
				env->scope->rootInstance = instance;
				return env->scope;
			}
			return 0;
		}

		Ptr<GuiInstanceContextScope> LoadInstance(
			Ptr<GuiResource> resource,
			const WString& instancePath,
			description::ITypeDescriptor* expectedType
			)
		{
			Ptr<GuiInstanceContext> context=resource->GetValueByPath(instancePath).Cast<GuiInstanceContext>();
			if (context)
			{
				Ptr<GuiResourcePathResolver> resolver = new GuiResourcePathResolver(resource, resource->GetWorkingDirectory());
				return LoadInstanceFromContext(context, resolver, expectedType);
			}
			return 0;
		}

/***********************************************************************
InitializeInstance
***********************************************************************/

		Ptr<GuiInstanceContextScope> InitializeInstanceFromConstructor(
			Ptr<GuiInstanceEnvironment> env,
			GuiConstructorRepr* ctor,
			IGuiInstanceLoader* instanceLoader,
			const WString& typeName,
			description::Value instance,
			bool deserialized,
			List<FillInstanceBindingSetter>& bindingSetters
			)
		{
			// fill all attributes
			FillInstance(instance, env, ctor, instanceLoader, deserialized, typeName, bindingSetters);

			if (ctor->instanceName)
			{
				env->scope->referenceValues.Add(ctor->instanceName.Value(), instance);
			}
			return env->scope;
		}

		Ptr<GuiInstanceContextScope> InitializeInstance(
			Ptr<GuiResource> resource,
			const WString& instancePath,
			description::Value instance
			)
		{
			if (instance.GetRawPtr())
			{
				Ptr<GuiInstanceContext> context=resource->GetValueByPath(instancePath).Cast<GuiInstanceContext>();
				if (context)
				{
					Ptr<GuiResourcePathResolver> resolver = new GuiResourcePathResolver(resource, resource->GetWorkingDirectory());
					List<FillInstanceBindingSetter> bindingSetters;

					// search for a correct loader
					GuiConstructorRepr* ctor = context->instance.Obj();
					Ptr<GuiInstanceEnvironment> env = new GuiInstanceEnvironment(context, resolver);
					InstanceLoadingSource source=FindInstanceLoadingSource(env, ctor);

					// initialize the instance
					if(source.loader)
					{
						if (auto scope = InitializeInstanceFromConstructor(env, ctor, source.loader, source.typeName, instance, false, bindingSetters))
						{
							ExecuteBindingSetters(env, bindingSetters);
							return scope;
						}
					}
				}
			}
			return 0;
		}
	}
}