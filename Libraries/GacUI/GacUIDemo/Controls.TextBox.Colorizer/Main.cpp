#include "..\..\Public\Source\GacUIIncludes.h"
#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsDirect2DRenderer();
}

class IniColorizer : public GuiTextBoxColorizerBase
{
	typedef collections::Array<text::ColorEntry> ColorArray;
private:
	static const int	NORMAL_COLOR=0;
	static const int	SECTION_COLOR=1;
	static const int	ATTRIBUTE_COLOR=2;
	static const int	OPERATOR_COLOR=3;
	static const int	COMMENT_COLOR=4;

	ColorArray			colors;
public:
	IniColorizer()
	{
		text::ColorEntry entry=win7::Win7GetTextBoxTextColor();
		colors.Resize(5);
		
		// text color
		colors[NORMAL_COLOR]=entry;

		// section color
		entry.normal.text=Color(163, 21, 21);
		colors[SECTION_COLOR]=entry;

		// attribute color
		entry.normal.text=Color(255, 0, 0);
		colors[ATTRIBUTE_COLOR]=entry;

		// operator color
		entry.normal.text=Color(0, 0, 255);
		colors[OPERATOR_COLOR]=entry;

		// operator color
		entry.normal.text=Color(24, 128, 24);
		colors[COMMENT_COLOR]=entry;
	}

	int GetStartState()
	{
		return 0;
	}

	int ColorizeLineWithCRLF(const wchar_t* text, unsigned __int32* colors, int length, int startState)
	{
		if(length>0)
		{
			if(text[0]==L';')
			{
				for(int i=0;i<length;i++)
				{
					colors[i]=COMMENT_COLOR;
				}
			}
			else if(text[0]==L'[')
			{
				for(int i=0;i<length;i++)
				{
					colors[i]=(text[i]==L'[' || text[i]==L']')?OPERATOR_COLOR:SECTION_COLOR;
				}
			}
			else
			{
				bool afterAssign=false;
				for(int i=0;i<length;i++)
				{
					if(text[i]==L'=' && !afterAssign)
					{
						afterAssign=true;
						colors[i]=OPERATOR_COLOR;
					}
					else
					{
						colors[i]=afterAssign?NORMAL_COLOR:ATTRIBUTE_COLOR;
					}
				}
			}
		}
		return 0;
	}

	const ColorArray& GetColors()
	{
		return colors;
	}
};

class XmlColorizer : public GuiTextBoxColorizerBase
{
};

class CppColorizer : public GuiTextBoxColorizerBase
{
};

class TextBoxColorizerWindow : public GuiWindow
{
private:
	GuiMultilineTextBox*				textBox;
	GuiComboBoxListControl*				comboSelector;

	void comboSelector_SelectedIndexChanged(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		switch(comboSelector->GetSelectedIndex())
		{
		case 0:
			textBox->SetColorizer(new IniColorizer);
			textBox->SetText(
				L";This is a comment\r\n"
				L"[Section1]\r\n"
				L"Name=John Smith\r\n"
				L"ID=008\r\n"
				L"\r\n"
				L"[Section2]\r\n"
				L"Name=Kyon\r\n"
				L"ID=009\r\n"
				);
			break;
		default:
			textBox->SetColorizer(0);
		}
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
			listContent->GetItems().Add(L"INI colorizer");
			listContent->GetItems().Add(L"XML colorizer");
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