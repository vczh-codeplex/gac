#include "GuiGraphicsWindowsDirect2D.h"
#include "GuiGraphicsRenderersWindowsDirect2D.h"
#include "GuiGraphicsLayoutProviderWindowsDirect2D.h"
#include "..\..\Controls\GuiApplication.h"
#include <math.h>

namespace vl
{
	namespace presentation
	{
		namespace elements
		{

/***********************************************************************
GuiDirect2DElement
***********************************************************************/

			GuiDirect2DElement::GuiDirect2DElement()
			{
			}

			GuiDirect2DElement::~GuiDirect2DElement()
			{
			}
		}

		namespace elements_windows_d2d
		{
			using namespace elements;
			using namespace collections;

			D2D1::ColorF GetD2DColor(Color color)
			{
				return D2D1::ColorF(color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f);
			}

/***********************************************************************
CachedResourceAllocator
***********************************************************************/

			class CachedSolidBrushAllocator
			{
				DEFINE_CACHED_RESOURCE_ALLOCATOR(Color, ComPtr<ID2D1SolidColorBrush>)

				IWindowsDirect2DRenderTarget*	guiRenderTarget;
			public:
				CachedSolidBrushAllocator()
					:guiRenderTarget(0)
				{
				}

				void SetRenderTarget(IWindowsDirect2DRenderTarget* _guiRenderTarget)
				{
					guiRenderTarget=_guiRenderTarget;
				}

				ComPtr<ID2D1SolidColorBrush> CreateInternal(Color color)
				{
					ID2D1SolidColorBrush* brush=0;
					ID2D1RenderTarget* renderTarget=guiRenderTarget->GetDirect2DRenderTarget();
					HRESULT hr=renderTarget->CreateSolidColorBrush(GetD2DColor(color), &brush);
					if(!FAILED(hr))
					{
						return brush;
					}
					else
					{
						return 0;
					}
				}
			};

			class CachedLinearBrushAllocator
			{
				typedef Pair<Color, Color> ColorPair;
				DEFINE_CACHED_RESOURCE_ALLOCATOR(ColorPair, ComPtr<ID2D1LinearGradientBrush>)

				IWindowsDirect2DRenderTarget*	guiRenderTarget;
			public:
				CachedLinearBrushAllocator()
					:guiRenderTarget(0)
				{
				}

				void SetRenderTarget(IWindowsDirect2DRenderTarget* _guiRenderTarget)
				{
					guiRenderTarget=_guiRenderTarget;
				}

				ComPtr<ID2D1LinearGradientBrush> CreateInternal(ColorPair colors)
				{
					ID2D1RenderTarget* renderTarget=guiRenderTarget->GetDirect2DRenderTarget();
					ID2D1GradientStopCollection* stopCollection=0;
					{
						D2D1_GRADIENT_STOP stops[2];
						stops[0].color=GetD2DColor(colors.key);
						stops[0].position=0.0f;
						stops[1].color=GetD2DColor(colors.value);
						stops[1].position=1.0f;

						HRESULT hr=renderTarget->CreateGradientStopCollection(
							stops,
							2,
							D2D1_GAMMA_2_2,
							D2D1_EXTEND_MODE_CLAMP,
							&stopCollection);
						if(FAILED(hr)) return 0;
					}

					ID2D1LinearGradientBrush* brush=0;
					{
						D2D1_POINT_2F points[2]={{0, 0}, {0, 0}};
						HRESULT hr=renderTarget->CreateLinearGradientBrush(
							D2D1::LinearGradientBrushProperties(points[0], points[1]),
							stopCollection,
							&brush);
						stopCollection->Release();
						if(FAILED(hr)) return 0;
					}
					return brush;
				}
			};

			class CachedTextFormatAllocator
			{
			private:
				DEFINE_CACHED_RESOURCE_ALLOCATOR(FontProperties, Ptr<Direct2DTextFormatPackage>)
			public:

