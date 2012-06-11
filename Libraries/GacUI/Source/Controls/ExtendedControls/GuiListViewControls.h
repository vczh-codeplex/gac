/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUILISTVIEWCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUILISTVIEWCONTROLS

#include "..\GuiListControls.h"
#include "..\ExtendedControls\GuiMenuControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			class GuiListViewBase;

			namespace list
			{

/***********************************************************************
ListView Base
***********************************************************************/

				/// <summary>Item style provider base for <see cref="GuiListViewBase"/>.</summary>
				class ListViewItemStyleProviderBase: public Object, public GuiSelectableListControl::IItemStyleProvider, public Description<ListViewItemStyleProviderBase>
				{
				public:
					/// <summary>Item style controller base for <see cref="GuiListViewBase"/>.</summary>
					class ListViewItemStyleController : public ItemStyleControllerBase, public Description<ListViewItemStyleController>
					{
					protected:
						GuiSelectableButton*					backgroundButton;
						ListViewItemStyleProviderBase*			listViewItemStyleProvider;

					public:
						ListViewItemStyleController(ListViewItemStyleProviderBase* provider);
						~ListViewItemStyleController();

						/// <summary>Get the selection state.</summary>
						/// <returns>Returns true if this item is selected.</returns>
						bool									GetSelected();
						/// <summary>Set the selection state.</summary>
						/// <param name="value">Set to true to render this item as selected.</param>
						void									SetSelected(bool value);
					};

				protected:
					GuiListViewBase*							listControl;

				public:
					/// <summary>Create the item style provider.</summary>
					ListViewItemStyleProviderBase();
					~ListViewItemStyleProviderBase();

					void										AttachListControl(GuiListControl* value)override;
					void										DetachListControl()override;
					int											GetItemStyleId(int itemIndex)override;
					void										SetStyleSelected(GuiListControl::IItemStyleController* style, bool value)override;
				};
			}

			///<summary>List view column header control for detailed view.</summary>
			class GuiListViewColumnHeader : public GuiMenuButton, public Description<GuiListViewColumnHeader>
			{
			public:
				/// <summary>Represents the sorting state of list view items related to this column.</summary>
				enum ColumnSortingState
				{
					/// <summary>[T:vl.presentation.controls.GuiListViewColumnHeader.ColumnSortingState]Nor sorted.</summary>
					NotSorted,
					/// <summary>[T:vl.presentation.controls.GuiListViewColumnHeader.ColumnSortingState]Ascending.</summary>
					Ascending,
					/// <summary>[T:vl.presentation.controls.GuiListViewColumnHeader.ColumnSortingState]Descending.</summary>
					Descending,
				};
				
				/// <summary>Style provider for <see cref="GuiListViewColumnHeader"/>.</summary>
				class IStyleController : public virtual GuiMenuButton::IStyleController, public Description<IStyleController>
				{
				public:
					/// <summary>Notify that the column sorting state is changed.</summary>
					/// <param name="value">The new column sorting state.</param>
					virtual void								SetColumnSortingState(ColumnSortingState value)=0;
				};

			protected:
				IStyleController*								styleController;
				ColumnSortingState								columnSortingState;

			public:
				/// <summary>Create a control with a specified style controller.</summary>
				/// <param name="_styleController">The style controller.</param>
				GuiListViewColumnHeader(IStyleController* _styleController);
				~GuiListViewColumnHeader();

				/// <summary>Get the column sorting state.</summary>
				/// <returns>The column sorting state.</returns>
				ColumnSortingState								GetColumnSortingState();
				/// <summary>Set the column sorting state.</summary>
				/// <param name="value">The new column sorting state.</param>
				void											SetColumnSortingState(ColumnSortingState value);
			};

			/// <summary>List view base control. All list view controls inherit from this class. <see cref="list::ListViewItemStyleProviderBase"/> is suggested to be the base class of item style providers for list view control.</summary>
			class GuiListViewBase : public GuiSelectableListControl, public Description<GuiListViewBase>
			{
			public:
				/// <summary>Style provider for <see cref="GuiListViewBase"/>.</summary>
				class IStyleProvider : public virtual GuiSelectableListControl::IStyleProvider, public Description<IStyleProvider>
				{
				public:
					/// <summary>Create a style controller for an item background.</summary>
					/// <returns>The created style controller for an item background.</returns>
					virtual GuiSelectableButton::IStyleController*		CreateItemBackground()=0;
					/// <summary>Create a style controller for a column header.</summary>
					/// <returns>The created style controller for a column header.</returns>
					virtual GuiListViewColumnHeader::IStyleController*	CreateColumnStyle()=0;
					/// <summary>Get the primary text color.</summary>
					/// <returns>The primary text color.</returns>
					virtual Color										GetPrimaryTextColor()=0;
					/// <summary>Get the secondary text color.</summary>
					/// <returns>The secondary text color.</returns>
					virtual Color										GetSecondaryTextColor()=0;
					/// <summary>Get the item peparator text color.</summary>
					/// <returns>The item peparator text color.</returns>
					virtual Color										GetItemSeparatorColor()=0;
				};

			protected:
				IStyleProvider*									styleProvider;

			public:
				/// <summary>Create a list view base control.</summary>
				/// <param name="_styleProvider">The style provider for this control.</param>
				/// <param name="_itemProvider">The item provider for this control.</param>
				GuiListViewBase(IStyleProvider* _styleProvider, GuiListControl::IItemProvider* _itemProvider);
				~GuiListViewBase();
				
				/// <summary>Get the associated style provider.</summary>
				/// <returns>The style provider.</returns>
				IStyleProvider*									GetListViewStyleProvider();
				Ptr<GuiListControl::IItemStyleProvider>			SetStyleProvider(Ptr<GuiListControl::IItemStyleProvider> value)override;
			};

