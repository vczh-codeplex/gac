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
			PostWindow* postWindow=dynamic_cast<PostWindow*>(sender->GetRelatedControlHost());
			postWindow->Reply(authorElement->GetText());
		}

		PostItemControl::PostItemControl()
		{
			InitializeComponents();
		}

		void PostItemControl::Install(Ptr<Object> value)
		{
			postItem=value.Cast<PostItem>();
			if(postItem->title==L"")
			{
				titleElement->SetText(L"-----------------回复-----------------");
			}
			else
			{
				titleElement->SetText(L"◇ "+postItem->title);
			}
			authorElement->SetText(postItem->author);
			dateTimeElement->SetText(postItem->createDateTime);
			bodyElement->SetText(postItem->body);
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
					composition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
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

		void PostWindow::buttonPost_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			WString body=textBody->GetText();
			buttonPost->SetEnabled(false);
			GetApplication()->InvokeAsync([=]()
			{
				Ptr<NestleComment> comment=server->PostComment(post->id, body);
				GetApplication()->InvokeInMainThreadAndWait([=]()
				{
					buttonPost->SetEnabled(true);
					if(comment)
					{
						textBody->SetText(L"");
						textBody->SetFocus();
						RefreshPostItems();
					}
					else
					{
						GetCurrentController()->DialogService()->ShowMessageBox(
							GetBoundsComposition()->GetRelatedControlHost()->GetNativeWindow(),
							L"回帖失败，请检查网络连接是否畅通。"
							);
					}
				});
			});
		}

		void PostWindow::buttonCancel_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			Close();
		}

		void PostWindow::ClearPostItems()
		{
			FOREACH(PostItemControl*, postItemControl, postItemControls.Wrap())
			{
				GuiStackItemComposition* stackItem=dynamic_cast<GuiStackItemComposition*>(postItemControl->GetBoundsComposition()->GetParent());
				postItemStack->RemoveChild(stackItem);
				stackItem->RemoveChild(postItemControl->GetBoundsComposition());
				delete postItemControl;
				delete stackItem;
			}
			postItemControls.Clear();
		}

		void PostWindow::AddPostItem(Ptr<PostItem> postItem)
		{
			PostItemControl* postItemControl=new PostItemControl;
			postItemControl->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			postItemControl->Install(postItem);
			postItemControls.Add(postItemControl);

			GuiStackItemComposition* item=new GuiStackItemComposition;
			item->AddChild(postItemControl->GetBoundsComposition());
			postItemStack->AddChild(item);
		}

		void PostWindow::RefreshPostItems()
		{
			buttonPost->SetEnabled(false);
			buttonCancel->SetEnabled(false);
			postItemContainers->SetEnabled(false);

			GetApplication()->InvokeAsync([=]()
			{
				Ptr<NestlePost> newPost=server->GetTopic(post->id);

				GetApplication()->InvokeInMainThreadAndWait([=]()
				{
					post=newPost;
					ClearPostItems();
					{
						Ptr<PostItem> postItem=new PostItem;
						postItem->title=post->title;
						postItem->author=post->author;
						postItem->createDateTime=post->createDateTime;
						postItem->body=post->body;
						AddPostItem(postItem);
					}
					FOREACH(Ptr<NestleComment>, comment, post->comments.Wrap())
					{
						Ptr<PostItem> postItem=new PostItem;
						postItem->author=comment->author;
						postItem->createDateTime=comment->createDateTime;
						postItem->body=comment->body;
						AddPostItem(postItem);
					}

					buttonPost->SetEnabled(true);
					buttonCancel->SetEnabled(true);
					postItemContainers->SetEnabled(true);
				});
			});
		}

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
			RefreshPostItems();
		}

		PostWindow::~PostWindow()
		{
		}

		void PostWindow::Reply(const WString& author)
		{
			textBody->SetText(L"@"+author+L"\r\n"+textBody->GetText());
			textBody->Select(TextPos(), TextPos());
			textBody->SetFocus();
		}

/***********************************************************************
PostWindow::InitializeComponents
***********************************************************************/

		void PostWindow::InitializeComponents()
		{
			GuiTableComposition* table=new GuiTableComposition;
			GetContainerComposition()->AddChild(table);
			table->SetAlignmentToParent(Margin(2, 2, 2, 2));
			table->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			table->SetCellPadding(4);
			table->SetRowsAndColumns(3, 3);
			table->SetRowOption(0, GuiCellOption::PercentageOption(1.0));
			table->SetRowOption(1, GuiCellOption::AbsoluteOption(160));
			table->SetRowOption(2, GuiCellOption::MinSizeOption());
			table->SetColumnOption(0, GuiCellOption::MinSizeOption());
			table->SetColumnOption(1, GuiCellOption::PercentageOption(1.0));
			table->SetColumnOption(2, GuiCellOption::MinSizeOption());
			{
				postItemContainers=g::NewScrollContainer();
				postItemContainers->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				postItemContainers->SetHorizontalAlwaysVisible(false);

				postItemStack=new GuiStackComposition;
				postItemStack->SetAlignmentToParent(Margin(0, 0, 0, 0));
				postItemStack->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
				postItemStack->SetDirection(GuiStackComposition::Vertical);
				postItemContainers->GetContainerComposition()->AddChild(postItemStack);

				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetSite(0, 0, 1, 3);
				cell->AddChild(postItemContainers->GetBoundsComposition());
			}
			{
				textBody=g::NewMultilineTextBox();
				textBody->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				textBody->SetText(L"<请输入回复的内容>");

				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetSite(1, 0, 1, 3);
				cell->AddChild(textBody->GetBoundsComposition());
			}
			{
				buttonPost=g::NewButton();
				buttonPost->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				buttonPost->SetText(L"回帖");
				buttonPost->GetBoundsComposition()->SetPreferredMinSize(Size(80, 36));
				buttonPost->Clicked.AttachMethod(this, &PostWindow::buttonPost_Clicked);

				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetSite(2, 0, 1, 1);
				cell->AddChild(buttonPost->GetBoundsComposition());
			}
			{
				buttonCancel=g::NewButton();
				buttonCancel->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				buttonCancel->SetText(L"关闭");
				buttonCancel->GetBoundsComposition()->SetPreferredMinSize(Size(80, 36));
				buttonCancel->Clicked.AttachMethod(this, &PostWindow::buttonCancel_Clicked);

				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetSite(2, 2, 1, 1);
				cell->AddChild(buttonCancel->GetBoundsComposition());
			}
		}
	}
}