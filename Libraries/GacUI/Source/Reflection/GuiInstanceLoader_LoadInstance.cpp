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
					Ptr<GuiTextRepr> singleTextValue;
					{
						vint index = repr->setters.Keys().IndexOf(L"");
						if (index != -1)
						{
							auto setterValue = repr->setters.Values()[index];
							if (setterValue->values.Count() == 1)
							{
								singleTextValue = setterValue->values[0].Cast<GuiTextRepr>();
							}
						}
					}
					
					FOREACH(ITypeDescriptor*, typeDescriptor, acceptableTypes)
					{
						if (IValueSerializer* serializer = typeDescriptor->GetValueSerializer())
						{
							if (singleTextValue && serializer->Parse(singleTextValue->text, loadedValue))
							{
								result = true;
								return;
							}
						}
						else
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
			const WString& typeName,
			List<FillInstanceBindingSetter>& bindingSetters
			)
		{
			// reverse loop to set the default property (name == L"") after all other properties
			for(vint i=attSetter->setters.Count()-1;i>=0;i--)
			{
				WString propertyName=attSetter->setters.Keys()[i];
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
					List<ITypeDescriptor*> acceptableTypes, binderExpectedTypes;
					auto propertyType = propertyLoader->GetPropertyType(cachedPropertyValue, acceptableTypes);

					if (propertyType & IGuiInstanceLoader::SupportedProperty)
					{
						vint currentIndex = 0;
						for (vint i = 0; i < values.Count(); i++)
						{
							bool canRemoveLoadedValue = false;
							if (Ptr<GuiValueRepr> valueRepr = values[i])
							{
								if(propertyValue->binding==L"")
								{
									// default binding: set the value directly
									if (LoadValueVisitor::LoadValue(valueRepr, env, acceptableTypes, bindingSetters, cachedPropertyValue.propertyValue))
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
								if (propertyValue->binding == L"set")
								{
									// set binding: get the property value and apply another property list on it
									if(Ptr<GuiAttSetterRepr> propertyAttSetter=valueRepr.Cast<GuiAttSetterRepr>())
									{
										if(propertyLoader->GetPropertyValue(cachedPropertyValue) && cachedPropertyValue.propertyValue.GetRawPtr())
										{
											canRemoveLoadedValue = true;
											ITypeDescriptor* propertyTypeDescriptor=cachedPropertyValue.propertyValue.GetRawPtr()->GetTypeDescriptor();
											IGuiInstanceLoader* propertyInstanceLoader=GetInstanceLoaderManager()->GetLoader(propertyTypeDescriptor->GetTypeName());
											if(propertyInstanceLoader)
											{
												FillInstance(cachedPropertyValue.propertyValue, env, propertyAttSetter.Obj(), propertyInstanceLoader, propertyTypeDescriptor->GetTypeName(), bindingSetters);
											}
										}
									}
								}
								else if (IGuiInstanceBinder* binder=GetInstanceLoaderManager()->GetInstanceBinder(propertyValue->binding))
								{
									// other binding: provide the property value to the specified binder
									binderExpectedTypes.Clear();
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
									values[i] = 0;
									loadedValueCount++;
								}
							}
						}
					}

					if(propertyType & IGuiInstanceLoader::HandleByParentLoader)
					{
						propertyLoader=GetInstanceLoaderManager()->GetParentLoader(propertyLoader);
					}
					else
					{
						propertyLoader=0;
					}
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

			if(source.loader)
			{
				IGuiInstanceLoader* loader=source.loader;
				instanceLoader = source.loader;
				typeName=source.typeName;
				ITypeDescriptor* typeDescriptor=GetInstanceLoaderManager()->GetTypeDescriptorForType(source.typeName);

				if (!expectedType || typeDescriptor->CanConvertTo(expectedType))
				{
					while(loader && instance.IsNull())
					{
						instance=loader->CreateInstance(IGuiInstanceLoader::TypeInfo(typeName, typeDescriptor));
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
				FillInstance(instance, env, ctor, instanceLoader, typeName, bindingSetters);

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