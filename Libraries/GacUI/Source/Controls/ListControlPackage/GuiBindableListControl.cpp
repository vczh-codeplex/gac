#include "GuiBindableListControl.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace list;
			using namespace tree;
			using namespace reflection::description;

			Value ReadProperty(const Value& thisObject, const WString& propertyName)
			{
				if (!thisObject.IsNull() && propertyName != L"")
				{
					auto td = thisObject.GetTypeDescriptor();
					auto info = td->GetPropertyByName(propertyName, true);
					if (info && info->IsReadable())
					{
						return info->GetValue(thisObject);
					}
					else
					{
						return Value();
					}
				}
				return thisObject;
			}

			void WriteProperty(Value& thisObject, const WString& propertyName, const Value& newValue)
			{
				if (!thisObject.IsNull() && propertyName != L"")
				{
					auto td = thisObject.GetTypeDescriptor();
					auto info = td->GetPropertyByName(propertyName, true);
					if (info && info->IsWritable())
					{
						info->SetValue(thisObject, newValue);
					}
				}
			}

/***********************************************************************
GuiBindableTextList::ItemSource
***********************************************************************/

			GuiBindableTextList::ItemSource::ItemSource(Ptr<description::IValueEnumerable> _itemSource)
			{
				if (auto ol = _itemSource.Cast<IValueObservableList>())
				{
					itemSource = ol;
					itemChangedEventHandler = ol->ItemChanged.Add([this](vint start, vint oldCount, vint newCount)
					{
						InvokeOnItemModified(start, oldCount, newCount);
					});
				}
				else if (auto rl = _itemSource.Cast<IValueReadonlyList>())
				{
					itemSource = rl;
				}
				else
				{
					itemSource = IValueList::Create(GetLazyList<Value>(_itemSource));
				}
			}

			GuiBindableTextList::ItemSource::~ItemSource()
			{
				if (itemChangedEventHandler)
				{
					auto ol = itemSource.Cast<IValueObservableList>();
					ol->ItemChanged.Remove(itemChangedEventHandler);
				}
			}

			description::Value GuiBindableTextList::ItemSource::Get(vint index)
			{
				return itemSource->Get(index);
			}

			void GuiBindableTextList::ItemSource::UpdateBindingProperties()
			{
				InvokeOnItemModified(0, Count(), Count());
			}
					
			// ===================== GuiListControl::IItemProvider =====================
			
			vint GuiBindableTextList::ItemSource::Count()
			{
				return itemSource->GetCount();
			}
			
			IDescriptable* GuiBindableTextList::ItemSource::RequestView(const WString& identifier)
			{
				if (identifier == GuiListControl::IItemPrimaryTextView::Identifier)
				{
					return (GuiListControl::IItemPrimaryTextView*)this;
				}
				else if (identifier == TextItemStyleProvider::ITextItemView::Identifier)
				{
					return (TextItemStyleProvider::ITextItemView*)this;
				}
				else
				{
					return 0;
				}
			}
			
			void GuiBindableTextList::ItemSource::ReleaseView(IDescriptable* view)
			{
			}
					
			// ===================== GuiListControl::IItemPrimaryTextView =====================

			WString GuiBindableTextList::ItemSource::GetPrimaryTextViewText(vint itemIndex)
			{
				return GetText(itemIndex);
			}
			
			bool GuiBindableTextList::ItemSource::ContainsPrimaryText(vint itemIndex)
			{
				return 0 <= itemIndex && itemIndex < itemSource->GetCount();
			}
					
			// ===================== list::TextItemStyleProvider::ITextItemView =====================

			WString GuiBindableTextList::ItemSource::GetText(vint itemIndex)
			{
				if (0 <= itemIndex && itemIndex < itemSource->GetCount())
				{
					return ReadProperty(itemSource->Get(itemIndex), textProperty).GetText();
				}
				return L"";
			}
			
			bool GuiBindableTextList::ItemSource::GetChecked(vint itemIndex)
			{
				if (0 <= itemIndex && itemIndex < itemSource->GetCount())
				{
					auto value = ReadProperty(itemSource->Get(itemIndex), checkedProperty);
					if (value.GetTypeDescriptor() == description::GetTypeDescriptor<bool>())
					{
						return UnboxValue<bool>(value);
					}
				}
				return false;
			}
			
			void GuiBindableTextList::ItemSource::SetCheckedSilently(vint itemIndex, bool value)
			{
				if (0 <= itemIndex && itemIndex < itemSource->GetCount())
				{
					WriteProperty(itemSource->Get(itemIndex), checkedProperty, BoxValue(value));
				}
			}

