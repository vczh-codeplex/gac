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

				DataVisualizerBase::DataVisualizerBase(Ptr<IDataVisualizer> _decoratedDataVisualizer)
					:factory(0)
					,styleProvider(0)
					,boundsComposition(0)
					,decoratedDataVisualizer(_decoratedDataVisualizer)
				{
				}

				DataVisualizerBase::~DataVisualizerBase()
				{
					if(decoratedDataVisualizer)
					{
						GuiBoundsComposition* composition=decoratedDataVisualizer->GetBoundsComposition();
						if(composition->GetParent())
						{
							composition->GetParent()->RemoveChild(composition);
						}
						decoratedDataVisualizer=0;
					}
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
						GuiBoundsComposition* decoratedComposition=0;
						if(decoratedDataVisualizer)
						{
							decoratedComposition=decoratedDataVisualizer->GetBoundsComposition();
						}
						boundsComposition=CreateBoundsCompositionInternal(decoratedComposition);
					}
					return boundsComposition;
				}

				void DataVisualizerBase::BeforeVisualizerCell(IDataProvider* dataProvider, vint row, vint column)
				{
				}

				IDataVisualizer* DataVisualizerBase::GetDecoratedDataVisualizer()
				{
					return decoratedDataVisualizer.Obj();
				}
				
/***********************************************************************
ListViewMainColumnDataVisualizer
***********************************************************************/

				compositions::GuiBoundsComposition* ListViewMainColumnDataVisualizer::CreateBoundsCompositionInternal(compositions::GuiBoundsComposition* decoratedComposition)
				{
					GuiTableComposition* table=new GuiTableComposition;
					table->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
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

					text->SetAlignments(Alignment::Left, Alignment::Center);
					text->SetFont(font);
					text->SetColor(styleProvider->GetPrimaryTextColor());
					text->SetEllipse(true);
					text->SetText(dataProvider->GetCellText(row, column));
				}

				elements::GuiSolidLabelElement* ListViewMainColumnDataVisualizer::GetTextElement()
				{
					return text;
				}
				
/***********************************************************************
ListViewSubColumnDataVisualizer
***********************************************************************/

				compositions::GuiBoundsComposition* ListViewSubColumnDataVisualizer::CreateBoundsCompositionInternal(compositions::GuiBoundsComposition* decoratedComposition)
				{
					GuiBoundsComposition* composition=new GuiBoundsComposition;
					composition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
					composition->SetMargin(Margin(8, 0, 8, 0));

					text=GuiSolidLabelElement::Create();
					composition->SetOwnedElement(text);

					return composition;
				}

				ListViewSubColumnDataVisualizer::ListViewSubColumnDataVisualizer()
					:text(0)
				{
				}

				void ListViewSubColumnDataVisualizer::BeforeVisualizerCell(IDataProvider* dataProvider, vint row, vint column)
				{
					text->SetAlignments(Alignment::Left, Alignment::Center);
					text->SetFont(font);
					text->SetColor(styleProvider->GetSecondaryTextColor());
					text->SetEllipse(true);
					text->SetText(dataProvider->GetCellText(row, column));
				}

				elements::GuiSolidLabelElement* ListViewSubColumnDataVisualizer::GetTextElement()
				{
					return text;
				}
				
/***********************************************************************
CellBorderDataVisualizer
***********************************************************************/

				compositions::GuiBoundsComposition* CellBorderDataVisualizer::CreateBoundsCompositionInternal(compositions::GuiBoundsComposition* decoratedComposition)
				{
					GuiBoundsComposition* border1=0;
					GuiBoundsComposition* border2=0;
					{
						GuiSolidBorderElement* element=GuiSolidBorderElement::Create();
						element->SetColor(styleProvider->GetItemSeparatorColor());

						border1=new GuiBoundsComposition;
						border1->SetOwnedElement(element);
						border1->SetAlignmentToParent(Margin(-1, 0, 0, 0));
					}
					{
						GuiSolidBorderElement* element=GuiSolidBorderElement::Create();
						element->SetColor(styleProvider->GetItemSeparatorColor());

						border2=new GuiBoundsComposition;
						border2->SetOwnedElement(element);
						border2->SetAlignmentToParent(Margin(0, -1, 0, 0));
					}
					decoratedComposition->SetAlignmentToParent(Margin(0, 0, 1, 1));

					GuiBoundsComposition* composition=new GuiBoundsComposition;
					composition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
					composition->AddChild(border1);
					composition->AddChild(border2);
					composition->AddChild(decoratedComposition);

					return composition;
				}

				CellBorderDataVisualizer::CellBorderDataVisualizer(Ptr<IDataVisualizer> decoratedDataVisualizer)
					:DataVisualizerBase(decoratedDataVisualizer)
				{
				}

				void CellBorderDataVisualizer::BeforeVisualizerCell(IDataProvider* dataProvider, vint row, vint column)
				{
					decoratedDataVisualizer->BeforeVisualizerCell(dataProvider, row, column);
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