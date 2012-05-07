#include "..\..\Public\Source\GacUIIncludes.h"
#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsDirect2DRenderer();
}

class TextBoxColorizerWindow : public GuiWindow
{
private:
	GuiMultilineTextBox*				textBox;
	GuiComboBoxListControl*				comboSelector;

	void comboSelector_SelectedIndexChanged(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
	}
public:
	TextBoxColorizerWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		this->SetText(L"Controls.TextBox.Colorizer");
		this->GetContainerComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);

		GuiTableComposition* table=new GuiTableComposition;
		table->SetAlignmentToParent(Margin(0, 0, 0, 0));
		table->SetCellPadding(2);
		table->SetRowsAndColumns(2, 3);
		table->SetRowOption(0, GuiCellOption::MinSizeOption());
		table->SetRowOption(1, GuiCellOption::PercentageOption(1.0));
		table->SetColumnOption(0, GuiCellOption::MinSizeOption());
		table->SetColumnOption(1, GuiCellOption::MinSizeOption());
		table->SetColumnOption(2, GuiCellOption::PercentageOption(1.0));
		this->GetContainerComposition()->AddChild(table);
		
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(0, 0, 1, 1);

			GuiLabel* label=g::NewLabel();
			label->SetText(L"Select a colorizer: ");
			label->GetBoundsComposition()->SetAlignmentToParent(Margin(0, -1, 0, 0));
			cell->AddChild(label->GetBoundsComposition());
		}
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(0, 1, 1, 1);
			// combo box doesn't have a minimum width, so set it to 150.
			cell->SetPreferredMinSize(Size(150, 0));

			// create a text list control.
			GuiTextList* listContent=g::NewTextList();
			// insert text items.
			listContent->GetItems().Add(L"XML colorizer");
			listContent->GetItems().Add(L"C# colorizer");
			listContent->GetItems().Add(L"C++ colorizer");

			// use the text list control to create a combo box.
			// items in the text list control will be the data source.
			// the text list control will be displayed in the combo box dropdown.
			comboSelector=g::NewComboBox(listContent);
			comboSelector->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			comboSelector->SelectedIndexChanged.AttachMethod(this, &TextBoxColorizerWindow::comboSelector_SelectedIndexChanged);
			cell->AddChild(comboSelector->GetBoundsComposition());
		}
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(1, 0, 1, 3);

			textBox=g::NewMultilineTextBox();
			textBox->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			cell->AddChild(textBox->GetBoundsComposition());
		}

		comboSelector->SetSelectedIndex(0);

		// set the preferred minimum client size
		this->GetBoundsComposition()->SetPreferredMinSize(Size(640, 480));
		// call this to calculate the size immediately if any indirect content in the table changes
		// so that the window can calcaulte its correct size before calling the MoveToScreenCenter()
		this->ForceCalculateSizeImmediately();
		// move to the screen center
		this->MoveToScreenCenter();
	}

	~TextBoxColorizerWindow()
	{
	}
};

void GuiMain()
{
	GuiWindow* window=new TextBoxColorizerWindow();
	GetApplication()->Run(window);
	delete window;
}