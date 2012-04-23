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
			/// If you want to manually destroy a control, you should first remove it from its parent.
			/// The only way to remove a control from a parent control, is to remove the bounds composition from its parent composition. The same to inserting a control.
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
				/// <param name="_styleController">The style controller.</param>
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
				/// <summary>Put another control in the container composition of this control.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="control">The control to put in this control.</param>
				bool									AddChild(GuiControl* control);
				
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

			/// <summary>A control with 3 phases state transffering when mouse click happens.</summary>
			class GuiButton : public GuiControl, public Description<GuiButton>
			{
			public:
				/// <summary>The visual state.</summary>
				enum ControlState
				{
					/// <summary>[T:vl.presentation.controls.GuiButton.ControlState]Normal state.</summary>
					Normal,
					/// <summary>[T:vl.presentation.controls.GuiButton.ControlState]Active state.</summary>
					Active,
					/// <summary>[T:vl.presentation.controls.GuiButton.ControlState]Pressed state.</summary>
					Pressed,
				};

				/// <summary>Style controller interface for <see cref="GuiButton"/>.</summary>
				class IStyleController : virtual public GuiControl::IStyleController, public Description<IStyleController>
				{
				public:
					/// <summary>Called when the control state changed.</summary>
					/// <param name="value">The new control state.</param>
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
				/// <summary>Create a control with a specified style controller.</summary>
				/// <param name="_styleController">The style controller.</param>
				GuiButton(IStyleController* _styleController);
				~GuiButton();

				/// <summary>Mouse click event.</summary>
				compositions::GuiNotifyEvent			Clicked;
			};

			/// <summary>A <see cref="GuiButton"/> with a selection state.</summary>
			class GuiSelectableButton : public GuiButton, public Description<GuiSelectableButton>
			{
			public:
				/// <summary>Style controller interface for <see cref="GuiSelectableButton"/>.</summary>
				class IStyleController : public virtual GuiButton::IStyleController, public Description<IStyleController>
				{
				public:
					/// <summary>Called when the selection state changed.</summary>
					/// <param name="value">The new control state.</param>
					virtual void						SetSelected(bool value)=0;
				};

				/// <summary>Selection group controller. Control the selection state of all attached button.</summary>
				class GroupController : public GuiComponent, public Description<GroupController>
				{
				protected:
					collections::List<GuiSelectableButton*>	buttons;
				public:
					GroupController();
					~GroupController();

					/// <summary>Called when the group controller is attached to a <see cref="GuiSelectableButton"/>. use [M:vl.presentation.controls.GuiSelectableButton.SetGroupController] to attach or detach a group controller to or from a selectable button.</summary>
					virtual void						Attach(GuiSelectableButton* button);
					/// <summary>Called when the group controller is deteched to a <see cref="GuiSelectableButton"/>. use [M:vl.presentation.controls.GuiSelectableButton.SetGroupController] to attach or detach a group controller to or from a selectable button.</summary>
					virtual void						Detach(GuiSelectableButton* button);
					/// <summary>Called when the selection state of any <see cref="GuiSelectableButton"/> changed.</summary>
					virtual void						OnSelectedChanged(GuiSelectableButton* button)=0;
				};

				/// <summary>A mutex group controller, usually for radio buttons.</summary>
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
				/// <summary>Create a control with a specified style controller.</summary>
				/// <param name="_styleController">The style controller.</param>
				GuiSelectableButton(IStyleController* _styleController);
				~GuiSelectableButton();

				/// <summary>Group controller changed event.</summary>
				compositions::GuiNotifyEvent			GroupControllerChanged;
				/// <summary>Auto selection changed event.</summary>
				compositions::GuiNotifyEvent			AutoSelectionChanged;
				/// <summary>Selected changed event.</summary>
				compositions::GuiNotifyEvent			SelectedChanged;

				/// <summary>Get the attached group controller.</summary>
				/// <returns>The attached group controller.</returns>
				virtual GroupController*				GetGroupController();
				/// <summary>Set the attached group controller.</summary>
				/// <param name="value">The attached group controller.</param>
				virtual void							SetGroupController(GroupController* value);
				
				/// <summary>Get the auto selection state. True if the button is automatically selected or unselected when the button is clicked.</summary>
				/// <returns>The auto selection state.</returns>
				virtual bool							GetAutoSelection();
				/// <summary>Set the auto selection state. True if the button is automatically selected or unselected when the button is clicked.</summary>
				/// <param name="value">The auto selection state.</param>
				virtual void							SetAutoSelection(bool value);
				
				/// <summary>Get the selected state.</summary>
				/// <returns>The selected state.</returns>
				virtual bool							GetSelected();
				/// <summary>Set the selected state.</summary>
				/// <param name="value">The selected state.</param>
				virtual void							SetSelected(bool value);
			};

/***********************************************************************
Scrolls
***********************************************************************/

			/// <summary>A scroll control, which represents a one dimension sub range of a whole range.</summary>
			class GuiScroll : public GuiControl, public Description<GuiScroll>
			{
			public:
				/// <summary>A command executor for the style controller to change the control state.</summary>
				class ICommandExecutor : public virtual IDescriptable, public Description<ICommandExecutor>
				{
				public:
					/// <summary>Do small decrement.</summary>
					virtual void						SmallDecrease()=0;
					/// <summary>Do small increment.</summary>
					virtual void						SmallIncrease()=0;
					/// <summary>Do big decrement.</summary>
					virtual void						BigDecrease()=0;
					/// <summary>Do big increment.</summary>
					virtual void						BigIncrease()=0;

					/// <summary>Change to total size of the scroll.</summary>
					/// <param name="value">The total size.</param>
					virtual void						SetTotalSize(int value)=0;
					/// <summary>Change to page size of the scroll.</summary>
					/// <param name="value">The page size.</param>
					virtual void						SetPageSize(int value)=0;
					/// <summary>Change to position of the scroll.</summary>
					/// <param name="value">The position.</param>
					virtual void						SetPosition(int value)=0;
				};
				
				/// <summary>Style controller interface for <see cref="GuiScroll"/>.</summary>
				class IStyleController : public virtual GuiControl::IStyleController, public Description<IStyleController>
				{
				public:
					/// <summary>Called when the command executor is changed.</summary>
					/// <param name="value">The command executor.</param>
					virtual void						SetCommandExecutor(ICommandExecutor* value)=0;
					/// <summary>Called when the total size is changed.</summary>
					/// <param name="value">The total size.</param>
					virtual void						SetTotalSize(int value)=0;
					/// <summary>Called when the page size is changed.</summary>
					/// <param name="value">The page size.</param>
					virtual void						SetPageSize(int value)=0;
					/// <summary>Called when the position is changed.</summary>
					/// <param name="value">The position.</param>
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
				/// <summary>Create a control with a specified style controller.</summary>
				/// <param name="_styleController">The style controller.</param>
				GuiScroll(IStyleController* _styleController);
				~GuiScroll();
				
				/// <summary>Total size changed event.</summary>
				compositions::GuiNotifyEvent			TotalSizeChanged;
				/// <summary>Page size changed event.</summary>
				compositions::GuiNotifyEvent			PageSizeChanged;
				/// <summary>Position changed event.</summary>
				compositions::GuiNotifyEvent			PositionChanged;
				/// <summary>Small move changed event.</summary>
				compositions::GuiNotifyEvent			SmallMoveChanged;
				/// <summary>Big move changed event.</summary>
				compositions::GuiNotifyEvent			BigMoveChanged;
				
				/// <summary>Get the total size.</summary>
				/// <returns>The total size.</returns>
				virtual int								GetTotalSize();
				/// <summary>Set the total size.</summary>
				/// <param name="value">The total size.</param>
				virtual void							SetTotalSize(int value);
				/// <summary>Get the page size.</summary>
				/// <returns>The page size.</returns>
				virtual int								GetPageSize();
				/// <summary>Set the page size.</summary>
				/// <param name="value">The page size.</param>
				virtual void							SetPageSize(int value);
				/// <summary>Get the position.</summary>
				/// <returns>The position.</returns>
				virtual int								GetPosition();
				/// <summary>Set the position.</summary>
				/// <param name="value">The position.</param>
				virtual void							SetPosition(int value);
				/// <summary>Get the small move.</summary>
				/// <returns>The small move.</returns>
				virtual int								GetSmallMove();
				/// <summary>Set the small move.</summary>
				/// <param name="value">The small move.</param>
				virtual void							SetSmallMove(int value);
				/// <summary>Get the big move.</summary>
				/// <returns>The big move.</returns>
				virtual int								GetBigMove();
				/// <summary>Set the big move.</summary>
				/// <param name="value">The big move.</param>
				virtual void							SetBigMove(int value);
				
				/// <summary>Get the minimum possible position.</summary>
				/// <returns>The minimum possible position.</returns>
				int										GetMinPosition();
				/// <summary>Get the maximum possible position.</summary>
				/// <returns>The maximum possible position.</returns>
				int										GetMaxPosition();
			};

			/// <summary>A control with a vertical scroll bar and a horizontal scroll bar to perform partial viewing.</summary>
			class GuiScrollView : public GuiControl, public Description<GuiScrollView>
			{
			public:
				/// <summary>Style provider interface for <see cref="GuiScrollView"/>.</summary>
				class IStyleProvider : public virtual GuiControl::IStyleProvider, public Description<IStyleProvider>
				{
				public:
					/// <summary>Create a control style for the horizontal scroll bar.</summary>
					/// <returns>The created control style for the horizontal scroll bar.</returns>
					virtual GuiScroll::IStyleController*			CreateHorizontalScrollStyle()=0;
					/// <summary>Create a control style for the vertical scroll bar.</summary>
					/// <returns>The created control style for the vertical scroll bar.</returns>
					virtual GuiScroll::IStyleController*			CreateVerticalScrollStyle()=0;
					/// <summary>Get the default scroll size for scroll bars, width for vertical, height for horizontal.</summary>
					/// <returns>The default scroll size.</returns>
					virtual int										GetDefaultScrollSize()=0;
					/// <summary>Called when the control begins to initialize. The control pass the bounds composition, and the style provider can put background compositions and elements on it, and return a container composition to contain content and scroll bars.</summary>
					/// <returns>A container composition to contain content and scroll bars</returns>
					/// <param name="boundsComposition">The bounds composition to install background.</param>
					virtual compositions::GuiGraphicsComposition*	InstallBackground(compositions::GuiBoundsComposition* boundsComposition)=0;
				};
				
				/// <summary>Style controller for <see cref="GuiScrollView"/>.</summary>
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
					/// <summary>Create a style controller with a specified style provider.</summary>
					/// <param name="_styleProvider">The style provider.</param>
					StyleController(IStyleProvider* _styleProvider);
					~StyleController();

					/// <summary>Called when the style controller is attched to a <see cref="GuiScrollView"/>.</summary>
					/// <param name="_scrollView">The scroll view control that attached to.</param>
					void									SetScrollView(GuiScrollView* _scrollView);
					/// <summary>Called when the view size of the scroll view changed. Scroll bars will be adjusted.</summary>
					/// <param name="fullSize">The view size.</param>
					void									AdjustView(Size fullSize);
					/// <summary>Get the attached style provider.</summary>
					/// <returns>The attached style provider.</returns>
					IStyleProvider*							GetStyleProvider();

					/// <summary>Get the horizontal scroll control.</summary>
					/// <returns>The horizontal scroll control.</returns>
					GuiScroll*								GetHorizontalScroll();
					/// <summary>Get the vertical scroll control.</summary>
					/// <returns>The vertical scroll control.</returns>
					GuiScroll*								GetVerticalScroll();
					compositions::GuiTableComposition*		GetInternalTableComposition();
					compositions::GuiBoundsComposition*		GetInternalContainerComposition();

					/// <summary>Test is the horizontal scroll bar always visible even the content doesn't exceed the view bounds.</summary>
					/// <returns>Returns true if the horizontal scroll bar always visible even the content doesn't exceed the view bounds</returns>
					bool									GetHorizontalAlwaysVisible();
					/// <summary>Set is the horizontal scroll bar always visible even the content doesn't exceed the view bounds.</summary>
					/// <param name="value">Set to true if the horizontal scroll bar always visible even the content doesn't exceed the view bounds</param>
					void									SetHorizontalAlwaysVisible(bool value);
					/// <summary>Test is the vertical scroll bar always visible even the content doesn't exceed the view bounds.</summary>
					/// <returns>Returns true if the vertical scroll bar always visible even the content doesn't exceed the view bounds</returns>
					bool									GetVerticalAlwaysVisible();
					/// <summary>Set is the vertical scroll bar always visible even the content doesn't exceed the view bounds.</summary>
					/// <param name="value">Set to true if the vertical scroll bar always visible even the content doesn't exceed the view bounds</param>
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

				/// <summary>Calculate the full size of the content.</summary>
				/// <returns>The full size of the content.</returns>
				virtual Size							QueryFullSize()=0;
				/// <summary>Update the visible content using a view bounds. The view bounds is in the space from (0,0) to full size.</summary>
				/// <param name="viewBounds">The view bounds.</param>
				virtual void							UpdateView(Rect viewBounds)=0;
				
				/// <summary>Create a control with a specified style controller.</summary>
				/// <param name="_styleController">The style controller.</param>
				GuiScrollView(StyleController* _styleController);
			public:
				/// <summary>Create a control with a specified style provider.</summary>
				/// <param name="styleProvider">The style provider.</param>
				GuiScrollView(IStyleProvider* styleProvider);
				~GuiScrollView();

				/// <summary>Force to update contents and scroll bars.</summary>
				void									CalculateView();
				/// <summary>Get the view size.</summary>
				/// <returns>The view size.</returns>
				Size									GetViewSize();
				/// <summary>Get the view bounds.</summary>
				/// <returns>The view bounds.</returns>
				Rect									GetViewBounds();
				
				/// <summary>Get the horizontal scroll control.</summary>
				/// <returns>The horizontal scroll control.</returns>
				GuiScroll*								GetHorizontalScroll();
				/// <summary>Get the vertical scroll control.</summary>
				/// <returns>The vertical scroll control.</returns>
				GuiScroll*								GetVerticalScroll();
				/// <summary>Test is the horizontal scroll bar always visible even the content doesn't exceed the view bounds.</summary>
				/// <returns>Returns true if the horizontal scroll bar always visible even the content doesn't exceed the view bounds</returns>
				bool									GetHorizontalAlwaysVisible();
				/// <summary>Set is the horizontal scroll bar always visible even the content doesn't exceed the view bounds.</summary>
				/// <param name="value">Set to true if the horizontal scroll bar always visible even the content doesn't exceed the view bounds</param>
				void									SetHorizontalAlwaysVisible(bool value);
				/// <summary>Test is the vertical scroll bar always visible even the content doesn't exceed the view bounds.</summary>
				/// <returns>Returns true if the vertical scroll bar always visible even the content doesn't exceed the view bounds</returns>
				bool									GetVerticalAlwaysVisible();
				/// <summary>Set is the vertical scroll bar always visible even the content doesn't exceed the view bounds.</summary>
				/// <param name="value">Set to true if the vertical scroll bar always visible even the content doesn't exceed the view bounds</param>
				void									SetVerticalAlwaysVisible(bool value);
			};
			
			/// <summary>A control container with a vertical scroll bar and a horizontal scroll bar to perform partial viewing. When controls are added, removed, moved or resized, the scroll bars will adjust automatically.</summary>
			class GuiScrollContainer : public GuiScrollView, public Description<GuiScrollContainer>
			{
			public:
				/// <summary>Style controller for <see cref="GuiScrollContainer"/>.</summary>
				class StyleController : public GuiScrollView::StyleController, public Description<StyleController>
				{
				protected:
					compositions::GuiBoundsComposition*		controlContainerComposition;
				public:
					/// <summary>Create a style controller with a specified style provider.</summary>
					/// <param name="_styleProvider">The style provider.</param>
					StyleController(GuiScrollView::IStyleProvider* styleProvider);
					~StyleController();

					compositions::GuiGraphicsComposition*	GetContainerComposition()override;
					/// <summary>Update sub controls using a specified left-top position of the view bounds.</summary>
					/// <param name="leftTop">The specified left-top position of the view bounds.</param>
					void									MoveContainer(Point leftTop);
				};

			protected:
				StyleController*						styleController;

				void									OnControlContainerBoundsChanged(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				Size									QueryFullSize()override;
				void									UpdateView(Rect viewBounds)override;
			public:
				/// <summary>Create a control with a specified style provider.</summary>
				/// <param name="styleProvider">The style provider.</param>
				GuiScrollContainer(GuiScrollContainer::IStyleProvider* styleProvider);
				~GuiScrollContainer();
			};
		}
	}
}

#endif