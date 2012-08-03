#include "..\..\Public\Source\GacUIIncludes.h"

extern void SetupMainPanel(GuiControlHost* controlHost, GuiControl* container, GuiCellComposition* cell);

void SetupBasicWindow(GuiControlHost* controlHost, GuiControl* container)
{
	container->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	GuiTableComposition* table=new GuiTableComposition;
	table->SetAlignmentToParent(Margin(0, 0, 0, 0));
	container->GetContainerComposition()->AddChild(table);
	table->SetRowsAndColumns(2, 3);
	table->SetCellPadding(5);
	table->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	{
		table->SetRowOption(0, GuiCellOption::PercentageOption(1.0));
		table->SetRowOption(1, GuiCellOption::MinSizeOption());

		table->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
		table->SetColumnOption(1, GuiCellOption::MinSizeOption());
		table->SetColumnOption(2, GuiCellOption::MinSizeOption());
	}
	{
		GuiCellComposition* cell=new GuiCellComposition;
		table->AddChild(cell);
		cell->SetSite(0, 0, 1, 3);
		GuiScroll* progressBar=0;
		{
			GuiTableComposition* mainTable=new GuiTableComposition;
			cell->AddChild(mainTable);
			mainTable->SetAlignmentToParent(Margin(0, 0, 0, 0));
			mainTable->SetRowsAndColumns(4, 3);
			mainTable->SetCellPadding(0);
			mainTable->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);

			mainTable->SetRowOption(0, GuiCellOption::PercentageOption(1.0));
			mainTable->SetRowOption(1, GuiCellOption::AbsoluteOption(30));
			mainTable->SetRowOption(2, GuiCellOption::AbsoluteOption(win7::Win7TrackStyle::HandleLong));
			mainTable->SetRowOption(3, GuiCellOption::AbsoluteOption(win7::Win7ScrollStyle::DefaultSize));
			mainTable->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
			mainTable->SetColumnOption(1, GuiCellOption::AbsoluteOption(win7::Win7TrackStyle::HandleLong));
			mainTable->SetColumnOption(2, GuiCellOption::AbsoluteOption(win7::Win7ScrollStyle::DefaultSize));
			{
				GuiCellComposition* cell=new GuiCellComposition;
				mainTable->AddChild(cell);
				cell->SetSite(0, 0, 1, 1);
				SetupMainPanel(controlHost, container, cell);
			}
			{
				GuiCellComposition* cell=new GuiCellComposition;
				mainTable->AddChild(cell);
				cell->SetSite(0, 1, 1, 1);

				GuiScroll* scroll=new GuiScroll(new win7::Win7TrackStyle(win7::Win7TrackStyle::Vertical));
				scroll->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				scroll->SetTotalSize(10);
				cell->AddChild(scroll->GetBoundsComposition());
			}
			{
				GuiCellComposition* cell=new GuiCellComposition;
				mainTable->AddChild(cell);
				cell->SetSite(0, 2, 2, 1);

				GuiScroll* scroll=new GuiScroll(new win7::Win7ScrollStyle(win7::Win7ScrollStyle::Vertical));
				scroll->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				scroll->SetTotalSize(10);
				scroll->SetPageSize(6);
				scroll->SetBigMove(2);
				scroll->SetSmallMove(1);
				cell->AddChild(scroll->GetBoundsComposition());
			}
			{
				GuiCellComposition* cell=new GuiCellComposition;
				mainTable->AddChild(cell);
				cell->SetSite(1, 0, 1, 1);

				GuiScroll* scroll=new GuiScroll(new win7::Win7ProgressBarStyle);
				scroll->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				scroll->SetTotalSize(10);
				scroll->SetPageSize(4);
				cell->AddChild(scroll->GetBoundsComposition());
				progressBar=scroll;
			}
			{
				GuiCellComposition* cell=new GuiCellComposition;
				mainTable->AddChild(cell);
				cell->SetSite(2, 0, 1, 1);

				GuiScroll* scroll=new GuiScroll(new win7::Win7TrackStyle(win7::Win7TrackStyle::Horizontal));
				scroll->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				scroll->SetTotalSize(10);
				scroll->SetPageSize(4);
				cell->AddChild(scroll->GetBoundsComposition());

				scroll->PositionChanged.AttachLambda([progressBar, scroll](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
				{
					progressBar->SetPosition(scroll->GetPosition());
				});
			}
			{
				GuiCellComposition* cell=new GuiCellComposition;
				mainTable->AddChild(cell);
				cell->SetSite(3, 0, 1, 2);

				GuiScroll* scroll=new GuiScroll(new win7::Win7ScrollStyle(win7::Win7ScrollStyle::Horizontal));
				scroll->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				scroll->SetTotalSize(10);
				scroll->SetPageSize(6);
				scroll->SetBigMove(2);
				scroll->SetSmallMove(1);
				cell->AddChild(scroll->GetBoundsComposition());
			}
		}
	}

	const wchar_t* buttonTexts[]={L"Not Reactive", L"Close"};
	for(int i=0;i<2;i++)
	{
		GuiCellComposition* cell=new GuiCellComposition;
		table->AddChild(cell);
		cell->SetSite(1, i+1, 1, 1);
		cell->SetPreferredMinSize(Size(100, 0));

		GuiButton* button=new GuiButton(new win7::Win7ButtonStyle());
		cell->AddChild(button->GetBoundsComposition());
		button->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		button->SetText(buttonTexts[i]);

		button->Clicked.AttachLambda([controlHost](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			GuiButton* button=dynamic_cast<GuiButton*>(sender->GetAssociatedControl());
			if(button->GetText()==L"Close")
			{
				controlHost->Close();
			}
		});
	}
}