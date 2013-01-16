#include "ResourceManager.h"

namespace vl
{
	namespace presentation
	{

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

		Ptr<INativeImage> GuiResourceItem::AsImage()
		{
			return content.Cast<INativeImage>();
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
		}
	}
}