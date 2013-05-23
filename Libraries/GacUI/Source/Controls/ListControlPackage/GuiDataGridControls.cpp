#include "GuiDataGridControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace collections;
			using namespace compositions;

			namespace list
			{

				const wchar_t* const IDataProvider::Identifier = L"vl::presentation::controls::list::IDataProvider";
				
/***********************************************************************
ListViewMainColumnDataVisualizer
***********************************************************************/

				compositions::GuiBoundsComposition* ListViewMainColumnDataVisualizer::CreateBoundsCompositionInternal()
				{
					return 0;
				}

				void ListViewMainColumnDataVisualizer::BeforeVisualizerCell(IDataProvider* dataProvider, vint row, vint column)
				{
				}
				
/***********************************************************************
ListViewSubColumnDataVisualizer
***********************************************************************/

				compositions::GuiBoundsComposition* ListViewSubColumnDataVisualizer::CreateBoundsCompositionInternal()
				{
					return 0;
				}

				void ListViewSubColumnDataVisualizer::BeforeVisualizerCell(IDataProvider* dataProvider, vint row, vint column)
				{
				}

/***********************************************************************
DataGridItemProvider
***********************************************************************/

				void DataGridItemProvider::InvokeOnItemModified(vint start, vint count, vint newCount)
				{
					for(vint i=0;i<itemProviderCallbacks.Count();i++)
					{
						itemProviderCallbacks[i]->OnItemModified(start, count, newCount);
					}
				}

				void DataGridItemProvider::InvokeOnColumnChanged()
				{
					for(vint i=0;i<columnItemViewCallbacks.Count();i++)
					{
						columnItemViewCallbacks[i]->OnColumnChanged();
					}
				}

				DataGridItemProvider::DataGridItemProvider(IDataProvider* _dataProvider)
					:dataProvider(_dataProvider)
					,sortingColumn(-1)
					,sortingColumnAscending(true)
				{
				}

				DataGridItemProvider::~DataGridItemProvider()
				{
				}

				void DataGridItemProvider::SortByColumn(vint column, bool ascending)
				{
					sortingColumn=column;
					sortingColumnAscending=ascending;
					dataProvider->SortByColumn(column, ascending);
					InvokeOnColumnChanged();
					InvokeOnItemModified(0, dataProvider->GetRowCount(), dataProvider->GetRowCount());
				}

// ===================== GuiListControl::IItemProvider =====================

				bool DataGridItemProvider::AttachCallback(GuiListControl::IItemProviderCallback* value)
				{
					if(itemProviderCallbacks.Contains(value))
					{
						return false;
					}
					else
					{
						itemProviderCallbacks.Add(value);
						value->OnAttached(this);
						return true;
					}
				}

				bool DataGridItemProvider::DetachCallback(GuiListControl::IItemProviderCallback* value)
				{
					vint index=itemProviderCallbacks.IndexOf(value);
					if(index==-1)
					{
						return false;
					}
					else
					{
						value->OnAttached(0);
						itemProviderCallbacks.Remove(value);
						return true;
					}
				}

				vint DataGridItemProvider::Count()
				{
					return dataProvider->GetRowCount();
				}

				IDescriptable* DataGridItemProvider::RequestView(const WString& identifier)
				{
					if(identifier==IDataProvider::Identifier)
					{
						return dataProvider;
					}
					else if(identifier==ListViewItemStyleProvider::IListViewItemView::Identifier)
					{
						return (ListViewItemStyleProvider::IListViewItemView*)this;
					}
					else if(identifier==ListViewColumnItemArranger::IColumnItemView::Identifier)
					{
						return (ListViewColumnItemArranger::IColumnItemView*)this;
					}
					else if(identifier==GuiListControl::IItemPrimaryTextView::Identifier)
					{
						return (GuiListControl::IItemPrimaryTextView*)this;
					}
					else
					{
						return 0;
					}
				}

				void DataGridItemProvider::ReleaseView(IDescriptable* view)
				{
				}

// ===================== GuiListControl::IItemPrimaryTextView =====================

				WString DataGridItemProvider::GetPrimaryTextViewText(vint itemIndex)
				{
					return GetText(itemIndex);
				}

				bool DataGridItemProvider::ContainsPrimaryText(vint itemIndex)
				{
					return true;
				}

// ===================== list::ListViewItemStyleProvider::IListViewItemView =====================

				Ptr<GuiImageData> DataGridItemProvider::GetSmallImage(vint itemIndex)
				{
					return dataProvider->GetRowImage(itemIndex);
				}

				Ptr<GuiImageData> DataGridItemProvider::GetLargeImage(vint itemIndex)
				{
					return 0;
				}

				WString DataGridItemProvider::GetText(vint itemIndex)
				{
					return dataProvider->GetCellText(itemIndex, 0);
				}

				WString DataGridItemProvider::GetSubItem(vint itemIndex, vint index)
				{
					return dataProvider->GetCellText(itemIndex, index+1);
				}

				vint DataGridItemProvider::GetDataColumnCount()
				{
					return 0;
				}

				vint DataGridItemProvider::GetDataColumn(vint index)
				{
					return 0;
				}

// ===================== list::ListViewColumnItemArranger::IColumnItemView =====================
						
				bool DataGridItemProvider::AttachCallback(ListViewColumnItemArranger::IColumnItemViewCallback* value)
				{
					if(columnItemViewCallbacks.Contains(value))
					{
						return false;
					}
					else
					{
						columnItemViewCallbacks.Add(value);
						return true;
					}
				}

				bool DataGridItemProvider::DetachCallback(ListViewColumnItemArranger::IColumnItemViewCallback* value)
				{
					vint index=columnItemViewCallbacks.IndexOf(value);
					if(index==-1)
					{
						return false;
					}
					else
					{
						columnItemViewCallbacks.Remove(value);
						return true;
					}
				}

				vint DataGridItemProvider::GetColumnCount()
				{
					return dataProvider->GetColumnCount();
				}

				WString DataGridItemProvider::GetColumnText(vint index)
				{
					return dataProvider->GetColumnText(index);
				}

				vint DataGridItemProvider::GetColumnSize(vint index)
				{
					return dataProvider->GetColumnSize(index);
				}

				void DataGridItemProvider::SetColumnSize(vint index, vint value)
				{
					dataProvider->SetColumnSize(index, value);
					for(vint i=0;i<columnItemViewCallbacks.Count();i++)
					{
						columnItemViewCallbacks[i]->OnColumnChanged();
					}
				}

				GuiMenu* DataGridItemProvider::GetDropdownPopup(vint index)
				{
					return 0;
				}

				GuiListViewColumnHeader::ColumnSortingState DataGridItemProvider::GetSortingState(vint index)
				{
					if(index==sortingColumn)
					{
						return sortingColumnAscending
							?GuiListViewColumnHeader::Ascending
							:GuiListViewColumnHeader::Descending;
					}
					else
					{
						return GuiListViewColumnHeader::NotSorted;
					}
				}
				
/***********************************************************************
DataGridContentProvider
***********************************************************************/

				DataGridContentProvider::ItemContent::ItemContent(GuiListControl::IItemProvider* _itemProvider)
					:contentComposition(0)
					,itemProvider(_itemProvider)
				{
					columnItemView=dynamic_cast<ListViewColumnItemArranger::IColumnItemView*>(itemProvider->RequestView(ListViewColumnItemArranger::IColumnItemView::Identifier));
					dataProvider=dynamic_cast<IDataProvider*>(itemProvider->RequestView(IDataProvider::Identifier));

					contentComposition=new GuiBoundsComposition;
					contentComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);

					textTable=new GuiTableComposition;
					textTable->SetAlignmentToParent(Margin(0, 0, 0, 0));
					textTable->SetRowsAndColumns(1, 1);
					textTable->SetRowOption(0, GuiCellOption::MinSizeOption());
					textTable->SetColumnOption(0, GuiCellOption::AbsoluteOption(0));
					contentComposition->AddChild(textTable);
				}

				DataGridContentProvider::ItemContent::~ItemContent()
				{
					if(columnItemView)
					{
						itemProvider->ReleaseView(columnItemView);
					}
				}

				compositions::GuiBoundsComposition* DataGridContentProvider::ItemContent::GetContentComposition()
				{
					return contentComposition;
				}

				compositions::GuiBoundsComposition* DataGridContentProvider::ItemContent::GetBackgroundDecorator()
				{
					return 0;
				}

				void DataGridContentProvider::ItemContent::UpdateSubItemSize()
				{
					vint columnCount=columnItemView->GetColumnCount();
					for(vint i=0;i<columnCount;i++)
					{
						textTable->SetColumnOption(i, GuiCellOption::AbsoluteOption(columnItemView->GetColumnSize(i)));
					}
					textTable->UpdateCellBounds();
				}

				void DataGridContentProvider::ItemContent::Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, vint itemIndex)
				{
					for(vint i=0;i<textTable->GetColumns();i++)
					{
						GuiCellComposition* cell=textTable->GetSitedCell(0, i);
						textTable->RemoveChild(cell);
						delete cell;
					}

					vint columnCount=columnItemView->GetColumnCount();
					textTable->SetRowsAndColumns(1, columnCount);
					for(vint i=0;i<columnCount;i++)
					{
						GuiCellComposition* cell=new GuiCellComposition;
						textTable->AddChild(cell);
						cell->SetSite(0, i, 1, 1);
						cell->SetMargin(Margin(8, 0, 8, 0));

						//////////////////////////////
					}
					UpdateSubItemSize();
				}

				void DataGridContentProvider::OnColumnChanged()
				{
					vint count=listViewItemStyleProvider->GetCreatedItemStyles().Count();
					for(vint i=0;i<count;i++)
					{
						GuiListControl::IItemStyleController* itemStyleController=listViewItemStyleProvider->GetCreatedItemStyles().Get(i);
						ItemContent* itemContent=listViewItemStyleProvider->GetItemContent<ItemContent>(itemStyleController);
						if(itemContent)
						{
							itemContent->UpdateSubItemSize();
						}
					}
				}

				DataGridContentProvider::DataGridContentProvider()
					:itemProvider(0)
					,columnItemView(0)
					,listViewItemStyleProvider(0)
				{
				}

				DataGridContentProvider::~DataGridContentProvider()
				{
				}

				GuiListControl::IItemCoordinateTransformer* DataGridContentProvider::CreatePreferredCoordinateTransformer()
				{
					return new DefaultItemCoordinateTransformer;
				}

				GuiListControl::IItemArranger* DataGridContentProvider::CreatePreferredArranger()
				{
					return new ListViewColumnItemArranger;
				}

				ListViewItemStyleProvider::IListViewItemContent* DataGridContentProvider::CreateItemContent(const FontProperties& font)
				{
					return new ItemContent(itemProvider);
				}

				void DataGridContentProvider::AttachListControl(GuiListControl* value)
				{
					listViewItemStyleProvider=dynamic_cast<ListViewItemStyleProvider*>(value->GetStyleProvider());
					itemProvider=value->GetItemProvider();
					columnItemView=dynamic_cast<ListViewColumnItemArranger::IColumnItemView*>(itemProvider->RequestView(ListViewColumnItemArranger::IColumnItemView::Identifier));
					if(columnItemView)
					{
						columnItemView->AttachCallback(this);
					}
				}

				void DataGridContentProvider::DetachListControl()
				{
					if(columnItemView)
					{
						columnItemView->DetachCallback(this);
						itemProvider->ReleaseView(columnItemView);
					}
					itemProvider=0;
					listViewItemStyleProvider=0;
				}
			}

