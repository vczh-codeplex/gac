#include "GuiGraphicsDocumentElement.h"
#include "..\..\..\..\Common\Source\Stream\MemoryStream.h"
#include "..\..\..\..\Common\Source\Stream\Accessor.h"

namespace vl
{
	using namespace collections;

	namespace presentation
	{
		namespace elements
		{

/***********************************************************************
Visitors
***********************************************************************/

			namespace visitors
			{
				class ExtractTextVisitor : public Object, public DocumentRun::IVisitor
				{
				public:
					stream::StreamWriter&				writer;

					ExtractTextVisitor(stream::StreamWriter& _writer)
						:writer(_writer)
					{
					}

					void VisitContainer(DocumentContainerRun* run)
					{
						FOREACH(Ptr<DocumentRun>, subRun, run->runs)
						{
							subRun->Accept(this);
						}
					}

					void Visit(DocumentTextRun* run)override
					{
						writer.WriteString(run->text);
					}

					void Visit(DocumentStylePropertiesRun* run)override
					{
						VisitContainer(run);
					}

					void Visit(DocumentStyleApplicationRun* run)override
					{
						VisitContainer(run);
					}

					void Visit(DocumentHyperlinkTextRun* run)override
					{
						VisitContainer(run);
					}

					void Visit(DocumentImageRun* run)override
					{
						writer.WriteString(L"[Image]");
					}

					void Visit(DocumentTemplateApplicationRun* run)override
					{
						VisitContainer(run);
					}

					void Visit(DocumentTemplateContentRun* run)override
					{
						VisitContainer(run);
					}

					void Visit(DocumentParagraphRun* run)override
					{
						VisitContainer(run);
					}

					static WString ExtractText(Ptr<DocumentParagraphRun> run)
					{
						stream::MemoryStream stream;
						{
							stream::StreamWriter writer(stream);
							ExtractTextVisitor visitor(writer);
							run->Accept(&visitor);
						}

						stream.SeekFromBegin(0);
						stream::StreamReader reader(stream);
						return reader.ReadToEnd();
					}
				};

				class SetPropertiesVisitor : public Object, public DocumentRun::IVisitor
				{
					typedef DocumentModel::ResolvedStyle			ResolvedStyle;
				public:
					vint						start;
					vint						length;
					vint						selectionBegin;
					vint						selectionEnd;
					vint						hyperlinkId;
					List<ResolvedStyle>			styles;
					DocumentModel*				model;
					IGuiGraphicsParagraph*		paragraph;

					SetPropertiesVisitor(DocumentModel* _model, IGuiGraphicsParagraph* _paragraph, vint _selectionBegin, vint _selectionEnd)
						:start(0)
						,length(0)
						,hyperlinkId(DocumentRun::NullHyperlinkId)
						,model(_model)
						,paragraph(_paragraph)
						,selectionBegin(_selectionBegin)
						,selectionEnd(_selectionEnd)
					{
						ResolvedStyle style;
						style=model->GetStyle(DocumentModel::DefaultStyleName, style);
						styles.Add(style);
					}

					void VisitContainer(DocumentContainerRun* run)
					{
						FOREACH(Ptr<DocumentRun>, subRun, run->runs)
						{
							subRun->Accept(this);
						}
					}

					void ApplyStyle(vint start, vint length, vint hyperlinkedId, const ResolvedStyle& style)
					{
						paragraph->SetFont(start, length, style.style.fontFamily);
						paragraph->SetSize(start, length, style.style.size);
						paragraph->SetStyle(start, length, 
							(IGuiGraphicsParagraph::TextStyle)
							( (style.style.bold?IGuiGraphicsParagraph::Bold:0)
							| (style.style.italic?IGuiGraphicsParagraph::Italic:0)
							| (style.style.underline?IGuiGraphicsParagraph::Underline:0)
							| (style.style.strikeline?IGuiGraphicsParagraph::Strikeline:0)
							));
						if(hyperlinkId!=DocumentRun::NullHyperlinkId)
						{
							paragraph->SetInteractionId(start, length, hyperlinkId);
						}
					}

