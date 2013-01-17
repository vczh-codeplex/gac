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

		void EnumerateCommands(Ptr<GuiResource> resource, List<Ptr<XmlElement>>& packages, Dictionary<WString, Ptr<GuiToolstripCommand>>& commands)
		{
			FOREACH(Ptr<XmlElement>, package, packages)
			{
				auto commandsElement=XmlGetElement(package, L"commands");
				if(commandsElement)
				{
					FOREACH(Ptr<XmlElement>, commandElement,
						(commandsElement->subNodes
							>>FindType<XmlNode, XmlElement>()
							>>Where(LAMBDA([](Ptr<XmlElement> e){return e->name.value==L"command";}))
							))
					{
						if(auto name=XmlGetAttribute(commandElement, L"name"))
						if(!commands.Keys().Contains(name->value.value))
						{
							Ptr<GuiToolstripCommand> command=new GuiToolstripCommand();
							if(auto text=XmlGetElement(commandElement, L"text"))
							{
								command->SetText(XmlGetValue(text));
							}
							if(auto image=XmlGetElement(commandElement, L"image"))
							{
								command->SetImage(resource->GetValueByPath(XmlGetValue(image)).Cast<GuiImageData>());
							}
							commands.Add(name->value.value, command);
						}
					}
				}
			}
		}
	}
}