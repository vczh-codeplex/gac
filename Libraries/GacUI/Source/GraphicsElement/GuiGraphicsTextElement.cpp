#include "GuiGraphicsTextElement.h"
#include "..\..\..\..\Common\Source\Stream\MemoryStream.h"
#include "..\..\..\..\Common\Source\Stream\Accessor.h"

namespace vl
{
	using namespace collections;
	using namespace parsing::xml;

	namespace presentation
	{
		namespace elements
		{
			namespace text
			{

/***********************************************************************
text::TextLine
***********************************************************************/

				TextLine::TextLine()
					:text(0)
					,att(0)
					,availableOffsetCount(0)
					,bufferLength(0)
					,dataLength(0)
					,lexerFinalState(-1)
					,contextFinalState(-1)
				{
				}

				TextLine::~TextLine()
				{
				}

				vint TextLine::CalculateBufferLength(vint dataLength)
				{
					if(dataLength<1)dataLength=1;
					vint bufferLength=dataLength-dataLength%BlockSize;
					if(bufferLength<dataLength)
					{
						bufferLength+=BlockSize;
					}
					return bufferLength;
				}

				void TextLine::Initialize()
				{
					Finalize();
					text=new wchar_t[BlockSize];
					att=new CharAtt[BlockSize];
					bufferLength=BlockSize;

					memset(text, 0, sizeof(wchar_t)*bufferLength);
					memset(att, 0, sizeof(CharAtt)*bufferLength);
				}

				void TextLine::Finalize()
				{
					if(text)
					{
						delete[] text;
						text=0;
					}
					if(att)
					{
						delete[] att;
						att=0;
					}
					availableOffsetCount=0;
					bufferLength=0;
					dataLength=0;
				}

				bool TextLine::IsReady()
				{
					return text && att;
				}

				bool TextLine::Modify(vint start, vint count, const wchar_t* input, vint inputCount)
				{
					if(!text || !att || start<0 || count<0 || start+count>dataLength || inputCount<0) return false;

					vint newDataLength=dataLength-count+inputCount;
					vint newBufferLength=CalculateBufferLength(newDataLength);
					if(newBufferLength!=bufferLength)
					{
						wchar_t* newText=new wchar_t[newBufferLength];
						memcpy(newText, text, start*sizeof(wchar_t));
						memcpy(newText+start, input, inputCount*sizeof(wchar_t));
						memcpy(newText+start+inputCount, text+start+count, (dataLength-start-count)*sizeof(wchar_t));

						CharAtt* newAtt=new CharAtt[newBufferLength];
						memcpy(newAtt, att, start*sizeof(CharAtt));
						memset(newAtt+start, 0, inputCount*sizeof(CharAtt));
						memcpy(newAtt+start+inputCount, att+start+count, (dataLength-start-count)*sizeof(CharAtt));

						delete[] text;
						delete[] att;
						text=newText;
						att=newAtt;
					}
					else
					{
						memmove(text+start+inputCount, text+start+count, (dataLength-start-count)*sizeof(wchar_t));
						memmove(att+start+inputCount, att+start+count, (dataLength-start-count)*sizeof(CharAtt));
						memcpy(text+start, input, inputCount*sizeof(wchar_t));
						memset(att+start, 0, inputCount*sizeof(CharAtt));
					}
					dataLength=newDataLength;
					bufferLength=newBufferLength;
					if(availableOffsetCount>start)
					{
						availableOffsetCount=start;
					}

					return true;
				}

				TextLine TextLine::Split(vint index)
				{
					if(index<0 || index>dataLength) return TextLine();
					vint count=dataLength-index;
					TextLine line;
					line.Initialize();
					line.Modify(0, 0, text+index, count);
					memcpy(line.att, att+index, count*sizeof(CharAtt));
					Modify(index, count, L"", 0);
					return line;
				}

				void TextLine::AppendAndFinalize(TextLine& line)
				{
					vint oldDataLength=dataLength;
					Modify(oldDataLength, 0, line.text, line.dataLength);
					memcpy(att+oldDataLength, line.att, line.dataLength*sizeof(CharAtt));
					line.Finalize();
				}

/***********************************************************************
text::CharMeasurer
***********************************************************************/

