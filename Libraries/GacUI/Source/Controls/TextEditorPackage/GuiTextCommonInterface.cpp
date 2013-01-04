#include "GuiTextCommonInterface.h"

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
GuiTextBoxCommonInterface
***********************************************************************/

			void GuiTextBoxCommonInterface::RaiseTextChanged()
			{
				textControl->TextChanged.Execute(textControl->GetNotifyEventArguments());
			}

			void GuiTextBoxCommonInterface::RaiseSelectionChanged()
			{
				SelectionChanged.Execute(textControl->GetNotifyEventArguments());
			}

			void GuiTextBoxCommonInterface::InitializeCommonInterface(GuiControl* _textControl, GuiTextElementOperator* _textElementOperator)
			{
				textElementOperator=_textElementOperator;
				textControl=_textControl;
				SelectionChanged.SetAssociatedComposition(textControl->GetBoundsComposition());
				textElementOperator->SetTextBoxCommonInterface(this);

				undoRedoProcessor=new GuiTextBoxUndoRedoProcessor(textElementOperator);
				textElementOperator->AttachTextEditCallback(undoRedoProcessor);

				textElementOperator->AddShortcutCommand(new GuiTextElementOperator::ShortcutCommand(true, false, 'A', Func<void()>(this, &GuiTextBoxCommonInterface::SelectAll)));
				textElementOperator->AddShortcutCommand(new GuiTextElementOperator::ShortcutCommand(true, false, 'Z', Func<void()>(Func<bool()>(this, &GuiTextBoxCommonInterface::Undo))));
				textElementOperator->AddShortcutCommand(new GuiTextElementOperator::ShortcutCommand(true, false, 'Y', Func<void()>(Func<bool()>(this, &GuiTextBoxCommonInterface::Redo))));
				textElementOperator->AddShortcutCommand(new GuiTextElementOperator::ShortcutCommand(true, false, 'X', Func<void()>(Func<bool()>(this, &GuiTextBoxCommonInterface::Cut))));
				textElementOperator->AddShortcutCommand(new GuiTextElementOperator::ShortcutCommand(true, false, 'C', Func<void()>(Func<bool()>(this, &GuiTextBoxCommonInterface::Copy))));
				textElementOperator->AddShortcutCommand(new GuiTextElementOperator::ShortcutCommand(true, false, 'V', Func<void()>(Func<bool()>(this, &GuiTextBoxCommonInterface::Paste))));
			}

			GuiTextBoxCommonInterface::GuiTextBoxCommonInterface()
				:textElementOperator(0)
				,textControl(0)
			{
			}

			GuiTextBoxCommonInterface::~GuiTextBoxCommonInterface()
			{
				if(colorizer)
				{
					textElementOperator->DetachTextEditCallback(colorizer);
					colorizer=0;
				}
				if(undoRedoProcessor)
				{
					textElementOperator->DetachTextEditCallback(undoRedoProcessor);
					undoRedoProcessor=0;
				}
			}

			compositions::GuiGraphicsComposition* GuiTextBoxCommonInterface::GetTextComposition()
			{
				return textElementOperator->GetTextComposition();
			}

			Ptr<GuiTextBoxColorizerBase> GuiTextBoxCommonInterface::GetColorizer()
			{
				return colorizer;
			}

			void GuiTextBoxCommonInterface::SetColorizer(Ptr<GuiTextBoxColorizerBase> value)
			{
				if(textElementOperator)
				{
					if(colorizer)
					{
						textElementOperator->DetachTextEditCallback(colorizer);
					}
					colorizer=value;
					if(colorizer)
					{
						textElementOperator->AttachTextEditCallback(colorizer);
						textElementOperator->GetTextElement()->SetColors(colorizer->GetColors());
					}
				}
			}

			bool GuiTextBoxCommonInterface::CanUndo()
			{
				return undoRedoProcessor->CanUndo();
			}

			bool GuiTextBoxCommonInterface::CanRedo()
			{
				return undoRedoProcessor->CanRedo();
			}

			void GuiTextBoxCommonInterface::ClearUndoRedo()
			{
				undoRedoProcessor->ClearUndoRedo();
			}

			bool GuiTextBoxCommonInterface::GetModified()
			{
				return undoRedoProcessor->GetModified();
			}

			void GuiTextBoxCommonInterface::NotifyModificationSaved()
			{
				undoRedoProcessor->NotifyModificationSaved();
			}

			bool GuiTextBoxCommonInterface::Undo()
			{
				return undoRedoProcessor->Undo();
			}

			bool GuiTextBoxCommonInterface::Redo()
			{
				return undoRedoProcessor->Redo();
			}

			bool GuiTextBoxCommonInterface::CanCut()
			{
				return textElementOperator->CanCut();
			}

			bool GuiTextBoxCommonInterface::CanCopy()
			{
				return textElementOperator->CanCopy();
			}

			bool GuiTextBoxCommonInterface::CanPaste()
			{
				return textElementOperator->CanPaste();
			}

			void GuiTextBoxCommonInterface::SelectAll()
			{
				textElementOperator->SelectAll();
			}

			bool GuiTextBoxCommonInterface::Cut()
			{
				return textElementOperator->Cut();
			}

			bool GuiTextBoxCommonInterface::Copy()
			{
				return textElementOperator->Copy();
			}

			bool GuiTextBoxCommonInterface::Paste()
			{
				return textElementOperator->Paste();
			}

			WString GuiTextBoxCommonInterface::GetRowText(vint row)
			{
				TextPos start=textElementOperator->GetTextElement()->GetLines().Normalize(TextPos(row, 0));
				TextPos end=TextPos(start.row, textElementOperator->GetTextElement()->GetLines().GetLine(start.row).dataLength);
				return GetFragmentText(start, end);
			}

			WString GuiTextBoxCommonInterface::GetFragmentText(TextPos start, TextPos end)
			{
				start=textElementOperator->GetTextElement()->GetLines().Normalize(start);
				end=textElementOperator->GetTextElement()->GetLines().Normalize(end);
				return textElementOperator->GetTextElement()->GetLines().GetText(start, end);
			}
				
			vint GuiTextBoxCommonInterface::GetRowWidth(vint row)
			{
				return textElementOperator->GetTextElement()->GetLines().GetRowWidth(row);
			}

			vint GuiTextBoxCommonInterface::GetRowHeight()
			{
				return textElementOperator->GetTextElement()->GetLines().GetRowHeight();
			}

			vint GuiTextBoxCommonInterface::GetMaxWidth()
			{
				return textElementOperator->GetTextElement()->GetLines().GetMaxWidth();
			}

			vint GuiTextBoxCommonInterface::GetMaxHeight()
			{
				return textElementOperator->GetTextElement()->GetLines().GetMaxHeight();
			}

			TextPos GuiTextBoxCommonInterface::GetTextPosFromPoint(Point point)
			{
				Point view=textElementOperator->GetTextElement()->GetViewPosition();
				return textElementOperator->GetTextElement()->GetLines().GetTextPosFromPoint(Point(point.x+view.x, point.y+view.y));
			}

			Point GuiTextBoxCommonInterface::GetPointFromTextPos(TextPos pos)
			{
				Point view=textElementOperator->GetTextElement()->GetViewPosition();
				Point result=textElementOperator->GetTextElement()->GetLines().GetPointFromTextPos(pos);
				return Point(result.x-view.x, result.y-view.y);
			}

			Rect GuiTextBoxCommonInterface::GetRectFromTextPos(TextPos pos)
			{
				Point view=textElementOperator->GetTextElement()->GetViewPosition();
				Rect result=textElementOperator->GetTextElement()->GetLines().GetRectFromTextPos(pos);
				return Rect(Point(result.x1-view.x, result.y1-view.y), result.GetSize());
			}

			TextPos GuiTextBoxCommonInterface::GetNearestTextPos(Point point)
			{
				return textElementOperator->GetNearestTextPos(point);
			}

			TextPos GuiTextBoxCommonInterface::GetCaretBegin()
			{
				return textElementOperator->GetTextElement()->GetCaretBegin();
			}

			TextPos GuiTextBoxCommonInterface::GetCaretEnd()
			{
				return textElementOperator->GetTextElement()->GetCaretEnd();
			}

			TextPos GuiTextBoxCommonInterface::GetCaretSmall()
			{
				TextPos c1=GetCaretBegin();
				TextPos c2=GetCaretBegin();
				return c1<c2?c1:c2;
			}

			TextPos GuiTextBoxCommonInterface::GetCaretLarge()
			{
				TextPos c1=GetCaretBegin();
				TextPos c2=GetCaretBegin();
				return c1>c2?c1:c2;
			}

			void GuiTextBoxCommonInterface::Select(TextPos begin, TextPos end)
			{
				begin=textElementOperator->GetTextElement()->GetLines().Normalize(begin);
				end=textElementOperator->GetTextElement()->GetLines().Normalize(end);
				textElementOperator->Select(begin, end);
			}

			WString GuiTextBoxCommonInterface::GetSelectionText()
			{
				return textElementOperator->GetSelectionText();
			}

			void GuiTextBoxCommonInterface::SetSelectionText(const WString& value)
			{
				return textElementOperator->SetSelectionText(value);
			}

			bool GuiTextBoxCommonInterface::GetReadonly()
			{
				return textElementOperator->GetReadonly();
			}

			void GuiTextBoxCommonInterface::SetReadonly(bool value)
			{
				textElementOperator->SetReadonly(value);
			}
		}
	}
}