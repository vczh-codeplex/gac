/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUILISTCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUILISTCONTROLS

#include "GuiBasicControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
List Control
***********************************************************************/

			/// <summary>Represents a list control. A list control automatically read data sources and creates corresponding data item control from the item template.</summary>
			class GuiListControl : public GuiScrollView, public Description<GuiListControl>
			{
			public:
				class IItemProvider;
				class IItemStyleController;
				class IItemStyleProvider;

				//-----------------------------------------------------------
				// Callback Interfaces
				//-----------------------------------------------------------

				/// <summary>Item provider callback. Item providers use this interface to notify item modification.</summary>
				class IItemProviderCallback : public virtual IDescriptable, public Description<IItemProviderCallback>
				{
				public:
					/// <summary>Called when an item provider callback object is attached to an item provider.</summary>
					/// <param name="provider">The item provider.</param>
					virtual void								OnAttached(IItemProvider* provider)=0;
					/// <summary>Called when items in the item provider is modified.</summary>
					/// <param name="start">The index of the first modified item.</param>
					/// <param name="count">The number of all modified items.</param>
					/// <param name="newCount">The number of new items. If items are inserted or removed, newCount may not equals to count.</param>
					virtual void								OnItemModified(int start, int count, int newCount)=0;
				};

				/// <summary>Item arranger callback. Item arrangers use this interface to communicate with the list control. When setting positions for item controls, functions in this callback object is suggested to call because they use the result from the [T:vl.presentation.controls.GuiListControl.IItemCoordinateTransformer].</summary>
				class IItemArrangerCallback : public virtual IDescriptable, public Description<IItemArrangerCallback>
				{
				public:
					/// <summary>Request an item control representing an item in the item provider. This function is suggested to call when an item control gets into the visible area.</summary>
					/// <returns>The item control.</returns>
					/// <param name="itemIndex">The index of the item in the item provider.</param>
					virtual IItemStyleController*					RequestItem(int itemIndex)=0;
					/// <summary>Release an item control. This function is suggested to call when an item control gets out of the visible area.</summary>
					/// <param name="style">The item control.</param>
					virtual void									ReleaseItem(IItemStyleController* style)=0;
					/// <summary>Update the view location. The view location is the left-top position in the logic space of the list control.</summary>
					/// <param name="value">The new view location.</param>
					virtual void									SetViewLocation(Point value)=0;
					/// <summary>Get the preferred size of an item control.</summary>
					/// <returns>The preferred size of an item control.</returns>
					/// <param name="style">The item control.</param>
					virtual Size									GetStylePreferredSize(IItemStyleController* style)=0;
					/// <summary>Set the alignment of an item control.</summary>
					/// <param name="style">The item control.</param>
					/// <param name="margin">The new alignment.</param>
					virtual void									SetStyleAlignmentToParent(IItemStyleController* style, Margin margin)=0;
					/// <summary>Set the bounds of an item control.</summary>
					/// <param name="style">The item control.</param>
					/// <param name="margin">The new bounds.</param>
					virtual void									SetStyleBounds(IItemStyleController* style, Rect bounds)=0;
					/// <summary>Get the <see cref="compositions::GuiGraphicsComposition"/> that directly contains item controls.</summary>
					/// <returns>The <see cref="compositions::GuiGraphicsComposition"/> that directly contains item controls.</returns>
					virtual compositions::GuiGraphicsComposition*	GetContainerComposition()=0;
					/// <summary>Notify the list control that the total size of all item controls are changed.</summary>
					virtual void									OnTotalSizeChanged()=0;
				};

				//-----------------------------------------------------------
				// Common Views
				//-----------------------------------------------------------

				/// <summary>Primary text view for <see cref="IItemProvider"/>.</summary>
				class IItemPrimaryTextView : public virtual IDescriptable, public Description<IItemPrimaryTextView>
				{
				public:
					/// <summary>The identifier for this view.</summary>
					static const wchar_t* const					Identifier;

					/// <summary>Get the text of an item.</summary>
					/// <returns>The text of an item.</returns>
					/// <param name="itemIndex">The index of the item.</param>
					virtual WString								GetPrimaryTextViewText(int itemIndex)=0;
					/// <summary>Test does an item contain a text.</summary>
					/// <returns>Returns true if an item contains a text.</returns>
					/// <param name="itemIndex">The index of the item.</param>
					virtual bool								ContainsPrimaryText(int itemIndex)=0;
				};

				//-----------------------------------------------------------
				// Provider Interfaces
				//-----------------------------------------------------------

				/// <summary>Item provider for a <see cref="GuiListControl"/>.</summary>
				class IItemProvider : public virtual IDescriptable, public Description<IItemProvider>
				{
				public:
					/// <summary>Attach an item provider callback to this item provider.</summary>
					/// <returns>Returns true if this operation succeeded.</returns>
					/// <param name="value">The item provider callback.</param>
					virtual bool								AttachCallback(IItemProviderCallback* value)=0;
					/// <summary>Detach an item provider callback from this item provider.</summary>
					/// <returns>Returns true if this operation succeeded.</returns>
					/// <param name="value">The item provider callback.</param>
					virtual bool								DetachCallback(IItemProviderCallback* value)=0;
					/// <summary>Get the number of items in this item proivder.</summary>
					/// <returns>The number of items in this item proivder.</returns>
					virtual int									Count()=0;
					/// <summary>Request a view for this item provider. If the specified view is not supported, it returns null. If you want to get a view of type IXXX, use IXXX::Identifier as the identifier. When the view object is no longer needed, Calling to the [M:vl.presentation.controls.GuiListControl.IItemProvider.ReleaseView] is needed.</summary>
					/// <returns>The view object.</returns>
					/// <param name="identifier">The identifier for the requested view.</param>
					virtual IDescriptable*						RequestView(const WString& identifier)=0;
					/// <summary>Release a requested view.</summary>
					/// <param name="view">The view to release.</param>
					virtual void								ReleaseView(IDescriptable* view)=0;
				};
				
				/// <summary>Item style controller for a <see cref="GuiListControl"/>. Item style controller contains all information to render a binded item.</summary>
				class IItemStyleController : public virtual IDescriptable, public Description<IItemStyleController>
				{
				public:
					/// <summary>Get the owner [T:vl.presentation.controls.GuiListControl.IItemStyleProvider].</summary>
					/// <returns>The owner.</returns>
					virtual IItemStyleProvider*					GetStyleProvider()=0;
					/// <summary>Get the item style id for the binded item.</summary>
					/// <returns>The item style id.</returns>
					virtual int									GetItemStyleId()=0;
					/// <summary>Get the bounds composition that represnets the binded item.</summary>
					/// <returns>The bounds composition</returns>
					virtual compositions::GuiBoundsComposition*	GetBoundsComposition()=0;
					/// <summary>Test is this item style controller cacheable.</summary>
					/// <returns>Returns true if this item style controller is cacheable.</returns>
					virtual bool								IsCacheable()=0;
					/// <summary>Test is there an item binded to this item style controller.</summary>
					/// <returns>Returns true if an item is binded to this item style controller.</returns>
					virtual bool								IsInstalled()=0;
					/// <summary>Called when an item is binded to this item style controller.</summary>
					virtual void								OnInstalled()=0;
					/// <summary>Called when an item is unbinded to this item style controller.</summary>
					virtual void								OnUninstalled()=0;
				};
				
				/// <summary>Item style provider for a <see cref="GuiListControl"/>. When implementing an item style provider, the provider can require the item provider to support a specified view to access data for each item.</summary>
				class IItemStyleProvider : public virtual IDescriptable, public Description<IItemStyleProvider>
				{
				public:
					/// <summary>Called when an item style provider in installed to a <see cref="GuiListControl"/>.</summary>
					/// <param name="value">The list control.</param>
					virtual void								AttachListControl(GuiListControl* value)=0;
					/// <summary>Called when an item style provider in uninstalled from a <see cref="GuiListControl"/>.</summary>
					virtual void								DetachListControl()=0;
					/// <summary>Get a item style id from an item index.</summary>
					/// <returns>The item style id.</returns>
					/// <param name="itemIndex">The item index.</param>
					virtual int									GetItemStyleId(int itemIndex)=0;
					/// <summary>Create an item style controller from an item style id.</summary>
					/// <returns>The created item style controller.</returns>
					/// <param name="itemIndex">The item style id.</param>
					virtual IItemStyleController*				CreateItemStyle(int styleId)=0;
					/// <summary>Destroy an item style controller.</summary>
					/// <param name="style">The item style controller.</param>
					virtual void								DestroyItemStyle(IItemStyleController* style)=0;
					/// <summary>Bind an item to an item style controller.</summary>
					/// <param name="style">The item style controller.</param>
					/// <param name="itemIndex">The item style id.</param>
					virtual void								Install(IItemStyleController* style, int itemIndex)=0;
				};
				
				/// <summary>Item arranger for a <see cref="GuiListControl"/>. Item arranger decides how to arrange and item controls. When implementing an item arranger, <see cref="IItemArrangerCallback"/> is suggested to use when calculating locations and sizes for item controls.</summary>
				class IItemArranger : public virtual IItemProviderCallback, public Description<IItemArranger>
				{
				public:
					/// <summary>Called when an item arranger in installed to a <see cref="GuiListControl"/>.</summary>
					/// <param name="value">The list control.</param>
					virtual void								AttachListControl(GuiListControl* value)=0;
					/// <summary>Called when an item arranger in uninstalled from a <see cref="GuiListControl"/>.</summary>
					virtual void								DetachListControl()=0;
					/// <summary>Get the binded item arranger callback object.</summary>
					/// <returns>The binded item arranger callback object.</returns>
					virtual IItemArrangerCallback*				GetCallback()=0;
					/// <summary>Bind the item arranger callback object.</summary>
					/// <param name="value">The item arranger callback object to bind.</param>
					virtual void								SetCallback(IItemArrangerCallback* value)=0;
					/// <summary>Get the total size of all data controls.</summary>
					/// <returns>The total size.</returns>
					virtual Size								GetTotalSize()=0;
					/// <summary>Get the item style controller for an visible item index. If an item is not visible, it returns null.</summary>
					/// <returns>The item style controller.</returns>
					/// <param name="itemIndex">The item index.</param>
					virtual IItemStyleController*				GetVisibleStyle(int itemIndex)=0;
					/// <summary>Get the item index for an visible item style controller. If an item is not visible, it returns -1.</summary>
					/// <returns>The item index.</returns>
					/// <param name="style">The item style controller.</param>
					virtual int									GetVisibleIndex(IItemStyleController* style)=0;
					/// <summary>Called when the visible area of item container is changed.</summary>
					virtual void								OnViewChanged(Rect bounds)=0;
				};
				
				/// <summary>Item coordinate transformer for a <see cref="GuiListControl"/>. In all functions in this interface, real coordinate is in the list control's container space, virtual coordinate is in a space that the transformer created.</summary>
				class IItemCoordinateTransformer : public virtual IDescriptable, public Description<IItemCoordinateTransformer>
				{
				public:
					/// <summary>Translate real size to virtual size.</summary>
					/// <returns>The virtual size.</returns>
					/// <param name="size">The real size.</param>
					virtual Size								RealSizeToVirtualSize(Size size)=0;
					/// <summary>Translate virtual size to real size.</summary>
					/// <returns>The real size.</returns>
					/// <param name="size">The virtual size.</param>
					virtual Size								VirtualSizeToRealSize(Size size)=0;
					/// <summary>Translate real point to virtual point.</summary>
					/// <returns>The virtual point.</returns>
					/// <param name="realFullSize">The real full size.</param>
					/// <param name="point">The real point.</param>
					virtual Point								RealPointToVirtualPoint(Size realFullSize, Point point)=0;
					/// <summary>Translate virtual point to real point.</summary>
					/// <returns>The real point.</returns>
					/// <param name="realFullSize">The real full size.</param>
					/// <param name="point">The virtual point.</param>
					virtual Point								VirtualPointToRealPoint(Size realFullSize, Point point)=0;
					/// <summary>Translate real bounds to virtual bounds.</summary>
					/// <returns>The virtual bounds.</returns>
					/// <param name="realFullSize">The real full size.</param>
					/// <param name="rect">The real bounds.</param>
					virtual Rect								RealRectToVirtualRect(Size realFullSize, Rect rect)=0;
					/// <summary>Translate virtual bounds to real bounds.</summary>
					/// <returns>The real bounds.</returns>
					/// <param name="realFullSize">The real full size.</param>
					/// <param name="rect">The virtual bounds.</param>
					virtual Rect								VirtualRectToRealRect(Size realFullSize, Rect rect)=0;
					/// <summary>Translate real margin to margin size.</summary>
					/// <returns>The virtual margin.</returns>
					/// <param name="margin">The real margin.</param>
					virtual Margin								RealMarginToVirtualMargin(Margin margin)=0;
					/// <summary>Translate virtual margin to margin size.</summary>
					/// <returns>The real margin.</returns>
					/// <param name="margin">The virtual margin.</param>
					virtual Margin								VirtualMarginToRealMargin(Margin margin)=0;
				};

			protected:
				class ItemCallback : public IItemProviderCallback, public IItemArrangerCallback
				{
					typedef collections::List<IItemStyleController*>			StyleList;
				protected:
					GuiListControl*								listControl;
					IItemProvider*								itemProvider;
					StyleList									cachedStyles;
					StyleList									installedStyles;

				public:
					ItemCallback(GuiListControl* _listControl);
					~ItemCallback();

					void										ClearCache();

					void										OnAttached(IItemProvider* provider)override;
					void										OnItemModified(int start, int count, int newCount)override;
					IItemStyleController*						RequestItem(int itemIndex)override;
					void										ReleaseItem(IItemStyleController* style)override;
					void										SetViewLocation(Point value)override;
					Size										GetStylePreferredSize(IItemStyleController* style);
					void										SetStyleAlignmentToParent(IItemStyleController* style, Margin margin);
					void										SetStyleBounds(IItemStyleController* style, Rect bounds);
					compositions::GuiGraphicsComposition*		GetContainerComposition()override;
					void										OnTotalSizeChanged()override;
				};

				Ptr<ItemCallback>								callback;
				Ptr<IItemProvider>								itemProvider;
				Ptr<IItemStyleProvider>							itemStyleProvider;
				Ptr<IItemArranger>								itemArranger;
				Ptr<IItemCoordinateTransformer>					itemCoordinateTransformer;
				Size											fullSize;

				virtual void									OnItemModified(int start, int count, int newCount);
				virtual void									OnStyleInstalled(int itemIndex, IItemStyleController* style);
				virtual void									OnStyleUninstalled(IItemStyleController* style);
				
				void											OnRenderTargetChanged(elements::IGuiGraphicsRenderTarget* renderTarget)override;
				void											OnBeforeReleaseGraphicsHost()override;
				Size											QueryFullSize()override;
				void											UpdateView(Rect viewBounds)override;
				
				void											OnBoundsMouseButtonDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
				void											SetStyleProviderAndArranger(Ptr<IItemStyleProvider> styleProvider, Ptr<IItemArranger> arranger);
			public:
				/// <summary>Create a control with a specified style provider.</summary>
				/// <param name="styleProvider">The style provider.</param>
				/// <param name="_itemProvider">The item provider as a data source.</param>
				/// <param name="acceptFocus">Set to true if the list control is allowed to have a keyboard focus.</param>
				GuiListControl(IStyleProvider* _styleProvider, IItemProvider* _itemProvider, bool acceptFocus=false);
				~GuiListControl();

				/// <summary>Style provider changed event.</summary>
				compositions::GuiNotifyEvent					StyleProviderChanged;
				/// <summary>Arranger changed event.</summary>
				compositions::GuiNotifyEvent					ArrangerChanged;
				/// <summary>Coordinate transformer changed event.</summary>
				compositions::GuiNotifyEvent					CoordinateTransformerChanged;

				/// <summary>Get the item provider.</summary>
				/// <returns>The item provider.</returns>
				virtual IItemProvider*							GetItemProvider();
				/// <summary>Get the item style provider.</summary>
				/// <returns>The item style provider.</returns>
				virtual IItemStyleProvider*						GetStyleProvider();
				/// <summary>Set the item style provider</summary>
				/// <returns>The old item style provider</returns>
				/// <param name="value">The new item style provider</param>
				virtual Ptr<IItemStyleProvider>					SetStyleProvider(Ptr<IItemStyleProvider> value);
				/// <summary>Get the item arranger.</summary>
				/// <returns>The item arranger.</returns>
				virtual IItemArranger*							GetArranger();
				/// <summary>Set the item arranger</summary>
				/// <returns>The old item arranger</returns>
				/// <param name="value">The new item arranger</param>
				virtual Ptr<IItemArranger>						SetArranger(Ptr<IItemArranger> value);
				/// <summary>Get the item coordinate transformer.</summary>
				/// <returns>The item coordinate transformer.</returns>
				virtual IItemCoordinateTransformer*				GetCoordinateTransformer();
				/// <summary>Set the item coordinate transformer</summary>
				/// <returns>The old item coordinate transformer</returns>
				/// <param name="value">The new item coordinate transformer</param>
				virtual Ptr<IItemCoordinateTransformer>			SetCoordinateTransformer(Ptr<IItemCoordinateTransformer> value);
			};

