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
				/// <summary>The required <see cref="GuiListControl::IItemProvider"/> view for [T:vl.presentation.controls.GuiVirtualDataGrid].</summary>
				class IDataProvider : public virtual IDescriptable, public Description<IDataProvider>
				{
				public:
					/// <summary>The identifier for this view.</summary>
					static const wchar_t* const				Identifier;
					
					/// <summary>Get the number of all columns.</summary>
					/// <returns>The number of all columns.</returns>
					virtual vint							GetColumnCount()=0;
					/// <summary>Get the text of the column.</summary>
					/// <returns>The text of the column.</returns>
					/// <param name="column">The index of the column.</param>
					virtual WString							GetColumnText(vint column)=0;
					/// <summary>Get the size of the column.</summary>
					/// <returns>The size of the column.</returns>
					/// <param name="column">The index of the column.</param>
					virtual vint							GetColumnSize(vint column)=0;
					/// <summary>Set the size of the column.</summary>
					/// <param name="column">The index of the column.</param>
					/// <param name="value">The new size of the column.</param>
					virtual void							SetColumnSize(vint column, vint value)=0;
					
					/// <summary>Get the number of all rows.</summary>
					/// <returns>The number of all rows.</returns>
					virtual vint							GetRowCount()=0;
					/// <summary>Get the text of the cell.</summary>
					/// <returns>The text of the cell.</returns>
					/// <param name="row">The row number of the cell.</param>
					/// <param name="column">The column number of the cell.</param>
					virtual WString							GetCellText(vint row, vint column)=0;
				};

/***********************************************************************
Datagrid ItemProvider
***********************************************************************/
				
				/// <summary>Data grid item provider.</summary>
				class DataGridItemProvider
					: public Object
					, public virtual GuiListControl::IItemProvider
					, protected virtual GuiListControl::IItemPrimaryTextView
					, protected virtual ListViewItemStyleProvider::IListViewItemView
					, protected virtual ListViewColumnItemArranger::IColumnItemView
					, public Description<DataGridItemProvider>
				{
				protected:
					IDataProvider*																dataProvider;
					collections::List<GuiListControl::IItemProviderCallback*>					itemProviderCallbacks;
					collections::List<ListViewColumnItemArranger::IColumnItemViewCallback*>		columnItemViewCallbacks;

				public:
					/// <summary>Create the content provider.</summary>
					/// <param name="_dataProvider">The data provider for this control.</param>
					DataGridItemProvider(IDataProvider* _dataProvider);
					~DataGridItemProvider();

					// ===================== GuiListControl::IItemProvider =====================

					bool											AttachCallback(GuiListControl::IItemProviderCallback* value)override;
					bool											DetachCallback(GuiListControl::IItemProviderCallback* value)override;
					vint											Count()override;
					IDescriptable*									RequestView(const WString& identifier)override;
					void											ReleaseView(IDescriptable* view)override;
				protected:

					// ===================== GuiListControl::IItemPrimaryTextView =====================

					WString											GetPrimaryTextViewText(vint itemIndex)override;
					bool											ContainsPrimaryText(vint itemIndex)override;

					// ===================== list::ListViewItemStyleProvider::IListViewItemView =====================

					Ptr<GuiImageData>								GetSmallImage(vint itemIndex)override;
					Ptr<GuiImageData>								GetLargeImage(vint itemIndex)override;
					WString											GetText(vint itemIndex)override;
					WString											GetSubItem(vint itemIndex, vint index)override;
					vint											GetDataColumnCount()override;
					vint											GetDataColumn(vint index)override;

					// ===================== list::ListViewColumnItemArranger::IColumnItemView =====================
						
					bool											AttachCallback(ListViewColumnItemArranger::IColumnItemViewCallback* value)override;
					bool											DetachCallback(ListViewColumnItemArranger::IColumnItemViewCallback* value)override;
					vint											GetColumnCount()override;
					WString											GetColumnText(vint index)override;
					vint											GetColumnSize(vint index)override;
					void											SetColumnSize(vint index, vint value)override;
					GuiMenu*										GetDropdownPopup(vint index)override;
					GuiListViewColumnHeader::ColumnSortingState		GetSortingState(vint index)override;
				};

/***********************************************************************
Datagrid ContentProvider
***********************************************************************/
				
				/// <summary>Data grid content provider.</summary>
				class DatagridContentProvider
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

						GuiListControl::IItemProvider*					itemProvider;
						ListViewColumnItemArranger::IColumnItemView*	columnItemView;
						IDataProvider*									dataProvider;

					public:
						ItemContent(GuiListControl::IItemProvider* _itemProvider);
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

					void												OnColumnChanged()override;
				public:
					/// <summary>Create the content provider.</summary>
					DatagridContentProvider();
					~DatagridContentProvider();
					
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
			public:

			protected:
				Ptr<list::IDataProvider>						dataProvider;
			public:
				/// <summary>Create a data grid control in virtual mode.</summary>
				/// <param name="_styleProvider">The style provider for this control.</param>
				/// <param name="_dataProvider">The data provider for this control.</param>
				GuiVirtualDataGrid(IStyleProvider* _styleProvider, list::IDataProvider* _dataProvider);
				~GuiVirtualDataGrid();
			};

/***********************************************************************
DataSource Extensions
***********************************************************************/

/***********************************************************************
Visualizer Extensions
***********************************************************************/

/***********************************************************************
Editor Extensions
***********************************************************************/
		}
	}
}

#endif