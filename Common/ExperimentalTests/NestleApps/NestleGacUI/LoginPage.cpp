#include "LoginPage.h"

/***********************************************************************
LoginPage
***********************************************************************/

void LoginPage::buttonLogin_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
	nestleServer=0;
	WString apiKey=L"e1424eb84b6d4169a10c03fb1e73e140";
	WString apiSecret=L"9814021f20054b558105fca1df6559a7";
	WString username=textUsername->GetText();
	WString password=textPassword->GetText();
	SetEnabled(false);
	GetBoundsComposition()->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetSystemCursor(INativeCursor::LargeWaiting));

	GetApplication()->InvokeAsync([=]()
	{
		Ptr<NestleServer> server=new NestleServer(username, password, apiKey, apiSecret);
		GetApplication()->InvokeInMainThreadAndWait([=]()
		{
			if(server->IsLoginSuccess())
			{
				nestleServer=server;
				GuiEventArgs arguments;
				Logined.Execute(arguments);
			}
			else
			{
				GetCurrentController()->DialogService()->ShowMessageBox(
					GetBoundsComposition()->GetRelatedControlHost()->GetNativeWindow(),
					L"登录失败，请检查用户名或密码是否正确，或者网络连接是否畅通。"
					);
			}
			GetBoundsComposition()->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetDefaultSystemCursor());
			SetEnabled(true);
		});
	});
}

void LoginPage::buttonCancel_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
	Canceled.Execute(arguments);
}

LoginPage::LoginPage()
{
	Logined.SetAssociatedComposition(GetBoundsComposition());
	Canceled.SetAssociatedComposition(GetBoundsComposition());
	InitializeComponents();
}

Ptr<NestleServer> LoginPage::CreatedNestleServer()
{
	return nestleServer;
}

/***********************************************************************
LoginPage::InitializeComponents
***********************************************************************/

void LoginPage::InitializeComponents()
{
	GuiTableComposition* table=new GuiTableComposition;
	GetContainerComposition()->AddChild(table);
	table->SetAlignmentToParent(Margin(0, 0, 0, 0));
	table->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	table->SetRowsAndColumns(5, 6);
	table->SetCellPadding(8);
	table->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
	table->SetRowOption(1, GuiCellOption::MinSizeOption());
	table->SetRowOption(2, GuiCellOption::MinSizeOption());
	table->SetRowOption(3, GuiCellOption::MinSizeOption());
	table->SetRowOption(4, GuiCellOption::PercentageOption(0.5));
	table->SetColumnOption(0, GuiCellOption::PercentageOption(0.5));
	table->SetColumnOption(1, GuiCellOption::MinSizeOption());
	table->SetColumnOption(2, GuiCellOption::AbsoluteOption(40));
	table->SetColumnOption(3, GuiCellOption::MinSizeOption());
	table->SetColumnOption(4, GuiCellOption::MinSizeOption());
	table->SetColumnOption(5, GuiCellOption::PercentageOption(0.5));
	{
		GuiCellComposition* cell=new GuiCellComposition;
		table->AddChild(cell);
		cell->SetSite(1, 1, 1, 1);

		GuiLabel* label=g::NewLabel();
		label->SetText(L"用户名：");
		cell->AddChild(label->GetBoundsComposition());
	}
	{
		GuiCellComposition* cell=new GuiCellComposition;
		table->AddChild(cell);
		cell->SetSite(2, 1, 1, 1);

		GuiLabel* label=g::NewLabel();
		label->SetText(L"密码：");
		cell->AddChild(label->GetBoundsComposition());
	}
	{
		GuiCellComposition* cell=new GuiCellComposition;
		table->AddChild(cell);
		cell->SetSite(1, 2, 1, 3);

		textUsername=g::NewTextBox();
		textUsername->GetBoundsComposition()->SetPreferredMinSize(Size(0, 32));
		textUsername->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		cell->AddChild(textUsername->GetBoundsComposition());
	}
	{
		GuiCellComposition* cell=new GuiCellComposition;
		table->AddChild(cell);
		cell->SetSite(2, 2, 1, 3);

		textPassword=g::NewTextBox();
		textPassword->SetPasswordChar(L'*');
		textPassword->GetBoundsComposition()->SetPreferredMinSize(Size(0, 32));
		textPassword->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		cell->AddChild(textPassword->GetBoundsComposition());
	}
	{
		GuiCellComposition* cell=new GuiCellComposition;
		table->AddChild(cell);
		cell->SetSite(3, 3, 1, 1);

		buttonLogin=g::NewButton();
		buttonLogin->GetBoundsComposition()->SetPreferredMinSize(Size(80, 36));
		buttonLogin->SetText(L"  强势登录鸟窝  ");
		buttonLogin->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		buttonLogin->Clicked.AttachMethod(this, &LoginPage::buttonLogin_Clicked);
		cell->AddChild(buttonLogin->GetBoundsComposition());
	}
	{
		GuiCellComposition* cell=new GuiCellComposition;
		table->AddChild(cell);
		cell->SetSite(3, 4, 1, 1);

		buttonCancel=g::NewButton();
		buttonCancel->GetBoundsComposition()->SetPreferredMinSize(Size(80, 36));
		buttonCancel->SetText(L"  我呸！  ");
		buttonCancel->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		buttonCancel->Clicked.AttachMethod(this, &LoginPage::buttonCancel_Clicked);
		cell->AddChild(buttonCancel->GetBoundsComposition());
	}
	{
		GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
		element->SetFont(GetFont());
		element->SetWrapLine(true);
		element->SetText(L"点击登录，畅游@Kula的鸟窝网站，放松身心与鸟人们进行激情互动！");

		GuiCellComposition* cell=new GuiCellComposition;
		table->AddChild(cell);
		cell->SetOwnedElement(element);
		cell->SetSite(4, 1, 1, 4);
	}
}