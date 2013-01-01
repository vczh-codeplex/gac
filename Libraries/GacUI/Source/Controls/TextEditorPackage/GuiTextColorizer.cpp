#include "GuiTextColorizer.h"
#include <math.h>

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace elements;
			using namespace elements::text;
			using namespace compositions;

/***********************************************************************
GuiTextBoxColorizer
***********************************************************************/

			void GuiTextBoxColorizerBase::ColorizerThreadProc(void* argument)
			{
				GuiTextBoxColorizerBase* colorizer=(GuiTextBoxColorizerBase*)argument;
				while(!colorizer->isFinalizing)
				{
					int lineIndex=-1;
					wchar_t* text=0;
					unsigned __int32* colors=0;
					int length=0;
					int lexerState=-1;
					int contextState=-1;

					{
						SpinLock::Scope scope(*colorizer->elementModifyLock);
						if(colorizer->colorizedLineCount>=colorizer->element->GetLines().GetCount())
						{
							colorizer->isColorizerRunning=false;
							break;
						}

						lineIndex=colorizer->colorizedLineCount++;
						TextLine& line=colorizer->element->GetLines().GetLine(lineIndex);
						length=line.dataLength;
						text=new wchar_t[length+2];
						colors=new unsigned __int32[length+2];
						memcpy(text, line.text, sizeof(wchar_t)*length);
						text[length]=L'\r';
						text[length+1]=L'\n';
						lexerState=lineIndex==0?colorizer->GetLexerStartState():colorizer->element->GetLines().GetLine(lineIndex-1).lexerFinalState;
						contextState=lineIndex==0?colorizer->GetContextStartState():colorizer->element->GetLines().GetLine(lineIndex-1).contextFinalState;
					}

					colorizer->ColorizeLineWithCRLF(text, colors, length+2, lexerState, contextState);

					{
						SpinLock::Scope scope(*colorizer->elementModifyLock);
						if(lineIndex<colorizer->colorizedLineCount && lineIndex<colorizer->element->GetLines().GetCount())
						{
							TextLine& line=colorizer->element->GetLines().GetLine(lineIndex);
							line.lexerFinalState=lexerState;
							line.contextFinalState=contextState;
							for(int i=0;i<length;i++)
							{
								line.att[i].colorIndex=colors[i];
							}
						}
						delete[] text;
						delete[] colors;
					}
				}
				colorizer->colorizerRunningEvent.Leave();
			}

			void GuiTextBoxColorizerBase::StartColorizer()
			{
				if(!isColorizerRunning)
				{
					isColorizerRunning=true;
					colorizerRunningEvent.Enter();
					ThreadPoolLite::Queue(&GuiTextBoxColorizerBase::ColorizerThreadProc, this);
				}
			}

			void GuiTextBoxColorizerBase::StopColorizer()
			{
				isFinalizing=true;
				colorizerRunningEvent.Enter();
				colorizerRunningEvent.Leave();
				colorizedLineCount=0;
				isFinalizing=false;
			}

			GuiTextBoxColorizerBase::GuiTextBoxColorizerBase()
				:element(0)
				,elementModifyLock(0)
				,colorizedLineCount(0)
				,isColorizerRunning(false)
				,isFinalizing(false)
			{
			}

			GuiTextBoxColorizerBase::~GuiTextBoxColorizerBase()
			{
				StopColorizer();
			}

			void GuiTextBoxColorizerBase::Attach(elements::GuiColorizedTextElement* _element, SpinLock& _elementModifyLock)
			{
				if(_element)
				{
					SpinLock::Scope scope(_elementModifyLock);
					element=_element;
					elementModifyLock=&_elementModifyLock;
					StartColorizer();
				}
			}

			void GuiTextBoxColorizerBase::Detach()
			{
				if(element && elementModifyLock)
				{
					StopColorizer();
					SpinLock::Scope scope(*elementModifyLock);
					element=0;
					elementModifyLock=0;
				}
			}

			void GuiTextBoxColorizerBase::TextEditNotify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)
			{
				if(element && elementModifyLock)
				{
					SpinLock::Scope scope(*elementModifyLock);
					int line=originalStart.row<originalEnd.row?originalStart.row:originalEnd.row;
					if(colorizedLineCount>line)
					{
						colorizedLineCount=line;
					}
					StartColorizer();
				}
			}

