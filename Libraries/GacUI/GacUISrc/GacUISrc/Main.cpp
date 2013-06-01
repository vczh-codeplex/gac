#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "..\..\Source\GacUI.h"
#include "..\..\Source\NativeWindow\Windows\WinNativeWindow.h"
#include <Windows.h>
#include <msctf.h>

using namespace vl::collections;

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

	struct ElementData
	{
		vint		order;
		WString		symbol;
		WString		chinese;
		WString		english;
		double		weight;
		WString		valence;
		WString		electron;
	};

	class DataProvider : public list::StrongTypedDataProvider<ElementData>
	{
	protected:
		List<ElementData>							elements;
	public:
		DataProvider()
		{
			AddSortableFieldColumn(L"Order", &ElementData::order);
			AddFieldColumn(L"Symbol", &ElementData::symbol);
			AddFieldColumn(L"Chinese", &ElementData::chinese);
			AddFieldColumn(L"English", &ElementData::english);
			AddSortableFieldColumn(L"Weight", &ElementData::weight);
			AddFieldColumn(L"Valence", &ElementData::valence);
			AddFieldColumn(L"Electron", &ElementData::electron);
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