#include "NestleWindow.h"
#include "LoginPage.h"
#include "TopicListPage.h"

/***********************************************************************
NestleWindow
***********************************************************************/

void NestleWindow::PageInstall(Ptr<NestlePage> newPage)
{
	currentPage=newPage;
	pageContainerComposition->AddChild(newPage->GetBoundsComposition());
}

Ptr<NestlePage> NestleWindow::PageUninstall()
{
	Ptr<NestlePage> oldPage=0;
	if(currentPage)
	{
		pageContainerComposition->RemoveChild(currentPage->GetBoundsComposition());
		oldPage=currentPage;
		currentPage=0;
	}
	return oldPage;
}

void NestleWindow::PageForward(Ptr<NestlePage> newPage)
{
	Ptr<NestlePage> page=PageUninstall();
	if(page)
	{
		availableHistoryPages.Add(page);
	}
	PageInstall(newPage);
}

Ptr<NestlePage> NestleWindow::PageBackward()
{
	Ptr<NestlePage> oldPage=PageUninstall();
	if(availableHistoryPages.Count()>0)
	{
		int index=availableHistoryPages.Count()-1;
		Ptr<NestlePage> newPage=availableHistoryPages[index];
		availableHistoryPages.RemoveAt(index);
		PageInstall(newPage);
	}
	return oldPage;
}

void NestleWindow::PageClear()
{
	availableHistoryPages.Clear();
	PageBackward();
}

void NestleWindow::DelayDelete(Ptr<NestlePage> page)
{
	GetApplication()->InvokeInMainThread([page](){});
}

//--------------------------------------------------------------

void NestleWindow::LoginPage_Logined(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
	Ptr<LoginPage> loginPage=currentPage.Cast<LoginPage>();
	if(loginPage)
	{
		Ptr<NestleServer> server=loginPage->CreatedNestleServer();
		PageClear();
		TopicListPage* topicListPage=new TopicListPage(server);
		topicListPage->PostOpenRequested.AttachMethod(this, &NestleWindow::TopicListPage_PostOpenRequested);
		PageForward(topicListPage);
		DelayDelete(loginPage);
	}
	else
	{
		GetCurrentController()->DialogService()->ShowMessageBox(
			GetNativeWindow(),
			L"发生内部错误。"
			);
	}
}

void NestleWindow::LoginPage_Canceled_InitialLogin(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
	Close();
}

void NestleWindow::LoginPage_Canceled_NonInitialLogin(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
	PageBackward();
}

//--------------------------------------------------------------

void NestleWindow::TopicListPage_PostOpenRequested(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
{
}

//--------------------------------------------------------------

void NestleWindow::ShowLoginPage(bool initialLogin)
{
	LoginPage* loginPage=new LoginPage;
	loginPage->Logined.AttachMethod(this, &NestleWindow::LoginPage_Logined);
	if(initialLogin)
	{
		loginPage->Canceled.AttachMethod(this, &NestleWindow::LoginPage_Canceled_InitialLogin);
	}
	else
	{
		loginPage->Canceled.AttachMethod(this, &NestleWindow::LoginPage_Canceled_NonInitialLogin);
	}
	PageForward(loginPage);
}

NestleWindow::NestleWindow()
	:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	,pageContainerComposition(0)
	,currentPage(0)
{
	SetText(L"Vczh 鸟窝客户端 v1.0 (https://niaowo.me)");
	SetClientSize(Size(800, 600));
	GetBoundsComposition()->SetPreferredMinSize(Size(400, 300));
	GetContainerComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	InitializeComponents();
	ShowLoginPage(true);
	MoveToScreenCenter();
}

NestleWindow::~NestleWindow()
{
	PageClear();
}

/***********************************************************************
NestleWindow::InitializeComponents
***********************************************************************/

void NestleWindow::InitializeComponents()
{
	{
		Ptr<INativeImage> image=GetCurrentController()
			->ImageService()
			->CreateImageFromFile(L"..\\Resources\\Background.jpg");

		GuiImageFrameElement* element=GuiImageFrameElement::Create();
		element->SetImage(image);
		element->SetStretch(true);

		GuiBoundsComposition* composition=new GuiBoundsComposition();
		composition->SetOwnedElement(element);
		composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
		GetContainerComposition()->AddChild(composition);
	}
	{
		GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
		element->SetColor(Color(255, 255, 255, 224));

		GuiBoundsComposition* composition=new GuiBoundsComposition();
		composition->SetOwnedElement(element);
		composition->SetAlignmentToParent(Margin(24, 24, 24, 32));
		composition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
		GetContainerComposition()->AddChild(composition);

		pageContainerComposition=composition;
	}
	{
		FontProperties font=GetFont();
		font.bold=true;
		GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
		element->SetFont(font);
		element->SetColor(Color(255, 255, 255));
		element->SetAlignments(Alignment::Center, Alignment::Center);
		element->SetText(L"算法就是构造一个数据结构，把数据插入到数据结构的指定位置。 ---- @Kula");

		GuiBoundsComposition* composition=new GuiBoundsComposition();
		composition->SetOwnedElement(element);
		composition->SetAlignmentToParent(Margin(2, -1, 2, 2));
		composition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
		GetContainerComposition()->AddChild(composition);
	}
}