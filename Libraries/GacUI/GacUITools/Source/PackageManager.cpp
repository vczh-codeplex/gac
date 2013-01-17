#include "PackageManager.h"

namespace vl
{
	namespace gactools
	{

/***********************************************************************
Helper Functions
***********************************************************************/

		void GetMenuButtonAttributes(Ptr<GuiResource> resource, Ptr<XmlElement> parent, WString& text, Ptr<GuiImageData>& image)
		{
			if(auto textElement=XmlGetElement(parent, L"Text"))
			{
				text=XmlGetValue(textElement);
			}
			if(auto imageElement=XmlGetElement(parent, L"Image"))
			if(auto resourceElement=XmlGetAttribute(imageElement, L"resource"))
			{
				image=resource->GetValueByPath(resourceElement->value.value).Cast<GuiImageData>();
			}
		}

		WString TranslateResourceText(Ptr<GuiResource> resource, WString text, Regex& parameterRegex)
		{
			WString result;
			List<Ptr<RegexMatch>> matches;
			parameterRegex.Cut(text, false, matches);
			FOREACH(Ptr<RegexMatch>, match, matches)
			{
				if(match->Success())
				{
					WString name=match->Groups()[L"name"].Get(0).Value();
					WString value=match->Groups()[L"value"].Get(0).Value();
					if(name==L"resource")
					{
						auto textResource=resource->GetValueByPath(value).Cast<ObjectBox<WString>>();
						if(textResource)
						{
							result+=textResource->Unbox();
						}
					}
				}
				else
				{
					result+=match->Result().Value();
				}
			}
			return result;
		}

/***********************************************************************
EnumeratePackages
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

/***********************************************************************
EnumerateCommands
***********************************************************************/

		void EnumerateCommands(Ptr<GuiResource> resource, List<Ptr<XmlElement>>& packages, Dictionary<WString, Ptr<GuiToolstripCommand>>& commands)
		{
			FOREACH(Ptr<XmlElement>, package, packages)
			{
				if(auto commandsElement=XmlGetElement(package, L"Commands"))
				{
					FOREACH(Ptr<XmlElement>, commandElement,
						(commandsElement->subNodes
							>>FindType<XmlNode, XmlElement>()
							>>Where(LAMBDA([](Ptr<XmlElement> e){return e->name.value==L"Command";}))
							))
					{
						if(auto name=XmlGetAttribute(commandElement, L"id"))
						if(!commands.Keys().Contains(name->value.value))
						{
							Ptr<GuiToolstripCommand> command=new GuiToolstripCommand();
							WString text=L"<Empty-Command-Name>";
							Ptr<GuiImageData> image;
							GetMenuButtonAttributes(resource, commandElement, text, image);
							command->SetText(text);
							command->SetImage(image);
							commands.Add(name->value.value, command);
						}
					}
				}
			}
		}

/***********************************************************************
EnumerateMenuDefinitions
***********************************************************************/

		void EnumerateMenuDefinitions(Ptr<GuiResource> resource, List<Ptr<XmlElement>>& packages, Group<WString, ProprityMenuGroup>& existingMenuGroups)
		{
			FOREACH(Ptr<XmlElement>, package, packages)
			{
				// find all single menu items
				if(auto menusElement=XmlGetElement(package, L"Menus"))
				{
					List<Ptr<XmlElement>> menuElements;
					XmlGetElements(menusElement, L"Menu", menuElements);
					FOREACH(Ptr<XmlElement>, menuElement, menuElements)
					{
						if(auto id=XmlGetAttribute(menuElement, L"id"))
						if(auto parent=XmlGetElement(menuElement, L"Parent"))
						if(auto parentId=XmlGetAttribute(parent, L"id"))
						if(auto priority=XmlGetAttribute(parent, L"priority"))
						{
							Ptr<PackageXmlMenuItem> menuItem=new PackageXmlMenuItem;
							menuItem->id=id->value.value;
							menuItem->definition=menuElement;

							Ptr<PackageXmlMenuGroup> menuGroup=new PackageXmlMenuGroup;
							menuGroup->hasSeparator=false;
							menuGroup->menuItems.Add(menuItem);

							ProprityMenuGroup pair(wtoi(priority->value.value), menuGroup);
							existingMenuGroups.Add(parentId->value.value, pair);
						}
					}
				}

				// find all menu groups
				if(auto groupsElement=XmlGetElement(package, L"Groups"))
				{
					List<Ptr<XmlElement>> panelElements;
					XmlGetElements(groupsElement, L"Group", panelElements);
					FOREACH(Ptr<XmlElement>, groupElement, panelElements)
					{
						Ptr<PackageXmlMenuGroup> menuGroup=new PackageXmlMenuGroup;
						menuGroup->hasSeparator=true;

						List<Ptr<XmlElement>> parentElements;
						XmlGetElements(groupElement, L"Parent", parentElements);
						FOREACH(Ptr<XmlElement>, parentElement, parentElements)
						{
							if(auto parentId=XmlGetAttribute(parentElement, L"id"))
							if(auto priority=XmlGetAttribute(parentElement, L"priority"))
							{
								ProprityMenuGroup pair(wtoi(priority->value.value), menuGroup);
								existingMenuGroups.Add(parentId->value.value, pair);
							}
						}

						List<Ptr<XmlElement>> menuElements;
						XmlGetElements(groupElement, L"Menu", menuElements);
						FOREACH(Ptr<XmlElement>, menuElement, menuElements)
						{
							if(auto id=XmlGetAttribute(menuElement, L"id"))
							{
								Ptr<PackageXmlMenuItem> menuItem=new PackageXmlMenuItem;
								menuItem->id=id->value.value;
								menuItem->definition=menuElement;
								menuGroup->menuItems.Add(menuItem);
							}
						}
					}
				}
			}
		}

/***********************************************************************
EnumerateMenuDefinitions
***********************************************************************/

