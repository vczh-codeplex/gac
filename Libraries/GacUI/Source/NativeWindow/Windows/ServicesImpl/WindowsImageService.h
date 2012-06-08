/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Native Window::Windows Implementation

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWS_SERVICESIMPL_WINDOWSIMAGESERIVCE
#define VCZH_PRESENTATION_WINDOWS_SERVICESIMPL_WINDOWSIMAGESERIVCE

#include "..\..\GuiNativeWindow.h"
#include <windows.h>
#include <wincodec.h>

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
			class WindowsImageFrame : public Object, public INativeImageFrame
			{
			protected:
				INativeImage*													image;
				ComPtr<IWICBitmap>												frameBitmap;
				collections::Dictionary<void*, Ptr<INativeImageFrameCache>>		caches;

				void										Initialize(IWICBitmapSource* bitmapSource);
			public:
				WindowsImageFrame(INativeImage* _image, IWICBitmapFrameDecode* frameDecode);
				WindowsImageFrame(INativeImage* _image, IWICBitmap* sourceBitmap);
				~WindowsImageFrame();
				INativeImage*								GetImage();
				Size										GetSize();
				bool										SetCache(void* key, Ptr<INativeImageFrameCache> cache);
				Ptr<INativeImageFrameCache>					GetCache(void* key);
				Ptr<INativeImageFrameCache>					RemoveCache(void* key);
				IWICBitmap*									GetFrameBitmap();
			};

			class WindowsImage : public Object, public INativeImage
			{
			protected:
				INativeImageService*						imageService;
				ComPtr<IWICBitmapDecoder>					bitmapDecoder;
				collections::Array<Ptr<WindowsImageFrame>>	frames;
			public:
				WindowsImage(INativeImageService* _imageService, IWICBitmapDecoder* _bitmapDecoder);
				~WindowsImage();
				INativeImageService*						GetImageService();
				FormatType									GetFormat();
				int											GetFrameCount();
				INativeImageFrame*							GetFrame(int index);
			};

			class WindowsBitmapImage : public Object, public INativeImage
			{
			protected:
				INativeImageService*						imageService;
				Ptr<WindowsImageFrame>						frame;
				FormatType									formatType;
			public:
				WindowsBitmapImage(INativeImageService* _imageService, IWICBitmap* sourceBitmap, FormatType _formatType);
				~WindowsBitmapImage();
				INativeImageService*						GetImageService();
				FormatType									GetFormat();
				int											GetFrameCount();
				INativeImageFrame*							GetFrame(int index);
			};

			class WindowsImageService : public Object, public INativeImageService
			{
			protected:
				ComPtr<IWICImagingFactory>					imagingFactory;
			public:
				WindowsImageService();
				~WindowsImageService();
				Ptr<INativeImage>							CreateImageFromFile(const WString& path);
				Ptr<INativeImage>							CreateImageFromHBITMAP(HBITMAP handle);
				Ptr<INativeImage>							CreateImageFromHICON(HICON handle);
				IWICImagingFactory*							GetImagingFactory();
			};

			extern IWICImagingFactory*						GetWICImagingFactory();
			extern IWICBitmap*								GetWICBitmap(INativeImageFrame* frame);
			extern Ptr<INativeImage>						CreateImageFromHBITMAP(HBITMAP handle);
			extern Ptr<INativeImage>						CreateImageFromHICON(HICON handle);
		}
	}
}

#endif