/***********************************************************************
GuiVirtualDataGrid
***********************************************************************/

			using namespace list;

			void GuiVirtualDataGrid::OnColumnClicked(compositions::GuiGraphicsComposition* sender, compositions::GuiItemEventArgs& arguments)
			{
				if(dataProvider->IsColumnSortable(arguments.itemIndex))
				{
					switch(itemProvider->GetSortingState(arguments.itemIndex))
					{
					case GuiListViewColumnHeader::NotSorted:
						itemProvider->SortByColumn(arguments.itemIndex, true);
						break;
					case GuiListViewColumnHeader::Ascending:
						itemProvider->SortByColumn(arguments.itemIndex, false);
						break;
					case GuiListViewColumnHeader::Descending:
						itemProvider->SortByColumn(-1);
						break;
					}
				}
			}

			GuiVirtualDataGrid::GuiVirtualDataGrid(IStyleProvider* _styleProvider, list::IDataProvider* _dataProvider)
				:GuiVirtualListView(_styleProvider, new DataGridItemProvider(_dataProvider))
				,dataProvider(_dataProvider)
			{
				itemProvider=dynamic_cast<DataGridItemProvider*>(GetItemProvider());
				ChangeItemStyle(new DataGridContentProvider);

				ColumnClicked.AttachMethod(this, &GuiVirtualDataGrid::OnColumnClicked);
			}

			GuiVirtualDataGrid::~GuiVirtualDataGrid()
			{
			}
		}
	}
}