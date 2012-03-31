/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUIBASICCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUIBASICCONTROLS

#include "..\GraphicsElement\GuiGraphicsHost.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
Basic Construction
***********************************************************************/

			class GuiControl : public Object, public Description<GuiControl>
			{
				friend class compositions::GuiGraphicsComposition;
				typedef collections::List<GuiControl*>		ControlList;
			public:
				class IStyleController : public virtual IDescriptable, public Description<IStyleController>
				{
				public:
					virtual compositions::GuiBoundsComposition*	GetBoundsComposition()=0;
					virtual compositions::GuiGraphicsComposition*	GetContainerComposition()=0;
					virtual void								SetFocusableComposition(compositions::GuiGraphicsComposition* value)=0;
					virtual void								SetText(const WString& value)=0;
					virtual void								SetFont(const FontProperties& value)=0;
					virtual void								SetVisuallyEnabled(bool value)=0;
				};

				class IStyleProvider : public virtual IDescriptable, public Description<IStyleProvider>
				{
				public:
					virtual void								AssociateStyleController(IStyleController* controller)=0;
					virtual void								SetFocusableComposition(compositions::GuiGraphicsComposition* value)=0;
					virtual void								SetText(const WString& value)=0;
					virtual void								SetFont(const FontProperties& value)=0;
					virtual void								SetVisuallyEnabled(bool value)=0;
				};
			protected:
				Ptr<IStyleController>							styleController;
				compositions::GuiBoundsComposition*				boundsComposition;
				compositions::GuiGraphicsComposition*			focusableComposition;
				compositions::GuiGraphicsEventReceiver*			eventReceiver;

				bool									isEnabled;
				bool									isVisuallyEnabled;
				bool									isVisible;
				WString									text;
				FontProperties							font;

				GuiControl*								parent;
				ControlList								children;

				virtual void							OnChildInserted(GuiControl* control);
				virtual void							OnChildRemoved(GuiControl* control);
				virtual void							OnParentChanged(GuiControl* oldParent, GuiControl* newParent);
				virtual void							OnParentLineChanged();
				virtual void							OnRenderTargetChanged(elements::IGuiGraphicsRenderTarget* renderTarget);
				virtual void							OnBeforeReleaseGraphicsHost();
				virtual void							UpdateVisuallyEnabled();
				void									SetFocusableComposition(compositions::GuiGraphicsComposition* value);
			public:
				GuiControl(IStyleController* _styleController);
				~GuiControl();

				compositions::GuiNotifyEvent			VisibleChanged;
				compositions::GuiNotifyEvent			EnabledChanged;
				compositions::GuiNotifyEvent			VisuallyEnabledChanged;
				compositions::GuiNotifyEvent			TextChanged;
				compositions::GuiNotifyEvent			FontChanged;

				compositions::GuiEventArgs				GetNotifyEventArguments();
				IStyleController*						GetStyleController();
				compositions::GuiBoundsComposition*		GetBoundsComposition();
				compositions::GuiGraphicsComposition*	GetContainerComposition();
				compositions::GuiGraphicsComposition*	GetFocusableComposition();
				compositions::GuiGraphicsEventReceiver*	GetEventReceiver();
				GuiControl*								GetParent();
				int										GetChildrenCount();
				GuiControl*								GetChild(int index);
				
				virtual GuiControlHost*					GetRelatedControlHost();
				virtual bool							GetVisuallyEnabled();
				virtual bool							GetEnabled();
				virtual void							SetEnabled(bool value);
				virtual bool							GetVisible();
				virtual void							SetVisible(bool value);
				virtual const WString&					GetText();
				virtual void							SetText(const WString& value);
				virtual const FontProperties&			GetFont();
				virtual void							SetFont(const FontProperties& value);

				virtual IDescriptable*					QueryService(const WString& identifier);

				template<typename T>
				T* QueryService()
				{
					return dynamic_cast<T*>(QueryService(T::Identifier));
				}
			};

			class GuiComponent : public Object, public Description<GuiComponent>
			{
				friend class GuiControlHost;
			private:
				GuiControlHost*							controlHost;

				void									SetControlHost(GuiControlHost* value);
			public:
				GuiComponent();
				~GuiComponent();

				GuiControlHost*							GetControlHost();
			};

			class GuiImageData
			{
			protected:
				Ptr<INativeImage>				image;
				int								frameIndex;

			public:
				GuiImageData();
				GuiImageData(Ptr<INativeImage> _image, int _frameIndex);
				~GuiImageData();

				Ptr<INativeImage>				GetImage();
				int								GetFrameIndex();
			};

