#include "GuiBasicControls.h"
#include "GuiApplication.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace elements;
			using namespace compositions;
			using namespace collections;
			using namespace reflection::description;

/***********************************************************************
GuiControl::EmptyStyleController
***********************************************************************/

			GuiControl::EmptyStyleController::EmptyStyleController()
			{
				boundsComposition=new GuiBoundsComposition;
			}

			GuiControl::EmptyStyleController::~EmptyStyleController()
			{
			}

			compositions::GuiBoundsComposition* GuiControl::EmptyStyleController::GetBoundsComposition()
			{
				return boundsComposition;
			}

			compositions::GuiGraphicsComposition* GuiControl::EmptyStyleController::GetContainerComposition()
			{
				return boundsComposition;
			}

			void GuiControl::EmptyStyleController::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
			}

			void GuiControl::EmptyStyleController::SetText(const WString& value)
			{
			}

			void GuiControl::EmptyStyleController::SetFont(const FontProperties& value)
			{
			}

			void GuiControl::EmptyStyleController::SetVisuallyEnabled(bool value)
			{
			}

/***********************************************************************
GuiControl
***********************************************************************/

			void GuiControl::OnChildInserted(GuiControl* control)
			{
				GuiControl* oldParent=control->parent;
				children.Add(control);
				control->parent=this;
				control->OnParentChanged(oldParent, control->parent);
				control->UpdateVisuallyEnabled();
			}

			void GuiControl::OnChildRemoved(GuiControl* control)
			{
				GuiControl* oldParent=control->parent;
				control->parent=0;
				children.Remove(control);
				control->OnParentChanged(oldParent, control->parent);
			}

			void GuiControl::OnParentChanged(GuiControl* oldParent, GuiControl* newParent)
			{
				OnParentLineChanged();
			}

			void GuiControl::OnParentLineChanged()
			{
				for(vint i=0;i<children.Count();i++)
				{
					children[i]->OnParentLineChanged();
				}
			}

			void GuiControl::OnRenderTargetChanged(elements::IGuiGraphicsRenderTarget* renderTarget)
			{
			}

			void GuiControl::OnBeforeReleaseGraphicsHost()
			{
				for(vint i=0;i<children.Count();i++)
				{
					children[i]->OnBeforeReleaseGraphicsHost();
				}
			}

			void GuiControl::UpdateVisuallyEnabled()
			{
				bool newValue=isEnabled && (parent==0?true:parent->GetVisuallyEnabled());
				if(isVisuallyEnabled!=newValue)
				{
					isVisuallyEnabled=newValue;
					styleController->SetVisuallyEnabled(isVisuallyEnabled);
					VisuallyEnabledChanged.Execute(GetNotifyEventArguments());

					for(vint i=0;i<children.Count();i++)
					{
						children[i]->UpdateVisuallyEnabled();
					}
				}
			}

			void GuiControl::SetFocusableComposition(compositions::GuiGraphicsComposition* value)
			{
				if(focusableComposition!=value)
				{
					focusableComposition=value;
					styleController->SetFocusableComposition(focusableComposition);
				}
			}

			bool GuiControl::IsAltEnabled()
			{
				GuiControl* control = this;
				while (control)
				{
					if (!control->GetVisible() || !control->GetEnabled())
					{
						return false;
					}
					control = control->GetParent();
				}

				return true;
			}

			bool GuiControl::IsAltAvailable()
			{
				return focusableComposition != 0 && alt != L"";
			}

			compositions::GuiGraphicsComposition* GuiControl::GetAltComposition()
			{
				return boundsComposition;
			}

			compositions::IGuiAltActionHost* GuiControl::GetActivatingAltHost()
			{
				return activatingAltHost;
			}

			void GuiControl::OnActiveAlt()
			{
				SetFocus();
			}

			bool GuiControl::SharedPtrDestructorProc(DescriptableObject* obj, bool forceDisposing)
			{
				GuiControl* value=dynamic_cast<GuiControl*>(obj);
				if(value->GetBoundsComposition()->GetParent())
				{
					if (!forceDisposing) return false;
				}
				SafeDeleteControl(value);
				return true;
			}

			GuiControl::GuiControl(IStyleController* _styleController)
				:styleController(_styleController)
				,boundsComposition(_styleController->GetBoundsComposition())
				,focusableComposition(0)
				,eventReceiver(_styleController->GetBoundsComposition()->GetEventReceiver())
				,isEnabled(true)
				,isVisuallyEnabled(true)
				,isVisible(true)
				,activatingAltHost(0)
				,parent(0)
				,tooltipControl(0)
				,tooltipWidth(50)
			{
				boundsComposition->SetAssociatedControl(this);
				VisibleChanged.SetAssociatedComposition(boundsComposition);
				EnabledChanged.SetAssociatedComposition(boundsComposition);
				VisuallyEnabledChanged.SetAssociatedComposition(boundsComposition);
				AltChanged.SetAssociatedComposition(boundsComposition);
				TextChanged.SetAssociatedComposition(boundsComposition);
				FontChanged.SetAssociatedComposition(boundsComposition);

				font=GetCurrentController()->ResourceService()->GetDefaultFont();
				styleController->SetFont(font);
				styleController->SetText(text);
				styleController->SetVisuallyEnabled(isVisuallyEnabled);
				
				sharedPtrDestructorProc=&GuiControl::SharedPtrDestructorProc;
			}

			GuiControl::~GuiControl()
			{
				if(tooltipControl)
				{
					// the only legal parent is the GuiApplication::sharedTooltipWindow
					if(tooltipControl->GetBoundsComposition()->GetParent())
					{
						tooltipControl->GetBoundsComposition()->GetParent()->RemoveChild(tooltipControl->GetBoundsComposition());
					}
					delete tooltipControl;
				}
				if(parent || !styleController)
				{
					for(vint i=0;i<children.Count();i++)
					{
						delete children[i];
					}
				}
				else
				{
					for(vint i=children.Count()-1;i>=0;i--)
					{
						GuiControl* child=children[i];
						child->GetBoundsComposition()->GetParent()->RemoveChild(child->GetBoundsComposition());
						delete child;
					}
					delete boundsComposition;
				}
			}

			compositions::GuiEventArgs GuiControl::GetNotifyEventArguments()
			{
				return GuiEventArgs(boundsComposition);
			}

			GuiControl::IStyleController* GuiControl::GetStyleController()
			{
				return styleController.Obj();
			}

			compositions::GuiBoundsComposition* GuiControl::GetBoundsComposition()
			{
				return boundsComposition;
			}

			compositions::GuiGraphicsComposition* GuiControl::GetContainerComposition()
			{
				return styleController->GetContainerComposition();
			}

			compositions::GuiGraphicsComposition* GuiControl::GetFocusableComposition()
			{
				return focusableComposition;
			}

			compositions::GuiGraphicsEventReceiver* GuiControl::GetEventReceiver()
			{
				return eventReceiver;
			}

			GuiControl* GuiControl::GetParent()
			{
				return parent;
			}

			vint GuiControl::GetChildrenCount()
			{
				return children.Count();
			}

			GuiControl* GuiControl::GetChild(vint index)
			{
				return children[index];
			}

			bool GuiControl::AddChild(GuiControl* control)
			{
				return GetContainerComposition()->AddChild(control->GetBoundsComposition());
			}

			bool GuiControl::HasChild(GuiControl* control)
			{
				return children.Contains(control);
			}

			GuiControlHost* GuiControl::GetRelatedControlHost()
			{
				return parent?parent->GetRelatedControlHost():0;
			}

			bool GuiControl::GetVisuallyEnabled()
			{
				return isVisuallyEnabled;
			}

			bool GuiControl::GetEnabled()
			{
				return isEnabled;
			}

			void GuiControl::SetEnabled(bool value)
			{
				if(isEnabled!=value)
				{
					isEnabled=value;
					EnabledChanged.Execute(GetNotifyEventArguments());
					UpdateVisuallyEnabled();
				}
			}

			bool GuiControl::GetVisible()
			{
				return isVisible;
			}

			void GuiControl::SetVisible(bool value)
			{
				boundsComposition->SetVisible(value);
				if(isVisible!=value)
				{
					isVisible=value;
					VisibleChanged.Execute(GetNotifyEventArguments());
				}
			}

			const WString& GuiControl::GetAlt()
			{
				return alt;
			}

			bool GuiControl::SetAlt(const WString& value)
			{
				if (!IGuiAltAction::IsLegalAlt(value)) return false;
				if (alt != value)
				{
					alt = value;
					AltChanged.Execute(GetNotifyEventArguments());
				}
				return true;
			}

			void GuiControl::SetActivatingAltHost(compositions::IGuiAltActionHost* host)
			{
				activatingAltHost = host;
			}

			const WString& GuiControl::GetText()
			{
				return text;
			}

			void GuiControl::SetText(const WString& value)
			{
				if(text!=value)
				{
					text=value;
					styleController->SetText(text);
					TextChanged.Execute(GetNotifyEventArguments());
				}
			}

			const FontProperties& GuiControl::GetFont()
			{
				return font;
			}

			void GuiControl::SetFont(const FontProperties& value)
			{
				if(font!=value)
				{
					font=value;
					styleController->SetFont(font);
					FontChanged.Execute(GetNotifyEventArguments());
				}
			}

			void GuiControl::SetFocus()
			{
				if(focusableComposition)
				{
					GuiGraphicsHost* host=focusableComposition->GetRelatedGraphicsHost();
					if(host)
					{
						host->SetFocus(focusableComposition);
					}
				}
			}

			description::Value GuiControl::GetTag()
			{
				return tag;
			}

			void GuiControl::SetTag(const description::Value& value)
			{
				tag=value;
			}

			GuiControl* GuiControl::GetTooltipControl()
			{
				return tooltipControl;
			}

			GuiControl* GuiControl::SetTooltipControl(GuiControl* value)
			{
				GuiControl* oldTooltipControl=tooltipControl;
				tooltipControl=value;
				return oldTooltipControl;
			}

			vint GuiControl::GetTooltipWidth()
			{
				return tooltipWidth;
			}

			void GuiControl::SetTooltipWidth(vint value)
			{
				tooltipWidth=value;
			}

			bool GuiControl::DisplayTooltip(Point location)
			{
				if(!tooltipControl) return false;
				GetApplication()->ShowTooltip(this, tooltipControl, tooltipWidth, location);
				return true;
			}

			void GuiControl::CloseTooltip()
			{
				if(GetApplication()->GetTooltipOwner()==this)
				{
					GetApplication()->CloseTooltip();
				}
			}

			IDescriptable* GuiControl::QueryService(const WString& identifier)
			{
				if (identifier == IGuiAltAction::Identifier)
				{
					return (IGuiAltAction*)this;
				}
				else if (identifier == IGuiAltActionContainer::Identifier)
				{
					return 0;
				}
				else if(parent)
				{
					return parent->QueryService(identifier);
				}
				else
				{
					return 0;
				}
			}

