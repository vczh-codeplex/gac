#include "GuiDataGridControls.h"
#include "..\Styles\GuiThemeStyleFactory.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace collections;
			using namespace elements;
			using namespace compositions;
			using namespace theme;

			namespace list
			{

				const wchar_t* const IDataProvider::Identifier = L"vl::presentation::controls::list::IDataProvider";
				
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
					if(filters.Contains(value.Obj())) return false;
					filters.Add(value);
					InvokeOnFilterChanged();
					return true;
				}

				bool StructuredDataMultipleFilter::RemoveSubFilter(Ptr<IStructuredDataFilter> value)
				{
					if(!filters.Contains(value.Obj())) return false;
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
					filter=value;
					InvokeOnFilterChanged();
					return true;
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
StructuredDataProvider
***********************************************************************/

				vint StructuredDataProvider::TranslateRowNumber(vint row)
				{
					return row;
				}

				StructuredDataProvider::StructuredDataProvider(Ptr<IStructuredDataProvider> provider)
					:structuredDataProvider(provider)
					,commandExecutor(0)
				{
				}

				StructuredDataProvider::~StructuredDataProvider()
				{
				}

				void StructuredDataProvider::SetCommandExecutor(IDataProviderCommandExecutor* value)
				{
					commandExecutor=value;
					structuredDataProvider->SetCommandExecutor(commandExecutor);
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
					throw 0;
				}

				void StructuredDataProvider::SetColumnSize(vint column, vint value)
				{
					throw 0;
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
					throw 0;
				}
					
				vint StructuredDataProvider::GetRowCount()
				{
					return structuredDataProvider->GetRowCount();
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
					return structuredDataProvider->GetColumn(column)->VisualizeCell(TranslateRowNumber(row), dataVisualizer);
				}

				IDataEditorFactory* StructuredDataProvider::GetCellDataEditorFactory(vint row, vint column)
				{
					return structuredDataProvider->GetColumn(column)->GetCellDataEditorFactory(TranslateRowNumber(row));
				}

				void StructuredDataProvider::BeforeEditCell(vint row, vint column, IDataEditor* dataEditor)
				{
					return structuredDataProvider->GetColumn(column)->BeforeEditCell(TranslateRowNumber(row), dataEditor);
				}

				void StructuredDataProvider::SaveCellData(vint row, vint column, IDataEditor* dataEditor)
				{
					return structuredDataProvider->GetColumn(column)->SaveCellData(TranslateRowNumber(row), dataEditor);
				}
				
/***********************************************************************
DataVisualizerBase
***********************************************************************/

				DataVisualizerBase::DataVisualizerBase()
					:factory(0)
					,styleProvider(0)
					,boundsComposition(0)
				{
				}

				DataVisualizerBase::~DataVisualizerBase()
				{
					if(boundsComposition)
					{
						SafeDeleteComposition(boundsComposition);
					}
				}

				IDataVisualizerFactory* DataVisualizerBase::GetFactory()
				{
					return factory;
				}

				compositions::GuiBoundsComposition* DataVisualizerBase::GetBoundsComposition()
				{
					if(!boundsComposition)
					{
						boundsComposition=CreateBoundsCompositionInternal();
					}
					return boundsComposition;
				}

				void DataVisualizerBase::BeforeVisualizerCell(IDataProvider* dataProvider, vint row, vint column)
				{
				}
				
/***********************************************************************
ListViewMainColumnDataVisualizer
***********************************************************************/

				compositions::GuiBoundsComposition* ListViewMainColumnDataVisualizer::CreateBoundsCompositionInternal()
				{
					GuiTableComposition* table=new GuiTableComposition;
					table->SetRowsAndColumns(3, 2);
					table->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
					table->SetRowOption(1, GuiCellOption::MinSizeOption());
					table->SetRowOption(2, GuiCellOption::PercentageOption(0.5));
					table->SetColumnOption(0, GuiCellOption::MinSizeOption());
					table->SetColumnOption(1, GuiCellOption::PercentageOption(1.0));
					table->SetAlignmentToParent(Margin(0, 0, 0, 0));
					table->SetCellPadding(2);
					{
						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetSite(1, 0, 1, 1);
						cell->SetPreferredMinSize(Size(16, 16));

						image=GuiImageFrameElement::Create();
						image->SetStretch(true);
						cell->SetOwnedElement(image);
					}
					{
						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetSite(0, 1, 3, 1);
						cell->SetMargin(Margin(0, 0, 8, 0));

						text=GuiSolidLabelElement::Create();
						text->SetAlignments(Alignment::Left, Alignment::Center);
						text->SetFont(font);
						text->SetColor(styleProvider->GetPrimaryTextColor());
						text->SetEllipse(true);
						cell->SetOwnedElement(text);
					}
					return table;
				}

				ListViewMainColumnDataVisualizer::ListViewMainColumnDataVisualizer()
					:image(0)
					,text(0)
				{
				}

				void ListViewMainColumnDataVisualizer::BeforeVisualizerCell(IDataProvider* dataProvider, vint row, vint column)
				{
					Ptr<GuiImageData> imageData=dataProvider->GetRowImage(row);
					if(imageData)
					{
						image->SetImage(imageData->GetImage(), imageData->GetFrameIndex());
					}
					else
					{
						image->SetImage(0);
					}
					text->SetText(dataProvider->GetCellText(row, column));
				}
				
/***********************************************************************
ListViewSubColumnDataVisualizer
***********************************************************************/

				compositions::GuiBoundsComposition* ListViewSubColumnDataVisualizer::CreateBoundsCompositionInternal()
				{
					text=GuiSolidLabelElement::Create();
					text->SetAlignments(Alignment::Left, Alignment::Center);
					text->SetFont(font);
					text->SetColor(styleProvider->GetSecondaryTextColor());
					text->SetEllipse(true);

					GuiBoundsComposition* composition=new GuiBoundsComposition;
					composition->SetMargin(Margin(8, 0, 8, 0));
					composition->SetOwnedElement(text);
					return composition;
				}

				ListViewSubColumnDataVisualizer::ListViewSubColumnDataVisualizer()
					:text(0)
				{
				}

				void ListViewSubColumnDataVisualizer::BeforeVisualizerCell(IDataProvider* dataProvider, vint row, vint column)
				{
					text->SetText(dataProvider->GetCellText(row, column));
				}
				
/***********************************************************************
DataEditorBase
***********************************************************************/

				DataEditorBase::DataEditorBase()
					:factory(0)
					,callback(0)
					,boundsComposition(0)
				{
				}

				DataEditorBase::~DataEditorBase()
				{
					if(boundsComposition)
					{
						SafeDeleteComposition(boundsComposition);
					}
				}

				IDataEditorFactory* DataEditorBase::GetFactory()
				{
					return factory;
				}

				compositions::GuiBoundsComposition* DataEditorBase::GetBoundsComposition()
				{
					if(!boundsComposition)
					{
						boundsComposition=CreateBoundsCompositionInternal();
					}
					return boundsComposition;
				}

				void DataEditorBase::BeforeEditCell(IDataProvider* dataProvider, vint row, vint column)
				{
				}
				
/***********************************************************************
DataTextBoxEditor
***********************************************************************/

				compositions::GuiBoundsComposition* DataTextBoxEditor::CreateBoundsCompositionInternal()
				{
					return textBox->GetBoundsComposition();
				}

				DataTextBoxEditor::DataTextBoxEditor()
				{
					textBox=g::NewTextBox();
				}

				void DataTextBoxEditor::BeforeEditCell(IDataProvider* dataProvider, vint row, vint column)
				{
					DataEditorBase::BeforeEditCell(dataProvider, row, column);
					textBox->SetText(L"");
				}

				GuiSinglelineTextBox* DataTextBoxEditor::GetTextBox()
				{
					return textBox;
				}
				
/***********************************************************************
DataTextComboBoxEditor
***********************************************************************/

				compositions::GuiBoundsComposition* DataTextComboBoxEditor::CreateBoundsCompositionInternal()
				{
					return comboBox->GetBoundsComposition();
				}

				DataTextComboBoxEditor::DataTextComboBoxEditor()
				{
					textList=g::NewTextList();
					comboBox=g::NewComboBox(textList);
				}

				void DataTextComboBoxEditor::BeforeEditCell(IDataProvider* dataProvider, vint row, vint column)
				{
					DataEditorBase::BeforeEditCell(dataProvider, row, column);
					textList->GetItems().Clear();
				}

				GuiComboBoxListControl* DataTextComboBoxEditor::GetComboBoxControl()
				{
					return comboBox;
				}

				GuiTextList* DataTextComboBoxEditor::GetTextListControl()
				{
					return textList;
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

				void DataGridItemProvider::OnDataProviderColumnChanged()
				{
					InvokeOnColumnChanged();
				}

				void DataGridItemProvider::OnDataProviderItemModified(vint start, vint count, vint newCount)
				{
					InvokeOnItemModified(start, count, newCount);
				}

				DataGridItemProvider::DataGridItemProvider(IDataProvider* _dataProvider)
					:dataProvider(_dataProvider)
					,sortingColumn(-1)
					,sortingColumnAscending(true)
				{
					dataProvider->SetCommandExecutor(this);
				}

				DataGridItemProvider::~DataGridItemProvider()
				{
				}

				void DataGridItemProvider::SortByColumn(vint column, bool ascending)
				{
					sortingColumn=column;
					sortingColumnAscending=ascending;
					dataProvider->SortByColumn(column, ascending);
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
					return dataProvider->GetColumnPopup(index);
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
DataGridContentProvider::ItemContent
***********************************************************************/

				void DataGridContentProvider::ItemContent::RemoveCellsAndDataVisualizers()
				{
					for(vint i=0;i<dataVisualizers.Count();i++)
					{
						IDataVisualizer* visualizer=dataVisualizers[i].Obj();
						GuiGraphicsComposition* composition=visualizer->GetBoundsComposition();
						if(composition->GetParent())
						{
							composition->GetParent()->RemoveChild(composition);
						}
						dataVisualizers[i]=0;
					}
					dataVisualizers.Resize(0);

					for(vint i=0;i<textTable->GetColumns();i++)
					{
						GuiCellComposition* cell=textTable->GetSitedCell(0, i);
						SafeDeleteComposition(cell);
					}
				}

				IDataVisualizerFactory* DataGridContentProvider::ItemContent::GetDataVisualizerFactory(vint row, vint column)
				{
					IDataVisualizerFactory* factory=contentProvider->dataProvider->GetCellDataVisualizerFactory(row, column);
					if(factory)
					{
						return factory;
					}
					else if(column==0)
					{
						return &contentProvider->mainColumnDataVisualizerFactory;
					}
					else
					{
						return &contentProvider->subColumnDataVisualizerFactory;
					}
				}

				vint DataGridContentProvider::ItemContent::GetCellColumnIndex(compositions::GuiGraphicsComposition* composition)
				{
					for(vint i=0;i<textTable->GetColumns();i++)
					{
						GuiCellComposition* cell=textTable->GetSitedCell(0, i);
						if(composition==cell)
						{
							return i;
						}
					}
					return -1;
				}

				void DataGridContentProvider::ItemContent::OnCellMouseUp(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments)
				{
					vint index=GetCellColumnIndex(sender);
					if(index!=-1)
					{
						if(currentEditor && contentProvider->editingColumn==index)
						{
							return;
						}
						IDataEditorFactory* factory=contentProvider->dataProvider->GetCellDataEditorFactory(currentRow, index);
						currentEditor=contentProvider->OpenEditor(currentRow, index, factory);
						if(currentEditor)
						{
							GuiCellComposition* cell=dynamic_cast<GuiCellComposition*>(sender);
							currentEditor->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
							cell->AddChild(currentEditor->GetBoundsComposition());
						}
					}
				}

				DataGridContentProvider::ItemContent::ItemContent(DataGridContentProvider* _contentProvider, const FontProperties& _font)
					:contentComposition(0)
					,contentProvider(_contentProvider)
					,font(_font)
					,currentRow(-1)
					,currentEditor(0)
				{
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
					RemoveCellsAndDataVisualizers();
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
					vint columnCount=contentProvider->columnItemView->GetColumnCount();
					for(vint i=0;i<columnCount;i++)
					{
						textTable->SetColumnOption(i, GuiCellOption::AbsoluteOption(contentProvider->columnItemView->GetColumnSize(i)));
					}
					textTable->UpdateCellBounds();
				}

				void DataGridContentProvider::ItemContent::NotifyCloseEditor()
				{
					if(currentEditor)
					{
						GuiGraphicsComposition* composition=currentEditor->GetBoundsComposition();
						if(composition->GetParent())
						{
							composition->GetParent()->RemoveChild(composition);
						}
						currentEditor=0;
					}
				}

				void DataGridContentProvider::ItemContent::Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, vint itemIndex)
				{
					currentRow=itemIndex;
					bool refresh=false;
					if(dataVisualizers.Count()!=contentProvider->columnItemView->GetColumnCount())
					{
						refresh=true;
					}
					else
					{
						for(vint i=0;i<dataVisualizers.Count();i++)
						{
							IDataVisualizerFactory* factory=dataVisualizers[i]->GetFactory();
							if(factory!=GetDataVisualizerFactory(itemIndex, i))
							{
								refresh=true;
								break;
							}
						}
					}

					if(refresh)
					{
						RemoveCellsAndDataVisualizers();
						vint columnCount=contentProvider->columnItemView->GetColumnCount();

						dataVisualizers.Resize(columnCount);
						for(vint i=0;i<dataVisualizers.Count();i++)
						{
							IDataVisualizerFactory* factory=GetDataVisualizerFactory(itemIndex, i);
							dataVisualizers[i]=factory->CreateVisualizer(font, styleProvider);
						}

						textTable->SetRowsAndColumns(1, columnCount);
						for(vint i=0;i<columnCount;i++)
						{
							GuiCellComposition* cell=new GuiCellComposition;
							textTable->AddChild(cell);
							cell->SetSite(0, i, 1, 1);
							cell->GetEventReceiver()->leftButtonUp.AttachMethod(this, &ItemContent::OnCellMouseUp);

							GuiBoundsComposition* composition=dataVisualizers[i]->GetBoundsComposition();
							composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
							cell->AddChild(composition);
						}
					}

					for(vint i=0;i<dataVisualizers.Count();i++)
					{
						IDataVisualizer* dataVisualizer=dataVisualizers[i].Obj();
						dataVisualizer->BeforeVisualizerCell(contentProvider->dataProvider, itemIndex, i);
						contentProvider->dataProvider->VisualizeCell(itemIndex, i, dataVisualizer);
					}
					UpdateSubItemSize();
				}

				void DataGridContentProvider::ItemContent::Uninstall()
				{
					if(currentEditor)
					{
						contentProvider->CloseEditor();
					}
					currentRow=-1;
					currentEditor=0;
				}
				
/***********************************************************************
DataGridContentProvider
***********************************************************************/

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

				void DataGridContentProvider::OnAttached(GuiListControl::IItemProvider* provider)
				{
				}

				void DataGridContentProvider::OnItemModified(vint start, vint count, vint newCount)
				{
					if(!currentEditorRequestingSaveData)
					{
						CloseEditor();
					}
				}

				void DataGridContentProvider::NotifyCloseEditor()
				{
					if(listViewItemStyleProvider)
					{
						vint count=listViewItemStyleProvider->GetCreatedItemStyles().Count();
						for(vint i=0;i<count;i++)
						{
							GuiListControl::IItemStyleController* itemStyleController=listViewItemStyleProvider->GetCreatedItemStyles().Get(i);
							ItemContent* itemContent=listViewItemStyleProvider->GetItemContent<ItemContent>(itemStyleController);
							if(itemContent)
							{
								itemContent->NotifyCloseEditor();
							}
						}
					}
				}

				void DataGridContentProvider::RequestSaveData()
				{
					if(currentEditor)
					{
						currentEditorRequestingSaveData=true;
						dataProvider->SaveCellData(editingRow, editingColumn, currentEditor.Obj());
						currentEditorRequestingSaveData=false;
					}
				}

				IDataEditor* DataGridContentProvider::OpenEditor(vint row, vint column, IDataEditorFactory* editorFactory)
				{
					CloseEditor();
					if(editorFactory)
					{
						editingRow=row;
						editingColumn=column;
						currentEditor=editorFactory->CreateEditor(this);
						currentEditor->BeforeEditCell(dataProvider, row, column);
						dataProvider->BeforeEditCell(row, column, currentEditor.Obj());
					}
					return currentEditor.Obj();
				}

				void DataGridContentProvider::CloseEditor()
				{
					if(currentEditor)
					{
						NotifyCloseEditor();

						editingRow=-1;
						editingColumn=-1;
						currentEditor=0;
					}
				}

				DataGridContentProvider::DataGridContentProvider()
					:itemProvider(0)
					,columnItemView(0)
					,dataProvider(0)
					,listViewItemStyleProvider(0)
					,editingRow(-1)
					,editingColumn(-1)
					,currentEditorRequestingSaveData(false)
				{
				}

				DataGridContentProvider::~DataGridContentProvider()
				{
					CloseEditor();
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
					return new ItemContent(this, font);
				}

				void DataGridContentProvider::AttachListControl(GuiListControl* value)
				{
					listViewItemStyleProvider=dynamic_cast<ListViewItemStyleProvider*>(value->GetStyleProvider());
					itemProvider=value->GetItemProvider();
					itemProvider->AttachCallback(this);
					columnItemView=dynamic_cast<ListViewColumnItemArranger::IColumnItemView*>(itemProvider->RequestView(ListViewColumnItemArranger::IColumnItemView::Identifier));
					if(columnItemView)
					{
						columnItemView->AttachCallback(this);
					}
					dataProvider=dynamic_cast<IDataProvider*>(itemProvider->RequestView(IDataProvider::Identifier));
				}

				void DataGridContentProvider::DetachListControl()
				{
					dataProvider=0;
					if(columnItemView)
					{
						columnItemView->DetachCallback(this);
						itemProvider->ReleaseView(columnItemView);
						columnItemView=0;
					}
					itemProvider->DetachCallback(this);
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