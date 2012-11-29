#include "GuiGraphicsWindowsDirect2D.h"
#include "GuiGraphicsLayoutProviderWindowsDirect2D.h"

namespace vl
{
	namespace presentation
	{
		using namespace elements;
		using namespace collections;

		namespace elements_windows_d2d
		{

/***********************************************************************
WindowsDirect2DElementInlineObject
***********************************************************************/

			class WindowsDirect2DElementInlineObject : public IDWriteInlineObject
			{
			protected:
				int													counter;
				IGuiGraphicsParagraph::InlineObjectProperties		properties;
				Ptr<IGuiGraphicsElement>							element;
				int													start;
				int													length;

			public:
				WindowsDirect2DElementInlineObject(
					const IGuiGraphicsParagraph::InlineObjectProperties& _properties,
					Ptr<IGuiGraphicsElement> _element,
					int _start,
					int _length
					)
					:counter(1)
					,properties(_properties)
					,element(_element)
					,start(_start)
					,length(_length)
				{
				}

				~WindowsDirect2DElementInlineObject()
				{
					IGuiGraphicsRenderer* graphicsRenderer=element->GetRenderer();
					if(graphicsRenderer)
					{
						graphicsRenderer->SetRenderTarget(0);
					}
				}

				int GetStart()
				{
					return start;
				}

				int GetLength()
				{
					return length;
				}

				HRESULT STDMETHODCALLTYPE QueryInterface( 
					REFIID riid,
					_COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject
					)
				{
					if(ppvObject)
					{
						*ppvObject=NULL;
					}
					return E_NOINTERFACE;
				}

				ULONG STDMETHODCALLTYPE AddRef(void)
				{
					++counter;
					return S_OK;
				}

				ULONG STDMETHODCALLTYPE Release(void)
				{
					if(--counter==0)
					{
						delete this;
					}
					return S_OK;
				}

				STDMETHOD(Draw)(
					_In_opt_ void* clientDrawingContext,
					IDWriteTextRenderer* renderer,
					FLOAT originX,
					FLOAT originY,
					BOOL isSideways,
					BOOL isRightToLeft,
					_In_opt_ IUnknown* clientDrawingEffect
					)override
				{
					IGuiGraphicsRenderer* graphicsRenderer=element->GetRenderer();
					if(graphicsRenderer)
					{
						Rect bounds(Point((int)originX, (int)originY), properties.size);
						graphicsRenderer->Render(bounds);
					}
					return S_OK;
				}

				STDMETHOD(GetMetrics)(
					_Out_ DWRITE_INLINE_OBJECT_METRICS* metrics
					)override
				{
					metrics->width=(FLOAT)properties.size.x;
					metrics->height=(FLOAT)properties.size.y;
					metrics->baseline=(FLOAT)(properties.baseline==-1?properties.size.y:properties.baseline);
					metrics->supportsSideways=TRUE;
					return S_OK;
				}

				STDMETHOD(GetOverhangMetrics)(
					_Out_ DWRITE_OVERHANG_METRICS* overhangs
					)override
				{
					overhangs->left=0;
					overhangs->right=0;
					overhangs->top=0;
					overhangs->bottom=0;
					return S_OK;
				}

				STDMETHOD(GetBreakConditions)(
					_Out_ DWRITE_BREAK_CONDITION* breakConditionBefore,
					_Out_ DWRITE_BREAK_CONDITION* breakConditionAfter
					)override
				{
					switch(properties.breakCondition)
					{
					case IGuiGraphicsParagraph::StickToPreviousRun:
						*breakConditionBefore=DWRITE_BREAK_CONDITION_MAY_NOT_BREAK;
						*breakConditionAfter=DWRITE_BREAK_CONDITION_CAN_BREAK;
						break;
					case IGuiGraphicsParagraph::StickToNextRun:
						*breakConditionBefore=DWRITE_BREAK_CONDITION_CAN_BREAK;
						*breakConditionAfter=DWRITE_BREAK_CONDITION_MAY_NOT_BREAK;
						break;
					default:
						*breakConditionBefore=DWRITE_BREAK_CONDITION_CAN_BREAK;
						*breakConditionAfter=DWRITE_BREAK_CONDITION_CAN_BREAK;
					}
					return S_OK;
				}
			};

/***********************************************************************
WindowsDirect2DParagraph
***********************************************************************/

