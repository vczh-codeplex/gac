#include "GuiDataGridExtensions.h"
#include "..\Styles\GuiThemeStyleFactory.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			namespace list
			{
				using namespace compositions;
				using namespace elements;
				using namespace theme;
				
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
			}
		}
	}
}