/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Resource

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_RESOURCE
#define VCZH_PRESENTATION_RESOURCE

#include "..\..\Public\Source\GacUI.h"

using namespace vl::stream;
using namespace vl::collections;
using namespace vl::regex;
using namespace vl::parsing;
using namespace vl::parsing::xml;
using namespace vl::parsing::json;
using namespace vl::parsing::tabling;

namespace vl
{
	namespace presentation
	{

/***********************************************************************
Resource Structure
***********************************************************************/

		class GuiResourceItem;
		class GuiResourceFolder;
		class GuiResource;

		class GuiResourceNodeBase : public Object
		{
			friend class GuiResourceFolder;
		protected:
			GuiResourceFolder*						parent;
			WString									name;
			
		public:
			GuiResourceNodeBase();
			~GuiResourceNodeBase();

			GuiResourceFolder*						GetParent();
			const WString&							GetName();
		};

		class GuiResourceItem : public GuiResourceNodeBase
		{
			friend class GuiResourceFolder;
		protected:
			Ptr<Object>								content;
			
		public:
			GuiResourceItem();
			~GuiResourceItem();

			Ptr<Object>								GetContent();
			void									SetContent(Ptr<Object> value);

			Ptr<GuiImageData>						AsImage();
			Ptr<XmlDocument>						AsXml();
			Ptr<ObjectBox<WString>>					AsString();
		};

		class GuiResourceFolder : public GuiResourceNodeBase
		{
		protected:
			Dictionary<WString, Ptr<GuiResourceItem>>		items;
			Dictionary<WString, Ptr<GuiResourceFolder>>		folders;
		public:
			GuiResourceFolder();
			~GuiResourceFolder();

			const List<Ptr<GuiResourceItem>>&		GetItems();
			Ptr<GuiResourceItem>					GetItem(const WString& name);
			bool									AddItem(const WString& name, Ptr<GuiResourceItem> item);
			Ptr<GuiResourceItem>					RemoveItem(const WString& name);
			void									ClearItems();

			const List<Ptr<GuiResourceFolder>>&		GetFolders();
			Ptr<GuiResourceFolder>					GetFolder(const WString& name);
			bool									AddFolder(const WString& name, Ptr<GuiResourceFolder> folder);
			Ptr<GuiResourceFolder>					RemoveFolder(const WString& name);
			void									ClearFolders();

			Ptr<Object>								GetValueByPath(const WString& path);

			void									LoadResourceFolderXml(const WString& containingFolder, Ptr<XmlElement> folderXml, Ptr<ParsingTable> xmlParsingTable);
		};

/***********************************************************************
Resource Loader
***********************************************************************/

		class GuiResource : public GuiResourceFolder
		{
		public:
			GuiResource();
			~GuiResource();

			void									LoadResourceXml(const WString& filePath);
		};
	}
}

#endif