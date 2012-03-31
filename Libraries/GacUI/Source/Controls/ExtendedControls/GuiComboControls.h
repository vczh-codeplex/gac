/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUICOMBOCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUICOMBOCONTROLS

#include "..\GuiWindowControls.h"
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

			class GuiComboBoxBase : public GuiButton, public Description<GuiComboBoxBase>
			{
			public:
				class ICommandExecutor : public virtual IDescriptable, public Description<ICommandExecutor>
				{
				public:
					virtual void							ShowPopup()=0;
					virtual void							SelectItem()=0;
				};

				class IStyleController : public virtual GuiButton::IStyleController, public Description<IStyleController>
				{
				public:
					virtual void							SetCommandExecutor(ICommandExecutor* value)=0;
					virtual void							OnClicked()=0;
					virtual void							OnPopupOpened()=0;
					virtual void							OnPopupClosed()=0;
					virtual void							OnItemSelected()=0;
					virtual GuiControl::IStyleController*	CreatePopupStyle()=0;
				};
			protected:

				class CommandExecutor : public Object, public virtual ICommandExecutor
				{
				protected:
					GuiComboBoxBase*						combo;

				public:
					CommandExecutor(GuiComboBoxBase* _combo);
					~CommandExecutor();

					void									ShowPopup()override;
					void									SelectItem()override;
				};

				Ptr<CommandExecutor>						commandExecutor;
				IStyleController*							styleController;
				GuiPopup*									popup;

				virtual void								SelectItem();
				void										OnClicked(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnPopupOpened(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnPopupClosed(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
			public:
				GuiComboBoxBase(IStyleController* _styleController);
				~GuiComboBoxBase();

				compositions::GuiNotifyEvent				PopupOpened;
				compositions::GuiNotifyEvent				PopupClosed;
				compositions::GuiNotifyEvent				ItemSelecting;
				compositions::GuiNotifyEvent				ItemSelected;

				void										ShowPopup();
				GuiPopup*									GetPopup();
			};

/***********************************************************************
ComboBox with GuiListControl
***********************************************************************/

			class GuiComboBoxListControl : public GuiComboBoxBase, public Description<GuiComboBoxListControl>
			{
			protected:
				GuiSelectableListControl*					containedListControl;
				GuiListControl::IItemPrimaryTextView*		primaryTextView;

				virtual void								DisplaySelectedContent(int itemIndex);
				void										OnListControlSelectionChanged(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
			public:
				GuiComboBoxListControl(IStyleController* _styleController, GuiSelectableListControl* _containedListControl);
				~GuiComboBoxListControl();

				compositions::GuiNotifyEvent				SelectedIndexChanged;
				
				void										SetFont(const FontProperties& value)override;
				GuiSelectableListControl*					GetContainedListControl();

				int											GetSelectedIndex();
				void										SetSelectedIndex(int value);
				GuiListControl::IItemProvider*				GetItemProvider();
			};
		}
	}
}

#endif