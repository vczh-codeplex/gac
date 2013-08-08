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
				void										StopColorizer(bool forever);
				void										StopColorizerForever();
			public:
				GuiTextBoxColorizerBase();
				~GuiTextBoxColorizerBase();

				void										Attach(elements::GuiColorizedTextElement* _element, SpinLock& _elementModifyLock)override;
				void										Detach()override;
				void										TextEditNotify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)override;
				void										TextEditFinished()override;
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
RepeatingParsingExecutor
***********************************************************************/

			/// <summary>Repeating parsing executor.</summary>
			class RepeatingParsingExecutor : public RepeatingTaskExecutor<WString>
			{
			public:
				/// <summary>Callback.</summary>
				class ICallback : public virtual Interface
				{
				public:
					/// <summary>Callback when a parsing task is finished. <see cref="RepeatingParsingExecutor::ThreadSafeGetTreeNode"/> and <see cref="RepeatingParsingExecutor::ThreadSafeReturnTreeNode"/> can be used to get the new syntax tree.</summary>
					/// <param name="generatedNewNode">True indicates the parsing succeeded. Otherwise failed.</param>
					/// <param name="parsingExecutor">The parsing executor that parses the input.</param>
					virtual void											OnParsingFinished(bool generatedNewNode, RepeatingParsingExecutor* parsingExecutor)=0;
				};
			private:
				Ptr<parsing::tabling::ParsingGeneralParser>					grammarParser;
				WString														grammarRule;
				SpinLock													parsingTreeLock;
				Ptr<parsing::ParsingTreeObject>								parsingTreeNode;
				collections::List<ICallback*>								callbacks;

			protected:

				void														Execute(const WString& input)override;
			public:
				/// <summary>Initialize the parsing executor.</summary>
				/// <param name="_grammarParser">Parser generated from a grammar.</param>
				/// <param name="_grammarRule"></param>
				RepeatingParsingExecutor(Ptr<parsing::tabling::ParsingGeneralParser> _grammarParser, const WString& _grammarRule);
				~RepeatingParsingExecutor();
				
				/// <summary>Get the parsed syntax tree and block all threads when calling this function.</summary>
				/// <returns>The parsed syntax tree.</returns>
				Ptr<parsing::ParsingTreeObject>								ThreadSafeGetTreeNode();
				/// <summary>Unblock all threads that calling <see cref="ThreadSafeGetTreeNode"/>.</summary>
				void														ThreadSafeReturnTreeNode();
				/// <summary>Get the internal parser that parse the text.</summary>
				Ptr<parsing::tabling::ParsingGeneralParser>					GetParser();
				/// <summary>Detach callback.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="value">The callback.</param>
				bool														AttachCallback(ICallback* value);
				/// <summary>Detach callback.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="value">The callback.</param>
				bool														DetachCallback(ICallback* value);
			};

/***********************************************************************
GuiGrammarColorizer
***********************************************************************/

			/// <summary>Grammar based colorizer.</summary>
			class GuiGrammarColorizer abstract : public GuiTextBoxRegexColorizer, private RepeatingParsingExecutor::ICallback
			{
				typedef collections::Pair<WString, WString>					FieldDesc;
				typedef collections::Dictionary<FieldDesc, vint>			FieldContextColors;
				typedef collections::Dictionary<FieldDesc, vint>			FieldSemanticColors;
				typedef elements::text::ColorEntry							ColorEntry;
			private:
				Ptr<RepeatingParsingExecutor>								parsingExecutor;
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

			protected:
				void														Attach(elements::GuiColorizedTextElement* _element, SpinLock& _elementModifyLock)override;
				void														Detach()override;
				void														TextEditFinished()override;
				void														OnParsingFinished(bool generatedNewNode, RepeatingParsingExecutor* parsingExecutor)override;

				/// <summary>Called when a @SemanticColor attribute in a grammar is activated during colorizing to determine a color for the token.</summary>
				/// <param name="foundToken">Token syntax tree for the colorizing token.</param>
				/// <param name="tokenParent">The object syntax tree parent of the token.</param>
				/// <param name="type">Type of the parent.</param>
				/// <param name="field">Field of the parent that contains the token.</param>
				/// <param name="semantic">Semantic id that comes from the argument in the @SemanticColor attribute. Name-id mapping can be retrived using <see cref="GetSemanticId"/>.</param>
				/// <param name="token">Output argument for the result color. Name-id mapping can be retrived using <see cref="GetTokenId"/>.</param>
				virtual void												OnSemanticColorize(parsing::ParsingTreeToken* foundToken, parsing::ParsingTreeObject* tokenParent, const WString& type, const WString& field, vint semantic, vint& token);

				/// <summary>Call this function in the derived class's destructor when it overrided <see cref="OnSemanticColorize"/>.</summary>
				void														EnsureColorizerFinished();
			public:
				/// <summary>Create the colorizer with a created parsing executor.</summary>
				/// <param name="_parsingExecutor">The parsing executor.</param>
				GuiGrammarColorizer(Ptr<RepeatingParsingExecutor> _parsingExecutor);
				/// <summary>Create the colorizer with a specified grammar and start rule to create a <see cref="RepeatingParsingExecutor"/>.</summary>
				/// <param name="_grammarParser">Parser generated from a grammar.</param>
				/// <param name="_grammarRule"></param>
				GuiGrammarColorizer(Ptr<parsing::tabling::ParsingGeneralParser> _grammarParser, const WString& _grammarRule);
				~GuiGrammarColorizer();

				/// <summary>Submit a new code for parsing. Usually this function does not need to be called.</summary>
				/// <param name="code">The text of the text box.</param>
				void														SubmitCode(const WString& code);
				/// <summary>Get the id for a token name.</summary>
				/// <returns>The id.</returns>
				/// <param name="token">The name of the token.</param>
				vint														GetTokenId(const WString& token);
				/// <summary>Get the id for a semantic name.</summary>
				/// <returns>The id.</returns>
				/// <param name="token">The name of the semantic.</param>
				vint														GetSemanticId(const WString& semantic);
				/// <summary>Reset all color settings.</summary>
				void														BeginSetColors();
				/// <summary>Get all color names.</summary>
				/// <returns>All color names.</returns>
				const collections::SortedList<WString>&						GetColorNames();
				/// <summary>Get the color for a token theme name (@Color or @ContextColor("theme-name") in the grammar).</summary>
				/// <returns>The color.</returns>
				/// <param name="name">The token theme name.</param>
				ColorEntry													GetColor(const WString& name);
				/// <summary>Set a color for a token theme name (@Color or @ContextColor("theme-name") in the grammar).</summary>
				/// <param name="name">The token theme name.</param>
				/// <param name="entry">The color.</param>
				void														SetColor(const WString& name, const ColorEntry& entry);
				/// <summary>Set a color for a token theme name (@Color or @ContextColor("theme-name") in the grammar).</summary>
				/// <param name="name">The token theme name.</param>
				/// <param name="entry">The color.</param>
				void														SetColor(const WString& name, const Color& color);
				/// <summary>Submit all color settings.</summary>
				void														EndSetColors();
				void														ColorizeTokenContextSensitive(int lineIndex, const wchar_t* text, vint start, vint length, vint& token, int& contextState)override;

				/// <summary>Get the internal parsing executor.</summary>
				/// <returns>The parsing executor.</returns>
				Ptr<RepeatingParsingExecutor>								GetParsingExecutor();
			};
		}
	}
}

#endif