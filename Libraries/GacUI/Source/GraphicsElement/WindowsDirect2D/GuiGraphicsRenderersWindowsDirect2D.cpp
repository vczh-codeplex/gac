#include <math.h>
#include "GuiGraphicsRenderersWindowsDirect2D.h"

namespace vl
{
	namespace presentation
	{
		namespace elements_windows_d2d
		{
			using namespace collections;

/***********************************************************************
IMPLEMENT_BRUSH_ELEMENT_RENDERER
***********************************************************************/

#define IMPLEMENT_BRUSH_ELEMENT_RENDERER(TRENDERER)\
			void TRENDERER::InitializeInternal()\
			{\
			}\
			void TRENDERER::FinalizeInternal()\
			{\
				DestroyBrush(renderTarget);\
			}\
			void TRENDERER::RenderTargetChangedInternal(IWindowsDirect2DRenderTarget* oldRenderTarget, IWindowsDirect2DRenderTarget* newRenderTarget)\
			{\
				DestroyBrush(oldRenderTarget);\
				CreateBrush(newRenderTarget);\
			}\
			TRENDERER::TRENDERER()\
				:brush(0)\
			{\
			}\
			void TRENDERER::Render(Rect bounds)\

#define IMPLEMENT_BRUSH_ELEMENT_RENDERER_SOLID_COLOR_BRUSH(TRENDERER)\
			void TRENDERER::CreateBrush(IWindowsDirect2DRenderTarget* _renderTarget)\
			{\
				if(_renderTarget)\
				{\
					oldColor=element->GetColor();\
					brush=_renderTarget->CreateDirect2DBrush(oldColor);\
				}\
			}\
			void TRENDERER::DestroyBrush(IWindowsDirect2DRenderTarget* _renderTarget)\
			{\
				if(_renderTarget && brush)\
				{\
					_renderTarget->DestroyDirect2DBrush(oldColor);\
					brush=0;\
				}\
			}\
			void TRENDERER::OnElementStateChanged()\
			{\
				if(renderTarget)\
				{\
					Color color=element->GetColor();\
					if(oldColor!=color)\
					{\
						DestroyBrush(renderTarget);\
						CreateBrush(renderTarget);\
					}\
				}\
			}\

#define IMPLEMENT_BRUSH_ELEMENT_RENDERER_LINEAR_GRADIENT_BRUSH(TRENDERER)\
			void TRENDERER::CreateBrush(IWindowsDirect2DRenderTarget* _renderTarget)\
			{\
				if(_renderTarget)\
				{\
					oldColor=Pair<Color, Color>(element->GetColor1(), element->GetColor2());\
					brush=_renderTarget->CreateDirect2DLinearBrush(oldColor.key, oldColor.value);\
				}\
			}\
			void TRENDERER::DestroyBrush(IWindowsDirect2DRenderTarget* _renderTarget)\
			{\
				if(_renderTarget && brush)\
				{\
					_renderTarget->DestroyDirect2DLinearBrush(oldColor.key, oldColor.value);\
					brush=0;\
				}\
			}\
			void TRENDERER::OnElementStateChanged()\
			{\
				if(renderTarget)\
				{\
					Pair<Color, Color> color=Pair<Color, Color>(element->GetColor1(), element->GetColor2());\
					if(oldColor!=color)\
					{\
						DestroyBrush(renderTarget);\
						CreateBrush(renderTarget);\
					}\
				}\
			}\

/***********************************************************************
GuiSolidBorderElementRenderer
***********************************************************************/

			IMPLEMENT_BRUSH_ELEMENT_RENDERER_SOLID_COLOR_BRUSH(GuiSolidBorderElementRenderer)
			IMPLEMENT_BRUSH_ELEMENT_RENDERER(GuiSolidBorderElementRenderer)
			{
				ID2D1RenderTarget* d2dRenderTarget=renderTarget->GetDirect2DRenderTarget();
				switch(element->GetShape())
				{
				case ElementShape::Rectangle:
					d2dRenderTarget->DrawRectangle(
						D2D1::RectF((FLOAT)bounds.x1+0.5f, (FLOAT)bounds.y1+0.5f, (FLOAT)bounds.x2-0.5f, (FLOAT)bounds.y2-0.5f),
						brush
						);
					break;
				case ElementShape::Ellipse:
					d2dRenderTarget->DrawEllipse(
						D2D1::Ellipse(D2D1::Point2F((bounds.x1+bounds.x2)/2.0f, (bounds.y1+bounds.y2)/2.0f), bounds.Width()/2.0f, bounds.Height()/2.0f),
						brush
						);
					break;
				}
			}

/***********************************************************************
GuiRoundBorderElementRenderer
***********************************************************************/
				
			IMPLEMENT_BRUSH_ELEMENT_RENDERER_SOLID_COLOR_BRUSH(GuiRoundBorderElementRenderer)
			IMPLEMENT_BRUSH_ELEMENT_RENDERER(GuiRoundBorderElementRenderer)
			{
				ID2D1RenderTarget* d2dRenderTarget=renderTarget->GetDirect2DRenderTarget();
				d2dRenderTarget->DrawRoundedRectangle(
					D2D1::RoundedRect(
						D2D1::RectF((FLOAT)bounds.x1+0.5f, (FLOAT)bounds.y1+0.5f, (FLOAT)bounds.x2-0.5f, (FLOAT)bounds.y2-0.5f),
						(FLOAT)element->GetRadius(),
						(FLOAT)element->GetRadius()
						),
					brush
					);
			}

/***********************************************************************
Gui3DBorderElementRenderer
***********************************************************************/

			void Gui3DBorderElementRenderer::CreateBrush(IWindowsDirect2DRenderTarget* _renderTarget)
			{
				if(_renderTarget)
				{
					oldColor1=element->GetColor1();
					oldColor2=element->GetColor2();
					brush1=_renderTarget->CreateDirect2DBrush(oldColor1);
					brush2=_renderTarget->CreateDirect2DBrush(oldColor2);
				}
			}

