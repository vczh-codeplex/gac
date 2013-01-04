/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUITEXTCOMMONINTERFACE
#define VCZH_PRESENTATION_CONTROLS_GUITEXTCOMMONINTERFACE

#include "GuiTextElementOperator.h"
#include "GuiTextColorizer.h"
#include "GuiTextUndoRedo.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

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
				Ptr<GuiTextBoxUndoRedoProcessor>			undoRedoProcessor;

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

				/// <summary>Test can undo.</summary>
				/// <returns>Returns true if this action can be performed.</returns>
				bool										CanUndo();
				/// <summary>Test can redo.</summary>
				/// <returns>Returns true if this action can be performed.</returns>
				bool										CanRedo();
				/// <summary>Clear all undo and redo information.</summary>
				void										ClearUndoRedo();
				/// <summary>Test is the text box modified.</summary>
				/// <returns>Returns true if the text box is modified.</returns>
				bool										GetModified();
				/// <summary>Notify the text box that the current status is considered saved.</summary>
				void										NotifyModificationSaved();
				/// <summary>Perform the undo action.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				bool										Undo();
				/// <summary>Perform the redo action.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				bool										Redo();
				
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
				WString										GetRowText(vint row);
				/// <summary>Get the text from a specified range.</summary>
				/// <returns>The text from a specified range.</returns>
				/// <param name="start">The specified start position.</param>
				/// <param name="end">The specified end position.</param>
				WString										GetFragmentText(TextPos start, TextPos end);
				/// <summary>Get the width of a row.</summary>
				/// <returns>The width of a row in pixel.</returns>
				/// <param name="row">The specified row number</param>
				vint											GetRowWidth(vint row);
				/// <summary>Get the height of a row.</summary>
				/// <returns>The height of a row in pixel.</returns>
				vint											GetRowHeight();
				/// <summary>Get the maximum width of all rows.</summary>
				/// <returns>The maximum width of all rows.</returns>
				vint											GetMaxWidth();
				/// <summary>Get the total height of all rows.</summary>
				/// <returns>The total height of all rows.</returns>
				vint											GetMaxHeight();
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
		}
	}
}

#endif