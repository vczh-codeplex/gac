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

		InstanceLoadingSource FindInstanceLoadingSource(
			Ptr<GuiInstanceEnvironment> env,
			GuiConstructorRepr* ctor
			);

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

		description::Value LoadInstanceInternal(
			Ptr<GuiInstanceEnvironment> env,
			GuiConstructorRepr* ctor,
			description::ITypeDescriptor* expectedType,
			WString& typeName,
			List<FillInstanceBindingSetter>& bindingSetters
			);

		Ptr<GuiInstanceContextScope> LoadInstanceInternal(
			Ptr<GuiInstanceContext> context,
			Ptr<GuiResourcePathResolver> resolver,
			description::ITypeDescriptor* expectedType
			);

		namespace visitors
		{
/***********************************************************************
FindTypeVisitor
***********************************************************************/

			class FindTypeVisitor : public Object, public GuiInstancePattern::IVisitor
			{
			public:
				Ptr<GuiInstanceEnvironment>		env;
				WString							typeName;
				InstanceLoadingSource			result;

				FindTypeVisitor(Ptr<GuiInstanceEnvironment> _env, const WString& _typeName)
					:env(_env)
					,typeName(_typeName)
				{
				}

				Ptr<GuiInstanceContext> FindInstanceContextInFolder(Ptr<GuiResourceFolder> folder)
				{
					FOREACH(Ptr<GuiResourceItem>, item, folder->GetItems())
					{
						Ptr<GuiInstanceContext> context=item->GetContent().Cast<GuiInstanceContext>();
						if(context) return context;
					}
					FOREACH(Ptr<GuiResourceFolder>, folder, folder->GetFolders())
					{
						Ptr<GuiInstanceContext> context=FindInstanceContextInFolder(folder);
						if(context) return context;
					}
					return 0;
				}

				Ptr<GuiInstanceContext> FindInstanceContext(Ptr<Object> resource)
				{
					if(auto context=resource.Cast<GuiInstanceContext>())
					{
						return context;
					}
					else if(auto folder=resource.Cast<GuiResourceFolder>())
					{
						return FindInstanceContextInFolder(folder);
					}
					return 0;
				}

				void Visit(GuiInstanceResourcePattern* ns)
				{
					Ptr<Object> resource=env->resolver->ResolveResource(ns->protocol, ns->path);
					Ptr<GuiInstanceContext> context=FindInstanceContext(resource);
					if(context)
					{
						result=InstanceLoadingSource(context);
					}
				}

				void Visit(GuiInstanceNamePattern* ns)
				{
					WString fullName=ns->prefix+typeName+ns->postfix;
					IGuiInstanceLoader* loader=GetInstanceLoaderManager()->GetLoader(fullName);
					if(loader)
					{
						result=InstanceLoadingSource(loader, fullName);
					}
				}

				static InstanceLoadingSource FindType(GuiInstancePattern* pattern, Ptr<GuiInstanceEnvironment> env, const WString& typeName)
				{
					FindTypeVisitor visitor(env, typeName);
					pattern->Accept(&visitor);
					return visitor.result;
				}
			};

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
						loadedValue=LoadInstanceInternal(env, repr, typeDescriptor, _typeName, bindingSetters);
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
				FOREACH(Ptr<GuiInstancePattern>, pattern, namespaceInfo->patterns)
				{
					InstanceLoadingSource source=FindTypeVisitor::FindType(pattern.Obj(), env, ctor->typeName);
					if(source) return source;
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
Helper Functions
***********************************************************************/

		description::Value LoadInstanceInternal(
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
				}

				// if the target type is not the expected type, fail
				if (!expectedType || typeDescriptor->CanConvertTo(expectedType))
				{
					// traverse the loader and all ancestors to load the type
					IGuiInstanceLoader::TypeInfo typeInfo(typeName, typeDescriptor);
					while(loader && instance.IsNull())
					{
						if (singleTextValue && loader->IsDeserializable(typeInfo))
						{
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
				if (Ptr<GuiInstanceContextScope> scope = LoadInstanceInternal(source.context, env->resolver, expectedType))
				{
					typeName = scope->typeName;
					instance = scope->rootInstance;
					instanceLoader=GetInstanceLoaderManager()->GetLoader(typeName);
				}
			}

			if(instance.GetRawPtr() && instanceLoader)
			{
				FillInstance(instance, env, ctor, instanceLoader, deserialized, typeName, bindingSetters);

				vint index = ctor->referenceAttributes.Keys().IndexOf(L"Name");
				if (index != -1)
				{
					WString referenceName = ctor->referenceAttributes.Values()[index];
					if (!env->scope->referenceValues.Keys().Contains(referenceName))
					{
						env->scope->referenceValues.Add(referenceName, instance);
					}
				}
			}

			return instance;
		}

		Ptr<GuiInstanceContextScope> LoadInstanceInternal(
			Ptr<GuiInstanceContext> context,
			Ptr<GuiResourcePathResolver> resolver,
			description::ITypeDescriptor* expectedType
			)
		{
			Ptr<GuiInstanceEnvironment> env = new GuiInstanceEnvironment(context, resolver);
			List<FillInstanceBindingSetter> bindingSetters;
			env->scope->rootInstance=LoadInstanceInternal(env, context->instance.Obj(), expectedType, env->scope->typeName, bindingSetters);

			if (!env->scope->rootInstance.IsNull())
			{
				FOREACH(FillInstanceBindingSetter, bindingSetter, bindingSetters)
				{
					if (!bindingSetter.binder->SetPropertyValue(env, bindingSetter.loader, bindingSetter.propertyValue, bindingSetter.currentIndex))
					{
						bindingSetter.propertyValue.propertyValue.DeleteRawPtr();
					}
				}
				return env->scope;
			}
			return 0;
		}

/***********************************************************************
Helper Functions
***********************************************************************/

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
				return LoadInstanceInternal(context, resolver, expectedType);
			}
			return 0;
		}
	}
}