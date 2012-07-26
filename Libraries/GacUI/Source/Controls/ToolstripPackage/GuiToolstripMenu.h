/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUITOOLSTRIPMENU
#define VCZH_PRESENTATION_CONTROLS_GUITOOLSTRIPMENU

#include "GuiToolstripCommand.h"
#include "..\ExtendedControls\GuiMenuControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			/// <summary>Toolstrip button that can connect with a <see cref="GuiToolstripCommand"/>.</summary>
			class GuiToolstripButton : public GuiMenuButton, public Description<GuiToolstripButton>
			{
			protected:
				Ptr<GuiToolstripCommand>						command;
				Ptr<compositions::GuiNotifyEvent::IHandler>		descriptionChangedHandler;

				void											UpdateCommandContent();
				void											OnClicked(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void											OnCommandDescriptionChanged(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
			public:
				/// <summary>Create a control with a specified style controller.</summary>
				/// <param name="_styleController">The style controller.</param>
				GuiToolstripButton(IStyleController* _styleController);
				~GuiToolstripButton();

				/// <summary>Get the attached <see cref="GuiToolstripCommand"/>.</summary>
				/// <returns>The attached toolstrip command.</returns>
				Ptr<GuiToolstripCommand>						GetCommand();
				/// <summary>Detach from the previous <see cref="GuiToolstripCommand"/> and attach to a new one. If the command is null, this function only do detaching.</summary>
				/// <param name="value">The new toolstrip command.</param>
				void											SetCommand(Ptr<GuiToolstripCommand> value);
			};

			/// <summary>Toolstrip item collection.</summary>
			class GuiToolstripCollection : public Object, public collections::IList<GuiControl*>
			{
			protected:
				compositions::GuiStackComposition*			stackComposition;
				Ptr<compositions::GuiSubComponentMeasurer>	subComponentMeasurer;
				collections::List<GuiControl*>				items;

				void										RemoveAtInternal(vint index);
				void										InsertInternal(vint index, GuiControl* control);
			public:
				GuiToolstripCollection(compositions::GuiStackComposition* _stackComposition, Ptr<compositions::GuiSubComponentMeasurer> _subComponentMeasurer);
				~GuiToolstripCollection();

				collections::IEnumerator<GuiControl*>*		CreateEnumerator()const override;
				bool										Contains(GuiControl* const& item)const override;
				vint										Count()const override;
				GuiControl* const&							Get(vint index)const override;
				GuiControl* const&							operator[](vint index)const override;
				vint										IndexOf(GuiControl* const& item)const override;
				vint										Add(GuiControl* const& item)override;
				bool										Remove(GuiControl* const& item)override;
				bool										RemoveAt(vint index)override;
				bool										RemoveRange(vint index, vint count)override;
				bool										Clear()override;
				vint										Insert(vint index, GuiControl* const& item)override;
				bool										Set(vint index, GuiControl* const& item)override;
			};

			/// <summary>Toolstrip menu.</summary>
			class GuiToolstripMenu : public GuiMenu, public Description<GuiToolstripMenu>
			{
			public:
				/// <summary>Create a control with a specified style controller.</summary>
				/// <param name="_styleController">The style controller.</param>
				/// <param name="_owner">The owner menu item of the parent menu.</param>
				GuiToolstripMenu(IStyleController* _styleController, GuiControl* _owner);
				~GuiToolstripMenu();
			};

			/// <summary>Toolstrip menu bar.</summary>
			class GuiToolstripMenuBar : public GuiMenuBar, public Description<GuiToolstripMenuBar>
			{
			public:
				/// <summary>Create a control with a specified style controller.</summary>
				/// <param name="_styleController">The style controller.</param>
				GuiToolstripMenuBar(IStyleController* _styleController);
				~GuiToolstripMenuBar();
			};

			/// <summary>Toolstrip tool bar.</summary>
			class GuiToolstripToolbar : public GuiControl, public Description<GuiToolstripToolbar>
			{
			public:
				/// <summary>Create a control with a specified style controller.</summary>
				/// <param name="_styleController">The style controller.</param>
				GuiToolstripToolbar(IStyleController* _styleController);
				~GuiToolstripToolbar();
			};
		}
	}
}

#endif