/***********************************************************************
GuiTextBoxRegexColorizer
***********************************************************************/

			struct GuiTextBoxRegexColorizerProcData
			{
				GuiTextBoxRegexColorizer*		colorizer;
				const wchar_t*					text;
				unsigned __int32*				colors;
				int								contextState;
			};

			void GuiTextBoxRegexColorizer::ColorizerProc(void* argument, vint start, vint length, vint token)
			{
				GuiTextBoxRegexColorizerProcData& data=*(GuiTextBoxRegexColorizerProcData*)argument;
				data.colorizer->ColorizeTokenContextSensitive(data.text, start, length, token, data.contextState);
				for(int i=0;i<length;i++)
				{
					data.colors[start+i]=token+1;
				}
			}

			GuiTextBoxRegexColorizer::GuiTextBoxRegexColorizer()
			{
				colors.Resize(1);
			}

			GuiTextBoxRegexColorizer::~GuiTextBoxRegexColorizer()
			{
			}

			elements::text::ColorEntry GuiTextBoxRegexColorizer::GetDefaultColor()
			{
				return defaultColor;
			}

			collections::List<WString>& GuiTextBoxRegexColorizer::GetTokenRegexes()
			{
				return tokenRegexes;
			}

			collections::List<elements::text::ColorEntry>& GuiTextBoxRegexColorizer::GetTokenColors()
			{
				return tokenColors;
			}

			collections::List<elements::text::ColorEntry>& GuiTextBoxRegexColorizer::GetExtraTokenColors()
			{
				return extraTokenColors;
			}

			int GuiTextBoxRegexColorizer::GetExtraTokenIndexStart()
			{
				if(lexer)
				{
					return tokenColors.Count();
				}
				else
				{
					return -1;
				}
			}

			bool GuiTextBoxRegexColorizer::SetDefaultColor(elements::text::ColorEntry value)
			{
				if(lexer)
				{
					return false;
				}
				else
				{
					defaultColor=value;
					return true;
				}
			}

			int GuiTextBoxRegexColorizer::AddToken(const WString& regex, elements::text::ColorEntry color)
			{
				if(lexer)
				{
					return -1;
				}
				else
				{
					tokenRegexes.Add(regex);
					tokenColors.Add(color);
					return tokenColors.Count()-1;
				}
			}

			int GuiTextBoxRegexColorizer::AddExtraToken(elements::text::ColorEntry color)
			{
				if(lexer)
				{
					return -1;
				}
				else
				{
					extraTokenColors.Add(color);
					return extraTokenColors.Count()-1;
				}
			}

			bool GuiTextBoxRegexColorizer::Setup()
			{
				if(lexer || tokenRegexes.Count()==0)
				{
					return false;
				}
				else
				{
					lexer=new regex::RegexLexer(tokenRegexes);
					colors.Resize(1+tokenRegexes.Count()+extraTokenColors.Count());
					colors[0]=defaultColor;
					for(int i=0;i<tokenColors.Count();i++)
					{
						colors[i+1]=tokenColors[i];
					}
					for(int i=0;i<extraTokenColors.Count();i++)
					{
						colors[i+1+tokenColors.Count()]=extraTokenColors[i];
					}
					colorizer=new regex::RegexLexerColorizer(lexer->Colorize());
					return true;
				}
			}

			void GuiTextBoxRegexColorizer::ColorizeTokenContextSensitive(const wchar_t* text, vint start, vint length, vint& token, int& contextState)
			{
			}

			int GuiTextBoxRegexColorizer::GetLexerStartState()
			{
				return lexer?colorizer->GetStartState():-1;
			}

			int GuiTextBoxRegexColorizer::GetContextStartState()
			{
				return 0;
			}

			void GuiTextBoxRegexColorizer::ColorizeLineWithCRLF(const wchar_t* text, unsigned __int32* colors, int length, int& lexerState, int& contextState)
			{
				if(lexer)
				{
					GuiTextBoxRegexColorizerProcData data;
					data.colorizer=this;
					data.text=text;
					data.colors=colors;
					data.contextState=contextState;

					memset(colors, 0, sizeof(*colors)*length);
					colorizer->Reset(lexerState);
					colorizer->Colorize(text, length, &GuiTextBoxRegexColorizer::ColorizerProc, &data);

					lexerState=colorizer->GetCurrentState();
					contextState=data.contextState;
				}
				else
				{
					lexerState=-1;
					contextState=-1;
				}
			}

			const GuiTextBoxRegexColorizer::ColorArray& GuiTextBoxRegexColorizer::GetColors()
			{
				return colors;
			}
		}
	}
}