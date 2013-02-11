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
					WString						text;

					void Visit(DocumentTextRun* run)override
					{
						text=run->text;
					}

					void Visit(DocumentImageRun* run)override
					{
						text=L"[Image]";
					}

					static WString ExtractText(DocumentRun* run)
					{
						ExtractTextVisitor visitor;
						run->Accept(&visitor);
						return visitor.text;
					}
				};

				class SetPropertiesVisitor : public Object, public DocumentRun::IVisitor
				{
				public:
					vint							start;
					vint							length;
					IGuiGraphicsParagraph*		paragraph;

					SetPropertiesVisitor(vint _start, IGuiGraphicsParagraph* _paragraph)
						:start(_start)
						,length(0)
						,paragraph(_paragraph)
					{
					}

					void Visit(DocumentTextRun* run)override
					{
						length=run->text.Length();
						if(length>0)
						{
							paragraph->SetFont(start, length, run->style.fontFamily);
							paragraph->SetSize(start, length, run->style.size);
							paragraph->SetColor(start, length, run->color);
							paragraph->SetStyle(start, length, 
								(IGuiGraphicsParagraph::TextStyle)
								( (run->style.bold?IGuiGraphicsParagraph::Bold:0)
								| (run->style.italic?IGuiGraphicsParagraph::Italic:0)
								| (run->style.underline?IGuiGraphicsParagraph::Underline:0)
								| (run->style.strikeline?IGuiGraphicsParagraph::Strikeline:0)
								));
							start+=length;
						}
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
					}

					static vint SetProperty(vint start, IGuiGraphicsParagraph* paragraph, DocumentRun* run)
					{
						SetPropertiesVisitor visitor(start, paragraph);
						run->Accept(&visitor);
						return visitor.length;
					}
				};

				class SerializeRunVisitor : public Object, public DocumentRun::IVisitor
				{
				protected:

					const XmlElementWriter& Text(DocumentTextRun* run, const XmlElementWriter& writer)
					{
						return writer.Text(run->text);
					}

					const XmlElementWriter& Antialias(DocumentTextRun* run, const XmlElementWriter& writer)
					{
						if(!run->style.antialias)
						{
							return Text(run, writer.Element(L"na")).End();
						}
						else if(!run->style.verticalAntialias)
						{
							return Text(run, writer);
						}
						else
						{
							return Text(run, writer.Element(L"va")).End();
						}
					}

					const XmlElementWriter& Strikeline(DocumentTextRun* run, const XmlElementWriter& writer)
					{
						if(run->style.strikeline)
						{
							return Antialias(run, writer.Element(L"s")).End();
						}
						else
						{
							return Antialias(run, writer);
						}
					}

					const XmlElementWriter& Underline(DocumentTextRun* run, const XmlElementWriter& writer)
					{
						if(run->style.underline)
						{
							return Strikeline(run, writer.Element(L"u")).End();
						}
						else
						{
							return Strikeline(run, writer);
						}
					}

					const XmlElementWriter& Italic(DocumentTextRun* run, const XmlElementWriter& writer)
					{
						if(run->style.italic)
						{
							return Underline(run, writer.Element(L"i")).End();
						}
						else
						{
							return Underline(run, writer);
						}
					}

					const XmlElementWriter& Bold(DocumentTextRun* run, const XmlElementWriter& writer)
					{
						if(run->style.bold)
						{
							return Italic(run, writer.Element(L"b")).End();
						}
						else
						{
							return Italic(run, writer);
						}
					}

					const XmlElementWriter& Font(DocumentTextRun* run, const XmlElementWriter& writer)
					{
						const XmlElementWriter& font=writer.Element(L"font");
						font.Attribute(L"face", run->style.fontFamily);
						font.Attribute(L"size", itow(run->style.size));
						font.Attribute(L"color", run->color.ToString());
						Bold(run, font);
						return writer;
					}
				public:
					Ptr<XmlElement> container;

					void Visit(DocumentTextRun* run)override
					{
						if(run->text!=L"")
						{
							XmlElementWriter writer(container);
							Font(run, writer);
						}
					}

					void Visit(DocumentImageRun* run)override
					{
						XmlElementWriter writer(container);
						writer
							.Element(L"img")
							.Attribute(L"width", itow(run->size.x))
							.Attribute(L"height", itow(run->size.y))
							.Attribute(L"baseline", itow(run->baseline))
							.Attribute(L"frameIndex", itow(run->frameIndex))
							.Attribute(L"source", run->source)
							;
					}
				};

				class DeserializeNodeVisitor : public XmlNode::IVisitor
				{
				public:
					List<Pair<FontProperties, Color>>	styleStack;
					Ptr<DocumentParagraph>				paragraph;
					Ptr<DocumentLine>					line;
					WString								workingDirectory;

					DeserializeNodeVisitor(Ptr<DocumentParagraph> _paragraph, const WString& _workingDirectory)
						:paragraph(_paragraph)
						,workingDirectory(_workingDirectory)
					{
						styleStack.Add(Pair<FontProperties, Color>(GetCurrentController()->ResourceService()->GetDefaultFont(), Color()));
					}

					void PrintText(const WString& text)
					{
						if(!line)
						{
							line=new DocumentLine;
							paragraph->lines.Add(line);
						}
						Ptr<DocumentTextRun> run=new DocumentTextRun;
						run->style=styleStack[styleStack.Count()-1].key;
						run->color=styleStack[styleStack.Count()-1].value;
						run->text=text;
						line->runs.Add(run);
					}

					void Visit(XmlText* node)override
					{
						PrintText(node->content.value);
					}

					void Visit(XmlCData* node)override
					{
						PrintText(node->content.value);
					}

					void Visit(XmlAttribute* node)override
					{
					}

					void Visit(XmlComment* node)override
					{
					}

					void Visit(XmlElement* node)override
					{
						if(node->name.value==L"br")
						{
							if(!line) PrintText(L"");
							line=0;
						}
						else if(node->name.value==L"img")
						{
							if(!line)
							{
								line=new DocumentLine;
								paragraph->lines.Add(line);
							}
							Ptr<DocumentImageRun> run=new DocumentImageRun;
							FOREACH(Ptr<XmlAttribute>, att, node->attributes)
							{
								if(att->name.value==L"width")
								{
									run->size.x=wtoi(att->value.value);
								}
								else if(att->name.value==L"height")
								{
									run->size.y=wtoi(att->value.value);
								}
								else if(att->name.value==L"baseline")
								{
									run->baseline=wtoi(att->value.value);
								}
								else if(att->name.value==L"frameIndex")
								{
									run->frameIndex=wtoi(att->value.value);
								}
								else if(att->name.value==L"source")
								{
									run->source=att->value.value;
									if(Locale::Invariant().StartsWith(run->source, L"file://", Locale::IgnoreCase))
									{
										WString filename=run->source.Sub(7, run->source.Length()-7);
										if(filename.Length()>=2 && filename[1]!=L':')
										{
											filename=workingDirectory+filename;
										}
										run->image=GetCurrentController()->ImageService()->CreateImageFromFile(filename);
									}
								}
							}
							line->runs.Add(run);
						}
						else if(node->name.value==L"font")
						{
							auto style=styleStack[styleStack.Count()-1];
							FOREACH(Ptr<XmlAttribute>, att, node->attributes)
							{
								if(att->name.value==L"face")
								{
									style.key.fontFamily=att->value.value;
								}
								else if(att->name.value==L"size")
								{
									style.key.size=wtoi(att->value.value);
								}
								else if(att->name.value==L"color")
								{
									style.value=Color::Parse(att->value.value);
								}
							}
							styleStack.Add(style);
							FOREACH(Ptr<XmlNode>, sub, node->subNodes)
							{
								sub->Accept(this);
							}
							styleStack.RemoveAt(styleStack.Count()-1);
						}
						else if(node->name.value==L"b")
						{
							auto style=styleStack[styleStack.Count()-1];
							style.key.bold=true;
							styleStack.Add(style);
							FOREACH(Ptr<XmlNode>, sub, node->subNodes)
							{
								sub->Accept(this);
							}
							styleStack.RemoveAt(styleStack.Count()-1);
						}
						else if(node->name.value==L"i")
						{
							auto style=styleStack[styleStack.Count()-1];
							style.key.italic=true;
							styleStack.Add(style);
							FOREACH(Ptr<XmlNode>, sub, node->subNodes)
							{
								sub->Accept(this);
							}
							styleStack.RemoveAt(styleStack.Count()-1);
						}
						else if(node->name.value==L"u")
						{
							auto style=styleStack[styleStack.Count()-1];
							style.key.underline=true;
							styleStack.Add(style);
							FOREACH(Ptr<XmlNode>, sub, node->subNodes)
							{
								sub->Accept(this);
							}
							styleStack.RemoveAt(styleStack.Count()-1);
						}
						else if(node->name.value==L"s")
						{
							auto style=styleStack[styleStack.Count()-1];
							style.key.strikeline=true;
							styleStack.Add(style);
							FOREACH(Ptr<XmlNode>, sub, node->subNodes)
							{
								sub->Accept(this);
							}
							styleStack.RemoveAt(styleStack.Count()-1);
						}
						else if(node->name.value==L"va")
						{
							auto style=styleStack[styleStack.Count()-1];
							style.key.antialias=true;
							style.key.verticalAntialias=true;
							styleStack.Add(style);
							FOREACH(Ptr<XmlNode>, sub, node->subNodes)
							{
								sub->Accept(this);
							}
							styleStack.RemoveAt(styleStack.Count()-1);
						}
						else if(node->name.value==L"na")
						{
							auto style=styleStack[styleStack.Count()-1];
							style.key.antialias=false;
							style.key.verticalAntialias=false;
							styleStack.Add(style);
							FOREACH(Ptr<XmlNode>, sub, node->subNodes)
							{
								sub->Accept(this);
							}
							styleStack.RemoveAt(styleStack.Count()-1);
						}
						else
						{
							FOREACH(Ptr<XmlNode>, sub, node->subNodes)
							{
								sub->Accept(this);
							}
						}
					}

					void Visit(XmlInstruction* node)override
					{
					}

					void Visit(XmlDocument* node)override
					{
					}
				};
			}
			using namespace visitors;

