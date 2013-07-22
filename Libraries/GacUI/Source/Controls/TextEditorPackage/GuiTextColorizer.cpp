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
					vint lineIndex=-1;
					wchar_t* text=0;
					unsigned __int32* colors=0;
					vint length=0;
					vint lexerState=-1;
					vint contextState=-1;

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

					colorizer->ColorizeLineWithCRLF(lineIndex, text, colors, length+2, lexerState, contextState);

					{
						SpinLock::Scope scope(*colorizer->elementModifyLock);
						if(lineIndex<colorizer->colorizedLineCount && lineIndex<colorizer->element->GetLines().GetCount())
						{
							TextLine& line=colorizer->element->GetLines().GetLine(lineIndex);
							line.lexerFinalState=lexerState;
							line.contextFinalState=contextState;
							for(vint i=0;i<length;i++)
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
					vint line=originalStart.row<originalEnd.row?originalStart.row:originalEnd.row;
					if(colorizedLineCount>line)
					{
						colorizedLineCount=line;
					}
					StartColorizer();
				}
			}

			void GuiTextBoxColorizerBase::RestartColorizer()
			{
				SpinLock::Scope scope(*elementModifyLock);
				colorizedLineCount=0;
				StartColorizer();
			}

/***********************************************************************
GuiTextBoxRegexColorizer
***********************************************************************/

			struct GuiTextBoxRegexColorizerProcData
			{
				GuiTextBoxRegexColorizer*		colorizer;
				vint							lineIndex;
				const wchar_t*					text;
				unsigned __int32*				colors;
				vint							contextState;
			};

			void GuiTextBoxRegexColorizer::ColorizerProc(void* argument, vint start, vint length, vint token)
			{
				GuiTextBoxRegexColorizerProcData& data=*(GuiTextBoxRegexColorizerProcData*)argument;
				data.colorizer->ColorizeTokenContextSensitive(data.lineIndex, data.text, start, length, token, data.contextState);
				for(vint i=0;i<length;i++)
				{
					data.colors[start+i]=(int)token+1;
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

			vint GuiTextBoxRegexColorizer::GetExtraTokenIndexStart()
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

			vint GuiTextBoxRegexColorizer::AddToken(const WString& regex, elements::text::ColorEntry color)
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

			vint GuiTextBoxRegexColorizer::AddExtraToken(elements::text::ColorEntry color)
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

			void GuiTextBoxRegexColorizer::ClearTokens()
			{
				tokenRegexes.Clear();
				tokenColors.Clear();
				extraTokenColors.Clear();
				lexer=0;
			}

			void GuiTextBoxRegexColorizer::Setup()
			{
				if(lexer || tokenRegexes.Count()==0)
				{
					colors.Resize(1);
					colors[0]=defaultColor;
				}
				else
				{
					lexer=new regex::RegexLexer(tokenRegexes);
					colors.Resize(1+tokenRegexes.Count()+extraTokenColors.Count());
					colors[0]=defaultColor;
					for(vint i=0;i<tokenColors.Count();i++)
					{
						colors[i+1]=tokenColors[i];
					}
					for(vint i=0;i<extraTokenColors.Count();i++)
					{
						colors[i+1+tokenColors.Count()]=extraTokenColors[i];
					}
					colorizer=new regex::RegexLexerColorizer(lexer->Colorize());
				}
			}

			void GuiTextBoxRegexColorizer::ColorizeTokenContextSensitive(vint lineIndex, const wchar_t* text, vint start, vint length, vint& token, vint& contextState)
			{
			}

			vint GuiTextBoxRegexColorizer::GetLexerStartState()
			{
				return lexer?colorizer->GetStartState():-1;
			}

			vint GuiTextBoxRegexColorizer::GetContextStartState()
			{
				return 0;
			}

			void GuiTextBoxRegexColorizer::ColorizeLineWithCRLF(vint lineIndex, const wchar_t* text, unsigned __int32* colors, vint length, vint& lexerState, vint& contextState)
			{
				memset(colors, 0, sizeof(*colors)*length);
				if(lexer)
				{
					GuiTextBoxRegexColorizerProcData data;
					data.colorizer=this;
					data.lineIndex=lineIndex;
					data.text=text;
					data.colors=colors;
					data.contextState=contextState;

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