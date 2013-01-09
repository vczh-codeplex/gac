#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "..\..\Source\GacUI.h"
#include "..\..\Source\NativeWindow\Windows\WinNativeWindow.h"
#include "..\..\..\..\Common\Source\Stream\FileStream.h"
#include "..\..\..\..\Common\Source\Stream\Accessor.h"
#include "..\..\..\..\Common\Source\Stream\CharFormat.h"
#include "..\..\..\..\Common\Source\Parsing\ParsingTable.h"
#include <Windows.h>
#include <msctf.h>

using namespace vl::collections;
using namespace vl::stream;
using namespace vl::regex;
using namespace vl::parsing;
using namespace vl::parsing::tabling;

#define GUI_GRAPHICS_RENDERER_DIRECT2D

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
#ifdef GUI_GRAPHICS_RENDERER_GDI
	int result=SetupWindowsGDIRenderer();
#endif
#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D
	int result=SetupWindowsDirect2DRenderer();
#endif
}

namespace test
{
#pragma warning(push)
#pragma warning(disable:4584)
	class TextStoreACP : public IUnknown, public ITextStoreACP, public ITfContextOwnerCompositionSink
	{
#pragma warning(pop)
	public:
		ITextStoreACPSink*					textStoreCallback;
		HWND								hwnd;

		TextStoreACP()
			:textStoreCallback(0)
			,hwnd(0)
		{
		}

		virtual HRESULT STDMETHODCALLTYPE OnStartComposition( 
            /* [in] */ __RPC__in_opt ITfCompositionView *pComposition,
            /* [out] */ __RPC__out BOOL *pfOk)
		{
			return S_OK;
		}
        
        virtual HRESULT STDMETHODCALLTYPE OnUpdateComposition( 
            /* [in] */ __RPC__in_opt ITfCompositionView *pComposition,
            /* [in] */ __RPC__in_opt ITfRange *pRangeNew)
		{
			return S_OK;
		}
        
        virtual HRESULT STDMETHODCALLTYPE OnEndComposition( 
            /* [in] */ __RPC__in_opt ITfCompositionView *pComposition)
		{
			return S_OK;
		}

		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject)
		{
			if(ppvObject)
			{
				if(riid==IID_IUnknown)
				{
					*ppvObject=static_cast<IUnknown*>(this);
					return S_OK;
				}
				else if(riid==IID_ITextStoreACP)
				{
					*ppvObject=static_cast<ITextStoreACP*>(this);
					return S_OK;
				}
				else if(riid==IID_ITfContextOwnerCompositionSink)
				{
					*ppvObject=static_cast<ITfContextOwnerCompositionSink*>(this);
					return S_OK;
				}
			}
			return E_NOINTERFACE;
		}

        virtual ULONG STDMETHODCALLTYPE AddRef( void)
		{
			return S_OK;
		}

        virtual ULONG STDMETHODCALLTYPE Release( void)
		{
			return S_OK;
		}

