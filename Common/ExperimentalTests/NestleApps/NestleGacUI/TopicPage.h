#ifndef NESTLEGACUI_PAGES_TOPICPAGE
#define NESTLEGACUI_PAGES_TOPICPAGE

#include "NestlePage.h"

class TopicPage : public NestlePage
{
protected:
	Ptr<NestleServer>					nestleServer;
	Ptr<NestlePost>						post;

	GuiMultilineTextBox*				textPost;
	GuiListControl*						listComments;
	GuiMultilineTextBox*				textComment;
	GuiButton*							buttonComment;
	GuiButton*							buttonBack;
	GuiGraphicsComposition*				listCommentsContainer;

	void								RefreshCommentList();
	void								buttonComment_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
	void								buttonBack_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
protected:
	void								InitializeComponents();
public:
	TopicPage(Ptr<NestleServer> _nestleServer, Ptr<NestlePost> _post);

	GuiNotifyEvent						PostCloseRequested;
};

#endif