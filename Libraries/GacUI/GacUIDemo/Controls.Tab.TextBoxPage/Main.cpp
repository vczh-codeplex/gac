#include "..\..\Public\Source\GacUIIncludes.h"
#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsDirect2DRenderer();
}

class TextBoxPage : public GuiTabPage
{
private:
	static int pageCounter;

	GuiButton*				closeButton;
	GuiMultilineTextBox*	textBox;

	void OnPageContainerReady(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		GuiTableComposition* table=new GuiTableComposition;
		table->SetRowsAndColumns(2, 1);
		table->SetRowOption(0, GuiCellOption::MinSizeOption());
		table->SetRowOption(1, GuiCellOption::PercentageOption(1.0));
		table->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
		table->SetAlignmentToParent(Margin(0, 0, 0, 0));
		table->SetCellPadding(2);

		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(0, 0, 1, 1);
			
			closeButton=g::NewButton();
			closeButton->SetText(L"Close Me!");
			cell->AddChild(closeButton->GetBoundsComposition());
		}
		
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(1, 0, 1, 1);
			
			textBox=g::NewMultilineTextBox();
			textBox->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			cell->AddChild(textBox->GetBoundsComposition());
		}

		this->GetContainer()->GetContainerComposition()->AddChild(table);
	}

public:
	TextBoxPage()
		:closeButton(0)
		,textBox(0)
	{
		PageContainerReady.AttachMethod(this, &TextBoxPage::OnPageContainerReady);
		this->SetText(L"Page "+itow(++pageCounter));
	}

	~TextBoxPage()
	{
	}
};

int TextBoxPage::pageCounter=0;

class TextBoxPageWindow : public GuiWindow
{
private:
	GuiTab*						tabControl;
	GuiTabPage*					controlPanelPage;
public:
	TextBoxPageWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		this->SetText(L"Controls.Tab.TextBoxPage");
		this->GetBoundsComposition()->SetPreferredMinSize(Size(640, 480));

		tabControl=g::NewTab();
		tabControl->GetBoundsComposition()->SetAlignmentToParent(Margin(2, 2, 2, 2));
		this->AddChild(tabControl);

		controlPanelPage=tabControl->CreatePage();
		controlPanelPage->SetText(L"Control Panel");

		this->ForceCalculateSizeImmediately();
		this->MoveToScreenCenter();

		TextBoxPage* page=new TextBoxPage;
		tabControl->CreatePage(page);
	}

	~TextBoxPageWindow()
	{
	}
};

void GuiMain()
{
	GuiWindow* window=new TextBoxPageWindow();
	GetApplication()->Run(window);
	delete window;
}