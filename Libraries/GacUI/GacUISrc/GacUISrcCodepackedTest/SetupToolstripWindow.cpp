#include "..\..\Public\Source\GacUIIncludes.h"

class GifAnimation : public Object, public IGuiGraphicsAnimation
{
protected:
	unsigned __int64			startTime;
	GuiImageFrameElement*		element;
public:
	GifAnimation(GuiImageFrameElement* _element)
		:element(_element)
		,startTime(DateTime::LocalTime().totalMilliseconds)
	{
	}

	int GetTotalLength()
	{
		return 1;
	}

	int GetCurrentPosition()
	{
		return 0;
	}

	void Play(int currentPosition, int totalLength)
	{
		unsigned __int64 ms=DateTime::LocalTime().totalMilliseconds-startTime;
		int frameIndex=(ms/100)%element->GetImage()->GetFrameCount();
		element->SetImage(element->GetImage(), frameIndex);
	}

	void Stop()
	{
	}
};

GuiBoundsComposition* CreateImageFrame(Ptr<INativeImage> image, int frameIndex=0)
{
	GuiImageFrameElement* element=GuiImageFrameElement::Create();
	element->SetImage(image, frameIndex);

	GuiBoundsComposition* composition=new GuiBoundsComposition;
	composition->SetOwnedElement(element);
	composition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
	return composition;
}

GuiStackComposition* CreateSubMenuInternal(int count, const wchar_t** menuText, const wchar_t** menuImage)
{
	INativeImageService* imageService=GetCurrentController()->ImageService();

	GuiStackComposition* menuStack=new GuiStackComposition;
	menuStack->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	menuStack->SetDirection(GuiStackComposition::Vertical);
	menuStack->SetAlignmentToParent(Margin(0, 0, 0, 0));

	for(int i=0;i<count;i++)
	{
		GuiControl* menuItem=0;
		if(wcscmp(menuText[i], L"-")==0)
		{
			GuiControl* splitter=new GuiControl(new win7::Win7MenuSplitterStyle);
			menuItem=splitter;
		}
		else
		{
			GuiMenuButton* button=new GuiMenuButton(new win7::Win7MenuItemButtonStyle);
			button->SetText(menuText[i]);
			button->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			button->SetEnabled(i<6);
			if(menuImage && menuImage[i])
			{
				Ptr<INativeImage> image=imageService->CreateImageFromFile(WString(L"Resources\\")+menuImage[i]);
				button->SetImage(new GuiImageData(image, 0));
			}
			menuItem=button;
		}
		GuiStackItemComposition* item=new GuiStackItemComposition;
		item->AddChild(menuItem->GetBoundsComposition());
		menuStack->AddChild(item);
	}
	return menuStack;
}

template<int count>
GuiStackComposition* CreateSubMenu(const wchar_t* (&menuText)[count])
{
	return CreateSubMenuInternal(count, menuText, 0);
}

template<int count>
GuiStackComposition* CreateSubMenu(const wchar_t* (&menuText)[count], const wchar_t* (&menuImage)[count])
{
	return CreateSubMenuInternal(count, menuText, menuImage);
}

void CreateSubMenu(GuiMenu* menu, int index, GuiStackComposition* subMenu)
{
	GuiStackComposition* menuStack=dynamic_cast<GuiStackComposition*>(menu->GetContainerComposition()->Children()[0]);
	GuiMenuButton* button=dynamic_cast<GuiMenuButton*>(menuStack->GetStackItems()[index]->Children()[0]->GetAssociatedControl());
	if(!button->GetSubMenu())
	{
		button->CreateSubMenu();
		button->GetSubMenu()->GetContainerComposition()->AddChild(subMenu);
	}
}

