/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUIDATAGRIDCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUIDATAGRIDCONTROLS

#include "GuiListViewControls.h"

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

					/// <summary>Get the composition that renders the data.</summary>
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

					/// <summary>Get the composition that holds the editor for a cell.</summary>
					/// <returns>The composition.</returns>
					virtual compositions::GuiBoundsComposition*			GetBoundsComposition()=0;

					/// <summary>Called before editing a cell.</summary>
					/// <param name="dataProvider">The data provider.</param>
					/// <param name="row">The row number of the cell.</param>
					/// <param name="column">The column number of the cell.</param>
					virtual void										BeforeEditCell(IDataProvider* dataProvider, vint row, vint column)=0;

					/// <summary>Called after editing a cell.</summary>
					/// <param name="dataProvider">The data provider.</param>
					/// <param name="row">The row number of the cell.</param>
					/// <param name="column">The column number of the cell.</param>
					virtual void										AfterEditCell(IDataProvider* dataProvider, vint row, vint column)=0;
				};

				/// <summary>The required <see cref="GuiListControl::IItemProvider"/> view for [T:vl.presentation.controls.GuiVirtualDataGrid].</summary>
				class IDataProvider : public virtual IDescriptable, public Description<IDataProvider>
				{
				public:
					/// <summary>The identifier for this view.</summary>
					static const wchar_t* const							Identifier;
					
					/// <summary>Get the number of all columns.</summary>
					/// <returns>The number of all columns.</returns>
					virtual vint										GetColumnCount()=0;
					/// <summary>Get the text of the column.</summary>
					/// <returns>The text of the column.</returns>
					/// <param name="column">The index of the column.</param>
					virtual WString										GetColumnText(vint column)=0;
					/// <summary>Get the size of the column.</summary>
					/// <returns>The size of the column.</returns>
					/// <param name="column">The index of the column.</param>
					virtual vint										GetColumnSize(vint column)=0;
					/// <summary>Set the size of the column.</summary>
					/// <param name="column">The index of the column.</param>
					/// <param name="value">The new size of the column.</param>
					virtual void										SetColumnSize(vint column, vint value)=0;
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
					/// <summary>Get the text of the cell.</summary>
					/// <returns>The text of the cell.</returns>
					/// <param name="row">The row number of the cell.</param>
					/// <param name="column">The column number of the cell.</param>
					virtual WString										GetCellText(vint row, vint column)=0;
					/// <summary>Get the data visualizer factory that creates data visualizers for visualizing the cell.</summary>
					/// <returns>The data visualizer factory. The data grid control to use the predefined data visualizer if this function returns null.</returns>
					/// <param name="row">The row number of the cell.</param>
					/// <param name="column">The column number of the cell.</param>
					virtual IDataVisualizerFactory*						GetCellDataVisualizerFactory(vint row, vint column)=0;
					/// <summary>Called before visualizing the cell.</summary>
					/// <param name="row">The row number of the cell.</param>
					/// <param name="column">The column number of the cell.</param>
					/// <param name="dataVisualizer">The data visualizer to be updated.</param>
					virtual void										VisualizeCell(vint row, vint column, IDataVisualizer* dataVisualizer)=0;
					/// <summary>Get the data editor factory that creates data editors for editing the cell.</summary>
					/// <returns>The data editor factory. Returns null to disable editing.</returns>
					/// <param name="row">The row number of the cell.</param>
					/// <param name="column">The column number of the cell.</param>
					virtual IDataEditorFactory*							GetCellDataEditorFactory(vint row, vint column)=0;
					/// <summary>Called before edit the cell.</summary>
					/// <param name="row">The row number of the cell.</param>
					/// <param name="column">The column number of the cell.</param>
					/// <param name="dataEditor">The data editor.</param>
					virtual void										BeforeEditCell(vint row, vint column, IDataEditor* dataEditor)=0;
					/// <summary>Called after edit the cell.</summary>
					/// <param name="row">The row number of the cell.</param>
					/// <param name="column">The column number of the cell.</param>
					/// <param name="dataEditor">The data editor.</param>
					virtual void										AfterEditCell(vint row, vint column, IDataEditor* dataEditor)=0;
				};

