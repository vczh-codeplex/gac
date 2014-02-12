#include "GuiInstanceLoader.h"
#include "TypeDescriptors\GuiReflectionEvents.h"
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

			FillInstanceBindingSetter()
				:binder(0)
				,loader(0)
			{
			}
		};

		struct FillInstanceEventSetter
		{
			IGuiInstanceLoader*					loader;
			Ptr<GuiInstanceEventInfo>			eventInfo;
			IGuiInstanceLoader::PropertyValue	propertyValue;
			WString								handlerName;

			FillInstanceEventSetter()
				:loader(0)
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
			List<FillInstanceBindingSetter>& bindingSetters,
			List<FillInstanceEventSetter>& eventSetters
			);

		void FillInstance(
			description::Value createdInstance,
			Ptr<GuiInstanceEnvironment> env,
			GuiAttSetterRepr* attSetter,
			IGuiInstanceLoader* loader,
			bool skipDefaultProperty,
			const WString& typeName,
			List<FillInstanceBindingSetter>& bindingSetters,
			List<FillInstanceEventSetter>& eventSetters
			);

		description::Value CreateInstance(
			Ptr<GuiInstanceEnvironment> env,
			GuiConstructorRepr* ctor,
			description::ITypeDescriptor* expectedType,
			WString& typeName,
			List<FillInstanceBindingSetter>& bindingSetters,
			List<FillInstanceEventSetter>& eventSetters
			);

		void ExecuteBindingSetters(
			Ptr<GuiInstanceEnvironment> env,
			List<FillInstanceBindingSetter>& bindingSetters
			);

		void ExecuteEventSetters(
			description::Value createdInstance,
			Ptr<GuiInstanceEnvironment> env,
			List<FillInstanceEventSetter>& eventSetters
			);

		Ptr<GuiInstanceContextScope> InitializeInstanceFromConstructor(
			Ptr<GuiInstanceEnvironment> env,
			GuiConstructorRepr* ctor,
			IGuiInstanceLoader* instanceLoader,
			const WString& typeName,
			description::Value instance,
			bool deserialized,
			List<FillInstanceBindingSetter>& bindingSetters,
			List<FillInstanceEventSetter>& eventSetters
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
				List<FillInstanceEventSetter>&			eventSetters;
				bool									result;
				Value									loadedValue;

				LoadValueVisitor(Ptr<GuiInstanceEnvironment> _env, List<ITypeDescriptor*>& _acceptableTypes, List<FillInstanceBindingSetter>& _bindingSetters, List<FillInstanceEventSetter>& _eventSetters)
					:env(_env)
					, acceptableTypes(_acceptableTypes)
					, bindingSetters(_bindingSetters)
					, eventSetters(_eventSetters)
					, result(false)
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
						loadedValue=CreateInstance(env, repr, typeDescriptor, _typeName, bindingSetters, eventSetters);
						if (!loadedValue.IsNull())
						{
							result = true;
							return;
						}
					}
				}

				static bool LoadValue(Ptr<GuiValueRepr> valueRepr, Ptr<GuiInstanceEnvironment> env, List<ITypeDescriptor*>& acceptableTypes, List<FillInstanceBindingSetter>& bindingSetters, List<FillInstanceEventSetter>& eventSetters, Value& loadedValue)
				{
					LoadValueVisitor visitor(env, acceptableTypes, bindingSetters, eventSetters);
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
			List<FillInstanceBindingSetter>& bindingSetters,
			List<FillInstanceEventSetter>& eventSetters
			)
		{
			if (propertyValue.propertyName == L"Tag")
			{
				int a = 0;
			}
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
										FillInstance(propertyValue.propertyValue, env, propertyAttSetter.Obj(), propertyInstanceLoader, false, propertyTypeDescriptor->GetTypeName(), bindingSetters, eventSetters);
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
									if (LoadValueVisitor::LoadValue(valueRepr, env, propertyInfo->acceptableTypes, bindingSetters, eventSetters, propertyValue.propertyValue))
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
							FOREACH_INDEXER(Ptr<GuiValueRepr>, valueRepr, index, input)
							{
								if (valueRepr)
								{
									bool canRemoveLoadedValue = false;
									if (binding == L"")
									{
										// default binding: set the value directly
										if (LoadValueVisitor::LoadValue(valueRepr, env, propertyInfo->acceptableTypes, bindingSetters, eventSetters, propertyValue.propertyValue))
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
										if (LoadValueVisitor::LoadValue(valueRepr, env, binderExpectedTypes, bindingSetters, eventSetters, propertyValue.propertyValue))
										{
											canRemoveLoadedValue = true;
											FillInstanceBindingSetter bindingSetter;
											bindingSetter.binder = binder;
											bindingSetter.loader = propertyLoader;
											bindingSetter.propertyValue = propertyValue;
											bindingSetters.Add(bindingSetter);
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
								if (LoadValueVisitor::LoadValue(valueRepr, env, propertyInfo->acceptableTypes, bindingSetters, eventSetters, propertyValue.propertyValue))
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
			List<FillInstanceBindingSetter>& bindingSetters,
			List<FillInstanceEventSetter>& eventSetters
			)
		{
			IGuiInstanceLoader::TypeInfo typeInfo(typeName, createdInstance.GetTypeDescriptor());
			// reverse loop to set the default property (name == L"") after all other properties
			for (vint i = attSetter->setters.Count() - 1; i >= 0; i--)
			{
				WString propertyName=attSetter->setters.Keys()[i];
				if (propertyName == L"" && skipDefaultProperty)
				{
					continue;
				}

				auto propertyValue=attSetter->setters.Values()[i];
				IGuiInstanceLoader* propertyLoader=loader;
				IGuiInstanceLoader::PropertyValue cachedPropertyValue(
					typeInfo,
					propertyName,
					createdInstance
					);
				List<Ptr<GuiValueRepr>> input;
				List<Pair<Value, IGuiInstanceLoader*>> output;

				// extract all loaded property values
				CopyFrom(input, propertyValue->values);
				LoadInstancePropertyValue(env, propertyValue->binding, cachedPropertyValue, input, propertyLoader, false, output, bindingSetters, eventSetters);

				// if there is no binding, set all values into the specified property
				if (propertyValue->binding == L"")
				{
					for (vint i = 0; i < output.Count(); i++)
					{
						auto value = output[i].key;
						auto valueLoader = output[i].value;
						cachedPropertyValue.propertyValue = value;
						if (!valueLoader->SetPropertyValue(cachedPropertyValue))
						{
							value.DeleteRawPtr();
						}
					}
				}
			}

			// attach events
			for (vint i = 0; i < attSetter->eventHandlers.Count(); i++)
			{
				WString eventName = attSetter->eventHandlers.Keys()[i];
				WString handlerName = attSetter->eventHandlers.Values()[i];

				IGuiInstanceLoader::PropertyInfo propertyInfo(
					typeInfo,
					eventName
					);

				// get the loader to attach the event
				Ptr<GuiInstanceEventInfo> eventInfo;
				IGuiInstanceLoader* eventLoader = loader;
				{
					while (eventLoader)
					{
						if (eventInfo = eventLoader->GetEventType(propertyInfo))
						{
							if (eventInfo->support == GuiInstanceEventInfo::NotSupport)
							{
								eventInfo = 0;
							}
							break;
						}
						eventLoader = GetInstanceLoaderManager()->GetParentLoader(eventLoader);
					}
				}

				if (eventInfo)
				{
					FillInstanceEventSetter eventSetter;
					eventSetter.loader = eventLoader;
					eventSetter.eventInfo = eventInfo;
					eventSetter.propertyValue.typeInfo = propertyInfo.typeInfo;
					eventSetter.propertyValue.propertyName = propertyInfo.propertyName;
					eventSetter.propertyValue.instanceValue = createdInstance;
					eventSetter.handlerName = handlerName;
					eventSetters.Add(eventSetter);
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
			List<FillInstanceBindingSetter>& bindingSetters,
			List<FillInstanceEventSetter>& eventSetters
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
				if (!expectedType || expectedType==GetTypeDescriptor<Value>() || typeDescriptor->CanConvertTo(expectedType))
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
										LoadInstancePropertyValue(env, setterValue->binding, propertyValue, input, loader, true, output, bindingSetters, eventSetters);

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
				InitializeInstanceFromConstructor(env, ctor, instanceLoader, typeName, instance, deserialized, bindingSetters, eventSetters);
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
				if (!bindingSetter.binder->SetPropertyValue(env, bindingSetter.loader, bindingSetter.propertyValue))
				{
					bindingSetter.propertyValue.propertyValue.DeleteRawPtr();
				}
			}
		}

/***********************************************************************
ExecuteBindingSetters
***********************************************************************/

		void ExecuteEventSetters(
			description::Value createdInstance,
			Ptr<GuiInstanceEnvironment> env,
			List<FillInstanceEventSetter>& eventSetters
			)
		{
#ifndef VCZH_DEBUG_NO_REFLECTION
			// set all event attributes
			FOREACH(FillInstanceEventSetter, eventSetter, eventSetters)
			{
				// find a correct method
				if (auto group = createdInstance.GetTypeDescriptor()->GetMethodGroupByName(eventSetter.handlerName, true))
				{
					vint count = group->GetMethodCount();
					IMethodInfo* selectedMethod = 0;
					for (vint i = 0; i < count; i++)
					{
						auto method = group->GetMethod(i);
						if (method->GetParameterCount() != 2) goto UNSUPPORTED;

						auto returnType = method->GetReturn();
						auto senderType = method->GetParameter(0)->GetType();
						auto argumentType = method->GetParameter(1)->GetType();
					
						if (returnType->GetDecorator() != ITypeInfo::TypeDescriptor) goto UNSUPPORTED;
						if (returnType->GetTypeDescriptor() != description::GetTypeDescriptor<VoidValue>()) goto UNSUPPORTED;
					
						if (senderType->GetDecorator() != ITypeInfo::RawPtr) goto UNSUPPORTED;
						senderType = senderType->GetElementType();
						if (senderType->GetDecorator() != ITypeInfo::TypeDescriptor) goto UNSUPPORTED;
						if (senderType->GetTypeDescriptor() != description::GetTypeDescriptor<compositions::GuiGraphicsComposition>()) goto UNSUPPORTED;
					
						if (argumentType->GetDecorator() != ITypeInfo::RawPtr) goto UNSUPPORTED;
						argumentType = argumentType->GetElementType();
						if (argumentType->GetDecorator() != ITypeInfo::TypeDescriptor) goto UNSUPPORTED;
						if (argumentType->GetTypeDescriptor() != eventSetter.eventInfo->argumentType) goto UNSUPPORTED;

						selectedMethod = method;
						break;

					UNSUPPORTED:
						continue;
					}

					if (selectedMethod)
					{
						Value proxy = selectedMethod->CreateFunctionProxy(createdInstance);
						if (!proxy.IsNull())
						{
							auto propertyValue = eventSetter.propertyValue;
							propertyValue.propertyValue = proxy;
							eventSetter.loader->SetEventValue(propertyValue);
						}
					}
				}
			}
#endif
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
			List<FillInstanceEventSetter> eventSetters;
			Value instance = CreateInstance(env, context->instance.Obj(), expectedType, env->scope->typeName, bindingSetters, eventSetters);
			
			if (!instance.IsNull())
			{
				ExecuteBindingSetters(env, bindingSetters);
				ExecuteEventSetters(instance, env, eventSetters);
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
			List<FillInstanceBindingSetter>& bindingSetters,
			List<FillInstanceEventSetter>& eventSetters
			)
		{
			// fill all attributes
			FillInstance(instance, env, ctor, instanceLoader, deserialized, typeName, bindingSetters, eventSetters);

			if (ctor->instanceName)
			{
				env->scope->referenceValues.Add(ctor->instanceName.Value(), instance);
			}
			return env->scope;
		}

		extern Ptr<GuiInstanceContextScope> InitializeInstanceFromContext(
			Ptr<GuiInstanceContext> context,
			Ptr<GuiResourcePathResolver> resolver,
			description::Value instance
			)
		{
			List<FillInstanceBindingSetter> bindingSetters;
			List<FillInstanceEventSetter> eventSetters;

			// search for a correct loader
			GuiConstructorRepr* ctor = context->instance.Obj();
			Ptr<GuiInstanceEnvironment> env = new GuiInstanceEnvironment(context, resolver);
			InstanceLoadingSource source=FindInstanceLoadingSource(env, ctor);

			// initialize the instance
			if(source.loader)
			{
				if (auto scope = InitializeInstanceFromConstructor(env, ctor, source.loader, source.typeName, instance, false, bindingSetters, eventSetters))
				{
					ExecuteBindingSetters(env, bindingSetters);
					ExecuteEventSetters(instance, env, eventSetters);
					return scope;
				}
			}
			return 0;
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
					return InitializeInstanceFromContext(context, resolver, instance);
				}
			}
			return 0;
		}
	}
}