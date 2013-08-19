/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUITEXTUNDOREDO
#define VCZH_PRESENTATION_CONTROLS_GUITEXTUNDOREDO

#include "GuiTextGeneralOperations.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

			class GuiTextBoxCommonInterface;

/***********************************************************************
Undo Redo
***********************************************************************/

			class GuiGeneralUndoRedoProcessor : public Object
			{
			protected:
				class IEditStep : public Interface
				{
				public:
					virtual void							Undo()=0;
					virtual void							Redo()=0;
				};
				friend class collections::ArrayBase<Ptr<IEditStep>>;

			protected:
				collections::List<Ptr<IEditStep>>			steps;
				vint										firstFutureStep;
				vint										savedStep;
				bool										performingUndoRedo;

				void										PushStep(Ptr<IEditStep> step);
			public:
				GuiGeneralUndoRedoProcessor();
				~GuiGeneralUndoRedoProcessor();

				bool										CanUndo();
				bool										CanRedo();
				void										ClearUndoRedo();
				bool										GetModified();
				void										NotifyModificationSaved();
				bool										Undo();
				bool										Redo();
			};

			class GuiTextBoxUndoRedoProcessor : public GuiGeneralUndoRedoProcessor, public ICommonTextEditCallback
			{
			protected:
				class EditStep : public Object, public IEditStep
				{
				public:
					GuiTextBoxUndoRedoProcessor*			processor;
					TextEditNotifyStruct					arguments;
					
					void									Undo();
					void									Redo();
				};

				GuiTextBoxCommonInterface*					textBoxCommonInterface;
			public:
				GuiTextBoxUndoRedoProcessor(GuiTextBoxCommonInterface* _textBoxCommonInterface);
				~GuiTextBoxUndoRedoProcessor();

				void										Attach(elements::GuiColorizedTextElement* element, SpinLock& elementModifyLock)override;
				void										Detach()override;
				void										TextEditNotify(const TextEditNotifyStruct& arguments)override;
				void										TextCaretChanged(const TextCaretChangedStruct& arguments)override;
				void										TextEditFinished()override;
			};
		}
	}
}

#endif