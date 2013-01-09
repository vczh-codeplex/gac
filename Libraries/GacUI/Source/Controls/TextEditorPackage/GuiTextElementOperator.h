/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUITEXTELEMENTOPERATOR
#define VCZH_PRESENTATION_CONTROLS_GUITEXTELEMENTOPERATOR

#include "..\GuiContainerControls.h"

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

			/// <summary>Common interface for text-based editor controls.</summary>
			class GuiTextElementOperator : public Description<GuiTextElementOperator>
			{
			protected:
				class ICallback : public virtual IDescriptable, public Description<ICallback>
				{
				public:
					virtual TextPos							GetLeftWord(TextPos pos)=0;
					virtual TextPos							GetRightWord(TextPos pos)=0;
					virtual void							GetWord(TextPos pos, TextPos& begin, TextPos& end)=0;
					virtual vint							GetPageRows()=0;
					virtual bool							BeforeModify(TextPos start, TextPos end, const WString& originalText, WString& inputText)=0;
					virtual void							AfterModify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)=0;
					virtual void							ScrollToView(Point point)=0;
					virtual vint							GetTextMargin()=0;
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
					vint									GetPageRows()override;
					bool									BeforeModify(TextPos start, TextPos end, const WString& originalText, WString& inputText)override;
				};

			public:
				class ShortcutCommand
				{
				protected:
					bool									ctrl;
					bool									shift;
					vint									key;
					Func<void()>							action;
				public:
					ShortcutCommand(bool _ctrl, bool _shift, vint _key, const Func<void()> _action);
					~ShortcutCommand();

					bool									IsTheRightKey(bool _ctrl, bool _shift, vint _key);
					void									Execute();
				};

				class ITextEditCallback : public virtual IDescriptable, public Description<ITextEditCallback>
				{
				public:
					virtual void							Attach(elements::GuiColorizedTextElement* element, SpinLock& elementModifyLock)=0;
					virtual void							Detach()=0;
					virtual void							TextEditNotify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)=0;
				};

			private:
				elements::GuiColorizedTextElement*			textElement;
				compositions::GuiGraphicsComposition*		textComposition;
				GuiControl*									textControl;
				ICallback*									callback;
				bool										dragging;
				bool										readonly;

				SpinLock									elementModifyLock;
				collections::List<Ptr<ITextEditCallback>>	textEditCallbacks;
				collections::List<Ptr<ShortcutCommand>>		shortcutCommands;

				void										UpdateCaretPoint();
				void										Move(TextPos pos, bool shift);
				void										Modify(TextPos start, TextPos end, const WString& input);
				bool										ProcessKey(vint code, bool shift, bool ctrl);
					
				void										OnGotFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnLostFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnCaretNotify(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);

				void										OnLeftButtonDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
				void										OnLeftButtonUp(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
				void										OnMouseMove(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
				void										OnKeyDown(compositions::GuiGraphicsComposition* sender, compositions::GuiKeyEventArgs& arguments);
				void										OnCharInput(compositions::GuiGraphicsComposition* sender, compositions::GuiCharEventArgs& arguments);

			protected:

				void										Install(elements::GuiColorizedTextElement* _textElement, compositions::GuiGraphicsComposition* _textComposition, GuiControl* _textControl);
				ICallback*									GetCallback();
				void										SetCallback(ICallback* value);
				bool										AttachTextEditCallback(Ptr<ITextEditCallback> value);
				bool										DetachTextEditCallback(Ptr<ITextEditCallback> value);
				void										AddShortcutCommand(Ptr<ShortcutCommand> shortcutCommand);
				elements::GuiColorizedTextElement*			GetTextElement();
				void										UnsafeSetText(const WString& value);
			public:
				GuiTextElementOperator();
				~GuiTextElementOperator();

				/// <summary>Selection changed event.</summary>
				compositions::GuiNotifyEvent				SelectionChanged;
				
				/// <summary>Get a <see cref="compositions::GuiGraphicsComposition"/> that directly owns the <see cref="elements::GuiColorizedTextElement"/> instance.</summary>
				compositions::GuiGraphicsComposition*		GetTextComposition();

				//================ clipboard operations

				/// <summary>Test can the selection be cut.</summary>
				/// <returns>Returns true if the selection can be cut.</returns>
				bool										CanCut();
				/// <summary>Test can the selection be copied.</summary>
				/// <returns>Returns true if the selection can be cut.</returns>
				bool										CanCopy();
				/// <summary>Test can the content in the clipboard be pasted.</summary>
				/// <returns>Returns true if the content in the clipboard can be pasted.</returns>
				bool										CanPaste();
				/// <summary>Cut the selection text.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				bool										Cut();
				/// <summary>Copy the selection text.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				bool										Copy();
				/// <summary>Paste the content from the clipboard and replace the selected text.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				bool										Paste();

				//================ editing control

				/// <summary>Get the readonly mode.</summary>
				/// <returns>Returns true if the text box is readonly.</returns>
				bool										GetReadonly();
				/// <summary>Set the readonly mode.</summary>
				/// <param name="value">Set to true to make the texg box readonly.</param>
				void										SetReadonly(bool value);

				//================ text operations

				/// <summary>Select all text.</summary>
				void										SelectAll();
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
				
				/// <summary>Get the text from a specified row number.</summary>
				/// <returns>The text from a specified row number.</returns>
				/// <param name="row">The specified row number.</param>
				WString										GetRowText(vint row);
				/// <summary>Get the text from a specified range.</summary>
				/// <returns>The text from a specified range.</returns>
				/// <param name="start">The specified start position.</param>
				/// <param name="end">The specified end position.</param>
				WString										GetFragmentText(TextPos start, TextPos end);

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

				//================ position query

				/// <summary>Get the width of a row.</summary>
				/// <returns>The width of a row in pixel.</returns>
				/// <param name="row">The specified row number</param>
				vint										GetRowWidth(vint row);
				/// <summary>Get the height of a row.</summary>
				/// <returns>The height of a row in pixel.</returns>
				vint										GetRowHeight();
				/// <summary>Get the maximum width of all rows.</summary>
				/// <returns>The maximum width of all rows.</returns>
				vint										GetMaxWidth();
				/// <summary>Get the total height of all rows.</summary>
				/// <returns>The total height of all rows.</returns>
				vint										GetMaxHeight();
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
			};
		}
	}
}

#endif