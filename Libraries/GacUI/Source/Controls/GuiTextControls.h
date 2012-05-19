/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUITEXTCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUITEXTCONTROLS

#include "GuiBasicControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
Common Operations
***********************************************************************/

			class GuiTextBoxCommonInterface;

			class GuiTextElementOperator : public Object, public Description<GuiTextElementOperator>
			{
			public:
				class ICallback : public virtual IDescriptable, public Description<ICallback>
				{
				public:
					virtual TextPos							GetLeftWord(TextPos pos)=0;
					virtual TextPos							GetRightWord(TextPos pos)=0;
					virtual void							GetWord(TextPos pos, TextPos& begin, TextPos& end)=0;
					virtual int								GetPageRows()=0;
					virtual bool							BeforeModify(TextPos start, TextPos end, const WString& originalText, WString& inputText)=0;
					virtual void							AfterModify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)=0;
					virtual void							ScrollToView(Point point)=0;
					virtual int								GetTextMargin()=0;
				};

				class DefaultCallback : public Object, public ICallback, public Description<DefaultCallback>
				{
				protected:
					elements::GuiColorizedTextElement*		textElement;
					compositions::GuiGraphicsComposition*	textComposition;
					bool									readonly;
				public:
					DefaultCallback(elements::GuiColorizedTextElement* _textElement, compositions::GuiGraphicsComposition* _textComposition);
					~DefaultCallback();

					TextPos									GetLeftWord(TextPos pos)override;
					TextPos									GetRightWord(TextPos pos)override;
					void									GetWord(TextPos pos, TextPos& begin, TextPos& end)override;
					int										GetPageRows()override;
					bool									BeforeModify(TextPos start, TextPos end, const WString& originalText, WString& inputText)override;
				};

				class ITextEditCallback : public virtual IDescriptable, public Description<ITextEditCallback>
				{
				public:
					virtual void							Attach(elements::GuiColorizedTextElement* element, SpinLock& elementModifyLock)=0;
					virtual void							Detach()=0;
					virtual void							TextEditNotify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)=0;
				};

			protected:
				elements::GuiColorizedTextElement*			textElement;
				compositions::GuiGraphicsComposition*		textComposition;
				GuiControl*									textControl;
				GuiTextBoxCommonInterface*					textBoxCommonInterface;
				ICallback*									callback;
				bool										dragging;
				bool										readonly;

				SpinLock									elementModifyLock;
				collections::List<Ptr<ITextEditCallback>>	textEditCallbacks;

				void										UpdateCaretPoint();
				void										Move(TextPos pos, bool shift);
				void										Modify(TextPos start, TextPos end, const WString& input);
				void										ProcessKey(int code, bool shift, bool ctrl);
					
				void										OnGotFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnLostFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnCaretNotify(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);

				void										OnLeftButtonDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
				void										OnLeftButtonUp(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
				void										OnMouseMove(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
				void										OnKeyDown(compositions::GuiGraphicsComposition* sender, compositions::GuiKeyEventArgs& arguments);
				void										OnCharInput(compositions::GuiGraphicsComposition* sender, compositions::GuiCharEventArgs& arguments);
			public:
				GuiTextElementOperator();
				~GuiTextElementOperator();

				void										Install(elements::GuiColorizedTextElement* _textElement, compositions::GuiGraphicsComposition* _textComposition, GuiControl* _textControl);
				ICallback*									GetCallback();
				void										SetCallback(ICallback* value);
				bool										AttachTextEditCallback(Ptr<ITextEditCallback> value);
				bool										DetachTextEditCallback(Ptr<ITextEditCallback> value);
				GuiTextBoxCommonInterface*					GetTextBoxCommonInterface();
				void										SetTextBoxCommonInterface(GuiTextBoxCommonInterface* value);

				elements::GuiColorizedTextElement*			GetTextElement();
				compositions::GuiGraphicsComposition*		GetTextComposition();
				TextPos										GetNearestTextPos(Point point);
				void										Select(TextPos begin, TextPos end);
				WString										GetSelectionText();
				void										SetSelectionText(const WString& value);
				void										SetText(const WString& value);

				bool										CanCut();
				bool										CanCopy();
				bool										CanPaste();
				void										SelectAll();
				bool										Cut();
				bool										Copy();
				bool										Paste();

				bool										GetReadonly();
				void										SetReadonly(bool value);
			};

/***********************************************************************
Colorizer
***********************************************************************/
			
			/// <summary>The base class of text box colorizer.</summary>
			class GuiTextBoxColorizerBase : public Object, public GuiTextElementOperator::ITextEditCallback
			{
			public:
				typedef collections::Array<elements::text::ColorEntry>			ColorArray;
			protected:
				elements::GuiColorizedTextElement*			element;
				SpinLock*									elementModifyLock;
				volatile int								colorizedLineCount;
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

				/// <summary>Get the lexical analyzer start state for the first line.</summary>
				/// <returns>The lexical analyzer start state for the first line.</returns>
				virtual int									GetLexerStartState()=0;
				/// <summary>Get the context sensitive start state for the first line.</summary>
				/// <returns>The context sensitive start state for the first line.</returns>
				virtual int									GetContextStartState()=0;
				/// <summary>Colorizer one line with a start state.</summary>
				/// <param name="text">Text buffer.</param>
				/// <param name="colors">Color index buffer. The index should be in [0 .. [M:vl.presentation.controls.GuiTextBoxColorizerBase.GetColors]()-1].</param>
				/// <param name="length">The length of the buffer.</param>
				/// <param name="lexerState">The lexical analyzer state for this line. After executing this function, the new value of this argument indicates the new state.</param>
				/// <param name="contextState">The context sensitive state for this line. After executing this function, the new value of this argument indicates the new state.</param>
				virtual void								ColorizeLineWithCRLF(const wchar_t* text, unsigned __int32* colors, int length, int& lexerState, int& contextState)=0;
				/// <summary>Get the supported colors ordered by their indices.</summary>
				/// <returns>The supported colors ordered by their indices.</returns>
				virtual const ColorArray&					GetColors()=0;
			};

			/// <summary>Regex based colorizer.</summary>
			class GuiTextBoxRegexColorizer : public GuiTextBoxColorizerBase
			{
			protected:
				Ptr<regex::RegexLexer>							lexer;
				Ptr<regex::RegexLexerColorizer>					colorizer;
				ColorArray										colors;

				elements::text::ColorEntry						defaultColor;
				collections::List<WString>						tokenRegexes;
				collections::List<elements::text::ColorEntry>	tokenColors;
				collections::List<elements::text::ColorEntry>	extraTokenColors;

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
				collections::IReadonlyList<WString>&						GetTokenRegexes();
				/// <summary>Get all colors for tokens.</summary>
				/// <returns>All colors for tokens.</returns>
				collections::IReadonlyList<elements::text::ColorEntry>&		GetTokenColors();
				/// <summary>Get all colors for extra tokens.</summary>
				/// <returns>All colors for extra tokens.</returns>
				collections::IReadonlyList<elements::text::ColorEntry>&		GetExtraTokenColors();
				/// <summary>Get the first token index for the first extra token.</summary>
				/// <returns>The first token index for the first extra token. Returns -1 if this operation failed.</returns>
				int															GetExtraTokenIndexStart();
				
				/// <summary>Set the default color. Call [M:vl.presentation.controls.GuiTextBoxRegexColorizer.Setup] after finishing all configuration.</summary>
				/// <returns>Returns the token index of this token. Returns -1 if this operation failed.</returns>
				/// <param name="value">The default color.</param>
				bool														SetDefaultColor(elements::text::ColorEntry value);
				/// <summary>Add a token type. Call [M:vl.presentation.controls.GuiTextBoxRegexColorizer.Setup] after finishing all configuration.</summary>
				/// <returns>Returns the token index of this token. Returns -1 if this operation failed.</returns>
				/// <param name="regex">The regular expression for this token type.</param>
				/// <param name="color">The color for this token type.</param>
				int															AddToken(const WString& regex, elements::text::ColorEntry color);
				/// <summary>Add an extra  token type. Call [M:vl.presentation.controls.GuiTextBoxRegexColorizer.Setup] after finishing all configuration.</summary>
				/// <returns>Returns the extra token index of this token. The token index for this token is regex-token-count + extra-token-index Returns -1 if this operation failed.</returns>
				/// <param name="color">The color for this token type.</param>
				int															AddExtraToken(elements::text::ColorEntry color);
				/// <summary>Setup the colorizer. After that, the colorizer cannot be changed.</summary>
				bool														Setup();
				/// <summary>Callback function to set context sensitive state and change token accordingly.</summary>
				/// <param name="text">Text buffer.</param>
				/// <param name="start">The start position of the token.</param>
				/// <param name="length">The length of the token.</param>
				/// <param name="token">The token type. After executing this function, the new value of this argument indicates the new token type.</param>
				/// <param name="contextState">The context sensitive state. After executing this function, the new value of this argument indicates the new state.</param>
				virtual void												ColorizeTokenContextSensitive(const wchar_t* text, vint start, vint length, vint& token, int& contextState);


				int															GetLexerStartState()override;
				int															GetContextStartState()override;
				void														ColorizeLineWithCRLF(const wchar_t* text, unsigned __int32* colors, int length, int& lexerState, int& contextState)override;
				const ColorArray&											GetColors()override;
			};

/***********************************************************************
Common Interface
***********************************************************************/

			/// <summary>Common interface for text box controls.</summary>
			class GuiTextBoxCommonInterface : public Description<GuiTextBoxCommonInterface>
			{
				friend class GuiTextElementOperator;
			protected:
				GuiTextElementOperator*						textElementOperator;
				GuiControl*									textControl;
				Ptr<GuiTextBoxColorizerBase>				colorizer;

				void										RaiseTextChanged();
				void										RaiseSelectionChanged();
				void										InitializeCommonInterface(GuiControl* _textControl, GuiTextElementOperator* _textElementOperator);
			public:
				GuiTextBoxCommonInterface();
				~GuiTextBoxCommonInterface();

				/// <summary>Selection changed event.</summary>
				compositions::GuiNotifyEvent				SelectionChanged;

				/// <summary>Get a <see cref="compositions::GuiGraphicsComposition"/> that directly owns the <see cref="elements::GuiColorizedTextElement"/> instance.</summary>
				compositions::GuiGraphicsComposition*		GetTextComposition();

				/// <summary>Get the current colorizer.</summary>
				/// <returns>The current colorizer.</returns>
				Ptr<GuiTextBoxColorizerBase>				GetColorizer();
				/// <summary>Set the current colorizer.</summary>
				/// <param name="value">The current colorizer.</param>
				void										SetColorizer(Ptr<GuiTextBoxColorizerBase> value);
				
				/// <summary>Test can the selection be cut.</summary>
				/// <returns>Returns true if the selection can be cut.</returns>
				bool										CanCut();
				/// <summary>Test can the selection be copied.</summary>
				/// <returns>Returns true if the selection can be cut.</returns>
				bool										CanCopy();
				/// <summary>Test can the content in the clipboard be pasted.</summary>
				/// <returns>Returns true if the content in the clipboard can be pasted.</returns>
				bool										CanPaste();
				/// <summary>Select all text.</summary>
				void										SelectAll();
				/// <summary>Cut the selection text.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				bool										Cut();
				/// <summary>Copy the selection text.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				bool										Copy();
				/// <summary>Paste the content from the clipboard and replace the selected text.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				bool										Paste();
				
				/// <summary>Get the text from a specified row number.</summary>
				/// <returns>The text from a specified row number.</returns>
				/// <param name="row">The specified row number.</param>
				WString										GetRowText(int row);
				/// <summary>Get the text from a specified range.</summary>
				/// <returns>The text from a specified range.</returns>
				/// <param name="start">The specified start position.</param>
				/// <param name="end">The specified end position.</param>
				WString										GetFragmentText(TextPos start, TextPos end);
				/// <summary>Get the width of a row.</summary>
				/// <returns>The width of a row in pixel.</returns>
				/// <param name="row">The specified row number</param>
				int											GetRowWidth(int row);
				/// <summary>Get the height of a row.</summary>
				/// <returns>The height of a row in pixel.</returns>
				int											GetRowHeight();
				/// <summary>Get the maximum width of all rows.</summary>
				/// <returns>The maximum width of all rows.</returns>
				int											GetMaxWidth();
				/// <summary>Get the total height of all rows.</summary>
				/// <returns>The total height of all rows.</returns>
				int											GetMaxHeight();
				/// <summary>Get the nearest position of a character from a specified display position.</summary>
				/// <returns>Get the nearest position of a character.</returns>
				/// <param name="point">The specified display position.</param>
				TextPos										GetTextPosFromPoint(Point point);
				/// <summary>Get the display position of a character from a specified text position.</summary>
				/// <returns>Get the display position of a character.</returns>
				/// <param name="pos">The specified text position.</param>
				Point										GetPointFromTextPos(TextPos pos);
				/// <summary>Get the display bounds of a character from a specified text position.</summary>
				/// <returns>Get the display bounds of a character.</returns>
				/// <param name="pos">The specified text position.</param>
				Rect										GetRectFromTextPos(TextPos pos);
				/// <summary>Get the nearest text position from a specified display position.</summary>
				/// <returns>Get the nearest text position.</returns>
				/// <param name="point">The specified display position.</param>
				TextPos										GetNearestTextPos(Point point);

				/// <summary>Get the begin text position of the selection.</summary>
				/// <returns>The begin text position of the selection.</returns>
				TextPos										GetCaretBegin();
				/// <summary>Get the end text position of the selection.</summary>
				/// <returns>The end text position of the selection.</returns>
				TextPos										GetCaretEnd();
				/// <summary>Get the left-top text position of the selection.</summary>
				/// <returns>The left-top text position of the selection.</returns>
				TextPos										GetCaretSmall();
				/// <summary>Get the right-bottom text position of the selection.</summary>
				/// <returns>The right-bottom text position of the selection.</returns>
				TextPos										GetCaretLarge();
				/// <summary>Select (highlight) a part of text.</summary>
				/// <param name="begin">The begin position.</param>
				/// <param name="end">The end position. This is also the caret position.</param>
				void										Select(TextPos begin, TextPos end);
				
				/// <summary>Get the selected text.</summary>
				/// <returns>The selected text.</returns>
				WString										GetSelectionText();
				/// <summary>Set the selected text.</summary>
				/// <param name="value">The selected text.</param>
				void										SetSelectionText(const WString& value);

				/// <summary>Get the readonly mode.</summary>
				/// <returns>Returns true if the text box is readonly.</returns>
				bool										GetReadonly();
				/// <summary>Set the readonly mode.</summary>
				/// <param name="value">Set to true to make the texg box readonly.</param>
				void										SetReadonly(bool value);
			};

/***********************************************************************
MultilineTextBox
***********************************************************************/

			/// <summary>Multiline text box control.</summary>
			class GuiMultilineTextBox : public GuiScrollView, public GuiTextBoxCommonInterface, public Description<GuiMultilineTextBox>
			{
			public:
				static const int							TextMargin=3;

				class StyleController : public GuiScrollView::StyleController, public Description<StyleController>
				{
				protected:
					elements::GuiColorizedTextElement*		textElement;
					compositions::GuiBoundsComposition*		textComposition;
					GuiTextElementOperator					textElementOperator;
					Ptr<GuiTextElementOperator::ICallback>	defaultCallback;

				public:
					StyleController(GuiScrollView::IStyleProvider* styleProvider);
					~StyleController();

					elements::GuiColorizedTextElement*		GetTextElement();
					compositions::GuiGraphicsComposition*	GetTextComposition();
					GuiTextElementOperator*					GetTextElementOperator();
					void									SetViewPosition(Point value);
					void									SetFocusableComposition(compositions::GuiGraphicsComposition* value)override;

					WString									GetText();
					void									SetText(const WString& value)override;
					void									SetFont(const FontProperties& value)override;
					void									SetVisuallyEnabled(bool value)override;
				};

				class TextElementOperatorCallback : public GuiTextElementOperator::DefaultCallback, public Description<TextElementOperatorCallback>
				{
				protected:
					GuiMultilineTextBox*					textControl;
					StyleController*						textController;
				public:
					TextElementOperatorCallback(GuiMultilineTextBox* _textControl);

					void									AfterModify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)override;
					void									ScrollToView(Point point)override;
					int										GetTextMargin()override;
				};

			protected:
				StyleController*							styleController;

				void										CalculateViewAndSetScroll();
				void										OnRenderTargetChanged(elements::IGuiGraphicsRenderTarget* renderTarget)override;
				Size										QueryFullSize()override;
				void										UpdateView(Rect viewBounds)override;
				void										OnBoundsMouseButtonDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
			public:
				/// <summary>Create a control with a specified style provider.</summary>
				/// <param name="styleProvider">The style provider.</param>
				GuiMultilineTextBox(GuiMultilineTextBox::IStyleProvider* styleProvider);
				~GuiMultilineTextBox();

				const WString&								GetText()override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
			};

