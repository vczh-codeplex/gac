#ifndef NESTLEGACUI_CONTROLS_NESTLEWINDOW
#define NESTLEGACUI_CONTROLS_NESTLEWINDOW

#include "NestlePage.h"

/***********************************************************************
Main Window
***********************************************************************/

class NestleWindow : public GuiWindow
{
protected:
	GuiBoundsComposition*			pageContainerComposition;
	Ptr<NestlePage>					currentPage;
	List<Ptr<NestlePage>>			availableHistoryPages;

	void							PageInstall(Ptr<NestlePage> newPage);
	Ptr<NestlePage>					PageUninstall();
	void							PageForward(Ptr<NestlePage> newPage);
	Ptr<NestlePage>					PageBackward();
	void							PageClear();
	void							DelayDelete(Ptr<NestlePage> page);

	void							LoginPage_Logined(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
	void							LoginPage_Canceled_InitialLogin(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
	void							LoginPage_Canceled_NonInitialLogin(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
	void							TopicListPage_PostOpenRequested(GuiGraphicsComposition* sender, GuiEventArgs& arguments);

	void							ShowLoginPage(bool initialLogin);
protected:
	void							InitializeComponents();
public:

	NestleWindow();
	~NestleWindow();
};

#endif