				static ComPtr<IDWriteTextFormat> CreateDirect2DFont(const FontProperties& fontProperties)
				{
					IDWriteFactory* dwriteFactory=GetWindowsDirect2DObjectProvider()->GetDirectWriteFactory();
					IDWriteTextFormat* format=0;
					HRESULT hr=dwriteFactory->CreateTextFormat(
						fontProperties.fontFamily.Buffer(),
						NULL,
						(fontProperties.bold?DWRITE_FONT_WEIGHT_BOLD:DWRITE_FONT_WEIGHT_NORMAL),
						(fontProperties.italic?DWRITE_FONT_STYLE_ITALIC:DWRITE_FONT_STYLE_NORMAL),
						DWRITE_FONT_STRETCH_NORMAL,
						(FLOAT)fontProperties.size,
						L"",
						&format);
					if(!FAILED(hr))
					{
						format->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
						return format;
					}
					else
					{
						return 0;
					}
				}

				Ptr<Direct2DTextFormatPackage> CreateInternal(const FontProperties& fontProperties)
				{
					Ptr<Direct2DTextFormatPackage> textFormat=new Direct2DTextFormatPackage;
					textFormat->textFormat=CreateDirect2DFont(fontProperties);
					textFormat->trimming.granularity=DWRITE_TRIMMING_GRANULARITY_CHARACTER;
					textFormat->trimming.delimiter=0;
					textFormat->trimming.delimiterCount=0;

					IDWriteInlineObject* ellipseInlineObject=0;
					GetWindowsDirect2DObjectProvider()->GetDirectWriteFactory()->CreateEllipsisTrimmingSign(textFormat->textFormat.Obj(), &ellipseInlineObject);
					textFormat->ellipseInlineObject=ellipseInlineObject;
					return textFormat;
				}
			};

			class CachedCharMeasurerAllocator
			{
				DEFINE_CACHED_RESOURCE_ALLOCATOR(FontProperties, Ptr<text::CharMeasurer>)

			protected:
				class Direct2DCharMeasurer : public text::CharMeasurer
				{
				protected:
					ComPtr<IDWriteTextFormat>		font;
					vint								size;

					Size MeasureInternal(wchar_t character, IGuiGraphicsRenderTarget* renderTarget)
					{
						Size charSize(0, 0);
						IDWriteTextLayout* textLayout=0;
						HRESULT hr=GetWindowsDirect2DObjectProvider()->GetDirectWriteFactory()->CreateTextLayout(
							&character,
							1,
							font.Obj(),
							0,
							0,
							&textLayout);
						if(!FAILED(hr))
						{
							DWRITE_TEXT_METRICS metrics;
							hr=textLayout->GetMetrics(&metrics);
							if(!FAILED(hr))
							{
								charSize=Size((vint)ceil(metrics.widthIncludingTrailingWhitespace), (vint)ceil(metrics.height));
							}
							textLayout->Release();
						}
						return charSize;
					}

					vint MeasureWidthInternal(wchar_t character, IGuiGraphicsRenderTarget* renderTarget)
					{
						return MeasureInternal(character, renderTarget).x;
					}

					vint GetRowHeightInternal(IGuiGraphicsRenderTarget* renderTarget)
					{
						return MeasureInternal(L' ', renderTarget).y;
					}
				public:
					Direct2DCharMeasurer(ComPtr<IDWriteTextFormat> _font, vint _size)
						:text::CharMeasurer(_size)
						,size(_size)
						,font(_font)
					{
					}
				};
			public:
				Ptr<text::CharMeasurer> CreateInternal(const FontProperties& value)
				{
					return new Direct2DCharMeasurer(CachedTextFormatAllocator::CreateDirect2DFont(value), value.size);
				}
			};

/***********************************************************************
WindowsDirect2DRenderTarget
***********************************************************************/

