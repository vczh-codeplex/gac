#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "..\..\Source\GacUI.h"
#include "..\..\Source\NativeWindow\Windows\WinNativeWindow.h"
#include "..\..\Source\GraphicsElement\WindowsDirect2D\GuiGraphicsWindowsDirect2D.h"
#include <Windows.h>
#include <msctf.h>

using namespace vl::stream;
using namespace vl::collections;
using namespace vl::regex;

#define GUI_GRAPHICS_RENDERER_DIRECT2D

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
#ifdef GUI_GRAPHICS_RENDERER_GDI
	int result=SetupWindowsGDIRenderer();
#endif
#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D
	int result=SetupWindowsDirect2DRenderer();
#endif

#if _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return result;
}

namespace test
{

/***********************************************************************
TestWindow
***********************************************************************/

	struct ElementElectron
	{
		static const wchar_t*			RegexEec;

		vint		level;
		wchar_t		type;
		vint		typeOrder;
		vint		count;

		static vint Compare(const ElementElectron& a, const ElementElectron& b)
		{
			vint result=0;
			if(result==0) result=a.level-b.level;
			if(result==0) result=a.typeOrder-b.typeOrder;
			if(result==0) result=a.count-b.count;
			return result;
		}

		static void Parse(const Regex& regexEec, vint order, const WString& electron, vint& notationOrder, WString& notationName, List<ElementElectron>& ecs)
		{
			if(order<=2)
			{
				notationOrder=0;
				notationName=L"";
			}
			else if(order<=10)
			{
				notationOrder=2;
				notationName=L"[He]";
			}
			else if(order<=18)
			{
				notationOrder=10;
				notationName=L"[Ne]";
			}
			else if(order<=36)
			{
				notationOrder=18;
				notationName=L"[Ar]";
			}
			else if(order<=54)
			{
				notationOrder=36;
				notationName=L"[Kr]";
			}
			else if(order<=86)
			{
				notationOrder=54;
				notationName=L"[Xe]";
			}
			else
			{
				notationOrder=86;
				notationName=L"[Rn]";
			}

			ecs.Clear();
			RegexMatch::List matches;
			regexEec.Search(electron, matches);
			FOREACH(Ptr<RegexMatch>, match, matches)
			{
				ElementElectron ec;
				ec.level=wtoi(match->Groups()[L"level"].Get(0).Value());
				ec.type=match->Groups()[L"type"].Get(0).Value()[0];
				ec.count=wtoi(match->Groups()[L"count"].Get(0).Value());
				ec.typeOrder=-1;

				switch(ec.type)
				{
				case L's': ec.typeOrder=0; break;
				case L'p': ec.typeOrder=1; break;
				case L'd': ec.typeOrder=2; break;
				case L'f': ec.typeOrder=3; break;
				}

				ecs.Add(ec);
			}

			SortLambda(&ecs[0], ecs.Count(), &Compare);
		}

		WString ToString()
		{
			return itow(level)+type+itow(count);
		}
	};
	const wchar_t* ElementElectron::RegexEec = L"(<level>/d+)(<type>[spdf])(<count>/d+)";

	struct ElementElectronConfiguration
	{
		vint										nobleGasNotationOrder;
		WString										nobleGasNotationName;
		List<ElementElectron>						electrons;

		WString ToString()
		{
			WString ecsLabel=nobleGasNotationName;
			FOREACH(ElementElectron, ec, electrons)
			{
				ecsLabel+=L" "+ec.ToString();
			}
			return ecsLabel;
		}
	};

	struct ElementData
	{
		vint		order;
		WString		symbol;
		WString		chinese;
		WString		english;
		double		weight;
		WString		valence;
		WString		electron;
		Ptr<ElementElectronConfiguration>			ecs;

		static WString Partition(const wchar_t*& reading)
		{
			const wchar_t* next=wcschr(reading, L'\t');
			if(!next)
			{
				next=reading+wcslen(reading);
			}
			WString text(reading, next-reading);
			reading=*next?next+1:next;
			return text;
		}

		static ElementData Parse(const WString& text, const Regex& regexEec)
		{
			ElementData data;
			const wchar_t* reading=text.Buffer();
			data.order=wtoi(Partition(reading));
			data.symbol=Partition(reading);
			data.chinese=Partition(reading);
			data.weight=wtof(Partition(reading));
			data.electron=Partition(reading);
			data.valence=Partition(reading);
			data.english=Partition(reading);

			data.ecs=new ElementElectronConfiguration;
			ElementElectron::Parse(regexEec, data.order, data.electron, data.ecs->nobleGasNotationOrder, data.ecs->nobleGasNotationName, data.ecs->electrons);
			return data;
		}
	};

