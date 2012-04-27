/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Composition System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSCOMPOSITION
#define VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSCOMPOSITION

#include "GuiGraphicsElement.h"
#include "GuiGraphicsTextElement.h"
#include "GuiGraphicsEventReceiver.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			class GuiControl;
			class GuiControlHost;
		}

		namespace compositions
		{
			class GuiGraphicsHost;

/***********************************************************************
Basic Construction
***********************************************************************/

			/// <summary>
			/// Represents a composition for <see cref="elements::IGuiGraphicsElement"/>. A composition is a way to define the size and the position using the information from graphics elements and sub compositions.
			/// When a graphics composition is destroyed, all sub composition will be destroyed. The life cycle of the contained graphics element is partially controlled by the smart pointer to the graphics element inside the composition.
			/// </summary>
			class GuiGraphicsComposition : public Object, public Description<GuiGraphicsComposition>
			{
				typedef collections::IReadonlyList<GuiGraphicsComposition*> ICompositionList;
				typedef collections::List<GuiGraphicsComposition*> CompositionList;

				friend class controls::GuiControl;
				friend class GuiGraphicsHost;
			public:
				/// <summary>
				/// Minimum size limitation.
				/// </summary>
				enum MinSizeLimitation
				{
					/// <summary>[T:vl.presentation.compositions.GuiGraphicsComposition.MinSizeLimitation]No limitation for the minimum size.</summary>
					NoLimit,
					/// <summary>[T:vl.presentation.compositions.GuiGraphicsComposition.MinSizeLimitation]Minimum size of this composition is the minimum size of the contained graphics element.</summary>
					LimitToElement,
					/// <summary>[T:vl.presentation.compositions.GuiGraphicsComposition.MinSizeLimitation]Minimum size of this composition is combiniation of sub compositions and the minimum size of the contained graphics element.</summary>
					LimitToElementAndChildren,
				};
				
				/// <summary>
				/// Relation between the composition size and the parent composition.
				/// </summary>
				enum ParentSizeAffection
				{
					/// <summary>[T:vl.presentation.compositions.GuiGraphicsComposition.ParentSizeAffection]The size doesn't aware of the parent composition.</summary>
					NotAffectedByParent,
					/// <summary>[T:vl.presentation.compositions.GuiGraphicsComposition.ParentSizeAffection]The size partially limited by the parent composition.</summary>
					AffectedByParent,
					/// <summary>[T:vl.presentation.compositions.GuiGraphicsComposition.ParentSizeAffection]The size is decided by the parent composition.</summary>
					TotallyDecidedByParent,
				};
			protected:
				CompositionList								children;
				GuiGraphicsComposition*						parent;
				Ptr<elements::IGuiGraphicsElement>			ownedElement;
				bool										visible;
				elements::IGuiGraphicsRenderTarget*			renderTarget;
				MinSizeLimitation							minSizeLimitation;

				Ptr<compositions::GuiGraphicsEventReceiver>	eventReceiver;
				controls::GuiControl*						associatedControl;
				GuiGraphicsHost*							associatedHost;
				INativeCursor*								associatedCursor;

				Margin										margin;
				Margin										internalMargin;
				Size										preferredMinSize;

				virtual void								OnControlParentChanged(controls::GuiControl* control);
				virtual void								OnChildInserted(GuiGraphicsComposition* child);
				virtual void								OnChildRemoved(GuiGraphicsComposition* child);
				virtual void								OnParentChanged(GuiGraphicsComposition* oldParent, GuiGraphicsComposition* newParent);
				virtual void								OnRenderTargetChanged();
				
				virtual void								SetAssociatedControl(controls::GuiControl* control);
				virtual void								SetAssociatedHost(GuiGraphicsHost* host);
			public:
				GuiGraphicsComposition();
				~GuiGraphicsComposition();

				/// <summary>Get the parent composition.</summary>
				/// <returns>The parent composition.</returns>
				GuiGraphicsComposition*						GetParent();
				/// <summary>Get all child compositions ordered by z-order from low to high.</summary>
				/// <returns>Child compositions.</returns>
				const ICompositionList&						Children();
				/// <summary>Add a composition as a child.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="child">The child composition to add.</param>
				bool										AddChild(GuiGraphicsComposition* child);
				/// <summary>Add a composition as a child with a specified z-order.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="index">The z-order. 0 means the lowest position.</param>
				/// <param name="child">The child composition to add.</param>
				bool										InsertChild(int index, GuiGraphicsComposition* child);
				/// <summary>Remove a child composition.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="child">The child composition to remove.</param>
				bool										RemoveChild(GuiGraphicsComposition* child);
				/// <summary>Move a child composition to a new z-order.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="child">The child composition to move.</param>
				/// <param name="newIndex">The new z-order. 0 means the lowest position.</param>
				bool										MoveChild(GuiGraphicsComposition* child, int newIndex);

				/// <summary>Get the contained graphics element.</summary>
				/// <returns>The contained graphics element.</returns>
				Ptr<elements::IGuiGraphicsElement>			GetOwnedElement();
				/// <summary>Set the contained graphics element.</summary>
				/// <param name="element">The new graphics element to set.</param>
				void										SetOwnedElement(Ptr<elements::IGuiGraphicsElement> element);
				/// <summary>Get the visibility of the composition.</summary>
				/// <returns>Returns true if the composition is visible.</returns>
				bool										GetVisible();
				/// <summary>Set the visibility of the composition.</summary>
				/// <param name="value">Set to true to make the composition visible.</param>
				void										SetVisible(bool value);
				/// <summary>Get the minimum size limitation of the composition.</summary>
				/// <returns>The minimum size limitation of the composition.</returns>
				MinSizeLimitation							GetMinSizeLimitation();
				/// <summary>Set the minimum size limitation of the composition.</summary>
				/// <param name="value">The minimum size limitation of the composition.</param>
				void										SetMinSizeLimitation(MinSizeLimitation value);
				/// <summary>Get the binded render target.</summary>
				/// <returns>The binded render target.</returns>
				elements::IGuiGraphicsRenderTarget*			GetRenderTarget();
				/// <summary>Set the binded render target. This function is designed for internal usage. Users are not suggested to call this function directly.</summary>
				/// <param name="value">The binded render target.</param>
				void										SetRenderTarget(elements::IGuiGraphicsRenderTarget* value);

				/// <summary>Render the composition using an offset.</summary>
				/// <param name="offset">The offset.</param>
				void										Render(Size offset);
				/// <summary>Get the event receiver object. All user input events can be found in this object. If an event receiver is never been requested from the composition, the event receiver will not be created, and all route events will not pass through this event receiver(performance will be better).</summary>
				/// <returns>The event receiver.</returns>
				compositions::GuiGraphicsEventReceiver*		GetEventReceiver();
				/// <summary>Test if any event receiver has already been requested.</summary>
				/// <returns>Returns true if any event receiver has already been requested.</returns>
				bool										HasEventReceiver();
				/// <summary>Find a deepest composition that under a specified location. If the location is inside a compsition but not hit any sub composition, this function will return this composition.</summary>
				/// <returns>The deepest composition that under a specified location.</returns>
				/// <param name="location">The specified location.</param>
				GuiGraphicsComposition*						FindComposition(Point location);
				/// <summary>Get the bounds in the top composition space.</summary>
				/// <returns>The bounds in the top composition space.</returns>
				Rect										GetGlobalBounds();

				/// <summary>Get the associated control. A control is associated to a composition only when the composition represents the bounds of this control. Such a composition usually comes from a control template.</summary>
				/// <returns>The associated control.</returns>
				controls::GuiControl*						GetAssociatedControl();
				/// <summary>Get the associated graphics host. A graphics host is associated to a composition only when the composition becomes the bounds of the graphics host.</summary>
				/// <returns>The associated graphics host.</returns>
				GuiGraphicsHost*							GetAssociatedHost();
				/// <summary>Get the associated cursor.</summary>
				/// <returns>The associated cursor.</returns>
				INativeCursor*								GetAssociatedCursor();
				/// <summary>Set the associated cursor.</summary>
				/// <param name="cursor">The associated cursor.</param>
				void										SetAssociatedCursor(INativeCursor* cursor);
				
				/// <summary>Get the related control. A related control is the deepest control that contains this composition.</summary>
				/// <returns>The related control.</returns>
				controls::GuiControl*						GetRelatedControl();
				/// <summary>Get the related graphics host. A related graphics host is the graphics host that contains this composition.</summary>
				/// <returns>The related graphics host.</returns>
				GuiGraphicsHost*							GetRelatedGraphicsHost();
				/// <summary>Get the related control host. A related control host is the control host that contains this composition.</summary>
				/// <returns>The related control host.</returns>
				controls::GuiControlHost*					GetRelatedControlHost();
				/// <summary>Get the related cursor. A related cursor is from the deepest composition that contains this composition and associated with a cursor.</summary>
				/// <returns>The related cursor.</returns>
				INativeCursor*								GetRelatedCursor();
				
				/// <summary>Get the margin.</summary>
				/// <returns>The margin.</returns>
				virtual Margin								GetMargin();
				/// <summary>Set the margin.</summary>
				/// <param name="value">The margin.</param>
				virtual void								SetMargin(Margin value);
				/// <summary>Get the internal margin.</summary>
				/// <returns>The internal margin.</returns>
				virtual Margin								GetInternalMargin();
				/// <summary>Set the internal margin.</summary>
				/// <param name="value">The internal margin.</param>
				virtual void								SetInternalMargin(Margin value);
				/// <summary>Get the preferred minimum size.</summary>
				/// <returns>The preferred minimum size.</returns>
				virtual Size								GetPreferredMinSize();
				/// <summary>Set the preferred minimum size.</summary>
				/// <param name="value">The preferred minimum size.</param>
				virtual void								SetPreferredMinSize(Size value);
				/// <summary>Get the client area.</summary>
				/// <returns>The client area.</returns>
				virtual Rect								GetClientArea();
				/// <summary>Force to calculate layout and size immediately</summary>
				virtual void								ForceCalculateSizeImmediately();
				
				/// <summary>Get the parent size affection.</summary>
				/// <returns>The parent size affection.</returns>
				virtual ParentSizeAffection					GetAffectionFromParent()=0;
				/// <summary>Test is the size calculation affected by the parent.</summary>
				/// <returns>Returns true if the size calculation is affected by the parent.</returns>
				virtual bool								IsSizeAffectParent()=0;
				/// <summary>Get the preferred minimum client size.</summary>
				/// <returns>The preferred minimum client size.</returns>
				virtual Size								GetMinPreferredClientSize()=0;
				/// <summary>Get the preferred bounds.</summary>
				/// <returns>The preferred bounds.</returns>
				virtual Rect								GetPreferredBounds()=0;
				/// <summary>Get the bounds.</summary>
				/// <returns>The bounds.</returns>
				virtual Rect								GetBounds()=0;
			};

			/// <summary>
			/// A general implementation for <see cref="GuiGraphicsComposition"/>.
			/// </summary>
			class GuiGraphicsSite : public GuiGraphicsComposition, public Description<GuiGraphicsSite>
			{
			protected:

				/// <summary>Calculate the final bounds from an expected bounds.</summary>
				/// <returns>The final bounds according to some configuration like margin, minimum size, etc..</returns>
				/// <param name="expectedBounds">The expected bounds.</param>
				virtual Rect						GetBoundsInternal(Rect expectedBounds);
			public:
				GuiGraphicsSite();
				~GuiGraphicsSite();
				
				ParentSizeAffection					GetAffectionFromParent()override;
				bool								IsSizeAffectParent()override;
				Size								GetMinPreferredClientSize()override;
				Rect								GetPreferredBounds()override;
			};

/***********************************************************************
Basic Compositions
***********************************************************************/
			
			/// <summary>
			/// Represents a composition for the client area in an <see cref="INativeWindow"/>.
			/// </summary>
			class GuiWindowComposition : public GuiGraphicsSite, public Description<GuiWindowComposition>
			{
			protected:
				INativeWindow*						attachedWindow;
			public:
				GuiWindowComposition();
				~GuiWindowComposition();
				
				/// <summary>Get the attached native window object.</summary>
				/// <returns>The attached native window object.</returns>
				INativeWindow*						GetAttachedWindow();
				/// <summary>Attached a native window object.</summary>
				/// <param name="window">The native window object to attach.</param>
				void								SetAttachedWindow(INativeWindow* window);

				Rect								GetBounds()override;
				void								SetMargin(Margin value)override;
			};

			/// <summary>
			/// Represents a composition that is free to change the expected bounds.
			/// </summary>
			class GuiBoundsComposition : public GuiGraphicsSite, public Description<GuiBoundsComposition>
			{
			protected:
				Rect								compositionBounds;
				Rect								previousBounds;
				Margin								alignmentToParent;
				
			public:
				GuiBoundsComposition();
				~GuiBoundsComposition();

				/// <summary>Event that will be raised when the final bounds is changed.</summary>
				compositions::GuiNotifyEvent		BoundsChanged;
				
				ParentSizeAffection					GetAffectionFromParent()override;
				Rect								GetPreferredBounds()override;
				Rect								GetBounds()override;
				/// <summary>Set the expected bounds.</summary>
				/// <param name="value">The expected bounds.</param>
				void								SetBounds(Rect value);

				/// <summary>Make the composition not aligned to its parent.</summary>
				void								ClearAlignmentToParent();
				/// <summary>Get the alignment to its parent. -1 in each alignment component means that the corressponding side is not aligned to its parent.</summary>
				/// <returns>The alignment to its parent.</returns>
				Margin								GetAlignmentToParent();
				/// <summary>Set the alignment to its parent. -1 in each alignment component means that the corressponding side is not aligned to its parent.</summary>
				/// <param name="value">The alignment to its parent.</param>
				void								SetAlignmentToParent(Margin value);
				/// <summary>Test is the composition aligned to its parent.</summary>
				/// <returns>Returns true if the composition is aligned to its parent.</returns>
				bool								IsAlignedToParent();
			};

/***********************************************************************
Table Compositions
***********************************************************************/

			class GuiTableComposition;
			class GuiCellComposition;

			/// <summary>
			/// Represnets a sizing configuration for a row or a column.
			/// </summary>
			struct GuiCellOption
			{
				/// <summary>Sizing algorithm</summary>
				enum ComposeType
				{
					/// <summary>[T:vl.presentation.compositions.GuiCellOption.ComposeType]Set the size to an absolute value.</summary>
					Absolute,
					/// <summary>[T:vl.presentation.compositions.GuiCellOption.ComposeType]Set the size to a percentage number of the whole table.</summary>
					Percentage,
					/// <summary>[T:vl.presentation.compositions.GuiCellOption.ComposeType]Set the size to the minimum size of the cell element.</summary>
					MinSize,
				};

				/// <summary>Sizing algorithm</summary>
				ComposeType		composeType;
				/// <summary>The absolute size when <see cref="GuiCellOption::composeType"/> is <see cref="ComposeType"/>::Absolute.</summary>
				int				absolute;
				/// <summary>The percentage number when <see cref="GuiCellOption::composeType"/> is <see cref="ComposeType"/>::Percentage.</summary>
				double			percentage;

				GuiCellOption()
					:composeType(Absolute)
					,absolute(20)
					,percentage(0)
				{
				}

				bool operator==(const GuiCellOption& value){return false;}
				bool operator!=(const GuiCellOption& value){return true;}

				/// <summary>Creates an absolute sizing option</summary>
				/// <returns>The created option.</returns>
				/// <param name="value">The absolute size.</param>
				static GuiCellOption AbsoluteOption(int value)
				{
					GuiCellOption option;
					option.composeType=Absolute;
					option.absolute=value;
					return option;
				}
				
				/// <summary>Creates an percantage sizing option</summary>
				/// <returns>The created option.</returns>
				/// <param name="value">The percentage number.</param>
				static GuiCellOption PercentageOption(double value)
				{
					GuiCellOption option;
					option.composeType=Percentage;
					option.percentage=value;
					return option;
				}
				
				/// <summary>Creates an minimum sizing option</summary>
				/// <returns>The created option.</returns>
				static GuiCellOption MinSizeOption()
				{
					GuiCellOption option;
					option.composeType=MinSize;
					return option;
				}
			};

			/// <summary>
			/// Represents a table composition.
			/// </summary>
			class GuiTableComposition : public GuiBoundsComposition, public Description<GuiTableComposition>
			{
				friend class GuiCellComposition;
			protected:
				int											rows;
				int											columns;
				int											cellPadding;
				int											rowExtending;
				int											columnExtending;
				collections::Array<GuiCellOption>			rowOptions;
				collections::Array<GuiCellOption>			columnOptions;
				collections::Array<GuiCellComposition*>		cellCompositions;
				collections::Array<Rect>					cellBounds;
				Rect										previousBounds;
				Size										previousContentMinSize;
				Size										tableContentMinSize;

				int									GetSiteIndex(int _rows, int _columns, int _row, int _column);
				void								SetSitedCell(int _row, int _column, GuiCellComposition* cell);

				void								UpdateCellBoundsInternal(
														collections::Array<int>& dimSizes,
														int& dimSize, 
														int& dimSizeWithPercentage,
														collections::Array<GuiCellOption>& dimOptions,
														int GuiTableComposition::* dim1,
														int GuiTableComposition::* dim2,
														int (*getSize)(Size),
														int (*getLocation)(GuiCellComposition*),
														int (*getSpan)(GuiCellComposition*),
														int (*getRow)(int, int),
														int (*getCol)(int, int),
														int maxPass
														);
				void								UpdateCellBoundsPercentages(
														collections::Array<int>& dimSizes,
														int dimSize,
														int maxDimSize,
														collections::Array<GuiCellOption>& dimOptions
														);
				int									UpdateCellBoundsOffsets(
														collections::Array<int>& offsets,
														collections::Array<int>& sizes,
														int start,
														int max
														);
				
				void								UpdateCellBoundsInternal();
				void								UpdateTableContentMinSize();
				void								OnRenderTargetChanged()override;
			public:
				GuiTableComposition();
				~GuiTableComposition();

				/// <summary>Get the number of rows.</summary>
				/// <returns>The number of rows.</returns>
				int									GetRows();
				/// <summary>Get the number of columns.</summary>
				/// <returns>The number of columns.</returns>
				int									GetColumns();
				/// <summary>Change the number of rows and columns.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="_rows">The number of rows.</param>
				/// <param name="_columns">The number of columns.</param>
				bool								SetRowsAndColumns(int _rows, int _columns);
				/// <summary>Get the cell composition that covers the specified cell location.</summary>
				/// <returns>The cell composition that covers the specified cell location.</returns>
				/// <param name="_rows">The number of rows.</param>
				/// <param name="_columns">The number of columns.</param>
				GuiCellComposition*					GetSitedCell(int _row, int _column);

				/// <summary>Get the sizing option of the specified row.</summary>
				/// <returns>The sizing option of the specified row.</returns>
				/// <param name="_rows">The specified row number.</param>
				GuiCellOption						GetRowOption(int _row);
				/// <summary>Set the sizing option of the specified row.</summary>
				/// <param name="_rows">The specified row number.</param>
				/// <param name="option">The sizing option of the specified row.</param>
				void								SetRowOption(int _row, GuiCellOption option);
				/// <summary>Get the sizing option of the specified column.</summary>
				/// <returns>The sizing option of the specified column.</returns>
				/// <param name="_column">The specified column number.</param>
				GuiCellOption						GetColumnOption(int _column);
				/// <summary>Set the sizing option of the specified column.</summary>
				/// <param name="_column">The specified column number.</param>
				/// <param name="option">The sizing option of the specified column.</param>
				void								SetColumnOption(int _column, GuiCellOption option);

				/// <summary>Get the cell padding. A cell padding is the distance between a table client area and a cell, or between two cells.</summary>
				/// <returns>The cell padding.</returns>
				int									GetCellPadding();
				/// <summary>Set the cell padding. A cell padding is the distance between a table client area and a cell, or between two cells.</summary>
				/// <param name="_column">The cell padding.</param>
				void								SetCellPadding(int value);
				/// <summary>Get the cell area in the space of the table's parent composition's client area.</summary>
				/// <returns>The cell area.</returns>
				Rect								GetCellArea();
				/// <summary>Update the sizing of the table and cells after all rows' and columns' sizing options are prepared.</summary>
				void								UpdateCellBounds();
				
				void								ForceCalculateSizeImmediately()override;
				Size								GetMinPreferredClientSize()override;
				Rect								GetBounds()override;
			};

			/// <summary>
			/// Represents a cell composition of a <see cref="GuiTableComposition"/>.
			/// </summary>
			class GuiCellComposition : public GuiGraphicsSite, public Description<GuiCellComposition>
			{
				friend class GuiTableComposition;
			protected:
				int									row;
				int									rowSpan;
				int									column;
				int									columnSpan;
				GuiTableComposition*				tableParent;
				
				void								ClearSitedCells(GuiTableComposition* table);
				void								SetSitedCells(GuiTableComposition* table);
				void								ResetSiteInternal();
				bool								SetSiteInternal(int _row, int _column, int _rowSpan, int _columnSpan);
				void								OnParentChanged(GuiGraphicsComposition* oldParent, GuiGraphicsComposition* newParent)override;
				void								OnTableRowsAndColumnsChanged();
			public:
				GuiCellComposition();
				~GuiCellComposition();

				/// <summary>Get the owner table composition.</summary>
				/// <returns>The owner table composition.</returns>
				GuiTableComposition*				GetTableParent();

				/// <summary>Get the row number for this cell composition.</summary>
				/// <returns>The row number for this cell composition.</returns>
				int									GetRow();
				/// <summary>Get the total numbers of acrossed rows for this cell composition.</summary>
				/// <returns>The total numbers of acrossed rows for this cell composition.</returns>
				int									GetRowSpan();
				/// <summary>Get the column number for this cell composition.</summary>
				/// <returns>The column number for this cell composition.</returns>
				int									GetColumn();
				/// <summary>Get the total numbers of acrossed columns for this cell composition.</summary>
				/// <returns>The total numbers of acrossed columns for this cell composition.</returns>
				int									GetColumnSpan();
				/// <summary>Set the position for this cell composition in the table.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="_row">The row number for this cell composition.</param>
				/// <param name="_column">The column number for this cell composition.</param>
				/// <param name="_rowSpan">The total numbers of acrossed rows for this cell composition.</param>
				/// <param name="_columnSpan">The total numbers of acrossed columns for this cell composition.</param>
				bool								SetSite(int _row, int _column, int _rowSpan, int _columnSpan);

				Rect								GetBounds()override;
			};

/***********************************************************************
Stack Compositions
***********************************************************************/

			class GuiStackComposition;
			class GuiStackItemComposition;

			/// <summary>
			/// Represents a stack composition.
			/// </summary>
			class GuiStackComposition : public GuiBoundsComposition, public Description<GuiStackComposition>
			{
				friend class GuiStackItemComposition;

				typedef collections::List<GuiStackItemComposition*>				ItemCompositionList;
				typedef collections::IReadonlyList<GuiStackItemComposition*>	IItemCompositionList;
			public:
				/// <summary>Stack item layout direction.</summary>
				enum Direction
				{
					/// <summary>[T:vl.presentation.compositions.GuiStackComposition.Direction]Stack items is layouted horizontally.</summary>
					Horizontal,
					/// <summary>[T:vl.presentation.compositions.GuiStackComposition.Direction]Stack items is layouted vertically.</summary>
					Vertical,
				};
			protected:
				Direction							direction;
				ItemCompositionList					stackItems;
				collections::Array<Rect>			stackItemBounds;
				Size								stackItemTotalSize;
				int									padding;
				Rect								previousBounds;
				Margin								extraMargin;

				void								UpdateStackItemBounds();
				void								FixStackItemSizes();
				void								OnChildInserted(GuiGraphicsComposition* child)override;
				void								OnChildRemoved(GuiGraphicsComposition* child)override;
			public:
				GuiStackComposition();
				~GuiStackComposition();

				/// <summary>Get all stack items inside the stack composition.</summary>
				/// <returns>All stack items inside the stack composition.</returns>
				const IItemCompositionList&			GetStackItems();
				/// <summary>Insert an stack item at a specified position.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="index">The position.</param>
				/// <param name="item">The statck item to insert.</param>
				bool								InsertStackItem(int index, GuiStackItemComposition* item);
				
				/// <summary>Get the stack item layout direction.</summary>
				/// <returns>The stack item layout direction.</returns>
				Direction							GetDirection();
				/// <summary>Set the stack item layout direction.</summary>
				/// <param name="value">The stack item layout direction.</param>
				void								SetDirection(Direction value);
				/// <summary>Get the stack item padding.</summary>
				/// <returns>The stack item padding.</returns>
				int									GetPadding();
				/// <summary>Set the stack item padding.</summary>
				/// <param name="value">The stack item padding.</param>
				void								SetPadding(int value);

				Size								GetMinPreferredClientSize()override;
				Rect								GetBounds()override;
				
				/// <summary>Get the extra margin inside the stack composition.</summary>
				/// <returns>The extra margin inside the stack composition.</returns>
				Margin								GetExtraMargin();
				/// <summary>Set the extra margin inside the stack composition.</summary>
				/// <param name="value">The extra margin inside the stack composition.</param>
				void								SetExtraMargin(Margin value);
			};
			
			/// <summary>
			/// Represents a stack item composition of a <see cref="GuiStackComposition"/>.
			/// </summary>
			class GuiStackItemComposition : public GuiGraphicsSite, public Description<GuiStackItemComposition>
			{
				friend class GuiStackComposition;
			protected:
				GuiStackComposition*				stackParent;
				Rect								bounds;
				Margin								extraMargin;

				void								OnParentChanged(GuiGraphicsComposition* oldParent, GuiGraphicsComposition* newParent)override;
				Size								GetMinSize();
			public:
				GuiStackItemComposition();
				~GuiStackItemComposition();
				
				ParentSizeAffection					GetAffectionFromParent()override;
				bool								IsSizeAffectParent()override;
				Rect								GetBounds()override;
				/// <summary>Set the expected bounds of a stack item. In most of the cases only the size of the bounds is used.</summary>
				/// <param name="value">The expected bounds of a stack item.</param>
				void								SetBounds(Rect value);
				
				/// <summary>Get the extra margin for this stack item. An extra margin is used to enlarge the bounds of the stack item, but only the non-extra part will be used for decide the stack item layout.</summary>
				/// <returns>The extra margin for this stack item.</returns>
				Margin								GetExtraMargin();
				/// <summary>Set the extra margin for this stack item. An extra margin is used to enlarge the bounds of the stack item, but only the non-extra part will be used for decide the stack item layout.</summary>
				/// <param name="value">The extra margin for this stack item.</param>
				void								SetExtraMargin(Margin value);
			};

/***********************************************************************
Specialized Compositions
***********************************************************************/

			/// <summary>
			/// Represents a composition that is aligned to one border of the parent composition.
			/// </summary>
			class GuiSideAlignedComposition : public GuiGraphicsSite, public Description<GuiSideAlignedComposition>
			{
			public:
				/// <summary>The border to align.</summary>
				enum Direction
				{
					/// <summary>[T:vl.presentation.compositions.GuiSideAlignedComposition.Direction]The left border.</summary>
					Left,
					/// <summary>[T:vl.presentation.compositions.GuiSideAlignedComposition.Direction]The top border.</summary>
					Top,
					/// <summary>[T:vl.presentation.compositions.GuiSideAlignedComposition.Direction]The right border.</summary>
					Right,
					/// <summary>[T:vl.presentation.compositions.GuiSideAlignedComposition.Direction]The bottom border.</summary>
					Bottom,
				};
			protected:
				Direction							direction;
				int									maxLength;
				double								maxRatio;
			public:
				GuiSideAlignedComposition();
				~GuiSideAlignedComposition();
				
				/// <summary>Get the border to align.</summary>
				/// <returns>The border to align.</returns>
				Direction							GetDirection();
				/// <summary>Set the border to align.</summary>
				/// <param name="value">The border to align.</param>
				void								SetDirection(Direction value);
				/// <summary>Get the maximum length of this composition.</summary>
				/// <returns>The maximum length of this composition.</returns>
				int									GetMaxLength();
				/// <summary>Set the maximum length of this composition.</summary>
				/// <param name="value">The maximum length of this composition.</param>
				void								SetMaxLength(int value);
				/// <summary>Get the maximum ratio to limit the size according to the size of the parent.</summary>
				/// <returns>The maximum ratio to limit the size according to the size of the parent.</returns>
				double								GetMaxRatio();
				/// <summary>Set the maximum ratio to limit the size according to the size of the parent.</summary>
				/// <param name="value">The maximum ratio to limit the size according to the size of the parent.</param>
				void								SetMaxRatio(double value);
				
				ParentSizeAffection					GetAffectionFromParent()override;
				bool								IsSizeAffectParent()override;
				Rect								GetBounds()override;
			};

			/// <summary>
			/// Represents a composition that its location and size are decided by the client area of the parent composition by setting ratios.
			/// </summary>
			class GuiPartialViewComposition : public GuiGraphicsSite, public Description<GuiPartialViewComposition>
			{
			protected:
				double								wRatio;
				double								wPageSize;
				double								hRatio;
				double								hPageSize;

			public:
				GuiPartialViewComposition();
				~GuiPartialViewComposition();
				
				/// <summary>Get the width ratio to decided the horizontal location. Value in [0, 1-pageSize].</summary>
				/// <returns>The width ratio to decided the horizontal location.</returns>
				double								GetWidthRatio();
				/// <summary>Get the page size to decide the horizontal size. Value in [0, 1].</summary>
				/// <returns>The page size to decide the horizontal size.</returns>
				double								GetWidthPageSize();
				/// <summary>Get the height ratio to decided the vertical location. Value in [0, 1-pageSize].</summary>
				/// <returns>The height ratio to decided the vertical location.</returns>
				double								GetHeightRatio();
				/// <summary>Get the page size to decide the vertical size. Value in [0, 1].</summary>
				/// <returns>The page size to decide the vertical size.</returns>
				double								GetHeightPageSize();
				/// <summary>Set the width ratio to decided the horizontal location. Value in [0, 1-pageSize].</summary>
				/// <param name="value">The width ratio to decided the horizontal location.</param>
				void								SetWidthRatio(double value);
				/// <summary>Set the page size to decide the horizontal size. Value in [0, 1].</summary>
				/// <param name="value">The page size to decide the horizontal size.</param>
				void								SetWidthPageSize(double value);
				/// <summary>Set the height ratio to decided the vertical location. Value in [0, 1-pageSize].</summary>
				/// <param name="value">The height ratio to decided the vertical location.</param>
				void								SetHeightRatio(double value);
				/// <summary>Set the page size to decide the vertical size. Value in [0, 1].</summary>
				/// <param name="value">The page size to decide the vertical size.</param>
				void								SetHeightPageSize(double value);
				
				ParentSizeAffection					GetAffectionFromParent()override;
				bool								IsSizeAffectParent()override;
				Rect								GetBounds()override;
			};
		}
	}
}

#endif