			class WindowsDirect2DImageFrameCache : public Object, public INativeImageFrameCache
			{
			protected:
				IWindowsDirect2DRenderTarget*	renderTarget;
				INativeImageFrame*				cachedFrame;
				ComPtr<ID2D1Bitmap>				bitmap;
				ComPtr<ID2D1Bitmap>				disabledBitmap;
			public:
				WindowsDirect2DImageFrameCache(IWindowsDirect2DRenderTarget* _renderTarget)
					:renderTarget(_renderTarget)
				{
				}

				~WindowsDirect2DImageFrameCache()
				{
				}

				void OnAttach(INativeImageFrame* frame)override
				{
					cachedFrame=frame;
 					ID2D1Bitmap* d2dBitmap=0;
					HRESULT hr=renderTarget->GetDirect2DRenderTarget()->CreateBitmapFromWicBitmap(
						GetWindowsDirect2DObjectProvider()->GetWICBitmap(frame),
						&d2dBitmap
						);
					if(SUCCEEDED(hr))
					{
						bitmap=d2dBitmap;
					}
				}
				
				void OnDetach(INativeImageFrame* frame)override
				{
					renderTarget->DestroyBitmapCache(cachedFrame);
				}

				INativeImageFrame* GetFrame()
				{
					return cachedFrame;
				}

				ComPtr<ID2D1Bitmap> GetBitmap(bool enabled)
				{
					if(enabled)
					{
						return bitmap;
					}
					else
					{
						if(!disabledBitmap)
						{
							IWICBitmap* frameBitmap=GetWindowsDirect2DObjectProvider()->GetWICBitmap(cachedFrame);
 							ID2D1Bitmap* d2dBitmap=0;
							HRESULT hr=renderTarget->GetDirect2DRenderTarget()->CreateBitmapFromWicBitmap(
								frameBitmap,
								&d2dBitmap
								);
							if(SUCCEEDED(hr))
							{
								disabledBitmap=d2dBitmap;

								WICRect rect;
								rect.X=0;
								rect.Y=0;
								rect.Width=bitmap->GetPixelSize().width;
								rect.Height=bitmap->GetPixelSize().height;
								BYTE* buffer=new BYTE[rect.Width*rect.Height*4];
								hr=frameBitmap->CopyPixels(&rect, rect.Width*4, rect.Width*rect.Height*4, buffer);
								if(SUCCEEDED(hr))
								{
									vint count=rect.Width*rect.Height;
									BYTE* read=buffer;
									for(vint i=0;i<count;i++)
									{
										BYTE g=(read[0]+read[1]+read[2])/6+read[3]/2;
										read[0]=g;
										read[1]=g;
										read[2]=g;
										read+=4;
									}
									D2D1_RECT_U d2dRect;
									d2dRect.left=0;
									d2dRect.top=0;
									d2dRect.right=rect.Width;
									d2dRect.bottom=rect.Height;
									d2dBitmap->CopyFromMemory(&d2dRect, buffer, rect.Width*4);
								}
								delete[] buffer;
							}
						}
						return disabledBitmap;
					}
				}
			};

			class WindowsDirect2DRenderTarget : public Object, public IWindowsDirect2DRenderTarget
			{
				typedef SortedList<Ptr<WindowsDirect2DImageFrameCache>> ImageCacheList;
			protected:
				INativeWindow*					window;
				ID2D1RenderTarget*				d2dRenderTarget;
				List<Rect>						clippers;
				vint								clipperCoverWholeTargetCounter;

				CachedSolidBrushAllocator		solidBrushes;
				CachedLinearBrushAllocator		linearBrushes;
				ImageCacheList					imageCaches;

				ComPtr<IDWriteRenderingParams>	noAntialiasParams;
				ComPtr<IDWriteRenderingParams>	horizontalAntialiasParams;
				ComPtr<IDWriteRenderingParams>	bidirectionalAntialiasParams;

