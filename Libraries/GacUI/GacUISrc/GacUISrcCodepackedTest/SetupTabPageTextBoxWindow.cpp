#include "..\..\Public\Source\GacUIIncludes.h"

extern void SetupTextBoxWindow(GuiControlHost* controlHost, GuiControl* container);
extern void SetupSolidLabelElementLayoutWindow(GuiControlHost* controlHost, GuiControl* container);
extern void SetupDocumentElementLayoutWindow(GuiControlHost* controlHost, GuiControl* container, const WString& filename);

void SetupTabPageTextBoxlWindow(GuiControlHost* controlHost, GuiControl* container)
{
	container->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	GuiTab* tab=g::NewTab();
	tab->GetBoundsComposition()->SetAlignmentToParent(Margin(6, 6, 6, 6));
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"Colorized TextBox");
		SetupTextBoxWindow(controlHost, page->GetContainer());
	}
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"Solid Label Layout");
		SetupSolidLabelElementLayoutWindow(controlHost, page->GetContainer());
	}
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"document.xml");
		SetupDocumentElementLayoutWindow(controlHost, page->GetContainer(), L"Resources\\document.xml");
	}
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"document2.xml");
		SetupDocumentElementLayoutWindow(controlHost, page->GetContainer(), L"Resources\\document2.xml");
	}
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"XmlResource.xml");
	}
	container->GetContainerComposition()->AddChild(tab->GetBoundsComposition());
}