			void Gui3DBorderElementRenderer::DestroyBrush(IWindowsDirect2DRenderTarget* _renderTarget)
			{
				if(_renderTarget)
				{
					if(brush1)
					{
						_renderTarget->DestroyDirect2DBrush(oldColor1);
						brush1=0;
					}
					if(brush2)
					{
						_renderTarget->DestroyDirect2DBrush(oldColor2);
						brush2=0;
					}
				}
			}

			void Gui3DBorderElementRenderer::InitializeInternal()
			{
			}

			void Gui3DBorderElementRenderer::FinalizeInternal()
			{
				DestroyBrush(renderTarget);
			}

			void Gui3DBorderElementRenderer::RenderTargetChangedInternal(IWindowsDirect2DRenderTarget* oldRenderTarget, IWindowsDirect2DRenderTarget* newRenderTarget)
			{
				DestroyBrush(oldRenderTarget);
				CreateBrush(newRenderTarget);
			}

			Gui3DBorderElementRenderer::Gui3DBorderElementRenderer()
				:brush1(0)
				,brush2(0)
			{
			}

			void Gui3DBorderElementRenderer::Render(Rect bounds)
			{
				D2D1_RECT_F rect=D2D1::RectF((FLOAT)bounds.x1+0.5f, (FLOAT)bounds.y1+0.5f, (FLOAT)bounds.x2-0.5f, (FLOAT)bounds.y2-0.5f);
				ID2D1RenderTarget* d2dRenderTarget=renderTarget->GetDirect2DRenderTarget();

				d2dRenderTarget->DrawLine(D2D1::Point2F(rect.left, rect.top), D2D1::Point2F(rect.right, rect.top), brush1);
				d2dRenderTarget->DrawLine(D2D1::Point2F(rect.left, rect.top), D2D1::Point2F(rect.left, rect.bottom), brush1);
				d2dRenderTarget->DrawLine(D2D1::Point2F(rect.right, rect.bottom), D2D1::Point2F(rect.left, rect.bottom), brush2);
				d2dRenderTarget->DrawLine(D2D1::Point2F(rect.right, rect.bottom), D2D1::Point2F(rect.right, rect.top), brush2);
			}

			void Gui3DBorderElementRenderer::OnElementStateChanged()
			{
				if(renderTarget)
				{
					Color color1=element->GetColor1();
					Color color2=element->GetColor2();
					if(oldColor1!=color1 || oldColor2!=color2)
					{
						DestroyBrush(renderTarget);
						CreateBrush(renderTarget);
					}
				}
			}

/***********************************************************************
Gui3DSplitterElementRenderer
***********************************************************************/

			void Gui3DSplitterElementRenderer::CreateBrush(IWindowsDirect2DRenderTarget* _renderTarget)
			{
				if(_renderTarget)
				{
					oldColor1=element->GetColor1();
					oldColor2=element->GetColor2();
					brush1=_renderTarget->CreateDirect2DBrush(oldColor1);
					brush2=_renderTarget->CreateDirect2DBrush(oldColor2);
				}
			}

			void Gui3DSplitterElementRenderer::DestroyBrush(IWindowsDirect2DRenderTarget* _renderTarget)
			{
				if(_renderTarget)
				{
					if(brush1)
					{
						_renderTarget->DestroyDirect2DBrush(oldColor1);
						brush1=0;
					}
					if(brush2)
					{
						_renderTarget->DestroyDirect2DBrush(oldColor2);
						brush2=0;
					}
				}
			}

			void Gui3DSplitterElementRenderer::InitializeInternal()
			{
			}

			void Gui3DSplitterElementRenderer::FinalizeInternal()
			{
				DestroyBrush(renderTarget);
			}

			void Gui3DSplitterElementRenderer::RenderTargetChangedInternal(IWindowsDirect2DRenderTarget* oldRenderTarget, IWindowsDirect2DRenderTarget* newRenderTarget)
			{
				DestroyBrush(oldRenderTarget);
				CreateBrush(newRenderTarget);
			}

			Gui3DSplitterElementRenderer::Gui3DSplitterElementRenderer()
				:brush1(0)
				,brush2(0)
			{
			}

			void Gui3DSplitterElementRenderer::Render(Rect bounds)
			{
				D2D1_POINT_2F p11, p12, p21, p22;
				switch(element->GetDirection())
				{
				case Gui3DSplitterElement::Horizontal:
					{
						vint y=bounds.y1+bounds.Height()/2-1;
						p11=D2D1::Point2F((FLOAT)bounds.x1, (FLOAT)y+0.5f);
						p12=D2D1::Point2F((FLOAT)bounds.x2, (FLOAT)y+0.5f);
						p21=D2D1::Point2F((FLOAT)bounds.x1, (FLOAT)y+1.5f);
						p22=D2D1::Point2F((FLOAT)bounds.x2, (FLOAT)y+1.5f);
					}
					break;
				case Gui3DSplitterElement::Vertical:
					{
						vint x=bounds.x1+bounds.Width()/2-1;
						p11=D2D1::Point2F((FLOAT)x+0.5f, (FLOAT)bounds.y1-0.0f);
						p12=D2D1::Point2F((FLOAT)x+0.5f, (FLOAT)bounds.y2+0.0f);
						p21=D2D1::Point2F((FLOAT)x+1.5f, (FLOAT)bounds.y1-0.0f);
						p22=D2D1::Point2F((FLOAT)x+1.5f, (FLOAT)bounds.y2+0.0f);
					}
					break;
				}
				ID2D1RenderTarget* d2dRenderTarget=renderTarget->GetDirect2DRenderTarget();

				d2dRenderTarget->DrawLine(p11, p12, brush1);
				d2dRenderTarget->DrawLine(p21, p22, brush2);
			}