	class ElementRadioactiveColumnProvider : public list::StrongTypedFieldColumnProvider<ElementData, WString>
	{
	public:
		ElementRadioactiveColumnProvider(list::StrongTypedDataProvider<ElementData>* dataProvider, WString ElementData::* field)
			:StrongTypedFieldColumnProvider(dataProvider, field)
		{
		}

		void VisualizeCell(vint row, list::IDataVisualizer* dataVisualizer)override
		{
			StrongTypedFieldColumnProvider::VisualizeCell(row, dataVisualizer);
			ElementData data;
			dataProvider->GetRowData(row, data);
			if(data.order==43 || data.order==61 || data.order>=83)
			{
				GuiSolidLabelElement* text=dataVisualizer->GetVisualizer<list::ListViewSubColumnDataVisualizer>()->GetTextElement();
				text->SetColor(Color(255, 0, 0));

				FontProperties font=text->GetFont();
				font.bold=true;
				text->SetFont(font);
			}
		}
	};

	class ElementElectronDataVisualizer : public list::DataVisualizerBase
	{
	public:
		typedef list::DataVisualizerFactory<ElementElectronDataVisualizer>			Factory;
	protected:
		static const vint							GraphHeight		=64;
		static const vint							GraphWidth		=8;
		static const vint							GraphHole		=16;
		static const vint							GraphStep		=20;
		static const vint							ArcLeft			=16;
		static const vint							ArcTop			=12;
		static const vint							ArcRadius		=32;
		
		List<vint>									electronCounts;
		GuiDirect2DElement*							graphElement;

		ComPtr<ID2D1SolidColorBrush>				lineBrush;
		ComPtr<ID2D1SolidColorBrush>				textBrush;
		ComPtr<ID2D1SolidColorBrush>				backgroundBrush;
		ComPtr<ID2D1PathGeometry>					arcPath;
		ComPtr<IDWriteTextFormat>					textFormat;

		void OnBeforeRenderTargetChanged(GuiGraphicsComposition* sender, GuiDirect2DElementEventArgs& arguments)
		{
			lineBrush=0;
			backgroundBrush=0;
			textBrush=0;
			arcPath=0;
			textFormat=0;
		}

		void OnAfterRenderTargetChanged(GuiGraphicsComposition* sender, GuiDirect2DElementEventArgs& arguments)
		{
			{
				ID2D1SolidColorBrush* brush=0;
				D2D1::ColorF color(0.5f, 0.5f, 0.5f);
				HRESULT hr=arguments.rt->CreateSolidColorBrush(color, &brush);
				if(!FAILED(hr))
				{
					lineBrush=brush;
				}
			}
			{
				ID2D1SolidColorBrush* brush=0;
				D2D1::ColorF color(0.0f, 0.0f, 0.0f);
				HRESULT hr=arguments.rt->CreateSolidColorBrush(color, &brush);
				if(!FAILED(hr))
				{
					textBrush=brush;
				}
			}
			{
				ID2D1SolidColorBrush* brush=0;
				D2D1::ColorF color(1.0f, 1.0f, 1.0f);
				HRESULT hr=arguments.rt->CreateSolidColorBrush(color, &brush);
				if(!FAILED(hr))
				{
					backgroundBrush=brush;
				}
			}
			{
				ID2D1PathGeometry* path=0;
				HRESULT hr=arguments.factoryD2D->CreatePathGeometry(&path);
				if(!FAILED(hr))
				{
					arcPath=path;

					ID2D1GeometrySink* sink=0;
					hr=path->Open(&sink);
					if(!FAILED(hr))
					{
						D2D_POINT_2F p1=D2D1::Point2F(0, (FLOAT)ArcTop);
						D2D_POINT_2F p2=D2D1::Point2F((FLOAT)GraphWidth, (FLOAT)(GraphHeight-GraphHole)/2);
						D2D_POINT_2F p3=D2D1::Point2F((FLOAT)GraphWidth, (FLOAT)(GraphHeight+GraphHole)/2);
						D2D_POINT_2F p4=D2D1::Point2F(0, (FLOAT)(GraphHeight-ArcTop));
						D2D_SIZE_F size=D2D1::SizeF((FLOAT)ArcRadius, (FLOAT)ArcRadius);

						sink->BeginFigure(p1, D2D1_FIGURE_BEGIN_HOLLOW);
						sink->AddArc(D2D1::ArcSegment(
							p2,
							size,
							0,
							D2D1_SWEEP_DIRECTION_CLOCKWISE,
							D2D1_ARC_SIZE_SMALL
							));
						sink->EndFigure(D2D1_FIGURE_END_OPEN);
						
						sink->BeginFigure(p3, D2D1_FIGURE_BEGIN_HOLLOW);
						sink->AddArc(D2D1::ArcSegment(
							p4,
							size,
							0,
							D2D1_SWEEP_DIRECTION_CLOCKWISE,
							D2D1_ARC_SIZE_SMALL
							));
						sink->EndFigure(D2D1_FIGURE_END_OPEN);

						sink->Close();
						sink->Release();
					}
				}
			}
			{
				IDWriteTextFormat* format=0;
				HRESULT hr=arguments.factoryDWrite->CreateTextFormat(
					font.fontFamily.Buffer(),
					NULL,
					(font.bold?DWRITE_FONT_WEIGHT_BOLD:DWRITE_FONT_WEIGHT_NORMAL),
					(font.italic?DWRITE_FONT_STYLE_ITALIC:DWRITE_FONT_STYLE_NORMAL),
					DWRITE_FONT_STRETCH_NORMAL,
					(FLOAT)font.size,
					L"",
					&format);
				if(!FAILED(hr))
				{
					textFormat=format;
					format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
				}
			}
		}