/***********************************************************************
GuiComponent
***********************************************************************/
			
			GuiComponent::GuiComponent()
			{
			}

			GuiComponent::~GuiComponent()
			{
			}

			void GuiComponent::Attach(GuiInstanceRootObject* rootObject)
			{
			}

			void GuiComponent::Detach(GuiInstanceRootObject* rootObject)
			{
			}

/***********************************************************************
GuiInstanceRootObject
***********************************************************************/

			void GuiInstanceRootObject::ClearSubscriptions()
			{
				FOREACH(Ptr<IValueSubscription>, subscription, subscriptions)
				{
					subscription->Close();
				}
				subscriptions.Clear();
			}

			void GuiInstanceRootObject::ClearComponents()
			{
				for(vint i=0;i<components.Count();i++)
				{
					components[i]->Detach(this);
				}
				for(vint i=0;i<components.Count();i++)
				{
					delete components[i];
				}
				components.Clear();
			}

			void GuiInstanceRootObject::FinalizeInstance()
			{
				ClearSubscriptions();
				ClearComponents();
			}

			GuiInstanceRootObject::GuiInstanceRootObject()
			{
			}

			GuiInstanceRootObject::~GuiInstanceRootObject()
			{
			}

			Ptr<description::IValueSubscription> GuiInstanceRootObject::AddSubscription(Ptr<description::IValueSubscription> subscription)
			{
				if (subscriptions.Contains(subscription.Obj()))
				{
					return 0;
				}
				else
				{
					subscriptions.Add(subscription);
					return subscription;
				}
			}

			bool GuiInstanceRootObject::RemoveSubscription(Ptr<description::IValueSubscription> subscription)
			{
				return subscriptions.Remove(subscription.Obj());
			}

			bool GuiInstanceRootObject::ContainsSubscription(Ptr<description::IValueSubscription> subscription)
			{
				return subscriptions.Contains(subscription.Obj());
			}

			bool GuiInstanceRootObject::AddComponent(GuiComponent* component)
			{
				if(components.Contains(component))
				{
					return false;
				}
				else
				{
					components.Add(component);
					component->Attach(this);
					return true;
				}
			}

			bool GuiInstanceRootObject::RemoveComponent(GuiComponent* component)
			{
				vint index = components.IndexOf(component);
				if (index == -1)
				{
					return false;
				}
				{
					component->Detach(this);
					return components.RemoveAt(index);
				}
			}

			bool GuiInstanceRootObject::ContainsComponent(GuiComponent* component)
			{
				return components.Contains(component);
			}