			void Gui3DSplitterElementRenderer::OnElementStateChanged()
			{
				if(renderTarget)
				{
					Color color1=element->GetColor1();
					Color color2=element->GetColor2();
					if(oldColor1!=color1 || oldColor2!=color2)
					{
						DestroyBrush(renderTarget);
						CreateBrush(renderTarget);
					}
				}
			}

/***********************************************************************
GuiSolidBackgroundElementRenderer
***********************************************************************/
			
			IMPLEMENT_BRUSH_ELEMENT_RENDERER_SOLID_COLOR_BRUSH(GuiSolidBackgroundElementRenderer)
			IMPLEMENT_BRUSH_ELEMENT_RENDERER(GuiSolidBackgroundElementRenderer)
			{
				ID2D1RenderTarget* d2dRenderTarget=renderTarget->GetDirect2DRenderTarget();
				switch(element->GetShape())
				{
				case ElementShape::Rectangle:
					d2dRenderTarget->FillRectangle(
						D2D1::RectF((FLOAT)bounds.x1, (FLOAT)bounds.y1, (FLOAT)bounds.x2, (FLOAT)bounds.y2),
						brush
						);
					break;
				case ElementShape::Ellipse:
					d2dRenderTarget->FillEllipse(
						D2D1::Ellipse(D2D1::Point2F((bounds.x1+bounds.x2)/2.0f, (bounds.y1+bounds.y2)/2.0f), bounds.Width()/2.0f, bounds.Height()/2.0f),
						brush
						);
					break;
				}
			}

/***********************************************************************
GuiGradientBackgroundElementRenderer
***********************************************************************/

			IMPLEMENT_BRUSH_ELEMENT_RENDERER_LINEAR_GRADIENT_BRUSH(GuiGradientBackgroundElementRenderer)
			IMPLEMENT_BRUSH_ELEMENT_RENDERER(GuiGradientBackgroundElementRenderer)
			{
				D2D1_POINT_2F points[2];
				switch(element->GetDirection())
				{
				case GuiGradientBackgroundElement::Horizontal:
					{
						points[0].x=(FLOAT)bounds.x1;
						points[0].y=(FLOAT)bounds.y1;
						points[1].x=(FLOAT)bounds.x2;
						points[1].y=(FLOAT)bounds.y1;
					}
					break;
				case GuiGradientBackgroundElement::Vertical:
					{
						points[0].x=(FLOAT)bounds.x1;
						points[0].y=(FLOAT)bounds.y1;
						points[1].x=(FLOAT)bounds.x1;
						points[1].y=(FLOAT)bounds.y2;
					}
					break;
				case GuiGradientBackgroundElement::Slash:
					{
						points[0].x=(FLOAT)bounds.x2;
						points[0].y=(FLOAT)bounds.y1;
						points[1].x=(FLOAT)bounds.x1;
						points[1].y=(FLOAT)bounds.y2;
					}
					break;
				case GuiGradientBackgroundElement::Backslash:
					{
						points[0].x=(FLOAT)bounds.x1;
						points[0].y=(FLOAT)bounds.y1;
						points[1].x=(FLOAT)bounds.x2;
						points[1].y=(FLOAT)bounds.y2;
					}
					break;
				}

				brush->SetStartPoint(points[0]);
				brush->SetEndPoint(points[1]);
				
				ID2D1RenderTarget* d2dRenderTarget=renderTarget->GetDirect2DRenderTarget();
				switch(element->GetShape())
				{
				case ElementShape::Rectangle:
					d2dRenderTarget->FillRectangle(
						D2D1::RectF((FLOAT)bounds.x1, (FLOAT)bounds.y1, (FLOAT)bounds.x2, (FLOAT)bounds.y2),
						brush
						);
					break;
				case ElementShape::Ellipse:
					d2dRenderTarget->FillEllipse(
						D2D1::Ellipse(D2D1::Point2F((bounds.x1+bounds.x2)/2.0f, (bounds.y1+bounds.y2)/2.0f), bounds.Width()/2.0f, bounds.Height()/2.0f),
						brush
						);
					break;
				}
			}

/***********************************************************************
GuiSolidLabelElementRenderer
***********************************************************************/

			void GuiSolidLabelElementRenderer::CreateBrush(IWindowsDirect2DRenderTarget* _renderTarget)
			{
				if(_renderTarget)
				{
					oldColor=element->GetColor();
					brush=_renderTarget->CreateDirect2DBrush(oldColor);
				}
			}

			void GuiSolidLabelElementRenderer::DestroyBrush(IWindowsDirect2DRenderTarget* _renderTarget)
			{
				if(_renderTarget && brush)
				{
					_renderTarget->DestroyDirect2DBrush(oldColor);
					brush=0;
				}
			}

			void GuiSolidLabelElementRenderer::CreateTextFormat(IWindowsDirect2DRenderTarget* _renderTarget)
			{
				if(_renderTarget)
				{
					IWindowsDirect2DResourceManager* resourceManager=GetWindowsDirect2DResourceManager();
					oldFont=element->GetFont();
					textFormat=resourceManager->CreateDirect2DTextFormat(oldFont);
				}
			}

			void GuiSolidLabelElementRenderer::DestroyTextFormat(IWindowsDirect2DRenderTarget* _renderTarget)
			{
				if(_renderTarget && textFormat)
				{
					IWindowsDirect2DResourceManager* resourceManager=GetWindowsDirect2DResourceManager();
					resourceManager->DestroyDirect2DTextFormat(oldFont);
					textFormat=0;
				}
			}

