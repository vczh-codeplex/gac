/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUIMENUCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUIMENUCONTROLS

#include "..\GuiWindowControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
Menu Service
***********************************************************************/

			class GuiMenu;

			/// <summary>IGuiMenuService is a required service for menu item container.</summary>
			class IGuiMenuService : public virtual IDescriptable, public Description<IGuiMenuService>
			{
			public:
				/// <summary>The identifier for this service.</summary>
				static const wchar_t* const				Identifier;

				/// <summary>Direction to decide the position for a menu with specified control.</summary>
				enum Direction
				{
					/// <summary>[T:vl.presentation.controls.IGuiMenuService.Direction]Aligned to the top or bottom side.</summary>
					Horizontal,
					/// <summary>[T:vl.presentation.controls.IGuiMenuService.Direction]Aligned to the left or right side.</summary>
					Vertical,
				};
			protected:
				GuiMenu*								openingMenu;
			public:
				IGuiMenuService();

				/// <summary>Get the parent service. This service represents the parent menu that host the menu item control that contains this menu.</summary>
				/// <returns>The parent service.</returns>
				virtual IGuiMenuService*				GetParentMenuService()=0;
				/// <summary>Get the preferred direction to open the sub menu.</summary>
				/// <returns>The preferred direction to open the sub menu.</returns>
				virtual Direction						GetPreferredDirection()=0;
				/// <summary>Test is this menu is active. When an menu is active, the sub menu is automatically opened when the corresponding menu item is opened.</summary>
				/// <returns>Returns true if this menu is active.</returns>
				virtual bool							IsActiveState()=0;

				/// <summary>Called when the menu item is executed.</summary>
				virtual void							MenuItemExecuted();
				/// <summary>Get the opening sub menu.</summary>
				/// <returns>The opening sub menu.</returns>
				virtual GuiMenu*						GetOpeningMenu();
				/// <summary>Called when the sub menu is opened.</summary>
				/// <param name="menu">The sub menu.</param>
				virtual void							MenuOpened(GuiMenu* menu);
				/// <summary>Called when the sub menu is closed.</summary>
				/// <param name="menu">The sub menu.</param>
				virtual void							MenuClosed(GuiMenu* menu);
			};

