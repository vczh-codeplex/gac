#include "..\..\Public\Source\GacUIIncludes.h"
#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsDirect2DRenderer();
}

class CheckAndRadioWindow : public GuiWindow
{
private:
	GuiControl*										groupCheck;
	GuiControl*										groupRadio;

	GuiSelectableButton*							checkBoxes[3];
	GuiSelectableButton*							radioButtons[3];
	GuiSelectableButton::MutexGroupController*		radioButtonGroupController;

public:
	CheckAndRadioWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		this->SetText(L"Controls.Button.CheckAndRadio");
		// limit the size that the window should always show the whole content without cliping it
		this->GetContainerComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);

		// create a table to layout the 2 group boxes
		GuiTableComposition* table=new GuiTableComposition;
		// make the table to have 2 rows
		table->SetRowsAndColumns(1, 2);
		table->SetRowOption(0, GuiCellOption::MinSizeOption());
		table->SetColumnOption(0, GuiCellOption::PercentageOption(0.5));
		table->SetColumnOption(1, GuiCellOption::PercentageOption(0.5));
		// dock the table to fill the window
		table->SetAlignmentToParent(Margin(4, 4, 4, 4));
		table->SetCellPadding(6);
		// add the table to the window;
		this->GetContainerComposition()->AddChild(table);

		// add group box for check boxes
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			// this cell is the left cell
			cell->SetSite(0, 0, 1, 1);

			groupCheck=g::NewGroupBox();
			groupCheck->GetContainerComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			// all child controls should at least 10 pixels away from the group box
			groupCheck->GetContainerComposition()->SetInternalMargin(Margin(10, 10, 10, 10));
			// dock the group box to fill the cell
			groupCheck->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			groupCheck->SetText(L"Check Boxes");
			// add the button to the cell
			cell->AddChild(groupCheck->GetBoundsComposition());

			// create a stack to layout the 3 buttons from top to bottom shown like a list
			GuiStackComposition* stack=new GuiStackComposition;
			stack->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			stack->SetDirection(GuiStackComposition::Vertical);
			stack->SetAlignmentToParent(Margin(0, 0, 0, 0));
			stack->SetPadding(6);
			groupCheck->GetContainerComposition()->AddChild(stack);

			// create check boxes
			for(int i=0;i<3;i++)
			{
				GuiStackItemComposition* stackItem=new GuiStackItemComposition;
				stack->AddChild(stackItem);

				checkBoxes[i]=g::NewCheckBox();
				checkBoxes[i]->SetText(L"This is a check box "+itow(i+1));
				checkBoxes[i]->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				stackItem->AddChild(checkBoxes[i]->GetBoundsComposition());
			}
		}

		// add group box for radio buttons
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			// this cell is the right cell
			cell->SetSite(0, 1, 1, 1);

			groupRadio=g::NewGroupBox();
			groupRadio->GetContainerComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			groupRadio->GetContainerComposition()->SetInternalMargin(Margin(10, 10, 10, 10));
			groupRadio->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			groupRadio->SetText(L"Radio Buttons");
			cell->AddChild(groupRadio->GetBoundsComposition());

			// create a stack to layout the 3 buttons from top to bottom shown like a list
			GuiStackComposition* stack=new GuiStackComposition;
			stack->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			stack->SetDirection(GuiStackComposition::Vertical);
			stack->SetAlignmentToParent(Margin(0, 0, 0, 0));
			stack->SetPadding(6);
			groupRadio->GetContainerComposition()->AddChild(stack);
			
			// create a group controller to group those radio buttons together
			// so that select a radio button will unselect the previous one automatically

			radioButtonGroupController=new GuiSelectableButton::MutexGroupController;
			this->AddComponent(radioButtonGroupController);

			// create radio buttons
			for(int i=0;i<3;i++)
			{
				GuiStackItemComposition* stackItem=new GuiStackItemComposition;
				stack->AddChild(stackItem);

				radioButtons[i]=g::NewRadioButton();
				radioButtons[i]->SetText(L"This is a radio button "+itow(i+1));
				radioButtons[i]->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				radioButtons[i]->SetGroupController(radioButtonGroupController);
				stackItem->AddChild(radioButtons[i]->GetBoundsComposition());
			}
		}

		// call this to calculate the size immediately if any indirect content in the table changes
		// so that the window can calcaulte its correct size before calling the MoveToScreenCenter()
		table->UpdateCellBounds();
		// update the size
		this->SetBounds(Rect());
		// move to the screen center
		this->MoveToScreenCenter();
	}

	~CheckAndRadioWindow()
	{
	}
};

void GuiMain()
{
	GuiWindow* window=new CheckAndRadioWindow();
	GetApplication()->Run(window);
	delete window;
}