			void GuiSolidLabelElementRenderer::CreateTextLayout()
			{
				if(textFormat)
				{
					HRESULT hr=GetWindowsDirect2DObjectProvider()->GetDirectWriteFactory()->CreateTextLayout(
						oldText.Buffer(),
						(int)oldText.Length(),
						textFormat->textFormat.Obj(),
						0,
						0,
						&textLayout);
					if(!FAILED(hr))
					{
						if(oldFont.underline)
						{
							DWRITE_TEXT_RANGE textRange;
							textRange.startPosition=0;
							textRange.length=(int)oldText.Length();
							textLayout->SetUnderline(TRUE, textRange);
						}
						if(oldFont.strikeline)
						{
							DWRITE_TEXT_RANGE textRange;
							textRange.startPosition=0;
							textRange.length=(int)oldText.Length();
							textLayout->SetStrikethrough(TRUE, textRange);
						}
					}
					else
					{
						textLayout=0;
					}
				}
			}

			void GuiSolidLabelElementRenderer::DestroyTextLayout()
			{
				if(textLayout)
				{
					textLayout->Release();
					textLayout=0;
				}
			}

			void GuiSolidLabelElementRenderer::UpdateMinSize()
			{
				float maxWidth=0;
				DestroyTextLayout();
				bool calculateSizeFromTextLayout=false;
				if(renderTarget)
				{
					if(element->GetWrapLine())
					{
						if(element->GetWrapLineHeightCalculation())
						{
							CreateTextLayout();
							if(textLayout)
							{
								maxWidth=textLayout->GetMaxWidth();
								if(oldMaxWidth!=-1)
								{
									textLayout->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
									textLayout->SetMaxWidth((float)oldMaxWidth);
								}
								calculateSizeFromTextLayout=true;
							}
						}
					}
					else
					{
						CreateTextLayout();
						if(textLayout)
						{
							maxWidth=textLayout->GetMaxWidth();
							calculateSizeFromTextLayout=true;
						}
					}
				}
				if(calculateSizeFromTextLayout)
				{
					DWRITE_TEXT_METRICS metrics;
					HRESULT hr=textLayout->GetMetrics(&metrics);
					if(!FAILED(hr))
					{
						vint width=0;
						if(!element->GetEllipse() && !element->GetWrapLine() && !element->GetMultiline())
						{
							width=(vint)ceil(metrics.widthIncludingTrailingWhitespace);
						}
						minSize=Size(width, (vint)ceil(metrics.height));
					}
					textLayout->SetMaxWidth(maxWidth);
				}
				else
				{
					minSize=Size();
				}
			}

			void GuiSolidLabelElementRenderer::InitializeInternal()
			{
			}

			void GuiSolidLabelElementRenderer::FinalizeInternal()
			{
				DestroyTextLayout();
				DestroyBrush(renderTarget);
				DestroyTextFormat(renderTarget);
			}

			void GuiSolidLabelElementRenderer::RenderTargetChangedInternal(IWindowsDirect2DRenderTarget* oldRenderTarget, IWindowsDirect2DRenderTarget* newRenderTarget)
			{
				DestroyBrush(oldRenderTarget);
				DestroyTextFormat(oldRenderTarget);
				CreateBrush(newRenderTarget);
				CreateTextFormat(newRenderTarget);
				UpdateMinSize();
			}

			GuiSolidLabelElementRenderer::GuiSolidLabelElementRenderer()
				:brush(0)
				,textFormat(0)
				,textLayout(0)
				,oldText(L"")
				,oldMaxWidth(-1)
			{
			}

			void GuiSolidLabelElementRenderer::Render(Rect bounds)
			{
				if(!textLayout)
				{
					CreateTextLayout();
				}

				vint x=0;
				vint y=0;
				switch(element->GetHorizontalAlignment())
				{
				case Alignment::Left:
					x=bounds.Left();
					break;
				case Alignment::Center:
					x=bounds.Left()+(bounds.Width()-minSize.x)/2;
					break;
				case Alignment::Right:
					x=bounds.Right()-minSize.x;
					break;
				}
				switch(element->GetVerticalAlignment())
				{
				case Alignment::Top:
					y=bounds.Top();
					break;
				case Alignment::Center:
					y=bounds.Top()+(bounds.Height()-minSize.y)/2;
					break;
				case Alignment::Bottom:
					y=bounds.Bottom()-minSize.y;
					break;
				}

				renderTarget->SetTextAntialias(oldFont.antialias, oldFont.verticalAntialias);

				if(!element->GetEllipse() && !element->GetMultiline() && !element->GetWrapLine())
				{
					ID2D1RenderTarget* d2dRenderTarget=renderTarget->GetDirect2DRenderTarget();
					d2dRenderTarget->DrawTextLayout(
						D2D1::Point2F((FLOAT)x, (FLOAT)y),
						textLayout,
						brush,
						D2D1_DRAW_TEXT_OPTIONS_NO_SNAP
						);
				}
				else
				{
					IDWriteFactory* dwriteFactory=GetWindowsDirect2DObjectProvider()->GetDirectWriteFactory();
					DWRITE_TRIMMING trimming;
					IDWriteInlineObject* inlineObject;
					textLayout->GetTrimming(&trimming, &inlineObject);

					textLayout->SetWordWrapping(element->GetWrapLine()?DWRITE_WORD_WRAPPING_WRAP:DWRITE_WORD_WRAPPING_NO_WRAP);
					if(element->GetEllipse())
					{
						textLayout->SetTrimming(&textFormat->trimming, textFormat->ellipseInlineObject.Obj());
					}
					switch(element->GetHorizontalAlignment())
					{
					case Alignment::Left:
						textLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
						break;
					case Alignment::Center:
						textLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
						break;
					case Alignment::Right:
						textLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
						break;
					}
					if(!element->GetMultiline() && !element->GetWrapLine())
					{
						switch(element->GetVerticalAlignment())
						{
						case Alignment::Top:
							textLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
							break;
						case Alignment::Center:
							textLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
							break;
						case Alignment::Bottom:
							textLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
							break;
						}
					}

					Rect textBounds=bounds;
					if(element->GetEllipse() && !element->GetMultiline() && !element->GetWrapLine())
					{
						textBounds=Rect(Point(textBounds.x1, y), Size(bounds.Width(), minSize.y));
					}

					textLayout->SetMaxWidth((FLOAT)textBounds.Width());
					textLayout->SetMaxHeight((FLOAT)textBounds.Height());

					ID2D1RenderTarget* d2dRenderTarget=renderTarget->GetDirect2DRenderTarget();
					d2dRenderTarget->DrawTextLayout(
						D2D1::Point2F((FLOAT)textBounds.Left(), (FLOAT)textBounds.Top()),
						textLayout,
						brush,
						D2D1_DRAW_TEXT_OPTIONS_NO_SNAP
						);

					textLayout->SetTrimming(&trimming, inlineObject);
					if(oldMaxWidth!=textBounds.Width())
					{
						oldMaxWidth=textBounds.Width();
						UpdateMinSize();
					}
				}
			}

