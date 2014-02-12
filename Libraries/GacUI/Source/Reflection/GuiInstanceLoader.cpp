#include "GuiInstanceLoader.h"
#include "TypeDescriptors\GuiReflectionEvents.h"
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
			:support(NotSupport)
			, tryParent(false)
			, required(false)
			, constructorParameter(false)
		{
		}

		GuiInstancePropertyInfo::~GuiInstancePropertyInfo()
		{
		}

		Ptr<GuiInstancePropertyInfo> GuiInstancePropertyInfo::Unsupported()
		{
			return new GuiInstancePropertyInfo;
		}

		Ptr<GuiInstancePropertyInfo> GuiInstancePropertyInfo::Assign(description::ITypeDescriptor* typeDescriptor)
		{
			Ptr<GuiInstancePropertyInfo> info = new GuiInstancePropertyInfo;
			info->support = SupportAssign;
			if (typeDescriptor) info->acceptableTypes.Add(typeDescriptor);
			return info;
		}

		Ptr<GuiInstancePropertyInfo> GuiInstancePropertyInfo::AssignWithParent(description::ITypeDescriptor* typeDescriptor)
		{
			Ptr<GuiInstancePropertyInfo> info = Assign(typeDescriptor);
			info->tryParent = true;
			return info;
		}

		Ptr<GuiInstancePropertyInfo> GuiInstancePropertyInfo::Collection(description::ITypeDescriptor* typeDescriptor)
		{
			Ptr<GuiInstancePropertyInfo> info = Assign(typeDescriptor);
			info->support = SupportCollection;
			return info;
		}

		Ptr<GuiInstancePropertyInfo> GuiInstancePropertyInfo::CollectionWithParent(description::ITypeDescriptor* typeDescriptor)
		{
			Ptr<GuiInstancePropertyInfo> info = Collection(typeDescriptor);
			info->tryParent = true;
			return info;
		}

		Ptr<GuiInstancePropertyInfo> GuiInstancePropertyInfo::Set(description::ITypeDescriptor* typeDescriptor)
		{
			Ptr<GuiInstancePropertyInfo> info = new GuiInstancePropertyInfo;
			info->support = SupportSet;
			if (typeDescriptor) info->acceptableTypes.Add(typeDescriptor);
			return info;
		}

		Ptr<GuiInstancePropertyInfo> GuiInstancePropertyInfo::Array(description::ITypeDescriptor* typeDescriptor)
		{
			Ptr<GuiInstancePropertyInfo> info = new GuiInstancePropertyInfo;
			info->support = SupportArray;
			if (typeDescriptor) info->acceptableTypes.Add(typeDescriptor);
			return info;
		}

/***********************************************************************
GuiInstanceEventInfo
***********************************************************************/

		GuiInstanceEventInfo::GuiInstanceEventInfo()
			:support(NotSupport)
			, argumentType(0)
		{
		}

		GuiInstanceEventInfo::~GuiInstanceEventInfo()
		{
		}

		Ptr<GuiInstanceEventInfo> GuiInstanceEventInfo::Unsupported()
		{
			return new GuiInstanceEventInfo;
		}

		Ptr<GuiInstanceEventInfo> GuiInstanceEventInfo::Assign(description::ITypeDescriptor* typeDescriptor)
		{
			Ptr<GuiInstanceEventInfo> info = new GuiInstanceEventInfo;
			info->support = SupportAssign;
			info->argumentType = typeDescriptor;
			return info;
		}