/***********************************************************************
Selectable List Control
***********************************************************************/

			/// <summary>Represents a list control that each item is selectable.</summary>
			class GuiSelectableListControl : public GuiListControl, public Description<GuiSelectableListControl>
			{
			public:
				/// <summary>Item style provider for <see cref="GuiSelectableListControl"/>.</summary>
				class IItemStyleProvider : public virtual GuiListControl::IItemStyleProvider, public Description<IItemStyleProvider>
				{
				public:
					/// <summary>Change the visual affect of an item style controller to be selected or unselected.</summary>
					/// <param name="style">The item style controller.</param>
					/// <param name="value">Set to true if the item is expected to be rendered as selected.</param>
					virtual void								SetStyleSelected(IItemStyleController* style, bool value)=0;
				};

			protected:
				class StyleEvents
				{
				protected:
					GuiSelectableListControl*					listControl;
					IItemStyleController*						style;
					Ptr<compositions::GuiMouseEvent::IHandler>	leftButtonDownHandler;

					void										OnBoundsLeftButtonDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
				public:
					StyleEvents(GuiSelectableListControl* _listControl, IItemStyleController* _style);
					~StyleEvents();

					void										AttachEvents();
					void										DetachEvents();
				};

				friend class collections::ReadonlyListEnumerator<Ptr<StyleEvents>>;
				typedef collections::Dictionary<IItemStyleController*, Ptr<StyleEvents>>	VisibleStyleMap;

			protected:

				Ptr<IItemStyleProvider>							selectableStyleProvider;
				collections::SortedList<int>					selectedItems;
				VisibleStyleMap									visibleStyles;
				bool											multiSelect;

				void											OnItemModified(int start, int count, int newCount)override;
				void											OnStyleInstalled(int itemIndex, IItemStyleController* style)override;
				void											OnStyleUninstalled(IItemStyleController* style)override;
				virtual void									OnItemSelectionChanged(int itemIndex, bool value);
				virtual void									OnItemSelectionCleared();
			public:
				/// <summary>Create a control with a specified style provider.</summary>
				/// <param name="styleProvider">The style provider.</param>
				/// <param name="_itemProvider">The item provider as a data source.</param>
				GuiSelectableListControl(IStyleProvider* _styleProvider, IItemProvider* _itemProvider);
				~GuiSelectableListControl();

				/// <summary>Selection changed event.</summary>
				compositions::GuiNotifyEvent					SelectionChanged;

				Ptr<GuiListControl::IItemStyleProvider>			SetStyleProvider(Ptr<GuiListControl::IItemStyleProvider> value)override;

				/// <summary>Get the multiple selection mode.</summary>
				/// <returns>Returns true if multiple selection is enabled.</returns>
				bool											GetMultiSelect();
				/// <summary>Set the multiple selection mode.</summary>
				/// <param name="value">Set to true to enable multiple selection.</param>
				void											SetMultiSelect(bool value);
				
				/// <summary>Get indices of all selected items.</summary>
				/// <returns>Indices of all selected items.</returns>
				const collections::IReadonlyList<int>&			GetSelectedItems();
				/// <summary>Get the selection status of an item.</summary>
				/// <returns>The selection status of an item.</returns>
				/// <param name="itemIndex">The index of the item.</param>
				bool											GetSelected(int itemIndex);
				/// <summary>Set the selection status of an item.</summary>
				/// <param name="itemIndex">The index of the item.</param>
				/// <param name="value">Set to true to select the item.</param>
				void											SetSelected(int itemIndex, bool value);
				/// <summary>Unselect all items.</summary>
				void											ClearSelection();
			};

