#include "GuiDocument.h"
#include "..\Controls\GuiApplication.h"
#include "..\..\..\..\Common\Source\Stream\FileStream.h"
#include "..\..\..\..\Common\Source\Stream\Accessor.h"
#include "..\..\..\..\Common\Source\Stream\CharFormat.h"

namespace vl
{
	namespace presentation
	{
		using namespace controls;
		using namespace collections;
		using namespace parsing::tabling;
		using namespace parsing::xml;
		using namespace parsing::json;
		using namespace regex;

		WString GetFolderPath(const WString& filePath)
		{
			for(vint i=filePath.Length()-1;i>=0;i--)
			{
				if(filePath[i]==L'\\' || filePath[i]==L'/')
				{
					return filePath.Sub(0, i+1);
				}
			}
			return L"";
		}

		WString GetFileName(const WString& filePath)
		{
			for(vint i=filePath.Length()-1;i>=0;i--)
			{
				if(filePath[i]==L'\\' || filePath[i]==L'/')
				{
					return filePath.Sub(i+1, filePath.Length()-i-1);
				}
			}
			return filePath;
		}

		bool LoadTextFile(const WString& filePath, WString& text)
		{
			stream::FileStream fileStream(filePath, stream::FileStream::ReadOnly);
			return LoadTextFromStream(fileStream, text);
		}

		bool LoadTextFromStream(stream::IStream& stream, WString& text)
		{
			if(stream.IsAvailable())
			{
				stream::BomDecoder decoder;
				stream::DecoderStream decoderStream(stream, decoder);
				stream::StreamReader reader(decoderStream);
				text=reader.ReadToEnd();
				return true;
			}
			return false;
		}

		bool IsResourceUrl(const WString& text, WString& protocol, WString& path)
		{
			Pair<vint, vint> index=INVLOC.FindFirst(text, L"://", Locale::None);
			if(index.key!=-1)
			{
				protocol=INVLOC.ToLower(text.Sub(0, index.key));
				path=text.Sub(index.key+index.value, text.Length()-index.key-index.value);
				return true;
			}
			else
			{
				return false;
			}
		}

/***********************************************************************
GuiImageData
***********************************************************************/

		GuiImageData::GuiImageData()
			:frameIndex(-1)
		{
		}

		GuiImageData::GuiImageData(Ptr<INativeImage> _image, vint _frameIndex)
			:image(_image)
			,frameIndex(_frameIndex)
		{
		}

		GuiImageData::~GuiImageData()
		{
		}

		Ptr<INativeImage> GuiImageData::GetImage()
		{
			return image;
		}

		vint GuiImageData::GetFrameIndex()
		{
			return frameIndex;
		}

/***********************************************************************
GuiTextData
***********************************************************************/

		GuiTextData::GuiTextData()
		{
		}

		GuiTextData::GuiTextData(const WString& _text)
			:text(_text)
		{
		}

		WString GuiTextData::GetText()
		{
			return text;
		}

/***********************************************************************
GuiResourceNodeBase
***********************************************************************/

		GuiResourceNodeBase::GuiResourceNodeBase()
			:parent(0)
		{
		}

		GuiResourceNodeBase::~GuiResourceNodeBase()
		{
		}

		const WString& GuiResourceNodeBase::GetName()
		{
			return name;
		}

		GuiResourceFolder* GuiResourceNodeBase::GetParent()
		{
			return parent;
		}

/***********************************************************************
GuiResourceItem
***********************************************************************/

		GuiResourceItem::GuiResourceItem()
		{
		}

		GuiResourceItem::~GuiResourceItem()
		{
		}

		Ptr<Object> GuiResourceItem::GetContent()
		{
			return content;
		}

		void GuiResourceItem::SetContent(Ptr<Object> value)
		{
			content=value;
		}

		Ptr<GuiImageData> GuiResourceItem::AsImage()
		{
			return content.Cast<GuiImageData>();
		}

		Ptr<parsing::xml::XmlDocument> GuiResourceItem::AsXml()
		{
			return content.Cast<XmlDocument>();
		}

		Ptr<GuiTextData> GuiResourceItem::AsString()
		{
			return content.Cast<GuiTextData>();
		}

