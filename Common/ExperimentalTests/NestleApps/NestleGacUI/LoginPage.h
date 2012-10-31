#ifndef NESTLEGACUI_PAGES_LOGINPAGE
#define NESTLEGACUI_PAGES_LOGINPAGE

#include "NestlePage.h"

class LoginPage : public NestlePage
{
protected:
	GuiSinglelineTextBox*				textUsername;
	GuiSinglelineTextBox*				textPassword;
	GuiButton*							buttonLogin;
	GuiButton*							buttonCancel;
	Ptr<NestleServer>					nestleServer;

	void								buttonLogin_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
	void								buttonCancel_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
protected:
	void								InitializeComponents();
public:
	LoginPage();

	GuiNotifyEvent						Logined;
	GuiNotifyEvent						Canceled;

	Ptr<NestleServer>					CreatedNestleServer();
};

#endif