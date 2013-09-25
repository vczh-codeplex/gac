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
				vint												counter;
				IGuiGraphicsParagraph::InlineObjectProperties		properties;
				Ptr<IGuiGraphicsElement>							element;
				vint												start;
				vint												length;

			public:
				WindowsDirect2DElementInlineObject(
					const IGuiGraphicsParagraph::InlineObjectProperties& _properties,
					Ptr<IGuiGraphicsElement> _element,
					vint _start,
					vint _length
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

				vint GetStart()
				{
					return start;
				}

				vint GetLength()
				{
					return length;
				}

				HRESULT STDMETHODCALLTYPE QueryInterface( 
					REFIID riid,
					void __RPC_FAR *__RPC_FAR *ppvObject
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
					void* clientDrawingContext,
					IDWriteTextRenderer* renderer,
					FLOAT originX,
					FLOAT originY,
					BOOL isSideways,
					BOOL isRightToLeft,
					IUnknown* clientDrawingEffect
					)override
				{
					IGuiGraphicsRenderer* graphicsRenderer=element->GetRenderer();
					if(graphicsRenderer)
					{
						Rect bounds(Point((vint)originX, (vint)originY), properties.size);
						graphicsRenderer->Render(bounds);
					}
					return S_OK;
				}

				STDMETHOD(GetMetrics)(
					DWRITE_INLINE_OBJECT_METRICS* metrics
					)override
				{
					metrics->width=(FLOAT)properties.size.x;
					metrics->height=(FLOAT)properties.size.y;
					metrics->baseline=(FLOAT)(properties.baseline==-1?properties.size.y:properties.baseline);
					metrics->supportsSideways=TRUE;
					return S_OK;
				}

				STDMETHOD(GetOverhangMetrics)(
					DWRITE_OVERHANG_METRICS* overhangs
					)override
				{
					overhangs->left=0;
					overhangs->right=0;
					overhangs->top=0;
					overhangs->bottom=0;
					return S_OK;
				}

				STDMETHOD(GetBreakConditions)(
					DWRITE_BREAK_CONDITION* breakConditionBefore,
					DWRITE_BREAK_CONDITION* breakConditionAfter
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
				typedef Dictionary<Pair<vint, vint>, vint>													InteractionIdMap;
			protected:
				IGuiGraphicsLayoutProvider*			provider;
				ID2D1SolidColorBrush*				defaultTextColor;
				IDWriteFactory*						dwriteFactory;
				IWindowsDirect2DRenderTarget*		renderTarget;
				WString								paragraphText;
				ComPtr<IDWriteTextLayout>			textLayout;
				bool								wrapLine;
				vint								maxWidth;
				List<Color>							usedColors;
				InlineElementMap					inlineElements;
				InteractionIdMap					interactionIds;

				vint								caret;
				Color								caretColor;
				bool								caretFrontSide;
				ID2D1SolidColorBrush*				caretBrush;

				bool								formatDataAvailable;
				Array<DWRITE_LINE_METRICS>			lineMetrics;
				Array<vint>							lineStarts;
				Array<DWRITE_CLUSTER_METRICS>		clusterMetrics;
				Array<DWRITE_HIT_TEST_METRICS>		hitTestMetrics;
				Array<vint>							charHitTestMap;

/***********************************************************************
WindowsDirect2DParagraph (Initialization)
***********************************************************************/

				void PrepareFormatData()
				{
					if(!formatDataAvailable)
					{
						formatDataAvailable=true;
						{
							UINT32 lineCount=0;
							textLayout->GetLineMetrics(NULL, 0, &lineCount);
							lineMetrics.Resize(lineCount);
							if(lineCount>0)
							{
								textLayout->GetLineMetrics(&lineMetrics[0], lineCount, &lineCount);
							}

							lineStarts.Resize(lineCount);
							vint start=0;
							for(vint i=0;i<lineMetrics.Count();i++)
							{
								DWRITE_LINE_METRICS& metrics=lineMetrics[i];
								lineStarts[i]=start;
								start+=metrics.length+metrics.newlineLength;
							}
						}
						{
							UINT32 clusterCount=0;
							textLayout->GetClusterMetrics(NULL, 0, &clusterCount);
							clusterMetrics.Resize(clusterCount);
							if(clusterCount>0)
							{
								textLayout->GetClusterMetrics(&clusterMetrics[0], clusterCount, &clusterCount);
							}
						}
						{
							vint textPos=0;
							hitTestMetrics.Resize(clusterMetrics.Count());
							for(vint i=0;i<hitTestMetrics.Count();i++)
							{
								FLOAT x=0;
								FLOAT y=0;
								DWRITE_HIT_TEST_METRICS& metrics=hitTestMetrics[i];
								textLayout->HitTestTextPosition(textPos, FALSE, &x, &y, &metrics);
								textPos+=metrics.length;
							}
						}
						{
							charHitTestMap.Resize(paragraphText.Length());
							for(vint i=0;i<hitTestMetrics.Count();i++)
							{
								DWRITE_HIT_TEST_METRICS& metrics=hitTestMetrics[i];
								for(UINT32 j=0;j<metrics.length;j++)
								{
									charHitTestMap[metrics.textPosition+j]=i;
								}
							}
						}
					}
				}
			public:
				WindowsDirect2DParagraph(IGuiGraphicsLayoutProvider* _provider, const WString& _text, IGuiGraphicsRenderTarget* _renderTarget)
					:provider(_provider)
					,dwriteFactory(GetWindowsDirect2DObjectProvider()->GetDirectWriteFactory())
					,renderTarget(dynamic_cast<IWindowsDirect2DRenderTarget*>(_renderTarget))
					,paragraphText(_text)
					,textLayout(0)
					,wrapLine(true)
					,maxWidth(-1)
					,caret(-1)
					,caretFrontSide(false)
					,caretBrush(0)
					,formatDataAvailable(false)
				{
					FontProperties defaultFont=GetCurrentController()->ResourceService()->GetDefaultFont();
					Direct2DTextFormatPackage* package=GetWindowsDirect2DResourceManager()->CreateDirect2DTextFormat(defaultFont);
					defaultTextColor=renderTarget->CreateDirect2DBrush(Color(0, 0, 0));
					usedColors.Add(Color(0, 0, 0));

					IDWriteTextLayout* rawTextLayout=0;
					HRESULT hr=dwriteFactory->CreateTextLayout(
						_text.Buffer(),
						(int)_text.Length(),
						package->textFormat.Obj(),
						0,
						0,
						&rawTextLayout);
					if(!FAILED(hr))
					{
						textLayout=rawTextLayout;
						textLayout->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
					}
					interactionIds.Add(Pair<vint, vint>(0, _text.Length()), NullInteractionId);

					GetWindowsDirect2DResourceManager()->DestroyDirect2DTextFormat(defaultFont);
				}

				~WindowsDirect2DParagraph()
				{
					CloseCaret();
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

/***********************************************************************
WindowsDirect2DParagraph (Formatting)
***********************************************************************/

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
						formatDataAvailable=false;
					}
				}

				vint GetMaxWidth()override
				{
					return maxWidth;
				}

				void SetMaxWidth(vint value)override
				{
					if(maxWidth!=value)
					{
						maxWidth=value;
						textLayout->SetMaxWidth(value==-1?65536:(FLOAT)value);
						formatDataAvailable=false;
					}
				}

				Alignment GetParagraphAlignment()override
				{
					switch(textLayout->GetTextAlignment())
					{
					case DWRITE_TEXT_ALIGNMENT_LEADING:
						return Alignment::Left;
					case DWRITE_TEXT_ALIGNMENT_CENTER:
						return Alignment::Center;
					case DWRITE_TEXT_ALIGNMENT_TRAILING:
						return Alignment::Right;
					default:
						return Alignment::Left;
					}
				}

				void SetParagraphAlignment(Alignment value)override
				{
					formatDataAvailable=false;
					switch(value)
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
				}

				bool SetFont(vint start, vint length, const WString& value)override
				{
					if(length==0) return true;
					formatDataAvailable=false;

					DWRITE_TEXT_RANGE range;
					range.startPosition=(int)start;
					range.length=(int)length;
					HRESULT hr=textLayout->SetFontFamilyName(value.Buffer(), range);
					return !FAILED(hr);
				}

				bool SetSize(vint start, vint length, vint value)override
				{
					if(length==0) return true;
					formatDataAvailable=false;

					DWRITE_TEXT_RANGE range;
					range.startPosition=(int)start;
					range.length=(int)length;
					HRESULT hr=textLayout->SetFontSize((FLOAT)value, range);
					return !FAILED(hr);
				}

				bool SetStyle(vint start, vint length, TextStyle value)override
				{
					if(length==0) return true;
					formatDataAvailable=false;

					DWRITE_TEXT_RANGE range;
					range.startPosition=(int)start;
					range.length=(int)length;
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

				bool SetColor(vint start, vint length, Color value)override
				{
					if(length==0) return true;
					formatDataAvailable=false;

					ID2D1SolidColorBrush* brush=renderTarget->CreateDirect2DBrush(value);
					usedColors.Add(value);

					DWRITE_TEXT_RANGE range;
					range.startPosition=(int)start;
					range.length=(int)length;
					HRESULT hr=textLayout->SetDrawingEffect(brush, range);
					return !FAILED(hr);
				}

				bool SetInlineObject(vint start, vint length, const InlineObjectProperties& properties, Ptr<IGuiGraphicsElement> value)override
				{
					if(inlineElements.Keys().Contains(value.Obj()))
					{
						return false;
					}
					for(vint i=0;i<inlineElements.Count();i++)
					{
						ComPtr<WindowsDirect2DElementInlineObject> inlineObject=inlineElements.Values().Get(i);
						if(start<inlineObject->GetStart()+inlineObject->GetLength() && inlineObject->GetStart()<start+length)
						{
							return false;
						}
					}
					formatDataAvailable=false;

					ComPtr<WindowsDirect2DElementInlineObject> inlineObject=new WindowsDirect2DElementInlineObject(properties, value, start, length);
					DWRITE_TEXT_RANGE range;
					range.startPosition=(int)start;
					range.length=(int)length;
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

				bool ResetInlineObject(vint start, vint length)override
				{
					for(vint i=0;i<inlineElements.Count();i++)
					{
						IGuiGraphicsElement* element=inlineElements.Keys().Get(i);
						ComPtr<WindowsDirect2DElementInlineObject> inlineObject=inlineElements.Values().Get(i);
						if(inlineObject->GetStart()==start && inlineObject->GetLength()==length)
						{
							DWRITE_TEXT_RANGE range;
							range.startPosition=(int)start;
							range.length=(int)length;
							HRESULT hr=textLayout->SetInlineObject(NULL, range);
							if(!FAILED(hr))
							{
								formatDataAvailable=false;
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

				vint SearchInInteractionIdMap(vint textPosition)
				{
					vint start=0;
					vint end=interactionIds.Count()-1;
					while(start<=end)
					{
						vint middle=start+(end-start)/2;
						Pair<vint, vint> p=interactionIds.Keys()[middle];
						if(textPosition<p.key)
						{
							end=middle-1;
						}
						else if(p.key+p.value<=textPosition)
						{
							start=middle+1;
						}
						else
						{
							return middle;
						}
					}
					return -1;
				}

				vint CutInteractionIdMap(vint textPosition, vint index)
				{
					Pair<vint, vint> p=interactionIds.Keys()[index];
					CHECK_ERROR(p.key<=textPosition && textPosition<=p.key+p.value, L"WindowsDirect2DParagraph::CutInteractionIdMap(vint, vint)#textPositionÓëindex²»Æ¥Åä¡£");
					if(textPosition==p.key || textPosition==p.key+p.value)
					{
						return 0;
					}
					else
					{
						vint id=interactionIds.Values().Get(index);
						Pair<vint, vint> bp=p;
						Pair<vint, vint> ep=p;
						bp.value=textPosition-p.key;
						ep.key=textPosition;
						ep.value=p.value-bp.value;

						interactionIds.Remove(p);
						interactionIds.Add(bp, id);
						interactionIds.Add(ep, id);
						return 1;
					}
				}

				bool SetInteractionId(vint start, vint length, vint value)override
				{
					vint begin=SearchInInteractionIdMap(start);
					vint end=SearchInInteractionIdMap(start+length-1);
					if(begin==-1 || end==-1) return false;
					formatDataAvailable=false;

					vint offset=CutInteractionIdMap(start, begin);
					begin+=offset;
					end+=offset;
					CutInteractionIdMap(start+length, end);
					for(vint i=begin;i<=end;i++)
					{
						interactionIds.Set(interactionIds.Keys()[i], value);
					}
					return true;
				}

				bool HitTestPoint(Point point, vint& start, vint& length, vint& interactionId)override
				{
					DWRITE_HIT_TEST_METRICS metrics={0};
					BOOL trailingHit=FALSE;
					BOOL inside=FALSE;
					start=-1;
					length=0;
					interactionId=NullInteractionId;
					HRESULT hr=textLayout->HitTestPoint((FLOAT)point.x, (FLOAT)point.y, &trailingHit, &inside, &metrics);
					if(hr!=S_OK) return false;

					start=metrics.textPosition;
					length=metrics.length;
					vint index=SearchInInteractionIdMap(start);
					interactionId=index==-1?NullInteractionId:interactionIds.Values().Get(index);

					return inside==TRUE;
				}

				vint GetHeight()override
				{
					DWRITE_TEXT_METRICS metrics;
					textLayout->GetMetrics(&metrics);
					return (vint)metrics.height;
				}

/***********************************************************************
WindowsDirect2DParagraph (Rendering)
***********************************************************************/

				bool OpenCaret(vint _caret, Color _color, bool _frontSide)override
				{
					if(!IsValidCaret(_caret)) return false;
					if(caret!=-1) CloseCaret();
					caret=_caret;
					caretColor=_color;
					caretFrontSide=_frontSide;
					caretBrush=renderTarget->CreateDirect2DBrush(caretColor);
					return true;
				}

				bool CloseCaret()override
				{
					if(caret==-1) return false;
					caret=-1;
					renderTarget->DestroyDirect2DBrush(caretColor);
					caretBrush=0;
					return true;
				}

				void Render(Rect bounds)override
				{
					renderTarget->GetDirect2DRenderTarget()->DrawTextLayout(
						D2D1::Point2F((FLOAT)bounds.Left(), (FLOAT)bounds.Top()),
						textLayout.Obj(),
						defaultTextColor,
						D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);

					if(caret!=-1)
					{
						Rect caretBounds=GetCaretBounds(caret, caretFrontSide);
						vint x=caretBounds.x1+bounds.x1;
						vint y1=caretBounds.y1+bounds.y1;
						vint y2=y1+caretBounds.Height();

						renderTarget->GetDirect2DRenderTarget()->DrawLine(
							D2D1::Point2F((FLOAT)x-0.5f, (FLOAT)y1+0.5f),
							D2D1::Point2F((FLOAT)x-0.5f, (FLOAT)y2+0.5f),
							caretBrush
							);
						renderTarget->GetDirect2DRenderTarget()->DrawLine(
							D2D1::Point2F((FLOAT)x+0.5f, (FLOAT)y1+0.5f),
							D2D1::Point2F((FLOAT)x+0.5f, (FLOAT)y2+0.5f),
							caretBrush
							);
					}
				}

/***********************************************************************
WindowsDirect2DParagraph (Caret Helper)
***********************************************************************/

				void CorrectCaretSide(vint caret, bool& frontSide)
				{
					if(caret==0)
					{
						frontSide=false;
					}
					else if(caret==paragraphText.Length())
					{
						frontSide=true;
					}
				}

				vint GetCaretHitTestIndex(vint caret, bool& frontSide)
				{
					CorrectCaretSide(caret, frontSide);
					if(frontSide)
					{
						caret--;
					}

					return charHitTestMap[caret];
				}

				void GetLineIndexFromTextPos(vint textPos, vint& frontLineIndex, vint& backLineIndex)
				{
					frontLineIndex=-1;
					backLineIndex=-1;
					vint start=0;
					vint end=lineMetrics.Count()-1;
					while(start<=end)
					{
						vint middle=(start+end)/2;
						DWRITE_LINE_METRICS& metrics=lineMetrics[middle];
						vint lineStart=lineStarts[middle];
						vint lineEnd=lineStart+metrics.length;

						if(textPos<lineStart)
						{
							end=middle-1;
						}
						else if(textPos>lineEnd)
						{
							start=middle+1;
						}
						else if(textPos==lineStart && middle!=0)
						{
							DWRITE_LINE_METRICS& anotherLine=lineMetrics[middle-1];
							frontLineIndex=anotherLine.newlineLength==0?middle-1:middle;
							backLineIndex=middle;
							return;
						}
						else if(textPos==lineEnd && middle!=lineMetrics.Count()-1)
						{
							frontLineIndex=middle;
							backLineIndex=metrics.newlineLength==0?middle+1:middle;
							return;
						}
						else
						{
							frontLineIndex=middle;
							backLineIndex=middle;
							return;
						}
					}
				}

/***********************************************************************
WindowsDirect2DParagraph (Caret)
***********************************************************************/

				vint GetCaret(vint comparingCaret, CaretRelativePosition position, bool& preferFrontSide)override
				{
					PrepareFormatData();
					if(position==CaretFirst) return 0;
					if(position==CaretLast) return paragraphText.Length();
					if(!IsValidCaret(comparingCaret)) return -1;

					if(position==CaretMoveLeft)
					{
						if(comparingCaret==0) return 0;
						return hitTestMetrics[charHitTestMap[comparingCaret-1]].textPosition;
					}
					if(position==CaretMoveRight)
					{
						if(comparingCaret==paragraphText.Length()) return paragraphText.Length();
						vint index=charHitTestMap[comparingCaret];
						if(index==hitTestMetrics.Count()-1) return paragraphText.Length();
						return hitTestMetrics[index+1].textPosition;
					}

					vint frontLineIndex=-1;
					vint backLineIndex=-1;
					GetLineIndexFromTextPos(comparingCaret, frontLineIndex, backLineIndex);
					vint lineIndex=preferFrontSide?frontLineIndex:backLineIndex;
					DWRITE_LINE_METRICS& line=lineMetrics[lineIndex];

					if(position==CaretLineFirst)
					{
						return lineStarts[lineIndex];
					}
					if(position==CaretLineLast)
					{
						return lineStarts[lineIndex]+line.length;
					}
					throw 0;
				}

				Rect GetCaretBounds(vint caret, bool frontSide)override
				{
					PrepareFormatData();
					if(!IsValidCaret(caret)) return Rect();
					if(paragraphText.Length()==0) return Rect(Point(0, 0), Size(0, GetHeight()));

					vint index=GetCaretHitTestIndex(caret, frontSide);
					DWRITE_HIT_TEST_METRICS& hitTest=hitTestMetrics[index];
					DWRITE_CLUSTER_METRICS& cluster=clusterMetrics[index];
					if(cluster.isRightToLeft)
					{
						frontSide=!frontSide;
					}

					if(frontSide)
					{
						return Rect(
							Point((vint)(hitTest.left+hitTest.width), (vint)hitTest.top),
							Size(0, (vint)hitTest.height)
							);
					}
					else
					{
						return Rect(
							Point((vint)hitTest.left, (vint)hitTest.top),
							Size(0, (vint)hitTest.height)
							);
					}
				}

				vint GetCaretFromPoint(Point point)override
				{
					PrepareFormatData();
					throw 0;
				}

				vint GetNearestCaretFromTextPos(vint textPos, bool frontSide)override
				{
					PrepareFormatData();
					if(!IsValidTextPos(textPos)) return -1;
					if(textPos==0 || textPos==paragraphText.Length()) return textPos;
				
					vint index=charHitTestMap[textPos];
					DWRITE_HIT_TEST_METRICS& hitTest=hitTestMetrics[index];
					if(hitTest.textPosition==textPos)
					{
						return textPos;
					}
					else if(frontSide)
					{
						return hitTest.textPosition;
					}
					else
					{
						return hitTest.textPosition+hitTest.length;
					}
				}

				bool IsValidCaret(vint caret)override
				{
					PrepareFormatData();
					if(!IsValidTextPos(caret)) return false;
					if(caret==0 || caret==paragraphText.Length()) return true;
					if(hitTestMetrics[charHitTestMap[caret]].textPosition==caret) return true;
					return false;
				}

				bool IsValidTextPos(vint textPos)
				{
					return 0<=textPos && textPos<=paragraphText.Length();
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