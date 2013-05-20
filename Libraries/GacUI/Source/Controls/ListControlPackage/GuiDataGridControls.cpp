#include "GuiDataGridControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace collections;
			using namespace list;

/***********************************************************************
GuiVirtualDataGridItemProvider
***********************************************************************/

			class GuiVirtualDataGridItemProvider
				: public Object
				, public virtual GuiListControl::IItemProvider
				, protected virtual GuiListControl::IItemPrimaryTextView
				, protected virtual ListViewItemStyleProvider::IListViewItemView
				, protected virtual ListViewColumnItemArranger::IColumnItemView
			{
			protected:
				GuiVirtualDataGrid::IDataProvider*								dataProvider;
				List<GuiListControl::IItemProviderCallback*>					itemProviderCallbacks;
				List<ListViewColumnItemArranger::IColumnItemViewCallback*>		columnItemViewCallbacks;
			public:
				GuiVirtualDataGridItemProvider(GuiVirtualDataGrid::IDataProvider* _dataProvider)
					:dataProvider(_dataProvider)
				{
				}

/***********************************************************************
GuiVirtualDataGridItemProvider (GuiListControl::IItemProvider)
***********************************************************************/

				bool AttachCallback(GuiListControl::IItemProviderCallback* value)
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

				bool DetachCallback(GuiListControl::IItemProviderCallback* value)
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

				vint Count()
				{
					//////////////////////////////////////
					throw 0;
				}

				IDescriptable* RequestView(const WString& identifier)
				{
					if(identifier==ListViewItemStyleProvider::IListViewItemView::Identifier)
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

				void ReleaseView(IDescriptable* view)
				{
				}

/***********************************************************************
GuiVirtualDataGridItemProvider (GuiListControl::IItemPrimaryTextView)
***********************************************************************/

				WString GetPrimaryTextViewText(vint itemIndex)
				{
					return GetText(itemIndex);
				}

				bool ContainsPrimaryText(vint itemIndex)
				{
					return true;
				}

/***********************************************************************
GuiVirtualDataGridItemProvider (ListViewItemStyleProvider::IListViewItemView)
***********************************************************************/

				Ptr<GuiImageData> GetSmallImage(vint itemIndex)
				{
					return 0;
				}

				Ptr<GuiImageData> GetLargeImage(vint itemIndex)
				{
					return 0;
				}

				WString GetText(vint itemIndex)
				{
					//////////////////////////////////////
					throw 0;
				}

				WString GetSubItem(vint itemIndex, vint index)
				{
					//////////////////////////////////////
					throw 0;
				}

				vint GetDataColumnCount()
				{
					return 0;
				}

				vint GetDataColumn(vint index)
				{
					return 0;
				}

/***********************************************************************
GuiVirtualDataGridItemProvider (ListViewColumnItemArranger::IColumnItemView)
***********************************************************************/
						
				bool AttachCallback(ListViewColumnItemArranger::IColumnItemViewCallback* value)
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

				bool DetachCallback(ListViewColumnItemArranger::IColumnItemViewCallback* value)
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

				vint GetColumnCount()
				{
					//////////////////////////////////////
					throw 0;
				}

				WString GetColumnText(vint index)
				{
					//////////////////////////////////////
					throw 0;
				}

				vint GetColumnSize(vint index)
				{
					//////////////////////////////////////
					throw 0;
				}

				void SetColumnSize(vint index, vint value)
				{
					//////////////////////////////////////
					throw 0;
				}

				GuiMenu* GetDropdownPopup(vint index)
				{
					return 0;
				}

				GuiListViewColumnHeader::ColumnSortingState GetSortingState(vint index)
				{
					return GuiListViewColumnHeader::NotSorted;
				}
			};

/***********************************************************************
GuiVirtualDataGrid
***********************************************************************/

			GuiVirtualDataGrid::GuiVirtualDataGrid(IStyleProvider* _styleProvider, IDataProvider* _dataProvider)
				:GuiVirtualListView(_styleProvider, new GuiVirtualDataGridItemProvider(_dataProvider))
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