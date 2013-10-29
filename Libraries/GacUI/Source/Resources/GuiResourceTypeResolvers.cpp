#include "GuiResource.h"
#include "GuiDocument.h"
#include "GuiParserManager.h"
#include "..\Controls\GuiApplication.h"

namespace vl
{
	namespace presentation
	{
		using namespace controls;
		using namespace parsing;
		using namespace parsing::tabling;
		using namespace parsing::xml;

/***********************************************************************
Image Type Resolver
***********************************************************************/

		class GuiResourceImageTypeResolver : public Object, public IGuiResourceTypeResolver
		{
		public:
			WString GetType()
			{
				return L"Image";
			}

			WString GetPreloadType()
			{
				return L"";
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
				Ptr<INativeImage> image=GetCurrentController()->ImageService()->CreateImageFromFile(path);
				if(image)
				{
					return new GuiImageData(image, 0);
				}
				return 0;
			}

			Ptr<Object> ResolveResource(Ptr<Object> resource, Ptr<GuiResourcePathResolver> resolver)
			{
				return 0;
			}
		};

/***********************************************************************
Text Type Resolver
***********************************************************************/

		class GuiResourceTextTypeResolver : public Object, public IGuiResourceTypeResolver
		{
		public:
			WString GetType()
			{
				return L"Text";
			}

			WString GetPreloadType()
			{
				return L"";
			}

			bool IsDelayLoad()
			{
				return false;
			}

			Ptr<Object> ResolveResource(Ptr<parsing::xml::XmlElement> element)
			{
				return new GuiTextData(XmlGetValue(element));
			}

			Ptr<Object> ResolveResource(const WString& path)
			{
				WString text;
				if(LoadTextFile(path, text))
				{
					return new GuiTextData(text);
				}
				return 0;
			}

			Ptr<Object> ResolveResource(Ptr<Object> resource, Ptr<GuiResourcePathResolver> resolver)
			{
				return 0;
			}
		};

/***********************************************************************
Xml Type Resolver
***********************************************************************/

		class GuiResourceXmlTypeResolver : public Object, public IGuiResourceTypeResolver
		{
		public:
			WString GetType()
			{
				return L"Xml";
			}

			WString GetPreloadType()
			{
				return L"";
			}

			bool IsDelayLoad()
			{
				return false;
			}

			Ptr<Object> ResolveResource(Ptr<parsing::xml::XmlElement> element)
			{
				Ptr<XmlElement> root=XmlGetElements(element).First(0);
				if(root)
				{
					Ptr<XmlDocument> xml=new XmlDocument;
					xml->rootElement=root;
					return xml;
				}
				return 0;
			}

			Ptr<Object> ResolveResource(const WString& path)
			{
				Ptr<ParsingTable> table=GetParserManager()->GetParsingTable(L"XML");
				if(table)
				{
					WString text;
					if(LoadTextFile(path, text))
					{
						Ptr<XmlDocument> xml=XmlParseDocument(text, table);
						return xml;
					}
				}
				return 0;
			}

			Ptr<Object> ResolveResource(Ptr<Object> resource, Ptr<GuiResourcePathResolver> resolver)
			{
				return 0;
			}
		};

/***********************************************************************
Doc Type Resolver
***********************************************************************/

		class GuiResourceDocTypeResolver : public Object, public IGuiResourceTypeResolver
		{
		public:
			WString GetType()
			{
				return L"Doc";
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
					Ptr<DocumentModel> model=DocumentModel::LoadFromXml(xml, resolver);
					return model;
				}
				return 0;
			}
		};

/***********************************************************************
Type Resolver Plugin
***********************************************************************/

		class GuiResourceTypeResolverPlugin : public Object, public IGuiPlugin
		{
		public:
			void Load()override
			{
			}

			void AfterLoad()override
			{
				IGuiResourceResolverManager* manager=GetResourceResolverManager();
				manager->SetTypeResolver(new GuiResourceImageTypeResolver);
				manager->SetTypeResolver(new GuiResourceTextTypeResolver);
				manager->SetTypeResolver(new GuiResourceXmlTypeResolver);
				manager->SetTypeResolver(new GuiResourceDocTypeResolver);
			}

			void Unload()override
			{
			}
		};
		GUI_REGISTER_PLUGIN(GuiResourceTypeResolverPlugin)
	}
}