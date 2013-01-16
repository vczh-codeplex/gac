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

		class GuiResourceItem : public Object
		{
			friend class GuiResourceFolder;
		protected:
			WString						name;
			Ptr<Object>					content;
			GuiResourceFolder*			parent;
			
		public:
			GuiResourceItem();
			~GuiResourceItem();

			GuiResourceFolder*			GetParent();
			const WString&				GetName();
			Ptr<Object>					GetContent();
			void						SetContent(Ptr<Object> value);

			Ptr<INativeImage>			AsImage();
			Ptr<XmlDocument>			AsXml();
			Ptr<ObjectBox<WString>>		AsString();
		};

		class GuiResourceFolder : public Object
		{
		protected:
		public:
		};

		class GuiResource : public GuiResourceFolder
		{
		};

/***********************************************************************
Resource Loader
***********************************************************************/
	}
}

#endif