				CharMeasurer::CharMeasurer(vint _rowHeight)
					:oldRenderTarget(0)
					,rowHeight(_rowHeight)
				{
					memset(widths, 0, sizeof(widths));
				}

				CharMeasurer::~CharMeasurer()
				{
				}

				void CharMeasurer::SetRenderTarget(IGuiGraphicsRenderTarget* value)
				{
					if(oldRenderTarget!=value)
					{
						oldRenderTarget=value;
						rowHeight=GetRowHeightInternal(oldRenderTarget);
						memset(widths, 0, sizeof(widths));
					}
				}

				vint CharMeasurer::MeasureWidth(wchar_t character)
				{
					vint w=widths[character];
					if(w==0)
					{
						widths[character]=w=MeasureWidthInternal(character, oldRenderTarget);
					}
					return w;
				}

				vint CharMeasurer::GetRowHeight()
				{
					return rowHeight;
				}

/***********************************************************************
text::TextLines
***********************************************************************/

				TextLines::TextLines()
					:charMeasurer(0)
					,renderTarget(0)
					,tabWidth(1)
					,tabSpaceCount(4)
					,passwordChar(L'\0')
				{
					TextLine line;
					line.Initialize();
					lines.Add(line);
				}

				TextLines::~TextLines()
				{
					RemoveLines(0, lines.Count());
				}

				//--------------------------------------------------------

				vint TextLines::GetCount()
				{
					return lines.Count();
				}

				TextLine& TextLines::GetLine(vint row)
				{
					return lines[row];
				}

				CharMeasurer* TextLines::GetCharMeasurer()
				{
					return charMeasurer;
				}

				void TextLines::SetCharMeasurer(CharMeasurer* value)
				{
					charMeasurer=value;
					if(charMeasurer) charMeasurer->SetRenderTarget(renderTarget);
					ClearMeasurement();
				}

				IGuiGraphicsRenderTarget* TextLines::GetRenderTarget()
				{
					return renderTarget;
				}

				void TextLines::SetRenderTarget(IGuiGraphicsRenderTarget* value)
				{
					renderTarget=value;
					if(charMeasurer) charMeasurer->SetRenderTarget(renderTarget);
					ClearMeasurement();
				}

				WString TextLines::GetText(TextPos start, TextPos end)
				{
					if(!IsAvailable(start) || !IsAvailable(end) || start>end) return L"";

					if(start.row==end.row)
					{
						return WString(lines[start.row].text+start.column, end.column-start.column);
					}

					vint count=0;
					for(vint i=start.row+1;i<end.row;i++)
					{
						count+=lines[i].dataLength;
					}
					count+=lines[start.row].dataLength-start.column;
					count+=end.column;

					Array<wchar_t> buffer;
					buffer.Resize(count+(end.row-start.row)*2);
					wchar_t* writing=&buffer[0];

					for(vint i=start.row;i<=end.row;i++)
					{
						wchar_t* text=lines[i].text;
						vint chars=0;
						if(i==start.row)
						{
							text+=start.column;
							chars=lines[i].dataLength-start.column;
						}
						else if(i==end.row)
						{
							chars=end.column;
						}
						else
						{
							chars=lines[i].dataLength;
						}

						if(i!=start.row)
						{
							*writing++=L'\r';
							*writing++=L'\n';
						}
						memcpy(writing, text, chars*sizeof(wchar_t));
						writing+=chars;
					}
					return WString(&buffer[0], buffer.Count());
				}

				WString TextLines::GetText()
				{
					return GetText(TextPos(0, 0), TextPos(lines.Count()-1, lines[lines.Count()-1].dataLength));
				}

				void TextLines::SetText(const WString& value)
				{
					Modify(TextPos(0, 0), TextPos(lines.Count()-1, lines[lines.Count()-1].dataLength), value);
				}

				//--------------------------------------------------------

				bool TextLines::RemoveLines(vint start, vint count)
				{
					if(start<0 || count<0 || start+count>lines.Count()) return false;
					for(vint i=start+count-1;i>=start;i--)
					{
						lines[i].Finalize();
					}
					lines.RemoveRange(start, count);
					return true;
				}

				bool TextLines::IsAvailable(TextPos pos)
				{
					return 0<=pos.row && pos.row<lines.Count() && 0<=pos.column && pos.column<=lines[pos.row].dataLength;
				}

