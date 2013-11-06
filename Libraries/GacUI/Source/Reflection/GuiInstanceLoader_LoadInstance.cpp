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
				ITypeDescriptor*						typeDescriptor;
				Value									result;

				LoadValueVisitor(Ptr<GuiInstanceContext> _context, Ptr<GuiResourcePathResolver> _resolver, ITypeDescriptor* _typeDescriptor)
					:context(_context)
					,resolver(_resolver)
					,typeDescriptor(_typeDescriptor)
				{
				}

				void Visit(GuiTextRepr* repr)override
				{
					if(IValueSerializer* serializer=typeDescriptor->GetValueSerializer())
					{
						serializer->Parse(repr->text, result);
					}
				}

				void Visit(GuiAttSetterRepr* repr)override
				{
				}

				void Visit(GuiConstructorRepr* repr)override
				{
					if (IValueSerializer* serializer = typeDescriptor->GetValueSerializer())
					{
						vint index = repr->setters.Keys().IndexOf(L"");
						if (index != -1)
						{
							auto setterValue = repr->setters.Values()[index];
							if (setterValue->values.Count() == 1)
							{
								if (auto textRepr = setterValue->values[0].Cast<GuiTextRepr>())
								{
									result = LoadValueVisitor::LoadValue(textRepr, context, resolver, typeDescriptor);
								}
							}
						}
					}
					else
					{
						WString _typeName;
						ITypeDescriptor* _typeDescriptor=0;
						result=LoadInstance(context, repr, resolver, _typeName, _typeDescriptor);
					}
				}

				static bool LoadValue(Ptr<GuiValueRepr> valueRepr, Ptr<GuiInstanceContext> context, Ptr<GuiResourcePathResolver> resolver, List<ITypeDescriptor*>& acceptableTypes, Value& result)
				{
					LoadValueVisitor visitor(context, resolver, typeDescriptor);
					valueRepr->Accept(&visitor);
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

		void FillInstance(
			description::Value createdInstance,
			Ptr<GuiInstanceContext> context,
			GuiAttSetterRepr* attSetter,
			Ptr<GuiResourcePathResolver> resolver,
			IGuiInstanceLoader* loader,
			const WString& typeName,
			description::ITypeDescriptor* typeDescriptor
			)
		{
			// reverse loop to set the default property (name == L"") after all other properties
			for(vint i=attSetter->setters.Count()-1;i>=0;i--)
			{
				WString propertyName=attSetter->setters.Keys()[i];
				auto propertyValue=attSetter->setters.Values()[i];
				IGuiInstanceLoader* propertyLoader=loader;
				IGuiInstanceLoader::PropertyValue cachedPropertyValue(IGuiInstanceLoader::TypeInfo(typeName, typeDescriptor), propertyName, createdInstance);

				List<Ptr<GuiValueRepr>> values;
				CopyFrom(values, propertyValue->values);
				vint loadedValueCount = 0;

				// try to look for a loader to handle this property
				while(propertyLoader && loadedValueCount<values.Count())
				{
					List<ITypeDescriptor*> acceptableTypes, binderExpectedTypes;
					auto propertyType=propertyLoader->GetPropertyType(
						IGuiInstanceLoader::PropertyInfo(
							IGuiInstanceLoader::TypeInfo(typeName, typeDescriptor),
							propertyName
							),
						acceptableTypes
						);

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
												FillInstance(cachedPropertyValue.propertyValue, context, propertyAttSetter.Obj(), resolver, propertyInstanceLoader, propertyTypeDescriptor->GetTypeName(), propertyTypeDescriptor);
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
			ITypeDescriptor* typeDescriptor=0;
			return LoadInstance(context, context->instance.Obj(), resolver, typeName, typeDescriptor);
		}

		description::Value LoadInstance(
			Ptr<GuiInstanceContext> context,
			GuiConstructorRepr* ctor,
			Ptr<GuiResourcePathResolver> resolver,
			WString& typeName,
			description::ITypeDescriptor*& typeDescriptor
			)
		{
			InstanceLoadingSource source=FindInstanceLoadingSource(context, ctor, resolver);
			Value instance;
			if(source.loader)
			{
				IGuiInstanceLoader* loader=source.loader;
				typeName=source.typeName;
				typeDescriptor=GetInstanceLoaderManager()->GetTypeDescriptorForType(source.typeName);

				while(loader && instance.IsNull())
				{
					instance=loader->CreateInstance(context, ctor, resolver, IGuiInstanceLoader::TypeInfo(typeName, typeDescriptor));
					loader=GetInstanceLoaderManager()->GetParentLoader(loader);
				}

				if(instance.GetRawPtr())
				{
					FillInstance(instance, context, ctor, resolver, source.loader, typeName, typeDescriptor);
				}
			}
			else if(source.context)
			{
				instance=LoadInstance(source.context, source.context->instance.Obj(), resolver, typeName, typeDescriptor);
				IGuiInstanceLoader* loader=GetInstanceLoaderManager()->GetLoader(typeName);

				if(instance.GetRawPtr() && loader)
				{
					FillInstance(instance, context, ctor, resolver, loader, typeName, typeDescriptor);
				}
			}

			return instance;
		}
	}
}