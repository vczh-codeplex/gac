#include "GuiResource.h"
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
DocumentFileProtocolResolver
***********************************************************************/

		Ptr<INativeImage> DocumentFileProtocolResolver::ResolveImageInternal(const WString& protocol, const WString& path)
		{
			if(INVLOC.ToUpper(protocol)!=L"FILE") return 0;
			WString filename=path;
			if(filename.Length()>=2 && filename[1]!=L':')
			{
				filename=workingDirectory+filename;
			}
			return GetCurrentController()->ImageService()->CreateImageFromFile(filename);
		}

		DocumentFileProtocolResolver::DocumentFileProtocolResolver(const WString& _workingDirectory, Ptr<DocumentResolver> previousResolver)
			:DocumentResolver(previousResolver)
			,workingDirectory(_workingDirectory)
		{
		}

/***********************************************************************
DocumentResProtocolResolver
***********************************************************************/

		Ptr<INativeImage> DocumentResProtocolResolver::ResolveImageInternal(const WString& protocol, const WString& path)
		{
			if(INVLOC.ToUpper(protocol)!=L"RES") return 0;
			Ptr<GuiImageData> image=resource->GetValueByPath(path).Cast<GuiImageData>();
			if(image)
			{
				return image->GetImage();
			}
			else
			{
				return 0;
			}
		}

		DocumentResProtocolResolver::DocumentResProtocolResolver(Ptr<GuiResource> _resource, Ptr<DocumentResolver> previousResolver)
			:DocumentResolver(previousResolver)
			,resource(_resource)
		{
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

		Ptr<ObjectBox<WString>> GuiResourceItem::AsString()
		{
			return content.Cast<ObjectBox<WString>>();
		}

		Ptr<DocumentModel> GuiResourceItem::AsDocument()
		{
			return content.Cast<DocumentModel>();
		}

/***********************************************************************
GuiResourceFolder
***********************************************************************/

		void GuiResourceFolder::LoadResourceFolderXml(DelayLoading& delayLoading, const WString& containingFolder, Ptr<parsing::xml::XmlElement> folderXml, Ptr<parsing::tabling::ParsingTable> xmlParsingTable)
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
							folder->LoadResourceFolderXml(delayLoading, newContainingFolder, newFolderXml, xmlParsingTable);
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
						if(filePath!=L"")
						{
							stream::FileStream fileStream(filePath, stream::FileStream::ReadOnly);
							if(fileStream.IsAvailable())
							{
								if(element->name.value==L"Xml" || element->name.value==L"Doc")
								{
									WString text;
									if(LoadTextFromStream(fileStream, text))
									{
										Ptr<XmlDocument> xml=XmlParseDocument(text, xmlParsingTable);
										if(xml)
										{
											item->SetContent(xml);
											if(element->name.value==L"Doc")
											{
												delayLoading.documentModelFolders.Add(item, containingFolder);
											}
										}
									}
								}
								else if(element->name.value==L"Text")
								{
									WString text;
									if(LoadTextFromStream(fileStream, text))
									{
										item->SetContent(new ObjectBox<WString>(text));
									}
								}
								else if(element->name.value==L"Image")
								{
									Ptr<INativeImage> image=GetCurrentController()->ImageService()->CreateImageFromStream(fileStream);
									if(image)
									{
										Ptr<GuiImageData> imageData=new GuiImageData(image, 0);
										item->SetContent(imageData);
									}
								}
							}
						}
						else
						{
							if(element->name.value==L"Xml" || element->name.value==L"Doc")
							{
								Ptr<XmlElement> root=XmlGetElements(element).First(0);
								if(root)
								{
									Ptr<XmlDocument> xml=new XmlDocument;
									xml->rootElement=root;
									item->SetContent(xml);
									if(element->name.value==L"Doc")
									{
										delayLoading.documentModelFolders.Add(item, containingFolder);
									}
								}
							}
							else if(element->name.value==L"Text")
							{
								WString text=XmlGetValue(element);
								item->SetContent(new ObjectBox<WString>(text));
							}
							else if(element->name.value==L"Image")
							{
							}
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

/***********************************************************************
GuiResource
***********************************************************************/

		GuiResource::GuiResource()
		{
		}

		GuiResource::~GuiResource()
		{
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
				DelayLoading delayLoading;
				resource->LoadResourceFolderXml(delayLoading, GetFolderPath(filePath), xml->rootElement, table);

				for(vint i=0;i<delayLoading.documentModelFolders.Count();i++)
				{
					Ptr<GuiResourceItem> item=delayLoading.documentModelFolders.Keys()[i];
					WString folder=delayLoading.documentModelFolders.Values().Get(i);
					if(Ptr<XmlDocument> xml=item->AsXml())
					{
						Ptr<DocumentResolver> resolver=new DocumentResProtocolResolver(resource, new DocumentFileProtocolResolver(folder));
						Ptr<DocumentModel> model=DocumentModel::LoadFromXml(xml, resolver);
						item->SetContent(model);
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
	}
}