/***********************************************************************
Buttons
***********************************************************************/

			class GuiButton : public GuiControl, public Description<GuiButton>
			{
			public:
				enum ControlState
				{
					Normal,
					Active,
					Pressed,
				};

				class IStyleController : virtual public GuiControl::IStyleController, public Description<IStyleController>
				{
				public:
					virtual void						Transfer(ControlState value)=0;
				};
			protected:
				IStyleController*						styleController;
				bool									mousePressing;
				bool									mouseHoving;
				ControlState							controlState;
				
				void									OnParentLineChanged()override;
				void									UpdateControlState();
				void									OnLeftButtonDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
				void									OnLeftButtonUp(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
				void									OnMouseEnter(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void									OnMouseLeave(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
			public:
				GuiButton(IStyleController* _styleController);
				~GuiButton();

				compositions::GuiNotifyEvent			Clicked;
			};

			class GuiSelectableButton : public GuiButton, public Description<GuiSelectableButton>
			{
			public:
				class IStyleController : public virtual GuiButton::IStyleController, public Description<IStyleController>
				{
				public:
					virtual void						SetSelected(bool value)=0;
				};

				class GroupController : public GuiComponent, public Description<GroupController>
				{
				protected:
					collections::List<GuiSelectableButton*>	buttons;
				public:
					GroupController();
					~GroupController();

					virtual void						Attach(GuiSelectableButton* button);
					virtual void						Detach(GuiSelectableButton* button);
					virtual void						OnSelectedChanged(GuiSelectableButton* button)=0;
				};

				class MutexGroupController : public GroupController, public Description<MutexGroupController>
				{
				protected:
					bool								suppress;
				public:
					MutexGroupController();
					~MutexGroupController();

					void								OnSelectedChanged(GuiSelectableButton* button)override;
				};

			protected:
				IStyleController*						styleController;
				GroupController*						groupController;
				bool									autoSelection;
				bool									isSelected;

				void									OnClicked(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
			public:
				GuiSelectableButton(IStyleController* _styleController);
				~GuiSelectableButton();

				compositions::GuiNotifyEvent			GroupControllerChanged;
				compositions::GuiNotifyEvent			AutoSelectionChanged;
				compositions::GuiNotifyEvent			SelectedChanged;

				virtual GroupController*				GetGroupController();
				virtual void							SetGroupController(GroupController* value);

				virtual bool							GetAutoSelection();
				virtual void							SetAutoSelection(bool value);

				virtual bool							GetSelected();
				virtual void							SetSelected(bool value);
			};

/***********************************************************************
Scrolls
***********************************************************************/

			class GuiScroll : public GuiControl, public Description<GuiScroll>
			{
			public:
				class ICommandExecutor : public virtual IDescriptable, public Description<ICommandExecutor>
				{
				public:
					virtual void						SmallDecrease()=0;
					virtual void						SmallIncrease()=0;
					virtual void						BigDecrease()=0;
					virtual void						BigIncrease()=0;

					virtual void						SetTotalSize(int value)=0;
					virtual void						SetPageSize(int value)=0;
					virtual void						SetPosition(int value)=0;
				};

				class IStyleController : public virtual GuiControl::IStyleController, public Description<IStyleController>
				{
				public:
					virtual void						SetCommandExecutor(ICommandExecutor* value)=0;
					virtual void						SetTotalSize(int value)=0;
					virtual void						SetPageSize(int value)=0;
					virtual void						SetPosition(int value)=0;
				};
			protected:
				class CommandExecutor : public Object, public ICommandExecutor
				{
				protected:
					GuiScroll*							scroll;
				public:
					CommandExecutor(GuiScroll* _scroll);
					~CommandExecutor();

					void								SmallDecrease()override;
					void								SmallIncrease()override;
					void								BigDecrease()override;
					void								BigIncrease()override;

					void								SetTotalSize(int value)override;
					void								SetPageSize(int value)override;
					void								SetPosition(int value)override;
				};

				IStyleController*						styleController;
				Ptr<CommandExecutor>					commandExecutor;
				int										totalSize;
				int										pageSize;
				int										position;
				int										smallMove;
				int										bigMove;
			public:
				GuiScroll(IStyleController* _styleController);
				~GuiScroll();
				
				compositions::GuiNotifyEvent			TotalSizeChanged;
				compositions::GuiNotifyEvent			PageSizeChanged;
				compositions::GuiNotifyEvent			PositionChanged;
				compositions::GuiNotifyEvent			SmallMoveChanged;
				compositions::GuiNotifyEvent			BigMoveChanged;

				virtual int								GetTotalSize();
				virtual void							SetTotalSize(int value);
				virtual int								GetPageSize();
				virtual void							SetPageSize(int value);
				virtual int								GetPosition();
				virtual void							SetPosition(int value);
				virtual int								GetSmallMove();
				virtual void							SetSmallMove(int value);
				virtual int								GetBigMove();
				virtual void							SetBigMove(int value);

				int										GetMinPosition();
				int										GetMaxPosition();
			};

			class GuiScrollView : public GuiControl, public Description<GuiScrollView>
			{
			public:
				class IStyleProvider : public virtual GuiControl::IStyleProvider, public Description<IStyleProvider>
				{
				public:
					virtual GuiScroll::IStyleController*			CreateHorizontalScrollStyle()=0;
					virtual GuiScroll::IStyleController*			CreateVerticalScrollStyle()=0;
					virtual int										GetDefaultScrollSize()=0;
					virtual compositions::GuiGraphicsComposition*	InstallBackground(compositions::GuiBoundsComposition* boundsComposition)=0;
				};

				class StyleController : public Object, public GuiControl::IStyleController, public Description<StyleController>
				{
				protected:
					Ptr<IStyleProvider>						styleProvider;
					GuiScrollView*							scrollView;
					GuiScroll*								horizontalScroll;
					GuiScroll*								verticalScroll;
					compositions::GuiBoundsComposition*		boundsComposition;
					compositions::GuiTableComposition*		tableComposition;
					compositions::GuiCellComposition*		containerCellComposition;
					compositions::GuiBoundsComposition*		containerComposition;
					bool									horizontalAlwaysVisible;
					bool									verticalAlwaysVisible;

					void									UpdateTable();
				public:
					StyleController(IStyleProvider* _styleProvider);
					~StyleController();

					void									SetScrollView(GuiScrollView* _scrollView);
					void									AdjustView(Size fullSize);
					IStyleProvider*							GetStyleProvider();

					GuiScroll*								GetHorizontalScroll();
					GuiScroll*								GetVerticalScroll();
					compositions::GuiTableComposition*		GetInternalTableComposition();
					compositions::GuiBoundsComposition*		GetInternalContainerComposition();

					bool									GetHorizontalAlwaysVisible();
					void									SetHorizontalAlwaysVisible(bool value);
					bool									GetVerticalAlwaysVisible();
					void									SetVerticalAlwaysVisible(bool value);

					compositions::GuiBoundsComposition*		GetBoundsComposition()override;
					compositions::GuiGraphicsComposition*	GetContainerComposition()override;
					void									SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
					void									SetText(const WString& value)override;
					void									SetFont(const FontProperties& value)override;
					void									SetVisuallyEnabled(bool value)override;
				};
			protected:

				StyleController*						styleController;
				bool									supressScrolling;

				void									OnContainerBoundsChanged(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void									OnHorizontalScroll(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void									OnVerticalScroll(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void									CallUpdateView();
				void									Initialize();

				virtual Size							QueryFullSize()=0;
				virtual void							UpdateView(Rect viewBounds)=0;

				GuiScrollView(StyleController* _styleController);
			public:
				GuiScrollView(IStyleProvider* styleProvider);
				~GuiScrollView();

				void									CalculateView();
				Size									GetViewSize();
				Rect									GetViewBounds();

				GuiScroll*								GetHorizontalScroll();
				GuiScroll*								GetVerticalScroll();
				bool									GetHorizontalAlwaysVisible();
				void									SetHorizontalAlwaysVisible(bool value);
				bool									GetVerticalAlwaysVisible();
				void									SetVerticalAlwaysVisible(bool value);
			};

			class GuiScrollContainer : public GuiScrollView, public Description<GuiScrollContainer>
			{
			public:
				class StyleController : public GuiScrollView::StyleController, public Description<StyleController>
				{
				protected:
					compositions::GuiBoundsComposition*		controlContainerComposition;
				public:
					StyleController(GuiScrollView::IStyleProvider* styleProvider);
					~StyleController();

					compositions::GuiGraphicsComposition*	GetContainerComposition()override;
					void									MoveContainer(Point leftTop);
				};

			protected:
				StyleController*						styleController;

				void									OnControlContainerBoundsChanged(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				Size									QueryFullSize()override;
				void									UpdateView(Rect viewBounds)override;
			public:
				GuiScrollContainer(GuiScrollContainer::IStyleProvider* styleProvider);
				~GuiScrollContainer();
			};
		}
	}
}

#endif