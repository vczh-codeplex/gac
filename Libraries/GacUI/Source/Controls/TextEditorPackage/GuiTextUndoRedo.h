/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUITEXTUNDOREDO
#define VCZH_PRESENTATION_CONTROLS_GUITEXTUNDOREDO

#include "GuiTextElementOperator.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

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

			class GuiTextBoxUndoRedoProcessor : public GuiGeneralUndoRedoProcessor, public GuiTextElementOperator::ITextEditCallback
			{
			protected:
				class EditStep : public Object, public IEditStep
				{
				public:
					GuiTextBoxUndoRedoProcessor*			processor;
					TextPos									originalStart;
					TextPos									originalEnd;
					WString									originalText;
					TextPos									inputStart;
					TextPos									inputEnd;
					WString									inputText;
					
					void									Undo();
					void									Redo();
				};

				GuiTextElementOperator*						textElementOperator;
			public:
				GuiTextBoxUndoRedoProcessor(GuiTextElementOperator* _textElementOperator);
				~GuiTextBoxUndoRedoProcessor();

				void										Attach(elements::GuiColorizedTextElement* element, SpinLock& elementModifyLock);
				void										Detach();
				void										TextEditNotify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText);
			};
		}
	}
}

#endif