		Ptr<DocumentModel> GuiResourceItem::AsDocument()
		{
			return content.Cast<DocumentModel>();
		}

/***********************************************************************
GuiResourceFolder
***********************************************************************/

		void GuiResourceFolder::LoadResourceFolderXml(DelayLoadingList& delayLoadings, const WString& containingFolder, Ptr<parsing::xml::XmlElement> folderXml, Ptr<parsing::tabling::ParsingTable> xmlParsingTable)
		{
			ClearItems();
			ClearFolders();
			FOREACH(Ptr<XmlElement>, element, XmlGetElements(folderXml))
			{
				WString name;
				if(Ptr<XmlAttribute> nameAtt=XmlGetAttribute(element, L"name"))
				{
					name=nameAtt->value.value;
				}
				if(element->name.value==L"Folder")
				{
					if(name!=L"")
					{
						Ptr<GuiResourceFolder> folder=new GuiResourceFolder;
						if(AddFolder(name, folder))
						{
							WString newContainingFolder=containingFolder;
							Ptr<XmlElement> newFolderXml=element;
							if(Ptr<XmlAttribute> contentAtt=XmlGetAttribute(element, L"content"))
							{
								if(contentAtt->value.value==L"Link")
								{
									WString filePath=containingFolder+XmlGetValue(element);
									WString text;
									if(LoadTextFile(filePath, text))
									{
										Ptr<XmlDocument> xml=XmlParseDocument(text, xmlParsingTable);
										if(xml)
										{
											newContainingFolder=GetFolderPath(filePath);
											newFolderXml=xml->rootElement;
										}
									}
								}
							}
							folder->LoadResourceFolderXml(delayLoadings, newContainingFolder, newFolderXml, xmlParsingTable);
						}
					}
				}
				else
				{
					WString filePath;
					if(Ptr<XmlAttribute> contentAtt=XmlGetAttribute(element, L"content"))
					{
						if(contentAtt->value.value==L"File")
						{
							filePath=containingFolder+XmlGetValue(element);
							if(name==L"")
							{
								name=GetFileName(filePath);
							}
						}
					}

					Ptr<GuiResourceItem> item=new GuiResourceItem;
					if(AddItem(name, item))
					{
						WString type=element->name.value;
						IGuiResourceTypeResolver* typeResolver=GetResourceResolverManager()->GetTypeResolver(type);
						IGuiResourceTypeResolver* preloadResolver=typeResolver;

						if(typeResolver)
						{
							WString preloadType=typeResolver->GetPreloadType();
							if(preloadType!=L"")
							{
								preloadResolver=GetResourceResolverManager()->GetTypeResolver(preloadType);
							}
						}

						if(typeResolver && preloadResolver)
						{
							Ptr<Object> resource;
							if(filePath==L"")
							{
								resource=preloadResolver->ResolveResource(element);
							}
							else
							{
								resource=preloadResolver->ResolveResource(filePath);
							}

							if(typeResolver!=preloadResolver)
							{
								if(typeResolver->IsDelayLoad())
								{
									DelayLoading delayLoading;
									delayLoading.type=type;
									delayLoading.workingDirectory=containingFolder;
									delayLoading.preloadResource=item;
									delayLoadings.Add(delayLoading);
								}
								else if(resource)
								{
									resource=typeResolver->ResolveResource(resource, 0);
								}
							}
							item->SetContent(resource);
						}

						if(!item->GetContent())
						{
							RemoveItem(name);
						}
					}
				}
			}
		}

		GuiResourceFolder::GuiResourceFolder()
		{
		}

		GuiResourceFolder::~GuiResourceFolder()
		{
		}

		const GuiResourceFolder::ItemList& GuiResourceFolder::GetItems()
		{
			return items.Values();
		}

		Ptr<GuiResourceItem> GuiResourceFolder::GetItem(const WString& name)
		{
			vint index=items.Keys().IndexOf(name);
			return index==-1?0:items.Values().Get(index);
		}

		bool GuiResourceFolder::AddItem(const WString& name, Ptr<GuiResourceItem> item)
		{
			if(item->GetParent()!=0 || items.Keys().Contains(name)) return false;
			items.Add(name, item);
			item->parent=this;
			item->name=name;
			return true;
		}

