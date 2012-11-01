#include "GuiWindowControls.h"
#include "GuiApplication.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace elements;
			using namespace compositions;

/***********************************************************************
GuiControlHost
***********************************************************************/

			void GuiControlHost::OnNativeWindowChanged()
			{
			}

			void GuiControlHost::OnVisualStatusChanged()
			{
			}

			void GuiControlHost::Moved()
			{
				OnVisualStatusChanged();
			}

			void GuiControlHost::Enabled()
			{
				GuiControl::SetEnabled(true);
				OnVisualStatusChanged();
			}

			void GuiControlHost::Disabled()
			{
				GuiControl::SetEnabled(false);
				OnVisualStatusChanged();
			}

			void GuiControlHost::GotFocus()
			{
				WindowGotFocus.Execute(GetNotifyEventArguments());
				OnVisualStatusChanged();
			}

			void GuiControlHost::LostFocus()
			{
				WindowLostFocus.Execute(GetNotifyEventArguments());
				OnVisualStatusChanged();
			}

			void GuiControlHost::Activated()
			{
				WindowActivated.Execute(GetNotifyEventArguments());
				OnVisualStatusChanged();
			}

			void GuiControlHost::Deactivated()
			{
				WindowDeactivated.Execute(GetNotifyEventArguments());
				OnVisualStatusChanged();
			}

			void GuiControlHost::Opened()
			{
				WindowOpened.Execute(GetNotifyEventArguments());
			}

			void GuiControlHost::Closing(bool& cancel)
			{
				GuiRequestEventArgs arguments(GetStyleController()->GetBoundsComposition());
				arguments.cancel=cancel;
				WindowClosing.Execute(arguments);
				if(!arguments.handled)
				{
					cancel=arguments.cancel;
				}
			}

			void GuiControlHost::Closed()
			{
				WindowClosed.Execute(GetNotifyEventArguments());
			}

			void GuiControlHost::Destroying()
			{
				WindowDestroying.Execute(GetNotifyEventArguments());
				SetNativeWindow(0);
			}

			GuiControlHost::GuiControlHost(GuiControl::IStyleController* _styleController)
				:GuiControl(_styleController)
			{
				GetStyleController()->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				
				WindowGotFocus.SetAssociatedComposition(GetStyleController()->GetBoundsComposition());
				WindowLostFocus.SetAssociatedComposition(GetStyleController()->GetBoundsComposition());
				WindowActivated.SetAssociatedComposition(GetStyleController()->GetBoundsComposition());
				WindowDeactivated.SetAssociatedComposition(GetStyleController()->GetBoundsComposition());
				WindowOpened.SetAssociatedComposition(GetStyleController()->GetBoundsComposition());
				WindowClosing.SetAssociatedComposition(GetStyleController()->GetBoundsComposition());
				WindowClosed.SetAssociatedComposition(GetStyleController()->GetBoundsComposition());
				WindowDestroying.SetAssociatedComposition(GetStyleController()->GetBoundsComposition());

				host=new GuiGraphicsHost;
				host->GetMainComposition()->AddChild(GetStyleController()->GetBoundsComposition());
			}

			GuiControlHost::~GuiControlHost()
			{
				OnBeforeReleaseGraphicsHost();
				styleController=0;
				for(int i=0;i<components.Count();i++)
				{
					delete components[i];
				}
				delete host;
			}

			compositions::GuiGraphicsHost* GuiControlHost::GetGraphicsHost()
			{
				return host;
			}

			compositions::GuiGraphicsComposition* GuiControlHost::GetMainComposition()
			{
				return host->GetMainComposition();
			}

			INativeWindow* GuiControlHost::GetNativeWindow()
			{
				return host->GetNativeWindow();
			}

			void GuiControlHost::SetNativeWindow(INativeWindow* window)
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->UninstallListener(this);
				}
				host->SetNativeWindow(window);
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->InstallListener(this);
				}
				OnNativeWindowChanged();
			}

			void GuiControlHost::ForceCalculateSizeImmediately()
			{
				boundsComposition->ForceCalculateSizeImmediately();
				SetBounds(GetBounds());
			}

			void GuiControlHost::Render()
			{
				host->Render();
			}

			bool GuiControlHost::GetEnabled()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->IsEnabled();
				}
				else
				{
					return false;
				}
			}

			void GuiControlHost::SetEnabled(bool value)
			{
				if(host->GetNativeWindow())
				{
					if(value)
					{
						host->GetNativeWindow()->Enable();
					}
					else
					{
						host->GetNativeWindow()->Disable();
					}
				}
			}

			bool GuiControlHost::GetFocused()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->IsFocused();
				}
				else
				{
					return false;
				}
			}

			void GuiControlHost::SetFocused()
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->SetFocus();
				}
			}

			bool GuiControlHost::GetActivated()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->IsActivated();
				}
				else
				{
					return false;
				}
			}

			void GuiControlHost::SetActivated()
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->SetActivate();
				}
			}

			bool GuiControlHost::GetShowInTaskBar()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->IsAppearedInTaskBar();
				}
				else
				{
					return false;
				}
			}

			void GuiControlHost::SetShowInTaskBar(bool value)
			{
				if(host->GetNativeWindow())
				{
					if(value)
					{
						host->GetNativeWindow()->ShowInTaskBar();
					}
					else
					{
						host->GetNativeWindow()->HideInTaskBar();
					}
				}
			}

			bool GuiControlHost::GetEnabledActivate()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->IsEnabledActivate();
				}
				else
				{
					return false;
				}
			}

			void GuiControlHost::SetEnabledActivate(bool value)
			{
				if(host->GetNativeWindow())
				{
					if(value)
					{
						host->GetNativeWindow()->EnableActivate();
					}
					else
					{
						host->GetNativeWindow()->DisableActivate();
					}
				}
			}

			bool GuiControlHost::GetTopMost()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->GetTopMost();
				}
				else
				{
					return false;
				}
			}

			void GuiControlHost::SetTopMost(bool topmost)
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->SetTopMost(topmost);
				}
			}

			bool GuiControlHost::AddComponent(GuiComponent* component)
			{
				if(components.Contains(component))
				{
					return false;
				}
				else
				{
					components.Add(component);
					return true;
				}
			}

			bool GuiControlHost::RemoveComponent(GuiComponent* component)
			{
				return components.Remove(component);
			}

			bool GuiControlHost::ContainsComponent(GuiComponent* component)
			{
				return components.Contains(component);
			}

			compositions::IGuiShortcutKeyManager* GuiControlHost::GetShortcutKeyManager()
			{
				return host->GetShortcutKeyManager();
			}

			void GuiControlHost::SetShortcutKeyManager(compositions::IGuiShortcutKeyManager* value)
			{
				host->SetShortcutKeyManager(value);
			}

			Size GuiControlHost::GetClientSize()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->GetClientSize();
				}
				else
				{
					return Size(0, 0);
				}
			}

			void GuiControlHost::SetClientSize(Size value)
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->SetClientSize(value);
				}
			}

			Rect GuiControlHost::GetBounds()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->GetBounds();
				}
				else
				{
					return Rect();
				}
			}

			void GuiControlHost::SetBounds(Rect value)
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->SetBounds(value);
				}
			}

			GuiControlHost* GuiControlHost::GetRelatedControlHost()
			{
				return this;
			}

			const WString& GuiControlHost::GetText()
			{
				WString result;
				if(host->GetNativeWindow())
				{
					result=host->GetNativeWindow()->GetTitle();
				}
				if(result!=GuiControl::GetText())
				{
					GuiControl::SetText(result);
				}
				return GuiControl::GetText();
			}

			void GuiControlHost::SetText(const WString& value)
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->SetTitle(value);
					GuiControl::SetText(value);
				}
			}

			INativeScreen* GuiControlHost::GetRelatedScreen()
			{
				if(host->GetNativeWindow())
				{
					return GetCurrentController()->ScreenService()->GetScreen(host->GetNativeWindow());
				}
				else
				{
					return 0;
				}
			}

			void GuiControlHost::Show()
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->Show();
				}
			}

			void GuiControlHost::ShowDeactivated()
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->ShowDeactivated();
				}
			}

			void GuiControlHost::ShowRestored()
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->ShowRestored();
				}
			}

			void GuiControlHost::ShowMaximized()
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->ShowMaximized();
				}
			}

			void GuiControlHost::ShowMinimized()
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->ShowMinimized();
				}
			}

			void GuiControlHost::Hide()
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->Hide();
				}
			}

			void GuiControlHost::Close()
			{
				INativeWindow* window=host->GetNativeWindow();
				if(window)
				{
					if(GetCurrentController()->WindowService()->GetMainWindow()!=window)
					{
						window->Hide();
					}
					else
					{
						SetNativeWindow(0);
						GetCurrentController()->WindowService()->DestroyNativeWindow(window);
					}
				}
			}

			bool GuiControlHost::GetOpening()
			{
				INativeWindow* window=host->GetNativeWindow();
				if(window)
				{
					return window->IsVisible();
				}
				return false;
			}