		virtual HRESULT STDMETHODCALLTYPE AdviseSink( 
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][in] */ __RPC__in_opt IUnknown *punk,
            /* [in] */ DWORD dwMask)
		{
			if(textStoreCallback) return E_INVALIDARG;
			if(!punk) return E_INVALIDARG;
			ITextStoreACPSink* sink=0;
			HRESULT hr=punk->QueryInterface<ITextStoreACPSink>(&sink);
			if(hr!=S_OK || !sink) return E_UNEXPECTED;
			textStoreCallback=sink;
			textStoreCallback->AddRef();
			return S_OK;
		}
        
        virtual HRESULT STDMETHODCALLTYPE UnadviseSink( 
            /* [in] */ __RPC__in_opt IUnknown *punk)
		{
			if(!textStoreCallback) return E_INVALIDARG;
			if(!punk) return E_INVALIDARG;
			ITextStoreACPSink* sink=0;
			HRESULT hr=punk->QueryInterface<ITextStoreACPSink>(&sink);
			if(hr!=S_OK || !sink) return E_UNEXPECTED;
			if(sink==textStoreCallback)
			{
				textStoreCallback->Release();
				textStoreCallback=0;
			}
			sink->Release();
			return S_OK;
		}
        
        virtual HRESULT STDMETHODCALLTYPE RequestLock( 
            /* [in] */ DWORD dwLockFlags,
            /* [out] */ __RPC__out HRESULT *phrSession)
		{
			if(!textStoreCallback) return E_FAIL;
			if(!phrSession) return E_INVALIDARG;
			HRESULT hr=textStoreCallback->OnLockGranted(dwLockFlags);
			if(hr!=S_OK) return E_FAIL;
			*phrSession=hr;
			return S_OK;
		}
        
        virtual HRESULT STDMETHODCALLTYPE GetStatus( 
            /* [out] */ __RPC__out TS_STATUS *pdcs)
		{
			if(!pdcs) return E_INVALIDARG;
			pdcs->dwDynamicFlags=0;
			pdcs->dwStaticFlags=TS_SS_TRANSITORY | TS_SS_NOHIDDENTEXT;
			return S_OK;
		}
        
        virtual HRESULT STDMETHODCALLTYPE QueryInsert( 
            /* [in] */ LONG acpTestStart,
            /* [in] */ LONG acpTestEnd,
            /* [in] */ ULONG cch,
            /* [out] */ __RPC__out LONG *pacpResultStart,
            /* [out] */ __RPC__out LONG *pacpResultEnd)
		{
			*pacpResultStart=0;
			*pacpResultEnd=0;
			return S_OK;
		}
        
        virtual HRESULT STDMETHODCALLTYPE GetSelection( 
            /* [in] */ ULONG ulIndex,
            /* [in] */ ULONG ulCount,
            /* [length_is][size_is][out] */ __RPC__out_ecount_part(ulCount, *pcFetched) TS_SELECTION_ACP *pSelection,
            /* [out] */ __RPC__out ULONG *pcFetched)
		{
			if(!pcFetched) return E_INVALIDARG;
			*pcFetched=0;
			return S_OK;
		}
        
        virtual HRESULT STDMETHODCALLTYPE SetSelection( 
            /* [in] */ ULONG ulCount,
            /* [size_is][in] */ __RPC__in_ecount_full(ulCount) const TS_SELECTION_ACP *pSelection)
		{
			return S_OK;
		}
        
        virtual HRESULT STDMETHODCALLTYPE GetText( 
            /* [in] */ LONG acpStart,
            /* [in] */ LONG acpEnd,
            /* [length_is][size_is][out] */ __RPC__out_ecount_part(cchPlainReq, *pcchPlainRet) WCHAR *pchPlain,
            /* [in] */ ULONG cchPlainReq,
            /* [out] */ __RPC__out ULONG *pcchPlainRet,
            /* [length_is][size_is][out] */ __RPC__out_ecount_part(cRunInfoReq, *pcRunInfoRet) TS_RUNINFO *prgRunInfo,
            /* [in] */ ULONG cRunInfoReq,
            /* [out] */ __RPC__out ULONG *pcRunInfoRet,
            /* [out] */ __RPC__out LONG *pacpNext)
		{
			if(!pcchPlainRet) return E_INVALIDARG;
			if(!pcRunInfoRet) return E_INVALIDARG;
			if(!pacpNext) return E_INVALIDARG;
			*pcchPlainRet=0;
			*pcRunInfoRet=0;
			*pacpNext=0;
			return S_OK;
		}
        
        virtual HRESULT STDMETHODCALLTYPE SetText( 
            /* [in] */ DWORD dwFlags,
            /* [in] */ LONG acpStart,
            /* [in] */ LONG acpEnd,
            /* [size_is][in] */ __RPC__in_ecount_full(cch) const WCHAR *pchText,
            /* [in] */ ULONG cch,
            /* [out] */ __RPC__out TS_TEXTCHANGE *pChange)
		{
			return S_OK;
		}
        
        virtual HRESULT STDMETHODCALLTYPE GetFormattedText( 
            /* [in] */ LONG acpStart,
            /* [in] */ LONG acpEnd,
            /* [out] */ __RPC__deref_out_opt IDataObject **ppDataObject)
		{
			if(!ppDataObject) return E_INVALIDARG;
			*ppDataObject=0;
			return S_OK;
		}
        
        virtual HRESULT STDMETHODCALLTYPE GetEmbedded( 
            /* [in] */ LONG acpPos,
            /* [in] */ __RPC__in REFGUID rguidService,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ __RPC__deref_out_opt IUnknown **ppunk)
		{
			return E_NOTIMPL;
		}
        
        virtual HRESULT STDMETHODCALLTYPE QueryInsertEmbedded( 
            /* [in] */ __RPC__in const GUID *pguidService,
            /* [in] */ __RPC__in const FORMATETC *pFormatEtc,
            /* [out] */ __RPC__out BOOL *pfInsertable) 
		{
			if(!pfInsertable) return E_INVALIDARG;
			*pfInsertable=FALSE;
			return S_OK;
		}
        
        virtual HRESULT STDMETHODCALLTYPE InsertEmbedded( 
            /* [in] */ DWORD dwFlags,
            /* [in] */ LONG acpStart,
            /* [in] */ LONG acpEnd,
            /* [in] */ __RPC__in_opt IDataObject *pDataObject,
            /* [out] */ __RPC__out TS_TEXTCHANGE *pChange)
		{
			return E_NOTIMPL;
		}
        
        virtual HRESULT STDMETHODCALLTYPE InsertTextAtSelection( 
            /* [in] */ DWORD dwFlags,
            /* [size_is][in] */ __RPC__in_ecount_full(cch) const WCHAR *pchText,
            /* [in] */ ULONG cch,
            /* [out] */ __RPC__out LONG *pacpStart,
            /* [out] */ __RPC__out LONG *pacpEnd,
            /* [out] */ __RPC__out TS_TEXTCHANGE *pChange)
		{
			if(!pacpStart) return E_INVALIDARG;
			if(!pacpEnd) return E_INVALIDARG;
			if(!pChange) return E_INVALIDARG;
			*pacpStart=0;
			*pacpEnd=0;
			pChange->acpStart=0;
			pChange->acpOldEnd=0;
			pChange->acpNewEnd=0;
			return S_OK;
		}
        
        virtual HRESULT STDMETHODCALLTYPE InsertEmbeddedAtSelection( 
            /* [in] */ DWORD dwFlags,
            /* [in] */ __RPC__in_opt IDataObject *pDataObject,
            /* [out] */ __RPC__out LONG *pacpStart,
            /* [out] */ __RPC__out LONG *pacpEnd,
            /* [out] */ __RPC__out TS_TEXTCHANGE *pChange)
		{
			return E_NOTIMPL;
		}
        
        virtual HRESULT STDMETHODCALLTYPE RequestSupportedAttrs( 
            /* [in] */ DWORD dwFlags,
            /* [in] */ ULONG cFilterAttrs,
            /* [unique][size_is][in] */ __RPC__in_ecount_full_opt(cFilterAttrs) const TS_ATTRID *paFilterAttrs)
		{
			return S_OK;
		}
        
        virtual HRESULT STDMETHODCALLTYPE RequestAttrsAtPosition( 
            /* [in] */ LONG acpPos,
            /* [in] */ ULONG cFilterAttrs,
            /* [unique][size_is][in] */ __RPC__in_ecount_full_opt(cFilterAttrs) const TS_ATTRID *paFilterAttrs,
            /* [in] */ DWORD dwFlags) 
		{
			return S_OK;
		}
        
        virtual HRESULT STDMETHODCALLTYPE RequestAttrsTransitioningAtPosition( 
            /* [in] */ LONG acpPos,
            /* [in] */ ULONG cFilterAttrs,
            /* [unique][size_is][in] */ __RPC__in_ecount_full_opt(cFilterAttrs) const TS_ATTRID *paFilterAttrs,
            /* [in] */ DWORD dwFlags)
		{
			return S_OK;
		}
        
        virtual HRESULT STDMETHODCALLTYPE FindNextAttrTransition( 
            /* [in] */ LONG acpStart,
            /* [in] */ LONG acpHalt,
            /* [in] */ ULONG cFilterAttrs,
            /* [unique][size_is][in] */ __RPC__in_ecount_full_opt(cFilterAttrs) const TS_ATTRID *paFilterAttrs,
            /* [in] */ DWORD dwFlags,
            /* [out] */ __RPC__out LONG *pacpNext,
            /* [out] */ __RPC__out BOOL *pfFound,
            /* [out] */ __RPC__out LONG *plFoundOffset)
		{
			return TS_E_INVALIDPOS;
		}
        
        virtual HRESULT STDMETHODCALLTYPE RetrieveRequestedAttrs( 
            /* [in] */ ULONG ulCount,
            /* [length_is][size_is][out] */ __RPC__out_ecount_part(ulCount, *pcFetched) TS_ATTRVAL *paAttrVals,
            /* [out] */ __RPC__out ULONG *pcFetched)
		{
			if(!pcFetched) return E_INVALIDARG;
			*pcFetched=0;
			return S_OK;
		}
        
        virtual HRESULT STDMETHODCALLTYPE GetEndACP( 
            /* [out] */ __RPC__out LONG *pacp)
		{
			if(!pacp) return E_INVALIDARG;
			*pacp=0;
			return S_OK;
		}
        
        virtual HRESULT STDMETHODCALLTYPE GetActiveView( 
            /* [out] */ __RPC__out TsViewCookie *pvcView)
		{
			if(!pvcView) return E_INVALIDARG;
			*pvcView=(TsViewCookie)this;
			return S_OK;
		}
        
        virtual HRESULT STDMETHODCALLTYPE GetACPFromPoint( 
            /* [in] */ TsViewCookie vcView,
            /* [in] */ __RPC__in const POINT *ptScreen,
            /* [in] */ DWORD dwFlags,
            /* [out] */ __RPC__out LONG *pacp)
		{
			if(!pacp) return E_INVALIDARG;
			*pacp=0;
			return S_OK;
		}
        
        virtual HRESULT STDMETHODCALLTYPE GetTextExt( 
            /* [in] */ TsViewCookie vcView,
            /* [in] */ LONG acpStart,
            /* [in] */ LONG acpEnd,
            /* [out] */ __RPC__out RECT *prc,
            /* [out] */ __RPC__out BOOL *pfClipped)
		{
			if(!prc) return E_INVALIDARG;
			if(!pfClipped) return E_INVALIDARG;
			prc->left=0;
			prc->top=0;
			prc->right=0;
			prc->bottom=0;
			*pfClipped=TRUE;
			return S_OK;
		}
        
        virtual HRESULT STDMETHODCALLTYPE GetScreenExt( 
            /* [in] */ TsViewCookie vcView,
            /* [out] */ __RPC__out RECT *prc)
		{
			if(!prc) return E_INVALIDARG;
			prc->left=0;
			prc->top=0;
			prc->right=0;
			prc->bottom=0;
			return S_OK;
		}
        
        virtual HRESULT STDMETHODCALLTYPE GetWnd( 
            /* [in] */ TsViewCookie vcView,
            /* [out] */ __RPC__deref_out_opt HWND *phwnd)
		{
			if(!phwnd) return E_INVALIDARG;
			*phwnd=hwnd;
			return S_OK;
		}
	};

	class TestWindow : public GuiWindow
	{
	private:
		// global
		ITfThreadMgr*						threadManager;
		GuiMultilineTextBox*				textBox;

		// local
		ITfDocumentMgr*						documentManager;
		ITfContext*							context;
		TfEditCookie						editCookie;
		TfClientId							tid;
		TextStoreACP						textStore;

		void OnLeftButtonDown(GuiGraphicsComposition* sender, GuiMouseEventArgs& arguments)
		{
			BOOL focus;
			HRESULT hr=threadManager->IsThreadFocus(&focus);
			if(hr==S_OK)
			{
				SetText(WString(L"IsThreadFocus: ")+(focus==FALSE?L"FALSE":L"TRUE"));
			}
		}

		void OnGotFocus(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			threadManager->SetFocus(documentManager);
		}

		void OnLostFocus(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			threadManager->SetFocus(0);
		}
	public:
		TestWindow()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		{
			SetText(L"GacUISrc Test Application");
			SetClientSize(Size(640, 480));
			GetBoundsComposition()->SetPreferredMinSize(Size(320, 240));
			GetBoundsComposition()->GetEventReceiver()->leftButtonDown.AttachMethod(this, &TestWindow::OnLeftButtonDown);
			this->WindowGotFocus.AttachMethod(this, &TestWindow::OnGotFocus);
			this->WindowLostFocus.AttachMethod(this, &TestWindow::OnLostFocus);
			MoveToScreenCenter();

			textBox=g::NewMultilineTextBox();
			textBox->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			this->AddChild(textBox);

			HWND hwnd=windows::GetWindowsForm(GetNativeWindow())->GetWindowHandle();
			textStore.hwnd=hwnd;

			threadManager=0;
			documentManager=0;
			context=0;
			editCookie=0;
			tid=0;

			HRESULT hr=S_OK;
			hr=CoCreateInstance(CLSID_TF_ThreadMgr, NULL, CLSCTX_INPROC_SERVER, IID_ITfThreadMgr, (LPVOID*)&threadManager);
			hr=threadManager->Activate(&tid);
			hr=threadManager->CreateDocumentMgr(&documentManager);
			hr=documentManager->CreateContext(tid, 0, static_cast<IUnknown*>(&textStore), &context, &editCookie);
			hr=documentManager->Push(context);
		}
	};
}
using namespace test;

void GuiMain()
{
	TestWindow window;
	GetApplication()->Run(&window);
}