/***********************************************************************
Predefined ItemCoordinateTransformer
***********************************************************************/

			namespace list
			{
				/// <summary>Default item coordinate transformer. This transformer doesn't transform any coordinate.</summary>
				class DefaultItemCoordinateTransformer : public Object, virtual public GuiListControl::IItemCoordinateTransformer, public Description<DefaultItemCoordinateTransformer>
				{
				public:
					/// <summary>Create the transformer.</summary>
					DefaultItemCoordinateTransformer();
					~DefaultItemCoordinateTransformer();

					Size										RealSizeToVirtualSize(Size size)override;
					Size										VirtualSizeToRealSize(Size size)override;
					Point										RealPointToVirtualPoint(Size realFullSize, Point point)override;
					Point										VirtualPointToRealPoint(Size realFullSize, Point point)override;
					Rect										RealRectToVirtualRect(Size realFullSize, Rect rect)override;
					Rect										VirtualRectToRealRect(Size realFullSize, Rect rect)override;
					Margin										RealMarginToVirtualMargin(Margin margin)override;
					Margin										VirtualMarginToRealMargin(Margin margin)override;
				};
				
				/// <summary>Axis aligned item coordinate transformer. This transformer transforms coordinates by changing the axis direction.</summary>
				class AxisAlignedItemCoordinateTransformer : public Object, virtual public GuiListControl::IItemCoordinateTransformer, public Description<AxisAlignedItemCoordinateTransformer>
				{
				public:
					/// <summary>Axis direction.</summary>
					enum Alignment
					{
						/// <summary>[T:vl.presentation.controls.list.AxisAlignedItemCoordinateTransformer.Alignment]X:left, Y:down.</summary>
						LeftDown,
						/// <summary>[T:vl.presentation.controls.list.AxisAlignedItemCoordinateTransformer.Alignment]X:right, Y:down.</summary>
						RightDown,
						/// <summary>[T:vl.presentation.controls.list.AxisAlignedItemCoordinateTransformer.Alignment]X:left, Y:up.</summary>
						LeftUp,
						/// <summary>[T:vl.presentation.controls.list.AxisAlignedItemCoordinateTransformer.Alignment]X:right, Y:up.</summary>
						RightUp,
						/// <summary>[T:vl.presentation.controls.list.AxisAlignedItemCoordinateTransformer.Alignment]X:down, Y:left.</summary>
						DownLeft,
						/// <summary>[T:vl.presentation.controls.list.AxisAlignedItemCoordinateTransformer.Alignment]X:down, Y:right.</summary>
						DownRight,
						/// <summary>[T:vl.presentation.controls.list.AxisAlignedItemCoordinateTransformer.Alignment]X:up, Y:left.</summary>
						UpLeft,
						/// <summary>[T:vl.presentation.controls.list.AxisAlignedItemCoordinateTransformer.Alignment]X:up, Y:right.</summary>
						UpRight,
					};
				protected:
					Alignment									alignment;

				public:
					/// <summary>Create the transformer with a specified axis direction.</summary>
					/// <param name="_alignment">The specified axis direction.</param>
					AxisAlignedItemCoordinateTransformer(Alignment _alignment);
					~AxisAlignedItemCoordinateTransformer();

					/// <summary>Get the specified axis direction.</summary>
					/// <returns>The specified axis direction.</returns>
					Alignment									GetAlignment();
					Size										RealSizeToVirtualSize(Size size)override;
					Size										VirtualSizeToRealSize(Size size)override;
					Point										RealPointToVirtualPoint(Size realFullSize, Point point)override;
					Point										VirtualPointToRealPoint(Size realFullSize, Point point)override;
					Rect										RealRectToVirtualRect(Size realFullSize, Rect rect)override;
					Rect										VirtualRectToRealRect(Size realFullSize, Rect rect)override;
					Margin										RealMarginToVirtualMargin(Margin margin)override;
					Margin										VirtualMarginToRealMargin(Margin margin)override;
				};
			};