		void OnRendering(GuiGraphicsComposition* sender, GuiDirect2DElementEventArgs& arguments)
		{
			if(lineBrush && textBrush && arcPath && textFormat)
			{
				D2D1_MATRIX_3X2_F transform;
				arguments.rt->GetTransform(&transform);

				vint y=(GraphHeight-font.size)/2;

				for(vint i=0;i<electronCounts.Count();i++)
				{
					arguments.rt->SetTransform(
						D2D1::Matrix3x2F::Translation((FLOAT)(arguments.bounds.x1+ArcLeft+i*GraphStep), (FLOAT)arguments.bounds.y1)
						);
					arguments.rt->DrawGeometry(arcPath.Obj(), lineBrush.Obj());

					D2D1_RECT_F rect=D2D1::RectF((FLOAT)(electronCounts[i]<10?5:0), (FLOAT)y, 20, (FLOAT)(y+font.size));
					WString number=itow(electronCounts[i]);
					arguments.rt->DrawText(
						number.Buffer(),
						number.Length(),
						textFormat.Obj(),
						rect,
						textBrush.Obj()
						);
				}

				arguments.rt->SetTransform(&transform);
			}
		}

		GuiBoundsComposition* CreateBoundsCompositionInternal(GuiBoundsComposition* decoratedComposition)override
		{
			graphElement=GuiDirect2DElement::Create();
			graphElement->BeforeRenderTargetChanged.AttachMethod(this, &ElementElectronDataVisualizer::OnBeforeRenderTargetChanged);
			graphElement->AfterRenderTargetChanged.AttachMethod(this, &ElementElectronDataVisualizer::OnAfterRenderTargetChanged);
			graphElement->Rendering.AttachMethod(this, &ElementElectronDataVisualizer::OnRendering);

			GuiBoundsComposition* composition=new GuiBoundsComposition;
			composition->SetPreferredMinSize(Size(0, 64));
			composition->SetOwnedElement(graphElement);
			return composition;
		}
	public:
		ElementElectronDataVisualizer()
		{
		}

		void BeforeVisualizerCell(list::IDataProvider* dataProvider, vint row, vint column)override
		{
			electronCounts.Clear();
		}

		void ShowGraph(list::StrongTypedDataProvider<ElementData>* dataProvider, vint row)
		{
			Array<vint> counts;
			CopyFrom(counts, Range(0, 1).Repeat(10));

			ElementData data;
			vint order=row+1;
			while(order>0)
			{
				dataProvider->GetRowData(order-1, data);
				FOREACH(ElementElectron, ec, data.ecs->electrons)
				{
					counts[ec.level-1]+=ec.count;
				}
				order=data.ecs->nobleGasNotationOrder;
			}

			CopyFrom(electronCounts, From(counts).Take(counts.IndexOf(0)));
		}
	};

	class ElementElectronColumnProvider : public list::StrongTypedColumnProvider<ElementData, WString>
	{
	protected:

