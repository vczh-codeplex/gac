#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "..\..\..\..\Libraries\GacUI\Public\Source\GacUIIncludes.h"
#include "..\NestleConsole\NestleSDK.h"

using namespace vl::nestle;

#define GUI_GRAPHICS_RENDERER_DIRECT2D

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	int result=SetupWindowsDirect2DRenderer();

#if _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return result;
}

/***********************************************************************
Page
***********************************************************************/

class NestlePage : public GuiControl
{
private:
	class StyleController : public GuiControl::IStyleController
	{
	protected:
		GuiBoundsComposition*			composition;
	public:
		StyleController()
		{
			composition=new GuiBoundsComposition();
			composition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
		}

		compositions::GuiBoundsComposition* GetBoundsComposition()
		{
			return composition;
		}

		compositions::GuiGraphicsComposition* GetContainerComposition()
		{
			return composition;
		}

		void SetFocusableComposition(compositions::GuiGraphicsComposition* value)
		{
		}

		void SetText(const WString& value)
		{
		}

		void SetFont(const FontProperties& value)
		{
		}

		void SetVisuallyEnabled(bool value)
		{
		}
	};
public:
	NestlePage()
		:GuiControl(new StyleController)
	{
	}
};

/***********************************************************************
Login Page
***********************************************************************/

class LoginPage : public NestlePage
{
protected:
	GuiSinglelineTextBox*				textUsername;
	GuiSinglelineTextBox*				textPassword;
	GuiButton*							buttonLogin;
	GuiButton*							buttonCancel;
	Ptr<NestleServer>					nestleServer;

	void buttonLogin_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
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

	void buttonCancel_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		Canceled.Execute(arguments);
	}
protected:
	void InitializeComponents()
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
			buttonLogin->GetBoundsComposition()->SetPreferredMinSize(Size(80, 0));
			buttonLogin->SetText(L"强势登录鸟窝");
			buttonLogin->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			buttonLogin->Clicked.AttachMethod(this, &LoginPage::buttonLogin_Clicked);
			cell->AddChild(buttonLogin->GetBoundsComposition());
		}
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(3, 4, 1, 1);

			buttonCancel=g::NewButton();
			buttonCancel->GetBoundsComposition()->SetPreferredMinSize(Size(80, 0));
			buttonCancel->SetText(L"我呸！");
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
public:
	GuiNotifyEvent						Logined;
	GuiNotifyEvent						Canceled;

	Ptr<NestleServer> CreatedNestleServer()
	{
		return nestleServer;
	}

	LoginPage()
	{
		Logined.SetAssociatedComposition(GetBoundsComposition());
		Canceled.SetAssociatedComposition(GetBoundsComposition());
		InitializeComponents();
	}
};

/***********************************************************************
Topic List Page
***********************************************************************/

class TopicListPage : public NestlePage
{
protected:

	class ITopicItemView : public IDescriptable
	{
	public:
		static const wchar_t* const		Identifier;

		virtual Ptr<NestlePost>			GetPost(int itemIndex)=0;
	};

	//--------------------------------------------------------------

	class TopicItemProvider : public list::ItemProviderBase, private ITopicItemView
	{
	protected:
		Ptr<NestleTopicsPage>			topicsPage;

	private:
		Ptr<NestlePost> GetPost(int itemIndex)
		{
			return topicsPage->posts[itemIndex];
		}
	public:
		TopicItemProvider(Ptr<NestleTopicsPage> _topicsPage)
			:topicsPage(_topicsPage)
		{
		}

		int Count()
		{
			return topicsPage->posts.Count();
		}

		IDescriptable* RequestView(const WString& identifier)
		{
			if(identifier==ITopicItemView::Identifier)
			{
				return (ITopicItemView*)this;
			}
			else
			{
				return 0;
			}
		}

		void ReleaseView(IDescriptable* view)
		{
		}
	};

	//-------------------------------------------------------------

	class TopicItemStyleController : public list::ItemStyleControllerBase
	{
	protected:
		GuiSolidLabelElement*					titleElement;
		GuiSolidLabelElement*					authorElement;
		GuiSolidLabelElement*					dateTimeElement;
		GuiSolidLabelElement*					descriptionElement;

		GuiSelectableButton*					backgroundButton;
		GuiLabel*								labelRead;
		Ptr<NestlePost>							post;

		void labelRead_OnMouseEnter(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			FontProperties font=labelRead->GetFont();
			font.underline=true;
			labelRead->SetFont(font);
		}

		void labelRead_OnMouseLeave(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			FontProperties font=labelRead->GetFont();
			font.underline=false;
			labelRead->SetFont(font);
		}