/***********************************************************************
text::DocumentModel
***********************************************************************/

			Ptr<DocumentModel> DocumentModel::LoadFromXml(Ptr<parsing::xml::XmlDocument> xml, const WString& workingDirectory)
			{
				Ptr<DocumentModel> model=new DocumentModel;
				if(xml->rootElement->name.value==L"Doc")
				if(Ptr<XmlElement> content=XmlGetElement(xml->rootElement, L"Content"))
				FOREACH(Ptr<XmlElement>, p, XmlGetElements(content, L"p"))
				{
					Ptr<DocumentParagraph> paragraph=new DocumentParagraph;
					model->paragraphs.Add(paragraph);
					DeserializeNodeVisitor visitor(paragraph, workingDirectory);
					p->Accept(&visitor);
				}
				return model;
			}

			Ptr<parsing::xml::XmlDocument> DocumentModel::SaveToXml()
			{
				SerializeRunVisitor visitor;
				Ptr<XmlDocument> xml=new XmlDocument;
				Ptr<XmlElement> doc=new XmlElement;
				doc->name.value=L"Doc";
				xml->rootElement=doc;
				{
					Ptr<XmlElement> content=new XmlElement;
					content->name.value=L"Content";
					doc->subNodes.Add(content);

					FOREACH(Ptr<DocumentParagraph>, p, paragraphs)
					{
						Ptr<XmlElement> paragraph=new XmlElement;
						paragraph->name.value=L"p";
						content->subNodes.Add(paragraph);

						FOREACH(Ptr<DocumentLine>, l, p->lines)
						{
							FOREACH(Ptr<DocumentRun>, r, l->runs)
							{
								visitor.container=paragraph;
								r->Accept(&visitor);
							}
							{
								Ptr<XmlElement> line=new XmlElement;
								line->name.value=L"br";
								paragraph->subNodes.Add(line);
							}
						}
					}
				}
				return xml;
			}

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
					text::ParagraphCache* cache=paragraphCaches[i].Obj();
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
							Ptr<text::DocumentParagraph> paragraph=element->document->paragraphs[i];
							Ptr<text::ParagraphCache> cache=paragraphCaches[i];
							if(!cache)
							{
								cache=new text::ParagraphCache;
								paragraphCaches[i]=cache;

								stream::MemoryStream stream;
								{
									stream::StreamWriter writer(stream);
									FOREACH(Ptr<text::DocumentLine>, line, paragraph->lines)
									{
										FOREACH(Ptr<text::DocumentRun>, run, line->runs)
										{
											WString text=ExtractTextVisitor::ExtractText(run.Obj());
											writer.WriteString(text);
										}
										writer.WriteString(L"\r\n");
									}
								}
								{
									stream.SeekFromBegin(0);
									stream::StreamReader reader(stream);
									cache->fullText=reader.ReadToEnd();
								}
							}

							if(!cache->graphicsParagraph)
							{
								cache->graphicsParagraph=layoutProvider->CreateParagraph(cache->fullText, renderTarget);
								vint start=0;
								FOREACH(Ptr<text::DocumentLine>, line, paragraph->lines)
								{
									FOREACH(Ptr<text::DocumentRun>, run, line->runs)
									{
										vint length=SetPropertiesVisitor::SetProperty(start, cache->graphicsParagraph.Obj(), run.Obj());
										start+=length;
									}
									start+=2;
								}
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
					paragraphDistance=GetCurrentController()->ResourceService()->GetDefaultFont().size;
					vint defaultHeight=paragraphDistance;

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

/***********************************************************************
GuiDocumentElement
***********************************************************************/

			GuiDocumentElement::GuiDocumentElement()
			{
			}

			GuiDocumentElement::~GuiDocumentElement()
			{
			}

			Ptr<text::DocumentModel> GuiDocumentElement::GetDocument()
			{
				return document;
			}

			void GuiDocumentElement::SetDocument(Ptr<text::DocumentModel> value)
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
		}
	}
}