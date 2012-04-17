/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUITEXTLISTCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUITEXTLISTCONTROLS

#include "..\GuiListControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			class GuiVirtualTextList;

			namespace list
			{

/***********************************************************************
TextList Style Provider
***********************************************************************/

				/// <summary>Item style controller for <see cref="GuiVirtualTextList"/> or <see cref="GuiSelectableListControl"/>.</summary>
				class TextItemStyleProvider : public Object, public GuiSelectableListControl::IItemStyleProvider, public Description<TextItemStyleProvider>
				{
				public:
					/// <summary>Style provider for <see cref="TextItemStyleProvider"/>.</summary>
					class ITextItemStyleProvider : public virtual IDescriptable, public Description<ITextItemStyleProvider>
					{
					public:
						/// <summary>Create the background style controller for an text item. The button selection state represents the text item selection state.</summary>
						/// <returns>The created background style controller.</returns>
						virtual GuiSelectableButton::IStyleController*		CreateBackgroundStyleController()=0;
						/// <summary>Create the bullet style controller for an text item. The button selection state represents the text item check state.</summary>
						/// <returns>The created bullet style controller.</returns>
						virtual GuiSelectableButton::IStyleController*		CreateBulletStyleController()=0;
					};

					/// <summary>The required <see cref="GuiListControl::IItemProvider"/> view for <see cref="TextItemStyleProvider"/>.</summary>
					class ITextItemView : public virtual GuiListControl::IItemPrimaryTextView, public Description<ITextItemView>
					{
					public:
						/// <summary>The identifier for this view.</summary>
						static const wchar_t* const				Identifier;

						/// <summary>Get the text of an item.</summary>
						/// <returns>The text of an item.</returns>
						/// <param name="itemIndex">The index of an item.</param>
						virtual WString							GetText(int itemIndex)=0;
						/// <summary>Get the check state of an item.</summary>
						/// <returns>The check state of an item.</returns>
						/// <param name="itemIndex">The index of an item.</param>
						virtual bool							GetChecked(int itemIndex)=0;
						/// <summary>Set the check state of an item without invoving any UI action.</summary>
						/// <param name="itemIndex">The index of an item.</param>
						/// <param name="value">The new check state.</param>
						virtual void							SetCheckedSilently(int itemIndex, bool value)=0;
					};

					/// <summary>The item style controller for <see cref="TextItemStyleProvider"/>.</summary>
					class TextItemStyleController : public ItemStyleControllerBase, public Description<TextItemStyleController>
					{
					protected:
						GuiSelectableButton*					backgroundButton;
						GuiSelectableButton*					bulletButton;
						elements::GuiSolidLabelElement*			textElement;
						TextItemStyleProvider*					textItemStyleProvider;

						void									OnBulletSelectedChanged(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
					public:
						/// <summary>Create a item style controller with a specified item style provider callback.</summary>
						/// <param name="provider">The item style provider callback.</param>
						TextItemStyleController(TextItemStyleProvider* provider);
						~TextItemStyleController();
						
						/// <summary>Get the selection state of this item.</summary>
						/// <returns>The selection state of this item.</returns>
						bool									GetSelected();
						/// <summary>Set the selection state of this item.</summary>
						/// <param name="value">The selection state of this item.</param>
						void									SetSelected(bool value);
						/// <summary>Get the check state of this item.</summary>
						/// <returns>The check state of this item.</returns>
						bool									GetChecked();
						/// <summary>Set the check state of this item.</summary>
						/// <param name="value">The check state of this item.</param>
						void									SetChecked(bool value);
						/// <summary>Get the text of this item.</summary>
						/// <returns>The text of this item.</returns>
						const WString&							GetText();
						/// <summary>Set the text of this item.</summary>
						/// <param name="value">The text of this item.</param>
						void									SetText(const WString& value);
					};

				protected:
					Ptr<ITextItemStyleProvider>					textItemStyleProvider;
					ITextItemView*								textItemView;
					GuiListControl*								listControl;

					void										OnStyleCheckedChanged(TextItemStyleController* style);
				public:
					/// <summary>Create a item style provider with a specified item style provider callback.</summary>
					/// <param name="_textItemStyleProvider">The item style provider callback.</param>
					TextItemStyleProvider(ITextItemStyleProvider* _textItemStyleProvider);
					~TextItemStyleProvider();

					void										AttachListControl(GuiListControl* value)override;
					void										DetachListControl()override;
					int											GetItemStyleId(int itemIndex)override;
					GuiListControl::IItemStyleController*		CreateItemStyle(int styleId)override;
					void										DestroyItemStyle(GuiListControl::IItemStyleController* style)override;
					void										Install(GuiListControl::IItemStyleController* style, int itemIndex)override;
					void										SetStyleSelected(GuiListControl::IItemStyleController* style, bool value)override;
				};

/***********************************************************************
TextList Data Source
***********************************************************************/

				/// <summary>Text item. This is the item data structure for [T:vl.presentation.controls.list.TextItemProvider].</summary>
				class TextItem
				{
					friend class TextItemProvider;
				protected:
					WString										text;
					bool										checked;
				public:
					/// <summary>Create an empty text item.</summary>
					TextItem();
					/// <summary>Copy a text item.</summary>
					/// <param name="item">The source text item.</param>
					TextItem(const TextItem& item);
					/// <summary>Create a text item with specified text and check state.</summary>
					/// <param name="_text">The text.</param>
					/// <param name="_checked">The check state.</param>
					TextItem(const WString& _text, bool _checked=false);
					/// <summary>Create a text item with specified text and check state.</summary>
					/// <param name="_text">The text.</param>
					/// <param name="_checked">The check state.</param>
					TextItem(const wchar_t* _text, bool _checked=false);
					~TextItem();

					bool										operator==(const TextItem& value)const;
					bool										operator!=(const TextItem& value)const;
					
					/// <summary>Get the text of this item.</summary>
					/// <returns>The text of this item.</returns>
					const WString&								GetText()const;
					/// <summary>Get the check state of this item.</summary>
					/// <returns>The check state of this item.</returns>
					bool										GetChecked()const;
				};

				/// <summary>Item provider for <see cref="GuiVirtualTextList"/> or <see cref="GuiSelectableListControl"/>.</summary>
				class TextItemProvider : public ListProvider<TextItem>, protected TextItemStyleProvider::ITextItemView, public Description<TextItemProvider>
				{
				protected:
					bool										ContainsPrimaryText(int itemIndex)override;
					WString										GetPrimaryTextViewText(int itemIndex)override;
					WString										GetText(int itemIndex)override;
					bool										GetChecked(int itemIndex)override;
					void										SetCheckedSilently(int itemIndex, bool value)override;
				public:
					TextItemProvider();
					~TextItemProvider();
					
					/// <summary>Set the text of an item.</summary>
					/// <param name="itemIndex">The index of an item.</param>
					/// <param name="value">The text of an item.</param>
					void										SetText(int itemIndex, const WString& value);
					/// <summary>Set the check state of an item.</summary>
					/// <param name="itemIndex">The index of an item.</param>
					/// <param name="value">The check state of an item.</param>
					void										SetChecked(int itemIndex, bool value);

					IDescriptable*								RequestView(const WString& identifier)override;
					void										ReleaseView(IDescriptable* view)override;
				};
			}

/***********************************************************************
TextList Control
***********************************************************************/

			/// <summary>Text list control in virtual mode.</summary>
			class GuiVirtualTextList : public GuiSelectableListControl, public Description<GuiVirtualTextList>
			{
			public:
				/// <summary>Create a Text list control in virtual mode.</summary>
				/// <param name="_styleProvider">The style provider for this control.</param>
				/// <param name="_itemStyleProvider">The item style provider callback for this control.</param>
				/// <param name="IStyleProvider">The item provider for this control.</param>
				GuiVirtualTextList(IStyleProvider* _styleProvider, list::TextItemStyleProvider::ITextItemStyleProvider* _itemStyleProvider, GuiListControl::IItemProvider* _itemProvider);
				~GuiVirtualTextList();
				
				Ptr<GuiListControl::IItemStyleProvider>			SetStyleProvider(Ptr<GuiListControl::IItemStyleProvider> value)override;
				/// <summary>Set the item style provider callback.</summary>
				/// <returns>The old item style provider.</returns>
				/// <param name="itemStyleProvider">The new item style provider callback.</param>
				Ptr<GuiListControl::IItemStyleProvider>			ChangeItemStyle(list::TextItemStyleProvider::ITextItemStyleProvider* itemStyleProvider);
			};
			
			/// <summary>Text list control.</summary>
			class GuiTextList : public GuiVirtualTextList, public Description<GuiTextList>
			{
			protected:
				list::TextItemProvider*							items;
			public:
				/// <summary>Create a Text list control.</summary>
				/// <param name="_styleProvider">The style provider for this control.</param>
				/// <param name="_itemStyleProvider">The item style provider callback for this control.</param>
				GuiTextList(IStyleProvider* _styleProvider, list::TextItemStyleProvider::ITextItemStyleProvider* _itemStyleProvider);
				~GuiTextList();

				/// <summary>Get all text items.</summary>
				/// <returns>All text items.</returns>
				list::TextItemProvider&							GetItems();
			};
		}
	}
}

#endif