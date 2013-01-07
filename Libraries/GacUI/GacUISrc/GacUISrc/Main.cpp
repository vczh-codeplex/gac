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
	class TestWindow : public GuiWindow, ITfContextOwnerCompositionSink
	{
	private:
		ITfThreadMgr* threadManager;
		ITfDocumentMgr* documentManager;
		ITfDocumentMgr* previousDocumentManager;
		ITfContext* context;
		TfEditCookie editCookie;
		TfClientId tid;

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

		HRESULT STDMETHODCALLTYPE OnStartComposition( 
            /* [in] */ __RPC__in_opt ITfCompositionView *pComposition,
            /* [out] */ __RPC__out BOOL *pfOk)
		{
			return S_OK;
		}
        
        HRESULT STDMETHODCALLTYPE OnUpdateComposition( 
            /* [in] */ __RPC__in_opt ITfCompositionView *pComposition,
            /* [in] */ __RPC__in_opt ITfRange *pRangeNew)
		{
			return S_OK;
		}
        
        HRESULT STDMETHODCALLTYPE OnEndComposition( 
            /* [in] */ __RPC__in_opt ITfCompositionView *pComposition)
		{
			return S_OK;
		}

		void OnLeftButtonDown(GuiGraphicsComposition* sender, GuiMouseEventArgs& arguments)
		{
			::SetFocus(windows::GetWindowsForm(GetNativeWindow())->GetWindowHandle());
			BOOL focus;
			HRESULT hr=threadManager->IsThreadFocus(&focus);
		}
	public:
		TestWindow()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		{
			SetText(L"GacUISrc Test Application");
			SetClientSize(Size(640, 480));
			GetBoundsComposition()->SetPreferredMinSize(Size(320, 240));
			GetBoundsComposition()->GetEventReceiver()->leftButtonDown.AttachMethod(this, &TestWindow::OnLeftButtonDown);
			MoveToScreenCenter();

			threadManager=0;
			documentManager=0;
			previousDocumentManager=0;
			context=0;
			editCookie=0;
			tid=0;

			HRESULT hr=S_OK;
			hr=CoCreateInstance(CLSID_TF_ThreadMgr, NULL, CLSCTX_INPROC_SERVER, IID_ITfThreadMgr, (LPVOID*)&threadManager);
			hr=threadManager->Activate(&tid);
			hr=threadManager->CreateDocumentMgr(&documentManager);
			hr=documentManager->CreateContext(tid, 0, static_cast<ITfContextOwnerCompositionSink*>(this), &context, &editCookie);
			hr=documentManager->Push(context);
			hr=threadManager->AssociateFocus(windows::GetWindowsForm(GetNativeWindow())->GetWindowHandle(), documentManager, &previousDocumentManager);
		}
	};
}
using namespace test;

void GuiMain()
{
	TestWindow window;
	GetApplication()->Run(&window);
}