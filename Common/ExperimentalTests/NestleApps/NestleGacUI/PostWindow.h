#ifndef VCZH_NESTLECLIENT_WINDOWS_POSTWINDOW
#define VCZH_NESTLECLIENT_WINDOWS_POSTWINDOW

#include "..\..\..\..\Libraries\GacUI\Public\Source\GacUIIncludes.h"
#include "..\NestleConsole\NestleSDK.h"
#include "CustomizableListBoxHelper.h"

namespace vl
{
	namespace nestle
	{
		class PostItem : public Object
		{
		public:
			WString								title;
			WString								author;
			WString								createDateTime;
			WString								body;
			bool								isComment;
			int									id;
		};

		class PostItemControl : public list::ObjectItemControl
		{
		protected:
			GuiSolidLabelElement*				titleElement;
			GuiSolidLabelElement*				authorElement;
			GuiSolidLabelElement*				dateTimeElement;
			GuiSolidLabelElement*				bodyElement;

			GuiButton*							buttonReply;
			GuiButton*							buttonEdit;
			GuiButton*							buttonDelete;
			Ptr<PostItem>						postItem;

			void buttonReply_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void buttonEdit_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void buttonDelete_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
		protected:
			void InitializeComponents();
		public:
			PostItemControl();

			void								Install(Ptr<Object> value)override;
		};

		class PostWindow : public GuiWindow
		{
		protected:
			Ptr<NestleServer>					server;
			Ptr<NestlePost>						post;

			GuiScrollContainer*					postItemContainers;
			GuiStackComposition*				postItemStack;
			List<PostItemControl*>				postItemControls;
			GuiMultilineTextBox*				textBody;
			GuiButton*							buttonPost;
			GuiButton*							buttonUploadPicture;
			GuiButton*							buttonCancel;

			void								buttonPost_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void								buttonUploadPicture_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);
			void								buttonCancel_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments);

			void								ClearPostItems();
			void								AddPostItem(Ptr<PostItem> postItem);
		protected:
			void								InitializeComponents();
		public:
			PostWindow(Ptr<NestleServer> _server, Ptr<NestlePost> _post);
			~PostWindow();

			void								RefreshPostItems();
			void								Reply(const WString& author);
			Ptr<NestleServer>					GetServer();
			bool								IsCurrentUser(const WString& author);
		};
	}
}

#endif