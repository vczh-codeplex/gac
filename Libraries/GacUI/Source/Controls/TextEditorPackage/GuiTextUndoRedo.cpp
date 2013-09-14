#include "GuiTextUndoRedo.h"
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
GuiGeneralUndoRedoProcessor
***********************************************************************/

			GuiGeneralUndoRedoProcessor::GuiGeneralUndoRedoProcessor()
				:firstFutureStep(0)
				,savedStep(0)
				,performingUndoRedo(false)
			{
			}

			GuiGeneralUndoRedoProcessor::~GuiGeneralUndoRedoProcessor()
			{
			}

			void GuiGeneralUndoRedoProcessor::PushStep(Ptr<IEditStep> step)
			{
				if(!performingUndoRedo)
				{
					if(firstFutureStep<savedStep)
					{
						savedStep=-1;
					}

					vint count=steps.Count()-firstFutureStep;
					if(count>0)
					{
						steps.RemoveRange(firstFutureStep, count);
					}
				
					steps.Add(step);
					firstFutureStep=steps.Count();
				}
			}

			bool GuiGeneralUndoRedoProcessor::CanUndo()
			{
				return firstFutureStep>0;
			}

			bool GuiGeneralUndoRedoProcessor::CanRedo()
			{
				return steps.Count()>firstFutureStep;
			}

			void GuiGeneralUndoRedoProcessor::ClearUndoRedo()
			{
				if(!performingUndoRedo)
				{
					steps.Clear();
					firstFutureStep=0;
					savedStep=-1;
				}
			}

			bool GuiGeneralUndoRedoProcessor::GetModified()
			{
				return firstFutureStep!=savedStep;
			}

			void GuiGeneralUndoRedoProcessor::NotifyModificationSaved()
			{
				if(!performingUndoRedo)
				{
					savedStep=firstFutureStep;
				}
			}

			bool GuiGeneralUndoRedoProcessor::Undo()
			{
				if(!CanUndo()) return false;
				performingUndoRedo=true;
				firstFutureStep--;
				steps[firstFutureStep]->Undo();
				performingUndoRedo=false;
				return true;
			}

			bool GuiGeneralUndoRedoProcessor::Redo()
			{
				if(!CanRedo()) return false;
				performingUndoRedo=true;
				firstFutureStep++;
				steps[firstFutureStep-1]->Redo();
				performingUndoRedo=false;
				return true;
			}

/***********************************************************************
GuiTextBoxUndoRedoProcessor::EditStep
***********************************************************************/

			void GuiTextBoxUndoRedoProcessor::EditStep::Undo()
			{
				GuiTextBoxCommonInterface* ci=dynamic_cast<GuiTextBoxCommonInterface*>(processor->ownerComposition->GetRelatedControl());
				if(ci)
				{
					ci->Select(arguments.inputStart, arguments.inputEnd);
					ci->SetSelectionText(arguments.originalText);
					ci->Select(arguments.originalStart, arguments.originalEnd);
				}
			}

			void GuiTextBoxUndoRedoProcessor::EditStep::Redo()
			{
				GuiTextBoxCommonInterface* ci=dynamic_cast<GuiTextBoxCommonInterface*>(processor->ownerComposition->GetRelatedControl());
				if(ci)
				{
					ci->Select(arguments.originalStart, arguments.originalEnd);
					ci->SetSelectionText(arguments.inputText);
					ci->Select(arguments.inputStart, arguments.inputEnd);
				}
			}

/***********************************************************************
GuiTextBoxUndoRedoProcessor
***********************************************************************/

			GuiTextBoxUndoRedoProcessor::GuiTextBoxUndoRedoProcessor()
				:ownerComposition(0)
			{
			}

			GuiTextBoxUndoRedoProcessor::~GuiTextBoxUndoRedoProcessor()
			{
			}

			void GuiTextBoxUndoRedoProcessor::Attach(elements::GuiColorizedTextElement* element, SpinLock& elementModifyLock, compositions::GuiGraphicsComposition* _ownerComposition, vuint editVersion)
			{
				ownerComposition=_ownerComposition;
			}

			void GuiTextBoxUndoRedoProcessor::Detach()
			{
				ClearUndoRedo();
			}

			void GuiTextBoxUndoRedoProcessor::TextEditNotify(const TextEditNotifyStruct& arguments)
			{
				Ptr<EditStep> step=new EditStep;
				step->processor=this;
				step->arguments=arguments;
				PushStep(step);
			}

			void GuiTextBoxUndoRedoProcessor::TextCaretChanged(const TextCaretChangedStruct& arguments)
			{
			}

			void GuiTextBoxUndoRedoProcessor::TextEditFinished(vuint editVersion)
			{
			}
		}
	}
}