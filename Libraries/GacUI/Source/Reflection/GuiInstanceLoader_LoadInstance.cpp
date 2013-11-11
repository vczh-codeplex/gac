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

		void FillInstance(
			description::Value createdInstance,
			Ptr<GuiInstanceEnvironment> env,
			GuiAttSetterRepr* attSetter,
			IGuiInstanceLoader* loader,
			bool skipDefaultProperty,
			const WString& typeName,
			List<FillInstanceBindingSetter>& bindingSetters
			);

		description::Value LoadInstance(
			Ptr<GuiInstanceEnvironment> env,
			GuiConstructorRepr* ctor,
			description::ITypeDescriptor* expectedType,
			WString& typeName,
			List<FillInstanceBindingSetter>& bindingSetters
			);

		Ptr<GuiInstanceContextScope> LoadInstance(
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
						loadedValue=LoadInstance(env, repr, typeDescriptor, _typeName, bindingSetters);
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
Helper Functions
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

				List<Ptr<GuiValueRepr>> values;
				CopyFrom(values, propertyValue->values);
				vint loadedValueCount = 0;

				// try to look for a loader to handle this property
				while(propertyLoader && loadedValueCount<values.Count())
				{
					if (auto propertyInfo = propertyLoader->GetPropertyType(cachedPropertyValue))
					{
						if (propertyInfo->support == GuiInstancePropertyInfo::NotSupport)
						{
							break;
						}

						switch (propertyInfo->support)
						{
						case GuiInstancePropertyInfo::SupportSet:
							if (values.Count() != 1) return;
							if (propertyValue->binding == L"set")
							{

								// set binding: get the property value and apply another property list on it
								if(Ptr<GuiAttSetterRepr> propertyAttSetter=values[0].Cast<GuiAttSetterRepr>())
								{
									if(propertyLoader->GetPropertyValue(cachedPropertyValue) && cachedPropertyValue.propertyValue.GetRawPtr())
									{
										values[0] = 0;
										loadedValueCount++;

										ITypeDescriptor* propertyTypeDescriptor=cachedPropertyValue.propertyValue.GetRawPtr()->GetTypeDescriptor();
										IGuiInstanceLoader* propertyInstanceLoader=GetInstanceLoaderManager()->GetLoader(propertyTypeDescriptor->GetTypeName());
										if(propertyInstanceLoader)
										{
											FillInstance(cachedPropertyValue.propertyValue, env, propertyAttSetter.Obj(), propertyInstanceLoader, false, propertyTypeDescriptor->GetTypeName(), bindingSetters);
										}
									}
								}
							}
							break;
						case GuiInstancePropertyInfo::SupportCollection:
							if (propertyValue->binding == L"")
							{
								vint currentIndex = 0;
								FOREACH_INDEXER(Ptr<GuiValueRepr>, valueRepr, index, values)
								{
									if (valueRepr)
									{
										// default binding: set the value directly
										if (LoadValueVisitor::LoadValue(valueRepr, env, propertyInfo->acceptableTypes, bindingSetters, cachedPropertyValue.propertyValue))
										{
											values[index] = 0;
											loadedValueCount++;

											if(propertyLoader->SetPropertyValue(cachedPropertyValue, currentIndex))
											{
												currentIndex++;
											}
											else
											{
												cachedPropertyValue.propertyValue.DeleteRawPtr();
											}
										}
									}
								}
							}
							break;
						case GuiInstancePropertyInfo::SupportAssign:
							if (values.Count() != 1) return;
							if (propertyValue->binding != L"set")
							{
								vint currentIndex = 0;
								FOREACH_INDEXER(Ptr<GuiValueRepr>, valueRepr, index, values)
								{
									if (valueRepr)
									{
										bool canRemoveLoadedValue = false;
										if (propertyValue->binding == L"")
										{
											// default binding: set the value directly
											if (LoadValueVisitor::LoadValue(valueRepr, env, propertyInfo->acceptableTypes, bindingSetters, cachedPropertyValue.propertyValue))
											{
												canRemoveLoadedValue = true;
												if(propertyLoader->SetPropertyValue(cachedPropertyValue, currentIndex))
												{
													currentIndex++;
												}
												else
												{
													cachedPropertyValue.propertyValue.DeleteRawPtr();
												}
											}
										}
										else if (IGuiInstanceBinder* binder=GetInstanceLoaderManager()->GetInstanceBinder(propertyValue->binding))
										{
											// other binding: provide the property value to the specified binder
											List<ITypeDescriptor*> binderExpectedTypes;
											binder->GetExpectedValueTypes(binderExpectedTypes);
											if (LoadValueVisitor::LoadValue(valueRepr, env, binderExpectedTypes, bindingSetters, cachedPropertyValue.propertyValue))
											{
												canRemoveLoadedValue = true;
												FillInstanceBindingSetter bindingSetter;
												bindingSetter.binder = binder;
												bindingSetter.loader = propertyLoader;
												bindingSetter.propertyValue = cachedPropertyValue;
												bindingSetter.currentIndex = currentIndex;
												bindingSetters.Add(bindingSetter);
												currentIndex++;
											}
										}

										if (canRemoveLoadedValue)
										{
											values[index] = 0;
											loadedValueCount++;
										}
									}
								}
							}
							break;
						case GuiInstancePropertyInfo::SupportArray:
							if (propertyValue->binding == L"")
							{
								auto list = IValueList::Create();
								FOREACH_INDEXER(Ptr<GuiValueRepr>, valueRepr, index, values)
								{
									// default binding: add the value to the list
									if (LoadValueVisitor::LoadValue(valueRepr, env, propertyInfo->acceptableTypes, bindingSetters, cachedPropertyValue.propertyValue))
									{
										values[index] = 0;
										loadedValueCount++;
										list->Add(cachedPropertyValue.propertyValue);
									}
								}

								// set the whole list to the property
								cachedPropertyValue.propertyValue = Value::From(list);
								propertyLoader->SetPropertyValue(cachedPropertyValue, 0);
							}
							break;
						}

						if (!propertyInfo->tryParent)
						{
							break;
						}
					}
					propertyLoader=GetInstanceLoaderManager()->GetParentLoader(propertyLoader);
				}
			}
		}

		description::Value LoadInstance(
			Ptr<GuiInstanceEnvironment> env,
			GuiConstructorRepr* ctor,
			description::ITypeDescriptor* expectedType,
			WString& typeName,
			List<FillInstanceBindingSetter>& bindingSetters
			)
		{
			InstanceLoadingSource source=FindInstanceLoadingSource(env, ctor);
			Value instance;
			IGuiInstanceLoader* instanceLoader = 0;
			bool deserialized = false;

			if(source.loader)
			{
				IGuiInstanceLoader* loader=source.loader;
				instanceLoader = source.loader;
				typeName=source.typeName;
				ITypeDescriptor* typeDescriptor=GetInstanceLoaderManager()->GetTypeDescriptorForType(source.typeName);
				
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

				if (!expectedType || typeDescriptor->CanConvertTo(expectedType))
				{
					IGuiInstanceLoader::TypeInfo typeInfo(typeName, typeDescriptor);
					while(loader && instance.IsNull())
					{
						if (singleTextValue && loader->IsDeserializable(typeInfo))
						{
							instance = loader->Deserialize(typeInfo, singleTextValue->text);
							if (!instance.IsNull())
							{
								deserialized = true;
							}
						}
						else if (loader->IsCreatable(typeInfo))
						{
							instance=loader->CreateInstance(typeInfo);
						}
						loader=GetInstanceLoaderManager()->GetParentLoader(loader);
					}
				}
			}
			else if(source.context)
			{
				if (Ptr<GuiInstanceContextScope> scope = LoadInstance(source.context, env->resolver, expectedType))
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

		Ptr<GuiInstanceContextScope> LoadInstance(
			Ptr<GuiInstanceContext> context,
			Ptr<GuiResourcePathResolver> resolver,
			description::ITypeDescriptor* expectedType
			)
		{
			Ptr<GuiInstanceEnvironment> env = new GuiInstanceEnvironment(context, resolver);
			List<FillInstanceBindingSetter> bindingSetters;
			env->scope->rootInstance=LoadInstance(env, context->instance.Obj(), expectedType, env->scope->typeName, bindingSetters);

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
				return LoadInstance(context, resolver, expectedType);
			}
			return 0;
		}
	}
}