/***********************************************************************
GuiBindableTextList
***********************************************************************/

			GuiBindableTextList::GuiBindableTextList(IStyleProvider* _styleProvider, list::TextItemStyleProvider::ITextItemStyleProvider* _itemStyleProvider, Ptr<description::IValueEnumerable> _itemSource)
				:GuiVirtualTextList(_styleProvider, _itemStyleProvider, new ItemSource(_itemSource))
			{
				itemSource = dynamic_cast<ItemSource*>(GetItemProvider());

				TextPropertyChanged.SetAssociatedComposition(boundsComposition);
				TextPropertyChanged.SetAssociatedComposition(boundsComposition);
			}

			GuiBindableTextList::~GuiBindableTextList()
			{
			}

			const WString& GuiBindableTextList::GetTextProperty()
			{
				return itemSource->textProperty;
			}

			void GuiBindableTextList::SetTextProperty(const WString& value)
			{
				if (itemSource->textProperty != value)
				{
					itemSource->textProperty = value;
					itemSource->UpdateBindingProperties();
					TextPropertyChanged.Execute(GetNotifyEventArguments());
				}
			}

			const WString& GuiBindableTextList::GetCheckedProperty()
			{
				return itemSource->checkedProperty;
			}

			void GuiBindableTextList::SetCheckedProperty(const WString& value)
			{
				if (itemSource->checkedProperty != value)
				{
					itemSource->checkedProperty = value;
					itemSource->UpdateBindingProperties();
					CheckedPropertyChanged.Execute(GetNotifyEventArguments());
				}
			}

			description::Value GuiBindableTextList::GetSelectedItem()
			{
				vint index = GetSelectedItemIndex();
				if (index == -1) return Value();
				return itemSource->Get(index);
			}

/***********************************************************************
GuiBindableListView::ListViewDataColumns
***********************************************************************/

			void GuiBindableListView::ListViewDataColumns::NotifyUpdateInternal(vint start, vint count, vint newCount)
			{
				itemProvider->NotifyUpdate(0, itemProvider->Count());
			}

			GuiBindableListView::ListViewDataColumns::ListViewDataColumns()
				:itemProvider(0)
			{
			}

			GuiBindableListView::ListViewDataColumns::~ListViewDataColumns()
			{
			}

/***********************************************************************
GuiBindableListView::ListViewColumns
***********************************************************************/

			void GuiBindableListView::ListViewColumns::NotifyUpdateInternal(vint start, vint count, vint newCount)
			{
				for(vint i=0;i<itemProvider->columnItemViewCallbacks.Count();i++)
				{
					itemProvider->columnItemViewCallbacks[i]->OnColumnChanged();
				}
				itemProvider->NotifyUpdate(0, itemProvider->Count());
			}

			GuiBindableListView::ListViewColumns::ListViewColumns()
				:itemProvider(0)
			{
			}

			GuiBindableListView::ListViewColumns::~ListViewColumns()
			{
			}

