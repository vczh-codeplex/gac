/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUIDATAGRIDCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUIDATAGRIDCONTROLS

#include "GuiListViewControls.h"
#include "GuiComboControls.h"
#include "GuiTextListControls.h"
#include "..\TextEditorPackage\GuiTextControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
Datagrid Interfaces
***********************************************************************/

			namespace list
			{
				class IDataVisualizerFactory;
				class IDataVisualizer;
				class IDataEditorCallback;
				class IDataEditorFactory;
				class IDataEditor;
				class IDataProviderCommandExecutor;
				class IDataProvider;

				/// <summary>The visualizer factory.</summary>
				class IDataVisualizerFactory : public virtual IDescriptable, public Description<IDataVisualizerFactory>
				{
				public:
					/// <summary>Create a data visualizer.</summary>
					/// <returns>The created data visualizer.</returns>
					/// <param name="font">The font for the list view control.</param>
					/// <param name="styleProvider">The style provider for the list view control.</param>
					virtual Ptr<IDataVisualizer>						CreateVisualizer(const FontProperties& font, GuiListViewBase::IStyleProvider* styleProvider)=0;
				};

				/// <summary>The visualizer for each cell in [T:vl.presentation.controls.GuiVirtualDataGrid].</summary>
				class IDataVisualizer : public virtual IDescriptable, public Description<IDataVisualizer>
				{
				public:
					/// <summary>Get the factory object that creates this visualizer.</summary>
					/// <returns>The factory object.</returns>
					virtual IDataVisualizerFactory*						GetFactory()=0;

					/// <summary>Get the composition that renders the data.. The data visualizer should maintain this bounds composition, and delete it when necessary.</summary>
					/// <returns>The composition.</returns>
					virtual compositions::GuiBoundsComposition*			GetBoundsComposition()=0;

					/// <summary>Called before visualizing a cell.</summary>
					/// <param name="dataProvider">The data provider.</param>
					/// <param name="row">The row number of the cell.</param>
					/// <param name="column">The column number of the cell.</param>
					virtual void										BeforeVisualizerCell(IDataProvider* dataProvider, vint row, vint column)=0;
				};

				/// <summary>The editor callback.</summary>
				class IDataEditorCallback : public virtual IDescriptable, public Description<IDataEditorCallback>
				{
				public:
					/// <summary>Called when the editor needs to save the new data to the data provider.</summary>
					virtual void										RequestSaveData()=0;
				};

				/// <summary>The editor factory.</summary>
				class IDataEditorFactory : public virtual IDescriptable, public Description<IDataEditorFactory>
				{
				public:
					/// <summary>Create a data editor.</summary>
					/// <returns>The created data editor.</returns>
					/// <param name="callback">The callback for the created editor to send notification.</param>
					virtual Ptr<IDataEditor>							CreateEditor(IDataEditorCallback* callback)=0;
				};

				/// <summary>The editor for each cell in [T:vl.presentation.controls.GuiVirtualDataGrid].</summary>
				class IDataEditor : public virtual IDescriptable, public Description<IDataEditor>
				{
				public:
					/// <summary>Get the factory object that creates this editor.</summary>
					/// <returns>The factory object.</returns>
					virtual IDataEditorFactory*							GetFactory()=0;

					/// <summary>Get the composition that holds the editor for a cell. The data editor should maintain this bounds composition, and delete it when necessary.</summary>
					/// <returns>The composition.</returns>
					virtual compositions::GuiBoundsComposition*			GetBoundsComposition()=0;

					/// <summary>Called before editing a cell.</summary>
					/// <param name="dataProvider">The data provider.</param>
					/// <param name="row">The row number of the cell.</param>
					/// <param name="column">The column number of the cell.</param>
					virtual void										BeforeEditCell(IDataProvider* dataProvider, vint row, vint column)=0;
				};

				/// <summary>The command executor for [T:vl.presentation.controls.list.IDataProvider] to send notification.</summary>
				class IDataProviderCommandExecutor : public virtual IDescriptable, public Description<IDataProviderCommandExecutor>
				{
				public:
					/// <summary>Called when any column is changed (inserted, removed, text changed, etc.).</summary>
					virtual void										OnDataProviderColumnChanged()=0;

					/// <summary>Called when items in the data provider is modified.</summary>
					/// <param name="start">The index of the first modified row.</param>
					/// <param name="count">The number of all modified rows.</param>
					/// <param name="newCount">The number of new rows. If rows are inserted or removed, newCount may not equals to count.</param>
					virtual void										OnDataProviderItemModified(vint start, vint count, vint newCount)=0;
				};

				/// <summary>The required <see cref="GuiListControl::IItemProvider"/> view for [T:vl.presentation.controls.GuiVirtualDataGrid].</summary>
				class IDataProvider : public virtual IDescriptable, public Description<IDataProvider>
				{
				public:
					/// <summary>The identifier for this view.</summary>
					static const wchar_t* const							Identifier;
					
					/// <summary>Set the command executor.</summary>
					/// <param name="value">The command executor.</param>
					virtual void										SetCommandExecutor(IDataProviderCommandExecutor* value)=0;
					/// <summary>Get the number of all columns.</summary>
					/// <returns>The number of all columns.</returns>
					virtual vint										GetColumnCount()=0;
					/// <summary>Get the text for the column.</summary>
					/// <returns>The text for the column.</returns>
					/// <param name="column">The index for the column.</param>
					virtual WString										GetColumnText(vint column)=0;
					/// <summary>Get the size for the column.</summary>
					/// <returns>The size for the column.</returns>
					/// <param name="column">The index for the column.</param>
					virtual vint										GetColumnSize(vint column)=0;
					/// <summary>Set the size for the column.</summary>
					/// <param name="column">The index for the column.</param>
					/// <param name="value">The new size for the column.</param>
					virtual void										SetColumnSize(vint column, vint value)=0;
					/// <summary>Get the popup binded to the column.</summary>
					/// <returns>The popup binded to the column.</returns>
					/// <param name="column">The index of the column.</param>
					virtual GuiMenu*									GetColumnPopup(vint column)=0;
					/// <summary>Test is a column sortable.</summary>
					/// <returns>Returns true if this column is sortable.</returns>
					/// <param name="column">The index of the column.</param>
					virtual bool										IsColumnSortable(vint column)=0;
					/// <summary>Set the column sorting state to update the data.</summary>
					/// <param name="column">The index of the column. Set to -1 means go back to the unsorted state.</param>
					/// <param name="ascending">Set to true if the data is sorted in ascending order.</param>
					virtual void										SortByColumn(vint column, bool ascending)=0;
					
					/// <summary>Get the number of all rows.</summary>
					/// <returns>The number of all rows.</returns>
					virtual vint										GetRowCount()=0;
					/// <summary>Get the image for the row.</summary>
					/// <returns>The image.</returns>
					/// <param name="row">The row number.</param>
					virtual Ptr<GuiImageData>							GetRowImage(vint row)=0;
					/// <summary>Get the text for the cell.</summary>
					/// <returns>The text for the cell.</returns>
					/// <param name="row">The row number for the cell.</param>
					/// <param name="column">The column number for the cell.</param>
					virtual WString										GetCellText(vint row, vint column)=0;
					/// <summary>Get the data visualizer factory that creates data visualizers for visualizing the cell.</summary>
					/// <returns>The data visualizer factory. The data grid control to use the predefined data visualizer if this function returns null.</returns>
					/// <param name="row">The row number for the cell.</param>
					/// <param name="column">The column number for the cell.</param>
					virtual IDataVisualizerFactory*						GetCellDataVisualizerFactory(vint row, vint column)=0;
					/// <summary>Called before visualizing the cell.</summary>
					/// <param name="row">The row number for the cell.</param>
					/// <param name="column">The column number for the cell.</param>
					/// <param name="dataVisualizer">The data visualizer to be updated.</param>
					virtual void										VisualizeCell(vint row, vint column, IDataVisualizer* dataVisualizer)=0;
					/// <summary>Get the data editor factory that creates data editors for editing the cell.</summary>
					/// <returns>The data editor factory. Returns null to disable editing.</returns>
					/// <param name="row">The row number for the cell.</param>
					/// <param name="column">The column number for the cell.</param>
					virtual IDataEditorFactory*							GetCellDataEditorFactory(vint row, vint column)=0;
					/// <summary>Called before editing the cell.</summary>
					/// <param name="row">The row number for the cell.</param>
					/// <param name="column">The column number for the cell.</param>
					/// <param name="dataEditor">The data editor.</param>
					virtual void										BeforeEditCell(vint row, vint column, IDataEditor* dataEditor)=0;
					/// <summary>Called when saving data for the editing cell.</summary>
					/// <param name="row">The row number for the cell.</param>
					/// <param name="column">The column number for the cell.</param>
					/// <param name="dataEditor">The data editor.</param>
					virtual void										SaveCellData(vint row, vint column, IDataEditor* dataEditor)=0;
				};

/***********************************************************************
DataSource Extensions
***********************************************************************/

				/// <summary>The command executor for [T:vl.presentation.controls.list.IStructuredDataFilter] to send notification.</summary>
				class IStructuredDataFilterCommandExecutor : public virtual IDescriptable, public Description<IStructuredDataFilterCommandExecutor>
				{
				public:
					/// <summary>Called when the filter structure or arguments are changed.</summary>
					virtual void										OnFilterChanged()=0;
				};

				/// <summary>Structured data filter.</summary>
				class IStructuredDataFilter : public virtual IDescriptable, public Description<IStructuredDataFilter>
				{
				public:
					/// <summary>Set the command executor.</summary>
					/// <param name="value">The command executor.</param>
					virtual void										SetCommandExecutor(IStructuredDataFilterCommandExecutor* value)=0;
					/// <summary>Filter a row.</summary>
					/// <returns>Returns true when a row is going to display on the control.</returns>
					/// <param name="row">The row number.</param>
					virtual bool										Filter(vint row)=0;
				};

				/// <summary>Structured data sorter.</summary>
				class IStructuredDataSorter : public virtual IDescriptable, public Description<IStructuredDataSorter>
				{
				public:
					/// <summary>Get the order of two rows.</summary>
					/// <returns>Returns 0 if the order doesn't matter. Returns negative number if row1 needs to put before row2. Returns positive number if row1 needs to put after row2.</returns>
					/// <param name="row1">The row number of the first row.</param>
					/// <param name="row2">The row number of the second row.</param>
					virtual vint										Compare(vint row1, vint row2)=0;
				};

				/// <summary>Structure data column.</summary>
				class IStructuredColumnProvider : public virtual IDescriptable, public Description<IStructuredColumnProvider>
				{
				public:
					/// <summary>Get the text for the column.</summary>
					/// <returns>The text of the column.</returns>
					virtual WString										GetText()=0;
					/// <summary>Get the popup binded to the column.</summary>
					/// <returns>The popup binded to the column.</returns>
					virtual GuiMenu*									GetPopup()=0;
					/// <summary>Get the inherent filter for the column.</summary>
					/// <returns>The inherent filter. Returns null if the column doesn't have a filter.</returns>
					virtual Ptr<IStructuredDataFilter>					GetInherentFilter()=0;
					/// <summary>Get the inherent sorter for the column.</summary>
					/// <returns>The inherent sorter. Returns null if the column doesn't have a sorter.</returns>
					virtual Ptr<IStructuredDataSorter>					GetInherentSorter()=0;
					
					/// <summary>Get the text for the cell.</summary>
					/// <returns>The text for the cell.</returns>
					/// <param name="row">The row number for the cell.</param>
					virtual WString										GetCellText(vint row)=0;
					/// <summary>Get the data visualizer factory that creates data visualizers for visualizing the cell.</summary>
					/// <returns>The data visualizer factory. The data grid control to use the predefined data visualizer if this function returns null.</returns>
					/// <param name="row">The row number for the cell.</param>
					virtual IDataVisualizerFactory*						GetCellDataVisualizerFactory(vint row)=0;
					/// <summary>Called before visualizing the cell.</summary>
					/// <param name="row">The row number for the cell.</param>
					/// <param name="column">The column number for the cell.</param>
					/// <param name="dataVisualizer">The data visualizer to be updated.</param>
					virtual void										VisualizeCell(vint row, IDataVisualizer* dataVisualizer)=0;
					/// <summary>Get the data editor factory that creates data editors for editing the cell.</summary>
					/// <returns>The data editor factory. Returns null to disable editing.</returns>
					/// <param name="row">The row number for the cell.</param>
					virtual IDataEditorFactory*							GetCellDataEditorFactory(vint row)=0;
					/// <summary>Called before editing the cell.</summary>
					/// <param name="row">The row number for the cell.</param>
					/// <param name="dataEditor">The data editor.</param>
					virtual void										BeforeEditCell(vint row, IDataEditor* dataEditor)=0;
					/// <summary>Called when saving data for the editing cell.</summary>
					/// <param name="row">The row number for the cell.</param>
					/// <param name="dataEditor">The data editor.</param>
					virtual void										SaveCellData(vint row, IDataEditor* dataEditor)=0;
				};

				/// <summary>Structured data provider for [T:vl.presentation.controls.GuiVirtualDataGrid].</summary>
				class IStructuredDataProvider : public virtual IDescriptable, public Description<IStructuredDataProvider>
				{
				public:
					/// <summary>Set the command executor.</summary>
					/// <param name="value">The command executor.</param>
					virtual void										SetCommandExecutor(IDataProviderCommandExecutor* value)=0;
					/// <summary>Get the number of all columns.</summary>
					/// <returns>The number of all columns.</returns>
					virtual vint										GetColumnCount()=0;
					/// <summary>Get the number of all rows.</summary>
					/// <returns>The number of all rows.</returns>
					virtual vint										GetRowCount()=0;
					/// <summary>Get the <see cref="IStructuredColumnProvider"/> object for the column.</summary>
					/// <returns>The <see cref="IStructuredColumnProvider"/> object.</returns>
					/// <param name="column">The column number.</param>
					virtual IStructuredColumnProvider*					GetColumn(vint column)=0;
					/// <summary>Get the image for the row.</summary>
					/// <returns>The image.</returns>
					/// <param name="row">The row number.</param>
					virtual Ptr<GuiImageData>							GetRowImage(vint row)=0;
				};

/***********************************************************************
Filter Extensions
***********************************************************************/

				/// <summary>Base class for <see cref="IStructuredDataFilter"/>.</summary>
				class StructuredDataFilterBase : public Object, public virtual IStructuredDataFilter, public Description<StructuredDataFilterBase>
				{
				protected:
					IStructuredDataFilterCommandExecutor*				commandExecutor;

					/// <summary>Called when the structure or properties for this filter is changed.</summary>
					void												InvokeOnFilterChanged();
				public:
					StructuredDataFilterBase();

					void												SetCommandExecutor(IStructuredDataFilterCommandExecutor* value)override;
				};
				
				/// <summary>Base class for a <see cref="IStructuredDataFilter"/> that contains multiple sub filters.</summary>
				class StructuredDataMultipleFilter : public StructuredDataFilterBase, public Description<StructuredDataMultipleFilter>
				{
				protected:
					collections::List<Ptr<IStructuredDataFilter>>		filters;

				public:
					StructuredDataMultipleFilter();

					/// <summary>Add a sub filter.</summary>
					/// <returns>Returns true if this operation succeeded.</returns>
					/// <param name="value">The sub filter.</param>
					bool												AddSubFilter(Ptr<IStructuredDataFilter> value);
					/// <summary>Remove a sub filter.</summary>
					/// <returns>Returns true if this operation succeeded.</returns>
					/// <param name="value">The sub filter.</param>
					bool												RemoveSubFilter(Ptr<IStructuredDataFilter> value);
					void												SetCommandExecutor(IStructuredDataFilterCommandExecutor* value)override;
				};

				/// <summary>A filter that keep a row if all sub filters agree.</summary>
				class StructuredDataAndFilter : public StructuredDataMultipleFilter, public Description<StructuredDataAndFilter>
				{
				public:
					/// <summary>Create the filter.</summary>
					StructuredDataAndFilter();

					bool												Filter(vint row)override;
				};
				
				/// <summary>A filter that keep a row if one of all sub filters agrees.</summary>
				class StructuredDataOrFilter : public StructuredDataMultipleFilter, public Description<StructuredDataOrFilter>
				{
				public:
					/// <summary>Create the filter.</summary>
					StructuredDataOrFilter();

					bool												Filter(vint row)override;
				};
				
				/// <summary>A filter that keep a row if the sub filter not agrees.</summary>
				class StructuredDataNotFilter : public StructuredDataFilterBase, public Description<StructuredDataNotFilter>
				{
				protected:
					Ptr<IStructuredDataFilter>							filter;
				public:
					/// <summary>Create the filter.</summary>
					StructuredDataNotFilter();
					
					/// <summary>Set a sub filter.</summary>
					/// <returns>Returns true if this operation succeeded.</returns>
					/// <param name="value">The sub filter.</param>
					bool												SetSubFilter(Ptr<IStructuredDataFilter> value);
					bool												Filter(vint row)override;
				};

/***********************************************************************
Sorter Extensions
***********************************************************************/
				
				/// <summary>A multi-level <see cref="IStructuredDataSorter"/>.</summary>
				class StructuredDataMultipleSorter : public Object, public virtual IStructuredDataSorter, public Description<StructuredDataMultipleSorter>
				{
				protected:
					Ptr<IStructuredDataSorter>							leftSorter;
					Ptr<IStructuredDataSorter>							rightSorter;
				public:
					/// <summary>Create the sorter.</summary>
					StructuredDataMultipleSorter();
					
					/// <summary>Set the first sub sorter.</summary>
					/// <returns>Returns true if this operation succeeded.</returns>
					/// <param name="value">The sub sorter.</param>
					bool												SetLeftSorter(Ptr<IStructuredDataSorter> value);
					/// <summary>Set the second sub sorter.</summary>
					/// <returns>Returns true if this operation succeeded.</returns>
					/// <param name="value">The sub sorter.</param>
					bool												SetRightSorter(Ptr<IStructuredDataSorter> value);
					vint												Compare(vint row1, vint row2)override;
				};

/***********************************************************************
Structured DataSource Extensions
***********************************************************************/

/***********************************************************************
Visualizer Extensions
***********************************************************************/
				
				/// <summary>Base class for all data visualizers.</summary>
				class DataVisualizerBase : public Object, public virtual IDataVisualizer
				{
					template<typename T>
					friend class DataVisualizerFactory;
				protected:
					IDataVisualizerFactory*								factory;
					FontProperties										font;
					GuiListViewBase::IStyleProvider*					styleProvider;
					compositions::GuiBoundsComposition*					boundsComposition;

					virtual compositions::GuiBoundsComposition*			CreateBoundsCompositionInternal()=0;
				public:
					/// <summary>Create the data visualizer.</summary>
					DataVisualizerBase();
					~DataVisualizerBase();

					IDataVisualizerFactory*								GetFactory()override;
					compositions::GuiBoundsComposition*					GetBoundsComposition()override;
					void												BeforeVisualizerCell(IDataProvider* dataProvider, vint row, vint column)override;
				};
				
				template<typename TVisualizer>
				class DataVisualizerFactory : public Object, public virtual IDataVisualizerFactory, public Description<DataVisualizerFactory<TVisualizer>>
				{
				public:
					Ptr<IDataVisualizer> CreateVisualizer(const FontProperties& font, GuiListViewBase::IStyleProvider* styleProvider)override
					{
						DataVisualizerBase* dataVisualizer=new TVisualizer;
						dataVisualizer->factory=this;
						dataVisualizer->font=font;
						dataVisualizer->styleProvider=styleProvider;
						return dataVisualizer;
					}
				};

				/// <summary>Data visualizer that displays an image and a text. Use ListViewMainColumnDataVisualizer::Factory as the factory class.</summary>
				class ListViewMainColumnDataVisualizer : public DataVisualizerBase
				{
				public:
					typedef DataVisualizerFactory<ListViewMainColumnDataVisualizer>			Factory;
				protected:
					elements::GuiImageFrameElement*						image;
					elements::GuiSolidLabelElement*						text;

					compositions::GuiBoundsComposition*					CreateBoundsCompositionInternal()override;
				public:
					/// <summary>Create the data visualizer.</summary>
					ListViewMainColumnDataVisualizer();

					void												BeforeVisualizerCell(IDataProvider* dataProvider, vint row, vint column)override;
				};
				
				/// <summary>Data visualizer that displays a text. Use ListViewSubColumnDataVisualizer::Factory as the factory class.</summary>
				class ListViewSubColumnDataVisualizer : public DataVisualizerBase
				{
				public:
					typedef DataVisualizerFactory<ListViewSubColumnDataVisualizer>			Factory;
				protected:
					elements::GuiSolidLabelElement*						text;

					compositions::GuiBoundsComposition*					CreateBoundsCompositionInternal()override;
				public:
					/// <summary>Create the data visualizer.</summary>
					ListViewSubColumnDataVisualizer();

					void												BeforeVisualizerCell(IDataProvider* dataProvider, vint row, vint column)override;
				};

/***********************************************************************
Editor Extensions
***********************************************************************/
				
				/// <summary>Base class for all data editors.</summary>
				class DataEditorBase : public Object, public virtual IDataEditor
				{
					template<typename T>
					friend class DataEditorFactory;
				protected:
					IDataEditorFactory*									factory;
					IDataEditorCallback*								callback;
					compositions::GuiBoundsComposition*					boundsComposition;

					virtual compositions::GuiBoundsComposition*			CreateBoundsCompositionInternal()=0;
				public:
					/// <summary>Create the data editor.</summary>
					DataEditorBase();
					~DataEditorBase();

					IDataEditorFactory*									GetFactory()override;
					compositions::GuiBoundsComposition*					GetBoundsComposition()override;
					void												BeforeEditCell(IDataProvider* dataProvider, vint row, vint column)override;
				};
				
				template<typename TEditor>
				class DataEditorFactory : public Object, public virtual IDataEditorFactory, public Description<DataEditorFactory<TEditor>>
				{
				public:
					Ptr<IDataEditor> CreateEditor(IDataEditorCallback* callback)override
					{
						DataEditorBase* dataEditor=new TEditor;
						dataEditor->factory=this;
						dataEditor->callback=callback;
						return dataEditor;
					}
				};
				
				/// <summary>Data editor that displays a text box. Use DataTextBoxEditor::Factory as the factory class.</summary>
				class DataTextBoxEditor : public DataEditorBase
				{
				public:
					typedef DataEditorFactory<DataTextBoxEditor>							Factory;
				protected:
					GuiSinglelineTextBox*								textBox;

					compositions::GuiBoundsComposition*					CreateBoundsCompositionInternal()override;
				public:
					/// <summary>Create the data editor.</summary>
					DataTextBoxEditor();

					void												BeforeEditCell(IDataProvider* dataProvider, vint row, vint column)override;
					GuiSinglelineTextBox*								GetTextBox();
				};
				
				/// <summary>Data editor that displays a text combo box. Use DataTextComboBoxEditor::Factory as the factory class.</summary>
				class DataTextComboBoxEditor : public DataEditorBase
				{
				public:
					typedef DataEditorFactory<DataTextComboBoxEditor>						Factory;
				protected:
					GuiComboBoxListControl*								comboBox;
					GuiTextList*										textList;

					compositions::GuiBoundsComposition*					CreateBoundsCompositionInternal()override;
				public:
					/// <summary>Create the data editor.</summary>
					DataTextComboBoxEditor();

					void												BeforeEditCell(IDataProvider* dataProvider, vint row, vint column)override;
					GuiComboBoxListControl*								GetComboBoxControl();
					GuiTextList*										GetTextListControl();
				};

/***********************************************************************
Datagrid ItemProvider
***********************************************************************/
				
				/// <summary>Data grid item provider.</summary>
				class DataGridItemProvider
					: public Object
					, public virtual GuiListControl::IItemProvider
					, public virtual GuiListControl::IItemPrimaryTextView
					, public virtual ListViewItemStyleProvider::IListViewItemView
					, public virtual ListViewColumnItemArranger::IColumnItemView
					, protected virtual IDataProviderCommandExecutor
					, public Description<DataGridItemProvider>
				{
				protected:
					IDataProvider*																dataProvider;
					collections::List<GuiListControl::IItemProviderCallback*>					itemProviderCallbacks;
					collections::List<ListViewColumnItemArranger::IColumnItemViewCallback*>		columnItemViewCallbacks;
					vint																		sortingColumn;
					bool																		sortingColumnAscending;

					void												InvokeOnItemModified(vint start, vint count, vint newCount);
					void												InvokeOnColumnChanged();
					void												OnDataProviderColumnChanged()override;
					void												OnDataProviderItemModified(vint start, vint count, vint newCount)override;
				public:
					/// <summary>Create the content provider.</summary>
					/// <param name="_dataProvider">The data provider for this control.</param>
					DataGridItemProvider(IDataProvider* _dataProvider);
					~DataGridItemProvider();

					void												SortByColumn(vint column, bool ascending=true);

					// ===================== GuiListControl::IItemProvider =====================

					bool												AttachCallback(GuiListControl::IItemProviderCallback* value)override;
					bool												DetachCallback(GuiListControl::IItemProviderCallback* value)override;
					vint												Count()override;
					IDescriptable*										RequestView(const WString& identifier)override;
					void												ReleaseView(IDescriptable* view)override;

					// ===================== GuiListControl::IItemPrimaryTextView =====================

					WString												GetPrimaryTextViewText(vint itemIndex)override;
					bool												ContainsPrimaryText(vint itemIndex)override;

					// ===================== list::ListViewItemStyleProvider::IListViewItemView =====================

					Ptr<GuiImageData>									GetSmallImage(vint itemIndex)override;
					Ptr<GuiImageData>									GetLargeImage(vint itemIndex)override;
					WString												GetText(vint itemIndex)override;
					WString												GetSubItem(vint itemIndex, vint index)override;
					vint												GetDataColumnCount()override;
					vint												GetDataColumn(vint index)override;

					// ===================== list::ListViewColumnItemArranger::IColumnItemView =====================
						
					bool												AttachCallback(ListViewColumnItemArranger::IColumnItemViewCallback* value)override;
					bool												DetachCallback(ListViewColumnItemArranger::IColumnItemViewCallback* value)override;
					vint												GetColumnCount()override;
					WString												GetColumnText(vint index)override;
					vint												GetColumnSize(vint index)override;
					void												SetColumnSize(vint index, vint value)override;
					GuiMenu*											GetDropdownPopup(vint index)override;
					GuiListViewColumnHeader::ColumnSortingState			GetSortingState(vint index)override;
				};

/***********************************************************************
Datagrid ContentProvider
***********************************************************************/
				
				/// <summary>Data grid content provider.</summary>
				class DataGridContentProvider
					: public Object
					, public virtual ListViewItemStyleProvider::IListViewItemContentProvider
					, protected virtual ListViewColumnItemArranger::IColumnItemViewCallback
					, protected virtual GuiListControl::IItemProviderCallback
					, protected virtual IDataEditorCallback
					, public Description<ListViewDetailContentProvider>
				{
				protected:
					class ItemContent : public Object, public virtual ListViewItemStyleProvider::IListViewItemContent
					{
					protected:
						compositions::GuiBoundsComposition*				contentComposition;
						compositions::GuiTableComposition*				textTable;

						DataGridContentProvider*						contentProvider;
						FontProperties									font;

						collections::Array<Ptr<IDataVisualizer>>		dataVisualizers;
						vint											currentRow;
						IDataEditor*									currentEditor;

						void											RemoveCellsAndDataVisualizers();
						IDataVisualizerFactory*							GetDataVisualizerFactory(vint row, vint column);
						vint											GetCellColumnIndex(compositions::GuiGraphicsComposition* composition);
						void											OnCellMouseUp(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
					public:
						ItemContent(DataGridContentProvider* _contentProvider, const FontProperties& _font);
						~ItemContent();

						compositions::GuiBoundsComposition*				GetContentComposition()override;
						compositions::GuiBoundsComposition*				GetBackgroundDecorator()override;
						void											UpdateSubItemSize();
						void											NotifyCloseEditor();
						void											Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, vint itemIndex)override;
						void											Uninstall()override;
					};

					Size												iconSize;
					GuiListControl::IItemProvider*						itemProvider;
					list::IDataProvider*								dataProvider;
					ListViewColumnItemArranger::IColumnItemView*		columnItemView;
					ListViewItemStyleProvider*							listViewItemStyleProvider;

					ListViewMainColumnDataVisualizer::Factory			mainColumnDataVisualizerFactory;
					ListViewSubColumnDataVisualizer::Factory			subColumnDataVisualizerFactory;

					vint												editingRow;
					vint												editingColumn;
					Ptr<IDataEditor>									currentEditor;
					bool												currentEditorRequestingSaveData;

					void												OnColumnChanged()override;
					void												OnAttached(GuiListControl::IItemProvider* provider)override;
					void												OnItemModified(vint start, vint count, vint newCount)override;

					void												NotifyCloseEditor();
					void												RequestSaveData();
					IDataEditor*										OpenEditor(vint row, vint column, IDataEditorFactory* editorFactory);
					void												CloseEditor();
				public:
					/// <summary>Create the content provider.</summary>
					DataGridContentProvider();
					~DataGridContentProvider();
					
					GuiListControl::IItemCoordinateTransformer*			CreatePreferredCoordinateTransformer()override;
					GuiListControl::IItemArranger*						CreatePreferredArranger()override;
					ListViewItemStyleProvider::IListViewItemContent*	CreateItemContent(const FontProperties& font)override;
					void												AttachListControl(GuiListControl* value)override;
					void												DetachListControl()override;
				};
			}

/***********************************************************************
DataGrid Control
***********************************************************************/

			/// <summary>Data grid control in virtual mode.</summary>
			class GuiVirtualDataGrid : public GuiVirtualListView, public Description<GuiVirtualDataGrid>
			{
			protected:
				list::DataGridItemProvider*								itemProvider;
				Ptr<list::IDataProvider>								dataProvider;

				void													OnColumnClicked(compositions::GuiGraphicsComposition* sender, compositions::GuiItemEventArgs& arguments);
			public:
				/// <summary>Create a data grid control in virtual mode.</summary>
				/// <param name="_styleProvider">The style provider for this control.</param>
				/// <param name="_dataProvider">The data provider for this control.</param>
				GuiVirtualDataGrid(IStyleProvider* _styleProvider, list::IDataProvider* _dataProvider);
				~GuiVirtualDataGrid();
			};
		}
	}
}

#endif