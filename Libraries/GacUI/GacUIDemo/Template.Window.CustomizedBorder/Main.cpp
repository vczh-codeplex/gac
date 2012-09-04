#include "..\..\Public\Source\GacUIIncludes.h"
#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsDirect2DRenderer();
}

/***********************************************************************
CustomTemplateWindowStyle
***********************************************************************/

class CustomTemplateWindowStyle : public GuiWindow::IStyleController
{
protected:
	GuiTableComposition*				boundsComposition;
	GuiBoundsComposition*				titleComposition;
	GuiSolidLabelElement*				titleElement;
	GuiBoundsComposition*				containerComposition;

	void AddBorderCell(int row, int column, int rowSpan, int columnSpan, INativeWindowListener::HitTestResult hitTestResult)
	{
		GuiCellComposition* cell=new GuiCellComposition;
		boundsComposition->AddChild(cell);
		cell->SetSite(row, column, rowSpan, columnSpan);
		cell->SetAssociatedHitTestResult(hitTestResult);
	}
public:
	CustomTemplateWindowStyle()
	{
		Color borderColor(0, 122, 204);
		Color titleBackgroundColor(45, 45, 48);
		Color backgroundColor(30, 30, 30);
		Color titleColor(153, 153, 153);

		boundsComposition=new GuiTableComposition;
		boundsComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
		boundsComposition->SetRowsAndColumns(4, 3);
		boundsComposition->SetRowOption(0, GuiCellOption::AbsoluteOption(3));
		boundsComposition->SetRowOption(1, GuiCellOption::MinSizeOption());
		boundsComposition->SetRowOption(2, GuiCellOption::PercentageOption(1.0));
		boundsComposition->SetRowOption(3, GuiCellOption::AbsoluteOption(3));
		boundsComposition->SetColumnOption(0, GuiCellOption::AbsoluteOption(3));
		boundsComposition->SetColumnOption(1, GuiCellOption::PercentageOption(1.0));
		boundsComposition->SetColumnOption(2, GuiCellOption::AbsoluteOption(3));
		{
			GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
			element->SetColor(borderColor);
			boundsComposition->SetOwnedElement(element);
		}
		AddBorderCell(0, 0, 1, 1, INativeWindowListener::BorderLeftTop);
		AddBorderCell(0, 2, 1, 1, INativeWindowListener::BorderRightTop);
		AddBorderCell(3, 0, 1, 1, INativeWindowListener::BorderLeftBottom);
		AddBorderCell(3, 2, 1, 1, INativeWindowListener::BorderRightBottom);
		AddBorderCell(1, 0, 2, 1, INativeWindowListener::BorderLeft);
		AddBorderCell(1, 2, 2, 1, INativeWindowListener::BorderRight);
		AddBorderCell(0, 1, 1, 1, INativeWindowListener::BorderTop);
		AddBorderCell(3, 1, 1, 1, INativeWindowListener::BorderBottom);
		{
			GuiCellComposition* cell=new GuiCellComposition;
			boundsComposition->AddChild(cell);
			cell->SetSite(1, 1, 1, 1);
			cell->SetAssociatedHitTestResult(INativeWindowListener::Title);
			{
				GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
				element->SetColor(titleBackgroundColor);
				cell->SetOwnedElement(element);
			}

			titleComposition=new GuiBoundsComposition;
			titleComposition->SetAlignmentToParent(Margin(5, 5, 5, 5));
			titleComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
			cell->AddChild(titleComposition);

			titleElement=GuiSolidLabelElement::Create();
			titleElement->SetColor(titleColor);
			titleComposition->SetOwnedElement(titleElement);

			FontProperties font=GetCurrentController()->ResourceService()->GetDefaultFont();
			titleElement->SetFont(font);
		}
		{
			GuiCellComposition* cell=new GuiCellComposition;
			boundsComposition->AddChild(cell);
			cell->SetSite(2, 1, 1, 1);
			cell->SetAssociatedHitTestResult(INativeWindowListener::Client);

			containerComposition=new GuiBoundsComposition;
			containerComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
			cell->AddChild(containerComposition);

			GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
			element->SetColor(backgroundColor);
			containerComposition->SetOwnedElement(element);
		}
	}

	GuiBoundsComposition* GetBoundsComposition()override
	{
		return boundsComposition;
	}

	GuiGraphicsComposition* GetContainerComposition()override
	{
		return containerComposition;
	}

	void SetFocusableComposition(compositions::GuiGraphicsComposition* value)override
	{
	}

	void SetText(const WString& value)override
	{
		titleElement->SetText(value);
		boundsComposition->ForceCalculateSizeImmediately();
	}

	void SetFont(const FontProperties& value)override
	{
	}

	void SetVisuallyEnabled(bool value)override
	{
	}

	void InitializeNativeWindowProperties(GuiWindow* window)override
	{
		// remove all native windows features
		window->SetMinimizedBox(false);
		window->SetMaximizedBox(false);
		window->SetTitleBar(false);
		window->SetSizeBox(false);
	}
};

/***********************************************************************
CustomTemplateWindow
***********************************************************************/

class CustomTemplateWindow : public GuiWindow
{
private:
public:
	CustomTemplateWindow()
		:GuiWindow(new CustomTemplateWindowStyle)
	{
		this->SetText(L"Template.Window.CustomizedBorder");

		// set the preferred minimum client size
		this->GetBoundsComposition()->SetPreferredMinSize(Size(640, 480));
		// call this to calculate the size immediately if any indirect content in the table changes
		// so that the window can calcaulte its correct size before calling the MoveToScreenCenter()
		this->ForceCalculateSizeImmediately();
		// move to the screen center
		this->MoveToScreenCenter();
	}

	~CustomTemplateWindow()
	{
	}
};

/***********************************************************************
GuiMain
***********************************************************************/

void GuiMain()
{
	GuiWindow* window=new CustomTemplateWindow;
	GetApplication()->Run(window);
	delete window;
}