/***********************************************************************
SinglelineTextBox
***********************************************************************/
			
			/// <summary>Single text box control.</summary>
			class GuiSinglelineTextBox : public GuiControl, public GuiTextBoxCommonInterface, public Description<GuiSinglelineTextBox>
			{
			public:
				static const int							TextMargin=3;

				class IStyleProvider : public virtual GuiControl::IStyleProvider, public Description<IStyleProvider>
				{
				public:
					virtual compositions::GuiGraphicsComposition*		InstallBackground(compositions::GuiBoundsComposition* background)=0;
				};

				class StyleController : public Object, public GuiControl::IStyleController, public Description<StyleController>
				{
				protected:
					Ptr<IStyleProvider>						styleProvider;
					compositions::GuiBoundsComposition*		boundsComposition;
					compositions::GuiGraphicsComposition*	containerComposition;

					GuiSinglelineTextBox*					textBox;
					elements::GuiColorizedTextElement*		textElement;
					compositions::GuiTableComposition*		textCompositionTable;
					compositions::GuiCellComposition*		textComposition;
					GuiTextElementOperator					textElementOperator;
					Ptr<GuiTextElementOperator::ICallback>	defaultCallback;

				public:
					StyleController(IStyleProvider* _styleProvider);
					~StyleController();

					void									SetTextBox(GuiSinglelineTextBox* value);
					void									RearrangeTextElement();
					compositions::GuiBoundsComposition*		GetBoundsComposition();
					compositions::GuiGraphicsComposition*	GetContainerComposition();
					void									SetFocusableComposition(compositions::GuiGraphicsComposition* value);

					WString									GetText();
					void									SetText(const WString& value);
					void									SetFont(const FontProperties& value);
					void									SetVisuallyEnabled(bool value);

					elements::GuiColorizedTextElement*		GetTextElement();
					compositions::GuiGraphicsComposition*	GetTextComposition();
					GuiTextElementOperator*					GetTextElementOperator();
					void									SetViewPosition(Point value);
				};

				class TextElementOperatorCallback : public GuiTextElementOperator::DefaultCallback, public Description<TextElementOperatorCallback>
				{
				protected:
					GuiSinglelineTextBox*					textControl;
					StyleController*						textController;
				public:
					TextElementOperatorCallback(GuiSinglelineTextBox* _textControl);

					bool									BeforeModify(TextPos start, TextPos end, const WString& originalText, WString& inputText)override;
					void									AfterModify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)override;
					void									ScrollToView(Point point)override;
					int										GetTextMargin()override;
				};
			protected:
				StyleController*							styleController;
				
				void										OnRenderTargetChanged(elements::IGuiGraphicsRenderTarget* renderTarget)override;
				void										OnBoundsMouseButtonDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
			public:
				/// <summary>Create a control with a specified style provider.</summary>
				/// <param name="styleProvider">The style provider.</param>
				GuiSinglelineTextBox(GuiSinglelineTextBox::IStyleProvider* styleProvider);
				~GuiSinglelineTextBox();

				const WString&								GetText()override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
			};
		}
	}
}

#endif