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
			class GuiTextBoxCommonInterface : public GuiTextElementOperator, public Description<GuiTextBoxCommonInterface>
			{
			protected:
				Ptr<GuiTextBoxColorizerBase>				colorizer;
				Ptr<GuiTextBoxUndoRedoProcessor>			undoRedoProcessor;

			public:
				GuiTextBoxCommonInterface();
				~GuiTextBoxCommonInterface();

				//================ colorizing

				/// <summary>Get the current colorizer.</summary>
				/// <returns>The current colorizer.</returns>
				Ptr<GuiTextBoxColorizerBase>				GetColorizer();
				/// <summary>Set the current colorizer.</summary>
				/// <param name="value">The current colorizer.</param>
				void										SetColorizer(Ptr<GuiTextBoxColorizerBase> value);

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
		}
	}
}

#endif