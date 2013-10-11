/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUIDOCUMENTVIEWER
#define VCZH_PRESENTATION_CONTROLS_GUIDOCUMENTVIEWER

#include "..\GuiContainerControls.h"

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
				elements::GuiDocumentElement*				documentElement;
				compositions::GuiBoundsComposition*			documentComposition;
				vint										activeHyperlinkId;
				vint										draggingHyperlinkId;
				bool										dragging;
				EditMode									editMode;
				GuiControl*									senderControl;

				void										Move(TextPos caret, bool shift, bool frontSide);
				bool										ProcessKey(vint code, bool shift, bool ctrl);
				void										InstallDocumentViewer(GuiControl* _sender, compositions::GuiGraphicsComposition* _container);
				void										SetActiveHyperlinkId(vint value);
				void										OnCaretNotify(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnGotFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnLostFocus(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void										OnKeyDown(compositions::GuiGraphicsComposition* sender, compositions::GuiKeyEventArgs& arguments);
				void										OnCharInput(compositions::GuiGraphicsComposition* sender, compositions::GuiCharEventArgs& arguments);
				void										OnMouseMove(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
				void										OnMouseDown(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
				void										OnMouseUp(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
				void										OnMouseLeave(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);

				virtual void								EnsureRectVisible(Rect bounds);
			public:
				GuiDocumentCommonInterface();
				~GuiDocumentCommonInterface();

				/// <summary>Active hyperlink changed event.</summary>
				compositions::GuiNotifyEvent				ActiveHyperlinkChanged;
				/// <summary>Active hyperlink executed event.</summary>
				compositions::GuiNotifyEvent				ActiveHyperlinkExecuted;
				
				/// <summary>Get the document.</summary>
				/// <returns>The document.</returns>
				Ptr<DocumentModel>							GetDocument();
				/// <summary>Set the document. When a document is set to this element, modifying the document without invoking <see cref="NotifyParagraphUpdated"/> will lead to undefined behavior.</summary>
				/// <param name="value">The document.</param>
				void										SetDocument(Ptr<DocumentModel> value);

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

				/// <summary>Notify that some paragraphs are updated.</summary>
				/// <param name="index">The start paragraph index.</param>
				/// <param name="oldCount">The number of paragraphs to be updated.</param>
				/// <param name="newCount">The number of updated paragraphs.</param>
				/// <param name="updatedText">Set to true to notify that the text is updated.</param>
				void										NotifyParagraphUpdated(vint index, vint oldCount, vint newCount, bool updatedText);
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
				/// <summary>Get the active hyperlink id. Returns -1 indicates there is no active hyperlink.</summary>
				/// <returns>The active hyperlink id.</returns>
				vint										GetActiveHyperlinkId();
				/// <summary>Get the href attribute of the active hyperlink.</summary>
				/// <returns>The href attribute of the active hyperlink.</returns>
				WString										GetActiveHyperlinkReference();
				/// <summary>Get the edit mode of this control.</summary>
				/// <returns>The edit mode.</returns>
				EditMode									GetEditMode();
				/// <summary>Set the edit mode of this control.</summary>
				/// <param name="value">The edit mode.</param>
				void										SetEditMode(EditMode value);
			};

/***********************************************************************
GuiDocumentViewer
***********************************************************************/
			
			/// <summary>Scrollable document viewer for displaying <see cref="DocumentModel"/>.</summary>
			class GuiDocumentViewer : public GuiScrollContainer, public GuiDocumentCommonInterface, public Description<GuiDocumentViewer>
			{
			protected:

				void										EnsureRectVisible(Rect bounds)override;
			public:
				/// <summary>Create a control with a specified style provider.</summary>
				/// <param name="styleProvider">The style provider.</param>
				GuiDocumentViewer(GuiDocumentViewer::IStyleProvider* styleProvider);
				~GuiDocumentViewer();
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
			};
		}
	}
}

#endif