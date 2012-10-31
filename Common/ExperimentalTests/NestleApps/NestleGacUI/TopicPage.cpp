#include "TopicPage.h"

/***********************************************************************
ICommentItemView
***********************************************************************/

class ICommentItemView : public IDescriptable
{
public:
	static const wchar_t* const		Identifier;

	virtual Ptr<NestleComment>		GetComment(int itemIndex)=0;
};

const wchar_t* const ICommentItemView::Identifier=L"VCZH-NESTLE-GACUI-CLIENT-ICommentItemView";

/***********************************************************************
CommentItemProvider
***********************************************************************/

class CommentItemProvider : public list::ItemProviderBase, private ICommentItemView
{
protected:
	Ptr<NestlePost>					post;

private:
	Ptr<NestleComment> GetComment(int itemIndex)
	{
		return post->comments[itemIndex];
	}
public:
	CommentItemProvider(Ptr<NestlePost> _post)
		:post(_post)
	{
	}

	int Count()
	{
		return post->comments.Count();
	}

	IDescriptable* RequestView(const WString& identifier)
	{
		if(identifier==ICommentItemView::Identifier)
		{
			return (ICommentItemView*)this;
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
CommentItemStyleController
***********************************************************************/

class CommentItemStyleController : public list::ItemStyleControllerBase
{
protected:
	GuiSolidLabelElement*					authorElement;
	GuiSolidLabelElement*					dateTimeElement;
	GuiSolidLabelElement*					contentElement;
	GuiSolidBackgroundElement*				backgroundElement;

	GuiControl*								backgroundControl;
	GuiButton*								buttonRead;
	Ptr<NestleComment>						comment;
	TopicPage*								topicPage;

protected:
	void InitializeComponents()
	{
		backgroundElement=GuiSolidBackgroundElement::Create();
		backgroundElement->SetColor(Color(255, 255, 255, 192));
		backgroundControl=new GuiControl(new ContainerControlStyle);
		{
			GuiBoundsComposition* composition=new GuiBoundsComposition;
			composition->SetOwnedElement(backgroundElement);
			composition->SetAlignmentToParent(Margin(0, 0, 10, 10));
			backgroundControl->GetBoundsComposition()->AddChild(composition);
		}

		FontProperties titleFont=backgroundControl->GetFont();
		titleFont.size=28;
		titleFont.bold=true;
		FontProperties bodyFont=backgroundControl->GetFont();
		bodyFont.size=16;
		FontProperties buttonFont=backgroundControl->GetFont();
		titleFont.size=22;

		Color titleColor(0, 0, 0);
		Color frameColor(128, 128, 128);
		Color bodyColor(64, 64, 64);

		GuiTableComposition* table=new GuiTableComposition;
		backgroundControl->GetContainerComposition()->AddChild(table);
		table->SetAlignmentToParent(Margin(30, 4, 30, 4));
		table->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
		table->SetRowsAndColumns(3, 4);
		table->SetCellPadding(4);
		table->SetRowOption(0, GuiCellOption::MinSizeOption());
		table->SetRowOption(1, GuiCellOption::AbsoluteOption(60));
		table->SetRowOption(2, GuiCellOption::AbsoluteOption(10));
		table->SetColumnOption(0, GuiCellOption::MinSizeOption());
		table->SetColumnOption(1, GuiCellOption::PercentageOption(0.5));
		table->SetColumnOption(2, GuiCellOption::MinSizeOption());
		table->SetColumnOption(3, GuiCellOption::MinSizeOption());
		{
			GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
			element->SetFont(bodyFont);
			element->SetText(L"◇ Po主：");
			element->SetColor(frameColor);

			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetOwnedElement(element);
			cell->SetSite(0, 0, 1, 1);
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
			cell->SetSite(0, 1, 1, 1);
		}
		{
			GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
			element->SetFont(bodyFont);
			element->SetText(L"◇ 时间：");
			element->SetColor(frameColor);

			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetOwnedElement(element);
			cell->SetSite(0, 2, 1, 1);
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
			cell->SetSite(0, 3, 1, 1);
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
				contentElement=element;

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
			cell->SetSite(1, 0, 1, 4);
		}
	}
public:
	CommentItemStyleController(GuiListControl::IItemStyleProvider* _styleProvider, TopicPage* _topicPage)
		:ItemStyleControllerBase(_styleProvider, 0)
		,topicPage(_topicPage)
	{
		InitializeComponents();
		Initialize(backgroundControl->GetBoundsComposition(), backgroundControl);
	}

	void SetComment(Ptr<NestleComment> _comment)
	{
		comment=_comment;
		authorElement->SetText(comment->author);
		dateTimeElement->SetText(comment->createDateTime);
		contentElement->SetText(L"    "+comment->body);
	}
};

/***********************************************************************
CommentItemStyleProvider
***********************************************************************/

class CommentItemStyleProvider : public Object, public GuiListControl::IItemStyleProvider
{
protected:
	ICommentItemView*				commentItemView;
	TopicPage*						topicPage;
public:
	CommentItemStyleProvider(TopicPage* _topicPage)
		:commentItemView(0)
		,topicPage(_topicPage)
	{
	}
		
	void AttachListControl(GuiListControl* value)
	{
		commentItemView=dynamic_cast<ICommentItemView*>(value->GetItemProvider()->RequestView(ICommentItemView::Identifier));
	}

	void DetachListControl()
	{
	}

	int GetItemStyleId(int itemIndex)
	{
		return itemIndex;
	}

	GuiListControl::IItemStyleController* CreateItemStyle(int styleId)
	{
		return new CommentItemStyleController(this, topicPage);
	}

	void DestroyItemStyle(GuiListControl::IItemStyleController* style)
	{
		delete dynamic_cast<CommentItemStyleController*>(style);
	}

	void Install(GuiListControl::IItemStyleController* style, int itemIndex)
	{
		CommentItemStyleController* commentStyle=dynamic_cast<CommentItemStyleController*>(style);
		Ptr<NestleComment> comment=commentItemView->GetComment(itemIndex);
		commentStyle->SetComment(comment);
	}
};

/***********************************************************************
TopicPage
***********************************************************************/

TopicPage::TopicPage(Ptr<NestleServer> _nestleServer, Ptr<NestlePost> _post)
	:nestleServer(_nestleServer)
{
	GetBoundsComposition()->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetSystemCursor(INativeCursor::LargeWaiting));
	GetApplication()->InvokeAsync([=]()
	{
		post=nestleServer->GetTopic(_post->id);

		GetApplication()->InvokeInMainThreadAndWait([=]()
		{
			InitializeComponents();
			GetBoundsComposition()->SetAssociatedCursor(GetCurrentController()->ResourceService()->GetDefaultSystemCursor());
		});
	});
}

/***********************************************************************
TopicPage::InitializeComponents()
***********************************************************************/

void TopicPage::InitializeComponents()
{
	GuiTableComposition* table=new GuiTableComposition;
	GetContainerComposition()->AddChild(table);
	table->SetAlignmentToParent(Margin(0, 0, 0, 0));
	table->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	table->SetRowsAndColumns(3, 4);
	table->SetCellPadding(10);
	table->SetRowOption(0, GuiCellOption::PercentageOption(1.0));
	table->SetRowOption(1, GuiCellOption::AbsoluteOption(100));
	table->SetRowOption(2, GuiCellOption::MinSizeOption());
	table->SetColumnOption(0, GuiCellOption::MinSizeOption());
	table->SetColumnOption(1, GuiCellOption::PercentageOption(0.5));
	table->SetColumnOption(2, GuiCellOption::PercentageOption(0.5));
	table->SetColumnOption(3, GuiCellOption::MinSizeOption());
	{
		textPost=g::NewMultilineTextBox();
		textPost->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		textPost->SetReadonly(false);
		textPost->SetText(post->body);

		GuiCellComposition* cell=new GuiCellComposition;
		table->AddChild(cell);
		cell->SetSite(0, 0, 1, 2);
		cell->AddChild(textPost->GetBoundsComposition());
	}
	{
		CommentItemProvider* itemProvider=new CommentItemProvider(post);
		listComments=new GuiListControl(new TransparentListBoxStyle(), itemProvider);
		listComments->SetArranger(new list::FixedHeightItemArranger);
		listComments->SetStyleProvider(new CommentItemStyleProvider(this));
		listComments->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		listComments->SetHorizontalAlwaysVisible(false);

		GuiCellComposition* cell=new GuiCellComposition;
		table->AddChild(cell);
		cell->SetSite(0, 2, 1, 2);
		cell->AddChild(listComments->GetBoundsComposition());
	}
	{
		textComment=g::NewMultilineTextBox();
		textComment->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));

		GuiCellComposition* cell=new GuiCellComposition;
		table->AddChild(cell);
		cell->SetSite(1, 0, 1, 4);
		cell->AddChild(textComment->GetBoundsComposition());
	}
	{
		buttonBack=g::NewButton();
		buttonBack->SetText(L"  Po主再见！  ");
		buttonBack->GetBoundsComposition()->SetPreferredMinSize(Size(0, 32));

		GuiCellComposition* cell=new GuiCellComposition;
		table->AddChild(cell);
		cell->SetSite(2, 0, 1, 1);
		cell->AddChild(buttonBack->GetBoundsComposition());
	}
	{
		buttonComment=g::NewButton();
		buttonComment->SetText(L"  我要一战！  ");
		buttonComment->GetBoundsComposition()->SetPreferredMinSize(Size(0, 32));

		GuiCellComposition* cell=new GuiCellComposition;
		table->AddChild(cell);
		cell->SetSite(2, 3, 1, 1);
		cell->AddChild(buttonComment->GetBoundsComposition());
	}
}