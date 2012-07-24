#include "..\..\Public\Source\GacUIIncludes.h"

extern void SetupRawToolstripWindow(GuiControlHost* controlHost, GuiControl* container);
extern void SetupManagedToolstripWindow(GuiControlHost* controlHost, GuiControl* container);

void SetupTabPageToolstripWindow(GuiControlHost* controlHost, GuiControl* container)
{
	container->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	GuiTab* tab=new GuiTab(new win7::Win7TabStyle);
	tab->GetBoundsComposition()->SetAlignmentToParent(Margin(6, 6, 6, 6));
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"Raw Toolstrip");
		SetupRawToolstripWindow(controlHost, page->GetContainer());
	}
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"Managed Toolstrip");
		SetupManagedToolstripWindow(controlHost, page->GetContainer());
	}
	container->GetContainerComposition()->AddChild(tab->GetBoundsComposition());
}