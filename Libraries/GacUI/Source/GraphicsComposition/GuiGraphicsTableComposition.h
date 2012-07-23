/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Composition System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSTABLECOMPOSITION
#define VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSTABLECOMPOSITION

#include "GuiGraphicsBasicComposition.h"

namespace vl
{
	namespace presentation
	{
		namespace compositions
		{

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
		}
	}
}

#endif