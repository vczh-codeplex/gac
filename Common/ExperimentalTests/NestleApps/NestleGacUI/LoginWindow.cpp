#include "LoginWindow.h"

namespace vl
{
	namespace nestle
	{

/***********************************************************************
LoginWindow
***********************************************************************/

		void LoginWindow::buttonLogin_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			nestleServer=0;
			WString apiKey=L"e1424eb84b6d4169a10c03fb1e73e140";
			WString apiSecret=L"9814021f20054b558105fca1df6559a7";
			WString username=textUsername->GetText();
			WString password=textPassword->GetText();
			textUsername->SetEnabled(false);
			textPassword->SetEnabled(false);
			buttonLogin->SetEnabled(false);
			buttonCancel->SetEnabled(false);
			GetBoundsComposition()->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetSystemCursor(INativeCursor::LargeWaiting));

			GetApplication()->InvokeAsync([=]()
			{
				Ptr<NestleServer> server=new NestleServer(username, password, apiKey, apiSecret);
				GetApplication()->InvokeInMainThreadAndWait([=]()
				{
					if(server->IsLoginSuccess())
					{
						nestleServer=server;
						Close();
					}
					else
					{
						GetCurrentController()->DialogService()->ShowMessageBox(
							GetBoundsComposition()->GetRelatedControlHost()->GetNativeWindow(),
							L"��¼ʧ�ܣ������û����������Ƿ���ȷ���������������Ƿ�ͨ��"
							);
					}
					GetBoundsComposition()->SetAssociatedCursor(0);
					textUsername->SetEnabled(true);
					textPassword->SetEnabled(true);
					buttonLogin->SetEnabled(true);
					buttonCancel->SetEnabled(true);
				});
			});
		}

		void LoginWindow::buttonCancel_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			Close();
		}

		LoginWindow::LoginWindow()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		{
			InitializeComponents();
		}

		LoginWindow::~LoginWindow()
		{
		}

		Ptr<NestleServer> LoginWindow::CreatedNestleServer()
		{
			return nestleServer;
		}

/***********************************************************************
LoginWindow::InitializeComponents
***********************************************************************/

		void LoginWindow::InitializeComponents()
		{
			SetText(L"��½���� (https://niaowo.me)");
			SetMaximizedBox(false);
			SetMinimizedBox(false);
			SetSizeBox(false);
			SetClientSize(Size(640, 480));
			GetBoundsComposition()->SetPreferredMinSize(Size(640, 480));
			GetContainerComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			MoveToScreenCenter();

			GuiGraphicsComposition* loginContainer=0;
			{
				GuiTableComposition* table=new GuiTableComposition;
				table->SetAlignmentToParent(Margin(0, 0, 0, 0));
				table->SetRowsAndColumns(2, 1);
				table->SetRowOption(0, GuiCellOption::PercentageOption(1.0));
				table->SetRowOption(1, GuiCellOption::AbsoluteOption(32));
				table->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
				GetContainerComposition()->AddChild(table);
				{
					Ptr<INativeImage> image=GetCurrentController()
						->ImageService()
						->CreateImageFromFile(L"..\\Resources\\LoginWindowBackground.jpg");

					GuiImageFrameElement* element=GuiImageFrameElement::Create();
					element->SetImage(image);
					element->SetStretch(true);
					table->SetOwnedElement(element);
				}
				{
					GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
					element->SetColor(Color(255, 255, 255, 160));

					GuiCellComposition* cell=new GuiCellComposition;
					table->AddChild(cell);
					cell->SetSite(0, 0, 1, 1);
					cell->SetOwnedElement(element);
					loginContainer=cell;
				}
				{
					FontProperties font=GetFont();
					font.bold=true;
					GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
					element->SetFont(font);
					element->SetColor(Color(255, 255, 255));
					element->SetAlignments(Alignment::Center, Alignment::Center);
					element->SetText(L"�㷨���ǹ���һ�����ݽṹ�������ݲ��뵽���ݽṹ��ָ��λ�á� ---- @Kula");

					GuiCellComposition* cell=new GuiCellComposition;
					table->AddChild(cell);
					cell->SetSite(1, 0, 1, 1);
					cell->SetOwnedElement(element);
				}
			}
			{
				GuiTableComposition* table=new GuiTableComposition;
				loginContainer->AddChild(table);
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
					label->SetText(L"�û�����");
					cell->AddChild(label->GetBoundsComposition());
				}
				{
					GuiCellComposition* cell=new GuiCellComposition;
					table->AddChild(cell);
					cell->SetSite(2, 1, 1, 1);

					GuiLabel* label=g::NewLabel();
					label->SetText(L"���룺");
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
					buttonLogin->SetText(L"  ǿ�Ƶ�¼����  ");
					buttonLogin->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
					buttonLogin->Clicked.AttachMethod(this, &LoginWindow::buttonLogin_Clicked);
					cell->AddChild(buttonLogin->GetBoundsComposition());
				}
				{
					GuiCellComposition* cell=new GuiCellComposition;
					table->AddChild(cell);
					cell->SetSite(3, 4, 1, 1);

					buttonCancel=g::NewButton();
					buttonCancel->GetBoundsComposition()->SetPreferredMinSize(Size(80, 36));
					buttonCancel->SetText(L"  ���ޣ�  ");
					buttonCancel->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
					buttonCancel->Clicked.AttachMethod(this, &LoginWindow::buttonCancel_Clicked);
					cell->AddChild(buttonCancel->GetBoundsComposition());
				}
				{
					GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
					element->SetFont(GetFont());
					element->SetWrapLine(true);
					element->SetText(L"�����¼������@Kula��������վ�����������������ǽ��м��黥����");

					GuiCellComposition* cell=new GuiCellComposition;
					table->AddChild(cell);
					cell->SetOwnedElement(element);
					cell->SetSite(4, 1, 1, 4);
				}
			}
		}
	}
}