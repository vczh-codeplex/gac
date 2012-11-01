#ifndef VCZH_NESTLECLIENT_WINDOWS_MAINWINDOW
#define VCZH_NESTLECLIENT_WINDOWS_MAINWINDOW

#include "LoginWindow.h"

namespace vl
{
	namespace nestle
	{
		class MainWindow : public GuiWindow
		{
		protected:
			GuiToolstripToolbar*				toolbar;
			GuiToolstripCommand*				commandRefresh;
			GuiToolstripCommand*				commandPrevious;
			GuiToolstripCommand*				commandNext;
			GuiToolstripCommand*				commandNewPost;
			GuiToolstripCommand*				commandSearch;
			GuiToolstripCommand*				commandUser;
			GuiToolstripCommand*				commandUserLogin;
			GuiToolstripCommand*				commandUserLogout;

			GuiGraphicsComposition*				containerLogin;
			GuiButton*							buttonLogin;
			GuiGraphicsComposition*				containerTopicList;

			Ptr<NestleServer>					server;
			Ptr<LoginWindow>					loginWindow;

			void								SwitchToLogoutStatus();
			void								SwitchToLoginStatus();

			void								loginWindow_Closed(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void								commandRefresh_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void								commandPrevious_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void								commandNext_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void								commandNewPost_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void								commandSearch_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void								commandUserLogin_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void								commandUserLogout_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
		protected:
			void								InitializeComponents();
		public:
			MainWindow();
			~MainWindow();
		};
	}
}

#endif