/***********************************************************************
IGuiInstanceLoader
***********************************************************************/

		bool IGuiInstanceLoader::IsDeserializable(const TypeInfo& typeInfo)
		{
			return false;
		}

		description::Value IGuiInstanceLoader::Deserialize(const TypeInfo& typeInfo, const WString& text)
		{
			return Value();
		}

		bool IGuiInstanceLoader::IsCreatable(const TypeInfo& typeInfo)
		{
			return false;
		}

		description::Value IGuiInstanceLoader::CreateInstance(const TypeInfo& typeInfo, collections::Group<WString, description::Value>& constructorArguments)
		{
			return Value();
		}

		bool IGuiInstanceLoader::IsInitializable(const TypeInfo& typeInfo)
		{
			return false;
		}

		Ptr<GuiInstanceContextScope> IGuiInstanceLoader::InitializeInstance(const TypeInfo& typeInfo, description::Value instance)
		{
			return 0;
		}

		void IGuiInstanceLoader::GetPropertyNames(const TypeInfo& typeInfo, List<WString>& propertyNames)
		{
		}

		void IGuiInstanceLoader::GetConstructorParameters(const TypeInfo& typeInfo, collections::List<WString>& propertyNames)
		{
		}

		Ptr<GuiInstancePropertyInfo> IGuiInstanceLoader::GetPropertyType(const PropertyInfo& propertyInfo)
		{
			return 0;
		}

		bool IGuiInstanceLoader::GetPropertyValue(PropertyValue& propertyValue)
		{
			return false;
		}

		bool IGuiInstanceLoader::SetPropertyValue(PropertyValue& propertyValue)
		{
			return false;
		}

		void IGuiInstanceLoader::GetEventNames(const TypeInfo& typeInfo, collections::List<WString>& eventNames)
		{
		}

		Ptr<GuiInstanceEventInfo> IGuiInstanceLoader::GetEventType(const PropertyInfo& eventInfo)
		{
			return 0;
		}

		bool IGuiInstanceLoader::SetEventValue(PropertyValue& propertyValue)
		{
			return false;
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
				Ptr<RegexMatch> match = regexElementName.MatchHead(text);
				if (match && match->Result().Length() != text.Length()) return 0;

				Ptr<ElementName> elementName = new ElementName;
				if (match->Groups().Keys().Contains(L"namespaceName"))
				{
					elementName->namespaceName = match->Groups()[L"namespaceName"][0].Value();
				}
				if (match->Groups().Keys().Contains(L"category"))
				{
					elementName->category = match->Groups()[L"category"][0].Value();
				}
				if (match->Groups().Keys().Contains(L"name"))
				{
					elementName->name = match->Groups()[L"name"][0].Value();
				}
				if (match->Groups().Keys().Contains(L"binding"))
				{
					elementName->binding = match->Groups()[L"binding"][0].Value();
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

			Ptr<DescriptableObject> ResolveResource(Ptr<parsing::xml::XmlElement> element)
			{
				return 0;
			}

			Ptr<DescriptableObject> ResolveResource(const WString& path)
			{
				return 0;
			}

			Ptr<DescriptableObject> ResolveResource(Ptr<DescriptableObject> resource, Ptr<GuiResourcePathResolver> resolver)
			{
				Ptr<XmlDocument> xml = resource.Cast<XmlDocument>();
				if (xml)
				{
					Ptr<GuiInstanceContext> context = GuiInstanceContext::LoadFromXml(xml);
					return context;
				}
				return 0;
			}
		};

/***********************************************************************
GuiDefaultInstanceLoader
***********************************************************************/

		class GuiDefaultInstanceLoader : public Object, public IGuiInstanceLoader
		{
		public:
			static IMethodInfo* GetDefaultConstructor(ITypeDescriptor* typeDescriptor)
			{
				if (auto ctors = typeDescriptor->GetConstructorGroup())
				{
					vint count = ctors->GetMethodCount();
					for (vint i = 0; i < count; i++)
					{
						IMethodInfo* method = ctors->GetMethod(i);
						if (method->GetParameterCount() == 0)
						{
							return method;
						}
					}
				}
				return 0;
			}

			WString GetTypeName()override
			{
				return L"";
			}

			//***********************************************************************************

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

			bool IsCreatable(const TypeInfo& typeInfo)override
			{
				return GetDefaultConstructor(typeInfo.typeDescriptor) != 0;
			}

			description::Value CreateInstance(const TypeInfo& typeInfo, collections::Group<WString, description::Value>& constructorArguments)override
			{
				if (IMethodInfo* method = GetDefaultConstructor(typeInfo.typeDescriptor))
				{
					return method->Invoke(Value(), (Value::xs()));
				}
				return Value();
			}

			bool IsInitializable(const TypeInfo& typeInfo)override
			{
				return false;
			}

			Ptr<GuiInstanceContextScope> InitializeInstance(const TypeInfo& typeInfo, description::Value instance)override
			{
				return 0;
			}

			//***********************************************************************************

			void ProcessGenericType(ITypeInfo* propType, ITypeInfo*& genericType, ITypeInfo*& elementType, bool& readableList, bool& writableList, bool& collectionType)
			{
				genericType = 0;
				elementType = 0;
				readableList = false;
				writableList = false;
				collectionType = false;
				if (propType->GetDecorator() == ITypeInfo::SharedPtr && propType->GetElementType()->GetDecorator() == ITypeInfo::Generic)
				{
					propType = propType->GetElementType();
					genericType = propType->GetElementType();
					if (genericType->GetTypeDescriptor() == description::GetTypeDescriptor<IValueList>())
					{
						elementType = propType->GetGenericArgument(0);
						readableList = true;
						writableList = true;
						collectionType = true;
					}
					else if (genericType->GetTypeDescriptor() == description::GetTypeDescriptor<IValueEnumerator>())
					{
						collectionType = true;
					}
					else if (genericType->GetTypeDescriptor() == description::GetTypeDescriptor<IValueEnumerable>())
					{
						elementType = propType->GetGenericArgument(0);
						readableList = true;
						collectionType = true;
					}
					else if (genericType->GetTypeDescriptor() == description::GetTypeDescriptor<IValueReadonlyList>())
					{
						elementType = propType->GetGenericArgument(0);
						readableList = true;
						collectionType = true;
					}
					else if (genericType->GetTypeDescriptor() == description::GetTypeDescriptor<IValueReadonlyDictionary>())
					{
						collectionType = true;
					}
					else if (genericType->GetTypeDescriptor() == description::GetTypeDescriptor<IValueDictionary>())
					{
						collectionType = true;
					}
				}
			}

			ITypeInfo* GetPropertyReflectionTypeInfo(const PropertyInfo& propertyInfo, GuiInstancePropertyInfo::Support& support)
			{
				support = GuiInstancePropertyInfo::NotSupport;
				IPropertyInfo* prop = propertyInfo.typeInfo.typeDescriptor->GetPropertyByName(propertyInfo.propertyName, true);
				if (prop)
				{
					ITypeInfo* propType = prop->GetReturn();
					ITypeInfo* genericType = 0;
					ITypeInfo* elementType = 0;
					bool readableList = false;
					bool writableList = false;
					bool collectionType = false;
					ProcessGenericType(propType, genericType, elementType, readableList, writableList, collectionType);

					if (prop->IsWritable())
					{
						if (collectionType)
						{
							if (readableList)
							{
								support = GuiInstancePropertyInfo::SupportArray;
								return elementType;
							}
						}
						else if (genericType)
						{
							support = GuiInstancePropertyInfo::SupportAssign;
							return genericType;
						}
						else
						{
							support = GuiInstancePropertyInfo::SupportAssign;
							return propType;
						}
					}
					else if (prop->IsReadable())
					{
						if (collectionType)
						{
							if (writableList)
							{
								support = GuiInstancePropertyInfo::SupportCollection;
								return elementType;
							}
						}
						else if (!genericType)
						{
							if (propType->GetDecorator() == ITypeInfo::SharedPtr || propType->GetDecorator() == ITypeInfo::RawPtr)
							{
								support = GuiInstancePropertyInfo::SupportSet;
								return propType;
							}
						}
					}
				}
				return 0;
			}

			bool FillPropertyInfo(Ptr<GuiInstancePropertyInfo> propertyInfo, ITypeInfo* propType)
			{
				switch (propType->GetDecorator())
				{
				case ITypeInfo::RawPtr:
				case ITypeInfo::SharedPtr:
				case ITypeInfo::Nullable:
					FillPropertyInfo(propertyInfo, propType->GetElementType());
					return true;
				case ITypeInfo::TypeDescriptor:
					propertyInfo->acceptableTypes.Add(propType->GetTypeDescriptor());
					return true;
				}
				return false;
			}

			void CollectPropertyNames(const TypeInfo& typeInfo, ITypeDescriptor* typeDescriptor, collections::List<WString>& propertyNames)
			{
				vint propertyCount = typeDescriptor->GetPropertyCount();
				for (vint i = 0; i < propertyCount; i++)
				{
					WString propertyName = typeDescriptor->GetProperty(i)->GetName();
					if (!propertyNames.Contains(propertyName))
					{
						auto info = GetPropertyType(PropertyInfo(typeInfo, propertyName));
						if (info && info->support != GuiInstancePropertyInfo::NotSupport)
						{
							propertyNames.Add(propertyName);
						}
					}
				}

				vint parentCount = typeDescriptor->GetBaseTypeDescriptorCount();
				for (vint i = 0; i < parentCount; i++)
				{
					CollectPropertyNames(typeInfo, typeDescriptor->GetBaseTypeDescriptor(i), propertyNames);
				}
			}

			//***********************************************************************************

			void GetPropertyNames(const TypeInfo& typeInfo, collections::List<WString>& propertyNames)override
			{
				CollectPropertyNames(typeInfo, typeInfo.typeDescriptor, propertyNames);
			}

			void GetConstructorParameters(const TypeInfo& typeInfo, collections::List<WString>& propertyNames)override
			{
			}

			Ptr<GuiInstancePropertyInfo> GetPropertyType(const PropertyInfo& propertyInfo)override
			{
				GuiInstancePropertyInfo::Support support = GuiInstancePropertyInfo::NotSupport;
				if (ITypeInfo* propType = GetPropertyReflectionTypeInfo(propertyInfo, support))
				{
					Ptr<GuiInstancePropertyInfo> propertyInfo = new GuiInstancePropertyInfo;
					propertyInfo->support = support;

					if (FillPropertyInfo(propertyInfo, propType))
					{
						return propertyInfo;
					}
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

			bool SetPropertyValue(PropertyValue& propertyValue)override
			{
				GuiInstancePropertyInfo::Support support = GuiInstancePropertyInfo::NotSupport;
				if (GetPropertyReflectionTypeInfo(propertyValue, support))
				{
					switch (support)
					{
					case GuiInstancePropertyInfo::SupportCollection:
						{
							Value value = propertyValue.instanceValue.GetProperty(propertyValue.propertyName);
							if (auto list = dynamic_cast<IValueList*>(value.GetRawPtr()))
							{
								list->Add(propertyValue.propertyValue);
								return true;
							}
						}
						break;
					case GuiInstancePropertyInfo::SupportAssign:
					case GuiInstancePropertyInfo::SupportArray:
						propertyValue.instanceValue.SetProperty(propertyValue.propertyName, propertyValue.propertyValue);
						return true;
					}
				}
				return false;
			}

			//***********************************************************************************

			void CollectEventNames(const TypeInfo& typeInfo, ITypeDescriptor* typeDescriptor, collections::List<WString>& eventNames)
			{
				vint eventCount = typeDescriptor->GetEventCount();
				for (vint i = 0; i < eventCount; i++)
				{
					WString eventName = typeDescriptor->GetEvent(i)->GetName();
					if (!eventNames.Contains(eventName))
					{
						auto info = GetEventType(PropertyInfo(typeInfo, eventName));
						if (info && info->support != GuiInstanceEventInfo::NotSupport)
						{
							eventNames.Add(eventName);
						}
					}
				}

				vint parentCount = typeDescriptor->GetBaseTypeDescriptorCount();
				for (vint i = 0; i < parentCount; i++)
				{
					CollectEventNames(typeInfo, typeDescriptor->GetBaseTypeDescriptor(i), eventNames);
				}
			}

			//***********************************************************************************

			void GetEventNames(const TypeInfo& typeInfo, collections::List<WString>& eventNames)override
			{
				CollectEventNames(typeInfo, typeInfo.typeDescriptor, eventNames);
			}

			Ptr<GuiInstanceEventInfo> GetEventType(const PropertyInfo& eventInfo)override
			{
				if (IEventInfo* ev = eventInfo.typeInfo.typeDescriptor->GetEventByName(eventInfo.propertyName, true))
				{
#ifndef VCZH_DEBUG_NO_REFLECTION
					auto handlerType = ev->GetHandlerType();
					if (handlerType->GetDecorator() != ITypeInfo::SharedPtr) goto UNSUPPORTED;

					auto genericType = handlerType->GetElementType();
					if (genericType->GetDecorator() != ITypeInfo::Generic) goto UNSUPPORTED;

					auto functionType = genericType->GetElementType();
					if (functionType->GetDecorator() != ITypeInfo::TypeDescriptor) goto UNSUPPORTED;
					if (functionType->GetTypeDescriptor() != description::GetTypeDescriptor<IValueFunctionProxy>()) goto UNSUPPORTED;

					if (genericType->GetGenericArgumentCount() != 3) goto UNSUPPORTED;
					auto returnType = genericType->GetGenericArgument(0);
					auto senderType = genericType->GetGenericArgument(1);
					auto argumentType = genericType->GetGenericArgument(2);
					
					if (returnType->GetDecorator() != ITypeInfo::TypeDescriptor) goto UNSUPPORTED;
					if (returnType->GetTypeDescriptor() != description::GetTypeDescriptor<VoidValue>()) goto UNSUPPORTED;
					
					if (senderType->GetDecorator() != ITypeInfo::RawPtr) goto UNSUPPORTED;
					senderType = senderType->GetElementType();
					if (senderType->GetDecorator() != ITypeInfo::TypeDescriptor) goto UNSUPPORTED;
					if (senderType->GetTypeDescriptor() != description::GetTypeDescriptor<compositions::GuiGraphicsComposition>()) goto UNSUPPORTED;
					
					if (argumentType->GetDecorator() != ITypeInfo::RawPtr) goto UNSUPPORTED;
					argumentType = argumentType->GetElementType();
					if (argumentType->GetDecorator() != ITypeInfo::TypeDescriptor) goto UNSUPPORTED;
					if (!argumentType->GetTypeDescriptor()->CanConvertTo(description::GetTypeDescriptor<compositions::GuiEventArgs>())) goto UNSUPPORTED;

					return GuiInstanceEventInfo::Assign(argumentType->GetTypeDescriptor());

				UNSUPPORTED:
#endif
					return GuiInstanceEventInfo::Unsupported();
				}
				return 0;
			}

			bool SetEventValue(PropertyValue& propertyValue)override
			{
				auto eventInfo = GetEventType(propertyValue);
				if (eventInfo && eventInfo->support == GuiInstanceEventInfo::SupportAssign)
				{
					propertyValue.instanceValue.AttachEvent(propertyValue.propertyName, propertyValue.propertyValue);
					return true;
				}
				return false;
			}
		};

/***********************************************************************
GuiResourceInstanceLoader
***********************************************************************/

		class GuiResourceInstanceLoader : public Object, public IGuiInstanceLoader
		{
		protected:
			Ptr<GuiResource>						resource;
			Ptr<GuiInstanceContext>					context;
		public:
			GuiResourceInstanceLoader(Ptr<GuiResource> _resource, Ptr<GuiInstanceContext> _context)
				:resource(_resource)
				, context(_context)
			{
			}

			WString GetTypeName()override
			{
				return context->className.Value();
			}

			bool IsCreatable(const TypeInfo& typeInfo)override
			{
				return typeInfo.typeName == context->className.Value();
			}

			description::Value CreateInstance(const TypeInfo& typeInfo, collections::Group<WString, description::Value>& constructorArguments)override
			{
				if (typeInfo.typeName == context->className.Value())
				{
					if (auto typeDescriptor = GetGlobalTypeManager()->GetTypeDescriptor(typeInfo.typeName))
					{
						if (auto method = GuiDefaultInstanceLoader::GetDefaultConstructor(typeDescriptor))
						{
							return method->Invoke(Value(), (Value::xs()));
						}
					}

					Ptr<GuiResourcePathResolver> resolver = new GuiResourcePathResolver(resource, resource->GetWorkingDirectory());
					auto scope = LoadInstanceFromContext(context, resolver);

					if (scope)
					{
						return scope->rootInstance;
					}
				}
				return Value();
			}

			bool IsInitializable(const TypeInfo& typeInfo)override
			{
				return typeInfo.typeName == context->className.Value();
			}

			Ptr<GuiInstanceContextScope> InitializeInstance(const TypeInfo& typeInfo, description::Value instance)override
			{
				if (typeInfo.typeName == context->className.Value())
				{
					Ptr<GuiResourcePathResolver> resolver = new GuiResourcePathResolver(resource, resource->GetWorkingDirectory());
					auto scope = InitializeInstanceFromContext(context, resolver, instance);
					return scope;
				}
				return false;
			}
		};

/***********************************************************************
GuiInstanceLoaderManager
***********************************************************************/

		IGuiInstanceLoaderManager* instanceLoaderManager = 0;

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
			typedef Dictionary<WString, Ptr<GuiResource>>						ResourceMap;

			Ptr<IGuiInstanceLoader>					rootLoader;
			BinderMap								binders;
			VirtualTypeInfoMap						typeInfos;
			ResourceMap								resources;

			bool IsTypeExists(const WString& name)
			{
				return GetGlobalTypeManager()->GetTypeDescriptor(name) != 0 || typeInfos.Keys().Contains(name);
			}

			void FindParentTypeInfos(Ptr<VirtualTypeInfo> typeInfo, ITypeDescriptor* searchType)
			{
				if (searchType != typeInfo->typeDescriptor)
				{
					vint index = typeInfos.Keys().IndexOf(searchType->GetTypeName());
					if (index == -1)
					{
						typeInfo->parentTypes.Add(searchType);
					}
					else
					{
						typeInfo->parentTypeInfos.Add(typeInfos.Values()[index].Obj());
						return;
					}
				}

				vint count = searchType->GetBaseTypeDescriptorCount();
				for (vint i = 0; i < count; i++)
				{
					ITypeDescriptor* baseType = searchType->GetBaseTypeDescriptor(i);
					FindParentTypeInfos(typeInfo, baseType);
				}
			}

			void FillParentTypeInfos(Ptr<VirtualTypeInfo> typeInfo)
			{
				typeInfo->parentTypes.Clear();
				typeInfo->parentTypeInfos.Clear();

				ITypeDescriptor* searchType = typeInfo->typeDescriptor;
				if (!searchType)
				{
					vint index = typeInfos.Keys().IndexOf(typeInfo->parentTypeName);
					if (index == -1)
					{
						searchType = GetGlobalTypeManager()->GetTypeDescriptor(typeInfo->parentTypeName);
						typeInfo->typeDescriptor = searchType;
						typeInfo->parentTypes.Add(searchType);
					}
					else
					{
						VirtualTypeInfo* parentTypeInfo = typeInfos.Values()[index].Obj();
						typeInfo->typeDescriptor = parentTypeInfo->typeDescriptor;
						typeInfo->parentTypeInfos.Add(parentTypeInfo);
						return;
					}
				}

				if (searchType)
				{
					FindParentTypeInfos(typeInfo, searchType);
				}
			}

			IGuiInstanceLoader* GetLoaderFromType(ITypeDescriptor* typeDescriptor)
			{
				vint index = typeInfos.Keys().IndexOf(typeDescriptor->GetTypeName());
				if (index == -1)
				{
					vint count = typeDescriptor->GetBaseTypeDescriptorCount();
					for (vint i = 0; i < count; i++)
					{
						ITypeDescriptor* baseType = typeDescriptor->GetBaseTypeDescriptor(i);
						IGuiInstanceLoader* loader = GetLoaderFromType(baseType);
						if (loader) return loader;
					}
					return 0;
				}
				else
				{
					return typeInfos.Values()[index]->loader.Obj();
				}
			}

			void GetClassesInResource(Ptr<GuiResourceFolder> folder, Dictionary<WString, Ptr<GuiInstanceContext>>& classes)
			{
				FOREACH(Ptr<GuiResourceItem>, item, folder->GetItems())
				{
					if (auto context = item->GetContent().Cast<GuiInstanceContext>())
					{
						if (context->className && !classes.Keys().Contains(context->className.Value()))
						{
							classes.Add(context->className.Value(), context);
						}
					}
				}
				FOREACH(Ptr<GuiResourceFolder>, subFolder, folder->GetFolders())
				{
					GetClassesInResource(subFolder, classes);
				}
			}
		public:
			GuiInstanceLoaderManager()
			{
				rootLoader = new GuiDefaultInstanceLoader;
			}

			void Load()override
			{
				instanceLoaderManager = this;
			}

			void AfterLoad()override
			{
				{
					IGuiResourceResolverManager* manager = GetResourceResolverManager();
					manager->SetTypeResolver(new GuiResourceInstanceTypeResolver);
				}
				{
					IGuiParserManager* manager = GetParserManager();
					manager->SetParser(L"INSTANCE-ELEMENT-NAME", new GuiInstanceContextElementNameParser);
				}
			}

			void Unload()override
			{
				instanceLoaderManager = 0;
			}

			bool AddInstanceBinder(Ptr<IGuiInstanceBinder> binder)override
			{
				if (binders.Keys().Contains(binder->GetBindingName())) return false;
				binders.Add(binder->GetBindingName(), binder);
				return true;
			}

			IGuiInstanceBinder* GetInstanceBinder(const WString& bindingName)override
			{
				vint index = binders.Keys().IndexOf(bindingName);
				return index == -1 ? 0 : binders.Values()[index].Obj();
			}

			bool CreateVirtualType(const WString& parentType, Ptr<IGuiInstanceLoader> loader)override
			{
				if (IsTypeExists(loader->GetTypeName()) || !IsTypeExists(parentType)) return false;

				Ptr<VirtualTypeInfo> typeInfo = new VirtualTypeInfo;
				typeInfo->typeName = loader->GetTypeName();
				typeInfo->parentTypeName = parentType;
				typeInfo->loader = loader;
				typeInfos.Add(loader->GetTypeName(), typeInfo);
				FillParentTypeInfos(typeInfo);

				return true;
			}

			bool SetLoader(Ptr<IGuiInstanceLoader> loader)override
			{
				vint index = typeInfos.Keys().IndexOf(loader->GetTypeName());
				if (index != -1) return false;

				ITypeDescriptor* typeDescriptor = GetGlobalTypeManager()->GetTypeDescriptor(loader->GetTypeName());
				if (typeDescriptor == 0) return false;

				Ptr<VirtualTypeInfo> typeInfo = new VirtualTypeInfo;
				typeInfo->typeName = loader->GetTypeName();
				typeInfo->typeDescriptor = typeDescriptor;
				typeInfo->loader = loader;
				typeInfos.Add(typeInfo->typeName, typeInfo);
				FillParentTypeInfos(typeInfo);

				FOREACH(Ptr<VirtualTypeInfo>, derived, typeInfos.Values())
				{
					if (derived->parentTypes.Contains(typeInfo->typeDescriptor))
					{
						FillParentTypeInfos(derived);
					}
				}

				return true;
			}

			IGuiInstanceLoader* GetLoader(const WString& typeName)override
			{
				vint index = typeInfos.Keys().IndexOf(typeName);
				if (index != -1)
				{
					return typeInfos.Values()[index]->loader.Obj();
				}

				ITypeDescriptor* typeDescriptor = GetGlobalTypeManager()->GetTypeDescriptor(typeName);
				if (typeDescriptor)
				{
					IGuiInstanceLoader* loader = GetLoaderFromType(typeDescriptor);
					return loader ? loader : rootLoader.Obj();
				}
				return 0;
			}

			IGuiInstanceLoader* GetParentLoader(IGuiInstanceLoader* loader)override
			{
				vint index = typeInfos.Keys().IndexOf(loader->GetTypeName());
				if (index != -1)
				{
					Ptr<VirtualTypeInfo> typeInfo = typeInfos.Values()[index];
					if (typeInfo->parentTypeInfos.Count() > 0)
					{
						return typeInfo->parentTypeInfos[0]->loader.Obj();
					}
					return rootLoader.Obj();
				}
				return 0;
			}

			description::ITypeDescriptor* GetTypeDescriptorForType(const WString& typeName)override
			{
				vint index = typeInfos.Keys().IndexOf(typeName);
				return index == -1
					? GetGlobalTypeManager()->GetTypeDescriptor(typeName)
					: typeInfos.Values()[index]->typeDescriptor;
			}

			void GetVirtualTypes(collections::List<WString>& typeNames)override
			{
				for (vint i = 0; i < typeInfos.Count(); i++)
				{
					if (typeInfos.Values()[i]->parentTypeName != L"")
					{
						typeNames.Add(typeInfos.Keys()[i]);
					}
				}
			}

			WString GetParentTypeForVirtualType(const WString& virtualType)override
			{
				vint index = typeInfos.Keys().IndexOf(virtualType);
				if (index != -1)
				{
					auto typeInfo = typeInfos.Values()[index];
					return typeInfo->parentTypeName;
				}
				return L"";
			}

			bool SetResource(const WString& name, Ptr<GuiResource> resource)override
			{
				vint index = resources.Keys().IndexOf(name);
				if (index != -1) return false;

				Ptr<GuiResourcePathResolver> resolver = new GuiResourcePathResolver(resource, resource->GetWorkingDirectory());
				Dictionary<WString, Ptr<GuiInstanceContext>> classes;
				Dictionary<WString, WString> parentTypes;
				GetClassesInResource(resource, classes);

				FOREACH(Ptr<GuiInstanceContext>, context, classes.Values())
				{
					if (typeInfos.Keys().Contains(context->className.Value()))
					{
						return false;
					}

					Ptr<GuiInstanceEnvironment> env = new GuiInstanceEnvironment(context, resolver);
					auto loadingSource = FindInstanceLoadingSource(env, context->instance.Obj());
					if (!loadingSource.loader) return false;
					parentTypes.Add(context->className.Value(), loadingSource.typeName);
				}
				
				FOREACH(WString, className, classes.Keys())
				{
					auto context = classes[className];
					auto parentType = parentTypes[className];

					Ptr<IGuiInstanceLoader> loader = new GuiResourceInstanceLoader(resource, context);
					if (GetGlobalTypeManager()->GetTypeDescriptor(context->className.Value()))
					{
						SetLoader(loader);
					}
					else
					{
						CreateVirtualType(parentType, loader);
					}
				}

				resources.Add(name, resource);
				return true;
			}

			Ptr<GuiResource> GetResource(const WString& name)override
			{
				vint index = resources.Keys().IndexOf(name);
				return index == -1 ? 0 : resources.Values()[index];
			}
		};
		GUI_REGISTER_PLUGIN(GuiInstanceLoaderManager)
	}
}