/***********************************************************************
GuiWindow::DefaultBehaviorStyleController
***********************************************************************/

			GuiWindow::DefaultBehaviorStyleController::DefaultBehaviorStyleController()
				:window(0)
			{
			}

			GuiWindow::DefaultBehaviorStyleController::~DefaultBehaviorStyleController()
			{
			}

			void GuiWindow::DefaultBehaviorStyleController::AttachWindow(GuiWindow* _window)
			{
				window=_window;
			}

			void GuiWindow::DefaultBehaviorStyleController::InitializeNativeWindowProperties()
			{
			}

			bool GuiWindow::DefaultBehaviorStyleController::GetMaximizedBox()
			{
				if(window->GetNativeWindow())
				{
					return window->GetNativeWindow()->GetMaximizedBox();
				}
				else
				{
					return false;
				}
			}

			void GuiWindow::DefaultBehaviorStyleController::SetMaximizedBox(bool visible)
			{
				if(window->GetNativeWindow())
				{
					window->GetNativeWindow()->SetMaximizedBox(visible);
				}
			}

			bool GuiWindow::DefaultBehaviorStyleController::GetMinimizedBox()
			{
				if(window->GetNativeWindow())
				{
					return window->GetNativeWindow()->GetMinimizedBox();
				}
				else
				{
					return false;
				}
			}

			void GuiWindow::DefaultBehaviorStyleController::SetMinimizedBox(bool visible)
			{
				if(window->GetNativeWindow())
				{
					window->GetNativeWindow()->SetMinimizedBox(visible);
				}
			}

			bool GuiWindow::DefaultBehaviorStyleController::GetBorder()
			{
				if(window->GetNativeWindow())
				{
					return window->GetNativeWindow()->GetBorder();
				}
				else
				{
					return false;
				}
			}

			void GuiWindow::DefaultBehaviorStyleController::SetBorder(bool visible)
			{
				if(window->GetNativeWindow())
				{
					window->GetNativeWindow()->SetBorder(visible);
				}
			}

			bool GuiWindow::DefaultBehaviorStyleController::GetSizeBox()
			{
				if(window->GetNativeWindow())
				{
					return window->GetNativeWindow()->GetSizeBox();
				}
				else
				{
					return false;
				}
			}

			void GuiWindow::DefaultBehaviorStyleController::SetSizeBox(bool visible)
			{
				if(window->GetNativeWindow())
				{
					window->GetNativeWindow()->SetSizeBox(visible);
				}
			}

			bool GuiWindow::DefaultBehaviorStyleController::GetIconVisible()
			{
				if(window->GetNativeWindow())
				{
					return window->GetNativeWindow()->GetIconVisible();
				}
				else
				{
					return false;
				}
			}

			void GuiWindow::DefaultBehaviorStyleController::SetIconVisible(bool visible)
			{
				if(window->GetNativeWindow())
				{
					window->GetNativeWindow()->SetIconVisible(visible);
				}
			}

			bool GuiWindow::DefaultBehaviorStyleController::GetTitleBar()
			{
				if(window->GetNativeWindow())
				{
					return window->GetNativeWindow()->GetTitleBar();
				}
				else
				{
					return false;
				}
			}

			void GuiWindow::DefaultBehaviorStyleController::SetTitleBar(bool visible)
			{
				if(window->GetNativeWindow())
				{
					window->GetNativeWindow()->SetTitleBar(visible);
				}
			}