				ComPtr<IDWriteRenderingParams> CreateRenderingParams(DWRITE_RENDERING_MODE renderingMode, IDWriteRenderingParams* defaultParams, IDWriteFactory* dwriteFactory)
				{
					IDWriteRenderingParams* renderingParams=0;
					FLOAT gamma=defaultParams->GetGamma();
					FLOAT enhancedContrast=defaultParams->GetEnhancedContrast();
					FLOAT clearTypeLevel=defaultParams->GetClearTypeLevel();
					DWRITE_PIXEL_GEOMETRY pixelGeometry=defaultParams->GetPixelGeometry();
					HRESULT hr=dwriteFactory->CreateCustomRenderingParams(
						gamma,
						enhancedContrast,
						clearTypeLevel,
						pixelGeometry,
						renderingMode,
						&renderingParams);
					if(!FAILED(hr))
					{
						return renderingParams;
					}
					else
					{
						return 0;
					}
				}
			public:
				WindowsDirect2DRenderTarget(INativeWindow* _window)
					:window(_window)
					,d2dRenderTarget(0)
					,clipperCoverWholeTargetCounter(0)
				{
					solidBrushes.SetRenderTarget(this);
					linearBrushes.SetRenderTarget(this);

					IDWriteFactory* dwriteFactory=GetWindowsDirect2DObjectProvider()->GetDirectWriteFactory();
					IDWriteRenderingParams* defaultParams=0;
					HRESULT hr=dwriteFactory->CreateRenderingParams(&defaultParams);
					if(!FAILED(hr))
					{
						noAntialiasParams=CreateRenderingParams(DWRITE_RENDERING_MODE_CLEARTYPE_GDI_NATURAL, defaultParams, dwriteFactory);
						horizontalAntialiasParams=CreateRenderingParams(DWRITE_RENDERING_MODE_CLEARTYPE_NATURAL, defaultParams, dwriteFactory);
						bidirectionalAntialiasParams=CreateRenderingParams(DWRITE_RENDERING_MODE_CLEARTYPE_NATURAL_SYMMETRIC, defaultParams, dwriteFactory);
						defaultParams->Release();
					}
				}

				~WindowsDirect2DRenderTarget()
				{
					while(imageCaches.Count())
					{
						Ptr<WindowsDirect2DImageFrameCache> cache=imageCaches[imageCaches.Count()-1];
						cache->GetFrame()->RemoveCache(this);
					}
				}

				ID2D1RenderTarget* GetDirect2DRenderTarget()override
				{
					return d2dRenderTarget?d2dRenderTarget:GetWindowsDirect2DObjectProvider()->GetNativeWindowDirect2DRenderTarget(window);
				}

				ComPtr<ID2D1Bitmap> GetBitmap(INativeImageFrame* frame, bool enabled)override
				{
					Ptr<INativeImageFrameCache> cache=frame->GetCache(this);
					if(cache)
					{
						return cache.Cast<WindowsDirect2DImageFrameCache>()->GetBitmap(enabled);
					}
					else
					{
						Ptr<WindowsDirect2DImageFrameCache> d2dCache=new WindowsDirect2DImageFrameCache(this);
						if(frame->SetCache(this, d2dCache))
						{
							imageCaches.Add(d2dCache);
							return d2dCache->GetBitmap(enabled);
						}
						else
						{
							return 0;
						}
					}
				}

				void DestroyBitmapCache(INativeImageFrame* frame)override
				{
					WindowsDirect2DImageFrameCache* cache=frame->GetCache(this).Cast<WindowsDirect2DImageFrameCache>().Obj();
					imageCaches.Remove(cache);
				}

				void SetTextAntialias(bool antialias, bool verticalAntialias)override
				{
					ComPtr<IDWriteRenderingParams> params;
					if(!antialias)
					{
						params=noAntialiasParams;
					}
					else if(!verticalAntialias)
					{
						params=horizontalAntialiasParams;
					}
					else
					{
						params=bidirectionalAntialiasParams;
					}
					if(params && d2dRenderTarget)
					{
						d2dRenderTarget->SetTextRenderingParams(params.Obj());
					}
				}

				void StartRendering()override
				{
					d2dRenderTarget=GetWindowsDirect2DObjectProvider()->GetNativeWindowDirect2DRenderTarget(window);
					d2dRenderTarget->BeginDraw();
					d2dRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
				}