/***********************************************************************
Predefined ItemArranger
***********************************************************************/

			namespace list
			{
				/// <summary>Ranged item arranger. This arranger implements most of the common functionality for those arrangers that display a continuing subset of item at a time.</summary>
				class RangedItemArrangerBase : public Object, virtual public GuiListControl::IItemArranger, public Description<RangedItemArrangerBase>
				{
					typedef collections::List<GuiListControl::IItemStyleController*>		StyleList;
				protected:
					GuiListControl::IItemArrangerCallback*		callback;
					GuiListControl::IItemProvider*				itemProvider;
					Rect										viewBounds;
					int											startIndex;
					StyleList									visibleStyles;

					virtual void								ClearStyles();
					virtual void								OnStylesCleared()=0;
					virtual Size								OnCalculateTotalSize()=0;
					virtual void								OnViewChangedInternal(Rect oldBounds, Rect newBounds)=0;
				public:
					/// <summary>Create the arranger.</summary>
					RangedItemArrangerBase();
					~RangedItemArrangerBase();

					void										OnAttached(GuiListControl::IItemProvider* provider)override;
					void										OnItemModified(int start, int count, int newCount)override;
					void										AttachListControl(GuiListControl* value)override;
					void										DetachListControl()override;
					GuiListControl::IItemArrangerCallback*		GetCallback()override;
					void										SetCallback(GuiListControl::IItemArrangerCallback* value)override;
					Size										GetTotalSize()override;
					GuiListControl::IItemStyleController*		GetVisibleStyle(int itemIndex)override;
					int											GetVisibleIndex(GuiListControl::IItemStyleController* style)override;
					void										OnViewChanged(Rect bounds)override;
				};
				
				/// <summary>Fixed height item arranger. This arranger lists all item with the same height value. This value is the maximum height of all minimum heights of displayed items.</summary>
				class FixedHeightItemArranger : public RangedItemArrangerBase, public Description<FixedHeightItemArranger>
				{
				protected:
					int											rowHeight;
					bool										suppressOnViewChanged;

					virtual void								RearrangeItemBounds();
					virtual int									GetWidth();
					virtual int									GetYOffset();
					void										OnStylesCleared()override;
					Size										OnCalculateTotalSize()override;
					void										OnViewChangedInternal(Rect oldBounds, Rect newBounds)override;
				public:
					/// <summary>Create the arranger.</summary>
					FixedHeightItemArranger();
					~FixedHeightItemArranger();
				};

				/// <summary>Fixed size multiple columns item arranger. This arranger adjust all items in multiple lines with the same size. The width is the maximum width of all minimum widths of displayed items. The same to height.</summary>
				class FixedSizeMultiColumnItemArranger : public RangedItemArrangerBase, public Description<FixedSizeMultiColumnItemArranger>
				{
				protected:
					Size										itemSize;
					bool										suppressOnViewChanged;

					virtual void								RearrangeItemBounds();
					void										CalculateRange(Size itemSize, Rect bounds, int count, int& start, int& end);
					void										OnStylesCleared()override;
					Size										OnCalculateTotalSize()override;
					void										OnViewChangedInternal(Rect oldBounds, Rect newBounds)override;
				public:
					/// <summary>Create the arranger.</summary>
					FixedSizeMultiColumnItemArranger();
					~FixedSizeMultiColumnItemArranger();
				};
				
				/// <summary>Fixed size multiple columns item arranger. This arranger adjust all items in multiple columns with the same height. The height is the maximum width of all minimum height of displayed items. Each item will displayed using its minimum width.</summary>
				class FixedHeightMultiColumnItemArranger : public RangedItemArrangerBase, public Description<FixedHeightMultiColumnItemArranger>
				{
				protected:
					int											itemHeight;
					bool										suppressOnViewChanged;

					virtual void								RearrangeItemBounds();
					void										CalculateRange(int itemHeight, Rect bounds, int& rows, int& startColumn);
					void										OnStylesCleared()override;
					Size										OnCalculateTotalSize()override;
					void										OnViewChangedInternal(Rect oldBounds, Rect newBounds)override;
				public:
					/// <summary>Create the arranger.</summary>
					FixedHeightMultiColumnItemArranger();
					~FixedHeightMultiColumnItemArranger();
				};
			}