template<int count>
void CreateToolbar(Ptr<INativeImage> (&imageButtons)[count], GuiStackComposition* toolStack)
{
	const wchar_t* fileMenuText[]={L"New", L"Open", L"Save", L"Save As...", L"-", L"Page Setting...", L"Print...", L"-", L"Exit"};
	const wchar_t* fileMenuImage[]={L"_New.png", L"_Open.png", L"_Save.png", L"_SaveAs.png", 0, 0, L"_Print.png", 0, 0};
	const wchar_t* editMenuText[]={L"Undo", L"Redo", L"-", L"Cut", L"Copy", L"Paste", L"Delete", L"-", L"Find...", L"Find Next", L"Replace...", L"Go to...", L"-", L"Select All", L"Time/Date"};
	const wchar_t* editMenuImage[]={L"_Undo.png", L"_Redo.png", 0, L"_Cut.png", L"_Copy.png", L"_Paste.png", L"_Delete.png", 0, 0, 0, 0, 0, 0, 0, 0};

	for(int i=0;i<sizeof(imageButtons)/sizeof(*imageButtons);i++)
	{
		if(imageButtons[i])
		{
			GuiMenuButton* button=0;
			switch(i)
			{
			case 0:
				{
					button=new GuiMenuButton(new win7::Win7ToolstripButtonStyle(false, win7::Win7ToolstripButtonStyle::DropdownButton));
					button->CreateSubMenu();
					button->GetSubMenu()->GetContainerComposition()->AddChild(CreateSubMenu(fileMenuText, fileMenuImage));
				}
				break;
			case 1:
				{
					button=new GuiMenuButton(new win7::Win7ToolstripButtonStyle(false, win7::Win7ToolstripButtonStyle::SplitButton));
					button->CreateSubMenu();
					button->GetSubMenu()->GetContainerComposition()->AddChild(CreateSubMenu(editMenuText, editMenuImage));
				}
				break;
			default:
				{
					button=new GuiMenuButton(new win7::Win7ToolstripButtonStyle(false, win7::Win7ToolstripButtonStyle::CommandButton));
				}
			}
			button->SetImage(new GuiImageData(imageButtons[i], 0));

			GuiStackItemComposition* item=new GuiStackItemComposition;
			item->AddChild(button->GetBoundsComposition());
			toolStack->AddChild(item);
		}
		else
		{
			GuiControl* splitter=new GuiControl(new win7::Win7ToolstripSplitterStyle);

			GuiStackItemComposition* item=new GuiStackItemComposition;
			item->AddChild(splitter->GetBoundsComposition());
			toolStack->AddChild(item);
		}
	}
}