					void ApplyColor(vint start, vint length, vint hyperlinkedId, const ResolvedStyle& style)
					{
						paragraph->SetColor(start, length, style.color);
						paragraph->SetBackgroundColor(start, length, style.backgroundColor);
					}

					void Visit(DocumentTextRun* run)override
					{
						length=run->text.Length();
						if(length>0)
						{
							ResolvedStyle style=styles[styles.Count()-1];
							ApplyStyle(start, length, hyperlinkId, style);
							ApplyColor(start, length, hyperlinkId, style);

							vint styleStart=start;
							vint styleEnd=styleStart+length;
							if(styleStart<selectionEnd && selectionBegin<styleEnd)
							{
								vint s2=styleStart>selectionBegin?styleStart:selectionBegin;
								vint s3=selectionEnd<styleEnd?selectionEnd:styleEnd;

								if(s2<s3)
								{
									ResolvedStyle selectionStyle=model->GetStyle(DocumentModel::SelectionStyleName, style);
									ApplyColor(s2, s3-s2, hyperlinkId, selectionStyle);
								}
							}
						}
						start+=length;
					}

					void Visit(DocumentStylePropertiesRun* run)override
					{
						ResolvedStyle style=styles[styles.Count()-1];
						style=model->GetStyle(run->style, style);
						styles.Add(style);
						VisitContainer(run);
						styles.RemoveAt(styles.Count()-1);
					}

					void Visit(DocumentStyleApplicationRun* run)override
					{
						ResolvedStyle style=styles[styles.Count()-1];
						style=model->GetStyle(run->styleName, style);
						styles.Add(style);
						VisitContainer(run);
						styles.RemoveAt(styles.Count()-1);
					}

					void Visit(DocumentHyperlinkTextRun* run)override
					{
						ResolvedStyle style=styles[styles.Count()-1];
						style=model->GetStyle(run->styleName, style);
						styles.Add(style);
						vint oldHyperlinkId=hyperlinkId;
						hyperlinkId=run->hyperlinkId;
						VisitContainer(run);
						hyperlinkId=oldHyperlinkId;
						styles.RemoveAt(styles.Count()-1);
					}

					void Visit(DocumentImageRun* run)override
					{
						// [Image]
						length=7;

						IGuiGraphicsParagraph::InlineObjectProperties properties;
						properties.size=run->size;
						properties.baseline=run->baseline;
						properties.breakCondition=IGuiGraphicsParagraph::Alone;

						Ptr<GuiImageFrameElement> element=GuiImageFrameElement::Create();
						element->SetImage(run->image, run->frameIndex);
						element->SetStretch(true);

						paragraph->SetInlineObject(start, length, properties, element);
						if(hyperlinkId!=DocumentRun::NullHyperlinkId)
						{
							paragraph->SetInteractionId(start, length, hyperlinkId);
						}
						start+=length;
					}

					void Visit(DocumentTemplateApplicationRun* run)override
					{
						VisitContainer(run);
					}

					void Visit(DocumentTemplateContentRun* run)override
					{
						VisitContainer(run);
					}

					void Visit(DocumentParagraphRun* run)override
					{
						VisitContainer(run);
					}

					static vint SetProperty(DocumentModel* model, IGuiGraphicsParagraph* paragraph, Ptr<DocumentParagraphRun> run, vint selectionBegin, vint selectionEnd)
					{
						SetPropertiesVisitor visitor(model, paragraph, selectionBegin, selectionEnd);
						run->Accept(&visitor);
						return visitor.length;
					}
				};
			}
			using namespace visitors;

/***********************************************************************
GuiDocumentElement::GuiDocumentElementRenderer
***********************************************************************/

			void GuiDocumentElement::GuiDocumentElementRenderer::InitializeInternal()
			{
			}

			void GuiDocumentElement::GuiDocumentElementRenderer::FinalizeInternal()
			{
			}