/***********************************************************************
Menu
***********************************************************************/

			/// <summary>Popup menu.</summary>
			class GuiMenu : public GuiPopup, private IGuiMenuService, public Description<GuiMenu>
			{
			private:
				IGuiMenuService*						parentMenuService;

				IGuiMenuService*						GetParentMenuService()override;
				Direction								GetPreferredDirection()override;
				bool									IsActiveState()override;
				void									MenuItemExecuted()override;
			protected:
				GuiControl*								owner;

				void									MouseClickedOnOtherWindow(GuiWindow* window)override;
				void									OnWindowOpened(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void									OnWindowClosed(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
			public:
				/// <summary>Create a control with a specified style controller.</summary>
				/// <param name="_styleController">The style controller.</param>
				/// <param name="_owner">The owner menu item of the parent menu.</param>
				GuiMenu(GuiControl::IStyleController* _styleController, GuiControl* _owner);
				~GuiMenu();

				/// <summary>Update the reference to the parent <see cref="IGuiMenuService"/>. This function is not required to call outside the menu or menu item control.</summary>
				void									UpdateMenuService();
				IDescriptable*							QueryService(const WString& identifier)override;
			};
			
			/// <summary>Menu bar.</summary>
			class GuiMenuBar : public GuiControl, private IGuiMenuService, public Description<GuiMenuBar>
			{
			private:
				IGuiMenuService*						GetParentMenuService()override;
				Direction								GetPreferredDirection()override;
				bool									IsActiveState()override;
			public:
				/// <summary>Create a control with a specified style controller.</summary>
				/// <param name="_styleController">The style controller.</param>
				GuiMenuBar(GuiControl::IStyleController* _styleController);
				~GuiMenuBar();
				
				IDescriptable*							QueryService(const WString& identifier)override;
			};

/***********************************************************************
MenuButton
***********************************************************************/

			/// <summary>Menu item.</summary>
			class GuiMenuButton : public GuiButton, public Description<GuiMenuButton>
			{
			public:
				/// <summary>Style controller interface for <see cref="GuiMenuButton"/>.</summary>
				class IStyleController : public GuiButton::IStyleController, public Description<IStyleController>
				{
				public:
					/// <summary>Create a style controller for the sub menu.</summary>
					/// <returns>The style controller for the sub menu.</returns>
					virtual GuiMenu::IStyleController*	CreateSubMenuStyleController()=0;
					/// <summary>Notify that the sub menu is created or destroyed.</summary>
					/// <param name="value">Set to true if the sub menu is created.</param>
					virtual void						SetSubMenuExisting(bool value)=0;
					/// <summary>Notify that the sub menu is opened or closed.</summary>
					/// <param name="value">Set to true if the sub menu is opened.</param>
					virtual void						SetSubMenuOpening(bool value)=0;
					/// <summary>Get the button control that is expected to be associated with a sub menu.</summary>
					/// <returns>The button control that is expected to be associated with a sub menu. Returns null means that the sub menu will be directly associated to the menu button.</returns>
					virtual GuiButton*					GetSubMenuHost()=0;
				};
			protected:
				IStyleController*						styleController;
				GuiMenu*								subMenu;
				bool									ownedSubMenu;
				Size									preferredMenuClientSize;
				IGuiMenuService*						ownerMenuService;

				GuiButton*								GetSubMenuHost();
				void									OpenSubMenuInternal();
				void									OnParentLineChanged()override;
				void									OnSubMenuWindowOpened(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void									OnSubMenuWindowClosed(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void									OnLeftButtonDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
				void									OnMouseEnter(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void									OnClicked(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
			public:
				/// <summary>Create a control with a specified style controller.</summary>
				/// <param name="_styleController">The style controller.</param>
				GuiMenuButton(IStyleController* _styleController);
				~GuiMenuButton();

				/// <summary>Sub menu opening changed event.</summary>
				compositions::GuiNotifyEvent			SubMenuOpeningChanged;

				/// <summary>Test does the sub menu exist.</summary>
				/// <returns>Returns true if the sub menu exists.</returns>
				bool									IsSubMenuExists();
				/// <summary>Get the sub menu. If the sub menu is not created, it returns null.</summary>
				/// <returns>The sub menu.</returns>
				GuiMenu*								GetSubMenu();
				/// <summary>Create the sub menu if necessary. The created sub menu is owned by this menu button.</summary>
				/// <param name="subMenuStyleController">The style controller for the sub menu. If this argument is null, it will call <see cref="IStyleController::CreateSubMenuStyleController"/> for a style controller.</param>
				void									CreateSubMenu(GuiMenu::IStyleController* subMenuStyleController=0);
				/// <summary>Associate a sub menu if there is no sub menu binded in this menu button. The associated sub menu is not owned by this menu button.</summary>
				/// <param name="value">The sub menu to associate.</param>
				void									SetSubMenu(GuiMenu* value);
				/// <summary>Destroy the sub menu if necessary.</summary>
				void									DestroySubMenu();
				/// <summary>Test is the sub menu owned by this menu button. If the sub menu is owned, both deleting this menu button or calling <see cref="DestroySubMenu"/> will delete the sub menu.</summary>
				/// <returns>Returns true if the sub menu is owned by this menu button.</returns>
				bool									GetOwnedSubMenu();

				/// <summary>Test is the sub menu opened.</summary>
				/// <returns>Returns true if the sub menu is opened.</returns>
				bool									GetSubMenuOpening();
				/// <summary>Open or close the sub menu.</summary>
				/// <param name="value">Set to true to open the sub menu.</param>
				void									SetSubMenuOpening(bool value);

				/// <summary>Get the preferred client size for the sub menu.</summary>
				/// <returns>The preferred client size for the sub menu.</returns>
				Size									GetPreferredMenuClientSize();
				/// <summary>Set the preferred client size for the sub menu.</summary>
				/// <param name="value">The preferred client size for the sub menu.</param>
				void									SetPreferredMenuClientSize(Size value);
			};
		}
	}
}

#endif