/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Native Window::Windows Implementation

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWS_WINNATIVEWINDOW
#define VCZH_PRESENTATION_WINDOWS_WINNATIVEWINDOW

#include "..\GuiNativeWindow.h"
#include <windows.h>
#include <wincodec.h>

namespace vl
{
	namespace presentation
	{
		namespace windows
		{

/***********************************************************************
Windows Platform Native Controller
***********************************************************************/

			class IWindowsForm : public Interface
			{
			public:
				virtual HWND								GetWindowHandle()=0;
				virtual Interface*							GetGraphicsHandler()=0;
				virtual void								SetGraphicsHandler(Interface* handler)=0;
			};

			extern INativeController*						CreateWindowsNativeController(HINSTANCE hInstance);
			extern IWindowsForm*							GetWindowsForm(INativeWindow* window);
			extern void										DestroyWindowsNativeController(INativeController* controller);

			extern IWICImagingFactory*						GetWICImagingFactory();
			extern IWICBitmap*								GetWICBitmap(INativeImageFrame* frame);
			extern Ptr<INativeImage>						CreateImageFromHBITMAP(HBITMAP handle);
			extern Ptr<INativeImage>						CreateImageFromHICON(HICON handle);
		}
	}
}

#endif