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
		namespace theme
		{
			class ITheme;
		}

		namespace controls
		{

/***********************************************************************
Toolstrip Item Collection
***********************************************************************/

			/// <summary>Toolstrip item collection.</summary>
			class GuiToolstripCollection : public Object, public collections::IList<GuiControl*>
			{
			public:
				class IContentCallback : public Interface
				{
				public:
					virtual void							UpdateLayout()=0;
				};
			protected:
				IContentCallback*							contentCallback;
				compositions::GuiStackComposition*			stackComposition;
				Ptr<compositions::GuiSubComponentMeasurer>	subComponentMeasurer;
				collections::List<GuiControl*>				items;

				void										InvokeUpdateLayout();
				void										OnInterestingMenuButtonPropertyChanged(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										RemoveAtInternal(vint index);
				void										InsertInternal(vint index, GuiControl* control);
			public:
				GuiToolstripCollection(IContentCallback* _contentCallback, compositions::GuiStackComposition* _stackComposition, Ptr<compositions::GuiSubComponentMeasurer> _subComponentMeasurer);
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

/***********************************************************************
Toolstrip Builder Facade
***********************************************************************/

			class GuiToolstripButton;

			/// <summary>Toolstrip builder.</summary>
			class GuiToolstripBuilder : public Object
			{
				friend class GuiToolstripMenu;
				friend class GuiToolstripMenuBar;
				friend class GuiToolstripToolbar;
			protected:
				enum Environment
				{
					Menu,
					MenuBar,
					Toolbar,
				};

				Environment									environment;
				GuiToolstripCollection*						toolstripItems;
				GuiToolstripBuilder*						previousBuilder;
				theme::ITheme*								theme;
				GuiToolstripButton*							lastCreatedButton;
				
				GuiToolstripBuilder(Environment _environment, GuiToolstripCollection* _toolstripItems);
			public:
				~GuiToolstripBuilder();

				/// <summary>Create a button.</summary>
				/// <returns>The current builder for continuing builder actions.</returns>
				/// <param name="image">The image for the created control.</param>
				/// <param name="text">The text for the created control.</param>
				GuiToolstripBuilder*						Button(Ptr<GuiImageData> image, const WString& text, GuiToolstripButton** result=0);
				/// <summary>Create a button.</summary>
				/// <returns>The current builder for continuing builder actions.</returns>
				/// <param name="command">The command for the created control.</param>
				GuiToolstripBuilder*						Button(Ptr<GuiToolstripCommand> command, GuiToolstripButton** result=0);
				/// <summary>Create a dropdown button.</summary>
				/// <returns>The current builder for continuing builder actions.</returns>
				/// <param name="image">The image for the created control.</param>
				/// <param name="text">The text for the created control.</param>
				GuiToolstripBuilder*						DropdownButton(Ptr<GuiImageData> image, const WString& text, GuiToolstripButton** result=0);
				/// <summary>Create a dropdown button.</summary>
				/// <returns>The current builder for continuing builder actions.</returns>
				/// <param name="command">The command for the created control.</param>
				GuiToolstripBuilder*						DropdownButton(Ptr<GuiToolstripCommand> command, GuiToolstripButton** result=0);
				/// <summary>Create a split button.</summary>
				/// <returns>The current builder for continuing builder actions.</returns>
				/// <param name="image">The image for the created control.</param>
				/// <param name="text">The text for the created control.</param>
				GuiToolstripBuilder*						SplitButton(Ptr<GuiImageData> image, const WString& text, GuiToolstripButton** result=0);
				/// <summary>Create a split button.</summary>
				/// <returns>The current builder for continuing builder actions.</returns>
				/// <param name="command">The command for the created control.</param>
				GuiToolstripBuilder*						SplitButton(Ptr<GuiToolstripCommand> command, GuiToolstripButton** result=0);
				/// <summary>Create a splitter.</summary>
				/// <returns>The current builder for continuing builder actions.</returns>
				GuiToolstripBuilder*						Splitter();
				/// <summary>Install a control.</summary>
				/// <returns>The current builder for continuing builder actions.</returns>
				GuiToolstripBuilder*						Control(GuiControl* control);
				/// <summary>Begin create sub menu.</summary>
				/// <returns>The builder of the last created control's sub menu for continuing builder actions.</returns>
				GuiToolstripBuilder*						BeginSubMenu();
				/// <summary>End create sub menu.</summary>
				/// <returns>The parent builder for continuing builder actions.</returns>
				GuiToolstripBuilder*						EndSubMenu();
			};

/***********************************************************************
Toolstrip Container
***********************************************************************/

			/// <summary>Toolstrip menu.</summary>
			class GuiToolstripMenu : public GuiMenu, protected GuiToolstripCollection::IContentCallback,  Description<GuiToolstripMenu>
			{
			protected:
				compositions::GuiStackComposition*			stackComposition;
				Ptr<GuiToolstripCollection>					toolstripItems;
				Ptr<compositions::GuiSubComponentMeasurer>	subComponentMeasurer;
				Ptr<GuiToolstripBuilder>					builder;

				void										UpdateLayout()override;
			public:
				/// <summary>Create a control with a specified style controller.</summary>
				/// <param name="_styleController">The style controller.</param>
				/// <param name="_owner">The owner menu item of the parent menu.</param>
				GuiToolstripMenu(IStyleController* _styleController, GuiControl* _owner);
				~GuiToolstripMenu();
				
				/// <summary>Get all managed child controls ordered by their positions.</summary>
				/// <returns>All managed child controls.</returns>
				GuiToolstripCollection&						GetToolstripItems();
				/// <summary>Get the builder facade.</summary>
				/// <returns>The builder facade.</returns>
				/// <param name="themeObject">The theme to use. If this argument is null, the current theme will be used.</param>
				GuiToolstripBuilder*						GetBuilder(theme::ITheme* themeObject=0);
			};

			/// <summary>Toolstrip menu bar.</summary>
			class GuiToolstripMenuBar : public GuiMenuBar, public Description<GuiToolstripMenuBar>
			{
			protected:
				compositions::GuiStackComposition*			stackComposition;
				Ptr<GuiToolstripCollection>					toolstripItems;
				Ptr<GuiToolstripBuilder>					builder;

			public:
				/// <summary>Create a control with a specified style controller.</summary>
				/// <param name="_styleController">The style controller.</param>
				GuiToolstripMenuBar(IStyleController* _styleController);
				~GuiToolstripMenuBar();
				
				/// <summary>Get all managed child controls ordered by their positions.</summary>
				/// <returns>All managed child controls.</returns>
				GuiToolstripCollection&						GetToolstripItems();
				/// <summary>Get the builder facade.</summary>
				/// <returns>The builder facade.</returns>
				/// <param name="themeObject">The theme to use. If this argument is null, the current theme will be used.</param>
				GuiToolstripBuilder*						GetBuilder(theme::ITheme* themeObject=0);
			};

			/// <summary>Toolstrip tool bar.</summary>
			class GuiToolstripToolbar : public GuiControl, public Description<GuiToolstripToolbar>
			{
			protected:
				compositions::GuiStackComposition*			stackComposition;
				Ptr<GuiToolstripCollection>					toolstripItems;
				Ptr<GuiToolstripBuilder>					builder;

			public:
				/// <summary>Create a control with a specified style controller.</summary>
				/// <param name="_styleController">The style controller.</param>
				GuiToolstripToolbar(IStyleController* _styleController);
				~GuiToolstripToolbar();
				
				/// <summary>Get all managed child controls ordered by their positions.</summary>
				/// <returns>All managed child controls.</returns>
				GuiToolstripCollection&						GetToolstripItems();
				/// <summary>Get the builder facade.</summary>
				/// <returns>The builder facade.</returns>
				/// <param name="themeObject">The theme to use. If this argument is null, the current theme will be used.</param>
				GuiToolstripBuilder*						GetBuilder(theme::ITheme* themeObject=0);
			};

/***********************************************************************
Toolstrip Component
***********************************************************************/

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

				/// <summary>Get the toolstrip sub menu. If the sub menu is not created, it returns null.</summary>
				/// <returns>The toolstrip sub menu.</returns>
				GuiToolstripMenu*								GetToolstripSubMenu();
				/// <summary>Create the toolstrip sub menu if necessary. The created toolstrip sub menu is owned by this menu button.</summary>
				/// <param name="subMenuStyleController">The style controller for the toolstrip sub menu. If this argument is null, it will call <see cref="IStyleController::CreateSubMenuStyleController"/> for a style controller.</param>
				void											CreateToolstripSubMenu(GuiToolstripMenu::IStyleController* subMenuStyleController=0);
			};
		}
	}
}

#endif