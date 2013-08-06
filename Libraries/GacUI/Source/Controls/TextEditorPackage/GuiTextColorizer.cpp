#include "GuiTextColorizer.h"
#include "..\Styles\GuiThemeStyleFactory.h"
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
			using namespace parsing;
			using namespace parsing::tabling;
			using namespace collections;
			using namespace theme;

/***********************************************************************
GuiTextBoxColorizerBase
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

			void GuiTextBoxColorizerBase::StopColorizer(bool forever)
			{
				isFinalizing=true;
				colorizerRunningEvent.Enter();
				colorizerRunningEvent.Leave();
				colorizedLineCount=0;
				if(!forever)
				{
					isFinalizing=false;
				}
			}

			void GuiTextBoxColorizerBase::StopColorizerForever()
			{
				StopColorizer(true);
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
				StopColorizerForever();
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
					StopColorizer(false);
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

			void GuiTextBoxColorizerBase::TextEditFinished()
			{
			}

			void GuiTextBoxColorizerBase::RestartColorizer()
			{
				if(element && elementModifyLock)
				{
					SpinLock::Scope scope(*elementModifyLock);
					colorizedLineCount=0;
					StartColorizer();
				}
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
				StopColorizerForever();
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

/***********************************************************************
RepeatingParsingExecutor
***********************************************************************/

			void RepeatingParsingExecutor::OnParsingFinished()
			{
			}

			void RepeatingParsingExecutor::Initialize(Ptr<parsing::tabling::ParsingGeneralParser> _grammarParser, const WString& _grammarRule)
			{
				grammarParser=_grammarParser;
				grammarRule=_grammarRule;
			}

			void RepeatingParsingExecutor::Execute(const WString& input)
			{
				List<Ptr<ParsingError>> errors;
				Ptr<ParsingTreeObject> node=grammarParser->Parse(input, grammarRule, errors).Cast<ParsingTreeObject>();
				if(node)
				{
					node->InitializeQueryCache();

					SpinLock::Scope scope(parsingTreeLock);
					parsingTreeNode=node;
				}
				if(node)
				{
					OnParsingFinished();
				}
				node=0;
			}

			RepeatingParsingExecutor::RepeatingParsingExecutor()
			{
			}

			RepeatingParsingExecutor::~RepeatingParsingExecutor()
			{
			}

			Ptr<parsing::ParsingTreeObject> RepeatingParsingExecutor::ThreadSafeGetTreeNode()
			{
				parsingTreeLock.Enter();
				return parsingTreeNode;
			}

			void RepeatingParsingExecutor::ThreadSafeReturnTreeNode()
			{
				parsingTreeLock.Leave();
			}

			Ptr<parsing::tabling::ParsingGeneralParser> RepeatingParsingExecutor::GetParser()
			{
				return grammarParser;
			}