		Ptr<GuiResourceItem> GuiResourceFolder::RemoveItem(const WString& name)
		{
			Ptr<GuiResourceItem> item=GetItem(name);
			if(!item) return 0;
			items.Remove(name);
			return item;
		}

		void GuiResourceFolder::ClearItems()
		{
			items.Clear();
		}

		const GuiResourceFolder::FolderList& GuiResourceFolder::GetFolders()
		{
			return folders.Values();
		}

		Ptr<GuiResourceFolder> GuiResourceFolder::GetFolder(const WString& name)
		{
			vint index=folders.Keys().IndexOf(name);
			return index==-1?0:folders.Values().Get(index);
		}

		bool GuiResourceFolder::AddFolder(const WString& name, Ptr<GuiResourceFolder> folder)
		{
			if(folder->GetParent()!=0 || folders.Keys().Contains(name)) return false;
			folders.Add(name, folder);
			folder->parent=this;
			folder->name=name;
			return true;
		}

		Ptr<GuiResourceFolder> GuiResourceFolder::RemoveFolder(const WString& name)
		{
			Ptr<GuiResourceFolder> folder=GetFolder(name);
			if(!folder) return 0;
			folders.Remove(name);
			return folder;
		}

		void GuiResourceFolder::ClearFolders()
		{
			folders.Clear();
		}

		Ptr<Object> GuiResourceFolder::GetValueByPath(const WString& path)
		{
			const wchar_t* buffer=path.Buffer();
			const wchar_t* index=wcschr(buffer, L'\\');
			if(!index) index=wcschr(buffer, '/');

			if(index)
			{
				WString name=path.Sub(0, index-buffer);
				Ptr<GuiResourceFolder> folder=GetFolder(name);
				if(folder)
				{
					vint start=index-buffer+1;
					return folder->GetValueByPath(path.Sub(start, path.Length()-start));
				}
			}
			else
			{
				Ptr<GuiResourceItem> item=GetItem(path);
				if(item)
				{
					return item->GetContent();
				}
			}
			return 0;
		}

		Ptr<GuiResourceFolder> GuiResourceFolder::GetFolderByPath(const WString& path)
		{
			const wchar_t* buffer=path.Buffer();
			const wchar_t* index=wcschr(buffer, L'\\');
			if(!index) index=wcschr(buffer, '/');
			if(!index) return 0;

			WString name=path.Sub(0, index-buffer);
			Ptr<GuiResourceFolder> folder=GetFolder(name);

			if(index-buffer==path.Length()-1)
			{
				return folder;
			}

			if(folder)
			{
				vint start=index-buffer+1;
				return folder->GetFolderByPath(path.Sub(start, path.Length()-start));
			}

			return 0;
		}

/***********************************************************************
GuiResource
***********************************************************************/

		GuiResource::GuiResource()
		{
		}

		GuiResource::~GuiResource()
		{
		}

		WString GuiResource::GetWorkingDirectory()
		{
			return workingDirectory;
		}

		Ptr<GuiResource> GuiResource::LoadFromXml(const WString& filePath)
		{
			Ptr<ParsingTable> table;
			Ptr<XmlDocument> xml;
			Ptr<GuiResource> resource;
			{
				WString text;
				if(LoadTextFile(filePath, text))
				{
					table=GetParserManager()->GetParsingTable(L"XML");
					xml=XmlParseDocument(text, table);
				}
			}
			if(xml)
			{
				resource=new GuiResource;
				resource->workingDirectory=GetFolderPath(filePath);
				DelayLoadingList delayLoadings;
				resource->LoadResourceFolderXml(delayLoadings, resource->workingDirectory, xml->rootElement, table);

				FOREACH(DelayLoading, delay, delayLoadings)
				{
					WString type=delay.type;
					WString folder=delay.workingDirectory;
					Ptr<GuiResourceItem> item=delay.preloadResource;

					IGuiResourceTypeResolver* typeResolver=GetResourceResolverManager()->GetTypeResolver(type);
					if(typeResolver && item->GetContent())
					{
						Ptr<GuiResourcePathResolver> pathResolver=new GuiResourcePathResolver(resource, folder);
						Ptr<Object> resource=typeResolver->ResolveResource(item->GetContent(), pathResolver);
						if(resource)
						{
							item->SetContent(resource);
						}
					}
				}
			}
			return resource;
		}

