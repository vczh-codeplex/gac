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
protected:
	void								InitializeComponents();
public:
	TopicPage(Ptr<NestleServer> _nestleServer, Ptr<NestlePost> _post);
};

#endif