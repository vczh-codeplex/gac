#include "TopicPage.h"

/***********************************************************************
TopicPage
***********************************************************************/

TopicPage::TopicPage(Ptr<NestleServer> _nestleServer, Ptr<NestlePost> _post)
	:nestleServer(_nestleServer)
	,post(_post)
{
	InitializeComponents();
}

/***********************************************************************
TopicPage::InitializeComponents()
***********************************************************************/

void TopicPage::InitializeComponents()
{
}