/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUITEXTLISTCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUITEXTLISTCONTROLS

#include "GuiListControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			class GuiVirtualTextList;
			class GuiTextList;

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
						/// <summary>Get the text color.</summary>
						/// <returns>The text color.</returns>
						virtual Color										GetTextColor()=0;
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
						virtual WString							GetText(vint itemIndex)=0;
						/// <summary>Get the check state of an item.</summary>
						/// <returns>The check state of an item.</returns>
						/// <param name="itemIndex">The index of an item.</param>
						virtual bool							GetChecked(vint itemIndex)=0;
						/// <summary>Set the check state of an item without invoving any UI action.</summary>
						/// <param name="itemIndex">The index of an item.</param>
						/// <param name="value">The new check state.</param>
						virtual void							SetCheckedSilently(vint itemIndex, bool value)=0;
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
					GuiVirtualTextList*							listControl;

					void										OnStyleCheckedChanged(TextItemStyleController* style);
				public:
					/// <summary>Create a item style provider with a specified item style provider callback.</summary>
					/// <param name="_textItemStyleProvider">The item style provider callback.</param>
					TextItemStyleProvider(ITextItemStyleProvider* _textItemStyleProvider);
					~TextItemStyleProvider();

					void										AttachListControl(GuiListControl* value)override;
					void										DetachListControl()override;
					vint										GetItemStyleId(vint itemIndex)override;
					GuiListControl::IItemStyleController*		CreateItemStyle(vint styleId)override;
					void										DestroyItemStyle(GuiListControl::IItemStyleController* style)override;
					void										Install(GuiListControl::IItemStyleController* style, vint itemIndex)override;
					void										SetStyleIndex(GuiListControl::IItemStyleController* style, vint value)override;
					void										SetStyleSelected(GuiListControl::IItemStyleController* style, bool value)override;
				};

/***********************************************************************
TextList Data Source
***********************************************************************/

				class TextItemProvider;

				/// <summary>Text item. This is the item data structure for [T:vl.presentation.controls.list.TextItemProvider].</summary>
				class TextItem : public Object, public Description<TextItem>
				{
					friend class TextItemProvider;
				protected:
					TextItemProvider*							owner;
					WString										text;
					bool										checked;

				public:
					/// <summary>Create an empty text item.</summary>
					TextItem();
					/// <summary>Create a text item with specified text and check state.</summary>
					/// <param name="_text">The text.</param>
					/// <param name="_checked">The check state.</param>
					TextItem(const WString& _text, bool _checked=false);
					~TextItem();

					bool										operator==(const TextItem& value)const;
					bool										operator!=(const TextItem& value)const;
					
					/// <summary>Get the text of this item.</summary>
					/// <returns>The text of this item.</returns>
					const WString&								GetText();
					/// <summary>Set the text of this item.</summary>
					/// <param name="value">The text of this item.</param>
					void										SetText(const WString& value);

					/// <summary>Get the check state of this item.</summary>
					/// <returns>The check state of this item.</returns>
					bool										GetChecked();
					/// <summary>Set the check state of this item.</summary>
					/// <param name="value">The check state of this item.</param>
					void										SetChecked(bool value);
				};

				/// <summary>Item provider for <see cref="GuiVirtualTextList"/> or <see cref="GuiSelectableListControl"/>.</summary>
				class TextItemProvider
					: public ListProvider<Ptr<TextItem>>
					, protected TextItemStyleProvider::ITextItemView
					, protected GuiListControl::IItemBindingView
					, public Description<TextItemProvider>
				{
					friend class TextItem;
					friend class vl::presentation::controls::GuiTextList;
				protected:
					GuiTextList*								listControl;

					void										AfterInsert(vint item, const Ptr<TextItem>& value)override;
					void										BeforeRemove(vint item, const Ptr<TextItem>& value)override;

					bool										ContainsPrimaryText(vint itemIndex)override;
					WString										GetPrimaryTextViewText(vint itemIndex)override;
					WString										GetText(vint itemIndex)override;
					bool										GetChecked(vint itemIndex)override;
					void										SetCheckedSilently(vint itemIndex, bool value)override;
					description::Value							GetBindingValue(vint itemIndex)override;
				public:
					TextItemProvider();
					~TextItemProvider();

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
				/// <param name="_itemProvider">The item provider for this control.</param>
				GuiVirtualTextList(IStyleProvider* _styleProvider, list::TextItemStyleProvider::ITextItemStyleProvider* _itemStyleProvider, GuiListControl::IItemProvider* _itemProvider);
				~GuiVirtualTextList();

				/// <summary>Item checked changed event.</summary>
				compositions::GuiItemNotifyEvent				ItemChecked;
				
				/// <summary>Set the item style provider.</summary>
				/// <returns>The old item style provider.</returns>
				/// <param name="itemStyleProvider">The new item style provider.</param>
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

				/// <summary>Get the selected item.</summary>
				/// <returns>Returns the selected item. If there are multiple selected items, or there is no selected item, null will be returned.</returns>
				Ptr<list::TextItem>								GetSelectedItem();
			};
		}
	}
}

#endif