#include "GuiMenuControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
IGuiMenuService
***********************************************************************/

			const wchar_t* const IGuiMenuService::Identifier = L"vl::presentation::controls::IGuiMenuService";

			IGuiMenuService::IGuiMenuService()
				:openingMenu(0)
			{
			}

			void IGuiMenuService::MenuItemExecuted()
			{
				if(openingMenu)
				{
					openingMenu->Hide();
				}
				if(GetParentMenuService())
				{
					GetParentMenuService()->MenuItemExecuted();
				}
			}

			GuiMenu* IGuiMenuService::GetOpeningMenu()
			{
				return openingMenu;
			}

			void IGuiMenuService::MenuOpened(GuiMenu* menu)
			{
				if(openingMenu!=menu && openingMenu)
				{
					openingMenu->Hide();
				}
				openingMenu=menu;
			}

			void IGuiMenuService::MenuClosed(GuiMenu* menu)
			{
				if(openingMenu==menu)
				{
					openingMenu=0;
				}
			}

/***********************************************************************
GuiMenu
***********************************************************************/

			IGuiMenuService* GuiMenu::GetParentMenuService()
			{
				return parentMenuService;
			}

			IGuiMenuService::Direction GuiMenu::GetPreferredDirection()
			{
				return IGuiMenuService::Vertical;
			}

			bool GuiMenu::IsActiveState()
			{
				return true;
			}

			void GuiMenu::MenuItemExecuted()
			{
				IGuiMenuService::MenuItemExecuted();
				Hide();
			}

			void GuiMenu::OnWindowOpened(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				if(parentMenuService)
				{
					parentMenuService->MenuOpened(this);
				}
			}

			void GuiMenu::MouseClickedOnOtherWindow(GuiWindow* window)
			{
				GuiMenu* targetMenu=dynamic_cast<GuiMenu*>(window);
				if(!targetMenu)
				{
					Hide();
				}
			}

			void GuiMenu::OnWindowClosed(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				if(parentMenuService)
				{
					parentMenuService->MenuClosed(this);
					GuiMenu* openingSubMenu=GetOpeningMenu();
					if(openingSubMenu)
					{
						openingSubMenu->Hide();
					}
				}
			}

			GuiMenu::GuiMenu(GuiControl::IStyleController* _styleController, GuiControl* _owner)
				:GuiPopup(_styleController)
				,owner(_owner)
				,parentMenuService(0)
			{
				GetNativeWindow()->SetAlwaysPassFocusToParent(true);
				UpdateMenuService();
				WindowOpened.AttachMethod(this, &GuiMenu::OnWindowOpened);
				WindowClosed.AttachMethod(this, &GuiMenu::OnWindowClosed);
			}

			GuiMenu::~GuiMenu()
			{
			}

			void GuiMenu::UpdateMenuService()
			{
				if(owner)
				{
					parentMenuService=owner->QueryService<IGuiMenuService>();
				}
			}

			IDescriptable* GuiMenu::QueryService(const WString& identifier)
			{
				if(identifier==IGuiMenuService::Identifier)
				{
					return (IGuiMenuService*)this;
				}
				else
				{
					return GuiPopup::QueryService(identifier);
				}
			}

/***********************************************************************
GuiMenuBar
***********************************************************************/

			IGuiMenuService* GuiMenuBar::GetParentMenuService()
			{
				return 0;
			}

			IGuiMenuService::Direction GuiMenuBar::GetPreferredDirection()
			{
				return IGuiMenuService::Horizontal;
			}

			bool GuiMenuBar::IsActiveState()
			{
				return GetOpeningMenu()!=0;
			}

			GuiMenuBar::GuiMenuBar(GuiControl::IStyleController* _styleController)
				:GuiControl(_styleController)
			{
			}

			GuiMenuBar::~GuiMenuBar()
			{
			}

			IDescriptable* GuiMenuBar::QueryService(const WString& identifier)
			{
				if(identifier==IGuiMenuService::Identifier)
				{
					return (IGuiMenuService*)this;
				}
				else
				{
					return GuiControl::QueryService(identifier);
				}
			}