				bool StopRendering()override
				{
					auto result = d2dRenderTarget->EndDraw();
					d2dRenderTarget=0;
					return result == S_OK;
				}

				void PushClipper(Rect clipper)override
				{
					if(clipperCoverWholeTargetCounter>0)
					{
						clipperCoverWholeTargetCounter++;
					}
					else
					{
						Rect previousClipper=GetClipper();
						Rect currentClipper;

						currentClipper.x1=(previousClipper.x1>clipper.x1?previousClipper.x1:clipper.x1);
						currentClipper.y1=(previousClipper.y1>clipper.y1?previousClipper.y1:clipper.y1);
						currentClipper.x2=(previousClipper.x2<clipper.x2?previousClipper.x2:clipper.x2);
						currentClipper.y2=(previousClipper.y2<clipper.y2?previousClipper.y2:clipper.y2);

						if(currentClipper.x1<currentClipper.x2 && currentClipper.y1<currentClipper.y2)
						{
							clippers.Add(currentClipper);
							d2dRenderTarget->PushAxisAlignedClip(
								D2D1::RectF((FLOAT)currentClipper.x1, (FLOAT)currentClipper.y1, (FLOAT)currentClipper.x2, (FLOAT)currentClipper.y2),
								D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
								);
						}
						else
						{
							clipperCoverWholeTargetCounter++;
						}
					}
				}

				void PopClipper()override
				{
					if(clipperCoverWholeTargetCounter>0)
					{
						clipperCoverWholeTargetCounter--;
					}
					else if(clippers.Count()>0)
					{
						clippers.RemoveAt(clippers.Count()-1);
						d2dRenderTarget->PopAxisAlignedClip();
					}
				}

				Rect GetClipper()override
				{
					if(clippers.Count()==0)
					{
						return Rect(Point(0, 0), window->GetClientSize());
					}
					else
					{
						return clippers[clippers.Count()-1];
					}
				}

				bool IsClipperCoverWholeTarget()override
				{
					return clipperCoverWholeTargetCounter>0;
				}

				ID2D1SolidColorBrush* CreateDirect2DBrush(Color color)override
				{
					return solidBrushes.Create(color).Obj();
				}

				void DestroyDirect2DBrush(Color color)override
				{
					solidBrushes.Destroy(color);
				}

				ID2D1LinearGradientBrush* CreateDirect2DLinearBrush(Color c1, Color c2)override
				{
					return linearBrushes.Create(Pair<Color, Color>(c1, c2)).Obj();
				}

				void DestroyDirect2DLinearBrush(Color c1, Color c2)override
				{
					linearBrushes.Destroy(Pair<Color, Color>(c1, c2));
				}
			};

/***********************************************************************
WindowsGDIResourceManager
***********************************************************************/

			class WindowsDirect2DResourceManager : public GuiGraphicsResourceManager, public IWindowsDirect2DResourceManager, public INativeControllerListener
			{
			protected:
				SortedList<Ptr<WindowsDirect2DRenderTarget>>		renderTargets;
				Ptr<WindowsDirect2DLayoutProvider>					layoutProvider;

				CachedTextFormatAllocator							textFormats;
				CachedCharMeasurerAllocator							charMeasurers;
			public:
				WindowsDirect2DResourceManager()
				{
					layoutProvider=new WindowsDirect2DLayoutProvider;
				}

				IGuiGraphicsRenderTarget* GetRenderTarget(INativeWindow* window)override
				{
					return GetWindowsDirect2DObjectProvider()->GetBindedRenderTarget(window);
				}
				
				void RecreateRenderTarget(INativeWindow* window)override
				{
					NativeWindowDestroying(window);
					GetWindowsDirect2DObjectProvider()->RecreateRenderTarget(window);
					NativeWindowCreated(window);
				}

				IGuiGraphicsLayoutProvider* GetLayoutProvider()override
				{
					return layoutProvider.Obj();
				}

