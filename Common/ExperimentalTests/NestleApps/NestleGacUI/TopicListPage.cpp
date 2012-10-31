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
	GuiLabel*								labelRead;
	Ptr<NestlePost>							post;
	TopicListPage*							topicListPage;

	void backgroundControl_OnMouseEnter(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		backgroundElement->SetColor(Color(126, 180, 234, 96));
	}

	void backgroundControl_OnMouseLeave(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		backgroundElement->SetColor(Color(0, 0, 0, 0));
	}

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
		topicListPage->InvokePostOpenRequested(post);
	}
protected:
	void InitializeComponents()
	{
		backgroundElement=GuiSolidBackgroundElement::Create();
		backgroundElement->SetColor(Color(0, 0, 0, 0));
		backgroundControl=new GuiControl(new ContainerControlStyle);
		backgroundControl->GetBoundsComposition()->SetOwnedElement(backgroundElement);
		backgroundControl->GetEventReceiver()->mouseEnter.AttachMethod(this, &TopicItemStyleController::backgroundControl_OnMouseEnter);
		backgroundControl->GetEventReceiver()->mouseLeave.AttachMethod(this, &TopicItemStyleController::backgroundControl_OnMouseLeave);

		FontProperties titleFont=backgroundControl->GetFont();
		titleFont.size=30;
		titleFont.bold=true;
		FontProperties bodyFont=backgroundControl->GetFont();
		bodyFont.size=18;
		FontProperties buttonFont=backgroundControl->GetFont();
		titleFont.size=24;

		Color titleColor(63, 72, 204);
		Color frameColor(136, 0, 21);
		Color bodyColor(64, 64, 64);
		Color buttonColor(191, 15, 23);

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
		table->SetColumnOption(3, GuiCellOption::PercentageOption(0.5));
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
			element->SetText(L"Po主：");
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
			element->SetText(L"时间：");
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
			GuiBoundsComposition* descriptionComposition;
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
			element->SetColor(frameColor);

			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetOwnedElement(element);
			cell->AddChild(descriptionComposition);
			cell->SetSite(2, 0, 1, 4);
		}
		{
			labelRead=g::NewLabel();
			labelRead->SetFont(buttonFont);
			labelRead->SetTextColor(buttonColor);
			labelRead->SetText(L"强势插入");
			labelRead->GetBoundsComposition()->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetSystemCursor(INativeCursor::Hand));
			labelRead->GetBoundsComposition()->SetAlignmentToParent(Margin(2, 2, 2, 5));
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
	TopicItemStyleController(GuiListControl::IItemStyleProvider* _styleProvider, TopicListPage* _topicListPage)
		:ItemStyleControllerBase(_styleProvider, 0)
		,topicListPage(_topicListPage)
	{
		InitializeComponents();
		Initialize(backgroundControl->GetBoundsComposition(), backgroundControl);
	}

	void SetPost(Ptr<NestlePost> _post)
	{
		post=_post;
		titleElement->SetText(post->title);
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
		return 0;
	}

	GuiListControl::IItemStyleController* CreateItemStyle(int styleId)
	{
		return new TopicItemStyleController(this, topicListPage);
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