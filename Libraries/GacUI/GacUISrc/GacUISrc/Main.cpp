#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "..\..\Source\GacUI.h"
#include "..\..\Source\NativeWindow\Windows\WinNativeWindow.h"
#include <Windows.h>
#include <msctf.h>

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
	class TestWindow : public GuiWindow, private ITfThreadMgrEventSink, private ITfContextOwnerCompositionSink
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

	private:
		GuiTab*					tab;
		GuiButton*				buttonAdd;
		GuiButton*				buttonRemove;
		vint					counter;

		void buttonAdd_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			buttonRemove->SetEnabled(true);
			GuiTabPage* page=tab->CreatePage();
			page->SetText(L"Page "+itow(++counter));
		}

		void buttonRemove_Clicked(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			GuiTabPage* page=tab->GetSelectedPage();
			tab->RemovePage(page);
			if(tab->GetPages().Count()==0)
			{
				buttonRemove->SetEnabled(false);
			}
			delete page;
		}
	public:
		TestWindow()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
			,clientId(0)
			,counter(0)
		{
			SetText(GetApplication()->GetExecutableFolder());
			GetContainerComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			GetContainerComposition()->SetPreferredMinSize(Size(640, 480));
			ForceCalculateSizeImmediately();
			MoveToScreenCenter();

			GuiTableComposition* table=new GuiTableComposition;
			this->GetContainerComposition()->AddChild(table);
			table->SetAlignmentToParent(Margin(0, 0, 0, 0));
			table->SetRowsAndColumns(2, 3);
			table->SetCellPadding(5);
			table->SetRowOption(0, GuiCellOption::MinSizeOption());
			table->SetRowOption(1, GuiCellOption::PercentageOption(1.0));
			table->SetColumnOption(0, GuiCellOption::MinSizeOption());
			table->SetColumnOption(1, GuiCellOption::MinSizeOption());
			table->SetColumnOption(2, GuiCellOption::PercentageOption(1.0));
			{
				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetSite(0, 0, 1, 1);

				buttonAdd=g::NewButton();
				buttonAdd->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				buttonAdd->SetText(L"Add Tab Page");
				buttonAdd->Clicked.AttachMethod(this, &TestWindow::buttonAdd_Clicked);
				cell->AddChild(buttonAdd->GetBoundsComposition());
			}
			{
				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetSite(0, 1, 1, 1);

				buttonRemove=g::NewButton();
				buttonRemove->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				buttonRemove->SetText(L"Remove Current Tab Page");
				buttonRemove->Clicked.AttachMethod(this, &TestWindow::buttonRemove_Clicked);
				cell->AddChild(buttonRemove->GetBoundsComposition());
			}
			{
				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetSite(1, 0, 1, 3);

				tab=g::NewTab();
				tab->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				cell->AddChild(tab->GetBoundsComposition());
			}
			for(vint i=0;i<3;i++)
			{
				GuiEventArgs arguments=buttonAdd->GetNotifyEventArguments();
				buttonAdd->Clicked.Execute(arguments);
			}

			Initialize();
		}

		~TestWindow()
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
}
using namespace test;

void GuiMain()
{
	TestWindow window;
	GetApplication()->Run(&window);
}