			void GuiDocumentElement::GuiDocumentElementRenderer::RenderTargetChangedInternal(IGuiGraphicsRenderTarget* oldRenderTarget, IGuiGraphicsRenderTarget* newRenderTarget)
			{
				for(vint i=0;i<paragraphCaches.Count();i++)
				{
					ParagraphCache* cache=paragraphCaches[i].Obj();
					if(cache)
					{
						cache->graphicsParagraph=0;
					}
				}
			}

			Ptr<GuiDocumentElement::GuiDocumentElementRenderer::ParagraphCache> GuiDocumentElement::GuiDocumentElementRenderer::EnsureAndGetCache(vint paragraphIndex, bool createParagraph)
			{
				if(paragraphIndex<0 || paragraphIndex>=paragraphCaches.Count()) return 0;
				Ptr<DocumentParagraphRun> paragraph=element->document->paragraphs[paragraphIndex];
				Ptr<ParagraphCache> cache=paragraphCaches[paragraphIndex];
				if(!cache)
				{
					cache=new ParagraphCache;
					cache->fullText=ExtractTextVisitor::ExtractText(paragraph);
					paragraphCaches[paragraphIndex]=cache;
				}

				if(createParagraph)
				{
					if(!cache->graphicsParagraph)
					{
						cache->graphicsParagraph=layoutProvider->CreateParagraph(cache->fullText, renderTarget);
						cache->graphicsParagraph->SetParagraphAlignment(paragraph->alignment);
						SetPropertiesVisitor::SetProperty(element->document.Obj(), cache->graphicsParagraph.Obj(), paragraph, cache->selectionBegin, cache->selectionEnd);
					}
					if(cache->graphicsParagraph->GetMaxWidth()!=lastMaxWidth)
					{
						cache->graphicsParagraph->SetMaxWidth(lastMaxWidth);
					}

					vint paragraphHeight=paragraphHeights[paragraphIndex];
					vint height=cache->graphicsParagraph->GetHeight();
					if(paragraphHeight!=height)
					{
						cachedTotalHeight+=height-paragraphHeight;
						paragraphHeight=height;
						paragraphHeights[paragraphIndex]=paragraphHeight;
						minSize=Size(0, cachedTotalHeight);
					}
				}

				return cache;
			}

			bool GuiDocumentElement::GuiDocumentElementRenderer::GetParagraphIndexFromPoint(Point point, vint& top, vint& index)
			{
				vint y=0;
				for(vint i=0;i<paragraphHeights.Count();i++)
				{
					vint paragraphHeight=paragraphHeights[i];
					if(y+paragraphHeight<=point.y)
					{
						y+=paragraphHeight+paragraphDistance;
						continue;
					}
					else if(y>point.y)
					{
						break;
					}
					else
					{
						top=y;
						index=i;
						return true;
					}
				}
				return false;
			}

			GuiDocumentElement::GuiDocumentElementRenderer::GuiDocumentElementRenderer()
				:paragraphDistance(0)
				,lastMaxWidth(-1)
				,cachedTotalHeight(0)
				,layoutProvider(GetGuiGraphicsResourceManager()->GetLayoutProvider())
				,lastCaret(-1, -1)
				,lastCaretFrontSide(false)
			{
			}

