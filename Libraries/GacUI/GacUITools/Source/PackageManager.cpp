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
					>>Select(LAMBDA([](Ptr<GuiResourceItem> item)
						{
							return item->GetContent();
						}))
					>>FindType<Object, XmlDocument>()
					>>Where(LAMBDA([](Ptr<XmlDocument> document)
						{
							return document->rootElement->name.value==L"Package";
						}))
					>>Select(LAMBDA([](Ptr<XmlDocument> document)
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