				TextPos TextLines::Normalize(TextPos pos)
				{
					if(pos.row<0)
					{
						return TextPos(0, 0);
					}
					else if(pos.row>=lines.Count())
					{
						return TextPos(lines.Count()-1, lines[lines.Count()-1].dataLength);
					}
					else
					{
						TextLine& line=lines[pos.row];
						if(pos.column<0)
						{
							return TextPos(pos.row, 0);
						}
						else if(pos.column>line.dataLength)
						{
							return TextPos(pos.row, line.dataLength);
						}
						else
						{
							return pos;
						}
					}
				}

				TextPos TextLines::Modify(TextPos start, TextPos end, const wchar_t** inputs, vint* inputCounts, vint rows)
				{
					if(!IsAvailable(start) || !IsAvailable(end) || start>end) return TextPos(-1, -1);

					if(rows==1)
					{
						if(start.row==end.row)
						{
							lines[start.row].Modify(start.column, end.column-start.column, inputs[0], inputCounts[0]);
						}
						else
						{
							if(end.row-start.row>1)
							{
								RemoveLines(start.row+1, end.row-start.row-1);
							}
							vint modifyCount=lines[start.row].dataLength-start.column+end.column;
							lines[start.row].AppendAndFinalize(lines[start.row+1]);
							lines.RemoveAt(start.row+1);
							lines[start.row].Modify(start.column, modifyCount, inputs[0], inputCounts[0]);
						}
						return TextPos(start.row, start.column+inputCounts[0]);
					}

					if(start.row==end.row)
					{
						TextLine newLine=lines[start.row].Split(end.column);
						lines.Insert(start.row+1, newLine);
						end=TextPos(start.row+1, 0);
					}

					vint oldMiddleLines=end.row-start.row-1;
					vint newMiddleLines=rows-2;
					if(oldMiddleLines<newMiddleLines)
					{
						for(vint i=oldMiddleLines;i<newMiddleLines;i++)
						{
							TextLine line;
							line.Initialize();
							lines.Insert(end.row, line);
						}
					}
					else if(oldMiddleLines>newMiddleLines)
					{
						RemoveLines(start.row+newMiddleLines+1, oldMiddleLines-newMiddleLines);
					}
					end.row+=newMiddleLines-oldMiddleLines;

					lines[start.row].Modify(start.column, lines[start.row].dataLength-start.column, inputs[0], inputCounts[0]);
					lines[end.row].Modify(0, end.column, inputs[rows-1], inputCounts[rows-1]);
					for(vint i=1;i<rows-1;i++)
					{
						lines[start.row+i].Modify(0, lines[start.row+i].dataLength, inputs[i], inputCounts[i]);
					}
					return TextPos(end.row, inputCounts[rows-1]);
				}

				TextPos TextLines::Modify(TextPos start, TextPos end, const wchar_t* input, vint inputCount)
				{
					List<const wchar_t*> inputs;
					List<vint> inputCounts;
					const wchar_t* previous=input;
					const wchar_t* current=input;

					while(true)
					{
						if(current==input+inputCount)
						{
							inputs.Add(previous);
							inputCounts.Add(current-previous);
							break;
						}
						else if(*current==L'\r' || *current==L'\n')
						{
							inputs.Add(previous);
							inputCounts.Add(current-previous);
							previous=current+(current[1]==L'\n'?2:1);
							current=previous;
						}
						else
						{
							current++;
						}
					}

					return Modify(start, end, &inputs[0], &inputCounts[0], inputs.Count());
				}

				TextPos TextLines::Modify(TextPos start, TextPos end, const wchar_t* input)
				{
					return Modify(start, end, input, wcslen(input));
				}

				TextPos TextLines::Modify(TextPos start, TextPos end, const WString& input)
				{
					return Modify(start, end, input.Buffer(), input.Length());
				}

				void TextLines::Clear()
				{
					RemoveLines(0, lines.Count());
					TextLine line;
					line.Initialize();
					lines.Add(line);
				}

				//--------------------------------------------------------

				void TextLines::ClearMeasurement()
				{
					for(vint i=0;i<lines.Count();i++)
					{
						lines[i].availableOffsetCount=0;
					}
					if(charMeasurer)
					{
						tabWidth=tabSpaceCount*charMeasurer->MeasureWidth(L' ');
					}
					if(tabWidth==0)
					{
						tabWidth=1;
					}
				}