			void GuiDocumentElement::GuiDocumentElementRenderer::Render(Rect bounds)
			{
				renderTarget->PushClipper(bounds);
				if(!renderTarget->IsClipperCoverWholeTarget())
				{
					vint maxWidth=bounds.Width();
					Rect clipper=renderTarget->GetClipper();
					vint cx=bounds.Left();
					vint cy=bounds.Top();
					vint y1=clipper.Top()-bounds.Top();
					vint y2=y1+clipper.Height();
					vint y=0;

					lastMaxWidth=maxWidth;

					for(vint i=0;i<paragraphHeights.Count();i++)
					{
						vint paragraphHeight=paragraphHeights[i];
						if(y+paragraphHeight<=y1)
						{
							y+=paragraphHeight+paragraphDistance;
							continue;
						}
						else if(y>=y2)
						{
							break;
						}
						else
						{
							Ptr<DocumentParagraphRun> paragraph=element->document->paragraphs[i];
							Ptr<ParagraphCache> cache=paragraphCaches[i];
							bool created=cache && cache->graphicsParagraph;
							cache=EnsureAndGetCache(i, true);
							if(!created && i==lastCaret.row)
							{
								cache->graphicsParagraph->OpenCaret(lastCaret.column, lastCaretColor, lastCaretFrontSide);
							}

							paragraphHeight=cache->graphicsParagraph->GetHeight();
							cache->graphicsParagraph->Render(Rect(Point(cx, cy+y), Size(maxWidth, paragraphHeight)));
						}

						y+=paragraphHeight+paragraphDistance;
					}
				}
				renderTarget->PopClipper();
			}

			void GuiDocumentElement::GuiDocumentElementRenderer::OnElementStateChanged()
			{
				if(element->document && element->document->paragraphs.Count()>0)
				{
					vint defaultSize=GetCurrentController()->ResourceService()->GetDefaultFont().size;
					paragraphDistance=defaultSize;
					vint defaultHeight=defaultSize;

					paragraphCaches.Resize(element->document->paragraphs.Count());
					paragraphHeights.Resize(element->document->paragraphs.Count());

					for(vint i=0;i<paragraphHeights.Count();i++)
					{
						paragraphHeights[i]=defaultHeight;
					}
					cachedTotalHeight=paragraphHeights.Count()*defaultHeight+(paragraphHeights.Count()-1)*paragraphDistance;
					minSize=Size(0, cachedTotalHeight);
				}
				else
				{
					paragraphCaches.Resize(0);
					paragraphHeights.Resize(0);
					cachedTotalHeight=0;
					minSize=Size(0, 0);
				}
			}

			void GuiDocumentElement::GuiDocumentElementRenderer::NotifyParagraphUpdated(vint index)
			{
				if(0<=index && index<paragraphCaches.Count())
				{
					Ptr<ParagraphCache> cache=paragraphCaches[index];
					if(cache)
					{
						cache->graphicsParagraph=0;
					}
				}
			}

			vint GuiDocumentElement::GuiDocumentElementRenderer::GetHyperlinkIdFromPoint(Point point)
			{
				vint top=0;
				vint index=-1;
				if(GetParagraphIndexFromPoint(point, top, index))
				{
					Ptr<ParagraphCache> cache=paragraphCaches[index];
					if(cache && cache->graphicsParagraph)
					{
						vint start=0;
						vint length=0;
						vint id=0;
						if(cache->graphicsParagraph->HitTestPoint(Point(point.x, point.y-top), start, length, id))
						{
							return id;
						}
					}
					return DocumentRun::NullHyperlinkId;
				}
				return DocumentRun::NullHyperlinkId;
			}

			void GuiDocumentElement::GuiDocumentElementRenderer::OpenCaret(TextPos caret, Color color, bool frontSide)
			{
				CloseCaret();
				lastCaret=caret;
				lastCaretColor=color;
				lastCaretFrontSide=frontSide;

				Ptr<ParagraphCache> cache=paragraphCaches[lastCaret.row];
				if(cache && cache->graphicsParagraph)
				{
					cache->graphicsParagraph->OpenCaret(lastCaret.column, lastCaretColor, lastCaretFrontSide);
				}
			}

			void GuiDocumentElement::GuiDocumentElementRenderer::CloseCaret()
			{
				if(lastCaret!=TextPos(-1, -1))
				{
					if(0<=lastCaret.row && lastCaret.row<paragraphCaches.Count())
					{
						Ptr<ParagraphCache> cache=paragraphCaches[lastCaret.row];
						if(cache && cache->graphicsParagraph)
						{
							cache->graphicsParagraph->CloseCaret();
						}
					}
				}
			}

