#ifndef NESTLEGACUI_PAGES_TOPICLISTPAGE
#define NESTLEGACUI_PAGES_TOPICLISTPAGE

#include "NestlePage.h"

class TopicListPage : public NestlePage
{
protected:
	Ptr<NestleServer>					nestleServer;
	GuiListControl*						topicList;
	Ptr<NestlePost>						selectedPost;

public:
	TopicListPage(Ptr<NestleServer> _nestleServer);

	GuiNotifyEvent						PostOpenRequested;

	Ptr<NestleServer>					GetServer();
	Ptr<NestlePost>						GetSelectedPost();
	void								InvokePostOpenRequested(Ptr<NestlePost> post);
};

#endif