/***********************************************************************
ListView ItemStyleProvider
***********************************************************************/

			namespace list
			{
				/// <summary>Base class for all predefined list view item style.</summary>
				class ListViewItemStyleProvider : public ListViewItemStyleProviderBase, public Description<ListViewItemStyleProvider>
				{
				public:
					/// <summary>The required <see cref="GuiListControl::IItemProvider"/> view for <see cref="ListViewItemStyleProvider"/>.</summary>
					class IListViewItemView : public virtual GuiListControl::IItemPrimaryTextView, public Description<IListViewItemView>
					{
					public:
						/// <summary>The identifier for this view.</summary>
						static const wchar_t* const				Identifier;

						/// <summary>Get the small image of an item.</summary>
						/// <returns>The small image.</returns>
						/// <param name="itemIndex">The index of the item.</param>
						virtual Ptr<GuiImageData>				GetSmallImage(int itemIndex)=0;
						/// <summary>Get the large image of an item.</summary>
						/// <returns>The large image.</returns>
						/// <param name="itemIndex">The index of the item.</param>
						virtual Ptr<GuiImageData>				GetLargeImage(int itemIndex)=0;
						/// <summary>Get the text of an item.</summary>
						/// <returns>The text.</returns>
						/// <param name="itemIndex">The index of the item.</param>
						virtual WString							GetText(int itemIndex)=0;
						/// <summary>Get the sub item text of an item. If the sub item index out of range, it returns an empty string.</summary>
						/// <returns>The sub item text.</returns>
						/// <param name="itemIndex">The index of the item.</param>
						/// <param name="iindextemIndex">The sub item index of the item.</param>
						virtual WString							GetSubItem(int itemIndex, int index)=0;

						/// <summary>Get the number of data columns.</summary>
						/// <returns>The number of data columns.</returns>
						virtual int								GetDataColumnCount()=0;
						/// <summary>Get the column index of the index-th data column.</summary>
						/// <returns>The column index.</returns>
						/// <param name="index">The order of the data column.</param>
						virtual int								GetDataColumn(int index)=0;

						/// <summary>Get the number of columns.</summary>
						/// <returns>The number of columns.</returns>
						virtual int								GetColumnCount()=0;
						/// <summary>Get the text of a column.</summary>
						/// <returns>The text.</returns>
						/// <param name="itemIndex">The index of the column.</param>
						virtual WString							GetColumnText(int index)=0;
					};

					/// <summary>Represents the extra item content information of a list view item.</summary>
					class IListViewItemContent : public virtual IDescriptable, public Description<IListViewItemContent>
					{
					public:
						/// <summary>Get the composition representing the whole item.</summary>
						/// <returns>The composition representing the whole item.</returns>
						virtual compositions::GuiBoundsComposition*				GetContentComposition()=0;
						/// <summary>Get the composition for the extra background decorator. If there is no decorator, it returns null.</summary>
						/// <returns>The composition for the extra background decorator.</returns>
						virtual compositions::GuiBoundsComposition*				GetBackgroundDecorator()=0;
						/// <summary>Install data of an item to the item content for rendering.</summary>
						/// <param name="styleProvider">Style provider for the list view control.</param>
						/// <param name="view">The <see cref="IListViewItemView"/> for the list view control.</param>
						/// <param name="itemIndex">The index of the item to install.</param>
						virtual void											Install(GuiListViewBase::IStyleProvider* styleProvider, IListViewItemView* view, int itemIndex)=0;
					};

					/// <summary>List view item content provider.</summary>
					class IListViewItemContentProvider : public virtual IDescriptable, public Description<IListViewItemContentProvider>
					{
					public:
						/// <summary>Create a default and preferred <see cref="GuiListControl::IItemCoordinateTransformer"/> for the related item style provider.</summary>
						/// <returns>The created item coordinate transformer.</returns>
						virtual GuiListControl::IItemCoordinateTransformer*		CreatePreferredCoordinateTransformer()=0;
						/// <summary>Create a default and preferred <see cref="GuiListControl::IItemArranger"/> for the related item style provider.</summary>
						/// <returns>The created item coordinate arranger.</returns>
						virtual GuiListControl::IItemArranger*					CreatePreferredArranger()=0;
						/// <summary>Create a <see cref="IListViewItemContent"/>.</summary>
						/// <returns>The created list view item content.</returns>
						virtual IListViewItemContent*							CreateItemContent(const FontProperties& font)=0;
						/// <summary>Called when the owner item style provider in installed to a <see cref="GuiListControl"/>.</summary>
						/// <param name="value">The list control.</param>
						virtual void											AttachListControl(GuiListControl* value)=0;
						/// <summary>Called when the owner item style provider in uninstalled from a <see cref="GuiListControl"/>.</summary>
						virtual void											DetachListControl()=0;
					};

					/// <summary>A list view item style controller with extra item content information.</summary>
					class ListViewContentItemStyleController : public ListViewItemStyleController, public Description<ListViewContentItemStyleController>
					{
					protected:
						ListViewItemStyleProvider*				listViewItemStyleProvider;
						Ptr<IListViewItemContent>				content;
					public:
						/// <summary>Create the item style controller.</summary>
						/// <param name="provider">The owner item style provider.</param>
						ListViewContentItemStyleController(ListViewItemStyleProvider* provider);
						~ListViewContentItemStyleController();

						/// <summary>Get the extra item content information.</summary>
						/// <returns>The extra item content information.</returns>
						IListViewItemContent*					GetItemContent();
						/// <summary>Install data of an item to the item style controller for rendering.</summary>
						/// <param name="view">The <see cref="IListViewItemView"/> for the list view control.</param>
						/// <param name="itemIndex">The index of the item to install.</param>
						void									Install(IListViewItemView* view, int itemIndex);
					};

				protected:

					typedef collections::List<GuiListControl::IItemStyleController*>				ItemStyleList;
					typedef collections::IReadonlyList<GuiListControl::IItemStyleController*>		IItemStyleList;

					IListViewItemView*							listViewItemView;
					Ptr<IListViewItemContentProvider>			listViewItemContentProvider;
					ItemStyleList								itemStyles;
				public:
					/// <summary>Create a list view item style provider using an item content provider.</summary>
					/// <param name="itemContentProvider">The item content provider.</param>
					ListViewItemStyleProvider(IListViewItemContentProvider* itemContentProvider);
					~ListViewItemStyleProvider();

					void										AttachListControl(GuiListControl* value)override;
					void										DetachListControl()override;
					GuiListControl::IItemStyleController*		CreateItemStyle(int styleId)override;
					void										DestroyItemStyle(GuiListControl::IItemStyleController* style)override;
					void										Install(GuiListControl::IItemStyleController* style, int itemIndex)override;

					/// <summary>Get all created item styles.</summary>
					/// <returns>All created item styles.</returns>
					const IItemStyleList&						GetCreatedItemStyles();
					/// <summary>Test is an item style controller placed in the list view control. If not, maybe the style controller is cached for reusing.</summary>
					/// <returns>Returns true if an item style controller is placed in the list view control.</returns>
					bool										IsItemStyleAttachedToListView(GuiListControl::IItemStyleController* itemStyle);
				};
			}

