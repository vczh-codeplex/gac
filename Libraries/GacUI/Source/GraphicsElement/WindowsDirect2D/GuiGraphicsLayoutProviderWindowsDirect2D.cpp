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
WindowsDirect2DParagraph
***********************************************************************/

			class WindowsDirect2DParagraph : public Object, public IGuiGraphicsParagraph
			{
			protected:
				IGuiGraphicsLayoutProvider*			provider;
				ID2D1SolidColorBrush*				defaultTextColor;
				IDWriteFactory*						dwriteFactory;
				IWindowsDirect2DRenderTarget*		renderTarget;
				ComPtr<IDWriteTextLayout>			textLayout;
				bool								wrapLine;
				int									maxWidth;
				List<Color>							usedColors;

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
					FOREACH(Color, color, usedColors.Wrap())
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
					return false;
				}

				bool ResetInlineObject(int start, int length)override
				{
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