			class WindowsDirect2DParagraph : public Object, public IGuiGraphicsParagraph
			{
				typedef Dictionary<IGuiGraphicsElement*, ComPtr<WindowsDirect2DElementInlineObject>>		InlineElementMap;
			protected:
				IGuiGraphicsLayoutProvider*			provider;
				ID2D1SolidColorBrush*				defaultTextColor;
				IDWriteFactory*						dwriteFactory;
				IWindowsDirect2DRenderTarget*		renderTarget;
				ComPtr<IDWriteTextLayout>			textLayout;
				bool								wrapLine;
				int									maxWidth;
				List<Color>							usedColors;
				InlineElementMap					inlineElements;

			public:
				WindowsDirect2DParagraph(IGuiGraphicsLayoutProvider* _provider, const WString& _text, IGuiGraphicsRenderTarget* _renderTarget)
					:provider(_provider)
					,dwriteFactory(GetWindowsDirect2DObjectProvider()->GetDirectWriteFactory())
					,renderTarget(dynamic_cast<IWindowsDirect2DRenderTarget*>(_renderTarget))
					,textLayout(0)
					,wrapLine(true)
					,maxWidth(-1)
				{
					FontProperties defaultFont=GetCurrentController()->ResourceService()->GetDefaultFont();
					Direct2DTextFormatPackage* package=GetWindowsDirect2DResourceManager()->CreateDirect2DTextFormat(defaultFont);
					defaultTextColor=renderTarget->CreateDirect2DBrush(Color(0, 0, 0));
					usedColors.Add(Color(0, 0, 0));

					IDWriteTextLayout* rawTextLayout=0;
					HRESULT hr=dwriteFactory->CreateTextLayout(
						_text.Buffer(),
						_text.Length(),
						package->textFormat.Obj(),
						0,
						0,
						&rawTextLayout);
					if(!FAILED(hr))
					{
						textLayout=rawTextLayout;
						textLayout->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
					}

					GetWindowsDirect2DResourceManager()->DestroyDirect2DTextFormat(defaultFont);
				}

				~WindowsDirect2DParagraph()
				{
					FOREACH(Color, color, usedColors)
					{
						renderTarget->DestroyDirect2DBrush(color);
					}
				}

				IGuiGraphicsLayoutProvider* GetProvider()override
				{
					return provider;
				}

				IGuiGraphicsRenderTarget* GetRenderTarget()override
				{
					return renderTarget;
				}

				bool GetWrapLine()override
				{
					return wrapLine;
				}

				void SetWrapLine(bool value)override
				{
					if(wrapLine!=value)
					{
						wrapLine=value;
						textLayout->SetWordWrapping(value?DWRITE_WORD_WRAPPING_WRAP:DWRITE_WORD_WRAPPING_NO_WRAP);
					}
				}

				int GetMaxWidth()override
				{
					return maxWidth;
				}

				void SetMaxWidth(int value)override
				{
					if(maxWidth!=value)
					{
						maxWidth=value;
						textLayout->SetMaxWidth(value==-1?65536:(FLOAT)value);
					}
				}

				bool SetFont(int start, int length, const WString& value)override
				{
					if(length==0) return true;
					DWRITE_TEXT_RANGE range;
					range.startPosition=start;
					range.length=length;
					HRESULT hr=textLayout->SetFontFamilyName(value.Buffer(), range);
					return !FAILED(hr);
				}

				bool SetSize(int start, int length, int value)override
				{
					if(length==0) return true;
					DWRITE_TEXT_RANGE range;
					range.startPosition=start;
					range.length=length;
					HRESULT hr=textLayout->SetFontSize((FLOAT)value, range);
					return !FAILED(hr);
				}

