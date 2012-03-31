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
			class IGuiStyleController;
			class GuiControl;
			class GuiControlHost;
		}

		namespace elements
		{
			class GuiGraphicsHost;

/***********************************************************************
Basic Construction
***********************************************************************/

			/// <summary>
			/// Represents a composition for <see cref="IGuiGraphicsElement"/>. A composition is a way to define the size and the position using the information from graphics elements and sub compositions.
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
					/// <summary>[T:vl.presentation.elements.GuiGraphicsComposition.MinSizeLimitation]No limitation for the minimum size.</summary>
					NoLimit,
					/// <summary>[T:vl.presentation.elements.GuiGraphicsComposition.MinSizeLimitation]Minimum size of this composition is the minimum size of the contained graphics element.</summary>
					LimitToElement,
					/// <summary>[T:vl.presentation.elements.GuiGraphicsComposition.MinSizeLimitation]Minimum size of this composition is combiniation of sub compositions and the minimum size of the contained graphics element.</summary>
					LimitToElementAndChildren,
				};
				
				/// <summary>
				/// Relation between the composition size and the parent composition.
				/// </summary>
				enum ParentSizeAffection
				{
					/// <summary>[T:vl.presentation.elements.GuiGraphicsComposition.ParentSizeAffection]The size doesn't aware of the parent composition.</summary>
					NotAffectedByParent,
					/// <summary>[T:vl.presentation.elements.GuiGraphicsComposition.ParentSizeAffection]The size partially limited by the parent composition.</summary>
					AffectedByParent,
					/// <summary>[T:vl.presentation.elements.GuiGraphicsComposition.ParentSizeAffection]The size is decided by the parent composition.</summary>
					TotallyDecidedByParent,
				};
			protected:
				CompositionList						children;
				GuiGraphicsComposition*				parent;
				Ptr<IGuiGraphicsElement>			ownedElement;
				bool								visible;
				IGuiGraphicsRenderTarget*			renderTarget;
				MinSizeLimitation					minSizeLimitation;

				Ptr<GuiGraphicsEventReceiver>		eventReceiver;
				controls::GuiControl*				associatedControl;
				GuiGraphicsHost*					associatedHost;
				INativeCursor*						associatedCursor;

				Margin								margin;
				Margin								internalMargin;
				Size								preferredMinSize;

				virtual void						OnControlParentChanged(controls::GuiControl* control);
				virtual void						OnChildInserted(GuiGraphicsComposition* child);
				virtual void						OnChildRemoved(GuiGraphicsComposition* child);
				virtual void						OnParentChanged(GuiGraphicsComposition* oldParent, GuiGraphicsComposition* newParent);
				virtual void						OnRenderTargetChanged();
				
				virtual void						SetAssociatedControl(controls::GuiControl* control);
				virtual void						SetAssociatedHost(GuiGraphicsHost* host);
			public:
				GuiGraphicsComposition();
				~GuiGraphicsComposition();

				/// <summary>Get the parent composition.</summary>
				/// <returns>The parent composition.</returns>
				GuiGraphicsComposition*				GetParent();
				/// <summary>Get all child compositions ordered by z-order from low to high.</summary>
				/// <returns>Child compositions.</returns>
				const ICompositionList&				Children();
				/// <summary>Add a composition as a child.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="child">The child composition to add.</param>
				bool								AddChild(GuiGraphicsComposition* child);
				/// <summary>Add a composition as a child with a specified z-order.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="index">The z-order. 0 means the lowest position.</param>
				/// <param name="child">The child composition to add.</param>
				bool								InsertChild(int index, GuiGraphicsComposition* child);
				/// <summary>Remove a child composition.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="child">The child composition to remove.</param>
				bool								RemoveChild(GuiGraphicsComposition* child);
				/// <summary>Move a child composition to a new z-order.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="child">The child composition to move.</param>
				/// <param name="newIndex">The new z-order. 0 means the lowest position.</param>
				bool								MoveChild(GuiGraphicsComposition* child, int newIndex);

				/// <summary>Get the contained graphics element.</summary>
				/// <returns>The contained graphics element.</returns>
				Ptr<IGuiGraphicsElement>			GetOwnedElement();
				/// <summary>Set the contained graphics element.</summary>
				/// <param name="element">The new graphics element to set.</param>
				void								SetOwnedElement(Ptr<IGuiGraphicsElement> element);
				/// <summary>Get the visibility of the composition.</summary>
				/// <returns>Returns true if the composition is visible.</returns>
				bool								GetVisible();
				/// <summary>Set the visibility of the composition.</summary>
				/// <param name="value">Set to true to make the composition visible.</param>
				void								SetVisible(bool value);
				/// <summary>Get the minimum size limitation of the composition.</summary>
				/// <returns>The minimum size limitation of the composition.</returns>
				MinSizeLimitation					GetMinSizeLimitation();
				/// <summary>Set the minimum size limitation of the composition.</summary>
				/// <param name="value">The minimum size limitation of the composition.</param>
				void								SetMinSizeLimitation(MinSizeLimitation value);
				/// <summary>Get the binded render target.</summary>
				/// <returns>The binded render target.</returns>
				IGuiGraphicsRenderTarget*			GetRenderTarget();
				/// <summary>Set the binded render target. This function is designed for internal usage. Users are not suggested to call this function directly.</summary>
				/// <param name="value">The binded render target.</param>
				void								SetRenderTarget(IGuiGraphicsRenderTarget* value);

				/// <summary>Render the composition using an offset.</summary>
				/// <param name="offset">The offset.</param>
				void								Render(Size offset);
				/// <summary>Get the event receiver object. All user input events can be found in this object. If an event receiver is never been requested from the composition, the event receiver will not be created, and all route events will not pass through this event receiver(performance will be better).</summary>
				/// <returns>The event receiver.</returns>
				GuiGraphicsEventReceiver*			GetEventReceiver();
				/// <summary>Test if any event receiver has already been requested.</summary>
				/// <returns>Returns true if any event receiver has already been requested.</returns>
				bool								HasEventReceiver();
				/// <summary>Find a deepest composition that under a specified location. If the location is inside a compsition but not hit any sub composition, this function will return this composition.</summary>
				/// <returns>The deepest composition that under a specified location.</returns>
				/// <param name="location">The specified location.</param>
				GuiGraphicsComposition*				FindComposition(Point location);
				/// <summary>Get the bounds in the top composition space.</summary>
				/// <returns>The bounds in the top composition space.</returns>
				Rect								GetGlobalBounds();

				/// <summary>Get the associated control. A control is associated to a composition only when the composition represents the bounds of this control. Such a composition usually comes from a control template.</summary>
				/// <returns>The associated control.</returns>
				controls::GuiControl*				GetAssociatedControl();
				/// <summary>Get the associated graphics host. A graphics host is associated to a composition only when the composition becomes the bounds of the graphics host.</summary>
				/// <returns>The associated graphics host.</returns>
				GuiGraphicsHost*					GetAssociatedHost();
				/// <summary>Get the associated cursor.</summary>
				/// <returns>The associated cursor.</returns>
				INativeCursor*						GetAssociatedCursor();
				/// <summary>Set the associated cursor.</summary>
				/// <param name="cursor">The associated cursor.</param>
				void								SetAssociatedCursor(INativeCursor* cursor);
				
				/// <summary>Get the related control. A related control is the deepest control that contains this composition.</summary>
				/// <returns>The related control.</returns>
				controls::GuiControl*				GetRelatedControl();
				/// <summary>Get the related graphics host. A related graphics host is the graphics host that contains this composition.</summary>
				/// <returns>The related graphics host.</returns>
				GuiGraphicsHost*					GetRelatedGraphicsHost();
				/// <summary>Get the related control host. A related control host is the control host that contains this composition.</summary>
				/// <returns>The related control host.</returns>
				controls::GuiControlHost*			GetRelatedControlHost();
				/// <summary>Get the related cursor. A related cursor is from the deepest composition that contains this composition and associated with a cursor.</summary>
				/// <returns>The related cursor.</returns>
				INativeCursor*						GetRelatedCursor();
				
				/// <summary>Get the margin.</summary>
				/// <returns>The margin.</returns>
				virtual Margin						GetMargin();
				/// <summary>Set the margin.</summary>
				/// <param name="value">The margin.</param>
				virtual void						SetMargin(Margin value);
				/// <summary>Get the internal margin.</summary>
				/// <returns>The internal margin.</returns>
				virtual Margin						GetInternalMargin();
				/// <summary>Set the internal margin.</summary>
				/// <param name="value">The internal margin.</param>
				virtual void						SetInternalMargin(Margin value);
				/// <summary>Get the preferred minimum size.</summary>
				/// <returns>The preferred minimum size.</returns>
				virtual Size						GetPreferredMinSize();
				/// <summary>Set the preferred minimum size.</summary>
				/// <param name="value">The preferred minimum size.</param>
				virtual void						SetPreferredMinSize(Size value);
				/// <summary>Get the client area.</summary>
				/// <returns>The client area.</returns>
				virtual Rect						GetClientArea();
				
				/// <summary>Get the parent size affection.</summary>
				/// <returns>The parent size affection.</returns>
				virtual ParentSizeAffection			GetAffectionFromParent()=0;
				/// <summary>Test is the size calculation affected by the parent.</summary>
				/// <returns>Returns true if the size calculation is affected by the parent.</returns>
				virtual bool						IsSizeAffectParent()=0;
				/// <summary>Get the preferred minimum client size.</summary>
				/// <returns>The preferred minimum client size.</returns>
				virtual Size						GetMinPreferredClientSize()=0;
				/// <summary>Get the preferred bounds.</summary>
				/// <returns>The preferred bounds.</returns>
				virtual Rect						GetPreferredBounds()=0;
				/// <summary>Get the bounds.</summary>
				/// <returns>The bounds.</returns>
				virtual Rect						GetBounds()=0;
			};

			class GuiGraphicsSite : public GuiGraphicsComposition, public Description<GuiGraphicsSite>
			{
			protected:

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

			class GuiWindowComposition : public GuiGraphicsSite, public Description<GuiWindowComposition>
			{
			protected:
				INativeWindow*						attachedWindow;
			public:
				GuiWindowComposition();
				~GuiWindowComposition();

				INativeWindow*						GetAttachedWindow();
				void								SetAttachedWindow(INativeWindow* window);

				Rect								GetBounds()override;
				void								SetMargin(Margin value)override;
			};

			class GuiBoundsComposition : public GuiGraphicsSite, public Description<GuiBoundsComposition>
			{
			protected:
				Rect								compositionBounds;
				Rect								previousBounds;
				Margin								alignmentToParent;
				
			public:
				GuiBoundsComposition();
				~GuiBoundsComposition();

				GuiNotifyEvent						BoundsChanged;
				
				ParentSizeAffection					GetAffectionFromParent()override;
				Rect								GetPreferredBounds()override;
				Rect								GetBounds()override;
				void								SetBounds(Rect value);

				void								ClearAlignmentToParent();
				Margin								GetAlignmentToParent();
				void								SetAlignmentToParent(Margin value);
				bool								IsAlignedToParent();
			};

/***********************************************************************
Table Compositions
***********************************************************************/

			class GuiTableComposition;
			class GuiCellComposition;

			struct GuiCellOption
			{
				enum ComposeType
				{
					Absolute,
					Percentage,
					MinSize,
				};

				ComposeType		composeType;
				int				absolute;
				double			percentage;

				GuiCellOption()
					:composeType(Absolute)
					,absolute(20)
					,percentage(0)
				{
				}

				bool operator==(const GuiCellOption& value){return false;}
				bool operator!=(const GuiCellOption& value){return true;}

				static GuiCellOption AbsoluteOption(int value)
				{
					GuiCellOption option;
					option.composeType=Absolute;
					option.absolute=value;
					return option;
				}

				static GuiCellOption PercentageOption(double value)
				{
					GuiCellOption option;
					option.composeType=Percentage;
					option.percentage=value;
					return option;
				}

				static GuiCellOption MinSizeOption()
				{
					GuiCellOption option;
					option.composeType=MinSize;
					return option;
				}
			};

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

				int									GetRows();
				int									GetColumns();
				bool								SetRowsAndColumns(int _rows, int _columns);
				GuiCellComposition*					GetSitedCell(int _row, int _column);

				GuiCellOption						GetRowOption(int _row);
				void								SetRowOption(int _row, GuiCellOption option);
				GuiCellOption						GetColumnOption(int _column);
				void								SetColumnOption(int _column, GuiCellOption option);

				int									GetCellPadding();
				void								SetCellPadding(int value);
				Rect								GetCellArea();
				void								UpdateCellBounds();
				
				Size								GetMinPreferredClientSize()override;
				Rect								GetBounds()override;
			};

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

				GuiTableComposition*				GetTableParent();

				int									GetRow();
				int									GetRowSpan();
				int									GetColumn();
				int									GetColumnSpan();
				bool								SetSite(int _row, int _column, int _rowSpan, int _columnSpan);

				Rect								GetBounds()override;
			};

