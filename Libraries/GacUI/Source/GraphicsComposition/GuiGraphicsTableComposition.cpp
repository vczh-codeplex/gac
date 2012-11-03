#include "GuiGraphicsTableComposition.h"
#include <math.h>

namespace vl
{
	namespace presentation
	{
		namespace compositions
		{
			using namespace collections;
			using namespace controls;
			using namespace elements;

/***********************************************************************
GuiTableComposition
***********************************************************************/

			namespace update_cell_bounds_helpers
			{
				int First(int a, int b)
				{
					return a;
				}

				int Second(int a, int b)
				{
					return b;
				}

				int X(Size s)
				{
					return s.x;
				}

				int Y(Size s)
				{
					return s.y;
				}

				int RL(GuiCellComposition* cell)
				{
					return cell->GetRow();
				}

				int CL(GuiCellComposition* cell)
				{
					return cell->GetColumn();
				}

				int RS(GuiCellComposition* cell)
				{
					return cell->GetRowSpan();
				}

				int CS(GuiCellComposition* cell)
				{
					return cell->GetColumnSpan();
				}
			}
			using namespace update_cell_bounds_helpers;

			int GuiTableComposition::GetSiteIndex(int _rows, int _columns, int _row, int _column)
			{
				return _row*_columns+_column;
			}

			void GuiTableComposition::SetSitedCell(int _row, int _column, GuiCellComposition* cell)
			{
				cellCompositions[GetSiteIndex(rows, columns, _row, _column)]=cell;
			}

			void GuiTableComposition::UpdateCellBoundsInternal(
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
				)
			{
				for(int pass=0;pass<maxPass;pass++)
				{
					for(int i=0;i<this->*dim1;i++)
					{
						GuiCellOption option=dimOptions[i];
						if(pass==0)
						{
							dimSizes[i]=0;
						}
						switch(option.composeType)
						{
						case GuiCellOption::Absolute:
							{
								dimSizes[i]=option.absolute;
							}
							break;
						case GuiCellOption::MinSize:
							{
								for(int j=0;j<this->*dim2;j++)
								{
									GuiCellComposition* cell=GetSitedCell(getRow(i, j), getCol(i, j));
									if(cell)
									{
										bool accept=false;
										if(pass==0)
										{
											accept=getSpan(cell)==1;
										}
										else
										{
											accept=getLocation(cell)+getSpan(cell)==i+1;
										}
										if(accept)
										{
											int size=getSize(cell->GetPreferredBounds().GetSize());
											int span=getSpan(cell);
											for(int k=1;k<span;k++)
											{
												size-=dimSizes[i-k]+cellPadding;
											}
											if(dimSizes[i]<size)
											{
												dimSizes[i]=size;
											}
										}
									}
								}
							}
							break;
						}
					}
				}
				
				bool percentageExists=false;
				for(int i=0;i<this->*dim1;i++)
				{
					GuiCellOption option=dimOptions[i];
					if(option.composeType==GuiCellOption::Percentage)
					{
						if(0.001<option.percentage)
						{
							percentageExists=true;
						}
					}
				}

				if(percentageExists)
				{
					for(int i=0;i<this->*dim1;i++)
					{
						GuiCellOption option=dimOptions[i];
						if(option.composeType==GuiCellOption::Percentage)
						{
							if(0.001<option.percentage)
							{
								for(int j=0;j<this->*dim2;j++)
								{
									GuiCellComposition* cell=GetSitedCell(getRow(i, j), getCol(i, j));
									if(cell)
									{
										int size=getSize(cell->GetPreferredBounds().GetSize());
										int start=getLocation(cell);
										int span=getSpan(cell);
										size-=(span-1)*cellPadding;
										double totalPercentage=0;

										for(int k=start;k<start+span;k++)
										{
											if(dimOptions[k].composeType==GuiCellOption::Percentage)
											{
												if(0.001<dimOptions[k].percentage)
												{
													totalPercentage+=dimOptions[k].percentage;
												}
											}
											else
											{
												size-=dimSizes[k];
											}
										}

										size=(int)ceil(size*option.percentage/totalPercentage);
										if(dimSizes[i]<size)
										{
											dimSizes[i]=size;
										}
									}
								}
							}
						}
					}

					int percentageTotalSize=0;
					for(int i=0;i<this->*dim1;i++)
					{
						GuiCellOption option=dimOptions[i];
						if(option.composeType==GuiCellOption::Percentage)
						{
							if(0.001<option.percentage)
							{
								int size=(int)ceil(dimSizes[i]/option.percentage);
								if(percentageTotalSize<size)
								{
									percentageTotalSize=size;
								}
							}
						}
					}

					double totalPercentage=0;
					for(int i=0;i<this->*dim1;i++)
					{
						GuiCellOption option=dimOptions[i];
						if(option.composeType==GuiCellOption::Percentage)
						{
							if(0.001<option.percentage)
							{
								totalPercentage+=option.percentage;
							}
						}
					}
					
					for(int i=0;i<this->*dim1;i++)
					{
						GuiCellOption option=dimOptions[i];
						if(option.composeType==GuiCellOption::Percentage)
						{
							if(0.001<option.percentage)
							{
								int size=(int)ceil(percentageTotalSize*option.percentage/totalPercentage);
								if(dimSizes[i]<size)
								{
									dimSizes[i]=size;
								}
							}
						}
					}
				}

				for(int i=0;i<this->*dim1;i++)
				{
					if(dimOptions[i].composeType!=GuiCellOption::Percentage)
					{
						dimSize+=dimSizes[i];
					}
					dimSizeWithPercentage+=dimSizes[i];
				}
			}