		void GetCellData(const ElementData& rowData, WString& cellData)override
		{
			cellData=rowData.ecs->ToString();
		}
	public:
		ElementElectronColumnProvider(list::StrongTypedDataProvider<ElementData>* dataProvider)
			:StrongTypedColumnProvider(dataProvider)
		{
		}

		void VisualizeCell(vint row, list::IDataVisualizer* dataVisualizer)override
		{
			StrongTypedColumnProvider::VisualizeCell(row, dataVisualizer);
			ElementElectronDataVisualizer* eedVisualizer=dataVisualizer->GetVisualizer<ElementElectronDataVisualizer>();
			if(eedVisualizer)
			{
				eedVisualizer->ShowGraph(dataProvider, row);
			}
		}
	};

	class DataProvider : public list::StrongTypedDataProvider<ElementData>
	{
	protected:
		List<ElementData>							elements;
		Ptr<list::IDataVisualizerFactory>			mainFactory;
		Ptr<list::IDataVisualizerFactory>			subFactory;
		Ptr<list::IDataVisualizerFactory>			eecFactory;
	public:
		DataProvider()
		{
			mainFactory=new list::CellBorderDataVisualizer::Factory(new list::ListViewMainColumnDataVisualizer::Factory);
			subFactory=new list::CellBorderDataVisualizer::Factory(new list::ListViewSubColumnDataVisualizer::Factory);
			eecFactory=new list::CellBorderDataVisualizer::Factory(new ElementElectronDataVisualizer::Factory);

			AddSortableFieldColumn(L"Order", &ElementData::order)
				->SetVisualizerFactory(mainFactory);

			AddStrongTypedColumn<WString>(L"Symbol", new ElementRadioactiveColumnProvider(this, &ElementData::symbol))
				->SetVisualizerFactory(subFactory);
			
			AddStrongTypedColumn<WString>(L"Chinese", new ElementRadioactiveColumnProvider(this, &ElementData::chinese))
				->SetVisualizerFactory(subFactory);
			
			AddStrongTypedColumn<WString>(L"English", new ElementRadioactiveColumnProvider(this, &ElementData::english))
				->SetVisualizerFactory(subFactory)
				->SetSize(120);

			AddSortableFieldColumn(L"Weight", &ElementData::weight)
				->SetVisualizerFactory(subFactory);

			AddFieldColumn(L"Valence", &ElementData::valence)
				->SetVisualizerFactory(subFactory)
				->SetSize(100);

			AddStrongTypedColumn<WString>(L"Electron", new ElementElectronColumnProvider(this))
				->SetVisualizerFactory(subFactory)
				->SetSize(160);

			AddStrongTypedColumn<WString>(L"Electron Graph", new ElementElectronColumnProvider(this))
				->SetVisualizerFactory(eecFactory)
				->SetSize(160);

			FileStream fileStream(L"..\\GacUISrcCodepackedTest\\Resources\\Chemical.txt", FileStream::ReadOnly);
			BomDecoder decoder;
			DecoderStream decoderStream(fileStream, decoder);
			StreamReader reader(decoderStream);

			Regex regexEec(ElementElectron::RegexEec);
			while(!reader.IsEnd())
			{
				WString line=reader.ReadLine();
				elements.Add(ElementData::Parse(line, regexEec));
			}
		}

		void GetRowData(vint row, ElementData& rowData)override
		{
			rowData=elements[row];
		}

		vint GetRowCount()override
		{
			return elements.Count();
		}
	};

	class TestWindow : public GuiWindow
	{
	private:
		GuiVirtualDataGrid*					dataGrid;
	public:
		TestWindow()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		{
			SetText(GetApplication()->GetExecutableFolder());
			GetContainerComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			GetContainerComposition()->SetPreferredMinSize(Size(1048, 576));
			ForceCalculateSizeImmediately();
			MoveToScreenCenter();

			dataGrid=new GuiVirtualDataGrid(GetCurrentTheme()->CreateListViewStyle(), new DataProvider);
			dataGrid->GetBoundsComposition()->SetAlignmentToParent(Margin(5, 5, 5, 5));
			dataGrid->SetHorizontalAlwaysVisible(false);
			dataGrid->SetVerticalAlwaysVisible(false);
			AddChild(dataGrid);
		}

		~TestWindow()
		{
		}
	};
}
using namespace test;

void GuiMain()
{
	TestWindow window;
	GetApplication()->Run(&window);
}