		Ptr<DocumentModel> GuiResource::GetDocumentByPath(const WString& path)
		{
			Ptr<DocumentModel> result=GetValueByPath(path).Cast<DocumentModel>();
			if(!result) throw ArgumentException(L"Path not exists.", L"GuiResource::GetDocumentByPath", L"path");
			return result;
		}

		Ptr<GuiImageData> GuiResource::GetImageByPath(const WString& path)
		{
			Ptr<GuiImageData> result=GetValueByPath(path).Cast<GuiImageData>();
			if(!result) throw ArgumentException(L"Path not exists.", L"GuiResource::GetImageByPath", L"path");
			return result;
		}

		Ptr<parsing::xml::XmlDocument> GuiResource::GetXmlByPath(const WString& path)
		{
			Ptr<XmlDocument> result=GetValueByPath(path).Cast<XmlDocument>();
			if(!result) throw ArgumentException(L"Path not exists.", L"GuiResource::GetXmlByPath", L"path");
			return result;
		}

		WString GuiResource::GetStringByPath(const WString& path)
		{
			Ptr<ObjectBox<WString>> result=GetValueByPath(path).Cast<ObjectBox<WString>>();
			if(!result) throw ArgumentException(L"Path not exists.", L"GuiResource::GetStringByPath", L"path");
			return result->Unbox();
		}

/***********************************************************************
IGuiParserManager
***********************************************************************/

		IGuiParserManager* parserManager=0;

		IGuiParserManager* GetParserManager()
		{
			return parserManager;
		}

		class GuiParserManager : public Object, public IGuiParserManager, public IGuiPlugin
		{
		protected:
			Dictionary<WString, Ptr<Table>>				tables;
			Dictionary<WString, Func<Ptr<Table>()>>		loaders;
			SpinLock									lock;
		public:
			GuiParserManager()
			{
			}

			~GuiParserManager()
			{
			}

			void Load()override
			{
				parserManager=this;
				SetParsingTable(L"XML", &XmlLoadTable);
				SetParsingTable(L"JSON", &JsonLoadTable);
			}

			void AfterLoad()override
			{
			}

			void Unload()override
			{
				parserManager=0;
			}

			Ptr<Table> GetParsingTable(const WString& name)
			{
				SPIN_LOCK(lock)
				{
					vint index=tables.Keys().IndexOf(name);
					if(index!=-1)
					{
						return tables.Values()[index];
					}

					index=loaders.Keys().IndexOf(name);
					if(index!=-1)
					{
						Ptr<Table> table=loaders.Values()[index]();
						tables.Add(name, table);
						return table;
					}
				}
				return 0;
			}

			bool SetParsingTable(const WString& name, Func<Ptr<Table>()> loader)
			{
				if(loaders.Keys().Contains(name)) return false;
				loaders.Add(name, loader);
				return true;
			}
		};
		GUI_REGISTER_PLUGIN(GuiParserManager)

/***********************************************************************
GuiResourcePathResolver
***********************************************************************/

		GuiResourcePathResolver::GuiResourcePathResolver(Ptr<GuiResource> _resource, const WString& _workingDirectory)
			:resource(_resource)
			,workingDirectory(_workingDirectory)
		{
		}

		GuiResourcePathResolver::~GuiResourcePathResolver()
		{
		}

		Ptr<Object> GuiResourcePathResolver::ResolveResource(const WString& protocol, const WString& path)
		{
			Ptr<IGuiResourcePathResolver> resolver;
			vint index=resolvers.Keys().IndexOf(protocol);
			if(index==-1)
			{
				IGuiResourcePathResolverFactory* factory=GetResourceResolverManager()->GetPathResolverFactory(protocol);
				if(factory)
				{
					resolver=factory->CreateResolver(resource, workingDirectory);
				}
				resolvers.Add(protocol, resolver);
			}
			else
			{
				resolver=resolvers.Values()[index];
			}

			if(resolver)
			{
				return resolver->ResolveResource(path);
			}
			else
			{
				return 0;
			}
		}

/***********************************************************************
GuiResourcePathFileResolver
***********************************************************************/