			void GuiTableComposition::UpdateCellBoundsPercentages(
				collections::Array<int>& dimSizes,
				int dimSize,
				int maxDimSize,
				collections::Array<GuiCellOption>& dimOptions
				)
			{
				if(maxDimSize>dimSize)
				{
					double totalPercentage=0;
					int percentageCount=0;
					for(int i=0;i<dimOptions.Count();i++)
					{
						GuiCellOption option=dimOptions[i];
						if(option.composeType==GuiCellOption::Percentage)
						{
							totalPercentage+=option.percentage;
							percentageCount++;
						}
					}
					if(percentageCount>0 && totalPercentage>0.001)
					{
						for(int i=0;i<dimOptions.Count();i++)
						{
							GuiCellOption option=dimOptions[i];
							if(option.composeType==GuiCellOption::Percentage)
							{
								dimSizes[i]=(int)((maxDimSize-dimSize)*option.percentage/totalPercentage);
							}
						}
					}
				}
			}

			int GuiTableComposition::UpdateCellBoundsOffsets(
				collections::Array<int>& offsets,
				collections::Array<int>& sizes,
				int start,
				int max
				)
			{
				offsets[0]=start;
				for(int i=1;i<offsets.Count();i++)
				{
					start+=cellPadding+sizes[i-1];
					offsets[i]=start;
				}

				int last=offsets.Count()-1;
				int right=offsets[last]+sizes[last];
				return max-right;
			}