/***********************************************************************
GuiGrammarColorizer
***********************************************************************/

			Ptr<parsing::tabling::ParsingTable::AttributeInfo> GuiGrammarColorizer::GetAttribute(vint index, const WString& name, vint argumentCount)
			{
				if(index!=-1)
				{
					Ptr<ParsingTable::AttributeInfo> att=GetParser()->GetTable()->GetAttributeInfo(index)->FindFirst(name);
					if(att && att->arguments.Count()==argumentCount)
					{
						return att;
					}
				}
				return 0;
			}

			Ptr<parsing::tabling::ParsingTable::AttributeInfo> GuiGrammarColorizer::GetColorAttribute(vint index)
			{
				return GetAttribute(index, L"Color", 1);
			}

			Ptr<parsing::tabling::ParsingTable::AttributeInfo> GuiGrammarColorizer::GetContextColorAttribute(vint index)
			{
				return GetAttribute(index, L"ContextColor", 1);
			}

			Ptr<parsing::tabling::ParsingTable::AttributeInfo> GuiGrammarColorizer::GetSemanticColorAttribute(vint index)
			{
				return GetAttribute(index, L"SemanticColor", 1);
			}

			void GuiGrammarColorizer::Attach(elements::GuiColorizedTextElement* _element, SpinLock& _elementModifyLock)
			{
				GuiTextBoxRegexColorizer::Attach(_element, _elementModifyLock);
				if(element && elementModifyLock)
				{
					SpinLock::Scope scope(*elementModifyLock);
					WString text=element->GetLines().GetText();
					SubmitTask(text.Buffer());
				}
			}

			void GuiGrammarColorizer::Detach()
			{
				GuiTextBoxRegexColorizer::Detach();
				if(element && elementModifyLock)
				{
					EnsureTaskFinished();
					StopColorizer(false);
				}
			}

			void GuiGrammarColorizer::TextEditFinished()
			{
				GuiTextBoxRegexColorizer::TextEditFinished();
				if(element && elementModifyLock)
				{
					SpinLock::Scope scope(*elementModifyLock);
					WString text=element->GetLines().GetText();
					SubmitTask(text.Buffer());
				}
			}

			void GuiGrammarColorizer::OnSemanticColorize(parsing::ParsingTreeToken* foundToken, parsing::ParsingTreeObject* tokenParent, const WString& type, const WString& field, vint semantic, vint& token)
			{
			}

			void GuiGrammarColorizer::Execute(const WString& input)
			{
				RepeatingParsingExecutor::Execute(input);
				RestartColorizer();
			}

			GuiGrammarColorizer::GuiGrammarColorizer()
			{
				BeginSetColors();
			}

			GuiGrammarColorizer::GuiGrammarColorizer(Ptr<parsing::tabling::ParsingGeneralParser> _grammarParser, const WString& _grammarRule)
			{
				Initialize(_grammarParser, _grammarRule);
				BeginSetColors();
			}

			GuiGrammarColorizer::~GuiGrammarColorizer()
			{
				EnsureTaskFinished();
				StopColorizerForever();
			}

			void GuiGrammarColorizer::SubmitCode(const WString& code)
			{
				SubmitTask(code.Buffer());
			}

			vint GuiGrammarColorizer::GetTokenId(const WString& token)
			{
				vint index=colorIndices.Keys().IndexOf(token);
				return index==-1?-1:colorIndices.Values().Get(index);
			}

			vint GuiGrammarColorizer::GetSemanticId(const WString& semantic)
			{
				vint index=semanticIndices.Keys().IndexOf(semantic);
				return index==-1?-1:semanticIndices.Values().Get(index);
			}

			void GuiGrammarColorizer::BeginSetColors()
			{
				ClearTokens();
				colorSettings.Clear();
				text::ColorEntry entry=GetCurrentTheme()->GetDefaultTextBoxColorEntry();
				SetDefaultColor(entry);
				colorSettings.Add(L"Default", entry);
			}

			const collections::SortedList<WString>& GuiGrammarColorizer::GetColorNames()
			{
				return colorSettings.Keys();
			}

			GuiGrammarColorizer::ColorEntry GuiGrammarColorizer::GetColor(const WString& name)
			{
				vint index=colorSettings.Keys().IndexOf(name);
				return index==-1?GetDefaultColor():colorSettings.Values().Get(index);
			}

			void GuiGrammarColorizer::SetColor(const WString& name, const ColorEntry& entry)
			{
				colorSettings.Set(name, entry);
			}

			void GuiGrammarColorizer::SetColor(const WString& name, const Color& color)
			{
				text::ColorEntry entry=GetDefaultColor();
				entry.normal.text=color;
				SetColor(name, entry);
			}

			void GuiGrammarColorizer::EndSetColors()
			{
				SortedList<WString> tokenColors;
				Ptr<ParsingTable> table=GetParser()->GetTable();
				colorIndices.Clear();
				colorContext.Clear();
				fieldContextColors.Clear();
				fieldSemanticColors.Clear();
				semanticIndices.Clear();

				// prepare tokens
				{
					vint tokenCount=table->GetTokenCount();
					for(vint token=ParsingTable::UserTokenStart;token<tokenCount;token++)
					{
						const ParsingTable::TokenInfo& tokenInfo=table->GetTokenInfo(token);
						if(Ptr<ParsingTable::AttributeInfo> att=GetColorAttribute(tokenInfo.attributeIndex))
						{
							tokenColors.Add(att->arguments[0]);
							vint tokenId=AddToken(tokenInfo.regex, GetColor(att->arguments[0]));
							colorIndices.Set(att->arguments[0], tokenId);
							colorContext.Add(false);
						}
						else if(Ptr<ParsingTable::AttributeInfo> att=GetContextColorAttribute(tokenInfo.attributeIndex))
						{
							tokenColors.Add(att->arguments[0]);
							vint tokenId=AddToken(tokenInfo.regex, GetColor(att->arguments[0]));
							colorIndices.Set(att->arguments[0], tokenId);
							colorContext.Add(true);
						}
						else
						{
							AddToken(tokenInfo.regex, GetDefaultColor());
							colorContext.Add(false);
						}
					}
				}

				// prepare extra tokens
				FOREACH_INDEXER(WString, color, index, colorSettings.Keys())
				{
					if(!tokenColors.Contains(color))
					{
						vint tokenId=AddExtraToken(colorSettings.Values().Get(index));
						colorIndices.Set(color, tokenId+colorContext.Count());
					}
				}

				// prepare fields
				{
					vint fieldCount=table->GetTreeFieldInfoCount();
					for(vint field=0;field<fieldCount;field++)
					{
						const ParsingTable::TreeFieldInfo& fieldInfo=table->GetTreeFieldInfo(field);
						if(Ptr<ParsingTable::AttributeInfo> att=GetColorAttribute(fieldInfo.attributeIndex))
						{
							vint index=colorIndices.Keys().IndexOf(att->arguments[0]);
							if(index!=-1)
							{
								fieldContextColors.Add(FieldDesc(fieldInfo.type, fieldInfo.field), colorIndices.Values().Get(index));
							}
						}
						else if(Ptr<ParsingTable::AttributeInfo> att=GetSemanticColorAttribute(fieldInfo.attributeIndex))
						{
							FieldDesc key(fieldInfo.type, fieldInfo.field);
							vint semantic=-1;
							vint index=semanticIndices.Keys().IndexOf(att->arguments[0]);
							if(index==-1)
							{
								semantic=semanticIndices.Count();
								semanticIndices.Add(att->arguments[0], semantic);
							}
							else
							{
								semantic=semanticIndices.Values().Get(index);
							}
							fieldSemanticColors.Add(key, semantic);
						}
					}
				}
				Setup();
			}

			void GuiGrammarColorizer::ColorizeTokenContextSensitive(int lineIndex, const wchar_t* text, vint start, vint length, vint& token, int& contextState)
			{
				Ptr<ParsingTreeNode> node=ThreadSafeGetTreeNode();
				if(node && token!=-1 && colorContext[token])
				{
					ParsingTextPos pos(lineIndex, start);
					ParsingTreeNode* foundNode=node->FindDeepestNode(pos);
					if(!foundNode) goto EXIT_COLORIZING;
					ParsingTreeToken* foundToken=dynamic_cast<ParsingTreeToken*>(foundNode);
					if(!foundToken) goto EXIT_COLORIZING;
					ParsingTreeObject* tokenParent=dynamic_cast<ParsingTreeObject*>(foundNode->GetParent());
					if(!tokenParent) goto EXIT_COLORIZING;
					vint index=tokenParent->GetMembers().Values().IndexOf(foundNode);
					if(index==-1) goto EXIT_COLORIZING;

					WString type=tokenParent->GetType();
					WString field=tokenParent->GetMembers().Keys().Get(index);
					FieldDesc key(type, field);

					index=fieldContextColors.Keys().IndexOf(key);
					if(index!=-1)
					{
						token=fieldContextColors.Values().Get(index);
						goto EXIT_COLORIZING;
					}

					index=fieldSemanticColors.Keys().IndexOf(key);
					if(index!=-1)
					{
						vint semantic=fieldSemanticColors.Values().Get(index);
						OnSemanticColorize(foundToken, tokenParent, type, field, semantic, token);
						goto EXIT_COLORIZING;
					}
				}
			EXIT_COLORIZING:
				ThreadSafeReturnTreeNode();
			}
		}
	}
}