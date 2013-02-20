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
				/// An empty style for <see cref="GuiControl"/>.
				/// </summary>
				class EmptyStyleController : public Object, public IStyleController, public Description<EmptyStyleController>
				{
				protected:
					compositions::GuiBoundsComposition*				boundsComposition;
				public:
					EmptyStyleController();
					~EmptyStyleController();

					compositions::GuiBoundsComposition*				GetBoundsComposition()override;
					compositions::GuiGraphicsComposition*			GetContainerComposition()override;
					void											SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;
					void											SetText(const WString& value)override;
					void											SetFont(const FontProperties& value)override;
					void											SetVisuallyEnabled(bool value)override;
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
				Ptr<Object>								tag;

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
				vint									GetChildrenCount();
				/// <summary>Get the child control using a specified index.</summary>
				/// <returns>The child control.</returns>
				/// <param name="index">The specified index.</param>
				GuiControl*								GetChild(vint index);
				/// <summary>Put another control in the container composition of this control.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="control">The control to put in this control.</param>
				bool									AddChild(GuiControl* control);
				/// <summary>Test if a control owned by this control.</summary>
				/// <returns>Returns true if the control is owned by this control.</returns>
				/// <param name="control">The control to test.</param>
				bool									HasChild(GuiControl* control);
				
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
				/// <summary>Focus this control.</summary>
				virtual void							SetFocus();

				/// <summary>Get the tag object of the control.</summary>
				/// <returns>The tag object of the control.</returns>
				Ptr<Object>								GetTag();
				/// <summary>Set the tag object of the control.</summary>
				/// <param name="value">The tag object of the control.</param>
				void									SetTag(Ptr<Object> value);

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

			template<typename T>
			class GuiObjectComponent : public GuiComponent
			{
			public:
				Ptr<T>				object;

				GuiObjectComponent()
				{
				}

				GuiObjectComponent(Ptr<T> _object)
					:object(_object)
				{
				}
			};

/***********************************************************************
Label
***********************************************************************/

			/// <summary>A control to display a text.</summary>
			class GuiLabel : public GuiControl, public Description<GuiLabel>
			{
			public:
				/// <summary>Style controller interface for <see cref="GuiLabel"/>.</summary>
				class IStyleController : virtual public GuiControl::IStyleController, public Description<IStyleController>
				{
				public:
					/// <summary>Get the default text color.</summary>
					/// <returns>The default text color.</returns>
					virtual Color						GetDefaultTextColor()=0;
					/// <summary>Called when the text color changed.</summary>
					/// <param name="value">The new text color.</param>
					virtual void						SetTextColor(Color value)=0;
				};
			protected:
				Color									textColor;
				IStyleController*						styleController;
			public:
				/// <summary>Create a control with a specified style controller.</summary>
				/// <param name="_styleController">The style controller.</param>
				GuiLabel(IStyleController* _styleController);
				~GuiLabel();
				
				/// <summary>Get the text color.</summary>
				/// <returns>The text color.</returns>
				Color									GetTextColor();
				/// <summary>Set the text color.</summary>
				/// <param name="value">The text color.</param>
				void									SetTextColor(Color value);
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
				bool									clickOnMouseUp;
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

				/// <summary>Test is the <see cref="Clicked"/> event raised when left mouse button up.</summary>
				/// <returns>Returns true if this event is raised when left mouse button up</returns>
				bool									GetClickOnMouseUp();
				/// <summary>Set is the <see cref="Clicked"/> event raised when left mouse button up or not.</summary>
				/// <param name="value">Set to true to make this event raised when left mouse button up</param>
				void									SetClickOnMouseUp(bool value);
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
					virtual void						SetTotalSize(vint value)=0;
					/// <summary>Change to page size of the scroll.</summary>
					/// <param name="value">The page size.</param>
					virtual void						SetPageSize(vint value)=0;
					/// <summary>Change to position of the scroll.</summary>
					/// <param name="value">The position.</param>
					virtual void						SetPosition(vint value)=0;
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
					virtual void						SetTotalSize(vint value)=0;
					/// <summary>Called when the page size is changed.</summary>
					/// <param name="value">The page size.</param>
					virtual void						SetPageSize(vint value)=0;
					/// <summary>Called when the position is changed.</summary>
					/// <param name="value">The position.</param>
					virtual void						SetPosition(vint value)=0;
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

					void								SetTotalSize(vint value)override;
					void								SetPageSize(vint value)override;
					void								SetPosition(vint value)override;
				};

				IStyleController*						styleController;
				Ptr<CommandExecutor>					commandExecutor;
				vint										totalSize;
				vint										pageSize;
				vint										position;
				vint										smallMove;
				vint										bigMove;
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
				virtual vint								GetTotalSize();
				/// <summary>Set the total size.</summary>
				/// <param name="value">The total size.</param>
				virtual void							SetTotalSize(vint value);
				/// <summary>Get the page size.</summary>
				/// <returns>The page size.</returns>
				virtual vint								GetPageSize();
				/// <summary>Set the page size.</summary>
				/// <param name="value">The page size.</param>
				virtual void							SetPageSize(vint value);
				/// <summary>Get the position.</summary>
				/// <returns>The position.</returns>
				virtual vint								GetPosition();
				/// <summary>Set the position.</summary>
				/// <param name="value">The position.</param>
				virtual void							SetPosition(vint value);
				/// <summary>Get the small move.</summary>
				/// <returns>The small move.</returns>
				virtual vint								GetSmallMove();
				/// <summary>Set the small move.</summary>
				/// <param name="value">The small move.</param>
				virtual void							SetSmallMove(vint value);
				/// <summary>Get the big move.</summary>
				/// <returns>The big move.</returns>
				virtual vint								GetBigMove();
				/// <summary>Set the big move.</summary>
				/// <param name="value">The big move.</param>
				virtual void							SetBigMove(vint value);
				
				/// <summary>Get the minimum possible position.</summary>
				/// <returns>The minimum possible position.</returns>
				vint										GetMinPosition();
				/// <summary>Get the maximum possible position.</summary>
				/// <returns>The maximum possible position.</returns>
				vint										GetMaxPosition();
			};
			
			namespace list
			{
/***********************************************************************
List interface common implementation
***********************************************************************/

				template<typename T, typename K=typename KeyType<T>::Type>
				class ItemsBase : public Object, public virtual collections::IEnumerable<T>
				{
				protected:
					collections::List<T, K>					items;

					virtual void NotifyUpdateInternal(vint start, vint count, vint newCount)
					{
					}

					virtual bool InsertInternal(vint index, const T& value)
					{
						items.Insert(index, value);
						return true;
					}

					virtual bool RemoveAtInternal(vint index, const T& value)
					{
						items.RemoveAt(index);
						return true;
					}
					
				public:
					ItemsBase()
					{
					}

					~ItemsBase()
					{
					}

					collections::IEnumerator<T>* CreateEnumerator()const
					{
						return items.CreateEnumerator();
					}

					bool NotifyUpdate(vint start, vint count=1)
					{
						if(start<0 || start>=items.Count() || count<=0 || start+count>items.Count())
						{
							return false;
						}
						else
						{
							NotifyUpdateInternal(start, count, count);
							return true;
						}
					}

					bool Contains(const K& item)const
					{
						return items.Contains(item);
					}

					vint Count()const
					{
						return items.Count();
					}

					vint Count()
					{
						return items.Count();
					}

					const T& Get(vint index)const
					{
						return items.Get(index);
					}

					const T& operator[](vint index)const
					{
						return items.Get(index);
					}

					vint IndexOf(const K& item)const
					{
						return items.IndexOf(item);
					}

					vint Add(const T& item)
					{
						return Insert(items.Count(), item);
					}

					bool Remove(const K& item)
					{
						vint index=items.IndexOf(item);
						if(index==-1) return false;
						return RemoveAt(index);
					}

					bool RemoveAt(vint index)
					{
						if(RemoveAtInternal(index, items[index]))
						{
							NotifyUpdateInternal(index, 1, 0);
							return true;
						}
						else
						{
							return false;
						}
					}

					bool RemoveRange(vint index, vint count)
					{
						if(count<=0) return false;
						if(0<=index && index<items.Count() && index+count<=items.Count())
						{
							while(count-->0)
							{
								RemoveAt(index+count);
							}
							return true;
						}
						else
						{
							return false;
						}
					}

					bool Clear()
					{
						while(items.Count()>0)
						{
							RemoveAt(items.Count()-1);
						}
						return true;
					}

					vint Insert(vint index, const T& item)
					{
						if(InsertInternal(index, item))
						{
							NotifyUpdateInternal(index, 0, 1);
							return index;
						}
						else
						{
							return -1;
						}
					}

					bool Set(vint index, const T& item)
					{
						if(Insert(index, item))
						{
							return RemoveAt(index+1);
						}
						else
						{
							return false;
						}
					}
				};
			}
		}
	}

	namespace collections
	{
		namespace randomaccess_internal
		{
			template<typename T>
			struct RandomAccessable<presentation::controls::list::ItemsBase<T>>
			{
				static const bool							CanRead = true;
				static const bool							CanResize = false;
			};
		}
	}
}

#endif