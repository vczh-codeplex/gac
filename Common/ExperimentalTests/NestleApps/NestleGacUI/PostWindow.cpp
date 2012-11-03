#include "PostWindow.h"

namespace vl
{
	namespace nestle
	{

/***********************************************************************
PostItemControl
***********************************************************************/

		void PostItemControl::buttonReply_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
		}

		PostItemControl::PostItemControl()
		{
			InitializeComponents();
		}

		void PostItemControl::Install(Ptr<Object> value)
		{
			postItem=value.Cast<PostItem>();
			titleElement->SetText(L"◇ "+postItem->title);
			authorElement->SetText(postItem->author);
			dateTimeElement->SetText(postItem->createDateTime);
			bodyElement->SetText(L"    "+postItem->body);
		}

/***********************************************************************
PostItemControl::InitializeComponents
***********************************************************************/

		void PostItemControl::InitializeComponents()
		{
			FontProperties titleFont=GetFont();
			titleFont.size=28;
			titleFont.bold=true;
			FontProperties bodyFont=GetFont();
			bodyFont.size=16;
			FontProperties buttonFont=GetFont();
			titleFont.size=22;

			Color titleColor(0, 0, 0);
			Color frameColor(128, 128, 128);
			Color bodyColor(64, 64, 64);

			GuiTableComposition* table=new GuiTableComposition;
			GetContainerComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			GetContainerComposition()->AddChild(table);
			table->SetAlignmentToParent(Margin(10, 4, 10, 4));
			table->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			table->SetRowsAndColumns(4, 4);
			table->SetCellPadding(4);
			table->SetRowOption(0, GuiCellOption::MinSizeOption());
			table->SetRowOption(1, GuiCellOption::MinSizeOption());
			table->SetRowOption(2, GuiCellOption::MinSizeOption());
			table->SetRowOption(3, GuiCellOption::MinSizeOption());
			table->SetColumnOption(0, GuiCellOption::MinSizeOption());
			table->SetColumnOption(1, GuiCellOption::PercentageOption(0.5));
			table->SetColumnOption(2, GuiCellOption::MinSizeOption());
			table->SetColumnOption(3, GuiCellOption::MinSizeOption());
			{
				GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
				element->SetFont(titleFont);
				element->SetEllipse(true);
				element->SetText(L"Title");
				element->SetColor(titleColor);
				titleElement=element;

				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetOwnedElement(element);
				cell->SetSite(0, 0, 1, 4);
			}
			{
				GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
				element->SetFont(bodyFont);
				element->SetText(L"◇ Po主：");
				element->SetColor(frameColor);

				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetOwnedElement(element);
				cell->SetSite(1, 0, 1, 1);
			}
			{
				GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
				element->SetFont(bodyFont);
				element->SetText(L"Author");
				element->SetColor(bodyColor);
				authorElement=element;

				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetOwnedElement(element);
				cell->SetSite(1, 1, 1, 1);
			}
			{
				GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
				element->SetFont(bodyFont);
				element->SetText(L"◇ 时间：");
				element->SetColor(frameColor);

				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetOwnedElement(element);
				cell->SetSite(1, 2, 1, 1);
			}
			{
				GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
				element->SetFont(bodyFont);
				element->SetText(L"DateTime");
				element->SetColor(bodyColor);
				dateTimeElement=element;

				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetOwnedElement(element);
				cell->SetSite(1, 3, 1, 1);
			}
			{
				GuiBoundsComposition* descriptionComposition=0;
				{
					GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
					element->SetFont(bodyFont);
					element->SetWrapLine(true);
					element->SetWrapLineHeightCalculation(true);
					element->SetEllipse(true);
					element->SetText(L"Description");
					element->SetColor(bodyColor);
					bodyElement=element;

					GuiBoundsComposition* composition=new GuiBoundsComposition;
					composition->SetOwnedElement(element);
					composition->SetAlignmentToParent(Margin(3, 3, 3, 3));
					descriptionComposition=composition;
				}

				GuiSolidBorderElement* element=GuiSolidBorderElement::Create();
				element->SetColor(Color(128, 128, 128));

				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetOwnedElement(element);
				cell->AddChild(descriptionComposition);
				cell->SetInternalMargin(Margin(3, 3, 3, 3));
				cell->SetSite(2, 0, 1, 4);
			}
			{
				buttonReply=g::NewButton();
				buttonReply->SetFont(buttonFont);
				buttonReply->SetText(L"我要♂一战");
				buttonReply->GetBoundsComposition()->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetSystemCursor(INativeCursor::Hand));
				buttonReply->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, -1, 0));
				buttonReply->Clicked.AttachMethod(this, &PostItemControl::buttonReply_Clicked);

				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->AddChild(buttonReply->GetBoundsComposition());
				cell->SetSite(3, 0, 1, 4);
			}
		}

/***********************************************************************
PostWindow
***********************************************************************/

		PostWindow::PostWindow(Ptr<NestleServer> _server, Ptr<NestlePost> _post)
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
			,server(_server)
			,post(_post)
		{
			SetText(L"围观帖子："+post->title);
			SetMinimizedBox(false);
			SetClientSize(Size(640, 480));
			GetBoundsComposition()->SetPreferredMinSize(Size(400, 300));
			GetContainerComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			MoveToScreenCenter();

			InitializeComponents();
		}

		PostWindow::~PostWindow()
		{
		}

/***********************************************************************
PostWindow::InitializeComponents
***********************************************************************/

		void PostWindow::InitializeComponents()
		{
		}
	}
}