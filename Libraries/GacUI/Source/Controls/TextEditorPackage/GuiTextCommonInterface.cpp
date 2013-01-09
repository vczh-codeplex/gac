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

			GuiTextBoxCommonInterface::GuiTextBoxCommonInterface()
			{
				undoRedoProcessor=new GuiTextBoxUndoRedoProcessor(this);
				AttachTextEditCallback(undoRedoProcessor);

				AddShortcutCommand(new GuiTextElementOperator::ShortcutCommand(true, false, 'Z', Func<void()>(Func<bool()>(this, &GuiTextBoxCommonInterface::Undo))));
				AddShortcutCommand(new GuiTextElementOperator::ShortcutCommand(true, false, 'Y', Func<void()>(Func<bool()>(this, &GuiTextBoxCommonInterface::Redo))));
			}

			GuiTextBoxCommonInterface::~GuiTextBoxCommonInterface()
			{
				if(colorizer)
				{
					DetachTextEditCallback(colorizer);
					colorizer=0;
				}
				if(undoRedoProcessor)
				{
					DetachTextEditCallback(undoRedoProcessor);
					undoRedoProcessor=0;
				}
			}

			Ptr<GuiTextBoxColorizerBase> GuiTextBoxCommonInterface::GetColorizer()
			{
				return colorizer;
			}

			void GuiTextBoxCommonInterface::SetColorizer(Ptr<GuiTextBoxColorizerBase> value)
			{
				if(colorizer)
				{
					DetachTextEditCallback(colorizer);
				}
				colorizer=value;
				if(colorizer)
				{
					AttachTextEditCallback(colorizer);
					GetTextElement()->SetColors(colorizer->GetColors());
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
		}
	}
}