/***********************************************************************
GuiBindableListView::ItemSource
***********************************************************************/

			GuiBindableListView::ItemSource::ItemSource(Ptr<description::IValueEnumerable> itemSource)
			{
				columns.itemProvider = this;
				dataColumns.itemProvider = this;
			}

			GuiBindableListView::ItemSource::~ItemSource()
			{
			}

			bool GuiBindableListView::ItemSource::NotifyUpdate(vint start, vint count)
			{
				if (start<0 || start >= itemSource->GetCount() || count <= 0 || start + count > itemSource->GetCount())
				{
					return false;
				}
				else
				{
					InvokeOnItemModified(start, count, count);
					return true;
				}
			}

			GuiBindableListView::ListViewDataColumns& GuiBindableListView::ItemSource::GetDataColumns()
			{
				return dataColumns;
			}

			GuiBindableListView::ListViewColumns& GuiBindableListView::ItemSource::GetColumns()
			{
				return columns;
			}

			// ===================== GuiListControl::IItemProvider =====================

			vint GuiBindableListView::ItemSource::Count()
			{
				return itemSource->GetCount();
			}

			IDescriptable* GuiBindableListView::ItemSource::RequestView(const WString& identifier)
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

			void GuiBindableListView::ItemSource::ReleaseView(IDescriptable* view)
			{
			}

			// ===================== GuiListControl::IItemPrimaryTextView =====================

			WString GuiBindableListView::ItemSource::GetPrimaryTextViewText(vint itemIndex)
			{
				return GetText(itemIndex);
			}

			bool GuiBindableListView::ItemSource::ContainsPrimaryText(vint itemIndex)
			{
				return true;
			}

			// ===================== list::ListViewItemStyleProvider::IListViewItemView =====================

			Ptr<GuiImageData> GuiBindableListView::ItemSource::GetSmallImage(vint itemIndex)
			{
				throw 0;
			}

			Ptr<GuiImageData> GuiBindableListView::ItemSource::GetLargeImage(vint itemIndex)
			{
				throw 0;
			}

			WString GuiBindableListView::ItemSource::GetText(vint itemIndex)
			{
				throw 0;
			}

			WString GuiBindableListView::ItemSource::GetSubItem(vint itemIndex, vint index)
			{
				throw 0;
			}

			vint GuiBindableListView::ItemSource::GetDataColumnCount()
			{
				return dataColumns.Count();
			}

			vint GuiBindableListView::ItemSource::GetDataColumn(vint index)
			{
				return dataColumns[index];
			}

			// ===================== list::ListViewColumnItemArranger::IColumnItemView =====================

			bool GuiBindableListView::ItemSource::AttachCallback(ListViewColumnItemArranger::IColumnItemViewCallback* value)
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

			bool GuiBindableListView::ItemSource::DetachCallback(ListViewColumnItemArranger::IColumnItemViewCallback* value)
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

			vint GuiBindableListView::ItemSource::GetColumnCount()
			{
				return columns.Count();
			}

			WString GuiBindableListView::ItemSource::GetColumnText(vint index)
			{
				if(index<0 || index>=columns.Count())
				{
					return L"";
				}
				else
				{
					return columns[index]->text;
				}
			}

			vint GuiBindableListView::ItemSource::GetColumnSize(vint index)
			{
				if(index<0 || index>=columns.Count())
				{
					return 0;
				}
				else
				{
					return columns[index]->size;
				}
			}

			void GuiBindableListView::ItemSource::SetColumnSize(vint index, vint value)
			{
				if(index>=0 && index<columns.Count())
				{
					columns[index]->size=value;
					for(vint i=0;i<columnItemViewCallbacks.Count();i++)
					{
						columnItemViewCallbacks[i]->OnColumnChanged();
					}
				}
			}

			GuiMenu* GuiBindableListView::ItemSource::GetDropdownPopup(vint index)
			{
				if(index<0 || index>=columns.Count())
				{
					return 0;
				}
				else
				{
					return columns[index]->dropdownPopup;
				}
			}

			GuiListViewColumnHeader::ColumnSortingState GuiBindableListView::ItemSource::GetSortingState(vint index)
			{
				if(index<0 || index>=columns.Count())
				{
					return GuiListViewColumnHeader::NotSorted;
				}
				else
				{
					return columns[index]->sortingState;
				}
			}

/***********************************************************************
GuiBindableListView
***********************************************************************/

			GuiBindableListView::GuiBindableListView(IStyleProvider* _styleProvider, Ptr<description::IValueEnumerable> _itemSource)
				:GuiVirtualListView(_styleProvider, new ItemSource(_itemSource))
			{
				itemSource = dynamic_cast<ItemSource*>(GetItemProvider());
			}

			GuiBindableListView::~GuiBindableListView()
			{
			}

			GuiBindableListView::ListViewDataColumns& GuiBindableListView::GetDataColumns()
			{
				return itemSource->GetDataColumns();
			}

			GuiBindableListView::ListViewColumns& GuiBindableListView::GetColumns()
			{
				return itemSource->GetColumns();
			}

/***********************************************************************
GuiBindableTreeView::ItemSource
***********************************************************************/

			GuiBindableTreeView::ItemSource::ItemSource(Ptr<description::IValueEnumerable> _itemSource)
			{
				throw 0;
			}

			GuiBindableTreeView::ItemSource::~ItemSource()
			{
				throw 0;
			}

			// ===================== tree::INodeRootProvider =====================

			tree::INodeProvider* GuiBindableTreeView::ItemSource::GetRootNode()
			{
				throw 0;
			}

			bool GuiBindableTreeView::ItemSource::CanGetNodeByVisibleIndex()
			{
				throw 0;
			}

			tree::INodeProvider* GuiBindableTreeView::ItemSource::GetNodeByVisibleIndex(vint index)
			{
				throw 0;
			}

			bool GuiBindableTreeView::ItemSource::AttachCallback(tree::INodeProviderCallback* value)
			{
				throw 0;
			}

			bool GuiBindableTreeView::ItemSource::DetachCallback(tree::INodeProviderCallback* value)
			{
				throw 0;
			}

			IDescriptable* GuiBindableTreeView::ItemSource::RequestView(const WString& identifier)
			{
				throw 0;
			}

			void GuiBindableTreeView::ItemSource::ReleaseView(IDescriptable* view)
			{
				throw 0;
			}

			// ===================== tree::INodeItemPrimaryTextView =====================

			WString GuiBindableTreeView::ItemSource::GetPrimaryTextViewText(tree::INodeProvider* node)
			{
				throw 0;
			}

			// ===================== tree::ITreeViewItemView =====================

			Ptr<GuiImageData> GuiBindableTreeView::ItemSource::GetNodeImage(tree::INodeProvider* node)
			{
				throw 0;
			}

			WString GuiBindableTreeView::ItemSource::GetNodeText(tree::INodeProvider* node)
			{
				throw 0;
			}