/***********************************************************************
Predefined ItemStyleController
***********************************************************************/

			namespace list
			{
				/// <summary>Item style controller base. This class provides common functionalities item style controllers.</summary>
				class ItemStyleControllerBase : public Object, public virtual GuiListControl::IItemStyleController, public Description<ItemStyleControllerBase>
				{
				protected:
					GuiListControl::IItemStyleProvider*			provider;
					int											styleId;
					compositions::GuiBoundsComposition*			boundsComposition;
					GuiControl*									associatedControl;
					bool										isInstalled;

					void										Initialize(compositions::GuiBoundsComposition* _boundsComposition, GuiControl* _associatedControl);
					void										Finalize();

					/// <summary>Create the item style controller with a specified item style provider and a specified item style id.</summary>
					/// <param name="_provider">The specified item style provider.</param>
					/// <param name="_styleId">The specified item style id.</param>
					ItemStyleControllerBase(GuiListControl::IItemStyleProvider* _provider, int _styleId);
				public:
					~ItemStyleControllerBase();
					
					GuiListControl::IItemStyleProvider*			GetStyleProvider()override;
					int											GetItemStyleId()override;
					compositions::GuiBoundsComposition*			GetBoundsComposition()override;
					bool										IsCacheable()override;
					bool										IsInstalled()override;
					void										OnInstalled()override;
					void										OnUninstalled()override;
				};
			}

