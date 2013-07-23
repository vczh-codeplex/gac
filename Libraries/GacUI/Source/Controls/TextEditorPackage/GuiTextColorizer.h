/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUITEXTCOLORIZER
#define VCZH_PRESENTATION_CONTROLS_GUITEXTCOLORIZER

#include "GuiTextGeneralOperations.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
GuiTextBoxColorizerBase
***********************************************************************/
			
			/// <summary>The base class of text box colorizer.</summary>
			class GuiTextBoxColorizerBase : public Object, public ICommonTextEditCallback
			{
			public:
				typedef collections::Array<elements::text::ColorEntry>			ColorArray;
			protected:
				elements::GuiColorizedTextElement*			element;
				SpinLock*									elementModifyLock;
				volatile vint								colorizedLineCount;
				volatile bool								isColorizerRunning;
				volatile bool								isFinalizing;
				SpinLock									colorizerRunningEvent;

				static void									ColorizerThreadProc(void* argument);

				void										StartColorizer();
				void										StopColorizer();
			public:
				GuiTextBoxColorizerBase();
				~GuiTextBoxColorizerBase();

				void										Attach(elements::GuiColorizedTextElement* _element, SpinLock& _elementModifyLock)override;
				void										Detach()override;
				void										TextEditNotify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)override;
				void										RestartColorizer();

				/// <summary>Get the lexical analyzer start state for the first line.</summary>
				/// <returns>The lexical analyzer start state for the first line.</returns>
				virtual vint								GetLexerStartState()=0;
				/// <summary>Get the context sensitive start state for the first line.</summary>
				/// <returns>The context sensitive start state for the first line.</returns>
				virtual vint								GetContextStartState()=0;
				/// <summary>Colorizer one line with a start state.</summary>
				/// <param name="lineIndex">Line index.</param>
				/// <param name="text">Text buffer.</param>
				/// <param name="colors">Color index buffer. The index should be in [0 .. [M:vl.presentation.controls.GuiTextBoxColorizerBase.GetColors]()-1].</param>
				/// <param name="length">The length of the buffer.</param>
				/// <param name="lexerState">The lexical analyzer state for this line. After executing this function, the new value of this argument indicates the new state.</param>
				/// <param name="contextState">The context sensitive state for this line. After executing this function, the new value of this argument indicates the new state.</param>
				virtual void								ColorizeLineWithCRLF(vint lineIndex, const wchar_t* text, unsigned __int32* colors, vint length, vint& lexerState, vint& contextState)=0;
				/// <summary>Get the supported colors ordered by their indices.</summary>
				/// <returns>The supported colors ordered by their indices.</returns>
				virtual const ColorArray&					GetColors()=0;
			};

/***********************************************************************
GuiTextBoxRegexColorizer
***********************************************************************/

			/// <summary>Regex based colorizer.</summary>
			class GuiTextBoxRegexColorizer : public GuiTextBoxColorizerBase
			{
			protected:
				Ptr<regex::RegexLexer>										lexer;
				Ptr<regex::RegexLexerColorizer>								colorizer;
				ColorArray													colors;

				elements::text::ColorEntry									defaultColor;
				collections::List<WString>									tokenRegexes;
				collections::List<elements::text::ColorEntry>				tokenColors;
				collections::List<elements::text::ColorEntry>				extraTokenColors;

				static void													ColorizerProc(void* argument, vint start, vint length, vint token);
			public:
				/// <summary>Create the colorizer.</summary>
				GuiTextBoxRegexColorizer();
				~GuiTextBoxRegexColorizer();

				/// <summary>Get the default color.</summary>
				/// <returns>The default color.</returns>
				elements::text::ColorEntry									GetDefaultColor();
				/// <summary>Get all regular expressions for tokens.</summary>
				/// <returns>All regular expressions for tokens.</returns>
				collections::List<WString>&									GetTokenRegexes();
				/// <summary>Get all colors for tokens.</summary>
				/// <returns>All colors for tokens.</returns>
				collections::List<elements::text::ColorEntry>&				GetTokenColors();
				/// <summary>Get all colors for extra tokens.</summary>
				/// <returns>All colors for extra tokens.</returns>
				collections::List<elements::text::ColorEntry>&				GetExtraTokenColors();
				/// <summary>Get the first token index for the first extra token.</summary>
				/// <returns>The first token index for the first extra token. Returns -1 if this operation failed.</returns>
				vint														GetExtraTokenIndexStart();
				
				/// <summary>Set the default color. Call [M:vl.presentation.controls.GuiTextBoxRegexColorizer.Setup] after finishing all configuration.</summary>
				/// <returns>Returns the token index of this token. Returns -1 if this operation failed.</returns>
				/// <param name="value">The default color.</param>
				bool														SetDefaultColor(elements::text::ColorEntry value);
				/// <summary>Add a token type. Call [M:vl.presentation.controls.GuiTextBoxRegexColorizer.Setup] after finishing all configuration.</summary>
				/// <returns>Returns the token index of this token. Returns -1 if this operation failed.</returns>
				/// <param name="regex">The regular expression for this token type.</param>
				/// <param name="color">The color for this token type.</param>
				vint														AddToken(const WString& regex, elements::text::ColorEntry color);
				/// <summary>Add an extra  token type. Call [M:vl.presentation.controls.GuiTextBoxRegexColorizer.Setup] after finishing all configuration.</summary>
				/// <returns>Returns the extra token index of this token. The token index for this token is regex-token-count + extra-token-index Returns -1 if this operation failed.</returns>
				/// <param name="color">The color for this token type.</param>
				vint														AddExtraToken(elements::text::ColorEntry color);
				/// <summary>Clear all token color settings.</summary>
				void														ClearTokens();
				/// <summary>Setup the colorizer. After that, the colorizer cannot be changed.</summary>
				void														Setup();
				/// <summary>Callback function to set context sensitive state and change token accordingly.</summary>
				/// <param name="lineIndex">Line index.</param>
				/// <param name="text">Text buffer.</param>
				/// <param name="start">The start position of the token.</param>
				/// <param name="length">The length of the token.</param>
				/// <param name="token">The token type. After executing this function, the new value of this argument indicates the new token type.</param>
				/// <param name="contextState">The context sensitive state. After executing this function, the new value of this argument indicates the new state.</param>
				virtual void												ColorizeTokenContextSensitive(vint lineIndex, const wchar_t* text, vint start, vint length, vint& token, vint& contextState);

				vint														GetLexerStartState()override;
				vint														GetContextStartState()override;
				void														ColorizeLineWithCRLF(vint lineIndex, const wchar_t* text, unsigned __int32* colors, vint length, vint& lexerState, vint& contextState)override;
				const ColorArray&											GetColors()override;
			};