				vint TextLines::GetTabSpaceCount()
				{
					return tabSpaceCount;
				}

				void TextLines::SetTabSpaceCount(vint value)
				{
					if(value<1) value=1;
					if(tabSpaceCount!=value)
					{
						tabSpaceCount=value;
						ClearMeasurement();
					}
				}

				void TextLines::MeasureRow(vint row)
				{
					TextLine& line=lines[row];
					vint offset=0;
					if(line.availableOffsetCount)
					{
						offset=line.att[line.availableOffsetCount-1].rightOffset;
					}
					for(vint i=line.availableOffsetCount;i<line.dataLength;i++)
					{
						CharAtt& att=line.att[i];
						wchar_t c=line.text[i];
						vint width=0;
						if(passwordChar)
						{
							width=charMeasurer->MeasureWidth(passwordChar);
						}
						else if(c==L'\t')
						{
							width=tabWidth-offset%tabWidth;
						}
						else
						{
							width=charMeasurer->MeasureWidth(line.text[i]);
						}
						offset+=width;
						att.rightOffset=(int)offset;
					}
					line.availableOffsetCount=line.dataLength;
				}

				vint TextLines::GetRowWidth(vint row)
				{
					if(row<0 || row>=lines.Count()) return -1;
					TextLine& line=lines[row];
					if(line.dataLength==0)
					{
						return 0;
					}
					else
					{
						MeasureRow(row);
						return line.att[line.dataLength-1].rightOffset;
					}
				}

				vint TextLines::GetRowHeight()
				{
					return charMeasurer->GetRowHeight();
				}

				vint TextLines::GetMaxWidth()
				{
					vint width=0;
					for(vint i=0;i<lines.Count();i++)
					{
						vint rowWidth=GetRowWidth(i);
						if(width<rowWidth)
						{
							width=rowWidth;
						}
					}
					return width;
				}

				vint TextLines::GetMaxHeight()
				{
					return lines.Count()*charMeasurer->GetRowHeight();
				}

				TextPos TextLines::GetTextPosFromPoint(Point point)
				{
					vint h=charMeasurer->GetRowHeight();
					if(point.y<0)
					{
						point.y=0;
					}
					else if(point.y>=h*lines.Count())
					{
						point.y=h*lines.Count()-1;
					}

					vint row=point.y/h;
					if(point.x<0)
					{
						return TextPos(row, 0);
					}
					else if(point.x>=GetRowWidth(row))
					{
						return TextPos(row, lines[row].dataLength);
					}
					TextLine& line=lines[row];

					vint i1=0, i2=line.dataLength;
					vint p1=0, p2=line.att[line.dataLength-1].rightOffset;
					while(i2-i1>1)
					{
						vint i=(i1+i2)/2;
						vint p=i==0?0:line.att[i-1].rightOffset;
						if(point.x<p)
						{
							i2=i;
							p2=p;
						}
						else
						{
							i1=i;
							p1=p;
						}
					}
					return TextPos(row, i1);
				}

				Point TextLines::GetPointFromTextPos(TextPos pos)
				{
					if(IsAvailable(pos))
					{
						vint y=pos.row*charMeasurer->GetRowHeight();
						if(pos.column==0)
						{
							return Point(0, y);
						}
						else
						{
							MeasureRow(pos.row);
							TextLine& line=lines[pos.row];
							return Point(line.att[pos.column-1].rightOffset, y);
						}
					}
					else
					{
						return Point(-1, -1);
					}
				}

				Rect TextLines::GetRectFromTextPos(TextPos pos)
				{
					Point point=GetPointFromTextPos(pos);
					if(point==Point(-1, -1))
					{
						return Rect(-1, -1, -1, -1);
					}
					else
					{
						vint h=charMeasurer->GetRowHeight();
						TextLine& line=lines[pos.row];
						if(pos.column==line.dataLength)
						{
							return Rect(point, Size(h/2, h));
						}
						else
						{
							return Rect(point, Size(line.att[pos.column].rightOffset-point.x, h));
						}
					}
				}

				//--------------------------------------------------------

				wchar_t TextLines::GetPasswordChar()
				{
					return passwordChar;
				}

				void TextLines::SetPasswordChar(wchar_t value)
				{
					passwordChar=value;
					ClearMeasurement();
				}
			}