			void GuiSolidLabelElementRenderer::OnElementStateChanged()
			{
				if(renderTarget)
				{
					Color color=element->GetColor();
					if(oldColor!=color)
					{
						DestroyBrush(renderTarget);
						CreateBrush(renderTarget);
					}

					FontProperties font=element->GetFont();
					if(oldFont!=font)
					{
						DestroyTextFormat(renderTarget);
						CreateTextFormat(renderTarget);
					}
				}
				oldText=element->GetText();
				UpdateMinSize();
			}

/***********************************************************************
GuiImageFrameElementRenderer
***********************************************************************/

			void GuiImageFrameElementRenderer::UpdateBitmap(IWindowsDirect2DRenderTarget* renderTarget)
			{
				if(renderTarget && element->GetImage())
				{
					INativeImageFrame* frame=element->GetImage()->GetFrame(element->GetFrameIndex());
					bitmap=renderTarget->GetBitmap(frame, element->GetEnabled());
					minSize=frame->GetSize();
				}
				else
				{
					bitmap=0;
					minSize=Size(0, 0);
				}
			}

			void GuiImageFrameElementRenderer::InitializeInternal()
			{
			}

			void GuiImageFrameElementRenderer::FinalizeInternal()
			{
			}

			void GuiImageFrameElementRenderer::RenderTargetChangedInternal(IWindowsDirect2DRenderTarget* oldRenderTarget, IWindowsDirect2DRenderTarget* newRenderTarget)
			{
				UpdateBitmap(newRenderTarget);
			}

			GuiImageFrameElementRenderer::GuiImageFrameElementRenderer()
			{
			}

			void GuiImageFrameElementRenderer::Render(Rect bounds)
			{
				if(bitmap)
				{
					ID2D1RenderTarget* d2dRenderTarget=renderTarget->GetDirect2DRenderTarget();
					D2D1_RECT_F source=D2D1::RectF(0, 0, (FLOAT)minSize.x, (FLOAT)minSize.y);
					D2D1_RECT_F destination;
					if(element->GetStretch())
					{
						destination=D2D1::RectF((FLOAT)bounds.x1, (FLOAT)bounds.y1, (FLOAT)bounds.x2, (FLOAT)bounds.y2);
					}
					else
					{
						vint x=0;
						vint y=0;
						switch(element->GetHorizontalAlignment())
						{
						case Alignment::Left:
							x=bounds.Left();
							break;
						case Alignment::Center:
							x=bounds.Left()+(bounds.Width()-minSize.x)/2;
							break;
						case Alignment::Right:
							x=bounds.Right()-minSize.x;
							break;
						}
						switch(element->GetVerticalAlignment())
						{
						case Alignment::Top:
							y=bounds.Top();
							break;
						case Alignment::Center:
							y=bounds.Top()+(bounds.Height()-minSize.y)/2;
							break;
						case Alignment::Bottom:
							y=bounds.Bottom()-minSize.y;
							break;
						}
						destination=D2D1::RectF((FLOAT)x, (FLOAT)y, (FLOAT)(x+minSize.x), (FLOAT)(y+minSize.y));
					}
					if(element->GetImage()->GetFormat()==INativeImage::Gif &&  element->GetFrameIndex()>0)
					{
						vint max=element->GetFrameIndex();
						for(vint i=0;i<=max;i++)
						{
							ComPtr<ID2D1Bitmap> frameBitmap=renderTarget->GetBitmap(element->GetImage()->GetFrame(i), element->GetEnabled());
							d2dRenderTarget->DrawBitmap(frameBitmap.Obj(), destination, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, source);
						}
					}
					else
					{
						d2dRenderTarget->DrawBitmap(bitmap.Obj(), destination, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, source);
					}
				}
			}

			void GuiImageFrameElementRenderer::OnElementStateChanged()
			{
				UpdateBitmap(renderTarget);
			}

/***********************************************************************
GuiPolygonElementRenderer
***********************************************************************/

			void GuiPolygonElementRenderer::CreateGeometry()
			{
				oldPoints.Resize(element->GetPointCount());
				if(oldPoints.Count()>0)
				{
					memcpy(&oldPoints[0], &element->GetPoint(0), sizeof(Point)*element->GetPointCount());
				}
				if(oldPoints.Count()>=3)
				{
					ID2D1PathGeometry* pg=0;
					GetWindowsDirect2DObjectProvider()->GetDirect2DFactory()->CreatePathGeometry(&pg);
					if(pg)
					{
						geometry=pg;
						FillGeometry(Point(0, 0));
					}
				}
			}

			void GuiPolygonElementRenderer::DestroyGeometry()
			{
				if(geometry)
				{
					geometry=0;
				}
			}

