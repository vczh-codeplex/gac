#include "..\..\Public\Source\GacUIIncludes.h"

using namespace collections;

void CreateSubMenuInternal(GuiControlHost* controlHost, GuiToolstripButton* parentMenuItem, int count, const wchar_t** menuText, const wchar_t** menuImage, const wchar_t** menuShortcut)
{
	parentMenuItem->CreateToolstripSubMenu();
	GuiToolstripMenu* parentMenu=parentMenuItem->GetToolstripSubMenu();
	INativeImageService* imageService=GetCurrentController()->ImageService();
	for(int i=0;i<count;i++)
	{
		GuiControl* menuItem=0;
		if(wcscmp(menuText[i], L"-")==0)
		{
			GuiControl* splitter=g::NewMenuSplitter();
			menuItem=splitter;
		}
		else
		{
			GuiToolstripButton* button=g::NewMenuItemButton();
			button->SetText(menuText[i]);
			button->SetEnabled(i<6);
			if(menuImage && menuImage[i])
			{
				Ptr<INativeImage> image=imageService->CreateImageFromFile(WString(L"Resources\\")+menuImage[i]);
				button->SetImage(new GuiImageData(image, 0));
			}
			if(menuShortcut && menuShortcut[i])
			{
				button->SetShortcutText(menuShortcut[i]);
			}
			menuItem=button;
		}
		parentMenu->GetToolstripItems().Add(menuItem);
	}
}

template<int count>
void CreateSubMenu(GuiControlHost* controlHost, GuiToolstripButton* parentMenuItem, const wchar_t* (&menuText)[count])
{
	CreateSubMenuInternal(controlHost, parentMenuItem, count, menuText, 0, 0);
}

template<int count>
void CreateSubMenu(GuiControlHost* controlHost, GuiToolstripButton* parentMenuItem, const wchar_t* (&menuText)[count], const wchar_t* (&menuImage)[count], const wchar_t* (&menuShortcut)[count])
{
	CreateSubMenuInternal(controlHost, parentMenuItem, count, menuText, menuImage, menuShortcut);
}

template<int count>
void CreateToolbar(GuiControlHost* controlHost, Ptr<INativeImage> (&imageButtons)[count], GuiToolstripToolbar* toolbar)
{
	const wchar_t* fileMenuText[]={L"New", L"Open", L"Save", L"Save As...", L"-", L"Page Setting...", L"Print...", L"-", L"Exit"};
	const wchar_t* fileMenuImage[]={L"_New.png", L"_Open.png", L"_Save.png", L"_SaveAs.png", 0, 0, L"_Print.png", 0, 0};
	const wchar_t* fileMenuShortcut[]={L"Ctrl+N", L"Ctrl+O", L"Ctrl+S", 0, 0, 0, 0, 0, 0};
	const wchar_t* editMenuText[]={L"Undo", L"Redo", L"-", L"Short Menu Text", L"Very Very Long Long Menu Test", L"-", L"Cut", L"Copy", L"Paste", L"Delete", L"-", L"Find...", L"Find Next", L"Replace...", L"Go to...", L"-", L"Select All", L"Time/Date"};
	const wchar_t* editMenuImage[]={L"_Undo.png", L"_Redo.png", 0, 0, 0, 0, L"_Cut.png", L"_Copy.png", L"_Paste.png", L"_Delete.png", 0, 0, 0, 0, 0, 0, 0, 0};
	const wchar_t* editMenuShortcut[]={L"Ctrl+Z", L"Ctrl+Y", 0, L"[Very Very long Long Shortcut Text]", L"[Short Shortcut Text]", 0, L"Ctrl+X", L"Ctrl+C", L"Ctrl+V", L"Del", 0, L"Ctrl+F", L"F3", L"Ctrl+H", 0, 0, L"Ctrl+A", 0};

	for(int i=0;i<sizeof(imageButtons)/sizeof(*imageButtons);i++)
	{
		if(imageButtons[i])
		{
			GuiToolstripButton* button=0;
			switch(i)
			{
			case 0:
				{
					button=g::NewToolbarDropdownButton();
					CreateSubMenu(controlHost, button, fileMenuText, fileMenuImage, fileMenuShortcut);
				}
				break;
			case 1:
				{
					button=g::NewToolbarSplitButton();
					CreateSubMenu(controlHost, button, editMenuText, editMenuImage, editMenuShortcut);
				}
				break;
			default:
				{
					button=g::NewToolbarButton();
				}
			}
			button->SetImage(new GuiImageData(imageButtons[i], 0));
			toolbar->GetToolstripItems().Add(button);
		}
		else
		{
			GuiControl* splitter=g::NewToolbarSplitter();
			toolbar->GetToolstripItems().Add(splitter);
		}
	}
}