/***********************************************************************
GuiWindow
***********************************************************************/

			void GuiWindow::OnNativeWindowChanged()
			{
				styleController->InitializeNativeWindowProperties();
				GuiControlHost::OnNativeWindowChanged();
			}

			void GuiWindow::OnVisualStatusChanged()
			{
				GuiControlHost::OnVisualStatusChanged();
			}

			void GuiWindow::MouseClickedOnOtherWindow(GuiWindow* window)
			{
			}

			GuiWindow::GuiWindow(IStyleController* _styleController)
				:GuiControlHost(_styleController)
				,styleController(_styleController)
			{
				ClipboardUpdated.SetAssociatedComposition(GetStyleController()->GetBoundsComposition());
				INativeWindow* window=GetCurrentController()->WindowService()->CreateNativeWindow();
				styleController->AttachWindow(this);
				SetNativeWindow(window);
				GetApplication()->RegisterWindow(this);
			}

			GuiWindow::~GuiWindow()
			{
				GetApplication()->UnregisterWindow(this);
				INativeWindow* window=host->GetNativeWindow();
				if(window)
				{
					SetNativeWindow(0);
					GetCurrentController()->WindowService()->DestroyNativeWindow(window);
				}
			}

			void GuiWindow::MoveToScreenCenter()
			{
				INativeScreen* screen=GetRelatedScreen();
				if(screen)
				{
					Rect screenBounds=screen->GetClientBounds();
					Rect windowBounds=GetBounds();
					SetBounds(
						Rect(
							Point(
								windowBounds.Left()+(screenBounds.Width()-windowBounds.Width())/2,
								windowBounds.Top()+(screenBounds.Height()-windowBounds.Height())/2
								),
							windowBounds.GetSize()
							)
						);
				}
			}

			bool GuiWindow::GetMaximizedBox()
			{
				return styleController->GetMaximizedBox();
			}

			void GuiWindow::SetMaximizedBox(bool visible)
			{
				styleController->SetMaximizedBox(visible);
			}

			bool GuiWindow::GetMinimizedBox()
			{
				return styleController->GetMinimizedBox();
			}

			void GuiWindow::SetMinimizedBox(bool visible)
			{
				styleController->SetMinimizedBox(visible);
			}

			bool GuiWindow::GetBorder()
			{
				return styleController->GetBorder();
			}

			void GuiWindow::SetBorder(bool visible)
			{
				styleController->SetBorder(visible);
			}

			bool GuiWindow::GetSizeBox()
			{
				return styleController->GetSizeBox();
			}

			void GuiWindow::SetSizeBox(bool visible)
			{
				styleController->SetSizeBox(visible);
			}

			bool GuiWindow::GetIconVisible()
			{
				return styleController->GetIconVisible();
			}

			void GuiWindow::SetIconVisible(bool visible)
			{
				styleController->SetIconVisible(visible);
			}

			bool GuiWindow::GetTitleBar()
			{
				return styleController->GetTitleBar();
			}

			void GuiWindow::SetTitleBar(bool visible)
			{
				styleController->SetTitleBar(visible);
			}

