/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUIDOCUMENTVIEWER
#define VCZH_PRESENTATION_CONTROLS_GUIDOCUMENTVIEWER

#include "../GuiContainerControls.h"
#include "GuiTextUndoRedo.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
GuiDocumentCommonInterface
***********************************************************************/
			
			/// <summary>Document displayer control common interface for displaying <see cref="DocumentModel"/>.</summary>
			class GuiDocumentCommonInterface abstract : public Description<GuiDocumentCommonInterface>
			{
			public:
				/// <summary>Represents the edit mode.</summary>
				enum EditMode
				{
					/// <summary>[T:vl.presentation.controls.GuiDocumentCommonInterface.EditMode]View the rich text only.</summary>
					ViewOnly,
					/// <summary>[T:vl.presentation.controls.GuiDocumentCommonInterface.EditMode]The rich text is selectable.</summary>
					Selectable,
					/// <summary>[T:vl.presentation.controls.GuiDocumentCommonInterface.EditMode]The rich text is editable.</summary>
					Editable,
				};
			protected:
				GuiControl*									documentControl;
				elements::GuiDocumentElement*				documentElement;
				compositions::GuiBoundsComposition*			documentComposition;
				Ptr<DocumentHyperlinkRun>					activeHyperlink;
				vint										activeHyperlinkParagraph;
				bool										dragging;
				EditMode									editMode;

				Ptr<GuiDocumentUndoRedoProcessor>			undoRedoProcessor;
				Ptr<compositions::GuiShortcutKeyManager>	internalShortcutKeyManager;

				void										UpdateCaretPoint();
				void										Move(TextPos caret, bool shift, bool frontSide);
				bool										ProcessKey(vint code, bool shift, bool ctrl);
				void										InstallDocumentViewer(GuiControl* _sender, compositions::GuiGraphicsComposition* _container);
				void										SetActiveHyperlink(Ptr<DocumentHyperlinkRun> hyperlink, vint paragraphIndex=-1);
				void										ActivateActiveHyperlink(bool activate);
				void										AddShortcutCommand(vint key, const Func<void()>& eventHandler);
				void										EditTextInternal(TextPos begin, TextPos end, const Func<void(TextPos, TextPos, vint&, vint&)>& editor);
				void										EditStyleInternal(TextPos begin, TextPos end, const Func<void(TextPos, TextPos)>& editor);

				void										OnCaretNotify(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnGotFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnLostFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnKeyDown(compositions::GuiGraphicsComposition* sender, compositions::GuiKeyEventArgs& arguments);
				void										OnCharInput(compositions::GuiGraphicsComposition* sender, compositions::GuiCharEventArgs& arguments);
				void										OnMouseMove(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
				void										OnMouseDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
				void										OnMouseUp(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
				void										OnMouseLeave(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);

				virtual Point								GetDocumentViewPosition();
				virtual void								EnsureRectVisible(Rect bounds);
			public:
				GuiDocumentCommonInterface();
				~GuiDocumentCommonInterface();

				/// <summary>Active hyperlink changed event.</summary>
				compositions::GuiNotifyEvent				ActiveHyperlinkChanged;
				/// <summary>Active hyperlink executed event.</summary>
				compositions::GuiNotifyEvent				ActiveHyperlinkExecuted;

				/// <summary>Selection changed event.</summary>
				compositions::GuiNotifyEvent				SelectionChanged;
				
				/// <summary>Get the document.</summary>
				/// <returns>The document.</returns>
				Ptr<DocumentModel>							GetDocument();
				/// <summary>Set the document. When a document is set to this element, modifying the document without invoking <see cref="NotifyParagraphUpdated"/> will lead to undefined behavior.</summary>
				/// <param name="value">The document.</param>
				void										SetDocument(Ptr<DocumentModel> value);

				//================ caret operations

				/// <summary>
				/// Get the begin position of the selection area.
				/// </summary>
				/// <returns>The begin position of the selection area.</returns>
				TextPos										GetCaretBegin();
				/// <summary>
				/// Get the end position of the selection area.
				/// </summary>
				/// <returns>The end position of the selection area.</returns>
				TextPos										GetCaretEnd();
				/// <summary>
				/// Set the end position of the selection area.
				/// </summary>
				/// <param name="begin">The begin position of the selection area.</param>
				/// <param name="end">The end position of the selection area.</param>
				void										SetCaret(TextPos begin, TextPos end);
				/// <summary>Calculate a caret using a specified point.</summary>
				/// <returns>The calculated caret.</returns>
				/// <param name="point">The specified point.</param>
				TextPos										CalculateCaretFromPoint(Point point);
				/// <summary>Get the bounds of a caret.</summary>
				/// <returns>The bounds.</returns>
				/// <param name="caret">The caret.</param>
				/// <param name="frontSide">Set to true to get the bounds for the character before it.</param>
				Rect										GetCaretBounds(TextPos caret, bool frontSide);

				//================ editing operations

				/// <summary>Notify that some paragraphs are updated.</summary>
				/// <param name="index">The start paragraph index.</param>
				/// <param name="oldCount">The number of paragraphs to be updated.</param>
				/// <param name="newCount">The number of updated paragraphs.</param>
				/// <param name="updatedText">Set to true to notify that the text is updated.</param>
				void										NotifyParagraphUpdated(vint index, vint oldCount, vint newCount, bool updatedText);
				/// <summary>Edit run in a specified range.</summary>
				/// <param name="begin">The begin position of the range.</param>
				/// <param name="end">The end position of the range.</param>
				/// <param name="model">The new run.</param>
				void										EditRun(TextPos begin, TextPos end, Ptr<DocumentModel> model);
				/// <summary>Edit text in a specified range.</summary>
				/// <param name="begin">The begin position of the range.</param>
				/// <param name="end">The end position of the range.</param>
				/// <param name="frontSide">Set to true to use the text style in front of the specified range.</param>
				/// <param name="text">The new text.</param>
				void										EditText(TextPos begin, TextPos end, bool frontSide, const collections::Array<WString>& text);
				/// <summary>Edit style in a specified range.</summary>
				/// <param name="begin">The begin position of the range.</param>
				/// <param name="end">The end position of the range.</param>
				/// <param name="style">The new style.</param>
				void										EditStyle(TextPos begin, TextPos end, Ptr<DocumentStyleProperties> style);
				/// <summary>Edit image in a specified range.</summary>
				/// <param name="begin">The begin position of the range.</param>
				/// <param name="end">The end position of the range.</param>
				/// <param name="image">The new image.</param>
				void										EditImage(TextPos begin, TextPos end, Ptr<GuiImageData> image);
				/// <summary>Set hyperlink in a specified range.</summary>
				/// <param name="paragraphIndex">The index of the paragraph to edit.</param>
				/// <param name="begin">The begin position of the range.</param>
				/// <param name="end">The end position of the range.</param>
				/// <param name="reference">The reference of the hyperlink.</param>
				/// <param name="normalStyleName">The normal style name of the hyperlink.</param>
				/// <param name="activeStyleName">The active style name of the hyperlink.</param>
				void										EditHyperlink(vint paragraphIndex, vint begin, vint end, const WString& reference, const WString& normalStyleName=DocumentModel::NormalLinkStyleName, const WString& activeStyleName=DocumentModel::ActiveLinkStyleName);
				/// <summary>Remove hyperlink in a specified range.</summary>
				/// <param name="paragraphIndex">The index of the paragraph to edit.</param>
				/// <param name="begin">The begin position of the range.</param>
				/// <param name="end">The end position of the range.</param>
				void										RemoveHyperlink(vint paragraphIndex, vint begin, vint end);
				/// <summary>Edit style name in a specified range.</summary>
				/// <param name="begin">The begin position of the range.</param>
				/// <param name="end">The end position of the range.</param>
				/// <param name="styleName">The new style name.</param>
				void										EditStyleName(TextPos begin, TextPos end, const WString& styleName);
				/// <summary>Remove style name in a specified range.</summary>
				/// <param name="begin">The begin position of the range.</param>
				/// <param name="end">The end position of the range.</param>
				void										RemoveStyleName(TextPos begin, TextPos end);
				/// <summary>Rename a style.</summary>
				/// <param name="oldStyleName">The name of the style.</param>
				/// <param name="newStyleName">The new name.</param>
				void										RenameStyle(const WString& oldStyleName, const WString& newStyleName);
				/// <summary>Clear all styles in a specified range.</summary>
				/// <param name="begin">The begin position of the range.</param>
				/// <param name="end">The end position of the range.</param>
				void										ClearStyle(TextPos begin, TextPos end);
				/// <summary>Summarize the text style in a specified range.</summary>
				/// <returns>The text style summary.</returns>
				/// <param name="begin">The begin position of the range.</param>
				/// <param name="end">The end position of the range.</param>
				Ptr<DocumentStyleProperties>				SummarizeStyle(TextPos begin, TextPos end);
				/// <summary>Set the alignment of paragraphs in a specified range.</summary>
				/// <param name="begin">The begin position of the range.</param>
				/// <param name="end">The end position of the range.</param>
				/// <param name="alignments">The alignment for each paragraph.</param>
				void										SetParagraphAlignment(TextPos begin, TextPos end, const collections::Array<Nullable<Alignment>>& alignments);

				//================ editing control

				/// <summary>Get the href attribute of the active hyperlink.</summary>
				/// <returns>The href attribute of the active hyperlink.</returns>
				WString										GetActiveHyperlinkReference();
				/// <summary>Get the edit mode of this control.</summary>
				/// <returns>The edit mode.</returns>
				EditMode									GetEditMode();
				/// <summary>Set the edit mode of this control.</summary>
				/// <param name="value">The edit mode.</param>
				void										SetEditMode(EditMode value);

				//================ selection operations

				/// <summary>Select all text.</summary>
				void										SelectAll();
				/// <summary>Get the selected text.</summary>
				/// <returns>The selected text.</returns>
				WString										GetSelectionText();
				/// <summary>Set the selected text.</summary>
				/// <param name="value">The selected text.</param>
				void										SetSelectionText(const WString& value);
				/// <summary>Get the selected model.</summary>
				/// <returns>The selected model.</returns>
				Ptr<DocumentModel>							GetSelectionModel();
				/// <summary>Set the selected model.</summary>
				/// <param name="value">The selected model.</param>
				void										SetSelectionModel(Ptr<DocumentModel> value);

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

				//================ undo redo control

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
			};

/***********************************************************************
GuiDocumentViewer
***********************************************************************/
			
			/// <summary>Scrollable document viewer for displaying <see cref="DocumentModel"/>.</summary>
			class GuiDocumentViewer : public GuiScrollContainer, public GuiDocumentCommonInterface, public Description<GuiDocumentViewer>
			{
			protected:

				Point										GetDocumentViewPosition()override;
				void										EnsureRectVisible(Rect bounds)override;
			public:
				/// <summary>Create a control with a specified style provider.</summary>
				/// <param name="styleProvider">The style provider.</param>
				GuiDocumentViewer(GuiDocumentViewer::IStyleProvider* styleProvider);
				~GuiDocumentViewer();

				const WString&								GetText()override;
				void										SetText(const WString& value)override;
			};

/***********************************************************************
GuiDocumentViewer
***********************************************************************/
			
			/// <summary>Static document viewer for displaying <see cref="DocumentModel"/>.</summary>
			class GuiDocumentLabel : public GuiControl, public GuiDocumentCommonInterface, public Description<GuiDocumentLabel>
			{
			public:
				/// <summary>Create a control with a specified style controller.</summary>
				/// <param name="styleController">The style controller.</param>
				GuiDocumentLabel(GuiDocumentLabel::IStyleController* styleController);
				~GuiDocumentLabel();
				
				const WString&								GetText()override;
				void										SetText(const WString& value)override;
			};
		}
	}
}

#endif