			void GuiPolygonElementRenderer::FillGeometry(Point offset)
			{
				if(geometry)
				{
					ID2D1GeometrySink* pgs=0;
					geometry->Open(&pgs);
					if(pgs)
					{
						D2D1_POINT_2F p;
						p.x=(FLOAT)(oldPoints[0].x+offset.x)+0.5f;
						p.y=(FLOAT)(oldPoints[0].y+offset.y)+0.5f;
						pgs->BeginFigure(p, D2D1_FIGURE_BEGIN_FILLED);
						for(vint i=1;i<oldPoints.Count();i++)
						{
							p.x=(FLOAT)(oldPoints[i].x+offset.x)+0.5f;
							p.y=(FLOAT)(oldPoints[i].y+offset.y)+0.5f;
							pgs->AddLine(p);
						}
						pgs->EndFigure(D2D1_FIGURE_END_CLOSED);
						pgs->Close();
						pgs->Release();
					}
				}
			}

			void GuiPolygonElementRenderer::RecreateResource(IWindowsDirect2DRenderTarget* oldRenderTarget, IWindowsDirect2DRenderTarget* newRenderTarget)
			{
				if(oldRenderTarget==newRenderTarget)
				{
					if(oldRenderTarget)
					{
						if(oldBorderColor!=element->GetBorderColor())
						{
							oldRenderTarget->DestroyDirect2DBrush(oldBorderColor);
							oldBorderColor=element->GetBorderColor();
							borderBrush=newRenderTarget->CreateDirect2DBrush(oldBorderColor);
						}
						if(oldBackgroundColor!=element->GetBackgroundColor())
						{
							oldRenderTarget->DestroyDirect2DBrush(oldBackgroundColor);
							oldBackgroundColor=element->GetBackgroundColor();
							backgroundBrush=newRenderTarget->CreateDirect2DBrush(oldBackgroundColor);
						}
					}
				}
				else
				{
					if(oldRenderTarget)
					{
						oldRenderTarget->DestroyDirect2DBrush(oldBorderColor);
						oldRenderTarget->DestroyDirect2DBrush(oldBackgroundColor);
					}
					if(newRenderTarget)
					{
						oldBorderColor=element->GetBorderColor();
						oldBackgroundColor=element->GetBackgroundColor();
						borderBrush=newRenderTarget->CreateDirect2DBrush(oldBorderColor);
						backgroundBrush=newRenderTarget->CreateDirect2DBrush(oldBackgroundColor);
					}
				}
			}

			void GuiPolygonElementRenderer::InitializeInternal()
			{
				oldBorderColor=element->GetBorderColor();
				oldBackgroundColor=element->GetBackgroundColor();
				RecreateResource(0, renderTarget);
				CreateGeometry();
			}

			void GuiPolygonElementRenderer::FinalizeInternal()
			{
				DestroyGeometry();
				RecreateResource(renderTarget, 0);
			}

			void GuiPolygonElementRenderer::RenderTargetChangedInternal(IWindowsDirect2DRenderTarget* oldRenderTarget, IWindowsDirect2DRenderTarget* newRenderTarget)
			{
				RecreateResource(oldRenderTarget, newRenderTarget);
			}

			GuiPolygonElementRenderer::GuiPolygonElementRenderer()
				:borderBrush(0)
				,backgroundBrush(0)
				,geometry(0)
			{
			}

			void GuiPolygonElementRenderer::Render(Rect bounds)
			{
				if(renderTarget && geometry)
				{
					ID2D1RenderTarget* d2dRenderTarget=renderTarget->GetDirect2DRenderTarget();
					vint offsetX=(bounds.Width()-minSize.x)/2+bounds.x1;
					vint offsetY=(bounds.Height()-minSize.y)/2+bounds.y1;

					D2D1_MATRIX_3X2_F oldT, newT;
					d2dRenderTarget->GetTransform(&oldT);
					newT=D2D1::Matrix3x2F::Translation((FLOAT)offsetX, (FLOAT)offsetY);
					d2dRenderTarget->SetTransform(&newT);

					d2dRenderTarget->FillGeometry(geometry.Obj(), backgroundBrush);
					d2dRenderTarget->DrawGeometry(geometry.Obj(), borderBrush);
					d2dRenderTarget->SetTransform(&oldT);
				}
			}

			void GuiPolygonElementRenderer::OnElementStateChanged()
			{
				minSize=element->GetSize();
				RecreateResource(renderTarget, renderTarget);

				bool polygonModified=false;
				if(oldPoints.Count()!=element->GetPointCount())
				{
					polygonModified=true;
				}
				else
				{
					for(vint i=0;i<oldPoints.Count();i++)
					{
						if(oldPoints[i]!=element->GetPoint(i))
						{
							polygonModified=true;
							break;
						}
					}
				}
				if(polygonModified)
				{
					DestroyGeometry();
					CreateGeometry();
				}
			}

/***********************************************************************
GuiColorizedTextElementRenderer
***********************************************************************/

			void GuiColorizedTextElementRenderer::CreateTextBrush(IWindowsDirect2DRenderTarget* _renderTarget)
			{
				if(_renderTarget)
				{
					colors.Resize(element->GetColors().Count());
					for(vint i=0;i<colors.Count();i++)
					{
						text::ColorEntry entry=element->GetColors().Get(i);
						ColorEntryResource newEntry;

						newEntry.normal.text=entry.normal.text;
						newEntry.normal.textBrush=_renderTarget->CreateDirect2DBrush(newEntry.normal.text);
						newEntry.normal.background=entry.normal.background;
						newEntry.normal.backgroundBrush=_renderTarget->CreateDirect2DBrush(newEntry.normal.background);
						newEntry.selectedFocused.text=entry.selectedFocused.text;
						newEntry.selectedFocused.textBrush=_renderTarget->CreateDirect2DBrush(newEntry.selectedFocused.text);
						newEntry.selectedFocused.background=entry.selectedFocused.background;
						newEntry.selectedFocused.backgroundBrush=_renderTarget->CreateDirect2DBrush(newEntry.selectedFocused.background);
						newEntry.selectedUnfocused.text=entry.selectedUnfocused.text;
						newEntry.selectedUnfocused.textBrush=_renderTarget->CreateDirect2DBrush(newEntry.selectedUnfocused.text);
						newEntry.selectedUnfocused.background=entry.selectedUnfocused.background;
						newEntry.selectedUnfocused.backgroundBrush=_renderTarget->CreateDirect2DBrush(newEntry.selectedUnfocused.background);
						colors[i]=newEntry;
					}
				}
			}

