#include "MainWindow.h"

namespace vl
{
	namespace nestle
	{

/***********************************************************************
TopicItemControl
***********************************************************************/

		void TopicItemControl::buttonRead_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
		}

		TopicItemControl::TopicItemControl()
		{
			InitializeComponents();
		}

		void TopicItemControl::Install(Ptr<Object> value)
		{
			post=value.Cast<NestlePost>();
			titleElement->SetText(L"◇ "+post->title);
			authorElement->SetText(post->author);
			dateTimeElement->SetText(post->createDateTime);
			descriptionElement->SetText(L"    "+post->description);
		}

/***********************************************************************
TopicItemControl::InitializeComponents
***********************************************************************/

		void TopicItemControl::InitializeComponents()
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
			table->SetRowOption(2, GuiCellOption::AbsoluteOption(60));
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
					element->SetEllipse(true);
					element->SetText(L"Description");
					element->SetColor(bodyColor);
					descriptionElement=element;

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
				buttonRead=g::NewButton();
				buttonRead->SetFont(buttonFont);
				buttonRead->SetText(L"………………  围观♂Po主  ………………");
				buttonRead->GetBoundsComposition()->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetSystemCursor(INativeCursor::Hand));
				buttonRead->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, -1, 0));
				buttonRead->Clicked.AttachMethod(this, &TopicItemControl::buttonRead_Clicked);

				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->AddChild(buttonRead->GetBoundsComposition());
				cell->SetSite(3, 0, 1, 4);
			}
		}

/***********************************************************************
MainWindow
***********************************************************************/

		void MainWindow::SwitchToLogoutStatus()
		{
			commandRefresh->SetEnabled(false);
			commandPrevious->SetEnabled(false);
			commandNext->SetEnabled(false);
			commandNewPost->SetEnabled(false);
			commandSearch->SetEnabled(false);
			commandUserLogout->SetEnabled(false);
			containerLogin->SetVisible(true);
			containerTopicList->SetVisible(false);
			server=0;
			currentPage=0;
			loginWindow=0;
		}

		void MainWindow::SwitchToLoginStatus()
		{
			commandRefresh->SetEnabled(true);
			commandPrevious->SetEnabled(true);
			commandNext->SetEnabled(true);
			commandNewPost->SetEnabled(true);
			commandSearch->SetEnabled(true);
			commandUserLogout->SetEnabled(true);
			containerLogin->SetVisible(false);
			containerTopicList->SetVisible(true);
		}

		void MainWindow::LoadTopics()
		{
			listTopics->GetItems().Clear();
			if(server)
			{
				SetEnabled(false);
				GetApplication()->InvokeAsync([=]()
				{
					listTopics->GetBoundsComposition()->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetSystemCursor(INativeCursor::LargeWaiting));
					Ptr<NestleTopicsPage> page=server->GetTopics(0);

					GetApplication()->InvokeInMainThreadAndWait([=]()
					{
						currentPage=page;
						FOREACH(Ptr<NestlePost>, post, page->posts.Wrap())
						{
							listTopics->GetItems().Add(post);
						}
						listTopics->GetBoundsComposition()->SetAssociatedCursor(0);
						SetEnabled(true);
						GetNativeWindow()->SetFocus();
					});
				});
			}
		}

		void MainWindow::loginWindow_Closed(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			server=loginWindow->CreatedNestleServer();
			if(server)
			{
				SwitchToLoginStatus();
			}
			SetEnabled(true);
			GetNativeWindow()->SetFocus();
			LoadTopics();
		}

		void MainWindow::commandRefresh_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
		}

		void MainWindow::commandPrevious_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
		}

		void MainWindow::commandNext_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
		}

		void MainWindow::commandNewPost_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
		}

		void MainWindow::commandSearch_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
		}

		void MainWindow::commandUserLogin_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			SetEnabled(false);
			loginWindow=new LoginWindow;
			loginWindow->WindowClosed.AttachMethod(this, &MainWindow::loginWindow_Closed);
			loginWindow->Show();
		}

		void MainWindow::commandUserLogout_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			SwitchToLogoutStatus();
		}

		MainWindow::MainWindow()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		{
			SetText(L"Vczh鸟窝客户端 v1.0 (https://niaowo.me)");
			SetMinimizedBox(false);
			SetClientSize(Size(800, 600));
			GetBoundsComposition()->SetPreferredMinSize(Size(640, 480));
			GetContainerComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			MoveToScreenCenter();

			InitializeComponents();

			commandRefresh->Executed.AttachMethod(this, &MainWindow::commandRefresh_Clicked);
			commandPrevious->Executed.AttachMethod(this, &MainWindow::commandPrevious_Clicked);
			commandNext->Executed.AttachMethod(this, &MainWindow::commandNext_Clicked);
			commandNewPost->Executed.AttachMethod(this, &MainWindow::commandNewPost_Clicked);
			commandSearch->Executed.AttachMethod(this, &MainWindow::commandSearch_Clicked);
			commandUserLogin->Executed.AttachMethod(this, &MainWindow::commandUserLogin_Clicked);
			commandUserLogout->Executed.AttachMethod(this, &MainWindow::commandUserLogout_Clicked);

			SwitchToLogoutStatus();
		}

		MainWindow::~MainWindow()
		{
		}