			using namespace text;

/***********************************************************************
GuiColorizedTextElement
***********************************************************************/

			GuiColorizedTextElement::GuiColorizedTextElement()
				:callback(0)
				,isVisuallyEnabled(true)
				,isFocused(false)
				,caretVisible(false)
			{
			}

			GuiColorizedTextElement::~GuiColorizedTextElement()
			{
				renderer->Finalize();
			}

			text::TextLines& GuiColorizedTextElement::GetLines()
			{
				return lines;
			}

			GuiColorizedTextElement::ICallback* GuiColorizedTextElement::GetCallback()
			{
				return callback;
			}

			void GuiColorizedTextElement::SetCallback(ICallback* value)
			{
				callback=value;
				if(!callback)
				{
					lines.SetCharMeasurer(0);
				}
			}

			const GuiColorizedTextElement::ColorArray& GuiColorizedTextElement::GetColors()
			{
				return colors;
			}

			void GuiColorizedTextElement::SetColors(const ColorArray& value)
			{
				CopyFrom(colors, value);
				if(callback) callback->ColorChanged();
				if(renderer)
				{
					renderer->OnElementStateChanged();
				}
			}

			const FontProperties& GuiColorizedTextElement::GetFont()
			{
				return font;
			}

			void GuiColorizedTextElement::SetFont(const FontProperties& value)
			{
				if(font!=value)
				{
					font=value;
					if(callback)
					{
						callback->FontChanged();
					}
					if(renderer)
					{
						renderer->OnElementStateChanged();
					}
				}
			}

			wchar_t GuiColorizedTextElement::GetPasswordChar()
			{
				return lines.GetPasswordChar();
			}

			void GuiColorizedTextElement::SetPasswordChar(wchar_t value)
			{
				if(lines.GetPasswordChar()!=value)
				{
					lines.SetPasswordChar(value);
					if(renderer)
					{
						renderer->OnElementStateChanged();
					}
				}
			}

			Point GuiColorizedTextElement::GetViewPosition()
			{
				return viewPosition;
			}

			void GuiColorizedTextElement::SetViewPosition(Point value)
			{
				if(viewPosition!=value)
				{
					viewPosition=value;
					if(renderer)
					{
						renderer->OnElementStateChanged();
					}
				}
			}

			bool GuiColorizedTextElement::GetVisuallyEnabled()
			{
				return isVisuallyEnabled;
			}

			void GuiColorizedTextElement::SetVisuallyEnabled(bool value)
			{
				if(isVisuallyEnabled!=value)
				{
					isVisuallyEnabled=value;
					if(renderer)
					{
						renderer->OnElementStateChanged();
					}
				}
			}

			bool GuiColorizedTextElement::GetFocused()
			{
				return isFocused;
			}

			void GuiColorizedTextElement::SetFocused(bool value)
			{
				if(isFocused!=value)
				{
					isFocused=value;
					if(renderer)
					{
						renderer->OnElementStateChanged();
					}
				}
			}

			TextPos GuiColorizedTextElement::GetCaretBegin()
			{
				return caretBegin;
			}

			void GuiColorizedTextElement::SetCaretBegin(TextPos value)
			{
				caretBegin=value;
			}

			TextPos GuiColorizedTextElement::GetCaretEnd()
			{
				return caretEnd;
			}

			void GuiColorizedTextElement::SetCaretEnd(TextPos value)
			{
				caretEnd=value;
			}

			bool GuiColorizedTextElement::GetCaretVisible()
			{
				return caretVisible;
			}

			void GuiColorizedTextElement::SetCaretVisible(bool value)
			{
				caretVisible=value;
			}

			Color GuiColorizedTextElement::GetCaretColor()
			{
				return caretColor;
			}

			void GuiColorizedTextElement::SetCaretColor(Color value)
			{
				if(caretColor!=value)
				{
					caretColor=value;
					if(renderer)
					{
						renderer->OnElementStateChanged();
					}
				}
			}

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
					typedef collections::Pair<FontProperties, Color>			RawStylePair;
				public:
					vint						start;
					vint						length;
					vint						hyperlinkId;
					List<RawStylePair>			styles;
					DocumentModel*				model;
					IGuiGraphicsParagraph*		paragraph;