/***********************************************************************
GuiCustomControl
***********************************************************************/

			GuiCustomControl::GuiCustomControl(IStyleController* _styleController)
				:GuiControl(_styleController)
			{
			}

			GuiCustomControl::~GuiCustomControl()
			{
				FinalizeInstance();
			}

/***********************************************************************
GuiLabel
***********************************************************************/

			GuiLabel::GuiLabel(IStyleController* _styleController)
				:GuiControl(_styleController)
				,styleController(_styleController)
			{
				textColor=styleController->GetDefaultTextColor();
				styleController->SetTextColor(textColor);
			}

			GuiLabel::~GuiLabel()
			{
			}

			Color GuiLabel::GetTextColor()
			{
				return textColor;
			}

			void GuiLabel::SetTextColor(Color value)
			{
				if(textColor!=value)
				{
					textColor=value;
					styleController->SetTextColor(textColor);
				}
			}

/***********************************************************************
GuiButton
***********************************************************************/

			void GuiButton::OnParentLineChanged()
			{
				GuiControl::OnParentLineChanged();
				if(GetRelatedControlHost()==0)
				{
					mousePressing=false;
					mouseHoving=false;
					UpdateControlState();
				}
			}

			void GuiButton::OnActiveAlt()
			{
				Clicked.Execute(GetNotifyEventArguments());
			}

			void GuiButton::UpdateControlState()
			{
				ControlState newControlState=Normal;
				if(mousePressing)
				{
					if(mouseHoving)
					{
						newControlState=Pressed;
					}
					else
					{
						newControlState=Active;
					}
				}
				else
				{
					if(mouseHoving)
					{
						newControlState=Active;
					}
					else
					{
						newControlState=Normal;
					}
				}
				if(controlState!=newControlState)
				{
					controlState=newControlState;
					styleController->Transfer(controlState);
				}
			}

			void GuiButton::OnLeftButtonDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
			{
				if(arguments.eventSource==boundsComposition)
				{
					mousePressing=true;
					boundsComposition->GetRelatedGraphicsHost()->SetFocus(boundsComposition);
					UpdateControlState();
					if(!clickOnMouseUp && arguments.eventSource->GetAssociatedControl()==this)
					{
						Clicked.Execute(GetNotifyEventArguments());
					}
				}
			}

			void GuiButton::OnLeftButtonUp(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
			{
				if(arguments.eventSource==boundsComposition)
				{
					mousePressing=false;
					UpdateControlState();
				}
				if(GetVisuallyEnabled())
				{
					if(mouseHoving && clickOnMouseUp && arguments.eventSource->GetAssociatedControl()==this)
					{
						Clicked.Execute(GetNotifyEventArguments());
					}
				}
			}

			void GuiButton::OnMouseEnter(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				if(arguments.eventSource==boundsComposition)
				{
					mouseHoving=true;
					UpdateControlState();
				}
			}

			void GuiButton::OnMouseLeave(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				if(arguments.eventSource==boundsComposition)
				{
					mouseHoving=false;
					UpdateControlState();
				}
			}

			GuiButton::GuiButton(IStyleController* _styleController)
				:GuiControl(_styleController)
				,styleController(_styleController)
				,clickOnMouseUp(true)
				,mousePressing(false)
				,mouseHoving(false)
				,controlState(Normal)
			{
				Clicked.SetAssociatedComposition(boundsComposition);
				styleController->Transfer(Normal);
				SetFocusableComposition(boundsComposition);

				GetEventReceiver()->leftButtonDown.AttachMethod(this, &GuiButton::OnLeftButtonDown);
				GetEventReceiver()->leftButtonUp.AttachMethod(this, &GuiButton::OnLeftButtonUp);
				GetEventReceiver()->mouseEnter.AttachMethod(this, &GuiButton::OnMouseEnter);
				GetEventReceiver()->mouseLeave.AttachMethod(this, &GuiButton::OnMouseLeave);
			}

			GuiButton::~GuiButton()
			{
			}

			bool GuiButton::GetClickOnMouseUp()
			{
				return clickOnMouseUp;
			}

			void GuiButton::SetClickOnMouseUp(bool value)
			{
				clickOnMouseUp=value;
			}

/***********************************************************************
GuiSelectableButton::GroupController
***********************************************************************/

			GuiSelectableButton::GroupController::GroupController()
			{
			}

			GuiSelectableButton::GroupController::~GroupController()
			{
				for(vint i=buttons.Count()-1;i>=0;i--)
				{
					buttons[i]->SetGroupController(0);
				}
			}

			void GuiSelectableButton::GroupController::Attach(GuiSelectableButton* button)
			{
				if(!buttons.Contains(button))
				{
					buttons.Add(button);
				}
			}

			void GuiSelectableButton::GroupController::Detach(GuiSelectableButton* button)
			{
				buttons.Remove(button);
			}

/***********************************************************************
GuiSelectableButton::MutexGroupController
***********************************************************************/

			GuiSelectableButton::MutexGroupController::MutexGroupController()
				:suppress(false)
			{
			}

			GuiSelectableButton::MutexGroupController::~MutexGroupController()
			{
			}

			void GuiSelectableButton::MutexGroupController::OnSelectedChanged(GuiSelectableButton* button)
			{
				if(!suppress)
				{
					suppress=true;
					for(vint i=0;i<buttons.Count();i++)
					{
						buttons[i]->SetSelected(buttons[i]==button);
					}
					suppress=false;
				}
			}

/***********************************************************************
GuiSelectableButton
***********************************************************************/

			void GuiSelectableButton::OnClicked(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				if(autoSelection)
				{
					SetSelected(!GetSelected());
				}
			}

			GuiSelectableButton::GuiSelectableButton(IStyleController* _styleController)
				:GuiButton(_styleController)
				,styleController(_styleController)
				,groupController(0)
				,autoSelection(true)
				,isSelected(false)
			{
				GroupControllerChanged.SetAssociatedComposition(boundsComposition);
				AutoSelectionChanged.SetAssociatedComposition(boundsComposition);
				SelectedChanged.SetAssociatedComposition(boundsComposition);

				Clicked.AttachMethod(this, &GuiSelectableButton::OnClicked);
				styleController->SetSelected(isSelected);
			}
			
			GuiSelectableButton::~GuiSelectableButton()
			{
				if(groupController)
				{
					groupController->Detach(this);
				}
			}

			GuiSelectableButton::GroupController* GuiSelectableButton::GetGroupController()
			{
				return groupController;
			}

			void GuiSelectableButton::SetGroupController(GroupController* value)
			{
				if(groupController)
				{
					groupController->Detach(this);
				}
				groupController=value;
				if(groupController)
				{
					groupController->Attach(this);
				}
				GroupControllerChanged.Execute(GetNotifyEventArguments());
			}

			bool GuiSelectableButton::GetAutoSelection()
			{
				return autoSelection;
			}

			void GuiSelectableButton::SetAutoSelection(bool value)
			{
				if(autoSelection!=value)
				{
					autoSelection=value;
					AutoSelectionChanged.Execute(GetNotifyEventArguments());
				}
			}

			bool GuiSelectableButton::GetSelected()
			{
				return isSelected;
			}

			void GuiSelectableButton::SetSelected(bool value)
			{
				if(isSelected!=value)
				{
					isSelected=value;
					styleController->SetSelected(isSelected);
					if(groupController)
					{
						groupController->OnSelectedChanged(this);
					}
					SelectedChanged.Execute(GetNotifyEventArguments());
				}
			}

/***********************************************************************
GuiScroll::CommandExecutor
***********************************************************************/

			GuiScroll::CommandExecutor::CommandExecutor(GuiScroll* _scroll)
				:scroll(_scroll)
			{
			}

			GuiScroll::CommandExecutor::~CommandExecutor()
			{
			}

			void GuiScroll::CommandExecutor::SmallDecrease()
			{
				scroll->SetPosition(scroll->GetPosition()-scroll->GetSmallMove());
			}

			void GuiScroll::CommandExecutor::SmallIncrease()
			{
				scroll->SetPosition(scroll->GetPosition()+scroll->GetSmallMove());
			}

			void GuiScroll::CommandExecutor::BigDecrease()
			{
				scroll->SetPosition(scroll->GetPosition()-scroll->GetBigMove());
			}

			void GuiScroll::CommandExecutor::BigIncrease()
			{
				scroll->SetPosition(scroll->GetPosition()+scroll->GetBigMove());
			}
			
			void GuiScroll::CommandExecutor::SetTotalSize(vint value)
			{
				scroll->SetTotalSize(value);
			}

			void GuiScroll::CommandExecutor::SetPageSize(vint value)
			{
				scroll->SetPageSize(value);
			}

			void GuiScroll::CommandExecutor::SetPosition(vint value)
			{
				scroll->SetPosition(value);
			}

/***********************************************************************
GuiScroll
***********************************************************************/

			GuiScroll::GuiScroll(IStyleController* _styleController)
				:GuiControl(_styleController)
				,styleController(_styleController)
				,totalSize(100)
				,pageSize(10)
				,position(0)
				,smallMove(1)
				,bigMove(10)
			{
				TotalSizeChanged.SetAssociatedComposition(boundsComposition);
				PageSizeChanged.SetAssociatedComposition(boundsComposition);
				PositionChanged.SetAssociatedComposition(boundsComposition);
				SmallMoveChanged.SetAssociatedComposition(boundsComposition);
				BigMoveChanged.SetAssociatedComposition(boundsComposition);

				commandExecutor=new CommandExecutor(this);
				styleController->SetCommandExecutor(commandExecutor.Obj());
				styleController->SetPageSize(pageSize);
				styleController->SetTotalSize(totalSize);
				styleController->SetPosition(position);
			}

			GuiScroll::~GuiScroll()
			{
			}

			vint GuiScroll::GetTotalSize()
			{
				return totalSize;
			}

			void GuiScroll::SetTotalSize(vint value)
			{
				if(totalSize!=value && 0<value)
				{
					totalSize=value;
					if(pageSize>totalSize)
					{
						SetPageSize(totalSize);
					}
					if(position>GetMaxPosition())
					{
						SetPosition(GetMaxPosition());
					}
					styleController->SetTotalSize(totalSize);
					TotalSizeChanged.Execute(GetNotifyEventArguments());
				}
			}

			vint GuiScroll::GetPageSize()
			{
				return pageSize;
			}

			void GuiScroll::SetPageSize(vint value)
			{
				if(pageSize!=value && 0<=value && value<=totalSize)
				{
					pageSize=value;
					if(position>GetMaxPosition())
					{
						SetPosition(GetMaxPosition());
					}
					styleController->SetPageSize(pageSize);
					PageSizeChanged.Execute(GetNotifyEventArguments());
				}
			}

			vint GuiScroll::GetPosition()
			{
				return position;
			}

			void GuiScroll::SetPosition(vint value)
			{
				vint min=GetMinPosition();
				vint max=GetMaxPosition();
				vint newPosition=
					value<min?min:
					value>max?max:
					value;
				if(position!=newPosition)
				{
					position=newPosition;
					styleController->SetPosition(position);
					PositionChanged.Execute(GetNotifyEventArguments());
				}
			}

			vint GuiScroll::GetSmallMove()
			{
				return smallMove;
			}

			void GuiScroll::SetSmallMove(vint value)
			{
				if(value>0 && smallMove!=value)
				{
					smallMove=value;
					SmallMoveChanged.Execute(GetNotifyEventArguments());
				}
			}

			vint GuiScroll::GetBigMove()
			{
				return bigMove;
			}

			void GuiScroll::SetBigMove(vint value)
			{
				if(value>0 && bigMove!=value)
				{
					bigMove=value;
					BigMoveChanged.Execute(GetNotifyEventArguments());
				}
			}

			vint GuiScroll::GetMinPosition()
			{
				return 0;
			}

			vint GuiScroll::GetMaxPosition()
			{
				return totalSize-pageSize;
			}
		}
	}
}