/***********************************************************************
ListView ItemContentProvider
***********************************************************************/

			namespace list
			{
				/// <summary>Big icon content provider.</summary>
				class ListViewBigIconContentProvider : public Object, public virtual ListViewItemStyleProvider::IListViewItemContentProvider, public Description<ListViewBigIconContentProvider>
				{
				protected:
					class ItemContent : public Object, public virtual ListViewItemStyleProvider::IListViewItemContent
					{
					protected:
						compositions::GuiBoundsComposition*				contentComposition;
						elements::GuiImageFrameElement*					image;
						elements::GuiSolidLabelElement*					text;

					public:
						ItemContent(Size iconSize, const FontProperties& font);
						~ItemContent();

						compositions::GuiBoundsComposition*				GetContentComposition()override;
						compositions::GuiBoundsComposition*				GetBackgroundDecorator()override;
						void											Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)override;
					};

					Size												iconSize;
				public:
					/// <summary>Create the content provider.</summary>
					/// <param name="_iconSize">The icon size.</param>
					ListViewBigIconContentProvider(Size _iconSize=Size(32, 32));
					~ListViewBigIconContentProvider();

					GuiListControl::IItemCoordinateTransformer*			CreatePreferredCoordinateTransformer()override;
					GuiListControl::IItemArranger*						CreatePreferredArranger()override;
					ListViewItemStyleProvider::IListViewItemContent*	CreateItemContent(const FontProperties& font)override;
					void												AttachListControl(GuiListControl* value)override;
					void												DetachListControl()override;
				};
				
				/// <summary>Small icon content provider.</summary>
				class ListViewSmallIconContentProvider : public Object, public virtual ListViewItemStyleProvider::IListViewItemContentProvider, public Description<ListViewSmallIconContentProvider>
				{
				protected:
					class ItemContent : public Object, public virtual ListViewItemStyleProvider::IListViewItemContent
					{
					protected:
						compositions::GuiBoundsComposition*				contentComposition;
						elements::GuiImageFrameElement*					image;
						elements::GuiSolidLabelElement*					text;

					public:
						ItemContent(Size iconSize, const FontProperties& font);
						~ItemContent();

						compositions::GuiBoundsComposition*				GetContentComposition()override;
						compositions::GuiBoundsComposition*				GetBackgroundDecorator()override;
						void											Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)override;
					};

					Size												iconSize;
				public:
					/// <summary>Create the content provider.</summary>
					/// <param name="_iconSize">The icon size.</param>
					ListViewSmallIconContentProvider(Size _iconSize=Size(16, 16));
					~ListViewSmallIconContentProvider();
					
					GuiListControl::IItemCoordinateTransformer*			CreatePreferredCoordinateTransformer()override;
					GuiListControl::IItemArranger*						CreatePreferredArranger()override;
					ListViewItemStyleProvider::IListViewItemContent*	CreateItemContent(const FontProperties& font)override;
					void												AttachListControl(GuiListControl* value)override;
					void												DetachListControl()override;
				};
				
				/// <summary>List content provider.</summary>
				class ListViewListContentProvider : public Object, public virtual ListViewItemStyleProvider::IListViewItemContentProvider, public Description<ListViewListContentProvider>
				{
				protected:
					class ItemContent : public Object, public virtual ListViewItemStyleProvider::IListViewItemContent
					{
					protected:
						compositions::GuiBoundsComposition*				contentComposition;
						elements::GuiImageFrameElement*					image;
						elements::GuiSolidLabelElement*					text;

					public:
						ItemContent(Size iconSize, const FontProperties& font);
						~ItemContent();

						compositions::GuiBoundsComposition*				GetContentComposition()override;
						compositions::GuiBoundsComposition*				GetBackgroundDecorator()override;
						void											Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)override;
					};

					Size												iconSize;
				public:
					/// <summary>Create the content provider.</summary>
					/// <param name="_iconSize">The icon size.</param>
					ListViewListContentProvider(Size _iconSize=Size(16, 16));
					~ListViewListContentProvider();
					
					GuiListControl::IItemCoordinateTransformer*			CreatePreferredCoordinateTransformer()override;
					GuiListControl::IItemArranger*						CreatePreferredArranger()override;
					ListViewItemStyleProvider::IListViewItemContent*	CreateItemContent(const FontProperties& font)override;
					void												AttachListControl(GuiListControl* value)override;
					void												DetachListControl()override;
				};
				
				/// <summary>Tile content provider.</summary>
				class ListViewTileContentProvider : public Object, public virtual ListViewItemStyleProvider::IListViewItemContentProvider, public Description<ListViewTileContentProvider>
				{
				protected:
					class ItemContent : public Object, public virtual ListViewItemStyleProvider::IListViewItemContent
					{
						typedef collections::Array<elements::GuiSolidLabelElement*>		DataTextElementArray;
					protected:
						compositions::GuiBoundsComposition*				contentComposition;
						elements::GuiImageFrameElement*					image;
						elements::GuiSolidLabelElement*					text;
						compositions::GuiTableComposition*				textTable;
						DataTextElementArray							dataTexts;

						void											RemoveTextElement(int textRow);
						elements::GuiSolidLabelElement*					CreateTextElement(int textRow, const FontProperties& font);
						void											ResetTextTable(int textRows);
					public:
						ItemContent(Size iconSize, const FontProperties& font);
						~ItemContent();

						compositions::GuiBoundsComposition*				GetContentComposition()override;
						compositions::GuiBoundsComposition*				GetBackgroundDecorator()override;
						void											Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)override;
					};

					Size												iconSize;
				public:
					/// <summary>Create the content provider.</summary>
					/// <param name="_iconSize">The icon size.</param>
					ListViewTileContentProvider(Size _iconSize=Size(32, 32));
					~ListViewTileContentProvider();
					
					GuiListControl::IItemCoordinateTransformer*			CreatePreferredCoordinateTransformer()override;
					GuiListControl::IItemArranger*						CreatePreferredArranger()override;
					ListViewItemStyleProvider::IListViewItemContent*	CreateItemContent(const FontProperties& font)override;
					void												AttachListControl(GuiListControl* value)override;
					void												DetachListControl()override;
				};
				
				/// <summary>View information content provider.</summary>
				class ListViewInformationContentProvider : public Object, public virtual ListViewItemStyleProvider::IListViewItemContentProvider, public Description<ListViewInformationContentProvider>
				{
				protected:
					class ItemContent : public Object, public virtual ListViewItemStyleProvider::IListViewItemContent
					{
						typedef collections::Array<elements::GuiSolidLabelElement*>		DataTextElementArray;
					protected:
						FontProperties									baselineFont;
						compositions::GuiBoundsComposition*				contentComposition;
						elements::GuiImageFrameElement*					image;
						elements::GuiSolidLabelElement*					text;
						compositions::GuiTableComposition*				textTable;
						DataTextElementArray							dataTexts;

						elements::GuiSolidBackgroundElement*			bottomLine;
						compositions::GuiBoundsComposition*				bottomLineComposition;

					public:
						ItemContent(Size iconSize, const FontProperties& font);
						~ItemContent();

						compositions::GuiBoundsComposition*				GetContentComposition()override;
						compositions::GuiBoundsComposition*				GetBackgroundDecorator()override;
						void											Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)override;
					};

					Size												iconSize;
				public:
					/// <summary>Create the content provider.</summary>
					/// <param name="_iconSize">The icon size.</param>
					ListViewInformationContentProvider(Size _iconSize=Size(32, 32));
					~ListViewInformationContentProvider();
					
					GuiListControl::IItemCoordinateTransformer*			CreatePreferredCoordinateTransformer()override;
					GuiListControl::IItemArranger*						CreatePreferredArranger()override;
					ListViewItemStyleProvider::IListViewItemContent*	CreateItemContent(const FontProperties& font)override;
					void												AttachListControl(GuiListControl* value)override;
					void												DetachListControl()override;
				};