			void GuiColorizedTextElementRenderer::DestroyTextBrush(IWindowsDirect2DRenderTarget* _renderTarget)
			{
				if(_renderTarget)
				{
					for(vint i=0;i<colors.Count();i++)
					{
						_renderTarget->DestroyDirect2DBrush(colors[i].normal.text);
						_renderTarget->DestroyDirect2DBrush(colors[i].normal.background);
						_renderTarget->DestroyDirect2DBrush(colors[i].selectedFocused.text);
						_renderTarget->DestroyDirect2DBrush(colors[i].selectedFocused.background);
						_renderTarget->DestroyDirect2DBrush(colors[i].selectedUnfocused.text);
						_renderTarget->DestroyDirect2DBrush(colors[i].selectedUnfocused.background);
					}
					colors.Resize(0);
				}
			}

			void GuiColorizedTextElementRenderer::CreateCaretBrush(IWindowsDirect2DRenderTarget* _renderTarget)
			{
				if(_renderTarget)
				{
					oldCaretColor=element->GetCaretColor();
					caretBrush=_renderTarget->CreateDirect2DBrush(oldCaretColor);
				}
			}

			void GuiColorizedTextElementRenderer::DestroyCaretBrush(IWindowsDirect2DRenderTarget* _renderTarget)
			{
				if(_renderTarget)
				{
					if(caretBrush)
					{
						_renderTarget->DestroyDirect2DBrush(oldCaretColor);
						caretBrush=0;
					}
				}
			}

			void GuiColorizedTextElementRenderer::ColorChanged()
			{
				DestroyTextBrush(renderTarget);
				CreateTextBrush(renderTarget);
			}

			void GuiColorizedTextElementRenderer::FontChanged()
			{
				IWindowsDirect2DResourceManager* resourceManager=GetWindowsDirect2DResourceManager();
				if(textFormat)
				{
					resourceManager->DestroyDirect2DTextFormat(oldFont);
					resourceManager->DestroyDirect2DCharMeasurer(oldFont);
				}
				oldFont=element->GetFont();
				textFormat=resourceManager->CreateDirect2DTextFormat(oldFont);
				element->GetLines().SetCharMeasurer(resourceManager->CreateDirect2DCharMeasurer(oldFont).Obj());
			}

			text::CharMeasurer* GuiColorizedTextElementRenderer::GetCharMeasurer()
			{
				return 0;
			}

			void GuiColorizedTextElementRenderer::InitializeInternal()
			{
				textFormat=0;
				caretBrush=0;
				element->SetCallback(this);
			}

			void GuiColorizedTextElementRenderer::FinalizeInternal()
			{
				DestroyTextBrush(renderTarget);
				DestroyCaretBrush(renderTarget);

				IWindowsDirect2DResourceManager* resourceManager=GetWindowsDirect2DResourceManager();
				if(textFormat)
				{
					resourceManager->DestroyDirect2DTextFormat(oldFont);
					resourceManager->DestroyDirect2DCharMeasurer(oldFont);
				}
			}

			void GuiColorizedTextElementRenderer::RenderTargetChangedInternal(IWindowsDirect2DRenderTarget* oldRenderTarget, IWindowsDirect2DRenderTarget* newRenderTarget)
			{
				DestroyTextBrush(oldRenderTarget);
				DestroyCaretBrush(oldRenderTarget);
				CreateTextBrush(newRenderTarget);
				CreateCaretBrush(newRenderTarget);
				element->GetLines().SetRenderTarget(newRenderTarget);
			}

