#include "..\..\Public\Source\GacUIIncludes.h"

extern void SetupBasicWindow(GuiControlHost* controlHost, GuiControl* container);
extern void SetupTextBoxWindow(GuiControlHost* controlHost, GuiControl* container);
extern void SetupTabPageListControlWindow(GuiControlHost* controlHost, GuiControl* container);
extern void SetupTabPageToolstripWindow(GuiControlHost* controlHost, GuiControl* container);
extern void SetupDialogWindow(GuiControlHost* controlHost, GuiControl* container);
extern void SetupRibbonWindow(GuiControlHost* controlHost, GuiControl* container);

void SetupTabPageWindow(GuiControlHost* controlHost, GuiControl* container)
{
	container->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	GuiTab* tab=g::NewTab();
	tab->GetBoundsComposition()->SetAlignmentToParent(Margin(6, 6, 6, 6));
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"Basic");
		SetupBasicWindow(controlHost, page->GetContainer());
	}
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"Text Box");
		SetupTextBoxWindow(controlHost, page->GetContainer());
	}
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"List Control");
		SetupTabPageListControlWindow(controlHost, page->GetContainer());
	}
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"Toolstrip");
		SetupTabPageToolstripWindow(controlHost, page->GetContainer());
	}
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"Dialogs");
		SetupDialogWindow(controlHost, page->GetContainer());
	}
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"Ribbon (not completed)");
		SetupRibbonWindow(controlHost, page->GetContainer());
	}
	container->GetContainerComposition()->AddChild(tab->GetBoundsComposition());
	tab->MovePage(tab->GetPages()[3], 2);
}