/***********************************************************************
DataSource Extensions
***********************************************************************/

/***********************************************************************
Visualizer Extensions
***********************************************************************/

				template<typename TVisualizer>
				class DataVisualizerBase : public Object, public virtual IDataVisualizer
				{
				public:
					class Factory : public Object, public virtual IDataVisualizerFactory, public Description<Factory>
					{
					public:
						Ptr<IDataVisualizer> CreateVisualizer(const FontProperties& font, GuiListViewBase::IStyleProvider* styleProvider)override
						{
							TVisualizer* dataVisualizer=new TVisualizer;
							dataVisualizer->factory=this;
							dataVisualizer->font=font;
							dataVisualizer->styleProvider=styleProvider;
							return dataVisualizer;
						}
					};

				protected:
					Factory*											factory;
					FontProperties										font;
					GuiListViewBase::IStyleProvider*					styleProvider;
					compositions::GuiBoundsComposition*					boundsComposition;

					virtual compositions::GuiBoundsComposition*			CreateBoundsCompositionInternal()=0;
				public:
					DataVisualizerBase()
						:factory(0)
						,styleProvider(0)
						,boundsComposition(0)
					{
					}

					IDataVisualizerFactory* GetFactory()override
					{
						return factory;
					}

					compositions::GuiBoundsComposition* GetBoundsComposition()override
					{
						if(!boundsComposition)
						{
							boundsComposition=CreateBoundsCompositionInternal();
						}
						return boundsComposition;
					}

					void BeforeVisualizerCell(IDataProvider* dataProvider, vint row, vint column)override
					{
					}
				};

				class ListViewMainColumnDataVisualizer : public DataVisualizerBase<ListViewMainColumnDataVisualizer>
				{
				protected:
					elements::GuiImageFrameElement*						image;
					elements::GuiSolidLabelElement*						text;

					compositions::GuiBoundsComposition*					CreateBoundsCompositionInternal()override;
				public:

					void												BeforeVisualizerCell(IDataProvider* dataProvider, vint row, vint column)override;
				};

				class ListViewSubColumnDataVisualizer : public DataVisualizerBase<ListViewSubColumnDataVisualizer>
				{
				protected:
					elements::GuiSolidLabelElement*						text;

					compositions::GuiBoundsComposition*					CreateBoundsCompositionInternal()override;
				public:

					void												BeforeVisualizerCell(IDataProvider* dataProvider, vint row, vint column)override;
				};

/***********************************************************************
Editor Extensions
***********************************************************************/

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
					, public Description<ListViewDetailContentProvider>
				{
				protected:
					class ItemContent : public Object, public virtual ListViewItemStyleProvider::IListViewItemContent
					{
					protected:
						compositions::GuiBoundsComposition*				contentComposition;
						compositions::GuiTableComposition*				textTable;

						DataGridContentProvider*						contentProvider;
						GuiListControl::IItemProvider*					itemProvider;
						FontProperties									font;

						ListViewColumnItemArranger::IColumnItemView*	columnItemView;
						IDataProvider*									dataProvider;
						collections::Array<Ptr<IDataVisualizer>>		dataVisualizers;

						void											RemoveCells();
						IDataVisualizerFactory*							GetDataVisualizerFactory(vint row, vint column);
					public:
						ItemContent(DataGridContentProvider* _contentProvider, GuiListControl::IItemProvider* _itemProvider, const FontProperties& _font);
						~ItemContent();

						compositions::GuiBoundsComposition*				GetContentComposition()override;
						compositions::GuiBoundsComposition*				GetBackgroundDecorator()override;
						void											UpdateSubItemSize();
						void											Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, vint itemIndex)override;
					};

					Size												iconSize;
					GuiListControl::IItemProvider*						itemProvider;
					ListViewColumnItemArranger::IColumnItemView*		columnItemView;
					ListViewItemStyleProvider*							listViewItemStyleProvider;
					ListViewMainColumnDataVisualizer::Factory			mainColumnDataVisualizerFactory;
					ListViewSubColumnDataVisualizer::Factory			subColumnDataVisualizerFactory;

					void												OnColumnChanged()override;
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