				void NativeWindowCreated(INativeWindow* window)override
				{
					WindowsDirect2DRenderTarget* renderTarget=new WindowsDirect2DRenderTarget(window);
					renderTargets.Add(renderTarget);
					GetWindowsDirect2DObjectProvider()->SetBindedRenderTarget(window, renderTarget);
				}

				void NativeWindowDestroying(INativeWindow* window)override
				{
					WindowsDirect2DRenderTarget* renderTarget=dynamic_cast<WindowsDirect2DRenderTarget*>(GetWindowsDirect2DObjectProvider()->GetBindedRenderTarget(window));
					GetWindowsDirect2DObjectProvider()->SetBindedRenderTarget(window, 0);
					renderTargets.Remove(renderTarget);
				}

				Direct2DTextFormatPackage* CreateDirect2DTextFormat(const FontProperties& fontProperties)override
				{
					return textFormats.Create(fontProperties).Obj();
				}

				void DestroyDirect2DTextFormat(const FontProperties& fontProperties)override
				{
					textFormats.Destroy(fontProperties);
				}

				Ptr<elements::text::CharMeasurer> CreateDirect2DCharMeasurer(const FontProperties& fontProperties)override
				{
					return charMeasurers.Create(fontProperties);
				}

				void DestroyDirect2DCharMeasurer(const FontProperties& fontProperties)override
				{
					charMeasurers.Destroy(fontProperties);
				}
			};
		}

		namespace elements_windows_d2d
		{
			IWindowsDirect2DResourceManager* windowsDirect2DResourceManager=0;

			IWindowsDirect2DResourceManager* GetWindowsDirect2DResourceManager()
			{
				return windowsDirect2DResourceManager;
			}

			void SetWindowsDirect2DResourceManager(IWindowsDirect2DResourceManager* resourceManager)
			{
				windowsDirect2DResourceManager=resourceManager;
			}

/***********************************************************************
OS Supporting
***********************************************************************/

			IWindowsDirect2DObjectProvider* windowsDirect2DObjectProvider=0;

			IWindowsDirect2DObjectProvider* GetWindowsDirect2DObjectProvider()
			{
				return windowsDirect2DObjectProvider;
			}

			void SetWindowsDirect2DObjectProvider(IWindowsDirect2DObjectProvider* provider)
			{
				windowsDirect2DObjectProvider=provider;
			}
		}
	}
}

/***********************************************************************
NativeMain
***********************************************************************/

using namespace vl::presentation;
using namespace vl::presentation::elements;

void RendererMainDirect2D()
{
	elements_windows_d2d::WindowsDirect2DResourceManager resourceManager;
	SetGuiGraphicsResourceManager(&resourceManager);
	elements_windows_d2d::SetWindowsDirect2DResourceManager(&resourceManager);
	GetCurrentController()->CallbackService()->InstallListener(&resourceManager);

	elements_windows_d2d::GuiSolidBorderElementRenderer::Register();
	elements_windows_d2d::GuiRoundBorderElementRenderer::Register();
	elements_windows_d2d::Gui3DBorderElementRenderer::Register();
	elements_windows_d2d::Gui3DSplitterElementRenderer::Register();
	elements_windows_d2d::GuiSolidBackgroundElementRenderer::Register();
	elements_windows_d2d::GuiGradientBackgroundElementRenderer::Register();
	elements_windows_d2d::GuiSolidLabelElementRenderer::Register();
	elements_windows_d2d::GuiImageFrameElementRenderer::Register();
	elements_windows_d2d::GuiPolygonElementRenderer::Register();
	elements_windows_d2d::GuiColorizedTextElementRenderer::Register();
	elements_windows_d2d::GuiDirect2DElementRenderer::Register();
	elements::GuiDocumentElement::GuiDocumentElementRenderer::Register();

	GuiApplicationMain();
	elements_windows_d2d::SetWindowsDirect2DResourceManager(0);
	SetGuiGraphicsResourceManager(0);
}