					SetPropertiesVisitor(DocumentModel* _model, IGuiGraphicsParagraph* _paragraph)
						:start(0)
						,length(0)
						,hyperlinkId(DocumentRun::NullHyperlinkId)
						,model(_model)
						,paragraph(_paragraph)
					{
						RawStylePair style;
						style=model->GetStyle(L"#Default", style);
						styles.Add(style);
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
						length=run->text.Length();
						if(length>0)
						{
							RawStylePair style=styles[styles.Count()-1];
							paragraph->SetFont(start, length, style.key.fontFamily);
							paragraph->SetSize(start, length, style.key.size);
							paragraph->SetColor(start, length, style.value);
							paragraph->SetStyle(start, length, 
								(IGuiGraphicsParagraph::TextStyle)
								( (style.key.bold?IGuiGraphicsParagraph::Bold:0)
								| (style.key.italic?IGuiGraphicsParagraph::Italic:0)
								| (style.key.underline?IGuiGraphicsParagraph::Underline:0)
								| (style.key.strikeline?IGuiGraphicsParagraph::Strikeline:0)
								));
							if(hyperlinkId!=DocumentRun::NullHyperlinkId)
							{
								paragraph->SetInteractionId(start, length, hyperlinkId);
							}
						}
						start+=length;
					}

					void Visit(DocumentStylePropertiesRun* run)override
					{
						RawStylePair style=styles[styles.Count()-1];
						style=model->GetStyle(run->style, style);
						styles.Add(style);
						VisitContainer(run);
						styles.RemoveAt(styles.Count()-1);
					}

					void Visit(DocumentStyleApplicationRun* run)override
					{
						RawStylePair style=styles[styles.Count()-1];
						style=model->GetStyle(run->styleName, style);
						styles.Add(style);
						VisitContainer(run);
						styles.RemoveAt(styles.Count()-1);
					}

					void Visit(DocumentHyperlinkTextRun* run)override
					{
						RawStylePair style=styles[styles.Count()-1];
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

					static vint SetProperty(DocumentModel* model, IGuiGraphicsParagraph* paragraph, Ptr<DocumentParagraphRun> run)
					{
						SetPropertiesVisitor visitor(model, paragraph);
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

			GuiDocumentElement::GuiDocumentElementRenderer::GuiDocumentElementRenderer()
				:paragraphDistance(0)
				,lastMaxWidth(-1)
				,cachedTotalHeight(0)
				,layoutProvider(GetGuiGraphicsResourceManager()->GetLayoutProvider())
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
							if(!cache)
							{
								cache=new ParagraphCache;
								cache->fullText=ExtractTextVisitor::ExtractText(paragraph);
								paragraphCaches[i]=cache;
							}

							if(!cache->graphicsParagraph)
							{
								cache->graphicsParagraph=layoutProvider->CreateParagraph(cache->fullText, renderTarget);
								cache->graphicsParagraph->SetParagraphAlignment(paragraph->alignment);
								SetPropertiesVisitor::SetProperty(element->document.Obj(), cache->graphicsParagraph.Obj(), paragraph);
							}
							if(cache->graphicsParagraph->GetMaxWidth()!=maxWidth)
							{
								cache->graphicsParagraph->SetMaxWidth(maxWidth);
								vint height=cache->graphicsParagraph->GetHeight();
								if(paragraphHeight!=height)
								{
									cachedTotalHeight+=height-paragraphHeight;
									paragraphHeight=height;
									paragraphHeights[i]=paragraphHeight;
								}
							}

							cache->graphicsParagraph->Render(Rect(Point(cx, cy+y), Size(maxWidth, paragraphHeight)));
						}

						y+=paragraphHeight+paragraphDistance;
					}

					lastMaxWidth=maxWidth;
					minSize=Size(0, cachedTotalHeight);
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
						Ptr<ParagraphCache> cache=paragraphCaches[i];
						if(cache && cache->graphicsParagraph)
						{
							vint start=0;
							vint length=0;
							vint id=0;
							if(cache->graphicsParagraph->HitTestPoint(Point(point.x, point.y-y), start, length, id))
							{
								return id;
							}
						}
						return DocumentRun::NullHyperlinkId;
					}
				}
				return DocumentRun::NullHyperlinkId;
			}

/***********************************************************************
GuiDocumentElement
***********************************************************************/

			GuiDocumentElement::GuiDocumentElement()
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