/***********************************************************************
ListView ItemContentProvider(Detailed)
***********************************************************************/

				/// <summary>List view column item arranger. This arranger contains column headers. When an column header is resized, all items will be notified via the [T:vl.presentation.controls.list.ListViewColumnItemArranger.IColumnItemView] for <see cref="GuiListControl::IItemProvider"/>.</summary>
				class ListViewColumnItemArranger : public FixedHeightItemArranger, public Description<ListViewColumnItemArranger>
				{
					typedef collections::List<GuiMenuButton*>							ColumnHeaderButtonList;
					typedef collections::List<compositions::GuiBoundsComposition*>		ColumnHeaderSplitterList;
				public:
					static const int							SplitterWidth=8;
					
					/// <summary>Callback for [T:vl.presentation.controls.list.ListViewColumnItemArranger.IColumnItemView]. Column item view use this interface to notify column related modification.</summary>
					class IColumnItemViewCallback : public virtual IDescriptable, public Description<IColumnItemViewCallback>
					{
					public:
						/// <summary>Called when any column is changed (inserted, removed, text changed, etc.).</summary>
						virtual void							OnColumnChanged()=0;
					};
					
					/// <summary>The required <see cref="GuiListControl::IItemProvider"/> view for <see cref="ListViewColumnItemArranger"/>.</summary>
					class IColumnItemView : public virtual IDescriptable, public Description<IColumnItemView>
					{
					public:
						/// <summary>The identifier for this view.</summary>
						static const wchar_t* const								Identifier;
						
						/// <summary>Attach an column item view callback to this view.</summary>
						/// <returns>Returns true if this operation succeeded.</returns>
						/// <param name="value">The column item view callback.</param>
						virtual bool											AttachCallback(IColumnItemViewCallback* value)=0;
						/// <summary>Detach an column item view callback from this view.</summary>
						/// <returns>Returns true if this operation succeeded.</returns>
						/// <param name="value">The column item view callback.</param>
						virtual bool											DetachCallback(IColumnItemViewCallback* value)=0;
						/// <summary>Get the number of all columns.</summary>
						/// <returns>The number of all columns.</returns>
						virtual int												GetColumnCount()=0;
						/// <summary>Get the text of the column.</summary>
						/// <returns>The text of the column.</returns>
						/// <param name="index">The index of the column.</param>
						virtual WString											GetColumnText(int index)=0;
						/// <summary>Get the size of the column.</summary>
						/// <returns>The size of the column.</returns>
						/// <param name="index">The index of the column.</param>
						virtual int												GetColumnSize(int index)=0;
						/// <summary>Set the size of the column.</summary>
						/// <param name="index">The index of the column.</param>
						/// <param name="value">The new size of the column.</param>
						virtual void											SetColumnSize(int index, int value)=0;
						/// <summary>Get the popup binded to the column.</summary>
						/// <returns>The popup binded to the column.</returns>
						/// <param name="index">The index of the column.</param>
						virtual GuiMenu*										GetDropdownPopup(int index)=0;
						/// <summary>Get the sorting state of the column.</summary>
						/// <returns>The sorting state of the column.</returns>
						/// <param name="index">The index of the column.</param>
						virtual GuiListViewColumnHeader::ColumnSortingState		GetSortingState(int index)=0;
					};
				protected:
					class ColumnItemViewCallback : public Object, public virtual IColumnItemViewCallback
					{
					protected:
						ListViewColumnItemArranger*				arranger;
					public:
						ColumnItemViewCallback(ListViewColumnItemArranger* _arranger);
						~ColumnItemViewCallback();

						void									OnColumnChanged();
					};

					GuiListViewBase*							listView;
					GuiListViewBase::IStyleProvider*			styleProvider;
					IColumnItemView*							columnItemView;
					Ptr<ColumnItemViewCallback>					columnItemViewCallback;
					compositions::GuiStackComposition*			columnHeaders;
					ColumnHeaderButtonList						columnHeaderButtons;
					ColumnHeaderSplitterList					columnHeaderSplitters;
					bool										splitterDragging;
					int											splitterLatestX;

					void										ColumnHeaderSplitterLeftButtonDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
					void										ColumnHeaderSplitterLeftButtonUp(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
					void										ColumnHeaderSplitterMouseMove(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);

					void										RearrangeItemBounds()override;
					int											GetWidth()override;
					int											GetYOffset()override;
					Size										OnCalculateTotalSize()override;
					void										DeleteColumnButtons();
					void										RebuildColumns();
				public:
					ListViewColumnItemArranger();
					~ListViewColumnItemArranger();

					void										AttachListControl(GuiListControl* value)override;
					void										DetachListControl()override;
				};
				
				/// <summary>Detail content provider.</summary>
				class ListViewDetailContentProvider
					: public Object
					, public virtual ListViewItemStyleProvider::IListViewItemContentProvider
					, protected virtual ListViewColumnItemArranger::IColumnItemViewCallback
					, public Description<ListViewDetailContentProvider>
				{
				protected:
					class ItemContent : public Object, public virtual ListViewItemStyleProvider::IListViewItemContent
					{
						typedef collections::List<elements::GuiSolidLabelElement*>		SubItemList;
					protected:
						compositions::GuiBoundsComposition*				contentComposition;
						elements::GuiImageFrameElement*					image;
						elements::GuiSolidLabelElement*					text;
						compositions::GuiTableComposition*				textTable;
						SubItemList										subItems;

						GuiListControl::IItemProvider*					itemProvider;
						ListViewColumnItemArranger::IColumnItemView*	columnItemView;

					public:
						ItemContent(Size iconSize, const FontProperties& font, GuiListControl::IItemProvider* _itemProvider);
						~ItemContent();

						compositions::GuiBoundsComposition*				GetContentComposition()override;
						compositions::GuiBoundsComposition*				GetBackgroundDecorator()override;
						void											UpdateSubItemSize();
						void											Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)override;
					};

					Size												iconSize;
					GuiListControl::IItemProvider*						itemProvider;
					ListViewColumnItemArranger::IColumnItemView*		columnItemView;
					ListViewItemStyleProvider*							listViewItemStyleProvider;

					void												OnColumnChanged()override;
				public:
					/// <summary>Create the content provider.</summary>
					/// <param name="_iconSize">The icon size.</param>
					ListViewDetailContentProvider(Size _iconSize=Size(16, 16));
					~ListViewDetailContentProvider();
					
					GuiListControl::IItemCoordinateTransformer*			CreatePreferredCoordinateTransformer()override;
					GuiListControl::IItemArranger*						CreatePreferredArranger()override;
					ListViewItemStyleProvider::IListViewItemContent*	CreateItemContent(const FontProperties& font)override;
					void												AttachListControl(GuiListControl* value)override;
					void												DetachListControl()override;
				};
			}

