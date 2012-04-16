/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUICONTAINERCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUICONTAINERCONTROLS

#include "..\GuiBasicControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
/***********************************************************************
Tab Control
***********************************************************************/

			class GuiTab;

			/// <summary>Represents a page of a <see cref="GuiTab"/>. A tab page is not a control.</summary>
			class GuiTabPage : public Object, public Description<GuiTabPage>
			{
				friend class GuiTab;
				friend class Ptr<GuiTabPage>;
			protected:
				GuiControl*										container;
				GuiTab*											owner;
				WString											text;
				
				GuiTabPage();
				~GuiTabPage();

				bool											AssociateTab(GuiTab* _owner, GuiControl::IStyleController* _styleController);
			public:
				/// <summary>Text changed event.</summary>
				compositions::GuiNotifyEvent					TextChanged;

				/// <summary>Get the container control to store all sub controls.</summary>
				/// <returns>The container control to store all sub controls.</returns>
				GuiControl*										GetContainer();
				/// <summary>Get the owner <see cref="GuiTab"/>.</summary>
				/// <returns>The owner <see cref="GuiTab"/>.</returns>
				GuiTab*											GetOwnerTab();
				/// <summary>Get the text rendered as the name for this page.</summary>
				/// <returns>The text rendered as the name for this page.</returns>
				const WString&									GetText();
				/// <summary>Set the text rendered as the name for this page.</summary>
				/// <param name="value">The text rendered as the name for this page.</param>
				void											SetText(const WString& param);
				/// <summary>Test is this page selected.</summary>
				/// <returns>Returns true if this page is selected.</returns>
				bool											GetSelected();
			};

			/// <summary>Represents a container with multiple named tabs.</summary>
			class GuiTab : public GuiControl, public Description<GuiTab>
			{
				friend class GuiTabPage;
			public:
				/// <summary>A command executor for the style controller to change the control state.</summary>
				class ICommandExecutor : public virtual IDescriptable, public Description<ICommandExecutor>
				{
				public:
					/// <summary>Select a tab page.</summary>
					/// <param name="index">The specified position for the tab page.</param>
					virtual void								ShowTab(int index)=0;
				};
				
				/// <summary>Style controller interface for <see cref="GuiTab"/>.</summary>
				class IStyleController : public virtual GuiControl::IStyleController, public Description<IStyleController>
				{
				public:
					/// <summary>Called when the command executor is changed.</summary>
					/// <param name="value">The command executor.</param>
					virtual void								SetCommandExecutor(ICommandExecutor* value)=0;
					/// <summary>Insert a tag header at the specified position.</summary>
					/// <param name="index">The specified position.</param>
					virtual void								InsertTab(int index)=0;
					/// <summary>Set the text of a tag header at the specified position.</summary>
					/// <param name="index">The specified position.</param>
					/// <param name="value">The text.</param>
					virtual void								SetTabText(int index, const WString& value)=0;
					/// <summary>Remove the tag header at the specified position.</summary>
					/// <param name="index">The specified position.</param>
					virtual void								RemoveTab(int index)=0;
					/// <summary>Move a tag header from a position to another.</summary>
					/// <param name="oldIndex">The old position.</param>
					/// <param name="newIndex">The new position.</param>
					virtual void								MoveTab(int oldIndex, int newIndex)=0;
					/// <summary>Render a tag header at the specified position as selected.</summary>
					/// <param name="index">The specified position.</param>
					virtual void								SetSelectedTab(int index)=0;
					/// <summary>Create a style controller for tab page container control.</summary>
					/// <returns>The created style controller.</returns>
					virtual GuiControl::IStyleController*		CreateTabPageStyleController()=0;
				};
			protected:
				class CommandExecutor : public Object, public ICommandExecutor
				{
				protected:
					GuiTab*										tab;
				public:
					CommandExecutor(GuiTab* _tab);
					~CommandExecutor();

					void										ShowTab(int index)override;
				};

				Ptr<CommandExecutor>							commandExecutor;
				IStyleController*								styleController;
				collections::List<GuiTabPage*>					tabPages;
				GuiTabPage*										selectedPage;
			public:
				/// <summary>Create a control with a specified style controller.</summary>
				/// <param name="_styleController">The style controller.</param>
				GuiTab(IStyleController* _styleController);
				~GuiTab();

				/// <summary>Selected page changed event.</summary>
				compositions::GuiNotifyEvent					SelectedPageChanged;

				/// <summary>Create a tag page at the specified index.</summary>
				/// <returns>The created page.</returns>
				/// <param name="index">The specified index. Set to -1 to insert at the last position.</param>
				GuiTabPage*										CreatePage(int index=-1);
				/// <summary>Insert a tag page at the specified index.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="page">The tab page to insert. This page should be a new page that has never been inserted to a <see cref="GuiTab"/>.</param>
				/// <param name="index">The specified index. Set to -1 to insert at the last position.</param>
				bool											CreatePage(GuiTabPage* page, int index=-1);
				/// <summary>Remove the tag page at the specified index.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="page">The tab page to remove.</param>
				bool											RemovePage(GuiTabPage* value);
				/// <summary>Move a tag page at the specified index to a new position.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="page">The tab page to move.</param>
				/// <param name="index">The new position.</param>
				bool											MovePage(GuiTabPage* page, int newIndex);
				/// <summary>Get all pages.</summary>
				/// <returns>All pages.</returns>
				const collections::IReadonlyList<GuiTabPage*>&	GetPages();

				/// <summary>Get the selected page.</summary>
				/// <returns>The selected page.</returns>
				GuiTabPage*										GetSelectedPage();
				/// <summary>Set the selected page.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="value">The selected page.</param>
				bool											SetSelectedPage(GuiTabPage* value);
			};
		}
	}
}

#endif