		void labelRead_OnLeftButtonDown(GuiGraphicsComposition* sender, GuiMouseEventArgs& arguments)
		{
		}
	protected:
		void InitializeComponents()
		{
			backgroundButton=new GuiSelectableButton(GetCurrentTheme()->CreateListItemBackgroundStyle());
			backgroundButton->SetAutoSelection(false);

			FontProperties titleFont=backgroundButton->GetFont();
			titleFont.size=24;
			titleFont.bold=true;
			FontProperties bodyFont=backgroundButton->GetFont();
			FontProperties buttonFont=backgroundButton->GetFont();
			titleFont.size=18;
			titleFont.bold=true;

			GuiTableComposition* table=new GuiTableComposition;
			backgroundButton->GetContainerComposition()->AddChild(table);
			table->SetAlignmentToParent(Margin(4, 4, 4, 4));
			table->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			table->SetRowsAndColumns(4, 4);
			table->SetCellPadding(4);
			table->SetRowOption(0, GuiCellOption::MinSizeOption());
			table->SetRowOption(1, GuiCellOption::MinSizeOption());
			table->SetRowOption(2, GuiCellOption::AbsoluteOption(52));
			table->SetRowOption(3, GuiCellOption::MinSizeOption());
			table->SetColumnOption(0, GuiCellOption::MinSizeOption());
			table->SetColumnOption(1, GuiCellOption::PercentageOption(0.5));
			table->SetColumnOption(2, GuiCellOption::MinSizeOption());
			table->SetColumnOption(3, GuiCellOption::PercentageOption(0.5));
			{
				GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
				element->SetFont(titleFont);
				element->SetEllipse(true);
				element->SetText(L"Title");
				element->SetColor(Color(63, 72, 204));
				titleElement=element;

				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetOwnedElement(element);
				cell->SetSite(0, 0, 1, 4);
			}
			{
				GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
				element->SetFont(bodyFont);
				element->SetText(L"Po主：");
				element->SetColor(Color(127, 127, 127));

				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetOwnedElement(element);
				cell->SetSite(1, 0, 1, 1);
			}
			{
				GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
				element->SetFont(bodyFont);
				element->SetText(L"Author");
				element->SetColor(Color(64, 64, 64));
				authorElement=element;

				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetOwnedElement(element);
				cell->SetSite(1, 1, 1, 1);
			}
			{
				GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
				element->SetFont(bodyFont);
				element->SetText(L"时间：");
				element->SetColor(Color(127, 127, 127));

				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetOwnedElement(element);
				cell->SetSite(1, 2, 1, 1);
			}
			{
				GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
				element->SetFont(bodyFont);
				element->SetText(L"DateTime");
				element->SetColor(Color(64, 64, 64));
				dateTimeElement=element;

				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetOwnedElement(element);
				cell->SetSite(1, 3, 1, 1);
			}
			{
				GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
				element->SetFont(bodyFont);
				element->SetWrapLine(true);
				element->SetEllipse(true);
				element->SetText(L"Description");
				element->SetColor(Color(64, 64, 64));
				descriptionElement=element;

				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetOwnedElement(element);
				cell->SetSite(2, 0, 1, 4);
			}
			{
				labelRead=g::NewLabel();
				labelRead->SetFont(buttonFont);
				labelRead->SetTextColor(Color(237, 28, 36));
				labelRead->SetText(L"强势插入");
				labelRead->GetBoundsComposition()->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetSystemCursor(INativeCursor::Hand));
				labelRead->GetBoundsComposition()->SetAlignmentToParent(Margin(-1, 2, 2, 2));
				labelRead->GetEventReceiver()->mouseEnter.AttachMethod(this, &TopicItemStyleController::labelRead_OnMouseEnter);
				labelRead->GetEventReceiver()->mouseLeave.AttachMethod(this, &TopicItemStyleController::labelRead_OnMouseLeave);
				labelRead->GetEventReceiver()->leftButtonDown.AttachMethod(this, &TopicItemStyleController::labelRead_OnLeftButtonDown);

				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->AddChild(labelRead->GetBoundsComposition());
				cell->SetSite(3, 0, 1, 4);
			}
		}
	public:
		TopicItemStyleController(GuiListControl::IItemStyleProvider* _styleProvider)
			:ItemStyleControllerBase(_styleProvider, 0)
		{
			InitializeComponents();
			Initialize(backgroundButton->GetBoundsComposition(), backgroundButton);
		}

		void SetPost(Ptr<NestlePost> _post)
		{
			post=_post;
			titleElement->SetText(post->title);
			authorElement->SetText(post->author);
			dateTimeElement->SetText(post->createDateTime);
			descriptionElement->SetText(post->description);
		}