void SetupTabPageToolstripWindow(GuiControlHost* controlHost, GuiControl* container)
{
	container->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	INativeImageService* imageService=GetCurrentController()->ImageService();

	const wchar_t* fileMenuText[]={L"New", L"Open", L"Save", L"Save As...", L"-", L"Page Setting...", L"Print...", L"-", L"Exit"};
	const wchar_t* fileMenuImage[]={L"_New.png", L"_Open.png", L"_Save.png", L"_SaveAs.png", 0, 0, L"_Print.png", 0, 0};
	const wchar_t* fileMenuShortcut[]={L"Ctrl+N", L"Ctrl+O", L"Ctrl+S", 0, 0, 0, 0, 0, 0};
	const wchar_t* fileNewMenuText[]={L"Project...", L"Web Site...", L"Team Project...", L"File...", L"Project From Existing Code..."};
	const wchar_t* fileOpenMenuText[]={L"Project/Solution...", L"Web Site...", L"Team Project...", L"File...", L"Convert..."};
	const wchar_t* filePrintMenuText[]={L"Print", L"Don't Print"};
	const wchar_t* editMenuText[]={L"Undo", L"Redo", L"-", L"Short Menu Text", L"Very Very Long Long Menu Test", L"-", L"Cut", L"Copy", L"Paste", L"Delete", L"-", L"Find...", L"Find Next", L"Replace...", L"Go to...", L"-", L"Select All", L"Time/Date"};
	const wchar_t* editMenuImage[]={L"_Undo.png", L"_Redo.png", 0, 0, 0, 0, L"_Cut.png", L"_Copy.png", L"_Paste.png", L"_Delete.png", 0, 0, 0, 0, 0, 0, 0, 0};
	const wchar_t* editMenuShortcut[]={L"Ctrl+Z", L"Ctrl+Y", 0, L"[Very Very long Long Shortcut Text]", L"[Short Shortcut Text]", 0, L"Ctrl+X", L"Ctrl+C", L"Ctrl+V", L"Del", 0, L"Ctrl+F", L"F3", L"Ctrl+H", 0, 0, L"Ctrl+A", 0};
	const wchar_t* formatMenuText[]={L"Wrap Text", L"Font..."};
	const wchar_t* viewMenuText[]={L"Status Bar"};
	const wchar_t* helpMenuText[]={L"View Help", L"About Notepad"};

	GuiToolstripMenuBar* menuBar=g::NewMenuBar();
	{
		const wchar_t* menuText[]={L"File", L"Edit", L"Format", L"View", L"Help"};
		for(int i=0;i<sizeof(menuText)/sizeof(*menuText);i++)
		{
			GuiToolstripButton* button=g::NewMenuBarButton();
			button->SetText(menuText[i]);
			menuBar->GetToolstripItems().Add(button);
			
			switch(i)
			{
			case 0:
				CreateSubMenu(controlHost, button, fileMenuText, fileMenuImage, fileMenuShortcut);
				CreateSubMenu(controlHost, dynamic_cast<GuiToolstripButton*>(button->GetToolstripSubMenu()->GetToolstripItems()[0]), fileNewMenuText);
				CreateSubMenu(controlHost, dynamic_cast<GuiToolstripButton*>(button->GetToolstripSubMenu()->GetToolstripItems()[1]), fileOpenMenuText);
				CreateSubMenu(controlHost, dynamic_cast<GuiToolstripButton*>(button->GetToolstripSubMenu()->GetToolstripItems()[6]), filePrintMenuText);
				break;
			case 1:
				CreateSubMenu(controlHost, button, editMenuText, editMenuImage, editMenuShortcut);
				break;
			case 2:
				CreateSubMenu(controlHost, button, formatMenuText);
				break;
			case 3:
				CreateSubMenu(controlHost, button, viewMenuText);
				break;
			case 4:
				CreateSubMenu(controlHost, button, helpMenuText);
				break;
			}
		}
	}

	GuiToolstripToolbar* smallToolbar=g::NewToolbar();
	{
		Ptr<INativeImage> imageButtons[]=
		{
			imageService->CreateImageFromFile(L"Resources\\SmallDoc.png"),
			imageService->CreateImageFromFile(L"Resources\\SmallDoc.png"),
			0,
			imageService->CreateImageFromFile(L"Resources\\_New.png"),
			imageService->CreateImageFromFile(L"Resources\\_Open.png"),
			imageService->CreateImageFromFile(L"Resources\\_Save.png"),
			imageService->CreateImageFromFile(L"Resources\\_SaveAs.png"),
			0,
			imageService->CreateImageFromFile(L"Resources\\_Print.png"),
			0,
			imageService->CreateImageFromFile(L"Resources\\_Undo.png"),
			imageService->CreateImageFromFile(L"Resources\\_Redo.png"),
			0,
			imageService->CreateImageFromFile(L"Resources\\_Cut.png"),
			imageService->CreateImageFromFile(L"Resources\\_Copy.png"),
			imageService->CreateImageFromFile(L"Resources\\_Paste.png"),
			imageService->CreateImageFromFile(L"Resources\\_Delete.png"),
		};
		CreateToolbar(controlHost, imageButtons, smallToolbar);
	}
	
	GuiToolstripToolbar* bigToolbar=g::NewToolbar();
	{
		Ptr<INativeImage> imageButtons[]=
		{
			imageService->CreateImageFromFile(L"Resources\\BigDoc.png"),
			imageService->CreateImageFromFile(L"Resources\\BigDoc.png"),
			0,
			imageService->CreateImageFromFile(L"Resources\\New.png"),
			imageService->CreateImageFromFile(L"Resources\\Open.png"),
			imageService->CreateImageFromFile(L"Resources\\Save.png"),
		};
		CreateToolbar(controlHost, imageButtons, bigToolbar);
	}

	GuiStackComposition* windowStack=new GuiStackComposition;
	windowStack->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	windowStack->SetDirection(GuiStackComposition::Vertical);
	windowStack->SetAlignmentToParent(Margin(0, 0, 0, 0));
	{
		menuBar->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		GuiStackItemComposition* item=new GuiStackItemComposition;
		item->AddChild(menuBar->GetBoundsComposition());
		windowStack->AddChild(item);
	}
	{
		smallToolbar->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		GuiStackItemComposition* item=new GuiStackItemComposition;
		item->AddChild(smallToolbar->GetBoundsComposition());
		windowStack->AddChild(item);
	}
	{
		bigToolbar->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		GuiStackItemComposition* item=new GuiStackItemComposition;
		item->AddChild(bigToolbar->GetBoundsComposition());
		windowStack->AddChild(item);
	}
	{
		const wchar_t* texts[]=
		{
			L"When using ReadFileEx you should check GetLastError even when the function returns \"success\" to check for conditions that are \"successes\" but have some outcome you might want to know about. For example, a buffer overflow when calling ReadFileEx will return TRUE, but GetLastError will report the overflow with ERROR_MORE_DATA. If the function call is successful and there are no warning conditions, GetLastError will return ERROR_SUCCESS.",
			L"The ReadFileEx function may fail if there are too many outstanding asynchronous I/O requests. In the event of such a failure, GetLastError can return ERROR_INVALID_USER_BUFFER or ERROR_NOT_ENOUGH_MEMORY.",
			L"To cancel all pending asynchronous I/O operations, use either: ",
			L"CancelIo¡ªthis function only cancels operations issued by the calling thread for the specified file handle.",
			L"CancelIoEx¡ªthis function cancels all operations issued by the threads for the specified file handle.",
		};
		const int rowCount=sizeof(texts)/sizeof(*texts);

		GuiTableComposition* table=new GuiTableComposition;
		table->SetAlignmentToParent(Margin(0, 0, 0, 0));
		table->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
		table->SetRowsAndColumns(1, 2);
		table->SetRowOption(0, GuiCellOption::MinSizeOption());
		table->SetColumnOption(0, GuiCellOption::PercentageOption(0.5));
		table->SetColumnOption(1, GuiCellOption::PercentageOption(0.5));
		{
			GuiStackComposition* sub=new GuiStackComposition;
			sub->SetDirection(GuiStackComposition::Vertical);
			sub->SetPadding(10);
			sub->SetAlignmentToParent(Margin(0, 0, 0, 0));
			sub->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			for(int i=0;i<rowCount;i++)
			{
				GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
				element->SetWrapLine(true);
				element->SetText(texts[i]);
				element->SetFont(container->GetFont());

				GuiStackItemComposition* item=new GuiStackItemComposition;
				item->SetOwnedElement(element);
				sub->AddChild(item);
			}

			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(0, 0, 1, 1);
			cell->AddChild(sub);
		}
		{
			GuiTableComposition* sub=new GuiTableComposition;
			sub->SetCellPadding(10);
			sub->SetAlignmentToParent(Margin(0, 0, 0, 0));
			sub->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			sub->SetRowsAndColumns(rowCount, 1);
			sub->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
			for(int i=0;i<rowCount;i++)
			{
				sub->SetRowOption(i, GuiCellOption::MinSizeOption());
			}
			for(int i=0;i<rowCount;i++)
			{
				GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
				element->SetWrapLine(true);
				element->SetText(texts[i]);
				element->SetFont(container->GetFont());

				GuiCellComposition* cell=new GuiCellComposition;
				sub->AddChild(cell);
				cell->SetSite(i, 0, 1, 1);
				cell->SetOwnedElement(element);
			}

			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(0, 1, 1, 1);
			cell->AddChild(sub);
		}
		GuiStackItemComposition* item=new GuiStackItemComposition;
		item->AddChild(table);
		windowStack->AddChild(item);
	}
	container->GetContainerComposition()->AddChild(windowStack);
}