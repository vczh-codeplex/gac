/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Native Window::Direct2D Provider for Windows Implementation::Renderer

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSWINDOWSDIRECT2D
#define VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSWINDOWSDIRECT2D

#include <D2D1.h>
#include <DWrite.h>
#include <wincodec.h>
#include "..\GuiGraphicsElement.h"
#include "..\GuiGraphicsComposition.h"

namespace vl
{
	namespace presentation
	{
		namespace elements_windows_d2d
		{

/***********************************************************************
Functionality
***********************************************************************/

			class IWindowsDirect2DRenderTarget : public elements::IGuiGraphicsRenderTarget
			{
			public:
				virtual ID2D1RenderTarget*					GetDirect2DRenderTarget()=0;
				virtual ComPtr<ID2D1Bitmap>					GetBitmap(INativeImageFrame* frame)=0;
				virtual void								DestroyBitmapCache(INativeImageFrame* frame)=0;

				virtual ID2D1SolidColorBrush*				CreateDirect2DBrush(Color color)=0;
				virtual void								DestroyDirect2DBrush(Color color)=0;
				virtual ID2D1LinearGradientBrush*			CreateDirect2DLinearBrush(Color c1, Color c2)=0;
				virtual void								DestroyDirect2DLinearBrush(Color c1, Color c2)=0;
			};

			class Direct2DTextFormatPackage
			{
			public:
				ComPtr<IDWriteTextFormat>		textFormat;
				DWRITE_TRIMMING					trimming;
				ComPtr<IDWriteInlineObject>		ellipseInlineObject;
			};

			class IWindowsDirect2DResourceManager : public Interface
			{
			public:
				virtual Direct2DTextFormatPackage*			CreateDirect2DTextFormat(const FontProperties& fontProperties)=0;
				virtual void								DestroyDirect2DTextFormat(const FontProperties& fontProperties)=0;
				virtual Ptr<elements::text::CharMeasurer>	CreateDirect2DCharMeasurer(const FontProperties& fontProperties)=0;
				virtual void								DestroyDirect2DCharMeasurer(const FontProperties& fontProperties)=0;
			};

			extern IWindowsDirect2DResourceManager*			GetWindowsDirect2DResourceManager();
			extern D2D1::ColorF								GetD2DColor(Color color);

/***********************************************************************
OS Supporting
***********************************************************************/

			class IWindowsDirect2DObjectProvider : public Interface
			{
			public:
				virtual ID2D1RenderTarget*					GetNativeWindowDirect2DRenderTarget(INativeWindow* window)=0;
				virtual ID2D1Factory*						GetDirect2DFactory()=0;
				virtual IDWriteFactory*						GetDirectWriteFactory()=0;
				virtual IWindowsDirect2DRenderTarget*		GetBindedRenderTarget(INativeWindow* window)=0;
				virtual void								SetBindedRenderTarget(INativeWindow* window, IWindowsDirect2DRenderTarget* renderTarget)=0;
				virtual IWICImagingFactory*					GetWICImagingFactory()=0;
				virtual IWICBitmapDecoder*					GetWICBitmapDecoder(INativeImage* image)=0;
				virtual IWICBitmap*							GetWICBitmap(INativeImageFrame* frame)=0;
			};

			extern IWindowsDirect2DObjectProvider*			GetWindowsDirect2DObjectProvider();
			extern void										SetWindowsDirect2DObjectProvider(IWindowsDirect2DObjectProvider* provider);
		}
	}
}

extern void RendererMainDirect2D();

#endif