			void GuiDocumentElement::GuiDocumentElementRenderer::SetSelection(TextPos begin, TextPos end)
			{
				if(begin>end)
				{
					TextPos t=begin;
					begin=end;
					end=t;
				}
				if(begin==end)
				{
					begin=TextPos(-1, -1);
					end=TextPos(-1, -1);
				}

				for(vint i=0;i<paragraphCaches.Count();i++)
				{
					if(begin.row<=i && i<=end.row)
					{
						Ptr<ParagraphCache> cache=EnsureAndGetCache(i, false);
						vint newBegin=i==begin.row?begin.column:0;
						vint newEnd=i==end.row?end.column:cache->fullText.Length();

						if(cache->selectionBegin!=newBegin || cache->selectionEnd!=newEnd)
						{
							cache->selectionBegin=newBegin;
							cache->selectionEnd=newEnd;
							NotifyParagraphUpdated(i);
						}
					}
					else
					{
						Ptr<ParagraphCache> cache=paragraphCaches[i];
						if(cache)
						{
							if(cache->selectionBegin!=-1 || cache->selectionEnd!=-1)
							{
								cache->selectionBegin=-1;
								cache->selectionEnd=-1;
								NotifyParagraphUpdated(i);
							}
						}
					}
				}
			}

			TextPos GuiDocumentElement::GuiDocumentElementRenderer::CalculateCaret(TextPos comparingCaret, IGuiGraphicsParagraph::CaretRelativePosition position, bool& preferFrontSide)
			{
				Ptr<ParagraphCache> cache=EnsureAndGetCache(comparingCaret.row, true);
				if(cache)
				{
					switch(position)
					{
					case IGuiGraphicsParagraph::CaretFirst:
						{
							preferFrontSide=false;
							vint caret=cache->graphicsParagraph->GetCaret(0, IGuiGraphicsParagraph::CaretFirst, preferFrontSide);
							return TextPos(comparingCaret.row, caret);
						}
					case IGuiGraphicsParagraph::CaretLast:
						{
							preferFrontSide=true;
							vint caret=cache->graphicsParagraph->GetCaret(0, IGuiGraphicsParagraph::CaretLast, preferFrontSide);
							return TextPos(comparingCaret.row, caret);
						}
					case IGuiGraphicsParagraph::CaretLineFirst:
						{
							preferFrontSide=false;
							vint caret=cache->graphicsParagraph->GetCaret(comparingCaret.column, IGuiGraphicsParagraph::CaretLineFirst, preferFrontSide);
							return TextPos(comparingCaret.row, caret);
						}
					case IGuiGraphicsParagraph::CaretLineLast:
						{
							preferFrontSide=true;
							vint caret=cache->graphicsParagraph->GetCaret(comparingCaret.column, IGuiGraphicsParagraph::CaretLineLast, preferFrontSide);
							return TextPos(comparingCaret.row, caret);
						}
					case IGuiGraphicsParagraph::CaretMoveUp:
						{
							vint caret=cache->graphicsParagraph->GetCaret(comparingCaret.column, IGuiGraphicsParagraph::CaretMoveUp, preferFrontSide);
							if(caret==comparingCaret.column && comparingCaret.row>0)
							{
								Rect caretBounds=cache->graphicsParagraph->GetCaretBounds(comparingCaret.column, preferFrontSide);
								Ptr<ParagraphCache> anotherCache=EnsureAndGetCache(comparingCaret.row-1, true);
								vint height=anotherCache->graphicsParagraph->GetHeight();
								caret=anotherCache->graphicsParagraph->GetCaretFromPoint(Point(caretBounds.x1, height));
								return TextPos(comparingCaret.row-1, caret);
							}
							else
							{
								return TextPos(comparingCaret.row, caret);
							}
						}
					case IGuiGraphicsParagraph::CaretMoveDown:
						{
							vint caret=cache->graphicsParagraph->GetCaret(comparingCaret.column, IGuiGraphicsParagraph::CaretMoveDown, preferFrontSide);
							if(caret==comparingCaret.column && comparingCaret.row<paragraphCaches.Count()-1)
							{
								Rect caretBounds=cache->graphicsParagraph->GetCaretBounds(comparingCaret.column, preferFrontSide);
								Ptr<ParagraphCache> anotherCache=EnsureAndGetCache(comparingCaret.row+1, true);
								caret=anotherCache->graphicsParagraph->GetCaretFromPoint(Point(caretBounds.x1, 0));
								return TextPos(comparingCaret.row+1, caret);
							}
							else
							{
								return TextPos(comparingCaret.row, caret);
							}
						}
					case IGuiGraphicsParagraph::CaretMoveLeft:
						{
							preferFrontSide=false;
							vint caret=cache->graphicsParagraph->GetCaret(comparingCaret.column, IGuiGraphicsParagraph::CaretMoveLeft, preferFrontSide);
							if(caret==comparingCaret.column && comparingCaret.row>0)
							{
								Ptr<ParagraphCache> anotherCache=EnsureAndGetCache(comparingCaret.row-1, true);
								caret=anotherCache->graphicsParagraph->GetCaret(0, IGuiGraphicsParagraph::CaretLast, preferFrontSide);
								return TextPos(comparingCaret.row-1, caret);
							}
							else
							{
								return TextPos(comparingCaret.row, caret);
							}
						}
					case IGuiGraphicsParagraph::CaretMoveRight:
						{
							preferFrontSide=true;
							vint caret=cache->graphicsParagraph->GetCaret(comparingCaret.column, IGuiGraphicsParagraph::CaretMoveRight, preferFrontSide);
							if(caret==comparingCaret.column && comparingCaret.row<paragraphCaches.Count()-1)
							{
								Ptr<ParagraphCache> anotherCache=EnsureAndGetCache(comparingCaret.row+1, true);
								caret=anotherCache->graphicsParagraph->GetCaret(0, IGuiGraphicsParagraph::CaretFirst, preferFrontSide);
								return TextPos(comparingCaret.row+1, caret);
							}
							else
							{
								return TextPos(comparingCaret.row, caret);
							}
						}
					}
				}
				return comparingCaret;
			}