/***********************************************************************
GuiBindableTreeView
***********************************************************************/

			GuiBindableTreeView::GuiBindableTreeView(IStyleProvider* _styleProvider, Ptr<description::IValueEnumerable> _itemSource)
				:GuiVirtualTreeView(_styleProvider, new ItemSource(_itemSource))
			{
				itemSource = dynamic_cast<ItemSource*>(GetNodeRootProvider());
			}

			GuiBindableTreeView::~GuiBindableTreeView()
			{
			}

/***********************************************************************
GuiBindableDataGrid::ItemSource
***********************************************************************/

			GuiBindableDataGrid::ItemSource::ItemSource(Ptr<description::IValueEnumerable> itemSource)
			{
				throw 0;
			}

			GuiBindableDataGrid::ItemSource::~ItemSource()
			{
				throw 0;
			}

			// ===================== list::IDataProvider =====================

			void GuiBindableDataGrid::ItemSource::SetCommandExecutor(list::IDataProviderCommandExecutor* value)
			{
				throw 0;
			}

			vint GuiBindableDataGrid::ItemSource::GetColumnCount()
			{
				throw 0;
			}

			WString GuiBindableDataGrid::ItemSource::GetColumnText(vint column)
			{
				throw 0;
			}

			vint GuiBindableDataGrid::ItemSource::GetColumnSize(vint column)
			{
				throw 0;
			}

			void GuiBindableDataGrid::ItemSource::SetColumnSize(vint column, vint value)
			{
				throw 0;
			}

			GuiMenu* GuiBindableDataGrid::ItemSource::GetColumnPopup(vint column)
			{
				throw 0;
			}

			bool GuiBindableDataGrid::ItemSource::IsColumnSortable(vint column)
			{
				throw 0;
			}

			void GuiBindableDataGrid::ItemSource::SortByColumn(vint column, bool ascending)
			{
				throw 0;
			}

			vint GuiBindableDataGrid::ItemSource::GetSortedColumn()
			{
				throw 0;
			}

			bool GuiBindableDataGrid::ItemSource::IsSortOrderAscending()
			{
				throw 0;
			}

			vint GuiBindableDataGrid::ItemSource::GetRowCount()
			{
				throw 0;
			}

			Ptr<GuiImageData> GuiBindableDataGrid::ItemSource::GetRowLargeImage(vint row)
			{
				throw 0;
			}

			Ptr<GuiImageData> GuiBindableDataGrid::ItemSource::GetRowSmallImage(vint row)
			{
				throw 0;
			}

			vint GuiBindableDataGrid::ItemSource::GetCellSpan(vint row, vint column)
			{
				throw 0;
			}

			WString GuiBindableDataGrid::ItemSource::GetCellText(vint row, vint column)
			{
				throw 0;
			}

			list::IDataVisualizerFactory* GuiBindableDataGrid::ItemSource::GetCellDataVisualizerFactory(vint row, vint column)
			{
				throw 0;
			}

			void GuiBindableDataGrid::ItemSource::VisualizeCell(vint row, vint column, list::IDataVisualizer* dataVisualizer)
			{
				throw 0;
			}

			list::IDataEditorFactory* GuiBindableDataGrid::ItemSource::GetCellDataEditorFactory(vint row, vint column)
			{
				throw 0;
			}

			void GuiBindableDataGrid::ItemSource::BeforeEditCell(vint row, vint column, list::IDataEditor* dataEditor)
			{
				throw 0;
			}

			void GuiBindableDataGrid::ItemSource::SaveCellData(vint row, vint column, list::IDataEditor* dataEditor)
			{
				throw 0;
			}

/***********************************************************************
GuiBindableDataGrid
***********************************************************************/

			GuiBindableDataGrid::GuiBindableDataGrid(IStyleProvider* _styleProvider, Ptr<description::IValueEnumerable> _itemSource)
				:GuiVirtualDataGrid(_styleProvider, new ItemSource(_itemSource))
			{
				itemSource = dynamic_cast<ItemSource*>(GetDataProvider());
			}

			GuiBindableDataGrid::~GuiBindableDataGrid()
			{
			}
		}
	}
}