/***********************************************************************
GuiMenuButton
***********************************************************************/

			GuiButton* GuiMenuButton::GetSubMenuHost()
			{
				GuiButton* button=styleController->GetSubMenuHost();
				return button?button:this;
			}

			void GuiMenuButton::OpenSubMenuInternal()
			{
				if(!GetSubMenuOpening())
				{
					if(ownerMenuService)
					{
						GuiMenu* openingSiblingMenu=ownerMenuService->GetOpeningMenu();
						if(openingSiblingMenu)
						{
							openingSiblingMenu->Hide();
						}
					}
					SetSubMenuOpening(true);
				}
			}

			void GuiMenuButton::OnParentLineChanged()
			{
				GuiButton::OnParentLineChanged();
				ownerMenuService=QueryService<IGuiMenuService>();
				if(subMenu)
				{
					subMenu->UpdateMenuService();
				}
			}

			void GuiMenuButton::OnSubMenuWindowOpened(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				SubMenuOpeningChanged.Execute(GetNotifyEventArguments());
				styleController->SetSubMenuOpening(true);
			}

			void GuiMenuButton::OnSubMenuWindowClosed(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				SubMenuOpeningChanged.Execute(GetNotifyEventArguments());
				styleController->SetSubMenuOpening(false);
			}

			void GuiMenuButton::OnLeftButtonDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
			{
				if(!GetSubMenu() && GetVisuallyEnabled())
				{
					if(ownerMenuService)
					{
						ownerMenuService->MenuItemExecuted();
					}
				}
			}

			void GuiMenuButton::OnMouseEnter(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				if(GetVisuallyEnabled())
				{
					if(ownerMenuService && ownerMenuService->IsActiveState())
					{
						OpenSubMenuInternal();
					}
				}
			}

			void GuiMenuButton::OnClicked(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				if(GetSubMenu())
				{
					OpenSubMenuInternal();
				}
			}

			GuiMenuButton::GuiMenuButton(IStyleController* _styleController)
				:GuiButton(_styleController)
				,styleController(_styleController)
				,subMenu(0)
				,ownedSubMenu(false)
				,ownerMenuService(0)
			{
				SetClickOnMouseUp(false);
				SubMenuOpeningChanged.SetAssociatedComposition(boundsComposition);
				GetSubMenuHost()->Clicked.AttachMethod(this, &GuiMenuButton::OnClicked);
				GetSubMenuHost()->GetEventReceiver()->leftButtonDown.AttachMethod(this, &GuiMenuButton::OnLeftButtonDown);
				GetSubMenuHost()->GetEventReceiver()->mouseEnter.AttachMethod(this, &GuiMenuButton::OnMouseEnter);
			}

			GuiMenuButton::~GuiMenuButton()
			{
				if(subMenu && ownedSubMenu)
				{
					delete subMenu;
				}
			}

			bool GuiMenuButton::IsSubMenuExists()
			{
				return subMenu!=0;
			}

			GuiMenu* GuiMenuButton::GetSubMenu()
			{
				return subMenu;
			}

			void GuiMenuButton::CreateSubMenu(GuiMenu::IStyleController* subMenuStyleController)
			{
				if(!subMenu)
				{
					subMenu=new GuiMenu(subMenuStyleController?subMenuStyleController:styleController->CreateSubMenuStyleController(), this);
					subMenu->WindowOpened.AttachMethod(this, &GuiMenuButton::OnSubMenuWindowOpened);
					subMenu->WindowClosed.AttachMethod(this, &GuiMenuButton::OnSubMenuWindowClosed);
					styleController->SetSubMenuExisting(true);
					ownedSubMenu=true;
				}
			}

			void GuiMenuButton::SetSubMenu(GuiMenu* value)
			{
				if(subMenu)
				{
					if(ownedSubMenu)
					{
						delete subMenu;
					}
				}
				subMenu=value;
				ownedSubMenu=false;
				styleController->SetSubMenuExisting(subMenu!=0);
			}

			void GuiMenuButton::DestroySubMenu()
			{
				if(subMenu)
				{
					if(ownedSubMenu)
					{
						delete subMenu;
					}
					subMenu=0;
					ownedSubMenu=false;
					styleController->SetSubMenuExisting(false);
				}
			}

			bool GuiMenuButton::GetOwnedSubMenu()
			{
				return subMenu && ownedSubMenu;
			}

			bool GuiMenuButton::GetSubMenuOpening()
			{
				if(subMenu)
				{
					return subMenu->GetOpening();
				}
				else
				{
					return false;
				}
			}

			void GuiMenuButton::SetSubMenuOpening(bool value)
			{
				if(subMenu)
				{
					if(value)
					{
						subMenu->SetClientSize(preferredMenuClientSize);
						IGuiMenuService::Direction direction=ownerMenuService?ownerMenuService->GetPreferredDirection():IGuiMenuService::Horizontal;
						subMenu->ShowPopup(GetSubMenuHost(), direction==IGuiMenuService::Horizontal);
					}
					else
					{
						subMenu->Close();
					}
				}
			}

			Size GuiMenuButton::GetPreferredMenuClientSize()
			{
				return preferredMenuClientSize;
			}

			void GuiMenuButton::SetPreferredMenuClientSize(Size value)
			{
				preferredMenuClientSize=value;
			}
		}
	}
}