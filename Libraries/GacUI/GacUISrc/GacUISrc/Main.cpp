#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "..\..\Source\GacUI.h"
#include "..\..\Source\NativeWindow\Windows\WinNativeWindow.h"
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
TsfTestWindow
***********************************************************************/

	class TsfTestWindow : public GuiWindow, private ITfThreadMgrEventSink, private ITfContextOwnerCompositionSink
	{
	private:
		ComPtr<ITfThreadMgr>				threadManager;
		ComPtr<ITfSource>					threadManagerSource;
		ComPtr<ITfDocumentMgr>				documentManager;
		ComPtr<ITfContext>					editContext;
		TfClientId							clientId;
		TfEditCookie						editCookie;

		IUnknown* GetUnknown()
		{
			return static_cast<IUnknown*>(static_cast<ITfThreadMgrEventSink*>(this));
		}

		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject)
		{
			if(!ppvObject)
			{
				return E_POINTER;
			}
			if(riid==IID_IUnknown)
			{
				*ppvObject=GetUnknown();
			}
			else if(riid==IID_ITfThreadMgrEventSink)
			{
				*ppvObject=static_cast<ITfThreadMgrEventSink*>(this);
			}
			else if(riid==IID_ITfContextOwnerCompositionSink)
			{
				*ppvObject=static_cast<ITfContextOwnerCompositionSink*>(this);
			}
			else
			{
				*ppvObject=NULL;
				return E_NOINTERFACE;
			}
			return S_OK;
		}

		ULONG STDMETHODCALLTYPE AddRef(void)
		{
			return 1;
		}

		ULONG STDMETHODCALLTYPE Release(void)
		{
			return 1;
		}

		HRESULT STDMETHODCALLTYPE OnInitDocumentMgr(ITfDocumentMgr *pdim)
		{
			return S_OK;
		}
        
		HRESULT STDMETHODCALLTYPE OnUninitDocumentMgr(ITfDocumentMgr *pdim)
		{
			return S_OK;
		}
        
		HRESULT STDMETHODCALLTYPE OnSetFocus(ITfDocumentMgr *pdimFocus, ITfDocumentMgr *pdimPrevFocus)
		{
			return S_OK;
		}
        
		HRESULT STDMETHODCALLTYPE OnPushContext(ITfContext *pic)
		{
			return S_OK;
		}
        
		HRESULT STDMETHODCALLTYPE OnPopContext(ITfContext *pic)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE OnStartComposition(ITfCompositionView *pComposition, BOOL *pfOk)
		{
			return S_OK;
		}
        
		HRESULT STDMETHODCALLTYPE OnUpdateComposition(ITfCompositionView *pComposition, ITfRange *pRangeNew)
		{
			return S_OK;
		}
        
		HRESULT STDMETHODCALLTYPE OnEndComposition(ITfCompositionView *pComposition)
		{
			return S_OK;
		}

	public:
		TsfTestWindow()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
			,clientId(0)
		{
			SetText(GetApplication()->GetExecutableFolder());
			GetContainerComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			GetContainerComposition()->SetPreferredMinSize(Size(640, 480));
			ForceCalculateSizeImmediately();
			MoveToScreenCenter();

			Initialize();
		}

		~TsfTestWindow()
		{
		}

		void Initialize()
		{
			ITfThreadMgr*		pThreadMgr=0;
			ITfSource*			pSource=0;
			ITfDocumentMgr*		pDocumentMgr=0;
			ITfContext*			pContext=0;

			HRESULT hr=CoCreateInstance(
				CLSID_TF_ThreadMgr, 
                NULL, 
                CLSCTX_INPROC_SERVER, 
                IID_ITfThreadMgr, 
                (void**)&pThreadMgr
				);
			hr=pThreadMgr->QueryInterface(IID_ITfSource, (void**)&pSource);
			DWORD cookie=0;
			hr=pSource->AdviseSink(IID_ITfThreadMgrEventSink, static_cast<ITfThreadMgrEventSink*>(this), &cookie);
			hr=pThreadMgr->Activate(&clientId);
			hr=pThreadMgr->CreateDocumentMgr(&pDocumentMgr);
			hr=pDocumentMgr->CreateContext(clientId, 0, GetUnknown(), &pContext, &editCookie);
			hr=pDocumentMgr->Push(pContext);
			{
				HWND hwnd=windows::GetWindowsForm(GetNativeWindow())->GetWindowHandle();
				ITfDocumentMgr* pPrevious=0;
				hr=pThreadMgr->AssociateFocus(hwnd, pDocumentMgr, &pPrevious);
				if(pPrevious)
				{
					pPrevious->Release();
				}
			}

			threadManager=pThreadMgr;
			threadManagerSource=pSource;
			documentManager=pDocumentMgr;
			editContext=pContext;
		}
	};

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
	};

	class DataProvider : public list::StrongTypedDataProvider<ElementData>
	{
	protected:
		List<ElementData>							elements;
		Ptr<list::IDataVisualizerFactory>			mainFactory;
		Ptr<list::IDataVisualizerFactory>			subFactory;
	public:
		DataProvider()
		{
			mainFactory=new list::CellBorderDataVisualizer::Factory(new list::ListViewMainColumnDataVisualizer::Factory);
			subFactory=new list::CellBorderDataVisualizer::Factory(new list::ListViewSubColumnDataVisualizer::Factory);

			AddSortableFieldColumn(L"Order", &ElementData::order)
				->SetVisualizerFactory(mainFactory);

			AddStrongTypedColumn<WString>(L"Symbol", new ElementRadioactiveColumnProvider(this, &ElementData::symbol))
				->SetVisualizerFactory(subFactory);
			
			AddStrongTypedColumn<WString>(L"Chinese", new ElementRadioactiveColumnProvider(this, &ElementData::chinese))
				->SetVisualizerFactory(subFactory);
			
			AddStrongTypedColumn<WString>(L"English", new ElementRadioactiveColumnProvider(this, &ElementData::english))
				->SetVisualizerFactory(subFactory)
				->SetSize(100);

			AddSortableFieldColumn(L"Weight", &ElementData::weight)
				->SetVisualizerFactory(subFactory);

			AddFieldColumn(L"Valence", &ElementData::valence)
				->SetVisualizerFactory(subFactory)
				->SetSize(100);

			AddStrongTypedColumn<WString>(L"Electron", new ElementElectronColumnProvider(this))
				->SetVisualizerFactory(subFactory)
				->SetSize(140);

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