		class GuiResourcePathFileResolver : public Object, public IGuiResourcePathResolver
		{
		protected:
			WString					workingDirectory;

		public:
			GuiResourcePathFileResolver(const WString& _workingDirectory)
				:workingDirectory(_workingDirectory)
			{
			}

			Ptr<Object> ResolveResource(const WString& path)
			{
				WString filename=path;
				if(filename.Length()>=2 && filename[1]!=L':')
				{
					filename=workingDirectory+filename;
				}
				Ptr<INativeImage> image=GetCurrentController()->ImageService()->CreateImageFromFile(filename);
				return new GuiImageData(image, 0);
			}

			class Factory : public Object, public IGuiResourcePathResolverFactory
			{
			public:
				WString GetProtocol()override
				{
					return L"file";
				}

				Ptr<IGuiResourcePathResolver> CreateResolver(Ptr<GuiResource> resource, const WString& workingDirectory)override
				{
					return new GuiResourcePathFileResolver(workingDirectory);
				}
			};
		};

/***********************************************************************
GuiResourcePathResResolver
***********************************************************************/

		class GuiResourcePathResResolver : public Object, public IGuiResourcePathResolver
		{
		protected:
			Ptr<GuiResource>		resource;

		public:
			GuiResourcePathResResolver(Ptr<GuiResource> _resource)
				:resource(_resource)
			{
			}

			Ptr<Object> ResolveResource(const WString& path)
			{
				return resource?resource->GetValueByPath(path):0;
			}

			class Factory : public Object, public IGuiResourcePathResolverFactory
			{
			public:
				WString GetProtocol()override
				{
					return L"res";
				}

				Ptr<IGuiResourcePathResolver> CreateResolver(Ptr<GuiResource> resource, const WString& workingDirectory)override
				{
					return new GuiResourcePathResResolver(resource);
				}
			};
		};

/***********************************************************************
IGuiResourceResolverManager
***********************************************************************/

		IGuiResourceResolverManager* resourceResolverManager=0;

		IGuiResourceResolverManager* GetResourceResolverManager()
		{
			return resourceResolverManager;
		}

		class GuiResourceResolverManager : public Object, public IGuiResourceResolverManager, public IGuiPlugin
		{
		protected:
			Dictionary<WString, Ptr<IGuiResourcePathResolverFactory>>		pathFactories;
			Dictionary<WString, Ptr<IGuiResourceTypeResolver>>				typeResolvers;

		public:
			GuiResourceResolverManager()
			{
			}

			~GuiResourceResolverManager()
			{
			}

			void Load()override
			{
				resourceResolverManager=this;
				SetPathResolverFactory(new GuiResourcePathFileResolver::Factory);
				SetPathResolverFactory(new GuiResourcePathResResolver::Factory);
			}

			void AfterLoad()override
			{
			}

			void Unload()override
			{
				resourceResolverManager=0;
			}

			IGuiResourcePathResolverFactory* GetPathResolverFactory(const WString& protocol)override
			{
				vint index=pathFactories.Keys().IndexOf(protocol);
				return index==-1?0:pathFactories.Values()[index].Obj();
			}

			bool SetPathResolverFactory(Ptr<IGuiResourcePathResolverFactory> factory)override
			{
				if(pathFactories.Keys().Contains(factory->GetProtocol())) return false;
				pathFactories.Add(factory->GetProtocol(), factory);
				return true;
			}

			IGuiResourceTypeResolver* GetTypeResolver(const WString& type)override
			{
				vint index=typeResolvers.Keys().IndexOf(type);
				return index==-1?0:typeResolvers.Values()[index].Obj();
			}

			bool SetTypeResolver(Ptr<IGuiResourceTypeResolver> resolver)override
			{
				if(typeResolvers.Keys().Contains(resolver->GetType())) return false;
				typeResolvers.Add(resolver->GetType(), resolver);
				return true;
			}
		};
		GUI_REGISTER_PLUGIN(GuiResourceResolverManager)
	}
}