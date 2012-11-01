#ifndef VCZH_NESTLECLIENT_WINDOWS_MAINWINDOW
#define VCZH_NESTLECLIENT_WINDOWS_MAINWINDOW

#include "LoginWindow.h"
#include "NewPostWindow.h"
#include "CustomizableListBoxHelper.h"

namespace vl
{
	namespace nestle
	{
		class TopicItemControl : public list::ObjectItemControl
		{
		protected:
			GuiSolidLabelElement*				titleElement;
			GuiSolidLabelElement*				authorElement;
			GuiSolidLabelElement*				dateTimeElement;
			GuiSolidLabelElement*				descriptionElement;

			GuiButton*							buttonRead;
			Ptr<NestlePost>						post;

			void buttonRead_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
		protected:
			void InitializeComponents();
		public:
			TopicItemControl();

			void								Install(Ptr<Object> value)override;
		};

		class MainWindow : public GuiWindow
		{
		protected:
			GuiToolstripToolbar*						toolbar;
			Ptr<GuiToolstripCommand>					commandRefresh;
			Ptr<GuiToolstripCommand>					commandPrevious;
			Ptr<GuiToolstripCommand>					commandFirstPage;
			Ptr<GuiToolstripCommand>					commandNext;
			Ptr<GuiToolstripCommand>					commandNewPost;
			Ptr<GuiToolstripCommand>					commandSearch;
			Ptr<GuiToolstripCommand>					commandUser;
			Ptr<GuiToolstripCommand>					commandUserLogin;
			Ptr<GuiToolstripCommand>					commandUserLogout;
			
			GuiLabel*									labelInformation;
			GuiGraphicsComposition*						containerLogin;
			GuiButton*									buttonLogin;
			GuiGraphicsComposition*						containerTopicList;
			GuiListControlTemplate<TopicItemControl>*	listTopics;

			Ptr<NestleServer>							server;
			Ptr<NestleTopicsPage>						currentPage;
			Ptr<LoginWindow>							loginWindow;
			Ptr<NewPostWindow>							newPostWindow;

			void										SwitchToLogoutStatus();
			void										SwitchToLoginStatus();
			void										EnableControls(bool enabled);
			void										LoadTopics(int pageIndex);

			void										loginWindow_Closed(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void										newPostWindow_Closed(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void										commandRefresh_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void										commandFirstPage_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void										commandPrevious_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void										commandNext_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void										commandNewPost_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void										commandSearch_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void										commandUserLogin_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void										commandUserLogout_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
		protected:
			void										InitializeComponents();
		public:
			MainWindow();
			~MainWindow();
		};
	}
}

#endif