				bool SetStyle(int start, int length, TextStyle value)override
				{
					if(length==0) return true;
					DWRITE_TEXT_RANGE range;
					range.startPosition=start;
					range.length=length;
					HRESULT hr=S_OK;

					hr=textLayout->SetFontStyle(value&Italic?DWRITE_FONT_STYLE_ITALIC:DWRITE_FONT_STYLE_NORMAL, range);
					if(FAILED(hr)) return false;
					hr=textLayout->SetFontWeight(value&Bold?DWRITE_FONT_WEIGHT_BOLD:DWRITE_FONT_WEIGHT_NORMAL, range);
					if(FAILED(hr)) return false;
					hr=textLayout->SetUnderline(value&Underline?TRUE:FALSE, range);
					if(FAILED(hr)) return false;
					hr=textLayout->SetStrikethrough(value&Strikeline?TRUE:FALSE, range);
					if(FAILED(hr)) return false;

					return true;
				}

				bool SetColor(int start, int length, Color value)override
				{
					if(length==0) return true;
					ID2D1SolidColorBrush* brush=renderTarget->CreateDirect2DBrush(value);
					usedColors.Add(value);

					DWRITE_TEXT_RANGE range;
					range.startPosition=start;
					range.length=length;
					HRESULT hr=textLayout->SetDrawingEffect(brush, range);
					return !FAILED(hr);
				}

				bool SetInlineObject(int start, int length, const InlineObjectProperties& properties, Ptr<IGuiGraphicsElement> value)override
				{
					if(inlineElements.Keys().Contains(value.Obj()))
					{
						return false;
					}
					for(int i=0;i<inlineElements.Count();i++)
					{
						ComPtr<WindowsDirect2DElementInlineObject> inlineObject=inlineElements.Values().Get(i);
						if(start<inlineObject->GetStart()+inlineObject->GetLength() && inlineObject->GetStart()<start+length)
						{
							return false;
						}
					}
					ComPtr<WindowsDirect2DElementInlineObject> inlineObject=new WindowsDirect2DElementInlineObject(properties, value, start, length);
					DWRITE_TEXT_RANGE range;
					range.startPosition=start;
					range.length=length;
					HRESULT hr=textLayout->SetInlineObject(inlineObject.Obj(), range);
					if(!FAILED(hr))
					{
						IGuiGraphicsRenderer* renderer=value->GetRenderer();
						if(renderer)
						{
							renderer->SetRenderTarget(renderTarget);
						}
						inlineElements.Add(value.Obj(), inlineObject);
						return true;
					}
					else
					{
						return false;
					}
				}

				bool ResetInlineObject(int start, int length)override
				{
					for(int i=0;i<inlineElements.Count();i++)
					{
						IGuiGraphicsElement* element=inlineElements.Keys().Get(i);
						ComPtr<WindowsDirect2DElementInlineObject> inlineObject=inlineElements.Values().Get(i);
						if(inlineObject->GetStart()==start && inlineObject->GetLength()==length)
						{
							DWRITE_TEXT_RANGE range;
							range.startPosition=start;
							range.length=length;
							HRESULT hr=textLayout->SetInlineObject(NULL, range);
							if(!FAILED(hr))
							{
								inlineElements.Remove(element);
								return true;
							}
							else
							{
								return false;
							}
						}
					}
					return false;
				}

				int GetHeight()override
				{
					DWRITE_TEXT_METRICS metrics;
					textLayout->GetMetrics(&metrics);
					return (int)metrics.height;
				}

				void Render(Rect bounds)override
				{
					renderTarget->GetDirect2DRenderTarget()->DrawTextLayout(
						D2D1::Point2F((FLOAT)bounds.Left(), (FLOAT)bounds.Top()),
						textLayout.Obj(),
						defaultTextColor,
						D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
				}
			};

/***********************************************************************
WindowsDirect2DLayoutProvider
***********************************************************************/

			Ptr<IGuiGraphicsParagraph> WindowsDirect2DLayoutProvider::CreateParagraph(const WString& text, IGuiGraphicsRenderTarget* renderTarget)
			{
				return new WindowsDirect2DParagraph(this, text, renderTarget);
			}
		}
	}
}