#ifndef NESTLEGACUI_PAGES_TOPICLISTPAGE
#define NESTLEGACUI_PAGES_TOPICLISTPAGE

#include "NestlePage.h"

/***********************************************************************
Topic List Page
***********************************************************************/

class TopicListPage : public NestlePage
{
protected:
	Ptr<NestleServer>					nestleServer;
	GuiSelectableListControl*			topicList;

public:
	TopicListPage(Ptr<NestleServer> _nestleServer);
};

#endif