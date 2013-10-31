#include "GuiInstanceLoader.h"
#include "..\Resources\GuiParserManager.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;
		using namespace parsing;
		using namespace parsing::xml;
		using namespace parsing::tabling;
		using namespace controls;
		using namespace regex;
		using namespace reflection::description;

/***********************************************************************
GuiInstanceContext::ElementName Parser
***********************************************************************/

		class GuiInstanceContextElementNameParser : public Object, public IGuiParser<GuiInstanceContext::ElementName>
		{
			typedef GuiInstanceContext::ElementName			ElementName;
		public:
			Regex						regexElementName;

			GuiInstanceContextElementNameParser()
				:regexElementName(L"((<namespaceName>[a-zA-Z_]/w*):)?((<category>[a-zA-Z_]/w*).)?(<name>[a-zA-Z_]/w*)(-(<binding>[a-zA-Z_]/w*))?")
			{
			}

			Ptr<ElementName> TypedParse(const WString& text)override
			{
				Ptr<RegexMatch> match=regexElementName.MatchHead(text);
				if(match && match->Result().Length()!=text.Length()) return 0;

				Ptr<ElementName> elementName=new ElementName;
				if(match->Groups().Keys().Contains(L"namespaceName"))
				{
					elementName->namespaceName=match->Groups()[L"namespaceName"][0].Value();
				}
				if(match->Groups().Keys().Contains(L"category"))
				{
					elementName->category=match->Groups()[L"category"][0].Value();
				}
				if(match->Groups().Keys().Contains(L"name"))
				{
					elementName->name=match->Groups()[L"name"][0].Value();
				}
				if(match->Groups().Keys().Contains(L"binding"))
				{
					elementName->binding=match->Groups()[L"binding"][0].Value();
				}
				return elementName;
			}

			Ptr<Object> Parse(const WString& text)override
			{
				return TypedParse(text);
			}
		};

/***********************************************************************
Instance Type Resolver
***********************************************************************/

		class GuiResourceInstanceTypeResolver : public Object, public IGuiResourceTypeResolver
		{
		public:
			WString GetType()
			{
				return L"Instance";
			}

			WString GetPreloadType()
			{
				return L"Xml";
			}

			bool IsDelayLoad()
			{
				return false;
			}

			Ptr<Object> ResolveResource(Ptr<parsing::xml::XmlElement> element)
			{
				return 0;
			}

			Ptr<Object> ResolveResource(const WString& path)
			{
				return 0;
			}

			Ptr<Object> ResolveResource(Ptr<Object> resource, Ptr<GuiResourcePathResolver> resolver)
			{
				Ptr<XmlDocument> xml=resource.Cast<XmlDocument>();
				if(xml)
				{
					Ptr<GuiInstanceContext> context=GuiInstanceContext::LoadFromXml(xml);
					return context;
				}
				return 0;
			}
		};