			TextPos GuiDocumentElement::GuiDocumentElementRenderer::CalculateCaretFromPoint(Point point)
			{
				vint top=0;
				vint index=-1;
				if(GetParagraphIndexFromPoint(point, top, index))
				{
					Ptr<ParagraphCache> cache=EnsureAndGetCache(index, true);
					Point paragraphPoint(point.x, point.y-top);
					vint caret=cache->graphicsParagraph->GetCaretFromPoint(paragraphPoint);
					return TextPos(index, caret);
				}
				return lastCaret;
			}

			Rect GuiDocumentElement::GuiDocumentElementRenderer::GetCaretBounds(TextPos caret, bool frontSide)
			{
				Ptr<ParagraphCache> cache=EnsureAndGetCache(caret.row, true);
				if(cache)
				{
					Rect bounds=cache->graphicsParagraph->GetCaretBounds(caret.column, frontSide);
					if(bounds!=Rect())
					{
						vint y=0;
						for(vint i=0;i<caret.row;i++)
						{
							y+=paragraphHeights[i]+paragraphDistance;
						}

						bounds.y1+=y;
						bounds.y2+=y;
						return bounds;
					}
				}
				return Rect();
			}

/***********************************************************************
GuiDocumentElement
***********************************************************************/

			void GuiDocumentElement::UpdateCaret()
			{
				Ptr<GuiDocumentElementRenderer> elementRenderer=renderer.Cast<GuiDocumentElementRenderer>();
				if(elementRenderer)
				{
					elementRenderer->SetSelection(caretBegin, caretEnd);
					if(caretVisible)
					{
						elementRenderer->OpenCaret(caretEnd, caretColor, caretFrontSide);
					}
					else
					{
						elementRenderer->CloseCaret();
					}
				}
			}

			GuiDocumentElement::GuiDocumentElement()
				:caretVisible(false)
				,caretFrontSide(false)
			{
			}

			GuiDocumentElement::~GuiDocumentElement()
			{
			}

