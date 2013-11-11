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
GuiInstancePropertyInfo
***********************************************************************/

		GuiInstancePropertyInfo::GuiInstancePropertyInfo()
			:supportAssign(false)
			, supportSet(false)
			, tryParent(false)
			, multipleValues(false)
			, required(false)
			, constructorParameter(false)
		{
		}

		GuiInstancePropertyInfo::~GuiInstancePropertyInfo()
		{
		}

		Ptr<GuiInstancePropertyInfo> GuiInstancePropertyInfo::Unsupported()
		{
			Ptr<GuiInstancePropertyInfo> info = new GuiInstancePropertyInfo;
			return info;
		}

		Ptr<GuiInstancePropertyInfo> GuiInstancePropertyInfo::Supported(description::ITypeDescriptor* typeDescriptor)
		{
			Ptr<GuiInstancePropertyInfo> info = new GuiInstancePropertyInfo;
			info->supportAssign = true;
			info->supportSet = true;
			if (typeDescriptor) info->acceptableTypes.Add(typeDescriptor);
			return info;
		}

		Ptr<GuiInstancePropertyInfo> GuiInstancePropertyInfo::SupportedWithParent(description::ITypeDescriptor* typeDescriptor)
		{
			Ptr<GuiInstancePropertyInfo> info = Supported(typeDescriptor);
			info->tryParent = true;
			return info;
		}

		Ptr<GuiInstancePropertyInfo> GuiInstancePropertyInfo::SupportedCollection(description::ITypeDescriptor* typeDescriptor)
		{
			Ptr<GuiInstancePropertyInfo> info = Supported(typeDescriptor);
			info->multipleValues = true;
			return info;
		}

		Ptr<GuiInstancePropertyInfo> GuiInstancePropertyInfo::SupportedCollectionWithParent(description::ITypeDescriptor* typeDescriptor)
		{
			Ptr<GuiInstancePropertyInfo> info = SupportedWithParent(typeDescriptor);
			info->multipleValues = true;
			return info;
		}

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
Default Instance Loader
***********************************************************************/

		class GuiDefaultInstanceLoader : public Object, public IGuiInstanceLoader
		{
		public:
			WString GetTypeName()override
			{
				return L"";
			}

			bool IsDeserializable(const TypeInfo& typeInfo)override
			{
				return typeInfo.typeDescriptor->GetValueSerializer() != 0;
			}

			description::Value Deserialize(const TypeInfo& typeInfo, const WString& text)override
			{
				if (IValueSerializer* serializer = typeInfo.typeDescriptor->GetValueSerializer())
				{
					Value loadedValue;
					if (serializer->Parse(text, loadedValue))
					{
						return loadedValue;
					}
				}
				return Value();
			}

			IMethodInfo* GetDefaultConstructor(const TypeInfo& typeInfo)
			{
				vint count = typeInfo.typeDescriptor->GetConstructorGroup()->GetMethodCount();
				for(vint i=0;i<count;i++)
				{
					IMethodInfo* method = typeInfo.typeDescriptor->GetConstructorGroup()->GetMethod(i);
					if(method->GetParameterCount()==0)
					{
						return method;
					}
				}
				return 0;
			}

			bool IsCreatable(const TypeInfo& typeInfo)override
			{
				return GetDefaultConstructor(typeInfo) != 0;
			}

			description::Value CreateInstance(const TypeInfo& typeInfo)override
			{
				if (IMethodInfo* method = GetDefaultConstructor(typeInfo))
				{
					return method->Invoke(Value(), (Value::xs()));
				}
				return Value();
			}

			ITypeInfo* GetPropertyReflectionTypeInfo(const PropertyInfo& propertyInfo)
			{
				IPropertyInfo* prop = propertyInfo.typeInfo.typeDescriptor->GetPropertyByName(propertyInfo.propertyName, true);
				if(prop)
				{
					ITypeInfo* propType=prop->GetReturn();
					if(IMethodInfo* method=prop->GetSetter())
					{
						if(method->GetParameterCount()==1)
						{
							propType=method->GetParameter(0)->GetType();
						}
					}
					return propType;
				}
				return 0;
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyTypeInternal(ITypeInfo* propType, bool collectionType)
			{
				switch(propType->GetDecorator())
				{
				case ITypeInfo::RawPtr:
				case ITypeInfo::SharedPtr:
				case ITypeInfo::Nullable:
					return GetPropertyTypeInternal(propType->GetElementType(), collectionType);
				case ITypeInfo::TypeDescriptor:
					if (collectionType)
					{
						return GuiInstancePropertyInfo::SupportedCollection(propType->GetTypeDescriptor());
					}
					else
					{
						return GuiInstancePropertyInfo::Supported(propType->GetTypeDescriptor());
					}
				case ITypeInfo::Generic:
					{
						ITypeDescriptor* genericType=propType->GetElementType()->GetTypeDescriptor();

						if(genericType==description::GetTypeDescriptor<IValueList>())
						{
							return GetPropertyTypeInternal(propType->GetGenericArgument(0), true);
						}
						else if(genericType==description::GetTypeDescriptor<IValueFunctionProxy>())
						{
							return GuiInstancePropertyInfo::Supported(genericType);
						}
					}
				}
				return GuiInstancePropertyInfo::Unsupported();
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				if (ITypeInfo* propType = GetPropertyReflectionTypeInfo(propertyInfo))
				{
					return GetPropertyTypeInternal(propType, false);
				}
				return GuiInstancePropertyInfo::Unsupported();
			}

			bool GetPropertyValue(PropertyValue& propertyValue)override
			{
				if (IPropertyInfo* prop = propertyValue.typeInfo.typeDescriptor->GetPropertyByName(propertyValue.propertyName, true))
				{
					if (prop->IsReadable())
					{
						propertyValue.propertyValue = prop->GetValue(propertyValue.instanceValue);
						return true;
					}
				}
				return false;
			}

			bool SetPropertyValue(PropertyValue& propertyValue, vint currentIndex)override
			{
				if (ITypeInfo* propType = GetPropertyReflectionTypeInfo(propertyValue))
				{
					if (propType->GetDecorator() == ITypeInfo::Generic)
					{
						ITypeDescriptor* genericType=propType->GetElementType()->GetTypeDescriptor();
						if (genericType == description::GetTypeDescriptor<IValueList>())
						{
							Value value = propertyValue.instanceValue.GetProperty(propertyValue.propertyName);
							if (auto list = dynamic_cast<IValueList*>(value.GetRawPtr()))
							{
								list->Add(propertyValue.propertyValue);
								return true;
							}
							else
							{
								return false;
							}
						}
					}
				}
				propertyValue.instanceValue.SetProperty(propertyValue.propertyName, propertyValue.propertyValue);
				return true;
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

			struct VirtualTypeInfo
			{
				WString								typeName;
				ITypeDescriptor*					typeDescriptor;
				WString								parentTypeName;				// for virtual type only
				Ptr<IGuiInstanceLoader>				loader;

				List<ITypeDescriptor*>				parentTypes;				// all direct or indirect base types that does not has a type info
				List<VirtualTypeInfo*>				parentTypeInfos;			// type infos for all registered direct or indirect base types

				VirtualTypeInfo()
					:typeDescriptor(0)
				{
				}
			};
			typedef Dictionary<WString, Ptr<VirtualTypeInfo>>					VirtualTypeInfoMap;

			Ptr<IGuiInstanceLoader>					rootLoader;
			BinderMap								binders;
			VirtualTypeInfoMap						typeInfos;

			bool IsTypeExists(const WString& name)
			{
				return GetGlobalTypeManager()->GetTypeDescriptor(name)!=0 || typeInfos.Keys().Contains(name);
			}

			void FindParentTypeInfos(Ptr<VirtualTypeInfo> typeInfo, ITypeDescriptor* searchType)
			{
				if(searchType!=typeInfo->typeDescriptor)
				{
					vint index=typeInfos.Keys().IndexOf(searchType->GetTypeName());
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

			void FillParentTypeInfos(Ptr<VirtualTypeInfo> typeInfo)
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
						typeInfo->typeDescriptor=searchType;
						typeInfo->parentTypes.Add(searchType);
					}
					else
					{
						VirtualTypeInfo* parentTypeInfo = typeInfos.Values()[index].Obj();
						typeInfo->typeDescriptor=parentTypeInfo->typeDescriptor;
						typeInfo->parentTypeInfos.Add(parentTypeInfo);
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
			GuiInstanceLoaderManager()
			{
				rootLoader=new GuiDefaultInstanceLoader;
			}

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

				Ptr<VirtualTypeInfo> typeInfo = new VirtualTypeInfo;
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

				Ptr<VirtualTypeInfo> typeInfo = new VirtualTypeInfo;
				typeInfo->typeName=loader->GetTypeName();
				typeInfo->typeDescriptor=typeDescriptor;
				typeInfo->loader=loader;
				typeInfos.Add(typeInfo->typeName, typeInfo);
				FillParentTypeInfos(typeInfo);
				
				FOREACH(Ptr<VirtualTypeInfo>, derived, typeInfos.Values())
				{
					if(derived->parentTypes.Contains(typeInfo->typeDescriptor))
					{
						FillParentTypeInfos(derived);
					}
				}

				return true;
			}

			IGuiInstanceLoader* GetLoader(const WString& typeName)override
			{
				vint index=typeInfos.Keys().IndexOf(typeName);
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
				vint index=typeInfos.Keys().IndexOf(loader->GetTypeName());
				if(index!=-1)
				{
					Ptr<VirtualTypeInfo> typeInfo = typeInfos.Values()[index];
					if(typeInfo->parentTypeInfos.Count()>0)
					{
						return typeInfo->parentTypeInfos[0]->loader.Obj();
					}
					return rootLoader.Obj();
				}
				return 0;
			}

			description::ITypeDescriptor* GetTypeDescriptorForType(const WString& typeName)override
			{
				vint index=typeInfos.Keys().IndexOf(typeName);
				return index==-1
					?GetGlobalTypeManager()->GetTypeDescriptor(typeName)
					:typeInfos.Values()[index]->typeDescriptor;
			}
		};
		GUI_REGISTER_PLUGIN(GuiInstanceLoaderManager)
	}
}