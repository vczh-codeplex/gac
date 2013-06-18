/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUIDATAGRIDCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUIDATAGRIDCONTROLS

#include "GuiDataGridInterfaces.h"
#include "GuiDataGridStructured.h"
#include "GuiDataGridExtensions.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			namespace list
			{

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

					void												InvokeOnItemModified(vint start, vint count, vint newCount);
					void												InvokeOnColumnChanged();
					void												OnDataProviderColumnChanged()override;
					void												OnDataProviderItemModified(vint start, vint count, vint newCount)override;
				public:
					/// <summary>Create the content provider.</summary>
					/// <param name="_dataProvider">The data provider for this control.</param>
					DataGridItemProvider(IDataProvider* _dataProvider);
					~DataGridItemProvider();

					IDataProvider*										GetDataProvider();
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
Virtual DataGrid Control
***********************************************************************/

			/// <summary>Data grid control in virtual mode.</summary>
			class GuiVirtualDataGrid : public GuiVirtualListView, public Description<GuiVirtualDataGrid>
			{
			protected:
				list::DataGridItemProvider*								itemProvider;
				Ptr<list::IDataProvider>								dataProvider;
				Ptr<list::StructuredDataProvider>						structuredDataProvider;

				void													OnColumnClicked(compositions::GuiGraphicsComposition* sender, compositions::GuiItemEventArgs& arguments);
				void													Initialize();
			public:
				/// <summary>Create a data grid control in virtual mode.</summary>
				/// <param name="_styleProvider">The style provider for this control.</param>
				/// <param name="_dataProvider">The data provider for this control.</param>
				GuiVirtualDataGrid(IStyleProvider* _styleProvider, list::IDataProvider* _dataProvider);
				/// <summary>Create a data grid control in virtual mode.</summary>
				/// <param name="_styleProvider">The style provider for this control.</param>
				/// <param name="_dataProvider">The data provider for this control.</param>
				GuiVirtualDataGrid(IStyleProvider* _styleProvider, list::IStructuredDataProvider* _dataProvider);
				~GuiVirtualDataGrid();

				/// <summary>Get the given data provider.</summary>
				/// <returns>The data provider.</returns>
				list::IDataProvider*									GetDataProvider();
				/// <summary>Get the given structured data provider.</summary>
				/// <returns>The structured data provider.</returns>
				list::StructuredDataProvider*							GetStructuredDataProvider();
			};

/***********************************************************************
StringGrid Control
***********************************************************************/

			class GuiStringGrid;

			namespace list
			{
				class StringGridItem
				{
				public:
					collections::List<WString>							strings;
				};

				class StringGridProvider : private StrongTypedDataProvider<Ptr<StringGridItem>>, public Description<StringGridProvider>
				{
					friend class GuiStringGrid;
				protected:
					collections::List<Ptr<StringGridItem>>				items;

					vint												GetRowCount()override;
					void												GetRowData(vint row, Ptr<StringGridItem>& rowData)override;
				public:
					StringGridProvider();
					~StringGridProvider();
				};
			}

			/// <summary>String grid control in virtual mode.</summary>
			class GuiStringGrid : public GuiVirtualDataGrid, public Description<GuiStringGrid>
			{
			protected:
			public:
				/// <summary>Create a string grid control.</summary>
				/// <param name="_styleProvider">The style provider for this control.</param>
				GuiStringGrid(IStyleProvider* _styleProvider);
				~GuiStringGrid();
			};
		}
	}
}

#endif