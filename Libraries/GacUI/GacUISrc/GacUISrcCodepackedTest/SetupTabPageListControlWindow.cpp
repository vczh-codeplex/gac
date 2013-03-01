#include "..\..\Public\Source\GacUI.h"

extern void SetupListControlWindow(GuiControlHost* controlHost, GuiControl* container);
extern void SetupListDirectionWindow(GuiControlHost* controlHost, GuiControl* container);
extern void SetupListviewWindow(GuiControlHost* controlHost, GuiControl* container);
extern void SetupTreeviewWindow(GuiControlHost* controlHost, GuiControl* container);

void SetupTabPageListControlWindow(GuiControlHost* controlHost, GuiControl* container)
{
	container->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	GuiTab* tab=g::NewTab();
	tab->GetBoundsComposition()->SetAlignmentToParent(Margin(6, 6, 6, 6));
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"Text List");
		SetupListControlWindow(controlHost, page->GetContainer());
	}
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"Layout Direction");
		SetupListDirectionWindow(controlHost, page->GetContainer());
	}
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"List View");
		SetupListviewWindow(controlHost, page->GetContainer());
	}
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"Tree View");
		SetupTreeviewWindow(controlHost, page->GetContainer());
	}
	container->GetContainerComposition()->AddChild(tab->GetBoundsComposition());
}