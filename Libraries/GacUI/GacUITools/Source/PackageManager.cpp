#include "PackageManager.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
UI Builder
***********************************************************************/

		void EnumeratePackages(Ptr<GuiResource> resource, List<Ptr<XmlElement>>& packages)
		{
			CopyFrom(packages,
				resource
					->GetFolder(L"Packages")
					->GetItems()
					>>Select(Func<Ptr<Object>(Ptr<GuiResourceItem>)>(
						[](Ptr<GuiResourceItem> item)
						{
							return item->GetContent();
						}))
					>>FindType<Object, XmlDocument>()
					>>Where(Func<bool(Ptr<XmlDocument>)>(
						[](Ptr<XmlDocument> document)
						{
							return document->rootElement->name.value==L"Package";
						}))
					>>Select(Func<Ptr<XmlElement>(Ptr<XmlDocument>)>(
						[](Ptr<XmlDocument> document)
						{
							return document->rootElement;
						}))
					);
		}

		void PackageEnumerateCommands(List<Ptr<XmlElement>>& packages, Dictionary<WString, Ptr<GuiToolstripCommand>>& commands)
		{
		}
	}
}