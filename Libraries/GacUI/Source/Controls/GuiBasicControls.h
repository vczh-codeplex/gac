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

			/// <summary>
			/// The base class of all controls.
			/// When the control is destroyed, it automatically destroys sub controls, and the bounds composition from the style controller.
			/// If you want to manually destroy a control, you should first remove it from it's parent.
			/// The only way to remove a control from a parent control, is to remove the bounds composition from it's parent composition. The same to inserting a control.
			/// </summary>
			class GuiControl : public Object, public Description<GuiControl>
			{
				friend class compositions::GuiGraphicsComposition;
				typedef collections::List<GuiControl*>		ControlList;
			public:
				/// <summary>
				/// Represents a style for a control. A style is something like a skin, but contains some default action handlers.
				/// </summary>
				class IStyleController : public virtual IDescriptable, public Description<IStyleController>
				{
				public:
					/// <summary>Get the bounds composition. A bounds composition represents all visible contents of a control.</summary>
					/// <returns>The bounds composition.</returns>
					virtual compositions::GuiBoundsComposition*		GetBoundsComposition()=0;
					/// <summary>Get the container composition. A container composition is where to place all bounds compositions for child controls.</summary>
					/// <returns>The container composition.</returns>
					virtual compositions::GuiGraphicsComposition*	GetContainerComposition()=0;
					/// <summary>Set the focusable composition. A focusable composition is the composition to be focused when the control is focused.</summary>
					/// <param name="value">The focusable composition.</param>
					virtual void									SetFocusableComposition(compositions::GuiGraphicsComposition* value)=0;
					/// <summary>Set the text to display on the control.</summary>
					/// <param name="value">The text to display.</param>
					virtual void									SetText(const WString& value)=0;
					/// <summary>Set the font to render the text.</summary>
					/// <param name="value">The font to render the text.</param>
					virtual void									SetFont(const FontProperties& value)=0;
					/// <summary>Set the enableing state to affect the rendering of the control.</summary>
					/// <param name="value">The enableing state.</param>
					virtual void									SetVisuallyEnabled(bool value)=0;
				};

				/// <summary>
				/// A style provider is a callback interface for some control that already provides a style controller, but the controller need callbacks to create sub compositions or handle actions.
				/// </summary>
				class IStyleProvider : public virtual IDescriptable, public Description<IStyleProvider>
				{
				public:
					/// <summary>Called when a style provider is associated with a style controller.</summary>
					/// <param name="controller">The style controller that is associated.</param>
					virtual void								AssociateStyleController(IStyleController* controller)=0;
					/// <summary>Set the focusable composition. A focusable composition is the composition to be focused when the control is focused.</summary>
					/// <param name="value">The focusable composition.</param>
					virtual void								SetFocusableComposition(compositions::GuiGraphicsComposition* value)=0;
					/// <summary>Set the text to display on the control.</summary>
					/// <param name="value">The text to display.</param>
					virtual void								SetText(const WString& value)=0;
					/// <summary>Set the font to render the text.</summary>
					/// <param name="value">The font to render the text.</param>
					virtual void								SetFont(const FontProperties& value)=0;
					/// <summary>Set the enableing state to affect the rendering of the control.</summary>
					/// <param name="value">The enableing state.</param>
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
				/// <summary>Create a control with a specified style controller.</summary>
				GuiControl(IStyleController* _styleController);
				~GuiControl();

				/// <summary>Visible event. This event will be raised when the visibility state of the control is changed.</summary>
				compositions::GuiNotifyEvent			VisibleChanged;
				/// <summary>Enabled event. This event will be raised when the enabling state of the control is changed.</summary>
				compositions::GuiNotifyEvent			EnabledChanged;
				/// <summary>
				/// Enabled event. This event will be raised when the visually enabling state of the control is changed. A visually enabling is combined by the enabling state and the parent's visually enabling state.
				/// A control is rendered as disabled, not only when the control itself is disabled, but also when the parent control is rendered as disabled.
				/// </summary>
				compositions::GuiNotifyEvent			VisuallyEnabledChanged;
				/// <summary>Text changed event. This event will be raised when the text of the control is changed.</summary>
				compositions::GuiNotifyEvent			TextChanged;
				/// <summary>Font changed event. This event will be raised when the font of the control is changed.</summary>
				compositions::GuiNotifyEvent			FontChanged;

				/// <summary>A function to create the argument for notify events that raised by itself.</summary>
				/// <returns>The created argument.</returns>
				compositions::GuiEventArgs				GetNotifyEventArguments();
				/// <summary>Get the associated style controller.</summary>
				/// <returns>The associated style controller.</returns>
				IStyleController*						GetStyleController();
				/// <summary>Get the bounds composition for the control. The value is from <see cref="IStyleController::GetBoundsComposition"/>.</summary>
				/// <returns>The bounds composition.</returns>
				compositions::GuiBoundsComposition*		GetBoundsComposition();
				/// <summary>Get the container composition for the control. The value is from <see cref="IStyleController::GetContainerComposition"/>.</summary>
				/// <returns>The container composition.</returns>
				compositions::GuiGraphicsComposition*	GetContainerComposition();
				/// <summary>Get the focusable composition for the control. A focusable composition is the composition to be focused when the control is focused.</summary>
				/// <returns>The focusable composition.</returns>
				compositions::GuiGraphicsComposition*	GetFocusableComposition();
				/// <summary>Get the event receiver from the bounds composition.</summary>
				/// <returns>The event receiver.</returns>
				compositions::GuiGraphicsEventReceiver*	GetEventReceiver();
				/// <summary>Get the parent control.</summary>
				/// <returns>The parent control.</returns>
				GuiControl*								GetParent();
				/// <summary>Get the number of child controls.</summary>
				/// <returns>The number of child controls.</returns>
				int										GetChildrenCount();
				/// <summary>Get the child control using a specified index.</summary>
				/// <returns>The child control.</returns>
				/// <param name="index">The specified index.</param>
				GuiControl*								GetChild(int index);
				
				/// <summary>Get the <see cref="GuiControlHost"/> that contains this control.</summary>
				/// <returns>The <see cref="GuiControlHost"/> that contains this control.</returns>
				virtual GuiControlHost*					GetRelatedControlHost();
				/// <summary>Test if this control is rendered as enabled.</summary>
				/// <returns>Returns true if this control is rendered as enabled.</returns>
				virtual bool							GetVisuallyEnabled();
				/// <summary>Test if this control is enabled.</summary>
				/// <returns>Returns true if this control is enabled.</returns>
				virtual bool							GetEnabled();
				/// <summary>Make the control enabled or disabled.</summary>
				/// <param name="value">Set to true to make the control enabled.</param>
				virtual void							SetEnabled(bool value);
				/// <summary>Test if this visible or invisible.</summary>
				/// <returns>Returns true if this control is visible.</returns>
				virtual bool							GetVisible();
				/// <summary>Make the control visible or invisible.</summary>
				/// <param name="value">Set to true to make the visible enabled.</param>
				virtual void							SetVisible(bool value);
				/// <summary>Get the text to display on the control.</summary>
				/// <returns>The text to display on the control.</returns>
				virtual const WString&					GetText();
				/// <summary>Set the text to display on the control.</summary>
				/// <param name="value">The text to display on the control.</param>
				virtual void							SetText(const WString& value);
				/// <summary>Get the font to render the text.</summary>
				/// <returns>The font to render the text.</returns>
				virtual const FontProperties&			GetFont();
				/// <summary>Set the font to render the text.</summary>
				/// <param name="value">The font to render the text.</param>
				virtual void							SetFont(const FontProperties& value);

				/// <summary>Query a service using an identifier. If you want to get a service of type IXXX, use IXXX::Identifier as the identifier.</summary>
				/// <returns>The requested service. If the control doesn't support this service, it will be null.</returns>
				virtual IDescriptable*					QueryService(const WString& identifier);

				template<typename T>
				T* QueryService()
				{
					return dynamic_cast<T*>(QueryService(T::Identifier));
				}
			};

			/// <summary>
			/// Represnets a component.
			/// </summary>
			class GuiComponent : public Object, public Description<GuiComponent>
			{
			public:
				GuiComponent();
				~GuiComponent();
			};
			
			/// <summary>
			/// Represnets an image to display.
			/// </summary>
			class GuiImageData
			{
			protected:
				Ptr<INativeImage>				image;
				int								frameIndex;

			public:
				/// <summary>Create an empty image data.</summary>
				GuiImageData();
				/// <summary>Create an image data with a specified image and a frame index.</summary>
				/// <param name="_image">The specified image.</param>
				/// <param name="_frameIndex">The specified frame index.</param>
				GuiImageData(Ptr<INativeImage> _image, int _frameIndex);
				~GuiImageData();

				/// <summary>Get the specified image.</summary>
				/// <returns>The specified image.</returns>
				Ptr<INativeImage>				GetImage();
				/// <summary>Get the specified frame index.</summary>
				/// <returns>The specified frame index.</returns>
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