			Ptr<DocumentModel> GuiDocumentElement::GetDocument()
			{
				return document;
			}

			void GuiDocumentElement::SetDocument(Ptr<DocumentModel> value)
			{
				document=value;
				if(renderer)
				{
					renderer->OnElementStateChanged();
					SetCaret(TextPos(), TextPos(), false);
				}
			}

			TextPos GuiDocumentElement::GetCaretBegin()
			{
				return caretBegin;
			}

			TextPos GuiDocumentElement::GetCaretEnd()
			{
				return caretEnd;
			}

			bool GuiDocumentElement::IsCaretEndPreferFrontSide()
			{
				return caretFrontSide;
			}

			void GuiDocumentElement::SetCaret(TextPos begin, TextPos end, bool frontSide)
			{
				caretBegin=begin;
				caretEnd=end;
				if(caretBegin<caretEnd)
				{
					caretFrontSide=true;
				}
				else if(caretBegin>caretEnd)
				{
					caretFrontSide=false;
				}
				else
				{
					caretFrontSide=frontSide;
				}
				UpdateCaret();
			}

			bool GuiDocumentElement::GetCaretVisible()
			{
				return caretVisible;
			}

			void GuiDocumentElement::SetCaretVisible(bool value)
			{
				caretVisible=value;
				UpdateCaret();
			}

			Color GuiDocumentElement::GetCaretColor()
			{
				return caretColor;
			}

			void GuiDocumentElement::SetCaretColor(Color value)
			{
				caretColor=value;
				UpdateCaret();
			}

			TextPos GuiDocumentElement::CalculateCaret(TextPos comparingCaret, IGuiGraphicsParagraph::CaretRelativePosition position, bool& preferFrontSide)
			{
				Ptr<GuiDocumentElementRenderer> elementRenderer=renderer.Cast<GuiDocumentElementRenderer>();
				if(elementRenderer)
				{
					TextPos caret=elementRenderer->CalculateCaret(comparingCaret, position, preferFrontSide);
					return caret.column==-1?comparingCaret:caret;
				}
				else
				{
					return comparingCaret;
				}
			}

			TextPos GuiDocumentElement::CalculateCaretFromPoint(Point point)
			{
				Ptr<GuiDocumentElementRenderer> elementRenderer=renderer.Cast<GuiDocumentElementRenderer>();
				if(elementRenderer)
				{
					return elementRenderer->CalculateCaretFromPoint(point);
				}
				else
				{
					return TextPos(0, 0);
				}
			}

			Rect GuiDocumentElement::GetCaretBounds(TextPos caret, bool frontSide)
			{
				Ptr<GuiDocumentElementRenderer> elementRenderer=renderer.Cast<GuiDocumentElementRenderer>();
				if(elementRenderer)
				{
					return elementRenderer->GetCaretBounds(caret, frontSide);
				}
				else
				{
					return Rect();
				}
			}
			
			void GuiDocumentElement::NotifyParagraphUpdated(vint index)
			{
				Ptr<GuiDocumentElementRenderer> elementRenderer=renderer.Cast<GuiDocumentElementRenderer>();
				if(elementRenderer)
				{
					elementRenderer->NotifyParagraphUpdated(index);
				}
			}

			vint GuiDocumentElement::GetHyperlinkIdFromPoint(Point point)
			{
				Ptr<GuiDocumentElementRenderer> elementRenderer=renderer.Cast<GuiDocumentElementRenderer>();
				if(elementRenderer)
				{
					return elementRenderer->GetHyperlinkIdFromPoint(point);
				}
				else
				{
					return DocumentRun::NullHyperlinkId;
				}
			}

			void GuiDocumentElement::ActivateHyperlink(vint hyperlinkId, bool active)
			{
				if(document)
				{
					vint paragraphIndex=document->ActivateHyperlink(hyperlinkId, active);
					if(paragraphIndex!=-1)
					{
						NotifyParagraphUpdated(paragraphIndex);
					}
				}
			}
		}
	}
}