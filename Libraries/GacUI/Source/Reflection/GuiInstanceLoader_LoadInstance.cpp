#include "GuiInstanceLoader.h"
#include "..\Resources\GuiParserManager.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;
		using namespace reflection::description;

		namespace visitors
		{
/***********************************************************************
FindTypeVisitor
***********************************************************************/

			class FindTypeVisitor : public Object, public GuiInstancePattern::IVisitor
			{
			public:
				Ptr<GuiInstanceContext>			context;
				Ptr<GuiResourcePathResolver>	resolver;
				WString							typeName;
				InstanceLoadingSource			result;

				FindTypeVisitor(Ptr<GuiInstanceContext> _context, Ptr<GuiResourcePathResolver> _resolver, const WString& _typeName)
					:context(_context)
					,resolver(_resolver)
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
					Ptr<Object> resource=resolver->ResolveResource(ns->protocol, ns->path);
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

				static InstanceLoadingSource FindType(GuiInstancePattern* pattern, Ptr<GuiInstanceContext> context, Ptr<GuiResourcePathResolver> resolver, const WString& typeName)
				{
					FindTypeVisitor visitor(context, resolver, typeName);
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
				Ptr<GuiInstanceContext>					context;
				Ptr<GuiResourcePathResolver>			resolver;
				List<ITypeDescriptor*>&					acceptableTypes;
				bool									result;
				Value									loadedValue;

				LoadValueVisitor(Ptr<GuiInstanceContext> _context, Ptr<GuiResourcePathResolver> _resolver, List<ITypeDescriptor*>& _acceptableTypes)
					:context(_context)
					,resolver(_resolver)
					,acceptableTypes(_acceptableTypes)
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
							loadedValue=LoadInstance(context, repr, resolver, typeDescriptor, _typeName);
							if (!loadedValue.IsNull())
							{
								result = true;
								return;
							}
						}
					}
				}

				static bool LoadValue(Ptr<GuiValueRepr> valueRepr, Ptr<GuiInstanceContext> context, Ptr<GuiResourcePathResolver> resolver, List<ITypeDescriptor*>& acceptableTypes, Value& loadedValue)
				{
					LoadValueVisitor visitor(context, resolver, acceptableTypes);
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
			Ptr<GuiInstanceContext> context,
			GuiConstructorRepr* ctor,
			Ptr<GuiResourcePathResolver> resolver
			)
		{
			vint index=context->namespaces.Keys().IndexOf(ctor->typeNamespace);
			if(index!=-1)
			{
				Ptr<GuiInstanceContext::NamespaceInfo> namespaceInfo=context->namespaces.Values()[index];
				FOREACH(Ptr<GuiInstancePattern>, pattern, namespaceInfo->patterns)
				{
					InstanceLoadingSource source=FindTypeVisitor::FindType(pattern.Obj(), context, resolver, ctor->typeName);
					if(source) return source;
				}
			}
			return InstanceLoadingSource();
		}

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

		void FillInstance(
			description::Value createdInstance,
			Ptr<GuiInstanceContext> context,
			GuiAttSetterRepr* attSetter,
			Ptr<GuiResourcePathResolver> resolver,
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
									if (LoadValueVisitor::LoadValue(valueRepr, context, resolver, acceptableTypes, cachedPropertyValue.propertyValue))
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
												FillInstance(cachedPropertyValue.propertyValue, context, propertyAttSetter.Obj(), resolver, propertyInstanceLoader, propertyTypeDescriptor->GetTypeName(), bindingSetters);
											}
										}
									}
								}
								else if (IGuiInstanceBinder* binder=GetInstanceLoaderManager()->GetInstanceBinder(propertyValue->binding))
								{
									// other binding: provide the property value to the specified binder
									binderExpectedTypes.Clear();
									binder->GetExpectedValueTypes(binderExpectedTypes);
									if (LoadValueVisitor::LoadValue(valueRepr, context, resolver, binderExpectedTypes, cachedPropertyValue.propertyValue))
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
			Ptr<GuiInstanceContext> context,
			Ptr<GuiResourcePathResolver> resolver
			)
		{
			WString typeName;
			return LoadInstance(context, context->instance.Obj(), resolver, 0, typeName);
		}

		description::Value LoadInstance(
			Ptr<GuiInstanceContext> context,
			GuiConstructorRepr* ctor,
			Ptr<GuiResourcePathResolver> resolver,
			description::ITypeDescriptor* expectedType,
			WString& typeName
			)
		{
			InstanceLoadingSource source=FindInstanceLoadingSource(context, ctor, resolver);
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
						instance=loader->CreateInstance(context, ctor, resolver, IGuiInstanceLoader::TypeInfo(typeName, typeDescriptor));
						loader=GetInstanceLoaderManager()->GetParentLoader(loader);
					}
				}
			}
			else if(source.context)
			{
				instance=LoadInstance(source.context, source.context->instance.Obj(), resolver, expectedType, typeName);
				instanceLoader=GetInstanceLoaderManager()->GetLoader(typeName);
			}

			if(instance.GetRawPtr() && instanceLoader)
			{
				List<FillInstanceBindingSetter> bindingSetters;
				FillInstance(instance, context, ctor, resolver, instanceLoader, typeName, bindingSetters);

				FOREACH(FillInstanceBindingSetter, bindingSetter, bindingSetters)
				{
					if (!bindingSetter.binder->SetPropertyValue(bindingSetter.loader, resolver, bindingSetter.propertyValue, bindingSetter.currentIndex))
					{
						bindingSetter.propertyValue.propertyValue.DeleteRawPtr();
					}
				}
			}

			return instance;
		}
	}
}