			void GuiTableComposition::UpdateCellBoundsInternal()
			{
				Array<int> rowOffsets, columnOffsets, rowSizes, columnSizes;
				rowOffsets.Resize(rows);
				rowSizes.Resize(rows);
				columnOffsets.Resize(columns);
				columnSizes.Resize(columns);
				{
					int rowTotal=(rows-1)*cellPadding;
					int columnTotal=(columns-1)*cellPadding;
					int rowTotalWithPercentage=rowTotal;
					int columnTotalWithPercentage=columnTotal;

					UpdateCellBoundsInternal(
						rowSizes,
						rowTotal,
						rowTotalWithPercentage,
						rowOptions,
						&GuiTableComposition::rows,
						&GuiTableComposition::columns,
						&Y,
						&RL,
						&RS,
						&First,
						&Second,
						1
						);
					UpdateCellBoundsInternal(
						columnSizes,
						columnTotal,
						columnTotalWithPercentage,
						columnOptions,
						&GuiTableComposition::columns,
						&GuiTableComposition::rows,
						&X,
						&CL,
						&CS,
						&Second,
						&First,
						1
						);

					Rect area=GetCellArea();
					UpdateCellBoundsPercentages(rowSizes, rowTotal, area.Height(), rowOptions);
					UpdateCellBoundsPercentages(columnSizes, columnTotal, area.Width(), columnOptions);
					rowExtending=UpdateCellBoundsOffsets(rowOffsets, rowSizes, cellPadding, cellPadding+area.Height());
					columnExtending=UpdateCellBoundsOffsets(columnOffsets, columnSizes, cellPadding, cellPadding+area.Width());

					for(int i=0;i<rows;i++)
					{
						for(int j=0;j<columns;j++)
						{
							int index=GetSiteIndex(rows, columns, i, j);
							cellBounds[index]=Rect(Point(columnOffsets[j], rowOffsets[i]), Size(columnSizes[j], rowSizes[i]));
						}
					}
				}
			}

			void GuiTableComposition::UpdateTableContentMinSize()
			{
				Array<int> rowSizes, columnSizes;
				rowSizes.Resize(rows);
				columnSizes.Resize(columns);
				{
					int rowTotal=(rows+1)*cellPadding;
					int columnTotal=(columns+1)*cellPadding;
					int rowTotalWithPercentage=rowTotal;
					int columnTotalWithPercentage=columnTotal;

					UpdateCellBoundsInternal(
						rowSizes,
						rowTotal,
						rowTotalWithPercentage,
						rowOptions,
						&GuiTableComposition::rows,
						&GuiTableComposition::columns,
						&Y,
						&RL,
						&RS,
						&First,
						&Second,
						2
						);
					UpdateCellBoundsInternal(
						columnSizes,
						columnTotal,
						columnTotalWithPercentage,
						columnOptions,
						&GuiTableComposition::columns,
						&GuiTableComposition::rows,
						&X,
						&CL,
						&CS,
						&Second,
						&First,
						2
						);
					tableContentMinSize=Size(columnTotalWithPercentage, rowTotalWithPercentage);
				}
				if(previousContentMinSize!=tableContentMinSize)
				{
					previousContentMinSize=tableContentMinSize;
					UpdateCellBoundsInternal();
				}
			}

			void GuiTableComposition::OnRenderTargetChanged()
			{
				if(GetRenderTarget())
				{
					UpdateTableContentMinSize();
				}
			}

			GuiTableComposition::GuiTableComposition()
				:rows(0)
				,columns(0)
				,cellPadding(0)
				,rowExtending(0)
				,columnExtending(0)
			{
				SetRowsAndColumns(1, 1);
			}

			GuiTableComposition::~GuiTableComposition()
			{
			}

			int GuiTableComposition::GetRows()
			{
				return rows;
			}

			int GuiTableComposition::GetColumns()
			{
				return columns;
			}

			bool GuiTableComposition::SetRowsAndColumns(int _rows, int _columns)
			{
				if(_rows<=0 || _columns<=0) return false;
				rowOptions.Resize(_rows);
				columnOptions.Resize(_columns);
				cellCompositions.Resize(_rows*_columns);
				cellBounds.Resize(_rows*_columns);
				for(int i=0;i<_rows*_columns;i++)
				{
					cellCompositions[i]=0;
					cellBounds[i]=Rect();
				}
				rows=_rows;
				columns=_columns;
				int childCount=Children().Count();
				for(int i=0;i<childCount;i++)
				{
					GuiCellComposition* cell=dynamic_cast<GuiCellComposition*>(Children()[i]);
					if(cell)
					{
						cell->OnTableRowsAndColumnsChanged();
					}
				}
				UpdateCellBounds();
				return true;
			}