			void GuiColorizedTextElementRenderer::Render(Rect bounds)
			{
				if(renderTarget)
				{
					ID2D1RenderTarget* d2dRenderTarget=renderTarget->GetDirect2DRenderTarget();
					wchar_t passwordChar=element->GetPasswordChar();
					Point viewPosition=element->GetViewPosition();
					Rect viewBounds(viewPosition, bounds.GetSize());
					vint startRow=element->GetLines().GetTextPosFromPoint(Point(viewBounds.x1, viewBounds.y1)).row;
					vint endRow=element->GetLines().GetTextPosFromPoint(Point(viewBounds.x2, viewBounds.y2)).row;
					TextPos selectionBegin=element->GetCaretBegin()<element->GetCaretEnd()?element->GetCaretBegin():element->GetCaretEnd();
					TextPos selectionEnd=element->GetCaretBegin()>element->GetCaretEnd()?element->GetCaretBegin():element->GetCaretEnd();
					bool focused=element->GetFocused();
					
					renderTarget->SetTextAntialias(oldFont.antialias, oldFont.verticalAntialias);

					for(vint row=startRow;row<=endRow;row++)
					{
						Rect startRect=element->GetLines().GetRectFromTextPos(TextPos(row, 0));
						Point startPoint=startRect.LeftTop();
						vint startColumn=element->GetLines().GetTextPosFromPoint(Point(viewBounds.x1, startPoint.y)).column;
						vint endColumn=element->GetLines().GetTextPosFromPoint(Point(viewBounds.x2, startPoint.y)).column;
						text::TextLine& line=element->GetLines().GetLine(row);

						vint x=startColumn==0?0:line.att[startColumn-1].rightOffset;
						for(vint column=startColumn; column<=endColumn; column++)
						{
							bool inSelection=false;
							if(selectionBegin.row==selectionEnd.row)
							{
								inSelection=(row==selectionBegin.row && selectionBegin.column<=column && column<selectionEnd.column);
							}
							else if(row==selectionBegin.row)
							{
								inSelection=selectionBegin.column<=column;
							}
							else if(row==selectionEnd.row)
							{
								inSelection=column<selectionEnd.column;
							}
							else
							{
								inSelection=selectionBegin.row<row && row<selectionEnd.row;
							}
							
							bool crlf=column==line.dataLength;
							vint colorIndex=crlf?0:line.att[column].colorIndex;
							if(colorIndex>=colors.Count())
							{
								colorIndex=0;
							}
							ColorItemResource& color=
								!inSelection?colors[colorIndex].normal:
								focused?colors[colorIndex].selectedFocused:
								colors[colorIndex].selectedUnfocused;
							vint x2=crlf?x+startRect.Height()/2:line.att[column].rightOffset;
							vint tx=x-viewPosition.x+bounds.x1;
							vint ty=startPoint.y-viewPosition.y+bounds.y1;
							
							if(color.background.a>0)
							{
								d2dRenderTarget->FillRectangle(D2D1::RectF((FLOAT)tx, (FLOAT)ty, (FLOAT)(tx+(x2-x)), (FLOAT)(ty+startRect.Height())), color.backgroundBrush);
							}
							if(!crlf)
							{
								d2dRenderTarget->DrawText(
									(passwordChar?&passwordChar:&line.text[column]),
									1,
									textFormat->textFormat.Obj(),
									D2D1::RectF((FLOAT)tx, (FLOAT)ty, (FLOAT)tx+1, (FLOAT)ty+1),
									color.textBrush,
									D2D1_DRAW_TEXT_OPTIONS_NO_SNAP,
									DWRITE_MEASURING_MODE_GDI_NATURAL
									);
							}
							x=x2;
						}
					}

					if(element->GetCaretVisible() && element->GetLines().IsAvailable(element->GetCaretEnd()))
					{
						Point caretPoint=element->GetLines().GetPointFromTextPos(element->GetCaretEnd());
						vint height=element->GetLines().GetRowHeight();
						Point p1(caretPoint.x-viewPosition.x+bounds.x1, caretPoint.y-viewPosition.y+bounds.y1+1);
						Point p2(caretPoint.x-viewPosition.x+bounds.x1, caretPoint.y+height-viewPosition.y+bounds.y1-1);
						d2dRenderTarget->DrawLine(
							D2D1::Point2F((FLOAT)p1.x+0.5f, (FLOAT)p1.y),
							D2D1::Point2F((FLOAT)p2.x+0.5f, (FLOAT)p2.y),
							caretBrush
							);
						d2dRenderTarget->DrawLine(
							D2D1::Point2F((FLOAT)p1.x-0.5f, (FLOAT)p1.y),
							D2D1::Point2F((FLOAT)p2.x-0.5f, (FLOAT)p2.y),
							caretBrush
							);
					}
				}
			}

			void GuiColorizedTextElementRenderer::OnElementStateChanged()
			{
				if(renderTarget)
				{
					Color caretColor=element->GetCaretColor();
					if(oldCaretColor!=caretColor)
					{
						DestroyCaretBrush(renderTarget);
						CreateCaretBrush(renderTarget);
					}
				}
			}

/***********************************************************************
GuiDirect2DElementRenderer
***********************************************************************/

			void GuiDirect2DElementRenderer::InitializeInternal()
			{
			}

			void GuiDirect2DElementRenderer::FinalizeInternal()
			{
			}

			void GuiDirect2DElementRenderer::RenderTargetChangedInternal(IWindowsDirect2DRenderTarget* oldRenderTarget, IWindowsDirect2DRenderTarget* newRenderTarget)
			{
				IDWriteFactory* fdw=GetWindowsDirect2DObjectProvider()->GetDirectWriteFactory();
				ID2D1Factory* fd2d=GetWindowsDirect2DObjectProvider()->GetDirect2DFactory();
				if(oldRenderTarget)
				{
					GuiDirect2DElementEventArgs arguments(element, oldRenderTarget->GetDirect2DRenderTarget(), fdw, fd2d, Rect());
					element->BeforeRenderTargetChanged.Execute(arguments);
				}
				if(newRenderTarget)
				{
					GuiDirect2DElementEventArgs arguments(element, newRenderTarget->GetDirect2DRenderTarget(), fdw, fd2d, Rect());
					element->AfterRenderTargetChanged.Execute(arguments);
				}
			}

			GuiDirect2DElementRenderer::GuiDirect2DElementRenderer()
			{
			}

			GuiDirect2DElementRenderer::~GuiDirect2DElementRenderer()
			{
			}
			
			void GuiDirect2DElementRenderer::Render(Rect bounds)
			{
				if(renderTarget)
				{
					IDWriteFactory* fdw=GetWindowsDirect2DObjectProvider()->GetDirectWriteFactory();
					ID2D1Factory* fd2d=GetWindowsDirect2DObjectProvider()->GetDirect2DFactory();
					renderTarget->PushClipper(bounds);
					if(!renderTarget->IsClipperCoverWholeTarget())
					{
						ID2D1RenderTarget* rt=renderTarget->GetDirect2DRenderTarget();
						GuiDirect2DElementEventArgs arguments(element, rt, fdw, fd2d, bounds);
						element->Rendering.Execute(arguments);
					}
					renderTarget->PopClipper();
				}
			}

			void GuiDirect2DElementRenderer::OnElementStateChanged()
			{
			}
		}
	}
}