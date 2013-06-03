/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUIDATAEXTENSIONS
#define VCZH_PRESENTATION_CONTROLS_GUIDATAEXTENSIONS

#include "GuiDataGridInterfaces.h"
#include "GuiComboControls.h"
#include "GuiTextListControls.h"
#include "..\TextEditorPackage\GuiTextControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			namespace list
			{

/***********************************************************************
Visualizer Extensions
***********************************************************************/
				
				/// <summary>Base class for all data visualizers.</summary>
				class DataVisualizerBase : public Object, public virtual IDataVisualizer
				{
					template<typename T>
					friend class DataVisualizerFactory;
					template<typename T>
					friend class DataDecoratableVisualizerFactory;
				protected:
					IDataVisualizerFactory*								factory;
					FontProperties										font;
					GuiListViewBase::IStyleProvider*					styleProvider;
					compositions::GuiBoundsComposition*					boundsComposition;
					Ptr<IDataVisualizer>								decoratedDataVisualizer;

					virtual compositions::GuiBoundsComposition*			CreateBoundsCompositionInternal(compositions::GuiBoundsComposition* decoratedComposition)=0;
				public:
					/// <summary>Create the data visualizer.</summary>
					/// <param name="_decoratedDataVisualizer">The decorated data visualizer inside the current data visualizer.</param>
					DataVisualizerBase(Ptr<IDataVisualizer> _decoratedDataVisualizer=0);
					~DataVisualizerBase();

					IDataVisualizerFactory*								GetFactory()override;
					compositions::GuiBoundsComposition*					GetBoundsComposition()override;
					void												BeforeVisualizerCell(IDataProvider* dataProvider, vint row, vint column)override;
					IDataVisualizer*									GetDecoratedDataVisualizer()override;
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
				
				template<typename TVisualizer>
				class DataDecoratableVisualizerFactory : public Object, public virtual IDataVisualizerFactory, public Description<DataDecoratableVisualizerFactory<TVisualizer>>
				{
				protected:
					Ptr<IDataVisualizerFactory>							decoratedFactory;
				public:
					DataDecoratableVisualizerFactory(Ptr<IDataVisualizerFactory> _decoratedFactory)
						:decoratedFactory(_decoratedFactory)
					{
					}

					Ptr<IDataVisualizer> CreateVisualizer(const FontProperties& font, GuiListViewBase::IStyleProvider* styleProvider)override
					{
						Ptr<IDataVisualizer> decoratedDataVisualizer=decoratedFactory->CreateVisualizer(font, styleProvider);
						DataVisualizerBase* dataVisualizer=new TVisualizer(decoratedDataVisualizer);
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

					compositions::GuiBoundsComposition*					CreateBoundsCompositionInternal(compositions::GuiBoundsComposition* decoratedComposition)override;
				public:
					/// <summary>Create the data visualizer.</summary>
					ListViewMainColumnDataVisualizer();

					void												BeforeVisualizerCell(IDataProvider* dataProvider, vint row, vint column)override;

					/// <summary>Get the internal text element.</summary>
					/// <returns>The text element.</returns>
					elements::GuiSolidLabelElement*						GetTextElement();
				};
				
				/// <summary>Data visualizer that displays a text. Use ListViewSubColumnDataVisualizer::Factory as the factory class.</summary>
				class ListViewSubColumnDataVisualizer : public DataVisualizerBase
				{
				public:
					typedef DataVisualizerFactory<ListViewSubColumnDataVisualizer>			Factory;
				protected:
					elements::GuiSolidLabelElement*						text;

					compositions::GuiBoundsComposition*					CreateBoundsCompositionInternal(compositions::GuiBoundsComposition* decoratedComposition)override;
				public:
					/// <summary>Create the data visualizer.</summary>
					ListViewSubColumnDataVisualizer();

					void												BeforeVisualizerCell(IDataProvider* dataProvider, vint row, vint column)override;

					/// <summary>Get the internal text element.</summary>
					/// <returns>The text element.</returns>
					elements::GuiSolidLabelElement*						GetTextElement();
				};
				
				/// <summary>Data visualizer that display a cell border out of another data visualizer.</summary>
				class CellBorderDataVisualizer : public DataVisualizerBase
				{
				public:
					typedef DataDecoratableVisualizerFactory<CellBorderDataVisualizer>		Factory;
				protected:

					compositions::GuiBoundsComposition*					CreateBoundsCompositionInternal(compositions::GuiBoundsComposition* decoratedComposition)override;
				public:
					/// <summary>Create the data visualizer.</summary>
					/// <param name="decoratedDataVisualizer">The decorated data visualizer.</param>
					CellBorderDataVisualizer(Ptr<IDataVisualizer> decoratedDataVisualizer);

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
			}
		}
	}
}

#endif