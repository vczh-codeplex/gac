#include "ResourceManager.h"

namespace vl
{
	namespace presentation
	{

/***********************************************************************
GuiResourceItem
***********************************************************************/

		GuiResourceItem::GuiResourceItem()
			:parent(0)
		{
		}

		GuiResourceItem::~GuiResourceItem()
		{
		}

		const WString& GuiResourceItem::GetName()
		{
			return name;
		}

		GuiResourceFolder* GuiResourceItem::GetParent()
		{
			return parent;
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

/***********************************************************************
GuiResource
***********************************************************************/

/***********************************************************************
Resource
***********************************************************************/
	}
}