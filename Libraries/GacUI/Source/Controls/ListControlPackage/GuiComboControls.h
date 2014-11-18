/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUICOMBOCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUICOMBOCONTROLS

#include "../GuiWindowControls.h"
#include "GuiTextListControls.h"
#include "GuiListViewControls.h"
#include "GuiTreeViewControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
ComboBox Base
***********************************************************************/

			/// <summary>The base class of combo box control.</summary>
			class GuiComboBoxBase : public GuiMenuButton, public Description<GuiComboBoxBase>
			{
			public:
				/// <summary>A command executor for the combo box to change the control state.</summary>
				class ICommandExecutor : public virtual IDescriptable, public Description<ICommandExecutor>
				{
				public:
					/// <summary>Notify that an item is selected, the combo box should close the popup and show the text of the selected item.</summary>
					virtual void							SelectItem()=0;
				};
				
				/// <summary>Style controller interface for <see cref="GuiComboBoxBase"/>.</summary>
				class IStyleController : public virtual GuiMenuButton::IStyleController, public Description<IStyleController>
				{
				public:
					/// <summary>Called when the command executor is changed.</summary>
					/// <param name="value">The command executor.</param>
					virtual void							SetCommandExecutor(ICommandExecutor* value)=0;
					/// <summary>Notify that an item is selected.</summary>
					virtual void							OnItemSelected()=0;
				};
			protected:

				class CommandExecutor : public Object, public virtual ICommandExecutor
				{
				protected:
					GuiComboBoxBase*						combo;

				public:
					CommandExecutor(GuiComboBoxBase* _combo);
					~CommandExecutor();

					void									SelectItem()override;
				};

				Ptr<CommandExecutor>						commandExecutor;
				IStyleController*							styleController;

				IGuiMenuService::Direction					GetSubMenuDirection()override;
				virtual void								SelectItem();
				void										OnBoundsChanged(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
			public:
				/// <summary>Create a control with a specified style controller.</summary>
				/// <param name="_styleController">The style controller.</param>
				GuiComboBoxBase(IStyleController* _styleController);
				~GuiComboBoxBase();

				/// <summary>Item selected event.</summary>
				compositions::GuiNotifyEvent				ItemSelected;
			};

/***********************************************************************
ComboBox with GuiListControl
***********************************************************************/

			/// <summary>Combo box list control. This control is a combo box with a list control in its popup.</summary>
			class GuiComboBoxListControl : public GuiComboBoxBase, public Description<GuiComboBoxListControl>
			{
			protected:
				GuiSelectableListControl*					containedListControl;
				GuiListControl::IItemPrimaryTextView*		primaryTextView;

				virtual void								DisplaySelectedContent(vint itemIndex);
				void										OnListControlSelectionChanged(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
			public:
				/// <summary>Create a control with a specified style controller and a list control that will be put in the popup control to show all items.</summary>
				/// <param name="_styleController">The style controller.</param>
				/// <param name="_containedListControl">The list controller.</param>
				GuiComboBoxListControl(IStyleController* _styleController, GuiSelectableListControl* _containedListControl);
				~GuiComboBoxListControl();

				/// <summary>Selected index changed event.</summary>
				compositions::GuiNotifyEvent				SelectedIndexChanged;
				
				void										SetFont(const FontProperties& value)override;
				/// <summary>Get the list control.</summary>
				/// <returns>The list control.</returns>
				GuiSelectableListControl*					GetContainedListControl();
				
				/// <summary>Get the selected index.</summary>
				/// <returns>The selected index.</returns>
				vint										GetSelectedIndex();
				/// <summary>Set the selected index.</summary>
				/// <param name="value">The selected index.</param>
				void										SetSelectedIndex(vint value);
				/// <summary>Get the item provider in the list control.</summary>
				/// <returns>The item provider in the list control.</returns>
				GuiListControl::IItemProvider*				GetItemProvider();
			};
		}
	}
}

#endif