		void BuildToolstripObject(Ptr<GuiResource> resource, List<Ptr<XmlElement>>& packages, GuiToolstripBuilder* rootBuilder, const WString& containerName, Dictionary<WString, Ptr<GuiToolstripCommand>>& commands, Group<WString, ProprityMenuGroup>& existingMenuGroups)
		{
			vint processingMenuItemIndex=-1;
			WString currentParentId=containerName;
			GuiToolstripButton* currentMenuButton=0;
			GuiToolstripBuilder* currentBuilder=0;
			Regex parameterRegex(L"/{(<name>/w+):(<value>[^{}]*)/}");
			List<Ptr<PackageXmlMenuItem>> processingMenuItems;

			FOREACH(Ptr<GuiToolstripCommand>, command, commands.Values())
			{
				command->SetText(TranslateResourceText(resource, command->GetText(), parameterRegex));
			}

			while(true)
			{
				vint index=existingMenuGroups.Keys().IndexOf(currentParentId);
				if(index!=-1)
				{
					List<Ptr<PackageXmlMenuGroup>> orderedGroups;
					CopyFrom(orderedGroups,
						(existingMenuGroups.GetByIndex(index)
							>>Where(LAMBDA([](ProprityMenuGroup p)
							{
								return p.value->menuItems.Count()>0;
							}))
							>>OrderBy(LAMBDA([](ProprityMenuGroup p1, ProprityMenuGroup p2)
							{
								return p1.key-p2.key;
							}))
							>>Select(LAMBDA([](ProprityMenuGroup p)
							{
								return p.value;
							}))
						));

					if(orderedGroups.Count()>0)
					{
						GuiToolstripBuilder* currentBuilder=0;
						if(currentMenuButton)
						{
							currentMenuButton->CreateToolstripSubMenu();
							GuiToolstripMenu* menu=currentMenuButton->GetToolstripSubMenu();
							currentBuilder=menu->GetBuilder();
						}
						else
						{
							currentBuilder=rootBuilder;
						}

						bool needSeparator=false;
						bool firstGroup=true;
						FOREACH(Ptr<PackageXmlMenuGroup>, group, orderedGroups)
						{
							if(!firstGroup && (needSeparator || group->hasSeparator))
							{
								currentBuilder->Splitter();
							}
							needSeparator=group->hasSeparator;
							firstGroup=false;

							FOREACH(Ptr<PackageXmlMenuItem>, item, group->menuItems)
							{
								if(auto commandAttribute=XmlGetAttribute(item->definition, L"command"))
								{
									vint index=commands.Keys().IndexOf(commandAttribute->value.value);
									if(index==-1)
									{
										currentBuilder->Button(0, L"<Unknown-Command-Id>:"+commandAttribute->value.value, &item->menuButton);
									}
									else
									{
										Ptr<GuiToolstripCommand> command=commands.Values().Get(index);
										currentBuilder->Button(command.Obj(), &item->menuButton);
									}
								}
								else
								{
									WString text=L"<Empty-Menu-Name>";
									Ptr<GuiImageData> image;
									GetMenuButtonAttributes(resource, item->definition, text, image);
									text=TranslateResourceText(resource, text, parameterRegex);
									currentBuilder->Button(image, text, &item->menuButton);
								}
								processingMenuItems.Add(item);
							}
						}
					}
				}

				processingMenuItemIndex++;
				if(processingMenuItemIndex>=processingMenuItems.Count())
				{
					break;
				}
				currentParentId=processingMenuItems[processingMenuItemIndex]->id;
				currentMenuButton=processingMenuItems[processingMenuItemIndex]->menuButton;
			}
		}

		void BuildMenu(Ptr<GuiResource> resource, List<Ptr<XmlElement>>& packages, GuiToolstripMenuBar* menu, const WString& containerName, Dictionary<WString, Ptr<GuiToolstripCommand>>& commands, Group<WString, ProprityMenuGroup>& existingMenuGroups)
		{
			BuildToolstripObject(resource, packages, menu->GetBuilder(), containerName, commands, existingMenuGroups);
		}

		void BuildToolbar(Ptr<GuiResource> resource, List<Ptr<XmlElement>>& packages, GuiToolstripToolbar* toolbar, const WString& containerName, Dictionary<WString, Ptr<GuiToolstripCommand>>& commands, Group<WString, ProprityMenuGroup>& existingMenuGroups)
		{
			BuildToolstripObject(resource, packages, toolbar->GetBuilder(), containerName, commands, existingMenuGroups);
		}
	}
}