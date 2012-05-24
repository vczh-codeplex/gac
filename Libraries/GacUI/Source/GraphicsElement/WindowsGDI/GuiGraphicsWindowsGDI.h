/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Native Window::GDI Provider for Windows Implementation::Renderer

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSWINDOWSGDI
#define VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSWINDOWSGDI

#include <wincodec.h>
#include "..\..\NativeWindow\Windows\GDI\WinGDI.h"
#include "..\GuiGraphicsElement.h"
#include "..\GuiGraphicsTextElement.h"
#include "..\GuiGraphicsComposition.h"

namespace vl
{
	namespace presentation
	{
		namespace elements_windows_gdi
		{

/***********************************************************************
Functionality
***********************************************************************/

			class IWindowsGDIRenderTarget : public elements::IGuiGraphicsRenderTarget
			{
			public:
				virtual windows::WinDC*					GetDC()=0;
			};

			class IWindowsGDIResourceManager : public Interface
			{
			public:
				virtual Ptr<windows::WinPen>				CreateGdiPen(Color color)=0;
				virtual void								DestroyGdiPen(Color color)=0;
				virtual Ptr<windows::WinBrush>				CreateGdiBrush(Color color)=0;
				virtual void								DestroyGdiBrush(Color color)=0;
				virtual Ptr<windows::WinFont>				CreateGdiFont(const FontProperties& fontProperties)=0;
				virtual void								DestroyGdiFont(const FontProperties& fontProperties)=0;
				virtual Ptr<elements::text::CharMeasurer>	CreateCharMeasurer(const FontProperties& fontProperties)=0;
				virtual void								DestroyCharMeasurer(const FontProperties& fontProperties)=0;

				virtual Ptr<windows::WinBitmap>				GetBitmap(INativeImageFrame* frame)=0;
				virtual void								DestroyBitmapCache(INativeImageFrame* frame)=0;
			};

			extern IWindowsGDIResourceManager*			GetWindowsGDIResourceManager();

/***********************************************************************
OS Supporting
***********************************************************************/

			class IWindowsGDIObjectProvider : public Interface
			{
			public:
				virtual windows::WinDC*						GetNativeWindowDC(INativeWindow* window)=0;
				virtual IWindowsGDIRenderTarget*			GetBindedRenderTarget(INativeWindow* window)=0;
				virtual void								SetBindedRenderTarget(INativeWindow* window, IWindowsGDIRenderTarget* renderTarget)=0;
				virtual IWICImagingFactory*					GetWICImagingFactory()=0;
				virtual IWICBitmap*							GetWICBitmap(INativeImageFrame* frame)=0;
			};

			extern IWindowsGDIObjectProvider*				GetWindowsGDIObjectProvider();
			extern void										SetWindowsGDIObjectProvider(IWindowsGDIObjectProvider* provider);
		}
	}
}

extern void RendererMainGDI();

#endif