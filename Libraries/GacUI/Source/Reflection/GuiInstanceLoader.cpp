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

/***********************************************************************
GuiInstanceContext::ElementName Parser
***********************************************************************/

		class GuiInstanceContextElementNameParser : public Object, public IGuiParser<GuiInstanceContext::ElementName>
		{
			typedef GuiInstanceContext::ElementName			ElementName;
		public:
			Regex						regexElementName;

			GuiInstanceContextElementNameParser()
				:regexElementName(L"(<namespaceName>[a-zA-Z_]/w*:)?(<category>[a-zA-Z_]/w*.)?(<name>[a-zA-Z_]/w*)(-<binding>[a-zA-Z_]/w*)?")
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
				return true;
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
					Ptr<GuiInstanceContext> context=GuiInstanceContext::LoadFromXml(xml, resolver);
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
				throw 0;
			}

			IGuiInstanceBinder* GetInstanceBinder(const WString& name)override
			{
				throw 0;
			}

			bool SetLoaderForType(description::ITypeDescriptor* baseType, Ptr<IGuiInstanceLoader> loader)override
			{
				throw 0;
			}

			bool SetLoaderForVirtualType(const WString& typeName, description::ITypeDescriptor* baseType, Ptr<IGuiInstanceLoader> loader)override
			{
				throw 0;
			}

			IGuiInstanceLoader* GetLoaderFromType(const WString& typeName)override
			{
				throw 0;
			}

			IGuiInstanceLoader* GetLoaderForInstance(Ptr<GuiInstanceContext> context, Ptr<GuiConstructorRepr> ctor, Ptr<GuiResourcePathResolver> resolver)override
			{
				throw 0;
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