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

		}
		using namespace visitors;

/***********************************************************************
Helper Functions
***********************************************************************/

		InstanceLoadingSource FindInstanceLoadingSource(Ptr<GuiInstanceContext> context, Ptr<GuiConstructorRepr> ctor, Ptr<GuiResourcePathResolver> resolver)
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

		description::Value LoadInstance(Ptr<GuiInstanceContext> context, Ptr<GuiResourcePathResolver> resolver)
		{
			return LoadInstance(context, context->instance, resolver);
		}

		description::Value LoadInstance(Ptr<GuiInstanceContext> context, Ptr<GuiConstructorRepr> ctor, Ptr<GuiResourcePathResolver> resolver)
		{
			InstanceLoadingSource source=FindInstanceLoadingSource(context, ctor, resolver);
			Value instance;
			if(source.loader)
			{
				instance=source.loader->CreateInstance(context, ctor, resolver, source.typeName, GetInstanceLoaderManager()->GetTypeDescriptorForType(source.typeName));
			}
			else if(source.context)
			{
				instance=LoadInstance(source.context, resolver);
			}

			FillInstance(instance, context, ctor, resolver, source.loader);
			return instance;
		}

		void FillInstance(description::Value createdInstance, Ptr<GuiInstanceContext> context, Ptr<GuiConstructorRepr> ctor, Ptr<GuiResourcePathResolver> resolver, IGuiInstanceLoader* loader)
		{
			if(createdInstance.GetRawPtr())
			{
			}
		}
	}
}