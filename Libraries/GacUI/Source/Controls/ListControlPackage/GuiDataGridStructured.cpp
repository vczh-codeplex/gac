#include "GuiDataGridStructured.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			namespace list
			{
				using namespace collections;
				
/***********************************************************************
StructuredDataFilterBase
***********************************************************************/

				void StructuredDataFilterBase::InvokeOnFilterChanged()
				{
					if(commandExecutor)
					{
						commandExecutor->OnFilterChanged();
					}
				}

				StructuredDataFilterBase::StructuredDataFilterBase()
					:commandExecutor(0)
				{
				}

				void StructuredDataFilterBase::SetCommandExecutor(IStructuredDataFilterCommandExecutor* value)
				{
					commandExecutor=value;
				}
				
/***********************************************************************
StructuredDataMultipleFilter
***********************************************************************/

				StructuredDataMultipleFilter::StructuredDataMultipleFilter()
				{
				}

				bool StructuredDataMultipleFilter::AddSubFilter(Ptr<IStructuredDataFilter> value)
				{
					if(!value) return false;
					if(filters.Contains(value.Obj())) return false;
					filters.Add(value);
					value->SetCommandExecutor(commandExecutor);
					InvokeOnFilterChanged();
					return true;
				}

				bool StructuredDataMultipleFilter::RemoveSubFilter(Ptr<IStructuredDataFilter> value)
				{
					if(!value) return false;
					if(!filters.Contains(value.Obj())) return false;
					value->SetCommandExecutor(0);
					filters.Remove(value.Obj());
					InvokeOnFilterChanged();
					return true;
				}

				void StructuredDataMultipleFilter::SetCommandExecutor(IStructuredDataFilterCommandExecutor* value)
				{
					StructuredDataFilterBase::SetCommandExecutor(value);
					for(vint i=0;i<filters.Count();i++)
					{
						filters[i]->SetCommandExecutor(value);
					}
				}
				
/***********************************************************************
StructuredDataAndFilter
***********************************************************************/

				StructuredDataAndFilter::StructuredDataAndFilter()
				{
				}

				bool StructuredDataAndFilter::Filter(vint row)
				{
					return From(filters)
						.All([row](Ptr<IStructuredDataFilter> filter)
						{
							return filter->Filter(row);
						});
				}
				
/***********************************************************************
StructuredDataOrFilter
***********************************************************************/
				
				StructuredDataOrFilter::StructuredDataOrFilter()
				{
				}

				bool StructuredDataOrFilter::Filter(vint row)
				{
					return From(filters)
						.Any([row](Ptr<IStructuredDataFilter> filter)
						{
							return filter->Filter(row);
						});
				}
				
/***********************************************************************
StructuredDataNotFilter
***********************************************************************/
				
				StructuredDataNotFilter::StructuredDataNotFilter()
				{
				}

				bool StructuredDataNotFilter::SetSubFilter(Ptr<IStructuredDataFilter> value)
				{
					if(filter==value) return false;
					if(filter)
					{
						filter->SetCommandExecutor(0);
					}
					filter=value;
					if(filter)
					{
						filter->SetCommandExecutor(commandExecutor);
					}
					InvokeOnFilterChanged();
					return true;
				}

				void StructuredDataNotFilter::SetCommandExecutor(IStructuredDataFilterCommandExecutor* value)
				{
					StructuredDataFilterBase::SetCommandExecutor(value);
					if(filter)
					{
						filter->SetCommandExecutor(value);
					}
				}

				bool StructuredDataNotFilter::Filter(vint row)
				{
					return filter?true:!filter->Filter(row);
				}
				
/***********************************************************************
StructuredDataMultipleSorter
***********************************************************************/

				StructuredDataMultipleSorter::StructuredDataMultipleSorter()
				{
				}

				bool StructuredDataMultipleSorter::SetLeftSorter(Ptr<IStructuredDataSorter> value)
				{
					if(leftSorter==value) return false;
					leftSorter=value;
					return true;
				}

				bool StructuredDataMultipleSorter::SetRightSorter(Ptr<IStructuredDataSorter> value)
				{
					if(rightSorter==value) return false;
					rightSorter=value;
					return true;
				}

				vint StructuredDataMultipleSorter::Compare(vint row1, vint row2)
				{
					if(leftSorter)
					{
						vint result=leftSorter->Compare(row1, row2);
						if(result!=0) return result;
					}
					if(rightSorter)
					{
						vint result=rightSorter->Compare(row1, row2);
						if(result!=0) return result;
					}
					return 0;
				}
				
/***********************************************************************
StructuredDataReverseSorter
***********************************************************************/

				StructuredDataReverseSorter::StructuredDataReverseSorter()
				{
				}

				bool StructuredDataReverseSorter::SetSubSorter(Ptr<IStructuredDataSorter> value)
				{
					if(sorter==value) return false;
					sorter=value;
					return true;
				}

				vint StructuredDataReverseSorter::Compare(vint row1, vint row2)
				{
					return sorter?-sorter->Compare(row1, row2):0;
				}
				
/***********************************************************************
StructuredDataProvider
***********************************************************************/

				void StructuredDataProvider::OnDataProviderColumnChanged()
				{
					RebuildFilter();
					ReorderRows();
					if(commandExecutor)
					{
						commandExecutor->OnDataProviderColumnChanged();
					}
				}

				void StructuredDataProvider::OnDataProviderItemModified(vint start, vint count, vint newCount)
				{
					// optimized for cell editing
					if(!currentSorter && !currentFilter)
					{
						if(count!=newCount)
						{
							ReorderRows();
						}
						commandExecutor->OnDataProviderItemModified(start, count, newCount);
					}
					else
					{
						ReorderRows();
						commandExecutor->OnDataProviderItemModified(0, GetRowCount(), GetRowCount());
					}
				}

				void StructuredDataProvider::OnFilterChanged()
				{
					ReorderRows();
					if(commandExecutor)
					{
						commandExecutor->OnDataProviderColumnChanged();
					}
				}

				void StructuredDataProvider::RebuildFilter()
				{
					if(currentFilter)
					{
						currentFilter->SetCommandExecutor(0);
						currentFilter=0;
					}

					List<Ptr<IStructuredDataFilter>> selectedFilters;
					CopyFrom(
						selectedFilters,
						Range(0, GetColumnCount())
							.Select([this](vint column){return structuredDataProvider->GetColumn(column)->GetInherentFilter();})
							.Where([](Ptr<IStructuredDataFilter> filter){return (bool)filter;})
						);
					if(additionalFilter)
					{
						selectedFilters.Add(additionalFilter);
					}
					if(selectedFilters.Count()>0)
					{
						Ptr<StructuredDataAndFilter> andFilter=new StructuredDataAndFilter;
						FOREACH(Ptr<IStructuredDataFilter>, filter, selectedFilters)
						{
							andFilter->AddSubFilter(filter);
						}
						currentFilter=andFilter;
					}

					if(currentFilter)
					{
						currentFilter->SetCommandExecutor(this);
					}
				}

				void StructuredDataProvider::ReorderRows()
				{
					reorderedRows.Clear();
					vint rowCount=structuredDataProvider->GetRowCount();

					if(currentFilter)
					{
						for(vint i=0;i<rowCount;i++)
						{
							if(currentFilter->Filter(i))
							{
								reorderedRows.Add(i);
							}
						}
					}
					else
					{
						for(vint i=0;i<rowCount;i++)
						{
							reorderedRows.Add(i);
						}
					}

					if(currentSorter && reorderedRows.Count()>0)
					{
						IStructuredDataSorter* sorter=currentSorter.Obj();
						SortLambda(&reorderedRows[0], reorderedRows.Count(), [sorter](vint a, vint b){return sorter->Compare(a, b);});
					}
				}

				vint StructuredDataProvider::TranslateRowNumber(vint row)
				{
					return reorderedRows[row];
				}

				Ptr<IStructuredDataFilter> StructuredDataProvider::GetAdditionalFilter()
				{
					return additionalFilter;
				}

				void StructuredDataProvider::SetAdditionalFilter(Ptr<IStructuredDataFilter> value)
				{
					additionalFilter=value;
					RebuildFilter();
					ReorderRows();
					if(commandExecutor)
					{
						commandExecutor->OnDataProviderColumnChanged();
						commandExecutor->OnDataProviderItemModified(0, GetRowCount(), GetRowCount());
					}
				}

				StructuredDataProvider::StructuredDataProvider(Ptr<IStructuredDataProvider> provider)
					:structuredDataProvider(provider)
					,commandExecutor(0)
				{
					structuredDataProvider->SetCommandExecutor(this);
					RebuildFilter();
					ReorderRows();
				}

				StructuredDataProvider::~StructuredDataProvider()
				{
				}

				void StructuredDataProvider::SetCommandExecutor(IDataProviderCommandExecutor* value)
				{
					commandExecutor=value;
				}

				vint StructuredDataProvider::GetColumnCount()
				{
					return structuredDataProvider->GetColumnCount();
				}

				WString StructuredDataProvider::GetColumnText(vint column)
				{
					return structuredDataProvider->GetColumn(column)->GetText();
				}

				vint StructuredDataProvider::GetColumnSize(vint column)
				{
					return structuredDataProvider->GetColumn(column)->GetSize();
				}

				void StructuredDataProvider::SetColumnSize(vint column, vint value)
				{
					structuredDataProvider->GetColumn(column)->SetSize(value);
				}

				GuiMenu* StructuredDataProvider::GetColumnPopup(vint column)
				{
					return structuredDataProvider->GetColumn(column)->GetPopup();
				}

				bool StructuredDataProvider::IsColumnSortable(vint column)
				{
					return structuredDataProvider->GetColumn(column)->GetInherentSorter();
				}

				void StructuredDataProvider::SortByColumn(vint column, bool ascending)
				{
					if(0<=column && column<structuredDataProvider->GetColumnCount())
					{
						Ptr<IStructuredDataSorter> sorter=structuredDataProvider->GetColumn(column)->GetInherentSorter();
						if(!sorter)
						{
							currentSorter=0;
						}
						else if(ascending)
						{
							currentSorter=sorter;
						}
						else
						{
							Ptr<StructuredDataReverseSorter> reverseSorter=new StructuredDataReverseSorter();
							reverseSorter->SetSubSorter(sorter);
							currentSorter=reverseSorter;
						}
					}
					else
					{
						currentSorter=0;
					}
					for(vint i=0;i<structuredDataProvider->GetColumnCount();i++)
					{
						structuredDataProvider->GetColumn(i)->SetSortingState(
							i!=column?GuiListViewColumnHeader::NotSorted:
							ascending?GuiListViewColumnHeader::Ascending:
							GuiListViewColumnHeader::Descending
							);
					}
					ReorderRows();
					if(commandExecutor)
					{
						commandExecutor->OnDataProviderColumnChanged();
						commandExecutor->OnDataProviderItemModified(0, GetRowCount(), GetRowCount());
					}
				}

				vint StructuredDataProvider::GetSortedColumn()
				{
					for(vint i=0;i<structuredDataProvider->GetColumnCount();i++)
					{
						GuiListViewColumnHeader::ColumnSortingState state=structuredDataProvider->GetColumn(i)->GetSortingState();
						if(state!=GuiListViewColumnHeader::NotSorted)
						{
							return i;
						}
					}
					return -1;
				}

				bool StructuredDataProvider::IsSortOrderAscending()
				{
					for(vint i=0;i<structuredDataProvider->GetColumnCount();i++)
					{
						GuiListViewColumnHeader::ColumnSortingState state=structuredDataProvider->GetColumn(i)->GetSortingState();
						if(state!=GuiListViewColumnHeader::NotSorted)
						{
							return state==GuiListViewColumnHeader::Ascending;
						}
					}
					return true;
				}
					
				vint StructuredDataProvider::GetRowCount()
				{
					return reorderedRows.Count();
				}

				Ptr<GuiImageData> StructuredDataProvider::GetRowImage(vint row)
				{
					return structuredDataProvider->GetRowImage(TranslateRowNumber(row));
				}

				WString StructuredDataProvider::GetCellText(vint row, vint column)
				{
					return structuredDataProvider->GetColumn(column)->GetCellText(TranslateRowNumber(row));
				}

				IDataVisualizerFactory* StructuredDataProvider::GetCellDataVisualizerFactory(vint row, vint column)
				{
					return structuredDataProvider->GetColumn(column)->GetCellDataVisualizerFactory(TranslateRowNumber(row));
				}

				void StructuredDataProvider::VisualizeCell(vint row, vint column, IDataVisualizer* dataVisualizer)
				{
					structuredDataProvider->GetColumn(column)->VisualizeCell(TranslateRowNumber(row), dataVisualizer);
				}

				IDataEditorFactory* StructuredDataProvider::GetCellDataEditorFactory(vint row, vint column)
				{
					return structuredDataProvider->GetColumn(column)->GetCellDataEditorFactory(TranslateRowNumber(row));
				}

				void StructuredDataProvider::BeforeEditCell(vint row, vint column, IDataEditor* dataEditor)
				{
					structuredDataProvider->GetColumn(column)->BeforeEditCell(TranslateRowNumber(row), dataEditor);
				}

				void StructuredDataProvider::SaveCellData(vint row, vint column, IDataEditor* dataEditor)
				{
					structuredDataProvider->GetColumn(column)->SaveCellData(TranslateRowNumber(row), dataEditor);
				}
				
/***********************************************************************
StructuredColummProviderBase
***********************************************************************/

				StructuredColummProviderBase::StructuredColummProviderBase()
					:commandExecutor(0)
					,size(0)
					,sortingState(GuiListViewColumnHeader::NotSorted)
					,popup(0)
				{
				}

				StructuredColummProviderBase::~StructuredColummProviderBase()
				{
				}
					
				void StructuredColummProviderBase::SetCommandExecutor(IDataProviderCommandExecutor* value)
				{
					commandExecutor=value;
				}

				void StructuredColummProviderBase::SetText(const WString& value)
				{
					text=value;
					if(commandExecutor)
					{
						commandExecutor->OnDataProviderColumnChanged();
					}
				}

				void StructuredColummProviderBase::SetPopup(GuiMenu* value)
				{
					popup=value;
					if(commandExecutor)
					{
						commandExecutor->OnDataProviderColumnChanged();
					}
				}

				void StructuredColummProviderBase::SetInherentFilter(Ptr<IStructuredDataFilter> value)
				{
					inherentFilter=value;
					if(commandExecutor)
					{
						commandExecutor->OnDataProviderColumnChanged();
					}
				}

				void StructuredColummProviderBase::SetInherentSorter(Ptr<IStructuredDataSorter> value)
				{
					inherentSorter=value;
					if(commandExecutor)
					{
						commandExecutor->OnDataProviderColumnChanged();
					}
				}

				WString StructuredColummProviderBase::GetText()
				{
					return text;
				}

				vint StructuredColummProviderBase::GetSize()
				{
					return size;
				}

				void StructuredColummProviderBase::SetSize(vint value)
				{
					size=value;
				}

				GuiListViewColumnHeader::ColumnSortingState StructuredColummProviderBase::GetSortingState()
				{
					return sortingState;
				}

				void StructuredColummProviderBase::SetSortingState(GuiListViewColumnHeader::ColumnSortingState value)
				{
					sortingState=value;
				}

				GuiMenu* StructuredColummProviderBase::GetPopup()
				{
					return popup;
				}

				Ptr<IStructuredDataFilter> StructuredColummProviderBase::GetInherentFilter()
				{
					return inherentFilter;
				}

				Ptr<IStructuredDataSorter> StructuredColummProviderBase::GetInherentSorter()
				{
					return inherentSorter;
				}
					
				IDataVisualizerFactory* StructuredColummProviderBase::GetCellDataVisualizerFactory(vint row)
				{
					return 0;
				}

				void StructuredColummProviderBase::VisualizeCell(vint row, IDataVisualizer* dataVisualizer)
				{
				}

				IDataEditorFactory* StructuredColummProviderBase::GetCellDataEditorFactory(vint row)
				{
					return 0;
				}

				void StructuredColummProviderBase::BeforeEditCell(vint row, IDataEditor* dataEditor)
				{
				}

				void StructuredColummProviderBase::SaveCellData(vint row, IDataEditor* dataEditor)
				{
				}
				
/***********************************************************************
StructuredDataProviderBase
***********************************************************************/

				bool StructuredDataProviderBase::AddColumn(Ptr<StructuredColummProviderBase> value)
				{
					if(columns.Contains(value.Obj())) return false;
					columns.Add(value);
					value->SetCommandExecutor(commandExecutor);
					if(commandExecutor)
					{
						commandExecutor->OnDataProviderColumnChanged();
					}
					return true;
				}

				StructuredDataProviderBase::StructuredDataProviderBase()
					:commandExecutor(0)
				{
				}

				StructuredDataProviderBase::~StructuredDataProviderBase()
				{
				}

				void StructuredDataProviderBase::SetCommandExecutor(IDataProviderCommandExecutor* value)
				{
					commandExecutor=value;
					FOREACH(Ptr<StructuredColummProviderBase>, column, columns)
					{
						column->SetCommandExecutor(commandExecutor);
					}
				}

				vint StructuredDataProviderBase::GetColumnCount()
				{
					return columns.Count();
				}

				IStructuredColumnProvider* StructuredDataProviderBase::GetColumn(vint column)
				{
					return columns[column].Obj();
				}

				Ptr<GuiImageData> StructuredDataProviderBase::GetRowImage(vint row)
				{
					return 0;
				}
			}
		}
	}
}