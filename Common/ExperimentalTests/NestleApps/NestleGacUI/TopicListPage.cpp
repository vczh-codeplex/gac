#include "TopicListPage.h"

/***********************************************************************
ITopicItemView
***********************************************************************/

class ITopicItemView : public IDescriptable
{
public:
	static const wchar_t* const		Identifier;

	virtual Ptr<NestlePost>			GetPost(int itemIndex)=0;
};

const wchar_t* const ITopicItemView::Identifier=L"VCZH-NESTLE-GACUI-CLIENT-ITopicItemView";

/***********************************************************************
TopicItemProvider
***********************************************************************/

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

/***********************************************************************
TopicItemStyleController
***********************************************************************/

class TopicItemStyleController : public list::ItemStyleControllerBase
{
protected:
	GuiSolidLabelElement*					titleElement;
	GuiSolidLabelElement*					authorElement;
	GuiSolidLabelElement*					dateTimeElement;
	GuiSolidLabelElement*					descriptionElement;
	GuiSolidBackgroundElement*				backgroundElement;

	GuiControl*								backgroundControl;
	GuiButton*								buttonRead;
	Ptr<NestlePost>							post;
	TopicListPage*							topicListPage;

	void buttonRead_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		topicListPage->InvokePostOpenRequested(post);
	}
protected:
	void InitializeComponents()
	{
		backgroundElement=GuiSolidBackgroundElement::Create();
		if(GetItemStyleId()==1)
		{
			backgroundElement->SetColor(Color(126, 180, 234, 96));
		}
		else
		{
			backgroundElement->SetColor(Color(0, 0, 0, 0));
		}
		backgroundControl=new GuiControl(new ContainerControlStyle);
		backgroundControl->GetBoundsComposition()->SetOwnedElement(backgroundElement);

		FontProperties titleFont=backgroundControl->GetFont();
		titleFont.size=28;
		titleFont.bold=true;
		FontProperties bodyFont=backgroundControl->GetFont();
		bodyFont.size=16;
		FontProperties buttonFont=backgroundControl->GetFont();
		titleFont.size=22;

		Color titleColor(63, 72, 204);
		Color frameColor(128, 128, 128);
		Color bodyColor(64, 64, 64);

		GuiTableComposition* table=new GuiTableComposition;
		backgroundControl->GetContainerComposition()->AddChild(table);
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
			GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
			element->SetFont(bodyFont);
			element->SetWrapLine(true);
			element->SetEllipse(true);
			element->SetText(L"Description");
			element->SetColor(bodyColor);
			descriptionElement=element;

			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetOwnedElement(element);
			cell->SetInternalMargin(Margin(3, 3, 3, 3));
			cell->SetSite(2, 0, 1, 4);
		}
		{
			buttonRead=g::NewButton();
			buttonRead->SetFont(buttonFont);
			buttonRead->SetText(L"………………  强势♂插入  ………………");
			buttonRead->GetBoundsComposition()->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetSystemCursor(INativeCursor::Hand));
			buttonRead->GetBoundsComposition()->SetAlignmentToParent(Margin(2, 2, 2, 5));
			buttonRead->Clicked.AttachMethod(this, &TopicItemStyleController::buttonRead_Clicked);

			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->AddChild(buttonRead->GetBoundsComposition());
			cell->SetSite(3, 0, 1, 4);
		}
	}
public:
	TopicItemStyleController(GuiListControl::IItemStyleProvider* _styleProvider, TopicListPage* _topicListPage, int styleId)
		:ItemStyleControllerBase(_styleProvider, styleId)
		,topicListPage(_topicListPage)
	{
		InitializeComponents();
		Initialize(backgroundControl->GetBoundsComposition(), backgroundControl);
	}

	void SetPost(Ptr<NestlePost> _post)
	{
		post=_post;
		titleElement->SetText(L"◇ "+post->title);
		authorElement->SetText(post->author);
		dateTimeElement->SetText(post->createDateTime);
		descriptionElement->SetText(post->description);
	}
};

/***********************************************************************
TopicItemStyleProvider
***********************************************************************/

class TopicItemStyleProvider : public Object, public GuiListControl::IItemStyleProvider
{
protected:
	ITopicItemView*					topicItemView;
	TopicListPage*					topicListPage;
public:
	TopicItemStyleProvider(TopicListPage* _topicListPage)
		:topicItemView(0)
		,topicListPage(_topicListPage)
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
		return itemIndex%2;
	}

	GuiListControl::IItemStyleController* CreateItemStyle(int styleId)
	{
		return new TopicItemStyleController(this, topicListPage, styleId);
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
};

/***********************************************************************
TopicListPage
***********************************************************************/

TopicListPage::TopicListPage(Ptr<NestleServer> _nestleServer)
	:nestleServer(_nestleServer)
	,topicList(0)
{
	GetBoundsComposition()->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetSystemCursor(INativeCursor::LargeWaiting));
	PostOpenRequested.SetAssociatedComposition(GetBoundsComposition());
	GetApplication()->InvokeAsync([=]()
	{
		Ptr<NestleTopicsPage> page=nestleServer->GetTopics(0);

		GetApplication()->InvokeInMainThreadAndWait([=]()
		{
			TopicItemProvider* itemProvider=new TopicItemProvider(page);
			topicList=new GuiListControl(new TransparentListBoxStyle(), itemProvider);
			topicList->SetArranger(new list::FixedHeightItemArranger);
			topicList->SetStyleProvider(new TopicItemStyleProvider(this));
			topicList->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			topicList->SetHorizontalAlwaysVisible(false);
			GetContainerComposition()->AddChild(topicList->GetBoundsComposition());
			GetBoundsComposition()->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetDefaultSystemCursor());
		});
	});
}

Ptr<NestleServer> TopicListPage::GetServer()
{
	return nestleServer;
}

Ptr<NestlePost> TopicListPage::GetSelectedPost()
{
	return selectedPost;
}

void TopicListPage::InvokePostOpenRequested(Ptr<NestlePost> post)
{
	selectedPost=post;
	GuiEventArgs arguments;
	PostOpenRequested.Execute(arguments);
}