/***********************************************************************
GuiInstanceLoaderManager
***********************************************************************/

		IGuiInstanceLoaderManager* instanceLoaderManager=0;

		IGuiInstanceLoaderManager* GetInstanceLoaderManager()
		{
			return instanceLoaderManager;
		}

		class GuiInstanceLoaderManager : public Object, public IGuiInstanceLoaderManager, public IGuiPlugin
		{
		protected:
			typedef Dictionary<WString, Ptr<IGuiInstanceBinder>>				BinderMap;

			struct TypeInfo
			{
				WString								typeName;
				ITypeDescriptor*					typeDescriptor;
				WString								parentTypeName;				// for virtual type only
				Ptr<IGuiInstanceLoader>				loader;

				List<ITypeDescriptor*>				parentTypes;				// all direct or indirect base types that does not has a type info
				List<TypeInfo*>						parentTypeInfos;			// type infos for all registered direct or indirect base types
			};
			typedef Dictionary<WString, Ptr<TypeInfo>>							TypeInfoMap;

			Ptr<IGuiInstanceLoader>					rootLoader;
			BinderMap								binders;
			TypeInfoMap								typeInfos;

			bool IsTypeExists(const WString& name)
			{
				return GetGlobalTypeManager()->GetTypeDescriptor(name)!=0 || typeInfos.Keys().Contains(name);
			}

			void FindParentTypeInfos(Ptr<TypeInfo> typeInfo, ITypeDescriptor* searchType)
			{
				if(searchType!=typeInfo->typeDescriptor)
				{
					vint index=typeInfos.Keys().Contains(searchType->GetTypeName());
					if(index==-1)
					{
						typeInfo->parentTypes.Add(searchType);
					}
					else
					{
						typeInfo->parentTypeInfos.Add(typeInfos.Values()[index].Obj());
						return;
					}
				}

				vint count=searchType->GetBaseTypeDescriptorCount();
				for(vint i=0;i<count;i++)
				{
					ITypeDescriptor* baseType=searchType->GetBaseTypeDescriptor(i);
					FindParentTypeInfos(typeInfo, baseType);
				}
			}

			void FillParentTypeInfos(Ptr<TypeInfo> typeInfo)
			{
				typeInfo->parentTypes.Clear();
				typeInfo->parentTypeInfos.Clear();

				ITypeDescriptor* searchType=typeInfo->typeDescriptor;
				if(!searchType)
				{
					vint index=typeInfos.Keys().IndexOf(typeInfo->parentTypeName);
					if(index==-1)
					{
						searchType=GetGlobalTypeManager()->GetTypeDescriptor(typeInfo->parentTypeName);
					}
					else
					{
						typeInfo->parentTypeInfos.Add(typeInfos.Values()[index].Obj());
						return;
					}
				}

				if(searchType)
				{
					FindParentTypeInfos(typeInfo, searchType);
				}
			}

			IGuiInstanceLoader* GetLoaderFromType(ITypeDescriptor* typeDescriptor)
			{
				vint index=typeInfos.Keys().IndexOf(typeDescriptor->GetTypeName());
				if(index==-1)
				{
					vint count=typeDescriptor->GetBaseTypeDescriptorCount();
					for(vint i=0;i<count;i++)
					{
						ITypeDescriptor* baseType=typeDescriptor->GetBaseTypeDescriptor(i);
						IGuiInstanceLoader* loader=GetLoaderFromType(baseType);
						if(loader) return loader;
					}
					return 0;
				}
				else
				{
					return typeInfos.Values()[index]->loader.Obj();
				}
			}
		public:
			void Load()override
			{
				instanceLoaderManager=this;
			}
				
			void AfterLoad()override
			{
				{
					IGuiResourceResolverManager* manager=GetResourceResolverManager();
					manager->SetTypeResolver(new GuiResourceInstanceTypeResolver);
				}
				{
					IGuiParserManager* manager=GetParserManager();
					manager->SetParser(L"INSTANCE-ELEMENT-NAME", new GuiInstanceContextElementNameParser);
				}
			}

			void Unload()override
			{
				instanceLoaderManager=0;
			}

			bool AddInstanceBinder(Ptr<IGuiInstanceBinder> binder)override
			{
				if(binders.Keys().Contains(binder->GetBindingName())) return false;
				binders.Add(binder->GetBindingName(), binder);
				return true;
			}

			IGuiInstanceBinder* GetInstanceBinder(const WString& bindingName)override
			{
				vint index=binders.Keys().IndexOf(bindingName);
				return index==-1?0:binders.Values()[index].Obj();
			}

			bool CreateVirtualType(const WString& typeName, const WString& parentType, Ptr<IGuiInstanceLoader> loader)override
			{
				if(IsTypeExists(typeName) || !IsTypeExists(parentType)) return false;

				Ptr<TypeInfo> typeInfo=new TypeInfo;
				typeInfo->typeName=typeName;
				typeInfo->parentTypeName=parentType;
				typeInfo->loader=loader;
				typeInfos.Add(typeName, typeInfo);
				FillParentTypeInfos(typeInfo);

				return true;
			}

			bool SetLoader(Ptr<IGuiInstanceLoader> loader)override
			{
				vint index=typeInfos.Keys().IndexOf(loader->GetTypeName());
				if(index!=-1) return false;

				ITypeDescriptor* typeDescriptor=GetGlobalTypeManager()->GetTypeDescriptor(loader->GetTypeName());
				if(typeDescriptor==0) return false;

				Ptr<TypeInfo> typeInfo=new TypeInfo;
				typeInfo->typeName=loader->GetTypeName();
				typeInfo->typeDescriptor=typeDescriptor;
				typeInfo->loader=loader;
				typeInfos.Add(typeInfo->typeName, typeInfo);
				FillParentTypeInfos(typeInfo);
				
				FOREACH(Ptr<TypeInfo>, derived, typeInfos.Values())
				{
					if(derived->parentTypes.Contains(typeInfo->typeDescriptor))
					{
						FillParentTypeInfos(derived);
					}
				}

				return false;
			}

			IGuiInstanceLoader* GetLoader(const WString& typeName)override
			{
				vint index=typeInfos.Keys().Contains(typeName);
				if(index!=-1)
				{
					return typeInfos.Values()[index]->loader.Obj();
				}

				ITypeDescriptor* typeDescriptor=GetGlobalTypeManager()->GetTypeDescriptor(typeName);
				if(typeDescriptor)
				{
					IGuiInstanceLoader* loader=GetLoaderFromType(typeDescriptor);
					return loader?loader:rootLoader.Obj();
				}
				return 0;
			}

			IGuiInstanceLoader* GetParentLoader(IGuiInstanceLoader* loader)override
			{
				vint index=typeInfos.Keys().Contains(loader->GetTypeName());
				if(index!=-1)
				{
					Ptr<TypeInfo> typeInfo=typeInfos.Values()[index];
					if(typeInfo->parentTypeInfos.Count()>0)
					{
						return typeInfo->parentTypeInfos[0]->loader.Obj();
					}
					return rootLoader.Obj();
				}
				return 0;
			}

			DescriptableObject* LoadObject(Ptr<GuiInstanceContext> context, Ptr<GuiResourcePathResolver> resolver)override
			{
				throw 0;
			}

			bool LoadObject(DescriptableObject* createdInstance, Ptr<GuiInstanceContext> context, Ptr<GuiResourcePathResolver> resolver)override
			{
				throw 0;
			}
		};
		GUI_REGISTER_PLUGIN(GuiInstanceLoaderManager)
	}
}