			GuiCellComposition* GuiTableComposition::GetSitedCell(int _row, int _column)
			{
				return cellCompositions[GetSiteIndex(rows, columns, _row, _column)];
			}

			GuiCellOption GuiTableComposition::GetRowOption(int _row)
			{
				return rowOptions[_row];
			}

			void GuiTableComposition::SetRowOption(int _row, GuiCellOption option)
			{
				rowOptions[_row]=option;
			}

			GuiCellOption GuiTableComposition::GetColumnOption(int _column)
			{
				return columnOptions[_column];
			}

			void GuiTableComposition::SetColumnOption(int _column, GuiCellOption option)
			{
				columnOptions[_column]=option;
			}

			int GuiTableComposition::GetCellPadding()
			{
				return cellPadding;
			}

			void GuiTableComposition::SetCellPadding(int value)
			{
				if(value<0) value=0;
				cellPadding=value;
			}

			Rect GuiTableComposition::GetCellArea()
			{
				Rect bounds(Point(0, 0), GuiBoundsComposition::GetBounds().GetSize());
				bounds.x1+=margin.left+internalMargin.left+cellPadding;
				bounds.y1+=margin.top+internalMargin.top+cellPadding;
				bounds.x2-=margin.right+internalMargin.right+cellPadding;
				bounds.y2-=margin.bottom+internalMargin.bottom+cellPadding;
				if(bounds.x2<bounds.x1) bounds.x2=bounds.x1;
				if(bounds.y2<bounds.y1) bounds.y2=bounds.y1;
				return bounds;
			}

			void GuiTableComposition::UpdateCellBounds()
			{
				UpdateCellBoundsInternal();
				UpdateTableContentMinSize();
			}

			void GuiTableComposition::ForceCalculateSizeImmediately()
			{
				GuiBoundsComposition::ForceCalculateSizeImmediately();
				UpdateCellBounds();
			}

			Size GuiTableComposition::GetMinPreferredClientSize()
			{
				return tableContentMinSize;
			}

			Rect GuiTableComposition::GetBounds()
			{
				Rect result;
				if(!IsAlignedToParent() && GetMinSizeLimitation()!=GuiGraphicsComposition::NoLimit)
				{
					result=Rect(compositionBounds.LeftTop(), compositionBounds.GetSize()-Size(columnExtending, rowExtending));
				}
				else
				{
					result=GuiBoundsComposition::GetBounds();
				}

				bool cellMinSizeModified=false;
				SortedList<GuiCellComposition*> cells;
				FOREACH(GuiCellComposition*, cell, cellCompositions.Wrap())
				{
					if(cell && !cells.Contains(cell))
					{
						cells.Add(cell);
						Size newSize=cell->GetPreferredBounds().GetSize();
						if(cell->lastPreferredSize!=newSize)
						{
							cell->lastPreferredSize=newSize;
							cellMinSizeModified=true;
						}
					}
				}

				if(previousBounds!=result || cellMinSizeModified)
				{
					previousBounds=result;
					UpdateCellBounds();
				}
				return result;
			}

/***********************************************************************
GuiCellComposition
***********************************************************************/

			void GuiCellComposition::ClearSitedCells(GuiTableComposition* table)
			{
				if(row!=-1 && column!=-1)
				{
					for(int r=0;r<rowSpan;r++)
					{
						for(int c=0;c<columnSpan;c++)
						{
							table->SetSitedCell(row+r, column+c, 0);
						}
					}
				}
			}

			void GuiCellComposition::SetSitedCells(GuiTableComposition* table)
			{
				for(int r=0;r<rowSpan;r++)
				{
					for(int c=0;c<columnSpan;c++)
					{
						table->SetSitedCell(row+r, column+c, this);
					}
				}
			}