/***********************************************************************
GrammarColorizer
***********************************************************************/

			class GrammarColorizer abstract : public GuiTextBoxRegexColorizer, public RepeatingTaskExecutor<WString>
			{
				typedef collections::Pair<WString, WString>					FieldDesc;
				typedef collections::Dictionary<FieldDesc, vint>			FieldContextColors;
				typedef collections::Dictionary<FieldDesc, vint>			FieldSemanticColors;
				typedef elements::text::ColorEntry							ColorEntry;
			private:
				Ptr<parsing::tabling::ParsingGeneralParser>					grammarParser;
				WString														grammarRule;
				SpinLock													parsingTreeLock;
				Ptr<parsing::ParsingTreeObject>								parsingTreeNode;

				collections::Dictionary<WString, ColorEntry>				colorSettings;
				collections::Dictionary<WString, vint>						colorIndices;
				collections::List<bool>										colorContext;
				FieldContextColors											fieldContextColors;
				FieldSemanticColors											fieldSemanticColors;
				collections::Dictionary<WString, vint>						semanticIndices;

				Ptr<parsing::tabling::ParsingTable::AttributeInfo>			GetAttribute(vint index, const WString& name, vint argumentCount);
				Ptr<parsing::tabling::ParsingTable::AttributeInfo>			GetColorAttribute(vint index);
				Ptr<parsing::tabling::ParsingTable::AttributeInfo>			GetContextColorAttribute(vint index);
				Ptr<parsing::tabling::ParsingTable::AttributeInfo>			GetSemanticColorAttribute(vint index);
				ColorEntry													GetColor(const WString& name);
			protected:
				virtual void												OnParsingFinished();
				virtual void												OnSemanticColorize(parsing::ParsingTreeToken* foundToken, parsing::ParsingTreeObject* tokenParent, const WString& type, const WString& field, vint semantic, vint& token);

				void														Initialize(Ptr<parsing::tabling::ParsingGeneralParser> _grammarParser, const WString& _grammarRule);
				void														Execute(const WString& input)override;
			public:
				GrammarColorizer();
				GrammarColorizer(Ptr<parsing::tabling::ParsingGeneralParser> _grammarParser, const WString& _grammarRule);

				Ptr<parsing::ParsingTreeObject>								ThreadSafeGetTreeNode();
				void														ThreadSafeReturnTreeNode();
				void														SubmitCode(const WString& code);
				vint														GetTokenId(const WString& token);
				vint														GetSemanticId(const WString& semantic);
				void														BeginSetColors();
				void														SetColor(const WString& name, const ColorEntry& entry);
				void														SetColor(const WString& name, const Color& color);
				void														EndSetColors();
				void														ColorizeTokenContextSensitive(int lineIndex, const wchar_t* text, vint start, vint length, vint& token, int& contextState)override;
			};
		}
	}
}

#endif