void SetupToolstripWindow(GuiControlHost* controlHost, GuiControl* container)
{
	container->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	INativeImageService* imageService=GetCurrentController()->ImageService();

	const wchar_t* fileMenuText[]={L"New", L"Open", L"Save", L"Save As...", L"-", L"Page Setting...", L"Print...", L"-", L"Exit"};
	const wchar_t* fileMenuImage[]={L"_New.png", L"_Open.png", L"_Save.png", L"_SaveAs.png", 0, 0, L"_Print.png", 0, 0};
	const wchar_t* fileNewMenuText[]={L"Project...", L"Web Site...", L"Team Project...", L"File...", L"Project From Existing Code..."};
	const wchar_t* fileOpenMenuText[]={L"Project/Solution...", L"Web Site...", L"Team Project...", L"File...", L"Convert..."};
	const wchar_t* filePrintMenuText[]={L"Print", L"Don't Print"};
	const wchar_t* editMenuText[]={L"Undo", L"Redo", L"-", L"Cut", L"Copy", L"Paste", L"Delete", L"-", L"Find...", L"Find Next", L"Replace...", L"Go to...", L"-", L"Select All", L"Time/Date"};
	const wchar_t* editMenuImage[]={L"_Undo.png", L"_Redo.png", 0, L"_Cut.png", L"_Copy.png", L"_Paste.png", L"_Delete.png", 0, 0, 0, 0, 0, 0, 0, 0};
	const wchar_t* formatMenuText[]={L"Wrap Text", L"Font..."};
	const wchar_t* viewMenuText[]={L"Status Bar"};
	const wchar_t* helpMenuText[]={L"View Help", L"About Notepad"};

	GuiStackComposition* menuStack=new GuiStackComposition;
	{
		menuStack->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
		const wchar_t* menuText[]={L"File", L"Edit", L"Format", L"View", L"Help"};
		for(int i=0;i<sizeof(menuText)/sizeof(*menuText);i++)
		{
			GuiMenuButton* button=new GuiMenuButton(new win7::Win7MenuBarButtonStyle);
			button->SetText(menuText[i]);
			button->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));

			GuiStackItemComposition* item=new GuiStackItemComposition;
			item->AddChild(button->GetBoundsComposition());
			menuStack->AddChild(item);
			
			button->CreateSubMenu();
			switch(i)
			{
			case 0:
				button->GetSubMenu()->GetContainerComposition()->AddChild(CreateSubMenu(fileMenuText, fileMenuImage));
				CreateSubMenu(button->GetSubMenu(), 0, CreateSubMenu(fileNewMenuText));
				CreateSubMenu(button->GetSubMenu(), 1, CreateSubMenu(fileOpenMenuText));
				CreateSubMenu(button->GetSubMenu(), 6, CreateSubMenu(filePrintMenuText));
				break;
			case 1:
				button->GetSubMenu()->GetContainerComposition()->AddChild(CreateSubMenu(editMenuText, editMenuImage));
				break;
			case 2:
				button->GetSubMenu()->GetContainerComposition()->AddChild(CreateSubMenu(formatMenuText));
				break;
			case 3:
				button->GetSubMenu()->GetContainerComposition()->AddChild(CreateSubMenu(viewMenuText));
				break;
			case 4:
				button->GetSubMenu()->GetContainerComposition()->AddChild(CreateSubMenu(helpMenuText));
				break;
			}
		}
	}

	GuiStackComposition* smallToolStack=new GuiStackComposition;
	{
		smallToolStack->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
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
		CreateToolbar(imageButtons, smallToolStack);
	}

	GuiStackComposition* bigToolStack=new GuiStackComposition;
	{
		bigToolStack->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
		Ptr<INativeImage> imageButtons[]=
		{
			imageService->CreateImageFromFile(L"Resources\\BigDoc.png"),
			imageService->CreateImageFromFile(L"Resources\\BigDoc.png"),
			0,
			imageService->CreateImageFromFile(L"Resources\\New.png"),
			imageService->CreateImageFromFile(L"Resources\\Open.png"),
			imageService->CreateImageFromFile(L"Resources\\Save.png"),
		};
		CreateToolbar(imageButtons, bigToolStack);
	}
	
	GuiBoundsComposition* picGif=0;
	{
		Ptr<INativeImage> imageGif=imageService->CreateImageFromFile(L"Resources\\Gif.gif");
		picGif=CreateImageFrame(imageGif);
		Ptr<GifAnimation> animation=new GifAnimation(picGif->GetOwnedElement().Cast<GuiImageFrameElement>().Obj());
		controlHost->GetGraphicsHost()->GetAnimationManager()->AddAnimation(animation);
	}

	GuiStackComposition* windowStack=new GuiStackComposition;
	windowStack->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	windowStack->SetDirection(GuiStackComposition::Vertical);
	windowStack->SetAlignmentToParent(Margin(0, 0, 0, 0));
	{
		GuiMenuBar* menuBar=new GuiMenuBar(new win7::Win7MenuBarStyle);
		menuBar->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
		menuBar->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		menuBar->GetContainerComposition()->AddChild(menuStack);

		GuiStackItemComposition* item=new GuiStackItemComposition;
		item->AddChild(menuBar->GetBoundsComposition());
		windowStack->AddChild(item);
	}
	{
		GuiStackItemComposition* item=new GuiStackItemComposition;
		item->AddChild(smallToolStack);
		windowStack->AddChild(item);
	}
	{
		GuiStackItemComposition* item=new GuiStackItemComposition;
		item->AddChild(bigToolStack);
		windowStack->AddChild(item);
	}
	{
		GuiStackItemComposition* item=new GuiStackItemComposition;
		item->AddChild(picGif);
		windowStack->AddChild(item);
	}
	container->GetContainerComposition()->AddChild(windowStack);
}