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
DataGridItemProvider
***********************************************************************/

				DataGridItemProvider::DataGridItemProvider(IDataProvider* _dataProvider)
					:dataProvider(_dataProvider)
				{
				}

				DataGridItemProvider::~DataGridItemProvider()
				{
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
					//////////////////////////////////////
					throw 0;
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
					return 0;
				}

				Ptr<GuiImageData> DataGridItemProvider::GetLargeImage(vint itemIndex)
				{
					return 0;
				}

				WString DataGridItemProvider::GetText(vint itemIndex)
				{
					//////////////////////////////////////
					throw 0;
				}

				WString DataGridItemProvider::GetSubItem(vint itemIndex, vint index)
				{
					//////////////////////////////////////
					throw 0;
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
					//////////////////////////////////////
					throw 0;
				}

				WString DataGridItemProvider::GetColumnText(vint index)
				{
					//////////////////////////////////////
					throw 0;
				}

				vint DataGridItemProvider::GetColumnSize(vint index)
				{
					//////////////////////////////////////
					throw 0;
				}

				void DataGridItemProvider::SetColumnSize(vint index, vint value)
				{
					//////////////////////////////////////
					throw 0;
				}

				GuiMenu* DataGridItemProvider::GetDropdownPopup(vint index)
				{
					return 0;
				}

				GuiListViewColumnHeader::ColumnSortingState DataGridItemProvider::GetSortingState(vint index)
				{
					return GuiListViewColumnHeader::NotSorted;
				}
				
/***********************************************************************
DatagridContentProvider
***********************************************************************/

				DatagridContentProvider::ItemContent::ItemContent(GuiListControl::IItemProvider* _itemProvider)
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

				DatagridContentProvider::ItemContent::~ItemContent()
				{
					if(columnItemView)
					{
						itemProvider->ReleaseView(columnItemView);
					}
				}

				compositions::GuiBoundsComposition* DatagridContentProvider::ItemContent::GetContentComposition()
				{
					return contentComposition;
				}

				compositions::GuiBoundsComposition* DatagridContentProvider::ItemContent::GetBackgroundDecorator()
				{
					return 0;
				}

				void DatagridContentProvider::ItemContent::UpdateSubItemSize()
				{
					vint columnCount=columnItemView->GetColumnCount();
					for(vint i=0;i<columnCount;i++)
					{
						textTable->SetColumnOption(i, GuiCellOption::AbsoluteOption(columnItemView->GetColumnSize(i)));
					}
					textTable->UpdateCellBounds();
				}

				void DatagridContentProvider::ItemContent::Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, vint itemIndex)
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

				void DatagridContentProvider::OnColumnChanged()
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

				DatagridContentProvider::DatagridContentProvider()
					:itemProvider(0)
					,columnItemView(0)
					,listViewItemStyleProvider(0)
				{
				}

				DatagridContentProvider::~DatagridContentProvider()
				{
				}

				GuiListControl::IItemCoordinateTransformer* DatagridContentProvider::CreatePreferredCoordinateTransformer()
				{
					return new DefaultItemCoordinateTransformer;
				}

				GuiListControl::IItemArranger* DatagridContentProvider::CreatePreferredArranger()
				{
					return new ListViewColumnItemArranger;
				}

				ListViewItemStyleProvider::IListViewItemContent* DatagridContentProvider::CreateItemContent(const FontProperties& font)
				{
					return new ItemContent(itemProvider);
				}

				void DatagridContentProvider::AttachListControl(GuiListControl* value)
				{
					listViewItemStyleProvider=dynamic_cast<ListViewItemStyleProvider*>(value->GetStyleProvider());
					itemProvider=value->GetItemProvider();
					columnItemView=dynamic_cast<ListViewColumnItemArranger::IColumnItemView*>(itemProvider->RequestView(ListViewColumnItemArranger::IColumnItemView::Identifier));
					if(columnItemView)
					{
						columnItemView->AttachCallback(this);
					}
				}

				void DatagridContentProvider::DetachListControl()
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

			GuiVirtualDataGrid::GuiVirtualDataGrid(IStyleProvider* _styleProvider, list::IDataProvider* _dataProvider)
				:GuiVirtualListView(_styleProvider, new DataGridItemProvider(_dataProvider))
				,dataProvider(_dataProvider)
			{
				ChangeItemStyle(new ListViewDetailContentProvider);
			}

			GuiVirtualDataGrid::~GuiVirtualDataGrid()
			{
			}
		}
	}
}