#include "ResourceManager.h"

namespace vl
{
	namespace presentation
	{
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

		Ptr<XmlDocument> GuiResourceItem::AsXml()
		{
			return content.Cast<XmlDocument>();
		}

		Ptr<ObjectBox<WString>> GuiResourceItem::AsString()
		{
			return content.Cast<ObjectBox<WString>>();
		}

/***********************************************************************
GuiResourceFolder
***********************************************************************/

		GuiResourceFolder::GuiResourceFolder()
		{
		}

		GuiResourceFolder::~GuiResourceFolder()
		{
		}

		const List<Ptr<GuiResourceItem>>& GuiResourceFolder::GetItems()
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

		const List<Ptr<GuiResourceFolder>>& GuiResourceFolder::GetFolders()
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

		void GuiResourceFolder::LoadResourceFolderXml(const WString& containingFolder, Ptr<XmlElement> folderXml, Ptr<ParsingTable> xmlParsingTable)
		{
			ClearItems();
			ClearFolders();
			FOREACH(Ptr<XmlNode>, node, folderXml->subNodes)
			{
				Ptr<XmlElement> element=node.Cast<XmlElement>();
				if(element)
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
										FileStream fileStream(filePath, FileStream::ReadOnly);
										if(fileStream.IsAvailable())
										{
											BomDecoder decoder;
											DecoderStream decoderStream(fileStream, decoder);
											StreamReader reader(decoderStream);
											WString text=reader.ReadToEnd();
											Ptr<XmlDocument> xml=XmlParseDocument(text, xmlParsingTable);
											if(xml)
											{
												newContainingFolder=GetFolderPath(filePath);
												newFolderXml=xml->rootElement;
											}
										}
									}
								}
								folder->LoadResourceFolderXml(newContainingFolder, newFolderXml, xmlParsingTable);
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
								FileStream fileStream(filePath, FileStream::ReadOnly);
								if(fileStream.IsAvailable())
								{
									if(element->name.value==L"Xml")
									{
										BomDecoder decoder;
										DecoderStream decoderStream(fileStream, decoder);
										StreamReader reader(decoderStream);
										WString text=reader.ReadToEnd();
										Ptr<XmlDocument> xml=XmlParseDocument(text, xmlParsingTable);
										item->SetContent(xml);
									}
									else if(element->name.value==L"Text")
									{
										BomDecoder decoder;
										DecoderStream decoderStream(fileStream, decoder);
										StreamReader reader(decoderStream);
										WString text=reader.ReadToEnd();
										item->SetContent(new ObjectBox<WString>(text));
									}
									else if(element->name.value==L"Image")
									{
										Ptr<INativeImage> image=GetCurrentController()->ImageService()->CreateImageFromStream(fileStream);
										if(image)
										{
											Ptr<GuiImageData> imageData=new GuiImageData(image, -1);
											item->SetContent(imageData);
										}
									}
								}
							}
							else
							{
								if(element->name.value==L"Xml")
								{
									WString text=XmlGetValue(element);
									Ptr<XmlDocument> xml=XmlParseDocument(text, xmlParsingTable);
									item->SetContent(xml);
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

		void GuiResource::LoadResourceXml(const WString& filePath)
		{
			Ptr<ParsingTable> table=XmlLoadTable();
			Ptr<XmlDocument> xml;
			{
				FileStream fileStream(filePath, FileStream::ReadOnly);
				BomDecoder decoder;
				DecoderStream decoderStream(fileStream, decoder);
				StreamReader reader(decoderStream);
				WString xmlText=reader.ReadToEnd();
				xml=XmlParseDocument(xmlText, table);
			}
			if(xml)
			{
				LoadResourceFolderXml(GetFolderPath(filePath), xml->rootElement, table);
			}
		}
	}
}