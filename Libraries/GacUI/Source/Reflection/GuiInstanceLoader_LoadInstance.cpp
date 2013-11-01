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
				}

				void Visit(GuiAttSetterRepr* repr)override
				{
				}

				void Visit(GuiConstructorRepr* repr)override
				{
				}

				static Value LoadValue(Ptr<GuiValueRepr> valueRepr, Ptr<GuiInstanceContext> context, Ptr<GuiResourcePathResolver> resolver, ITypeDescriptor* typeDescriptor)
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
			for(vint i=0;i<attSetter->setters.Count();i++)
			{
				WString propertyName=attSetter->setters.Keys()[i];
				auto propertyValue=attSetter->setters.Values()[i];
				IGuiInstanceLoader* propertyLoader=loader;

				while(propertyLoader)
				{
					ITypeDescriptor* elementType=0;
					bool nullable=false;
					auto propertyType=propertyLoader->GetPropertyType(typeName, typeDescriptor, propertyName, elementType, nullable);

					switch(propertyType)
					{
					case IGuiInstanceLoader::ValueProperty:
						if(propertyValue->values.Count()==1)
						{
							Ptr<GuiValueRepr> valueRepr=propertyValue->values[0];
							if(propertyValue->binding==L"")
							{
								Value value=LoadValueVisitor::LoadValue(valueRepr, context, resolver, elementType);
								propertyLoader->SetPropertyValue(createdInstance, typeName, typeDescriptor, propertyName, value);
							}
							else if(propertyValue->binding==L"set")
							{
								if(Ptr<GuiAttSetterRepr> propertyAttSetter=valueRepr.Cast<GuiAttSetterRepr>())
								{
									Value propertyValue=propertyLoader->GetPropertyValue(createdInstance, typeName, typeDescriptor, propertyName);
									if(propertyValue.GetRawPtr())
									{
										ITypeDescriptor* propertyTypeDescriptor=propertyValue.GetRawPtr()->GetTypeDescriptor();
										IGuiInstanceLoader* propertyInstanceLoader=GetInstanceLoaderManager()->GetLoader(propertyTypeDescriptor->GetTypeName());
										if(propertyInstanceLoader)
										{
											FillInstance(propertyValue, context, propertyAttSetter.Obj(), resolver, propertyInstanceLoader, propertyTypeDescriptor->GetTypeName(), propertyTypeDescriptor);
										}
									}
								}
							}
							else
							{
							}
						}
						break;
					case IGuiInstanceLoader::CollectionProperty:
						FOREACH(Ptr<GuiValueRepr>, valueRepr, propertyValue->values)
						{
							if(propertyValue->binding==L"")
							{
								Value value=LoadValueVisitor::LoadValue(valueRepr, context, resolver, elementType);
								propertyLoader->SetPropertyCollection(createdInstance, typeName, typeDescriptor, propertyName, value);
							}
							else if(propertyValue->binding!=L"set")
							{
							}
						}
						break;
					}

					if(propertyType==IGuiInstanceLoader::HandleByParentLoader)
					{
						propertyLoader=GetInstanceLoaderManager()->GetParentLoader(propertyLoader);
					}
					else
					{
						break;
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
					instance=loader->CreateInstance(context, ctor, resolver, typeName, typeDescriptor);
					loader=GetInstanceLoaderManager()->GetParentLoader(loader);
				}
			}
			else if(source.context)
			{
				instance=LoadInstance(source.context, source.context->instance.Obj(), resolver, typeName, typeDescriptor);
			}

			if(instance.GetRawPtr())
			{
				FillInstance(instance, context, ctor, resolver, source.loader, typeName, typeDescriptor);
			}

			return instance;
		}
	}
}