/***********************************************************************
GuiPopup
***********************************************************************/

			void GuiPopup::MouseClickedOnOtherWindow(GuiWindow* window)
			{
				Hide();
			}

			void GuiPopup::PopupOpened(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				GetApplication()->RegisterPopupOpened(this);
			}

			void GuiPopup::PopupClosed(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				GetApplication()->RegisterPopupClosed(this);
			}

			GuiPopup::GuiPopup(IStyleController* _styleController)
				:GuiWindow(_styleController)
			{
				SetMinimizedBox(false);
				SetMaximizedBox(false);
				SetSizeBox(false);
				SetTitleBar(false);
				SetShowInTaskBar(false);
				GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);

				WindowOpened.AttachMethod(this, &GuiPopup::PopupOpened);
				WindowClosed.AttachMethod(this, &GuiPopup::PopupClosed);
			}

			GuiPopup::~GuiPopup()
			{
				GetApplication()->RegisterPopupClosed(this);
			}

			bool GuiPopup::IsClippedByScreen(Point location)
			{
				SetBounds(Rect(location, GetBounds().GetSize()));
				INativeWindow* window=GetNativeWindow();
				if(window)
				{
					INativeScreen* screen=GetCurrentController()->ScreenService()->GetScreen(window);
					if(screen)
					{
						Rect screenBounds=screen->GetClientBounds();
						Rect windowBounds=window->GetBounds();
						return !screenBounds.Contains(windowBounds.LeftTop()) || !screenBounds.Contains(windowBounds.RightBottom());
					}
				}
				return true;
			}

			void GuiPopup::ShowPopup(Point location)
			{
				INativeWindow* window=GetNativeWindow();
				if(window)
				{
					INativeScreen* screen=GetCurrentController()->ScreenService()->GetScreen(window);
					if(screen)
					{
						Rect screenBounds=screen->GetClientBounds();
						Size size=window->GetBounds().GetSize();

						if(location.x<screenBounds.x1)
						{
							location.x=screenBounds.x1;
						}
						else if(location.x+size.x>screenBounds.x2)
						{
							location.x=screenBounds.x2-size.x;
						}

						if(location.y<screenBounds.y1)
						{
							location.y=screenBounds.y1;
						}
						else if(location.y+size.y>screenBounds.y2)
						{
							location.y=screenBounds.y2-size.y;
						}
					}
					SetBounds(Rect(location, GetBounds().GetSize()));
					bool value=GetNativeWindow()->IsEnabledActivate();
					ShowDeactivated();
				}
			}

			void GuiPopup::ShowPopup(GuiControl* control, bool preferredTopBottomSide)
			{
				INativeWindow* window=GetNativeWindow();
				if(window)
				{
					Point locations[4];
					Size size=window->GetBounds().GetSize();
					Rect controlBounds=control->GetBoundsComposition()->GetGlobalBounds();

					GuiControlHost* controlHost=control->GetBoundsComposition()->GetRelatedControlHost();
					if(controlHost)
					{
						INativeWindow* controlWindow=controlHost->GetNativeWindow();
						if(controlWindow)
						{
							Point controlClientOffset=controlWindow->GetClientBoundsInScreen().LeftTop();
							controlBounds.x1+=controlClientOffset.x;
							controlBounds.x2+=controlClientOffset.x;
							controlBounds.y1+=controlClientOffset.y;
							controlBounds.y2+=controlClientOffset.y;

							if(preferredTopBottomSide)
							{
								locations[0]=Point(controlBounds.x1, controlBounds.y2);
								locations[1]=Point(controlBounds.x2-size.x, controlBounds.y2);
								locations[2]=Point(controlBounds.x1, controlBounds.y1-size.y);
								locations[3]=Point(controlBounds.x2-size.x, controlBounds.y1-size.y);
							}
							else
							{
								locations[0]=Point(controlBounds.x2, controlBounds.y1);
								locations[1]=Point(controlBounds.x2, controlBounds.y2-size.y);
								locations[2]=Point(controlBounds.x1-size.x, controlBounds.y1);
								locations[3]=Point(controlBounds.x1-size.x, controlBounds.y2-size.y);
							}

							window->SetParent(controlWindow);
							for(int i=0;i<4;i++)
							{
								if(!IsClippedByScreen(locations[i]))
								{
									ShowPopup(locations[i]);
									return;
								}
							}
							ShowPopup(locations[0]);
						}
					}
				}
			}
		}
	}
}