		void SetSelected(bool value)
		{
			backgroundButton->SetSelected(value);
		}
	};

	//--------------------------------------------------------------

	class TopicItemStyleProvider : public Object, public GuiSelectableListControl::IItemStyleProvider
	{
	protected:
		ITopicItemView*					topicItemView;
	public:
		TopicItemStyleProvider()
			:topicItemView(0)
		{
		}
		
		void AttachListControl(GuiListControl* value)
		{
			topicItemView=dynamic_cast<ITopicItemView*>(value->GetItemProvider()->RequestView(ITopicItemView::Identifier));
		}

		void DetachListControl()
		{
		}

		int GetItemStyleId(int itemIndex)
		{
			return 0;
		}

		GuiListControl::IItemStyleController* CreateItemStyle(int styleId)
		{
			return new TopicItemStyleController(this);
		}

		void DestroyItemStyle(GuiListControl::IItemStyleController* style)
		{
			delete dynamic_cast<TopicItemStyleController*>(style);
		}

		void Install(GuiListControl::IItemStyleController* style, int itemIndex)
		{
			TopicItemStyleController* topicStyle=dynamic_cast<TopicItemStyleController*>(style);
			Ptr<NestlePost> post=topicItemView->GetPost(itemIndex);
			topicStyle->SetPost(post);
		}

		void SetStyleSelected(GuiListControl::IItemStyleController* style, bool value)
		{
			TopicItemStyleController* topicStyle=dynamic_cast<TopicItemStyleController*>(style);
			topicStyle->SetSelected(value);
		}
	};
protected:
	Ptr<NestleServer>					nestleServer;
	GuiSelectableListControl*			topicList;

public:
	TopicListPage(Ptr<NestleServer> _nestleServer)
		:nestleServer(_nestleServer)
		,topicList(0)
	{
		GetBoundsComposition()->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetSystemCursor(INativeCursor::LargeWaiting));
		GetApplication()->InvokeAsync([=]()
		{
			Ptr<NestleTopicsPage> page=nestleServer->GetTopics(0);

			GetApplication()->InvokeInMainThreadAndWait([=]()
			{
				TopicItemProvider* itemProvider=new TopicItemProvider(page);
				topicList=new GuiSelectableListControl(GetCurrentTheme()->CreateTextListStyle(), itemProvider);
				topicList->SetArranger(new list::FixedHeightItemArranger);
				topicList->SetStyleProvider(new TopicItemStyleProvider);
				topicList->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				topicList->SetHorizontalAlwaysVisible(false);
				GetContainerComposition()->AddChild(topicList->GetBoundsComposition());
				GetBoundsComposition()->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetDefaultSystemCursor());
			});
		});
	}
};

const wchar_t* const TopicListPage::ITopicItemView::Identifier=L"VCZH-NESTLE-GACUI-CLIENT-TopicListPage::ITopicItemView";

/***********************************************************************
Main Window
***********************************************************************/

class NestleWindow : public GuiWindow
{
protected:
	GuiBoundsComposition*			pageContainerComposition;
	Ptr<NestlePage>					currentPage;
	List<Ptr<NestlePage>>			availableHistoryPages;

	void PageInstall(Ptr<NestlePage> newPage)
	{
		currentPage=newPage;
		pageContainerComposition->AddChild(newPage->GetBoundsComposition());
	}

	Ptr<NestlePage> PageUninstall()
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

	void PageForward(Ptr<NestlePage> newPage)
	{
		Ptr<NestlePage> page=PageUninstall();
		if(page)
		{
			availableHistoryPages.Add(page);
		}
		PageInstall(newPage);
	}

	Ptr<NestlePage> PageBackward()
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

	void PageClear()
	{
		availableHistoryPages.Clear();
		PageBackward();
	}

	void DelayDelete(Ptr<NestlePage> page)
	{
		GetApplication()->InvokeInMainThread([page](){});
	}

	//--------------------------------------------------------------

	void LoginPage_Logined(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		Ptr<LoginPage> loginPage=currentPage.Cast<LoginPage>();
		if(loginPage)
		{
			Ptr<NestleServer> server=loginPage->CreatedNestleServer();
			PageClear();
			PageForward(new TopicListPage(server));
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

	void LoginPage_Canceled_InitialLogin(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		Close();
	}

	void LoginPage_Canceled_NonInitialLogin(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		PageBackward();
	}

	void ShowLoginPage(bool initialLogin)
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
protected:
	void InitializeComponents()
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
			element->SetColor(Color(255, 255, 255, 128));

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
			element->SetText(L"算法就是构造一个数据结构，把数据插入到数据结构的指定位置 ---- @Kula");

			GuiBoundsComposition* composition=new GuiBoundsComposition();
			composition->SetOwnedElement(element);
			composition->SetAlignmentToParent(Margin(2, -1, 2, 2));
			composition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
			GetContainerComposition()->AddChild(composition);
		}
	}
public:
	//
	NestleWindow()
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

	~NestleWindow()
	{
		PageClear();
	}
};

/***********************************************************************
GuiMain
***********************************************************************/

void GuiMain()
{
	{
		FontProperties font=GetCurrentController()->ResourceService()->GetDefaultFont();
		font.size=16;
		GetCurrentController()->ResourceService()->SetDefaultFont(font);
	}
	NestleWindow window;
	GetApplication()->Run(&window);
}