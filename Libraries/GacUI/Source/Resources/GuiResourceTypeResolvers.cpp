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
			WString GetType()override
			{
				return L"Image";
			}

			WString GetPreloadType()override
			{
				return L"";
			}

			bool IsDelayLoad()override
			{
				return false;
			}

			Ptr<parsing::xml::XmlElement> Serialize(Ptr<DescriptableObject> resource)override
			{
				return 0;
			}

			Ptr<DescriptableObject> ResolveResource(Ptr<parsing::xml::XmlElement> element, collections::List<WString>& errors)override
			{
				errors.Add(L"Image resource should be an image file.");
				return 0;
			}

			Ptr<DescriptableObject> ResolveResource(const WString& path, collections::List<WString>& errors)override
			{
				Ptr<INativeImage> image = GetCurrentController()->ImageService()->CreateImageFromFile(path);
				if(image)
				{
					return new GuiImageData(image, 0);
				}
				else
				{
					errors.Add(L"Failed to load file \"" + path + L"\".");
					return 0;
				}
			}

			Ptr<DescriptableObject> ResolveResource(Ptr<DescriptableObject> resource, Ptr<GuiResourcePathResolver> resolver, collections::List<WString>& errors)override
			{
				errors.Add(L"Internal error: Image resource doesn't need resource preloading.");
				return 0;
			}
		};

/***********************************************************************
Text Type Resolver
***********************************************************************/

		class GuiResourceTextTypeResolver : public Object, public IGuiResourceTypeResolver
		{
		public:
			WString GetType()override
			{
				return L"Text";
			}

			WString GetPreloadType()override
			{
				return L"";
			}

			bool IsDelayLoad()override
			{
				return false;
			}

			Ptr<parsing::xml::XmlElement> Serialize(Ptr<DescriptableObject> resource)override
			{
				if (auto obj = resource.Cast<GuiTextData>())
				{
					auto xmlContent = MakePtr<XmlText>();
					xmlContent->content.value = obj->GetText();

					auto xmlText = MakePtr<XmlElement>();
					xmlText->name.value = L"Text";
					xmlText->subNodes.Add(xmlContent);

					return xmlText;
				}
				return 0;
			}

			Ptr<DescriptableObject> ResolveResource(Ptr<parsing::xml::XmlElement> element, collections::List<WString>& errors)override
			{
				return new GuiTextData(XmlGetValue(element));
			}

			Ptr<DescriptableObject> ResolveResource(const WString& path, collections::List<WString>& errors)override
			{
				WString text;
				if(LoadTextFile(path, text))
				{
					return new GuiTextData(text);
				}
				else
				{
					errors.Add(L"Failed to load file \"" + path + L"\".");
					return 0;
				}
			}

			Ptr<DescriptableObject> ResolveResource(Ptr<DescriptableObject> resource, Ptr<GuiResourcePathResolver> resolver, collections::List<WString>& errors)override
			{
				errors.Add(L"Internal error: Text resource doesn't need resource preloading.");
				return 0;
			}
		};

/***********************************************************************
Xml Type Resolver
***********************************************************************/

		class GuiResourceXmlTypeResolver : public Object, public IGuiResourceTypeResolver
		{
		public:
			WString GetType()override
			{
				return L"Xml";
			}

			WString GetPreloadType()override
			{
				return L"";
			}

			bool IsDelayLoad()override
			{
				return false;
			}

			Ptr<parsing::xml::XmlElement> Serialize(Ptr<DescriptableObject> resource)override
			{
				if (auto obj = resource.Cast<XmlDocument>())
				{
					auto xmlXml = MakePtr<XmlElement>();
					xmlXml->name.value = L"Xml";
					xmlXml->subNodes.Add(obj->rootElement);
					return xmlXml;
				}
				return 0;
			}

			Ptr<DescriptableObject> ResolveResource(Ptr<parsing::xml::XmlElement> element, collections::List<WString>& errors)override
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

			Ptr<DescriptableObject> ResolveResource(const WString& path, collections::List<WString>& errors)override
			{
				if(auto parser=GetParserManager()->GetParser<XmlDocument>(L"XML"))
				{
					WString text;
					if(LoadTextFile(path, text))
					{
						return parser->TypedParse(text, errors);
					}
					else
					{
						errors.Add(L"Failed to load file \"" + path + L"\".");
					}
				}
				return 0;
			}

			Ptr<DescriptableObject> ResolveResource(Ptr<DescriptableObject> resource, Ptr<GuiResourcePathResolver> resolver, collections::List<WString>& errors)override
			{
				errors.Add(L"Internal error: Xml resource doesn't need resource preloading.");
				return 0;
			}
		};

/***********************************************************************
Doc Type Resolver
***********************************************************************/

		class GuiResourceDocTypeResolver : public Object, public IGuiResourceTypeResolver
		{
		public:
			WString GetType()override
			{
				return L"Doc";
			}

			WString GetPreloadType()override
			{
				return L"Xml";
			}

			bool IsDelayLoad()override
			{
				return true;
			}

			Ptr<parsing::xml::XmlElement> Serialize(Ptr<DescriptableObject> resource)override
			{
				if (auto obj = resource.Cast<DocumentModel>())
				{
					auto xmlDoc = MakePtr<XmlElement>();
					xmlDoc->name.value = L"Doc";
					xmlDoc->subNodes.Add(obj->SaveToXml()->rootElement);
					return xmlDoc;
				}
				return 0;
			}

			Ptr<DescriptableObject> ResolveResource(Ptr<parsing::xml::XmlElement> element, collections::List<WString>& errors)override
			{
				errors.Add(L"Internal error: Doc resource needs resource preloading.");
				return 0;
			}

			Ptr<DescriptableObject> ResolveResource(const WString& path, collections::List<WString>& errors)override
			{
				errors.Add(L"Internal error: Doc resource needs resource preloading.");
				return 0;
			}

			Ptr<DescriptableObject> ResolveResource(Ptr<DescriptableObject> resource, Ptr<GuiResourcePathResolver> resolver, collections::List<WString>& errors)override
			{
				Ptr<XmlDocument> xml = resource.Cast<XmlDocument>();
				if(xml)
				{
					Ptr<DocumentModel> model=DocumentModel::LoadFromXml(xml, resolver, errors);
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