/***********************************************************************
Predefined ItemProvider
***********************************************************************/

			namespace list
			{
				/// <summary>Item provider base. This class provider common functionalities for item providers.</summary>
				class ItemProviderBase : public Object, public virtual GuiListControl::IItemProvider, public Description<ItemProviderBase>
				{
				protected:
					collections::List<GuiListControl::IItemProviderCallback*>	callbacks;

					virtual void								InvokeOnItemModified(int start, int count, int newCount);
				public:
					/// <summary>Create the item provider.</summary>
					ItemProviderBase();
					~ItemProviderBase();

					bool										AttachCallback(GuiListControl::IItemProviderCallback* value);
					bool										DetachCallback(GuiListControl::IItemProviderCallback* value);
				};

				template<typename T, typename K=typename KeyType<T>::Type>
				class ListWrapperProvider : public ItemProviderBase, public collections::IList<T, K>
				{
				protected:
					collections::IList<T, K>*			proxy;

					ListWrapperProvider()
						:proxy(0)
					{
					}
				public:
					ListWrapperProvider(collections::IList<T, K>* _proxy)
						:proxy(_proxy)
					{
					}

					~ListWrapperProvider()
					{
					}

					bool NotifyUpdate(int start, int count=1)
					{
						if(start<0 || start>=proxy->Count() || count<=0 || start+count>proxy->Count())
						{
							return false;
						}
						else
						{
							InvokeOnItemModified(start, count, count);
							return true;
						}
					}

					collections::IEnumerator<T>* CreateEnumerator()const
					{
						return proxy->CreateEnumerator();
					}

					bool Contains(const K& item)const
					{
						return proxy->Contains(item);
					}

					vint Count()const
					{
						return proxy->Count();
					}

					vint Count()
					{
						return proxy->Count();
					}

					const T& Get(vint index)const
					{
						return proxy->Get(index);
					}

					const T& operator[](vint index)const
					{
						return (*proxy)[index];
					}

					vint IndexOf(const K& item)const
					{
						return proxy->IndexOf(item);
					}

					vint Add(const T& item)
					{
						return Insert(proxy->Count(), item);
					}

					bool Remove(const K& item)
					{
						vint index=proxy->IndexOf(item);
						if(index==-1) return false;
						return RemoveAt(index);
					}

					bool RemoveAt(vint index)
					{
						if(proxy->RemoveAt(index))
						{
							InvokeOnItemModified(index, 1, 0);
							return true;
						}
						else
						{
							return false;
						}
					}

					bool RemoveRange(vint index, vint count)
					{
						if(proxy->RemoveRange(index, count))
						{
							InvokeOnItemModified(index, count, 0);
							return true;
						}
						else
						{
							return false;
						}
					}

					bool Clear()
					{
						vint count=proxy->Count();
						if(proxy->Clear())
						{
							InvokeOnItemModified(0, count, 0);
							return true;
						}
						else
						{
							return false;
						}
					}

					vint Insert(vint index, const T& item)
					{
						vint result=proxy->Insert(index, item);
						InvokeOnItemModified(index, 0, 1);
						return result;
					}

					bool Set(vint index, const T& item)
					{
						if(proxy->Set(index, item))
						{
							InvokeOnItemModified(index, 1, 1);
							return true;
						}
						else
						{
							return false;
						}
					}
				};

				template<typename T>
				class ListProvider : public ListWrapperProvider<T>
				{
				protected:
					collections::List<T>		list;

				public:
					ListProvider()
					{
						proxy=&list.Wrap();
					}

					~ListProvider()
					{
					}
				};
			}
		}
	}
}

#endif