/***********************************************************************
Stack Compositions
***********************************************************************/

			class GuiStackComposition;
			class GuiStackItemComposition;

			class GuiStackComposition : public GuiBoundsComposition, public Description<GuiStackComposition>
			{
				friend class GuiStackItemComposition;

				typedef collections::List<GuiStackItemComposition*>				ItemCompositionList;
				typedef collections::IReadonlyList<GuiStackItemComposition*>	IItemCompositionList;
			public:
				enum Direction
				{
					Horizontal,
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

				const IItemCompositionList&			GetStackItems();
				bool								InsertStackItem(int index, GuiStackItemComposition* item);

				Direction							GetDirection();
				void								SetDirection(Direction value);
				int									GetPadding();
				void								SetPadding(int value);

				Size								GetMinPreferredClientSize()override;
				Rect								GetBounds()override;

				Margin								GetExtraMargin();
				void								SetExtraMargin(Margin value);
			};

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
				void								SetBounds(Rect value);

				Margin								GetExtraMargin();
				void								SetExtraMargin(Margin value);
			};

/***********************************************************************
Specialized Compositions
***********************************************************************/

			class GuiSideAlignedComposition : public GuiGraphicsSite, public Description<GuiSideAlignedComposition>
			{
			public:
				enum Direction
				{
					Left,
					Top,
					Right,
					Bottom,
				};
			protected:
				Direction							direction;
				int									maxLength;
				double								maxRatio;
			public:
				GuiSideAlignedComposition();
				~GuiSideAlignedComposition();

				Direction							GetDirection();
				void								SetDirection(Direction value);
				int									GetMaxLength();
				void								SetMaxLength(int value);
				double								GetMaxRatio();
				void								SetMaxRatio(double value);
				
				ParentSizeAffection					GetAffectionFromParent()override;
				bool								IsSizeAffectParent()override;
				Rect								GetBounds()override;
			};

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

				double								GetWidthRatio();
				double								GetWidthPageSize();
				double								GetHeightRatio();
				double								GetHeightPageSize();
				void								SetWidthRatio(double value);
				void								SetWidthPageSize(double value);
				void								SetHeightRatio(double value);
				void								SetHeightPageSize(double value);
				
				ParentSizeAffection					GetAffectionFromParent()override;
				bool								IsSizeAffectParent()override;
				Rect								GetBounds()override;
			};
		}
	}
}

#endif