			void GuiCellComposition::ResetSiteInternal()
			{
				row=-1;
				column=-1;
				rowSpan=1;
				columnSpan=1;
			}

			bool GuiCellComposition::SetSiteInternal(int _row, int _column, int _rowSpan, int _columnSpan)
			{
				if(!tableParent) return false;
				if(_row<0 || _row>=tableParent->rows || _column<0 || _column>=tableParent->columns) return false;
				if(_rowSpan<1 || _row+_rowSpan>tableParent->rows || _columnSpan<1 || _column+_columnSpan>tableParent->columns) return false;

				for(int r=0;r<_rowSpan;r++)
				{
					for(int c=0;c<_columnSpan;c++)
					{
						GuiCellComposition* cell=tableParent->GetSitedCell(_row+r, _column+c);
						if(cell && cell!=this)
						{
							return false;
						}
					}
				}
				ClearSitedCells(tableParent);
				row=_row;
				column=_column;
				rowSpan=_rowSpan;
				columnSpan=_columnSpan;
				SetSitedCells(tableParent);
				return true;
			}

			void GuiCellComposition::OnParentChanged(GuiGraphicsComposition* oldParent, GuiGraphicsComposition* newParent)
			{
				if(tableParent)
				{
					ClearSitedCells(tableParent);
				}
				tableParent=dynamic_cast<GuiTableComposition*>(newParent);
				if(!tableParent || !SetSiteInternal(row, column, rowSpan, columnSpan))
				{
					ResetSiteInternal();
				}
				if(tableParent)
				{
					tableParent->UpdateCellBounds();
				}
			}

			void GuiCellComposition::OnTableRowsAndColumnsChanged()
			{
				if(!SetSiteInternal(row, column, rowSpan, columnSpan))
				{
					ResetSiteInternal();
				}
			}

			GuiCellComposition::GuiCellComposition()
				:row(-1)
				,column(-1)
				,rowSpan(1)
				,columnSpan(1)
				,tableParent(0)
			{
				SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			}

			GuiCellComposition::~GuiCellComposition()
			{
			}

			GuiTableComposition* GuiCellComposition::GetTableParent()
			{
				return tableParent;
			}

			int GuiCellComposition::GetRow()
			{
				return row;
			}

			int GuiCellComposition::GetRowSpan()
			{
				return rowSpan;
			}

			int GuiCellComposition::GetColumn()
			{
				return column;
			}

			int GuiCellComposition::GetColumnSpan()
			{
				return columnSpan;
			}

			bool GuiCellComposition::SetSite(int _row, int _column, int _rowSpan, int _columnSpan)
			{
				if(SetSiteInternal(_row, _column, _rowSpan, _columnSpan))
				{
					tableParent->UpdateCellBounds();
					return true;
				}
				else
				{
					return false;
				}
			}

			Rect GuiCellComposition::GetBounds()
			{
				if(tableParent && row!=-1 && column!=-1)
				{
					Rect bounds1, bounds2;
					{
						int index=tableParent->GetSiteIndex(tableParent->rows, tableParent->columns, row, column);
						bounds1=tableParent->cellBounds[index];
					}
					{
						int index=tableParent->GetSiteIndex(tableParent->rows, tableParent->columns, row+rowSpan-1, column+columnSpan-1);
						bounds2=tableParent->cellBounds[index];
						if(tableParent->GetMinSizeLimitation()==GuiGraphicsComposition::NoLimit)
						{
							if(row+rowSpan==tableParent->rows)
							{
								bounds2.y2+=tableParent->rowExtending;
							}
							if(column+columnSpan==tableParent->columns)
							{
								bounds2.x2+=tableParent->columnExtending;
							}
						}
					}
					return Rect(bounds1.x1, bounds1.y1, bounds2.x2, bounds2.y2);
				}
				else
				{
					return Rect();
				}
			}
		}
	}
}