/***********************************************************************
LoginWindow::InitializeComponents
***********************************************************************/

		void MainWindow::InitializeComponents()
		{
			{
				GuiToolstripCommand* command=new GuiToolstripCommand;
				command->SetImage(new GuiImageData(GetCurrentController()->ImageService()->CreateImageFromFile(L"..\\Resources\\ToolstripRefresh.png"), 0));
				command->SetText(L"刷新");
				commandRefresh=command;
			}
			{
				GuiToolstripCommand* command=new GuiToolstripCommand;
				command->SetImage(new GuiImageData(GetCurrentController()->ImageService()->CreateImageFromFile(L"..\\Resources\\ToolstripPrevious.png"), 0));
				command->SetText(L"上一页");
				commandPrevious=command;
			}
			{
				GuiToolstripCommand* command=new GuiToolstripCommand;
				command->SetImage(new GuiImageData(GetCurrentController()->ImageService()->CreateImageFromFile(L"..\\Resources\\ToolstripNext.png"), 0));
				command->SetText(L"下一页");
				commandNext=command;
			}
			{
				GuiToolstripCommand* command=new GuiToolstripCommand;
				command->SetImage(new GuiImageData(GetCurrentController()->ImageService()->CreateImageFromFile(L"..\\Resources\\ToolstripNewPost.png"), 0));
				command->SetText(L"新帖子");
				commandNewPost=command;
			}
			{
				GuiToolstripCommand* command=new GuiToolstripCommand;
				command->SetImage(new GuiImageData(GetCurrentController()->ImageService()->CreateImageFromFile(L"..\\Resources\\ToolstripSearch.png"), 0));
				command->SetText(L"搜索");
				commandSearch=command;
			}
			{
				GuiToolstripCommand* command=new GuiToolstripCommand;
				command->SetImage(new GuiImageData(GetCurrentController()->ImageService()->CreateImageFromFile(L"..\\Resources\\ToolstripUser.png"), 0));
				command->SetText(L"用户");
				commandUser=command;
			}
			{
				GuiToolstripCommand* command=new GuiToolstripCommand;
				command->SetText(L"登录 ...");
				commandUserLogin=command;
			}
			{
				GuiToolstripCommand* command=new GuiToolstripCommand;
				command->SetText(L"注销");
				commandUserLogout=command;
			}
			GuiTableComposition* table=new GuiTableComposition;
			table->SetAlignmentToParent(Margin(0, 0, 0, 0));
			table->SetCellPadding(2);
			table->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			table->SetRowsAndColumns(2, 1);
			table->SetRowOption(0, GuiCellOption::MinSizeOption());
			table->SetRowOption(1, GuiCellOption::PercentageOption(1.0));
			table->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
			GetContainerComposition()->AddChild(table);
			{
				toolbar=g::NewToolbar();
				toolbar->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				toolbar->GetBuilder()
					->Button(commandRefresh)
					->Button(commandPrevious)
					->Button(commandNext)
					->Splitter()
					->Button(commandNewPost)
					->Button(commandSearch)
					->Splitter()
					->DropdownButton(commandUser)->BeginSubMenu()
						->Button(commandUserLogin)
						->Button(commandUserLogout)
						->EndSubMenu()
					;

				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetSite(0, 0, 1, 1);
				cell->AddChild(toolbar->GetBoundsComposition());
			}
			{
				{
					GuiTableComposition* table=new GuiTableComposition;
					containerLogin=table;
					table->SetAlignmentToParent(Margin(0, 0, 0, 0));
					table->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
					table->SetRowsAndColumns(3, 3);
					table->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
					table->SetRowOption(1, GuiCellOption::MinSizeOption());
					table->SetRowOption(2, GuiCellOption::PercentageOption(0.5));
					table->SetColumnOption(0, GuiCellOption::PercentageOption(0.5));
					table->SetColumnOption(1, GuiCellOption::MinSizeOption());
					table->SetColumnOption(2, GuiCellOption::PercentageOption(0.5));

					buttonLogin=g::NewButton();
					buttonLogin->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
					buttonLogin->GetBoundsComposition()->SetPreferredMinSize(Size(340, 100));
					buttonLogin->SetText(L"强势登录鸟窝");
					buttonLogin->Clicked.AttachMethod(this, &MainWindow::commandUserLogin_Clicked);

					FontProperties font=buttonLogin->GetFont();
					font.size=48;
					font.bold=true;
					buttonLogin->SetFont(font);

					GuiCellComposition* cell=new GuiCellComposition;
					table->AddChild(cell);
					cell->SetSite(1, 1, 1, 1);
					cell->AddChild(buttonLogin->GetBoundsComposition());
				}
				{
					listTopics=new GuiListControlTemplate<TopicItemControl>(false);
					listTopics->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
					listTopics->SetHorizontalAlwaysVisible(false);
					containerTopicList=listTopics->GetBoundsComposition();
				}

				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetSite(1, 0, 1, 1);
				cell->AddChild(containerLogin);
				cell->AddChild(containerTopicList);
			}
		}
	}
}