/***********************************************************************
ListView
***********************************************************************/

			namespace list
			{
				/// <summary>List view item.</summary>
				class ListViewItem
				{
				public:
					/// <summary>Small image.</summary>
					Ptr<GuiImageData>							smallImage;
					/// <summary>Large image.</summary>
					Ptr<GuiImageData>							largeImage;
					/// <summary>Item text.</summary>
					WString										text;
					/// <summary>Sub items.</summary>
					collections::List<WString>					subItems;
				};
				
				/// <summary>List view column.</summary>
				class ListViewColumn
				{
				public:
					/// <summary>Column text.</summary>
					WString											text;
					/// <summary>Column size.</summary>
					int												size;
					/// <summary>Column dropdown popup.</summary>
					GuiMenu*										dropdownPopup;
					/// <summary>Column sorting state.</summary>
					GuiListViewColumnHeader::ColumnSortingState		sortingState;

					/// <summary>Create a column with the specified text and size.</summary>
					/// <param name="_text">The specified text.</param>
					/// <param name="_size">The specified size.</param>
					ListViewColumn(const WString& _text=L"", int _size=160);
				};

				/// <summary>List view data column container.</summary>
				class ListViewDataColumns : public ItemsBase<int>
				{
					friend class ListViewItemProvider;
				protected:
					ListViewItemProvider*						itemProvider;

					void NotifyUpdateInternal(int start, int count, int newCount)override;
				public:
					/// <summary>Create a container.</summary>
					ListViewDataColumns();
					~ListViewDataColumns();
				};
				
				/// <summary>List view column container.</summary>
				class ListViewColumns : public ItemsBase<Ptr<ListViewColumn>>
				{
					friend class ListViewItemProvider;
				protected:
					ListViewItemProvider*						itemProvider;

					void NotifyUpdateInternal(int start, int count, int newCount)override;
				public:
					/// <summary>Create a container.</summary>
					ListViewColumns();
					~ListViewColumns();
				};
				
				/// <summary>Item provider for <see cref="GuiListViewBase"/> and <see cref="ListViewItemStyleProvider"/>.</summary>
				class ListViewItemProvider
					: public ListProvider<Ptr<ListViewItem>>
					, protected virtual ListViewItemStyleProvider::IListViewItemView
					, protected virtual ListViewColumnItemArranger::IColumnItemView
					, public Description<ListViewItemProvider>
				{
					friend class ListViewColumns;
					friend class ListViewDataColumns;
					typedef collections::List<ListViewColumnItemArranger::IColumnItemViewCallback*>		ColumnItemViewCallbackList;
				protected:
					ListViewDataColumns									dataColumns;
					ListViewColumns										columns;
					ColumnItemViewCallbackList							columnItemViewCallbacks;

					bool												ContainsPrimaryText(int itemIndex)override;
					WString												GetPrimaryTextViewText(int itemIndex)override;
					Ptr<GuiImageData>									GetSmallImage(int itemIndex)override;
					Ptr<GuiImageData>									GetLargeImage(int itemIndex)override;
					WString												GetText(int itemIndex)override;
					WString												GetSubItem(int itemIndex, int index)override;
					int													GetDataColumnCount()override;
					int													GetDataColumn(int index)override;

					bool												AttachCallback(ListViewColumnItemArranger::IColumnItemViewCallback* value)override;
					bool												DetachCallback(ListViewColumnItemArranger::IColumnItemViewCallback* value)override;
					int													GetColumnCount()override;
					WString												GetColumnText(int index)override;
					int													GetColumnSize(int index)override;
					void												SetColumnSize(int index, int value)override;
					GuiMenu*											GetDropdownPopup(int index)override;
					GuiListViewColumnHeader::ColumnSortingState			GetSortingState(int index)override;
				public:
					ListViewItemProvider();
					~ListViewItemProvider();

					IDescriptable*										RequestView(const WString& identifier)override;
					void												ReleaseView(IDescriptable* view)override;

					/// <summary>Get all data columns indices in columns.</summary>
					/// <returns>All data columns indices in columns.</returns>
					ListViewDataColumns&								GetDataColumns();
					/// <summary>Get all columns.</summary>
					/// <returns>All columns.</returns>
					ListViewColumns&									GetColumns();
				};
			}
			
			/// <summary>List view control in virtual mode.</summary>
			class GuiVirtualListView : public GuiListViewBase, public Description<GuiVirtualListView>
			{
			public:
				/// <summary>Create a list view control in virtual mode.</summary>
				/// <param name="_styleProvider">The style provider for this control.</param>
				/// <param name="_itemProvider">The item provider for this control.</param>
				GuiVirtualListView(IStyleProvider* _styleProvider, GuiListControl::IItemProvider* _itemProvider);
				~GuiVirtualListView();
				
				/// <summary>Set the item content provider.</summary>
				/// <param name="itemStyleProvider">The new item content provider.</param>
				void											ChangeItemStyle(list::ListViewItemStyleProvider::IListViewItemContentProvider* contentProvider);
			};
			
			/// <summary>List view control in virtual mode.</summary>
			class GuiListView : public GuiVirtualListView, public Description<GuiListView>
			{
			protected:
				list::ListViewItemProvider*						items;
			public:
				/// <summary>Create a list view control.</summary>
				/// <param name="_styleProvider">The style provider for this control.</param>
				GuiListView(IStyleProvider* _styleProvider);
				~GuiListView();
				
				/// <summary>Get all list view items.</summary>
				/// <returns>All list view items.</returns>
				list::ListViewItemProvider&						GetItems();
			};
		}
	}
}

#endif